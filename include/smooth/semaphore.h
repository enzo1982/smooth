 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SEMAPHORE_
#define _H_OBJSMOOTH_SEMAPHORE_

#define SSemaphore SMOOTHSemaphore

class SMOOTHSemaphore;

#include "object.h"

class SMOOTHAPI SMOOTHSemaphore : public SMOOTHObject
{
	private:
		LiSASemaphore	*semaphore;
	public:
				 SMOOTHSemaphore();
				~SMOOTHSemaphore();

		SMOOTHInt	 Wait();
		SMOOTHInt	 Release();
};

SMOOTHVAR SMOOTHInt OBJ_SEMAPHORE;

#endif
