 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_OBJECTPROPERTIES_
#define __OBJSMOOTH_OBJECTPROPERTIES_

#include <smooth/object.h>
#include <smooth/objectproperties.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/toolkit.h>

SMOOTHObjectProperties::SMOOTHObjectProperties()
{
	HDC	 dc = GetContext(0);

	pos.x		= 0;
	pos.y		= 0;
	size.cx		= 100;
	size.cy		= 100;

	orientation	= OR_UPPERLEFT;

	text		= NIL;
	tooltip		= NIL;

	textSize.cx	= 0;
	textSize.cy	= 0;
	tooltipSize.cx	= 0;
	tooltipSize.cy	= 0;

	checked		= SMOOTH::False;
	clicked		= SMOOTH::False;

	font		= I18N_DEFAULTFONT;
	fontColor	= SMOOTH::Setup::TextColor;

#ifdef __WIN32__
	fontSize	= -MulDiv(I18N_DEFAULTFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72);
	fontWeight	= FW_NORMAL;
#endif

	FreeContext(0, dc);
}

#endif
