 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cocoa/bitmapcocoa.h>

S::GUI::BitmapBackend *CreateBitmapCocoa_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapCocoa(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapCocoa_crSI(const S::GUI::Size &iSize, S::Int iDepth)
{
	return new S::GUI::BitmapCocoa(iSize, iDepth);
}

S::GUI::BitmapBackend *CreateBitmapCocoa_cI(const int null)
{
	return new S::GUI::BitmapCocoa(null);
}

S::GUI::BitmapBackend *CreateBitmapCocoa_crB(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapCocoa((const S::GUI::BitmapCocoa &) iBitmap);
}

S::Int	 bitmapCocoaTmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapCocoa_pV);
S::Int	 bitmapCocoaTmp_crSI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapCocoa_crSI);
S::Int	 bitmapCocoaTmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapCocoa_cI);
S::Int	 bitmapCocoaTmp_crB	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapCocoa_crB);

S::GUI::BitmapCocoa::BitmapCocoa(Void *iBitmap)
{
	Initialize();

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapCocoa::BitmapCocoa(const Size &iSize, Int iDepth)
{
	Initialize();

	CreateBitmap(iSize, iDepth);
}

S::GUI::BitmapCocoa::BitmapCocoa(const int null)
{
	Initialize();

	SetSystemBitmap(NIL);
}

S::GUI::BitmapCocoa::BitmapCocoa(const BitmapCocoa &iBitmap)
{
	Initialize();

	SetSystemBitmap((Void *) iBitmap.bitmap);

	depth = iBitmap.depth;
}

S::GUI::BitmapCocoa::~BitmapCocoa()
{
	DeleteBitmap();
}

S::Void S::GUI::BitmapCocoa::Initialize()
{
	type	= BITMAP_COCOA;
	bitmap	= NIL;
}

S::Bool S::GUI::BitmapCocoa::CreateBitmap(const Size &nSize, Int nDepth)
{
	DeleteBitmap();

	if (nDepth == -1)		  nDepth = [[NSScreen mainScreen] depth];
	if (nDepth != 24 && nDepth != 32) nDepth = 24;

	bitmap	= [[NSBitmapImageRep alloc] initWithBitmapDataPlanes: nil
							  pixelsWide: nSize.cx
							  pixelsHigh: nSize.cy
						       bitsPerSample: 8
						     samplesPerPixel: nDepth == 32 ? 4 : 3
							    hasAlpha: nDepth == 32
							    isPlanar: NO
						      colorSpaceName: NSCalibratedRGBColorSpace
							bitmapFormat: 0
							 bytesPerRow: 4 * nSize.cx
							bitsPerPixel: 32];

	if (bitmap == NIL) return False;

	bytes	= [bitmap bitmapData];

	size	= nSize;
	depth	= nDepth;
	bpp	= 32;
	align	= 4;

	return True;
}

S::Bool S::GUI::BitmapCocoa::DeleteBitmap()
{
	if (bitmap != NIL)
	{
		[bitmap release];

		bitmap	= NIL;

		size	= Size(0, 0);
		depth	= 0;

		bytes	= NIL;
		bpp	= 0;
		align	= 0;
	}

	return True;
}

S::Int S::GUI::BitmapCocoa::Scale(const Size &newSize)
{
	if (bitmap == NIL) return Error();

	if (size == newSize) return Success();

	NSBitmapImageRep	*backup = bitmap;
	Size			 backupSize = size;

	bitmap = NIL;

	CreateBitmap(newSize, depth);

	NSImage			*image	 = [[NSImage alloc] initWithSize: NSMakeSize(backupSize.cx, backupSize.cy)];

	[image addRepresentation: backup];

	NSAutoreleasePool	*pool	 = [[NSAutoreleasePool alloc] init];
	NSGraphicsContext	*context = [NSGraphicsContext graphicsContextWithBitmapImageRep: bitmap];

	[NSGraphicsContext saveGraphicsState];
	[NSGraphicsContext setCurrentContext: context];

	[image drawInRect: NSMakeRect(0, 0, newSize.cx, newSize.cy)
		 fromRect: NSMakeRect(0, 0, backupSize.cx, backupSize.cy)
		operation: NSCompositeCopy
		 fraction: 1.0];

	[NSGraphicsContext restoreGraphicsState];

	[pool release];
	[image release];
	[backup release];

	return Success();
}

S::Bool S::GUI::BitmapCocoa::SetSystemBitmap(Void *nBitmap)
{
	if (nBitmap == GetSystemBitmap()) return True;

	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		CreateBitmap(Size([(NSBitmapImageRep *) nBitmap pixelsWide], [(NSBitmapImageRep *) nBitmap pixelsHigh]), 8 * [(NSBitmapImageRep *) nBitmap samplesPerPixel]);

		/* Copy source bitmap to destination.
		 */
		NSAutoreleasePool	*pool	 = [[NSAutoreleasePool alloc] init];
		NSGraphicsContext	*context = [NSGraphicsContext graphicsContextWithBitmapImageRep: bitmap];

		[NSGraphicsContext saveGraphicsState];
		[NSGraphicsContext setCurrentContext: context];

		[(NSBitmapImageRep *) nBitmap drawInRect: NSMakeRect(0, 0, size.cx, size.cy)];

		[NSGraphicsContext restoreGraphicsState];

		[pool release];
	}

	return True;
}

S::Void *S::GUI::BitmapCocoa::GetSystemBitmap() const
{
	return (Void *) bitmap;
}

S::Bool S::GUI::BitmapCocoa::SetPixel(const Point &point, const Color &color)
{
	if (bytes == NIL)			      return False;
	if (point.y >= size.cy || point.x >= size.cx) return False;

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = (size.cy - point.y - 1) * (size.cx * 4) + point.x * 4;;

	switch (depth)
	{
		case 24:
			data[offset + 2] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 0] =  color	 & 255;

			return True;
		case 32:
			data[offset + 2] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 0] =  color	 & 255;
			data[offset + 3] = (color >> 24) & 255;

			return True;
	}

	return False;
}

S::GUI::Color S::GUI::BitmapCocoa::GetPixel(const Point &point) const
{
	if (bytes == NIL)			      return 0;
	if (point.y >= size.cy || point.x >= size.cx) return 0;

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = (size.cy - point.y - 1) * (size.cx * 4) + point.x * 4;;

	switch (depth)
	{
		case 24:
			return Color(			      data[offset + 2] << 16 | data[offset + 1] << 8 | data[offset + 0]);
 		case 32:
			return Color(data[offset + 3] << 24 | data[offset + 2] << 16 | data[offset + 1] << 8 | data[offset + 0]);
	}

	return 0;
}

S::GUI::BitmapBackend &S::GUI::BitmapCocoa::operator =(const BitmapBackend &newBitmap)
{
	SetSystemBitmap((Void *) ((BitmapCocoa &) newBitmap).bitmap);

	return *this;
}

S::Bool S::GUI::BitmapCocoa::operator ==(const int null) const
{
	if (bitmap == NIL)	return True;
	else			return False;
}

S::Bool S::GUI::BitmapCocoa::operator !=(const int null) const
{
	if (bitmap == NIL)	return False;
	else			return True;
}
