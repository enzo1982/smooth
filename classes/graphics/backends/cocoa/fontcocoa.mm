 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cocoa/fontcocoa.h>
#include <smooth/graphics/surface.h>
#include <smooth/foreach.h>
#include <smooth/init.h>

S::GUI::FontBackend *CreateFontCocoa(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontCocoa(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontCocoaTmp = S::GUI::FontBackend::SetBackend(&CreateFontCocoa);

S::Int	 addFontCocoaFreeTmp = S::AddFreeFunction(&S::GUI::FontCocoa::Free);

/* Cache computed text extents as this is very slow on OS X.
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

			Int		 ComputeCRC32(const String &text) const { return String(text).Append(fontName)
												     .Append(String::FromInt(fontSize	<< 16 |
															     fontWeight <<  4 |
															     fontStyle)).ComputeCRC32(); }
	};

	static Array<ExtentsCacheEntry *> extentsCache;
};

S::Int S::GUI::FontCocoa::Free()
{
	foreach (ExtentsCacheEntry *entry, extentsCache) delete entry;

	extentsCache.RemoveAll();

	return Success();
}

S::GUI::FontCocoa::FontCocoa(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_COCOA;
}

S::GUI::FontCocoa::~FontCocoa()
{
}

S::GUI::Size S::GUI::FontCocoa::GetTextSize(const String &iText, Bool scaled) const
{
	if (iText == NIL) return Size();

	ExtentsCacheEntry	 entry(fontName, fontSize, fontWeight, fontStyle);

	Float			 dpi	    = Surface().GetSurfaceDPI();
	Int			 crc	    = entry.ComputeCRC32(iText);

	if (extentsCache.Get(crc) != NIL)
	{
		if (scaled || Math::Abs(dpi - 96.0) < 0.1) return extentsCache.Get(crc)->GetExtents();
		else					   return extentsCache.Get(crc)->GetExtents() * 96.0 / dpi;
	}

	NSAutoreleasePool	*pool	    = [[NSAutoreleasePool alloc] init];
	NSFont			*font	    = [[NSFontManager sharedFontManager] fontWithFamily: [NSString stringWithUTF8String: fontName.ConvertTo("UTF-8")]
											 traits: (fontStyle & Font::Italic ? NSItalicFontMask : 0) | (fontWeight >= Font::Bold ? NSBoldFontMask : 0)
											 weight: 5
											   size: fontSize * dpi / 72.0];
	NSDictionary		*attributes = [NSDictionary dictionaryWithObject: font
									  forKey: NSFontAttributeName];
	NSAttributedString	*string	    = [[[NSAttributedString alloc] initWithString: [NSString stringWithUTF8String: iText.ConvertTo("UTF-8")]
									       attributes: attributes] autorelease];

	NSSize	 size = [string size];

	[pool release];

	if (scaled || Math::Abs(dpi - 96.0) < 0.1) entry.SetExtents(Size(size.width, size.height - 2));
	else					   entry.SetExtents(Size(size.width, size.height - 2) * 96.0 / dpi);

	if (extentsCache.Length() > 2048) Free();

	extentsCache.Add(new ExtentsCacheEntry(entry), crc);

	if (scaled || Math::Abs(dpi - 96.0) < 0.1) return Size(size.width, size.height - 2);
	else					   return Size(size.width, size.height - 2) * 96.0 / dpi;
}
