 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BITMAPDIRECT2D
#define H_OBJSMOOTH_BITMAPDIRECT2D

namespace smooth
{
	namespace GUI
	{
		class BitmapDirect2D;
	};
};

#include <windows.h>

#include "../gdi/bitmapgdi.h"

#include <d2d1_1.h>

namespace smooth
{
	namespace GUI
	{
		const Short	 BITMAP_DIRECT2D = 8;

		/* GDI bitmap with a cached copy on the Direct2D device.
		 *
		 * The DIB section remains the master copy. Any access that may
		 * change it invalidates the cached device bitmap, which is then
		 * recreated on its next use.
		 */
		class BitmapDirect2D : public BitmapGDI
		{
			private:
				class DeviceBitmap
				{
					public:
						ID2D1Device	*device;
						ID2D1Bitmap	*bitmap;

						Size		 size;
						UnsignedInt32	 modification;

								 DeviceBitmap() : device(NIL), bitmap(NIL), modification(0) { }

						Void		 Release();
				};

				mutable UnsignedInt32	 modification;

				mutable DeviceBitmap	 unscaled;
				mutable DeviceBitmap	 scaled;

				Void			 Modified() const	{ modification++; }

				static ID2D1Bitmap	*CreateDeviceBitmap(ID2D1DeviceContext *, const UnsignedByte *, const Size &, Int);
			public:
							 BitmapDirect2D(Void * = NIL);
							 BitmapDirect2D(const Size &, Int);
							 BitmapDirect2D(const int);
							 BitmapDirect2D(const BitmapDirect2D &);

							~BitmapDirect2D();

				Bool			 CreateBitmap(const Size &, Int);

				Void			*GetSystemBitmap() const;

				UnsignedByte		*GetBytes() const;

				Bool			 SetPixel(const Point &, const Color &);

				/* Get the bitmap, scaled to the given size, as a premultiplied
				 * Direct2D bitmap for the device of the given device context.
				 * The bitmap remains owned by this object.
				 */
				ID2D1Bitmap		*GetDeviceBitmap(ID2D1DeviceContext *, const Size &) const;
		};
	};
};

#endif
