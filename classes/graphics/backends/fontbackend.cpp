 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/fontbackend.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>
#include <smooth/init.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/graphics/backends/gdi/fontgdi.h>
#endif

/* Cache computed text extents as this is very slow on Haiku.
 */
namespace smooth
{
	class ExtentsCacheEntry
	{
		private:
			UnsignedInt32	 crc32;

			GUI::Size	 extents;
		public:
					 ExtentsCacheEntry(const String &text,
							   const String &fontName,
							   Int fontSize,
							   Int fontWeight,
							   Int fontStyle)	{ crc32 = String(text).Append(fontName)
												      .Append(String::FromInt(fontSize	 << 16 |
															      fontWeight <<  4 |
															      fontStyle)).ComputeCRC32(); }

			UnsignedInt32	 GetCRC32() const			{ return crc32; }

			const GUI::Size	&GetExtents() const			{ return extents; }
			Void		 SetExtents(const GUI::Size &nExtents)	{ extents = nExtents; }
	};

	static Array<ExtentsCacheEntry *, Void *>	 extentsCache;
};

S::Int	 addFontBackendInitTmp = S::AddInitFunction(&S::GUI::FontBackend::Initialize);
S::Int	 addFontBackendFreeTmp = S::AddFreeFunction(&S::GUI::FontBackend::Free);

S::GUI::FontBackend *CreateFontBackend(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::GUI::FontBackend *(*S::GUI::FontBackend::backend_creator)(const String &, Short, Short, Short, const Color &) = &CreateFontBackend;

S::Int S::GUI::FontBackend::SetBackend(FontBackend *(*backend)(const String &, Short, Short, Short, const Color &))
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::GUI::FontBackend *S::GUI::FontBackend::CreateBackendInstance(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor)
{
	return backend_creator(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::GUI::FontBackend::FontBackend(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor)
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) FontGDI();
#endif

	type = FONT_NONE;

	fontName   = iFontName;
	fontSize   = iFontSize;
	fontColor  = iFontColor;
	fontWeight = iFontWeight;
	fontStyle  = iFontStyle;
}

S::GUI::FontBackend::~FontBackend()
{
}

S::Int S::GUI::FontBackend::Initialize()
{
	extentsCache.EnableLocking();

	return Success();
}

S::Int S::GUI::FontBackend::Free()
{
	foreach (ExtentsCacheEntry *entry, extentsCache) delete entry;

	extentsCache.RemoveAll();

	return Success();
}

S::Short S::GUI::FontBackend::GetFontType() const
{
	return type;
}

S::Int S::GUI::FontBackend::GetTextSizeX(const String &text, Bool scaled) const
{
	if (text == NIL) return 0;

	Int	 length	= text.Length();
	Int	 offset	= 0;
	Int	 sizex	= 0;

	for (Int i = 0; i <= length; i++)
	{
		if (i < length && text[i] != '\n') continue;

		sizex  = (Int) Math::Max(sizex, GetTextSize(text.SubString(offset, i - offset), scaled).cx);
		offset = i + 1;
	}

	return sizex;
}

S::Int S::GUI::FontBackend::GetTextSizeY(const String &text, Bool scaled) const
{
	if (text == NIL) return 0;

	Int	 length	= text.Length();
	Int	 lines	= 1;

	for (Int i = 0; i < length; i++)
	{
		if (text[i] == '\n') lines++;
	}

	/* Line height is always the same regardless of the actual text,
	 * so we use a default text here and multiply by the numer of lines.
	 */
	static String	 string	= "abcdefghijklmnopqrstuvwxyz";

	return (lines * (GetTextSize(string, scaled).cy - 1)) + (lines - 1) * 3;
}

S::GUI::Size S::GUI::FontBackend::GetTextSize(const String &text, Bool scaled) const
{
	if (text == NIL) return Size();

	/* Check for existing cache entry.
	 */
	ExtentsCacheEntry	 entry(text, fontName, fontSize, fontWeight, fontStyle);

	Float			 dpi = Surface().GetSurfaceDPI();
	Int			 crc = entry.GetCRC32();

	if (ExtentsCacheEntry *cacheEntry = extentsCache.Get(crc))
	{
		if (scaled || Math::Abs(dpi - 96.0) < 0.1) return cacheEntry->GetExtents();
		else					   return cacheEntry->GetExtents() * 96.0 / dpi;
	}

	/* Compute scaled text size.
	 */
	Size	 size = GetTextSize(text);

	entry.SetExtents(size);

	/* Save at most 2048 cache entries.
	 */
	if (extentsCache.Length() >= 2048)
	{
		for (Int i = 0; i < 1024; i++)
		{
			delete extentsCache.GetFirst();

			extentsCache.RemoveNth(0);
		}
	}

	extentsCache.Add(new ExtentsCacheEntry(entry), crc);

	/* Return computed text size.
	 */
	if (scaled || Math::Abs(dpi - 96.0) < 0.1) return size;
	else					   return size * 96.0 / dpi;
}

S::GUI::Size S::GUI::FontBackend::GetTextSize(const String &text) const
{
	return Size();
}
