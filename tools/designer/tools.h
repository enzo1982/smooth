 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DESIGNER_TOOLS_
#define _H_OBJSMOOTH_DESIGNER_TOOLS_

#include <smoothx.h>

const SMOOTHInt	 WINDOW		= 0;
const SMOOTHInt	 CLIENT		= 1;
const SMOOTHInt	 SCREEN		= 2;

SMOOTHInt MouseX(HWND, SMOOTHInt);
SMOOTHInt MouseY(HWND, SMOOTHInt);

#endif
