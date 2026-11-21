 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/mcp/server.h>
#include <smooth/misc/mcp/tools.h>
#include <smooth/threads/thread.h>
#include <smooth/threads/mutex.h>
#include <smooth/system/event.h>
#include <smooth/system/system.h>
#include <smooth/gui/window/window.h>
#include <smooth/version.h>
#include <smooth/init.h>

#include <stdio.h>
#include <string.h>

#ifdef __WIN32__
#	include <io.h>
#	include <fcntl.h>
#endif

S::Int	 addMCPServerFreeTmp = S::AddFreeFunction(&S::MCP::Server::Free);

/* A function to run on the GUI thread. Freed by the waiting
 * thread or, if that gave up waiting, by the GUI thread.
 */
class S::MCP::Server::Job
{
	public:
		GUIFunction	 function;

		JSON		 arguments;
		JSON		 result;
		Bool		 success;

		volatile Bool	 done;
		Bool		 abandoned;

				 Job(GUIFunction iFunction, const JSON &iArguments) : function(iFunction), arguments(iArguments), success(False), done(False), abandoned(False) { }
};

S::String			 S::MCP::Server::mode;

S::Threads::Thread		*S::MCP::Server::thread = NIL;
S::Threads::Mutex		*S::MCP::Server::mutex	= NIL;

S::Array<S::MCP::Server::Job *, S::Void *>	 S::MCP::Server::jobs;

namespace
{
	/* Close all windows when the client disconnects.
	 */
	S::Bool CloseWindows(const S::MCP::JSON &, S::MCP::JSON &)
	{
		using namespace smooth;
		using namespace smooth::GUI;

		Array<Window *, Void *>	 windows;

		for (Int i = 0; i < Window::GetNOfWindows(); i++) windows.Add(Window::GetNthWindow(i));

		for (Int i = windows.Length() - 1; i >= 0; i--) windows.GetNth(i)->Close();

		return S::True;
	}
};

S::Void S::MCP::Server::SetMode(const String &nMode)
{
	mode = nMode;
}

S::Int S::MCP::Server::Start()
{
	if (mode == NIL || thread != NIL) return Success();

	if (mode != "stdio")
	{
		fprintf(stderr, "Warning: MCP mode '%s' is not supported. Use --mcp:stdio.\n", mode.ConvertTo("UTF-8"));

		return Error();
	}

	/* Use binary mode, so line endings are not translated.
	 */
#ifdef __WIN32__
	_setmode(_fileno(stdin), _O_BINARY);
	_setmode(_fileno(stdout), _O_BINARY);
#endif

	mutex = new Threads::Mutex();

	System::EventProcessor::onProcessEvents.Connect(&Server::ProcessJobs);

	thread = new Threads::Thread();
	thread->threadMain.Connect(&Server::ReadMessages);
	thread->Start();

	return Success();
}

S::Int S::MCP::Server::Free()
{
	if (thread == NIL) return Success();

	System::EventProcessor::onProcessEvents.Disconnect(&Server::ProcessJobs);

	/* Stop reading messages. Jobs are not freed, as
	 * the waiting thread may still access them.
	 */
	delete thread;
	delete mutex;

	thread = NIL;
	mutex  = NIL;

	return Success();
}

S::Int S::MCP::Server::ReadMessages(Threads::Thread *)
{
	/* Read newline delimited JSON-RPC messages.
	 */
	Int	 size	  = 0;
	Int	 capacity = 4096;
	char	*line	  = new char [capacity];

	while (True)
	{
		int	 c = getc(stdin);

		if (c == EOF) break;

		if (c != '\n')
		{
			if (size + 1 >= capacity)
			{
				char	*nLine = new char [capacity * 2];

				memcpy(nLine, line, size);

				delete [] line;

				line	  = nLine;
				capacity *= 2;
			}

			line[size++] = c;

			continue;
		}

		if (size > 0 && line[size - 1] == '\r') size--;

		line[size] = 0;

		/* Skip a UTF-8 byte order mark some clients send.
		 */
		const char	*message = line;

		if (strncmp(message, "\xEF\xBB\xBF", 3) == 0) message += 3;

		if (*message != 0) HandleMessage(message);

		size = 0;
	}

	delete [] line;

	/* The client disconnected, so close the application.
	 */
	JSON	 result;
	Bool	 success = False;

	RunOnGUIThread(&CloseWindows, JSON(), result, success, 0);

	return Success();
}

