 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DIVISIONBAR_
#define _H_OBJSMOOTH_DIVISIONBAR_

#define SDivisionbar SMOOTHDivisionbar

class SMOOTHDivisionbar;
class SMOOTHClient;
class SMOOTHMDIClient;

#include "object.h"

class SMOOTHAPI SMOOTHDivisionbar : public SMOOTHObject
{
	friend class SMOOTHClient;
	friend class SMOOTHMDIClient;
	protected:
		SMOOTHInt		 orientation;
	public:
					 SMOOTHDivisionbar(SMOOTHInt, SMOOTHInt);
					~SMOOTHDivisionbar();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_DIVISIONBAR;

#endif
