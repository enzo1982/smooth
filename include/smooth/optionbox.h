 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OPTIONBOX_
#define _H_OBJSMOOTH_OPTIONBOX_

#define SOptionBox SMOOTHOptionBox

class SMOOTHOptionBox;

#include "object.h"

class SMOOTHAPI SMOOTHOptionBox : public SMOOTHObject
{
	protected:
		SMOOTHInt		*variable;
		SMOOTHInt		 code;
		SMOOTHBool		 state;
	public:
					 SMOOTHOptionBox(SMOOTHString, SMOOTHPoint, SMOOTHSize, SMOOTHInt *, SMOOTHInt, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHOptionBox();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetText(SMOOTHString);
};

SMOOTHVAR SMOOTHInt OBJ_OPTIONBOX;

#endif
