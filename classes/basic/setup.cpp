 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/setup.h>

S::Int S::Setup::BackgroundColor = RGB(192, 192, 192);
S::Int S::Setup::ClientColor = RGB(255, 255, 255);
S::Int S::Setup::ClientTextColor = RGB(0, 0, 0);
S::Int S::Setup::LightGrayColor;
S::Int S::Setup::DividerLightColor;
S::Int S::Setup::DividerDarkColor;
S::Int S::Setup::TextColor = RGB(0, 0, 0);
S::Int S::Setup::GrayTextColor = RGB(128, 128, 128);
S::Int S::Setup::ShadowColor;
S::Int S::Setup::GradientStartColor;
S::Int S::Setup::GradientEndColor;
S::Int S::Setup::GradientTextColor = RGB(255, 255, 255);
S::Int S::Setup::InactiveGradientStartColor;
S::Int S::Setup::InactiveGradientEndColor;
S::Int S::Setup::InactiveGradientTextColor;
S::Int S::Setup::TooltipColor;
S::Int S::Setup::TooltipTextColor;

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
