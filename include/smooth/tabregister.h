 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TABREGISTER_
#define _H_OBJSMOOTH_TABREGISTER_

#define STabRegister SMOOTHTabRegister

class SMOOTHTabRegister;

#include "object.h"
#include "container.h"

class SMOOTHAPI SMOOTHTabRegister : public SMOOTHObject, public SMOOTHContainer
{
	protected:
		SMOOTHArray<SMOOTHInt>	 textSize;
		SMOOTHArray<SMOOTHBool>	 sizeSet;

		SMOOTHVoid		 GetSize();
	public:
					 SMOOTHTabRegister(SMOOTHPoint, SMOOTHSize);
					~SMOOTHTabRegister();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 RegisterObject(SMOOTHObject *);
		SMOOTHInt		 UnregisterObject(SMOOTHObject *);
};

SMOOTHVAR SMOOTHInt OBJ_TABREGISTER;

#endif
