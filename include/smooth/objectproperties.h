 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECTPROPERTIES_
#define _H_OBJSMOOTH_OBJECTPROPERTIES_

#define SObjectProperties SMOOTHObjectProperties

class SMOOTHObjectProperties;

#include "definitions.h"

class SMOOTHAPI SMOOTHObjectProperties
{
	public:
		SMOOTHPoint	 pos;
		SMOOTHSize	 size;

		SMOOTHInt	 orientation;

		SMOOTHString	 text;
		SMOOTHString	 tooltip;

		SMOOTHSize	 textSize;
		SMOOTHSize	 tooltipSize;

		SMOOTHString	 font;
		SMOOTHInt	 fontSize;
		SMOOTHInt	 fontWeight;
		SMOOTHInt	 fontColor;

		SMOOTHBool	 clicked;
		SMOOTHBool	 checked;

		SMOOTHProcMember;
		SMOOTHVoid	*procParam;

				 SMOOTHObjectProperties();
};

#endif
