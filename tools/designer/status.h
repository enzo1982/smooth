 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DESIGNER_STATUS_
#define _H_OBJSMOOTH_DESIGNER_STATUS_

#include <smoothx.h>
#include "edit_component.h"

const SMOOTHInt STATUS_EVENT_REPORT_NAME		= 0;
const SMOOTHInt STATUS_EVENT_REPORT_MOUSEPOSITION	= 1;
const SMOOTHInt STATUS_EVENT_REPORT_WINDOWPOSITION	= 2;
const SMOOTHInt STATUS_EVENT_REPORT_WINDOWSIZE		= 3;
const SMOOTHInt STATUS_EVENT_REPORT_QUIT		= 4;

typedef struct
{
	SMOOTHInt		 event;
	Designer_EditComponent	*dlg;
	SMOOTHString		 currdlgname;
	SMOOTHInt		 mousex;
	SMOOTHInt		 mousey;
	SMOOTHInt		 wndposx;
	SMOOTHInt		 wndposy;
	SMOOTHInt		 wndsizex;
	SMOOTHInt		 wndsizey;
	SMOOTHInt		 ticks;
} Designer_Status;

#endif
