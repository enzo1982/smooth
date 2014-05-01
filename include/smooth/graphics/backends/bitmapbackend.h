 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BITMAPBACKEND
#define H_OBJSMOOTH_BITMAPBACKEND

namespace smooth
{
	namespace GUI
	{
		class Surface;

		class Bitmap;
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
		const Short	 BITMAP_NONE = 0;

		class BitmapBackend
		{
			private:
				static BitmapBackend		*(*backend_creator_pV)(Void *);
				static BitmapBackend		*(*backend_creator_crSI)(const Size &, Int);
				static BitmapBackend		*(*backend_creator_cI)(const int);
				static BitmapBackend		*(*backend_creator_crB)(const BitmapBackend &);
			protected:
				Short				 type;

				Byte				 depth;
				Byte				 bpp;
				Byte				 align;

				Size				 size;
				Void				*bytes;
			public:
				static Int			 SetBackend(BitmapBackend *(*)(Void *));
				static Int			 SetBackend(BitmapBackend *(*)(const Size &, Int));
				static Int			 SetBackend(BitmapBackend *(*)(const int));
				static Int			 SetBackend(BitmapBackend *(*)(const BitmapBackend &));

				static BitmapBackend		*CreateBackendInstance(Void *);
				static BitmapBackend		*CreateBackendInstance(const Size &, Int);
				static BitmapBackend		*CreateBackendInstance(const int);
				static BitmapBackend		*CreateBackendInstance(const BitmapBackend &);

								 BitmapBackend(Void * = NIL);
								 BitmapBackend(const Size &, Int);
								 BitmapBackend(const int);
								 BitmapBackend(const BitmapBackend &);

				virtual				~BitmapBackend();

				Short				 GetBitmapType() const;

				const Size			&GetSize() const;
				Byte				 GetDepth() const;

				UnsignedByte			*GetBytes() const;
				Byte				 GetBitsPerPixel() const;
				Byte				 GetLineAlignment() const;

				virtual Bool			 CreateBitmap(const Size &, Int);
				virtual Bool			 DeleteBitmap();

				virtual Bool			 SetSystemBitmap(Void *);
				virtual Void			*GetSystemBitmap() const;

				virtual Int			 GrayscaleBitmap();
				virtual Int			 InvertColors();
				virtual Int			 ReplaceColor(const Color &, const Color &);
				virtual Int			 SetBackgroundColor(const Color &);

				virtual Bitmap			 Scale(const Size &) const;

				virtual Bool			 SetPixel(const Point &, const Color &);
				virtual Color			 GetPixel(const Point &) const;

				virtual BitmapBackend &operator	 =(const int);
				virtual BitmapBackend &operator	 =(const BitmapBackend &);

				virtual Bool operator		 ==(const int) const;
				virtual Bool operator		 !=(const int) const;
		};
	};
};

#endif