S::Void S::MCP::Server::HandleMessage(const char *text)
{
	JSON	 message;

	if (!JSON::Parse(text, message) || !message.IsObject())
	{
		SendError(JSON(), -32700, "Parse error");

		return;
	}

	const JSON	&id	= message.Get("id");
	const String	&method = message.Get("method").GetString();
	const JSON	&params = message.Get("params");

	/* Ignore notifications and responses.
	 */
	if (!message.Has("id") || !message.Has("method")) return;

	JSON	 result = JSON::NewObject();

	if (method == "initialize")
	{
		/* Use the client's protocol version if supported.
		 */
		const String	&requested = params.Get("protocolVersion").GetString();
		String		 version   = "2025-06-18";

		if (requested == "2025-03-26" || requested == "2024-11-05") version = requested;

		result.Set("protocolVersion", version);
		result.Set("capabilities", JSON::NewObject().Set("tools", JSON::NewObject()));
		result.Set("serverInfo", JSON::NewObject().Set("name", "smooth").Set("version", SMOOTH_VERSION));
		result.Set("instructions", Tools::GetInstructions());
	}
	else if (method == "ping")
	{
	}
	else if (method == "tools/list")
	{
		result.Set("tools", Tools::GetToolList());
	}
	else if (method == "tools/call")
	{
		if (!Tools::HasTool(params.Get("name").GetString())) { SendError(id, -32602, String("Unknown tool: ").Append(params.Get("name").GetString())); return; }

		result = Tools::CallTool(params.Get("name").GetString(), params.Get("arguments"));
	}
	else
	{
		SendError(id, -32601, String("Method not found: ").Append(method));

		return;
	}

	SendMessage(JSON::NewObject().Set("jsonrpc", "2.0").Set("id", id).Set("result", result));
}

S::Void S::MCP::Server::SendMessage(const JSON &message)
{
	char	*text = message.Serialize();

	fputs(text, stdout);
	fputc('\n', stdout);
	fflush(stdout);

	delete [] text;
}

S::Void S::MCP::Server::SendError(const JSON &id, Int code, const String &text)
{
	SendMessage(JSON::NewObject().Set("jsonrpc", "2.0").Set("id", id).Set("error", JSON::NewObject().Set("code", code).Set("message", text)));
}

S::Void S::MCP::Server::ProcessJobs()
{
	/* Run queued jobs. Jobs are removed from the queue before running,
	 * as they may process events themselves, e.g. for modal dialogs.
	 */
	while (True)
	{
		mutex->Lock();

		if (jobs.Length() == 0) { mutex->Release(); break; }

		Job	*job = jobs.GetFirst();

		jobs.RemoveNth(0);

		mutex->Release();

		job->success = job->function(job->arguments, job->result);

		mutex->Lock();

		job->done = True;

		Bool	 abandoned = job->abandoned;

		mutex->Release();

		if (abandoned) delete job;
	}
}

S::Bool S::MCP::Server::RunOnGUIThread(GUIFunction function, const JSON &arguments, JSON &result, Bool &success, Int timeout)
{
	Job	*job = new Job(function, arguments);

	mutex->Lock();
	jobs.Add(job);
	mutex->Release();

	/* Wait for the job to finish.
	 */
	for (Int waited = 0; !job->done && waited < timeout; waited += 5) System::System::Sleep(5);

	mutex->Lock();

	Bool	 done = job->done;

	if (!done) job->abandoned = True;

	mutex->Release();

	if (!done) return False;

	result	= job->result;
	success	= job->success;

	delete job;

	return True;
}
