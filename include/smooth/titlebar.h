 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TITLEBAR_
#define _H_OBJSMOOTH_TITLEBAR_

#define STitlebar SMOOTHTitlebar

class SMOOTHTitlebar;
class SMOOTHMenubar;
class SMOOTHWindow;

#include "object.h"

class SMOOTHAPI SMOOTHTitlebar : public SMOOTHObject
{
	friend class SMOOTHMenubar;
	friend class SMOOTHWindow;
	private:
		SMOOTHBool		 minchk;
		SMOOTHBool		 minclk;
		SMOOTHBool		 maxchk;
		SMOOTHBool		 maxclk;
		SMOOTHBool		 closechk;
		SMOOTHBool		 closeclk;
		SMOOTHRect		 nonmaxrect;
		SMOOTHBool		 min;
		SMOOTHBool		 max;
		SMOOTHBool		 close;
		SMOOTHInt		 origwndstyle;
	public:
					 SMOOTHTitlebar(SMOOTHBool, SMOOTHBool, SMOOTHBool);
					~SMOOTHTitlebar();
		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_TITLEBAR;

#endif
