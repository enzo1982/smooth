 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SCROLLBAR_
#define _H_OBJSMOOTH_SCROLLBAR_

namespace smooth
{
	namespace GUI
	{
		class Scrollbar;
	};

	namespace System
	{
		class Timer;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Scrollbar : public Widget
		{
			private:
				System::Timer		*timer;
				Bool			 timerActive;
				Int			 timerCount;

				Void			 TimerProc();
			protected:
				Bool			 button1Clicked;
				Bool			 button2Clicked;
				Bool			 button3Clicked;
				Bool			 button1Checked;
				Bool			 button2Checked;
				Bool			 button3Checked;

				Int			*variable;

				Int			 startValue;
				Int 			 endValue;

				Int			 mouseBias;
			public:
				static const Int	 classID;

							 Scrollbar(Point, Size, Int, Int *, Int, Int);
							~Scrollbar();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Int			 SetRange(Int, Int);
		};
	};
};

#endif
