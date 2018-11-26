 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_WINDOWHAIKU
#define H_OBJSMOOTH_WINDOWHAIKU

#include <Window.h>

namespace smooth
{
	namespace System
	{
		class Timer;
	};

	namespace GUI
	{
		class WindowHaiku;
	};
};

#include "../windowbackend.h"
#include "../../../../graphics/forms/rect.h"
#include "../../../../input/keyboard.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 WINDOW_HAIKU	= 4;

		class WindowHaiku : public WindowBackend
		{
			private:
				static Array<WindowHaiku *, Void *>	 windowBackends;

				Bool					 IsModalWindowActive();
			protected:
				Int					 id;

				BWindow					*wnd;
				BView					*view;

				const BMessage				*dropMessage;

				Float					 fontSize;

				Size					 minSize;
				Size					 maxSize;

				Int					 flags;

				Input::Keyboard::Key			 ConvertKey(Int, const BMessage &);
			public:
									 WindowHaiku(Void * = NIL);
									~WindowHaiku();

				Void					*GetSystemWindow() const;

				static WindowHaiku			*GetWindowBackend(BWindow *);

				Int					 ProcessSystemMessages(Int, Int, Int, const BMessage &);

				Int					 Open(const String &, const Point &, const Size &, Int);
				Int					 Close();

				Int					 RequestClose();

				Int					 SetTitle(const String &);

				const Array<String>			&GetDroppedFiles() const;

				Int					 SetMinimumSize(const Size &);
				Int					 SetMaximumSize(const Size &);

				Int					 Show();
				Int					 Hide();

				Int					 SetMetrics(const Point &, const Size &);
		};
	};
};

#endif
