 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_APPLICATION_
#define _H_OBJSMOOTH_APPLICATION_

#define SApplication SMOOTHApplication

class SMOOTHApplication;

#include "object.h"
#include "container.h"

class SMOOTHAPI SMOOTHApplication : public SMOOTHObject, public SMOOTHContainer
{
	public:
		 SMOOTHApplication();
		 SMOOTHApplication(SMOOTHString);
};

SMOOTHVAR SMOOTHInt OBJ_APPLICATION;

#endif
