 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BITMAPBACKEND_
#define _H_OBJSMOOTH_BITMAPBACKEND_

namespace smooth
{
	namespace GUI
	{
		class Surface;
		class BitmapBackend;
	};
};

#include "../definitions.h"
#include "../size.h"
#include "../rect.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 BITMAP_NULL = 0;

		class BitmapBackend
		{
			protected:
				Int				 type;

				Size				 size;
				Int				 depth;

				UnsignedByte			*bytes;
				Int				 align;
			public:
								 BitmapBackend(Void * = NIL);
								 BitmapBackend(Int, Int, Int);
								 BitmapBackend(const int);
								 BitmapBackend(const BitmapBackend &);

				virtual				~BitmapBackend();

				Int				 GetBitmapType();

				Size				 GetSize();
				Int				 GetDepth();

				UnsignedByte			*GetBytes();
				Int				 GetLineAlignment();

				virtual Bool			 CreateBitmap(Int, Int, Int);
				virtual Bool			 DeleteBitmap();

				virtual Bool			 SetSystemBitmap(Void *);
				virtual Void			*GetSystemBitmap();

				virtual Int			 GrayscaleBitmap();
				virtual Int			 ReplaceColor(Int, Int);

				virtual Bool			 SetPixel(Int, Int, UnsignedLong);
				virtual UnsignedLong		 GetPixel(Int, Int);

				virtual BitmapBackend &operator	 =(const int);
				virtual BitmapBackend &operator	 =(const BitmapBackend &);

				virtual Bool operator		 ==(const int);
				virtual Bool operator		 !=(const int);
		};
	};
};

#endif
