 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_EVENTHAIKU
#define H_OBJSMOOTH_EVENTHAIKU

namespace smooth
{
	namespace System
	{
		class EventHaiku;
	};
};

#include "../eventbackend.h"

#include <Application.h>

namespace smooth
{
	namespace System
	{
		const Short	 EVENT_HAIKU = 4;

		class EventHaiku : public EventBackend
		{
			struct Message
			{
				Void		*window;

				BMessage	 currentMessage;

				Int		 messageID;

				Int		 param1;
				Int		 param2;
			};

			private:
				static Int			 nested;

				static Array<Message *, Void *>	 messages;
			public:
				static Void			 EnqueueMessage(Void *, const BMessage &, Int, Int, Int);
				static Void			 ProcessMessage(Void *, const BMessage &, Int, Int, Int);

								 EventHaiku();
								~EventHaiku();

				Bool				 ProcessNextEvent();
		};
	};
};

#endif
