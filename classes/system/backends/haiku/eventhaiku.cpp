 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>
#include <smooth/system/system.h>
#include <smooth/system/backends/haiku/eventhaiku.h>
#include <smooth/gui/window/backends/haiku/windowhaiku.h>
#include <smooth/templates/nonblocking.h>
#include <smooth/foreach.h>

#include <Application.h>
#include <Window.h>

S::System::EventBackend *CreateEventHaiku()
{
	return new S::System::EventHaiku();
}

S::Int	 eventHaikuTmp = S::System::EventBackend::SetBackend(&CreateEventHaiku);

S::Array<S::System::EventHaiku::Message *, S::Void *>	 S::System::EventHaiku::messages;

S::System::EventHaiku::EventHaiku()
{
	type = EVENT_HAIKU;

	messages.EnableLocking();
}

S::System::EventHaiku::~EventHaiku()
{
}

S::Void S::System::EventHaiku::EnqueueMessage(Void *window, const BMessage &currentMessage, Int messageID, Int param1, Int param2)
{
	messages.LockForWrite();

	/* Process only the most recent B_MOUSE_MOVED and B_WINDOW_RESIZED message.
	 */
	if (messageID == B_MOUSE_MOVED || messageID == B_WINDOW_RESIZED)
	{
		foreachreverse (Message *message, messages)
		{
			if (message->window    == window &&
			    message->messageID == messageID) { messages.RemoveNth(foreachindex); delete message; }
		}
	}

	/* Add new message to queue.
	 */
	Message	*message = new Message();

	message->window		= window;
	message->currentMessage = currentMessage;
	message->messageID	= messageID;
	message->param1		= param1;
	message->param2		= param2;

	messages.Add(message);

	messages.Unlock();
}

S::Void S::System::EventHaiku::ProcessMessage(Void *window, const BMessage &currentMessage, Int messageID, Int param1, Int param2)
{
	EnqueueMessage(window, currentMessage, messageID, param1, param2);

	while (messages.Length() > 0) System::System::Sleep(0);
}

class HaikuApplication : public BApplication
{
	public:
		HaikuApplication() : BApplication("application/x-vnd.smooth-haiku")
		{
		}
};

static HaikuApplication	 app;

S::Int S::System::EventHaiku::ProcessNextEvent()
{
	/* Emulate a timeout of ~100ms by trying to find a message
	 * 10 times while sleeping for 10ms between trying.
	 */
	for (Int i = 0; i < 10; i++)
	{
		/* Check for a message and process it.
		 */
		messages.LockForWrite();

		if (messages.Length() > 0)
		{
			/* Get first message.
			 */
			Message	*message = messages.GetFirst();

			messages.RemoveNth(0);
			messages.Unlock();

			/* Find window and process message.
			 */
			GUI::WindowHaiku	*window = GUI::WindowHaiku::GetWindowBackend((BWindow *) message->window);

			if (window != NIL) window->ProcessSystemMessages(message->messageID, message->param1, message->param2, message->currentMessage);

			delete message;

			break;
		}

		messages.Unlock();

		/* Allow timeouts to be processed.
		 */
		EventProcessor::allowTimerInterrupts.Call();

		/* Now sleep for 10ms.
		 */
		System::System::Sleep(10);

		/* Prevent timeouts from being processed.
		 */
		EventProcessor::denyTimerInterrupts.Call();
	}

	return Success();
}
