 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TOOLTIP_
#define _H_OBJSMOOTH_TOOLTIP_

namespace smooth
{
	class Tooltip;
	class Timer;
	class ToolWindow;
};

#include "object.h"

namespace smooth
{
	class SMOOTHAPI Tooltip : public Object
	{
		private:
			ToolWindow	*toolWindow;
			Timer		*timer;

			Void		 TimerProc();
		protected:
			Int		 timeOut;
		public:
					 Tooltip();
					~Tooltip();

			Int		 Process(Int, Int, Int);

			Int		 Show();
			Int		 Hide();

			Int		 SetTimeout(Int);
	};

	SMOOTHVAR Int OBJ_TOOLTIP;
};

#endif
