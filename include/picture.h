 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_PICTURE_
#define _H_PICTURE_

#ifdef __WIN32__
#include <windows.h>
#endif

#include <smooth/system.h>
#include <smooth/toolkit.h>

class picture
{
	HBITMAP			 bmPic;
	HBITMAP			 bmOld;
	HDC			 hdc;
	bool			 keepHDC;

	unsigned long		 width;
	unsigned long		 height;

	unsigned char		*bytes;
	unsigned long		 bytewidth;

	public:
				 picture(bool toHDC = false);
				 picture(unsigned long, unsigned long, unsigned char, bool toHDC = false);
				~picture();

		bool		 DeleteBitmap();

		bool		 CreateBitmap(unsigned long, unsigned long, unsigned char, bool toHDC = false); 

		HBITMAP		 GetBitmap() { return bmPic; }
		unsigned long	 GetWidth() { return width; }
		unsigned long	 GetHeight() { return height; }
		unsigned char	*GetBytes() { return bytes; }

		void		 SetPixel(unsigned long, unsigned long, unsigned long);
		unsigned long	 GetPixel(unsigned long, unsigned long);

#ifdef __WIN32__
		bool		 BlitToDC(HDC, RECT *rcTrg = NIL, RECT *rcSrc = NIL);
		bool		 BlitToDC(picture *, RECT *rcTrg = NIL, RECT *rcSrc = NIL);

		bool		 BlitFromDC(HDC, RECT *, RECT *rcTrg = NIL);
#endif
};

inline void picture::SetPixel(unsigned long x, unsigned long y, unsigned long color)
{
	if (y >= height || x >= width) return;

	int offset = (height - ++y) * (((4 - (width * 3) & 3) & 3) + width * 3) + x * 3;

	bytes[offset + 0] = GetBlue(color);
	bytes[offset + 1] = GetGreen(color);
	bytes[offset + 2] = GetRed(color);
}

inline unsigned long picture::GetPixel(unsigned long x, unsigned long y)
{
	if (y >= height || x >= width) return 0;

	int offset = (height - ++y) * (((4 - (width * 3) & 3) & 3) + width * 3) + x * 3;

	return RGB(bytes[offset + 2], bytes[offset + 1], bytes[offset + 0]);
}

#endif
