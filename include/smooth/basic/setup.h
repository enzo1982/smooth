 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SETUP_
#define _H_OBJSMOOTH_SETUP_

namespace smooth
{
	class Setup;
};

#include "../definitions.h"

namespace smooth
{
	class SMOOTHAPI Setup
	{
		private:
					 Setup();
					 Setup(const Setup &);
		public:
					 Setup(const Setup *);

			static Int	 BackgroundColor;
			static Int	 ClientColor;
			static Int	 ClientTextColor;
			static Int	 LightGrayColor;
			static Int	 DividerLightColor;
			static Int	 DividerDarkColor;
			static Int	 TextColor;
			static Int	 GrayTextColor;
			static Int	 ShadowColor;
			static Int	 GradientStartColor;
			static Int	 GradientEndColor;
			static Int	 GradientTextColor;
			static Int	 InactiveGradientStartColor;
			static Int	 InactiveGradientEndColor;
			static Int	 InactiveGradientTextColor;
			static Int	 TooltipColor;
			static Int	 TooltipTextColor;

			static Bool	 enableUnicode;
			static Bool	 rightToLeft;
			static Bool	 useIconv;

			static Float	 FontSize;
	};
};

#endif
