 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cocoa/fontcocoa.h>
#include <smooth/graphics/surface.h>
#include <smooth/init.h>

S::GUI::FontBackend *CreateFontCocoa(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontCocoa(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontCocoaTmp = S::GUI::FontBackend::SetBackend(&CreateFontCocoa);

S::Int	 addFontCocoaInitTmp = S::AddInitFunction(&S::GUI::FontCocoa::Initialize);

S::Int S::GUI::FontCocoa::Initialize()
{
	/* The default font size on macOS is 13pt. However, apps like Finder and other Apple
	 * software use various font sizes between 11 and 13pt depending on the kind of label.
	 *
	 * smooth apps look too bulky with the default 13pt font size, so we apply a scale
	 * factor to reduce the standard size to 12pt.
	 */
	Float	 fontSize = [NSFont systemFontSize] / 13.0 * 12.0;

	Font::Default.ImportFrom("UTF-8", [[[NSFont systemFontOfSize: fontSize] familyName] UTF8String]);

	Setup::FontSize = fontSize / 96.0 * 72.0 / Font::DefaultSize;

	return Success();
}

S::GUI::FontCocoa::FontCocoa(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_COCOA;
}

S::GUI::FontCocoa::~FontCocoa()
{
}

S::GUI::Size S::GUI::FontCocoa::GetTextSize(const String &text) const
{
	if (text == NIL) return Size();

	NSAutoreleasePool	*pool	    = [[NSAutoreleasePool alloc] init];

	NSFont			*font	    = GetNativeFont(Font(fontName, fontSize, fontWeight, fontStyle));
	NSDictionary		*attributes = [NSDictionary dictionaryWithObject: font
									  forKey: NSFontAttributeName];
	NSAttributedString	*string	    = [[[NSAttributedString alloc] initWithString: [NSString stringWithUTF8String: text.ConvertTo("UTF-8")]
									       attributes: attributes] autorelease];

	NSSize	 size = [string size];

	[pool release];

	return Size(size.width, size.height - 2);
}

NSFont *S::GUI::FontCocoa::GetNativeFont(const Font &font)
{
	static SEL      monoSpacedDigitSystemFontSelector = @selector(monospacedDigitSystemFontOfSize:weight:);
	static Bool     queryMonospacedDigitFont = [[NSFont class] respondsToSelector: monoSpacedDigitSystemFontSelector];

	/* Get font object.
	 */
	Float   dpi        = Surface().GetSurfaceDPI();
	NSFont *nativeFont = nil;

	if (queryMonospacedDigitFont && font.GetName() == Font::Default)
	{
		/* Get font with monospaced digits if available.
		 */
		nativeFont = ((NSFont *(*)(id, SEL, CGFloat, CGFloat)) [[NSFont class] methodForSelector: monoSpacedDigitSystemFontSelector])([NSFont class], monoSpacedDigitSystemFontSelector, font.GetSize() * dpi / 72.0, 0.0);
	}
	else
	{
		/* Otherwise use standard system font.
		 */
		nativeFont = [[NSFontManager sharedFontManager] fontWithFamily: [NSString stringWithUTF8String: font.GetName().ConvertTo("UTF-8")]
									traits: 0
									weight: 5
									  size: font.GetSize() * dpi / 72.0];
	}

	/* Set font traits.
	 */
	if (font.GetWeight() >= Font::Bold) nativeFont = [[NSFontManager sharedFontManager] convertFont: nativeFont toHaveTrait: NSBoldFontMask];
	if (font.GetStyle() & Font::Italic) nativeFont = [[NSFontManager sharedFontManager] convertFont: nativeFont toHaveTrait: NSItalicFontMask];

	return nativeFont;
}
