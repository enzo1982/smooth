 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define MAKEUNICODESTR(x) L##x

#include <smooth/fontdlg.h>
#include <smooth/stk.h>
#include <smooth/graphics/gdi/windowgdi.h>

#include <commdlg.h>

S::DialogFontSelection::DialogFontSelection()
{
}

S::DialogFontSelection::~DialogFontSelection()
{
}

S::Int S::DialogFontSelection::ShowDialog()
{
	static CHOOSEFONTW	 cfw;
	static CHOOSEFONTA	 cfa;
	LOGFONTW		 lfw;
	LOGFONTA		 lfa;
	bool			 result;
	Int			 retValue = Success;

	if (parentWindow != NIL)
	{
		cfw.hwndOwner = ((GUI::WindowGDI *) parentWindow)->hwnd;
		cfa.hwndOwner = ((GUI::WindowGDI *) parentWindow)->hwnd;
	}
	else
	{
		cfw.hwndOwner = NIL;
		cfa.hwndOwner = NIL;
	}

	cfw.lStructSize	= sizeof(CHOOSEFONTW);
	cfw.lpLogFont	= &lfw;
	cfw.Flags	= CF_SCREENFONTS | CF_SCRIPTSONLY | CF_NOSCRIPTSEL | CF_EFFECTS | CF_NOOEMFONTS | CF_NOVERTFONTS;
	cfw.rgbColors	= 0;

	cfa.lStructSize	= sizeof(CHOOSEFONTA);
	cfa.lpLogFont	= &lfa;
	cfa.Flags	= CF_SCREENFONTS | CF_SCRIPTSONLY | CF_NOSCRIPTSEL | CF_EFFECTS | CF_NOOEMFONTS | CF_NOVERTFONTS;
	cfa.rgbColors	= 0;

	if (Setup::enableUnicode)	result = ChooseFontW(&cfw);
	else				result = ChooseFontA(&cfa);

	if (result)
	{
		if (Setup::enableUnicode)
		{
			font.SetName(lfw.lfFaceName);
			font.SetSize(cfw.iPointSize);
			font.SetWeight(lfw.lfWeight);
			font.SetItalic(lfw.lfItalic);
			font.SetUnderline(lfw.lfUnderline);
			font.SetStrikeOut(lfw.lfStrikeOut);
			font.SetColor(cfw.rgbColors);
		}
		else
		{
			font.SetName(lfa.lfFaceName);
			font.SetSize(cfa.iPointSize);
			font.SetWeight(lfa.lfWeight);
			font.SetItalic(lfa.lfItalic);
			font.SetUnderline(lfa.lfUnderline);
			font.SetStrikeOut(lfa.lfStrikeOut);
			font.SetColor(cfa.rgbColors);
		}
	}
	else
	{
		retValue = Error;
	}

	return retValue;
}

S::GUI::Font S::DialogFontSelection::GetFont()
{
	return font;
}
