 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DESIGNER_STATUS_
#define _H_OBJSMOOTH_DESIGNER_STATUS_

#include <smooth.h>
#include "edit_component.h"

const S::Int STATUS_EVENT_REPORT_NAME		= 0;
const S::Int STATUS_EVENT_REPORT_MOUSEPOSITION	= 1;
const S::Int STATUS_EVENT_REPORT_WINDOWPOSITION	= 2;
const S::Int STATUS_EVENT_REPORT_WINDOWSIZE	= 3;
const S::Int STATUS_EVENT_REPORT_QUIT		= 4;

typedef struct
{
	S::Int			 event;
	Designer_EditComponent	*dlg;
	S::String		 currdlgname;
	S::Int			 mousex;
	S::Int			 mousey;
	S::Int			 wndposx;
	S::Int			 wndposy;
	S::Int			 wndsizex;
	S::Int			 wndsizey;
	S::Int			 ticks;
} Designer_Status;

#endif
