 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DATETIME_
#define _H_OBJSMOOTH_DATETIME_

namespace smooth
{
	class DateTime;
};

#include "definitions.h"

namespace smooth
{
	class SMOOTHAPI DateTime
	{
		private:
			Int	 days;
			Int	 mseconds;
		public:
				 DateTime();
				~DateTime();

			Bool	 SetYMD(Int, Int, Int);
			Bool	 SetHMS(Int, Int, Int);

			Int	 GetYear();
			Int	 GetMonth();
			Int	 GetDay();

			Bool	 SetYear(Int);
			Bool	 SetMonth(Int);
			Bool	 SetDay(Int);

			Int	 GetHour();
			Int	 GetMinute();
			Int	 GetSecond();
			Int	 GetMSecond();

			Bool	 SetHour(Int);
			Bool	 SetMinute(Int);
			Bool	 SetSecond(Int);
			Bool	 SetMSecond(Int);
	};
};

#endif
