 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/basic/setup.h>
#include <smooth/graphics/color.h>

/* Init color constant with default values.
 */
S::GUI::Color	 S::Setup::BackgroundColor(240, 240, 240);
S::GUI::Color	 S::Setup::ClientColor(255, 255, 255);
S::GUI::Color	 S::Setup::ClientTextColor(0, 0, 0);
S::GUI::Color	 S::Setup::LightGrayColor(248, 248, 248);
S::GUI::Color	 S::Setup::DividerLightColor(237, 236, 232);
S::GUI::Color	 S::Setup::DividerDarkColor(128, 128, 128);
S::GUI::Color	 S::Setup::TextColor(0, 0, 0);
S::GUI::Color	 S::Setup::InactiveTextColor(128, 128, 128);
S::GUI::Color	 S::Setup::HighlightColor(55, 115, 215);
S::GUI::Color	 S::Setup::HighlightTextColor(255, 255, 255);
S::GUI::Color	 S::Setup::GradientStartColor(10, 36, 106);
S::GUI::Color	 S::Setup::GradientEndColor(166, 202, 240);
S::GUI::Color	 S::Setup::GradientTextColor(255, 255, 255);
S::GUI::Color	 S::Setup::InactiveGradientStartColor(128, 128, 128);
S::GUI::Color	 S::Setup::InactiveGradientEndColor(192, 192, 192);
S::GUI::Color	 S::Setup::InactiveGradientTextColor(212, 208, 200);
S::GUI::Color	 S::Setup::TooltipColor(255, 255, 225);
S::GUI::Color	 S::Setup::TooltipTextColor(0, 0, 0);

#ifdef __WIN32__
	S::Bool	 S::Setup::useIconv    = S::False;
#else
	S::Bool	 S::Setup::useIconv    = S::True;
#endif

S::Float	 S::Setup::FontSize    = 1.00;
S::Bool		 S::Setup::rightToLeft = S::False;

S::Int		 S::Setup::HoverTime   = 400;
S::Int		 S::Setup::HoverWidth  = 4;
S::Int		 S::Setup::HoverHeight = 4;

S::Setup::Setup()
{
}

S::Setup::Setup(const Setup &)
{
}

S::Setup::Setup(const Setup *)
{
}
