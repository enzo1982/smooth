 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/object.h>
#include <smooth/objectproperties.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/toolkit.h>

S::ObjectProperties::ObjectProperties()
{
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

	checked		= False;
	clicked		= False;
}
