 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TEXT_
#define _H_OBJSMOOTH_TEXT_

#define SText SMOOTHText

class SMOOTHText;

#include "object.h"

class SMOOTHAPI SMOOTHText : public SMOOTHObject
{
	public:
					 SMOOTHText(SMOOTHString, SMOOTHPoint);
					~SMOOTHText();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_TEXT;

#endif
