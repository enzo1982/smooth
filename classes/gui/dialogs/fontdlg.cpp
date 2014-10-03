 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
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
#	include <windows.h>
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
	static CHOOSEFONT	 cf;

	LOGFONT			 lf;

	if (parentWindow != NIL) cf.hwndOwner = (HWND) parentWindow->GetSystemWindow();
	else			 cf.hwndOwner = NIL;

	cf.lStructSize	= sizeof(CHOOSEFONT);
	cf.lpLogFont	= &lf;
	cf.Flags	= CF_SCREENFONTS | CF_SCRIPTSONLY | CF_NOSCRIPTSEL | CF_EFFECTS | CF_NOOEMFONTS | CF_NOVERTFONTS;
	cf.rgbColors	= 0;

	if (ChooseFont(&cf))
	{
		font.SetName(lf.lfFaceName);
		font.SetSize(cf.iPointSize);
		font.SetWeight(lf.lfWeight == FW_BOLD ? Font::Bold : Font::Normal);
		font.SetStyle((lf.lfItalic ? Font::Italic : Font::Normal) | (lf.lfUnderline ? Font::Underline : Font::Normal) | (lf.lfStrikeOut ? Font::StrikeOut : Font::Normal));
		font.SetColor(cf.rgbColors);
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
