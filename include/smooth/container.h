 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CONTAINER_
#define _H_OBJSMOOTH_CONTAINER_

#define SContainer SMOOTHContainer

class SMOOTHContainer;
class SMOOTHObject;
class SMOOTHSurface;

#include "array.h"
#include "containertype.h"

class SMOOTHAPI SMOOTHContainer
{
	protected:
		SMOOTHContainerType		 containerType;
		SMOOTHObject			*self;

		SMOOTHInt			 nOfObjects;
		SMOOTHArray<SMOOTHObject *>	 assocObjects;

		SMOOTHSurface			*nullSurface;
		SMOOTHSurface			*drawSurface;
	public:

						 SMOOTHContainer();
		virtual				~SMOOTHContainer();

		virtual SMOOTHInt		 RegisterObject(SMOOTHObject *);
		virtual SMOOTHInt		 UnregisterObject(SMOOTHObject *);

		SMOOTHInt			 GetNOfObjects();
		SMOOTHObject			*RequestObject(SMOOTHInt);

		virtual SMOOTHBool		 IsContainerCompatible(SMOOTHInt);
		SMOOTHContainerType		 GetContainerType();

		SMOOTHObject			*GetContainerObject();

		virtual SMOOTHSurface		*GetDrawSurface();
};

SMOOTHVAR SMOOTHInt OBJ_CONTAINER;

#endif
