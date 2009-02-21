 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define MAKEUNICODESTR(x) L##x

#include <smooth/gui/dialogs/fontdlg.h>
#include <smooth/gui/window/window.h>

#ifdef __WIN32__
#	include <commdlg.h>
#endif

S::GUI::Dialogs::FontSelection::FontSelection()
{
}

S::GUI::Dialogs::FontSelection::~FontSelection()
{
}

const Error &S::GUI::Dialogs::FontSelection::ShowDialog()
{
#ifdef __WIN32__
	static CHOOSEFONTW	 cfw;
	static CHOOSEFONTA	 cfa;
	LOGFONTW		 lfw;
	LOGFONTA		 lfa;
	bool			 result;

	if (parentWindow != NIL)
	{
		cfw.hwndOwner = (HWND) parentWindow->GetSystemWindow();
		cfa.hwndOwner = (HWND) parentWindow->GetSystemWindow();
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

	if (Setup::enableUnicode) result = ChooseFontW(&cfw);
	else			  result = ChooseFontA(&cfa);

	if (result)
	{
		if (Setup::enableUnicode)
		{
			font.SetName(lfw.lfFaceName);
			font.SetSize(cfw.iPointSize);
			font.SetWeight(lfw.lfWeight == FW_BOLD ? Font::Bold : Font::Normal);
			font.SetStyle((lfw.lfItalic ? Font::Italic : Font::Normal) | (lfw.lfUnderline ? Font::Underline : Font::Normal) | (lfw.lfStrikeOut ? Font::StrikeOut : Font::Normal));
			font.SetColor(cfw.rgbColors);
		}
		else
		{
			font.SetName(lfa.lfFaceName);
			font.SetSize(cfa.iPointSize);
			font.SetWeight(lfa.lfWeight == FW_BOLD ? Font::Bold : Font::Normal);
			font.SetStyle((lfa.lfItalic ? Font::Italic : Font::Normal) | (lfa.lfUnderline ? Font::Underline : Font::Normal) | (lfa.lfStrikeOut ? Font::StrikeOut : Font::Normal));
			font.SetColor(cfa.rgbColors);
		}
	}
	else
	{
		error = Error();
	}
#endif

	return error;
}

const S::GUI::Font &S::GUI::Dialogs::FontSelection::GetFont() const
{
	return font;
}
