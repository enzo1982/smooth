 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CONTAINERTYPE_
#define _H_OBJSMOOTH_CONTAINERTYPE_

#define SContainerType SMOOTHContainerType

class SMOOTHContainer;
class SMOOTHContainerType;

#include "definitions.h"
#include "array.h"

class SMOOTHAPI SMOOTHContainerType
{
	friend class SMOOTHContainer;
	private:
		SMOOTHInt		 type;
		SMOOTHContainer		*container;
	public:
					 SMOOTHContainerType();
					 SMOOTHContainerType(const SMOOTHContainerType &);

		operator		 SMOOTHInt();
		SMOOTHInt operator	 =(SMOOTHInt);

		SMOOTHBool operator	 ==(SMOOTHInt);
		SMOOTHBool operator	 !=(SMOOTHInt);
		SMOOTHBool operator	 ==(SMOOTHArray<SMOOTHInt> *);
		SMOOTHBool operator	 !=(SMOOTHArray<SMOOTHInt> *);
};

#endif
