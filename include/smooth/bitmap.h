 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BITMAP_
#define _H_OBJSMOOTH_BITMAP_

#define SBitmap SMOOTHBitmap

class SMOOTHBitmap;

#include "object.h"

class SMOOTHAPI SMOOTHBitmap : public SMOOTHObject
{
	protected:
		HBITMAP			 bitmap;
	public:
					 SMOOTHBitmap(HBITMAP, SMOOTHPoint, SMOOTHSize);
					~SMOOTHBitmap();

		virtual SMOOTHInt	 Paint(SMOOTHInt);

		SMOOTHInt		 SetBitmap(HBITMAP);
		HBITMAP			 GetBitmap();
};

SMOOTHVAR SMOOTHInt OBJ_HBITMAP;

#endif
