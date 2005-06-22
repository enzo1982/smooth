 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MULTIEDIT_
#define _H_OBJSMOOTH_MULTIEDIT_

namespace smooth
{
	namespace GUI
	{
		class MultiEdit;
		class Scrollbar;
	};

	namespace System
	{
		class Timer;
	};
};

#include "../widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI MultiEdit : public Widget
		{
			private:
				System::Timer		*timer;
			slots:
				Void			 TimerProc();

				Int			 GetNOfLines();
				Int			 GetNOfInvisibleLines();

				String			 GetVisibleText();
			protected:
				Int			 promptPos;
				Bool			 promptVisible;
				Int			 markStart;
				Int			 oMarkStart;
				Int			 markEnd;
				Bool			 marking;
				Int			 maxSize;
				Int			 invisibleChars;
				Int			 scrollbarPos;

				Scrollbar		*scrollbar;

				Void			 SetCursor(Int);
				Void			 RemoveCursor();

				Void			 MarkText(Int, Int);
				Void			 DeleteSelectedText();
				Void			 InsertText(String);
				Int			 ModifyText(String);
			slots:
				Void			 ScrollbarProc();
			public:
				static const Int	 classID;

							 MultiEdit(String, Point, Size, Int = 0);
							~MultiEdit();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Int			 Deactivate();

				Int			 SetText(const String &);

				Int			 GetCursorPos();

				Int			 MarkAll();
		};
	};
};

#endif
