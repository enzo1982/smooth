 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ARROWS_
#define _H_OBJSMOOTH_ARROWS_

namespace smooth
{
	namespace GUI
	{
		class Arrows;
	};

	class Timer;
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Arrows : public Widget
		{
			private:
				Timer			*timer;
				Bool			 timerActive;
				Int			 timerCount;

				Void			 TimerProc();
			protected:
				Bool			 arrow1Clicked;
				Bool			 arrow2Clicked;
				Bool			 arrow1Checked;
				Bool			 arrow2Checked;

				Int			*variable;

				Int			 startValue;
				Int 			 endValue;
			public:
				static const Int	 classID;

							 Arrows(Point, Size, Int, Int *, Int, Int);
							~Arrows();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Int			 SetRange(Int, Int);

				Int			 SetValue(Int);
				Int			 GetValue();
		};
	};
};

#endif
