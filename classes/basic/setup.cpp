 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/basic/setup.h>
#include <smooth/graphics/color.h>

S::GUI::Color S::Setup::BackgroundColor(192, 192, 192);
S::GUI::Color S::Setup::ClientColor(255, 255, 255);
S::GUI::Color S::Setup::ClientTextColor(0, 0, 0);
S::GUI::Color S::Setup::LightGrayColor;
S::GUI::Color S::Setup::DividerLightColor;
S::GUI::Color S::Setup::DividerDarkColor;
S::GUI::Color S::Setup::TextColor(0, 0, 0);
S::GUI::Color S::Setup::GrayTextColor(128, 128, 128);
S::GUI::Color S::Setup::ShadowColor;
S::GUI::Color S::Setup::GradientStartColor;
S::GUI::Color S::Setup::GradientEndColor;
S::GUI::Color S::Setup::GradientTextColor(255, 255, 255);
S::GUI::Color S::Setup::InactiveGradientStartColor;
S::GUI::Color S::Setup::InactiveGradientEndColor;
S::GUI::Color S::Setup::InactiveGradientTextColor;
S::GUI::Color S::Setup::TooltipColor;
S::GUI::Color S::Setup::TooltipTextColor;

S::Bool S::Setup::enableUnicode = S::False;
S::Bool S::Setup::rightToLeft = S::False;
S::Bool S::Setup::useIconv = S::False;

S::Float S::Setup::FontSize = 1.00;

S::Setup::Setup()
{
}

S::Setup::Setup(const Setup &)
{
}

S::Setup::Setup(const Setup *)
{
}
