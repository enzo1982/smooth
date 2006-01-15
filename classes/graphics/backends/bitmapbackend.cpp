 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/bitmapbackend.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/color.h>

#if defined __WIN32__ && defined __SMOOTH_STATIC__
	#include <smooth/graphics/backends/gdi/bitmapgdi.h>
#endif

S::GUI::BitmapBackend *CreateBitmapBackend_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapBackend(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapBackend_III(S::Int cx, S::Int cy, S::Int bpp)
{
	return new S::GUI::BitmapBackend(cx, cy, bpp);
}

S::GUI::BitmapBackend *CreateBitmapBackend_cI(const int nil)
{
	return new S::GUI::BitmapBackend(nil);
}

S::GUI::BitmapBackend *CreateBitmapBackend_crS(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapBackend(iBitmap);
}

S::Int	 bitmapBackendTmp_pV	= S::GUI::BitmapBackend::AddBackend(&CreateBitmapBackend_pV);
S::Int	 bitmapBackendTmp_III	= S::GUI::BitmapBackend::AddBackend(&CreateBitmapBackend_III);
S::Int	 bitmapBackendTmp_cI	= S::GUI::BitmapBackend::AddBackend(&CreateBitmapBackend_cI);
S::Int	 bitmapBackendTmp_crS	= S::GUI::BitmapBackend::AddBackend(&CreateBitmapBackend_crS);

S::Array<S::GUI::BitmapBackend *(*)(S::Void *), S::Void *>			*S::GUI::BitmapBackend::backend_creators_pV	= NIL;
S::Array<S::GUI::BitmapBackend *(*)(S::Int, S::Int, S::Int), S::Void *>		*S::GUI::BitmapBackend::backend_creators_III	= NIL;
S::Array<S::GUI::BitmapBackend *(*)(const int), S::Void *>			*S::GUI::BitmapBackend::backend_creators_cI	= NIL;
S::Array<S::GUI::BitmapBackend *(*)(const S::GUI::BitmapBackend &), S::Void *>	*S::GUI::BitmapBackend::backend_creators_crS	= NIL;

S::Int S::GUI::BitmapBackend::AddBackend(BitmapBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error();

	if (backend_creators_pV == NIL) backend_creators_pV = new Array<BitmapBackend *(*)(Void *), Void *>;

	backend_creators_pV->AddEntry(backend);

	return Success();
}

S::Int S::GUI::BitmapBackend::AddBackend(BitmapBackend *(*backend)(Int, Int, Int))
{
	if (backend == NIL) return Error();

	if (backend_creators_III == NIL) backend_creators_III = new Array<BitmapBackend *(*)(Int, Int, Int), Void *>;

	backend_creators_III->AddEntry(backend);

	return Success();
}

S::Int S::GUI::BitmapBackend::AddBackend(BitmapBackend *(*backend)(const int))
{
	if (backend == NIL) return Error();

	if (backend_creators_cI == NIL) backend_creators_cI = new Array<BitmapBackend *(*)(const int), Void *>;

	backend_creators_cI->AddEntry(backend);

	return Success();
}

S::Int S::GUI::BitmapBackend::AddBackend(BitmapBackend *(*backend)(const BitmapBackend &))
{
	if (backend == NIL) return Error();

	if (backend_creators_crS == NIL) backend_creators_crS = new Array<BitmapBackend *(*)(const BitmapBackend &), Void *>;

	backend_creators_crS->AddEntry(backend);

	return Success();
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(Void *iBitmap)
{
	if (backend_creators_pV->GetFirstEntry() != &CreateBitmapBackend_pV)	return backend_creators_pV->GetFirstEntry()(iBitmap);
	else									return backend_creators_pV->GetLastEntry()(iBitmap);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(Int cx, Int cy, Int bpp)
{
	if (backend_creators_III->GetFirstEntry() != &CreateBitmapBackend_III)	return backend_creators_III->GetFirstEntry()(cx, cy, bpp);
	else									return backend_creators_III->GetLastEntry()(cx, cy, bpp);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const int nil)
{
	if (backend_creators_cI->GetFirstEntry() != &CreateBitmapBackend_cI)	return backend_creators_cI->GetFirstEntry()(nil);
	else									return backend_creators_cI->GetLastEntry()(nil);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const BitmapBackend &iBitmap)
{
	if (backend_creators_crS->GetFirstEntry() != &CreateBitmapBackend_crS)	return backend_creators_crS->GetFirstEntry()(iBitmap);
	else									return backend_creators_crS->GetLastEntry()(iBitmap);
}

S::GUI::BitmapBackend::BitmapBackend(Void *iBitmap)
{
#if defined __WIN32__ && defined __SMOOTH_STATIC__
	volatile Bool	 null = 0;

	if (null) BitmapGDI();
#endif

	type	= BITMAP_NONE;

	size	= Size(0, 0);
	depth	= 0;

	bytes	= NIL;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(Int cx, Int cy, Int bpp)
{
	type	= BITMAP_NONE;

	size	= Size(cx, cy);
	depth	= bpp;

	bytes	= NIL;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(const int nil)
{
	type	= BITMAP_NONE;

	size	= Size(0, 0);
	depth	= 0;

	bytes	= NIL;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(const BitmapBackend &iBitmap)
{
	type	= BITMAP_NONE;

	size	= Size(0, 0);
	depth	= 0;

	bytes	= NIL;
	align	= 0;
}

S::GUI::BitmapBackend::~BitmapBackend()
{
}

S::Int S::GUI::BitmapBackend::GetBitmapType()
{
	return type;
}

const S::GUI::Size &S::GUI::BitmapBackend::GetSize()
{
	return size;
}

S::Int S::GUI::BitmapBackend::GetDepth()
{
	return depth;
}

S::UnsignedByte *S::GUI::BitmapBackend::GetBytes()
{
	return bytes;
}

S::Int S::GUI::BitmapBackend::GetLineAlignment()
{
	return align;
}

S::Bool S::GUI::BitmapBackend::CreateBitmap(Int cx, Int cy, Int bpp)
{
	return False;
}

S::Bool S::GUI::BitmapBackend::DeleteBitmap()
{
	return True;
}

S::Bool S::GUI::BitmapBackend::SetSystemBitmap(Void *nBitmap)
{
	return False;
}

S::Void *S::GUI::BitmapBackend::GetSystemBitmap() const
{
	return NIL;
}

S::Int S::GUI::BitmapBackend::GrayscaleBitmap()
{
	if (bytes == NIL) return Error();

	Color	 color;

	for (Int y = 0; y < size.cy; y++)
	{
		for (Int x = 0; x < size.cx; x++)
		{
			color = GetPixel(x, y);
			color = (color.GetRed() + color.GetGreen() + color.GetBlue()) / 3;
			color = Color(color, color, color);

			SetPixel(x, y, color);
		}
	}

	return Success();
}

S::Int S::GUI::BitmapBackend::ReplaceColor(Int color1, Int color2)
{
	if (bytes == NIL) return Error();

	for (Int y = 0; y < size.cy; y++)
	{
		for (Int x = 0; x < size.cx; x++)
		{
			if (GetPixel(x, y) == (UnsignedInt) color1) SetPixel(x, y, color2);
		}
	}

	return Success();
}

S::Bool S::GUI::BitmapBackend::SetPixel(Int x, Int y, UnsignedLong color)
{
	if (bytes == NIL)			return False;
	if (y >= size.cy || x >= size.cx)	return False;

	Bool	 done = False;
	Int	 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - ++y) * (((4 - (size.cx * 3) & 3) & 3) + size.cx * 3) + x * 3;

			bytes[offset + 0] = Color(color).GetBlue();
			bytes[offset + 1] = Color(color).GetGreen();
			bytes[offset + 2] = Color(color).GetRed();

			done = True;

			break;
		case 32:
			offset = (size.cy - ++y) * (((4 - (size.cx * 4) & 3) & 3) + size.cx * 4) + x * 4;

			bytes[offset + 0] = Color(color).GetBlue();
			bytes[offset + 1] = Color(color).GetGreen();
			bytes[offset + 2] = Color(color).GetRed();

			done = True;

			break;
	}

	return done;
}

S::UnsignedLong S::GUI::BitmapBackend::GetPixel(Int x, Int y)
{
	if (bytes == NIL)			return 0;
	if (y >= size.cy || x >= size.cx)	return 0;

	UnsignedLong	 color = 0;
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - ++y) * (((4 - (size.cx * 3) & 3) & 3) + size.cx * 3) + x * 3;

			color = Color(bytes[offset + 2], bytes[offset + 1], bytes[offset + 0]);

			break;
		case 32:
			offset = (size.cy - ++y) * (((4 - (size.cx * 4) & 3) & 3) + size.cx * 4) + x * 4;

			color = Color(bytes[offset + 2], bytes[offset + 1], bytes[offset + 0]);

			break;
	}

	return color;
}

S::GUI::BitmapBackend &S::GUI::BitmapBackend::operator =(const int nil)
{
	SetSystemBitmap(NIL);

	return *this;
}

S::GUI::BitmapBackend &S::GUI::BitmapBackend::operator =(const BitmapBackend &newBitmap)
{
	return *this;
}

S::Bool S::GUI::BitmapBackend::operator ==(const int nil) const
{
	return True;
}

S::Bool S::GUI::BitmapBackend::operator !=(const int nil) const
{
	return False;
}
