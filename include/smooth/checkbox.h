 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CHECKBOX_
#define _H_OBJSMOOTH_CHECKBOX_

#define SCheckBox SMOOTHCheckBox

class SMOOTHCheckBox;

#include "object.h"

class SMOOTHAPI SMOOTHCheckBox : public SMOOTHObject
{
	protected:
		SMOOTHBool		*variable;
		SMOOTHBool		 state;
	public:
					 SMOOTHCheckBox(SMOOTHString, SMOOTHPoint, SMOOTHSize, SMOOTHBool *, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHCheckBox();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_CHECKBOX;

#endif
