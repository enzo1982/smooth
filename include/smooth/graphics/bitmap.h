 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BITMAP_
#define _H_OBJSMOOTH_BITMAP_

namespace smooth
{
	namespace GUI
	{
		class BitmapBase;
	};
};

#include "../definitions.h"
#include "../size.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI BitmapBase
		{
			protected:
				Int				 type;

				Size				 size;
				Int				 depth;

				UnsignedByte			*bytes;
				Int				 align;
			public:
								 BitmapBase();
								 BitmapBase(const int);
								 BitmapBase(const BitmapBase &);

				virtual				~BitmapBase();

				Int				 GetBitmapType();

				Size				 GetSize();
				Int				 GetDepth();

				UnsignedByte			*GetBytes();
				Int				 GetLineAlignment();

				virtual Bool			 SetPixel(Int, Int, UnsignedLong);
				virtual UnsignedLong		 GetPixel(Int, Int);

//				virtual BitmapBase &operator	 =(const int) = 0;
//				virtual BitmapBase &operator	 =(const BitmapBase &) = 0;

//				virtual Bool operator		 ==(const int) = 0;
//				virtual Bool operator		 !=(const int) = 0;
		};
	};
};

#ifdef __WIN32__
	#include "gdi/bitmapgdi.h"
	#define Bitmap S::GUI::BitmapGDI
#else
	#include "none/bitmapnone.h"
	#define Bitmap S::GUI::BitmapNone
#endif

#endif
