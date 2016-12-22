 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
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
			String		 fontName;
			Int		 fontSize;
			Int		 fontWeight;
			Int		 fontStyle;

			GUI::Size	 extents;
		public:
					 ExtentsCacheEntry(const String &fnm, Int fsz, Int fwt, Int fst) : fontName(fnm),
													   fontSize(fsz),
													   fontWeight(fwt),
													   fontStyle(fst) { }

			const GUI::Size	&GetExtents() const			{ return extents; }
			Void		 SetExtents(const GUI::Size &nExtents)	{ extents = nExtents; }

			UnsignedInt32	 ComputeCRC32(const String &text) const { return String(text).Append(fontName)
												     .Append(String::FromInt(fontSize	<< 16 |
															     fontWeight <<  4 |
															     fontStyle)).ComputeCRC32(); }
	};

	static Array<ExtentsCacheEntry *> extentsCache;
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

	Int	 nOfChars = text.Length();
	Int	 sizex	= 0;
	Int	 lines	= 1;
	Int	 offset	= 0;

	for (Int k = 0; k < nOfChars; k++) if (text[k] == 10) lines++;

	for (Int i = 0; i < lines; i++)
	{
		Int	 origOffset = offset;
		String	 line;

		for (Int j = 0; j <= nOfChars; j++)
		{
			if (j + origOffset == nOfChars)
			{
				line[j] = 0;
				break;
			}

			if (text[j + origOffset] == 10 || text[j + origOffset] == 0)
			{
				offset++;
				line[j] = 0;
				break;
			}
			else
			{
				offset++;
				line[j] = text[j + origOffset];
			}
		}

		sizex = (Int) Math::Max(sizex, GetTextSize(line, scaled).cx);
	}

	return sizex;
}

S::Int S::GUI::FontBackend::GetTextSizeY(const String &text, Bool scaled) const
{
	if (text == NIL) return 0;

	Int	 lines		= 1;
	Int	 txtSize	= text.Length();

	for (Int i = 0; i < txtSize; i++)
	{
		if (text[i] == 10) lines++;
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
	ExtentsCacheEntry	 entry(fontName, fontSize, fontWeight, fontStyle);

	Float			 dpi = Surface().GetSurfaceDPI();
	Int			 crc = entry.ComputeCRC32(text);

	if (extentsCache.Get(crc) != NIL)
	{
		if (scaled || Math::Abs(dpi - 96.0) < 0.1) return extentsCache.Get(crc)->GetExtents();
		else					   return extentsCache.Get(crc)->GetExtents() * 96.0 / dpi;
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
