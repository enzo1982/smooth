 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECTTYPE_
#define _H_OBJSMOOTH_OBJECTTYPE_

#define SObjectType SMOOTHObjectType

class SMOOTHObject;
class SMOOTHObjectType;

#include "definitions.h"

class SMOOTHAPI SMOOTHObjectType
{
	friend class SMOOTHObject;
	private:
		SMOOTHInt		 type;
		SMOOTHObject		*object;
	public:
					 SMOOTHObjectType();
					 SMOOTHObjectType(const SMOOTHObjectType &);

		operator		 SMOOTHInt();
		SMOOTHInt operator	 =(SMOOTHInt);

		SMOOTHBool operator	 ==(SMOOTHInt);
		SMOOTHBool operator	 !=(SMOOTHInt);
};

#endif
