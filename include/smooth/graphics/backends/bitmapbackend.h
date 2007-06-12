 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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

#include "../../definitions.h"
#include "../forms/size.h"
#include "../forms/rect.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 BITMAP_NONE = 0;

		class BitmapBackend
		{
			private:
				static Array<BitmapBackend *(*)(Void *), Void *>		*backend_creators_pV;
				static Array<BitmapBackend *(*)(Int, Int, Int), Void *>		*backend_creators_III;
				static Array<BitmapBackend *(*)(const int), Void *>		*backend_creators_cI;
				static Array<BitmapBackend *(*)(const BitmapBackend &), Void *>	*backend_creators_crS;
			protected:
				Int								 type;

				Size								 size;
				Int								 depth;

				Void								*bytes;
				Int								 align;
			public:
				static Int							 AddBackend(BitmapBackend *(*)(Void *));
				static Int							 AddBackend(BitmapBackend *(*)(Int, Int, Int));
				static Int							 AddBackend(BitmapBackend *(*)(const int));
				static Int							 AddBackend(BitmapBackend *(*)(const BitmapBackend &));

				static BitmapBackend						*CreateBackendInstance(Void *);
				static BitmapBackend						*CreateBackendInstance(Int, Int, Int);
				static BitmapBackend						*CreateBackendInstance(const int);
				static BitmapBackend						*CreateBackendInstance(const BitmapBackend &);

												 BitmapBackend(Void * = NIL);
												 BitmapBackend(Int, Int, Int);
												 BitmapBackend(const int);
												 BitmapBackend(const BitmapBackend &);

				virtual								~BitmapBackend();

				Int								 GetBitmapType() const;

				const Size							&GetSize() const;
				Int								 GetDepth() const;

				UnsignedByte							*GetBytes() const;
				Int								 GetLineAlignment() const;

				virtual Bool							 CreateBitmap(Int, Int, Int);
				virtual Bool							 DeleteBitmap();

				virtual Bool							 SetSystemBitmap(Void *);
				virtual Void							*GetSystemBitmap() const;

				virtual Int							 GrayscaleBitmap();
				virtual Int							 InvertColors();
				virtual Int							 ReplaceColor(const Color &, const Color &);

				virtual Bool							 SetPixel(Int, Int, const Color &);
				virtual Color							 GetPixel(Int, Int) const;

				virtual BitmapBackend &operator					 =(const int);
				virtual BitmapBackend &operator					 =(const BitmapBackend &);

				virtual Bool operator						 ==(const int) const;
				virtual Bool operator						 !=(const int) const;
		};
	};
};

#endif
