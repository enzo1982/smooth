 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/net/protocols/http.h>
#include <smooth/io/drivers/driver_socket.h>

#include <time.h>

S::Net::Protocols::Protocol *CreateProtocolHTTP(const S::String &iURL)
{
	return new S::Net::Protocols::HTTP(iURL);
}

S::Int	 protocolHTTPTmp = S::Net::Protocols::Protocol::AddProtocol(&CreateProtocolHTTP, "http://");

S::Net::Protocols::HTTP::HTTP(const String &iURL) : Protocol(iURL)
{
	mode = HTTP_METHOD_GET;
}

S::Net::Protocols::HTTP::~HTTP()
{
}

S::Int S::Net::Protocols::HTTP::SetParameter(const String &key, const String &value)
{
	Parameter	 parameter;

	parameter.key	= key;
	parameter.value	= value;

	parameters.Add(parameter);

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetParameterFile(const String &key, const String &fileName)
{
	if (!S::File(fileName).Exists()) return Error();

	Parameter	 parameter;

	parameter.key	= key;
	parameter.value	= fileName;

	parameter.isFile= True;

	parameters.Add(parameter);

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetMode(Int nMode)
{
	mode = nMode;

	return Success();
}

S::Int S::Net::Protocols::HTTP::DownloadToFile(const String &fileName)
{
	Bool	 error = False;

	Int	 i, j;
	String	 server;
	String	 path;
	Int	 port = 80;

	for (i = 7; i < url.Length(); i++)
	{
		if (url[i] == '/' || url[i] == ':') break;

		server[i - 7] = url[i];
	}

	if (url[i] == ':')
	{
		String	 portString;

		for (i = i + 1; i < url.Length(); i++)
		{
			if (url[i] == '/') break;

			portString[i - server.Length() - 8] = url[i];
		}

		port = portString.ToInt();
	}

	for (j = i; j < url.Length(); j++)
	{
		path[j - i] = url[j];
	}

	IO::Driver	*socket	= new IO::DriverSocket(server, port);
	IO::InStream	*in	= new IO::InStream(IO::STREAM_DRIVER, socket);
	IO::OutStream	*out	= new IO::OutStream(IO::STREAM_STREAM, in);

	downloadProgress.Emit(0);
	downloadSpeed.Emit("");

	if (in->GetLastError() == IO::IO_ERROR_OK)
	{
		Buffer<UnsignedByte>	&buffer = ComposeHTTPRequest(server, path);

		uploadProgress.Emit(0);
		uploadSpeed.Emit("");

		Int	 startTicks	= clock();
		Int	 percent	= 0;

		for (Int i = 0; i < buffer.Size(); i += 1024)
		{
			if (!out->OutputData(((UnsignedByte *) buffer) + i, Math::Min(1024, buffer.Size() - i)))
			{
				error = True;

				break;
			}

			if (Math::Round(1000.0 * i / buffer.Size()) != percent)
			{
				percent = Math::Round(1000.0 * i / buffer.Size());

				uploadProgress.Emit(percent);
				uploadSpeed.Emit(String::FromInt(Math::Round((i / 1024) / (Float(clock() - startTicks) / 1000))).Append(" kB/s"));
			}
		}

		uploadProgress.Emit(1000);

		if (!error)
		{
			Int	 bytes = 0;
			String	 encoding;
			String	 str;

			do
			{
				str = in->InputLine();

				if (str.StartsWith("Transfer-Encoding: "))	encoding = str.Tail(str.Length() - 19);
				else if (str.StartsWith("Content-Length: "))	bytes = str.Tail(str.Length() - 16).ToInt();
			}
			while (str != NIL);

			while (true)
			{
				if (encoding == "chunked")
				{
					str = in->InputLine();
					bytes = DecodeHexNumber(str);

					if (bytes == 0) break;
				}

				IO::OutStream	*fOut		= new IO::OutStream(IO::STREAM_FILE, fileName, IO::OS_OVERWRITE);
				UnsignedByte	*buffer		= new UnsignedByte [1024];
				Int		 startTicks	= clock();
				Int		 percent	= 0;

				for (Int i = 0; i < bytes; i += 1024)
				{
					fOut->OutputData(in->InputData((Void *) buffer, Math::Min(1024, bytes - i)), Math::Min(1024, bytes - i));

					if (Math::Round(1000.0 * i / bytes) != percent)
					{
						percent = Math::Round(1000.0 * i / bytes);

						downloadProgress.Emit(percent);
						downloadSpeed.Emit(String::FromInt(Math::Round((i / 1024) / (Float(clock() - startTicks) / 1000))).Append(" kB/s"));
					}
				}

				delete [] buffer;
				delete fOut;

				if (encoding != "chunked") break;
			}
		}
	}
	else
	{
		error = True;
	}

	downloadProgress.Emit(1000);

	delete in;
	delete out;
	delete socket;

	if (!error)	return Success();
	else		return Error();
}

S::Buffer<S::UnsignedByte> &S::Net::Protocols::HTTP::ComposeHTTPRequest(const String &server, const String &path)
{
	Bool	 haveFiles = False;

	for (Int i = 0; i < parameters.GetNOfEntries(); i++)
	{
		if (parameters.GetNth(i).isFile) { haveFiles = True; break; }
	}

	if (mode == HTTP_METHOD_GET && !haveFiles)
	{
		String	 str = String("GET ").Append(path);

		for (Int i = 0; i < parameters.GetNOfEntries(); i++)
		{
			Parameter	 parameter = parameters.GetNth(i);

			if (i == 0) str.Append("?");
			else	    str.Append("&");

			str.Append(parameter.key).Append("=").Append(parameter.value);
		}

		str.Append(" HTTP/1.1\n").
		    Append("Host: ").Append(server).Append("\n").
		    Append("\n");

		requestBuffer.Resize(str.Length());

		for (Int i = 0; i < str.Length(); i++) requestBuffer[i] = str[i];
	}
	else if (mode == HTTP_METHOD_POST || haveFiles)
	{
		String		 contentType;
		IO::OutStream	*out = new IO::OutStream(IO::STREAM_FILE, "httprequest.temp", IO::OS_OVERWRITE);

		if (!haveFiles)
		{
			contentType = "application/x-www-form-urlencoded";

			for (Int i = 0; i < parameters.GetNOfEntries(); i++)
			{
				Parameter	 parameter = parameters.GetNth(i);

				if (i > 0) out->OutputString("&");

				out->OutputString(String(parameter.key).Append("=").Append(parameter.value));
			}
		}
		else
		{
			String	 separator = "THIS_STRING_SEPARATES";

			contentType = String("multipart/form-data; boundary=").Append(separator);

			out->OutputString(String("--").Append(separator).Append("\n"));
			out->OutputString("Content-Disposition: form-data; name=\"MAX_FILE_SIZE\"\n\n");
			out->OutputString("1000000\n");

			for (Int i = 0; i < parameters.GetNOfEntries(); i++)
			{
				out->OutputString(String("--").Append(separator).Append("\n"));

				Parameter	 parameter = parameters.GetNth(i);

				if (!parameter.isFile)
				{
					out->OutputString(String("Content-Disposition: form-data; name=\"").Append(parameter.key).Append("\"\n\n"));
					out->OutputString(String(parameter.value).Append("\n"));
				}
				else
				{
					out->OutputString(String("Content-Disposition: form-data; name=\"").Append(parameter.key).Append("\"; filename=\"").Append(parameter.value).Append("\"\n\n"));

					IO::InStream	*in = new IO::InStream(IO::STREAM_FILE, parameter.value, IO::IS_READONLY);

					for (Int j = 0; j < in->Size(); j++) out->OutputNumber(in->InputNumber(1), 1);

					delete in;

					out->OutputString("\n");
				}
			}

			out->OutputString(String("--").Append(separator).Append("--"));
		}

		delete out;

		IO::InStream	*in = new IO::InStream(IO::STREAM_FILE, "httprequest.temp", IO::IS_READONLY);

		String	 str = String("POST ").Append(path).Append(" HTTP/1.1\n").
			       Append("Host: ").Append(server).Append("\n").
			       Append("Content-Length: ").Append(String::FromInt(in->Size())).Append("\n").
			       Append("Content-Type: ").Append(contentType).Append("\n").
			       Append("\n");

		requestBuffer.Resize(str.Length() + in->Size());

		for (Int i = 0; i < str.Length(); i++) requestBuffer[i] = str[i];
		for (Int i = 0; i < in->Size(); i++) requestBuffer[str.Length() + i] = in->InputNumber(1);

		delete in;

		S::File("httprequest.temp").Delete();
	}

	return requestBuffer;
}

S::Int S::Net::Protocols::HTTP::DecodeHexNumber(const String &string)
{
	Int	 value = 0;
	Int	 length = 0;

	for (Int i = 0; i < string.Length(); i++)
	{
		if	((string[i] >= '0' && string[i] <= '9') ||
			 (string[i] >= 'a' && string[i] <= 'f')) length++;
		else						 break;
	}

	for (Int i = 0; i < length; i++)
	{
		if (string[i] >= '0' && string[i] <= '9')	value += ((string[i] - '0') << ((length - i - 1) * 4));
		else if (string[i] >= 'a' && string[i] <= 'f')	value += ((string[i] - 'a' + 10) << ((length - i - 1) * 4));
	}

	return value;
}
