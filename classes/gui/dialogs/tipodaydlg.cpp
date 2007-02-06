 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/tipodaydlg.h>

#include <smooth/definitions.h>
#include <smooth/gui/application/application.h>

#include <smooth/graphics/surface.h>

#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/widgets/basic/divider.h>

#include <smooth/gui/window/window.h>

#include <smooth/gui/widgets/basic/button.h>
#include <smooth/gui/widgets/basic/text.h>
#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/gui/widgets/basic/image.h>

#include <smooth/gui/widgets/layer.h>

#include <smooth/misc/string.h>
#include <smooth/i18n/i18n.h>

#include <smooth/resources.h>
#include <smooth/backends/win32/backendwin32.h>

#include <time.h>
#include <stdlib.h>

S::GUI::Dialogs::TipOfTheDay::TipOfTheDay(Bool *iShowTips)
{
	showTips = iShowTips;

	mode = TIP_ORDERED;
	offset = 0;

	Point	 pos;
	Size	 size;

	dlgwnd		= new Window(I18n::Translator::defaultTranslator->TranslateString("Tip of the day"), Point(Int((LiSAGetDisplaySizeX() - 350) / 2), Int((LiSAGetDisplaySizeY() - 300) / 2) - 50), Size(352, 302));

	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar		= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x = 87;
	pos.y = 29;

	btn_ok		= new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&TipOfTheDay::ButtonOK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 175;

	btn_next	= new Button(I18n::Translator::defaultTranslator->TranslateString("Next tip"), NIL, pos, size);
	btn_next->onAction.Connect(&TipOfTheDay::ButtonNext, this);
	btn_next->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y -= 2;

	size.cx = 150;

	check_showtips	= new CheckBox(I18n::Translator::defaultTranslator->TranslateString("Show tips on startup"), pos, size, showTips);
	check_showtips->SetOrientation(OR_LOWERLEFT);
	check_showtips->SetWidth(check_showtips->textSize.cx + 21);

	pos.x = 5;
	pos.y = 3;

	Bitmap	 bmp = Bitmap((HBITMAP) LoadImageA(hDllInstance, MAKEINTRESOURCEA(IDB_LIGHT), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS | LR_SHARED));

	bmp.ReplaceColor(RGB(192, 192, 192), Setup::BackgroundColor);

	img_light	= new Image(bmp, pos, Size(32, 32));

	pos.x = 40;
	pos.y = 7;

	txt_didyouknow	= new Text(I18n::Translator::defaultTranslator->TranslateString("Did you know..."), pos);
	txt_didyouknow->SetFont(Font("Verdana", 14, Font::Bold));

	pos.x = 7;

	txt_tip		= new Text("", pos);

	pos.x = 7;
	pos.y = 38;
	size.cx = 330;
	size.cy = 184;

	layer_inner	= new Layer();
	layer_inner->SetMetrics(pos, size);
	layer_inner->SetBackgroundColor(Setup::TooltipColor);

	Add(dlgwnd);

	dlgwnd->Add(titlebar);
	dlgwnd->Add(divbar);

	dlgwnd->Add(btn_ok);
	dlgwnd->Add(btn_next);
	dlgwnd->Add(check_showtips);

	dlgwnd->Add(img_light);
	dlgwnd->Add(txt_didyouknow);

	dlgwnd->GetMainLayer()->Add(layer_inner);

	layer_inner->Add(txt_tip);

	dlgwnd->onPaint.Connect(&TipOfTheDay::Paint, this);
}

S::GUI::Dialogs::TipOfTheDay::~TipOfTheDay()
{
	DeleteObject(dlgwnd);

	DeleteObject(titlebar);
	DeleteObject(divbar);

	DeleteObject(btn_ok);
	DeleteObject(btn_next);
	DeleteObject(check_showtips);

	DeleteObject(img_light);
	DeleteObject(txt_didyouknow);

	DeleteObject(layer_inner);

	DeleteObject(txt_tip);
}

const Error &S::GUI::Dialogs::TipOfTheDay::ShowDialog()
{
	if (caption != NIL) dlgwnd->SetText(caption);

	ButtonNext();

	dlgwnd->Show();
	dlgwnd->Stay();

	return error;
}

S::Int S::GUI::Dialogs::TipOfTheDay::AddTip(const String &tip)
{
	tips.Add(tip);

	return Success();
}

S::Int S::GUI::Dialogs::TipOfTheDay::SetMode(Int nMode, Int nOffset, Bool showOnStartup)
{
	switch (nMode)
	{
		case TIP_ORDERED:
			mode	= nMode;
			offset	= nOffset;

			break;
		case TIP_RANDOM:
			mode	= nMode;
			offset	= 0;

			break;
		default:
			return Error();
	}

	*showTips = showOnStartup;

	return Success();
}

S::Int S::GUI::Dialogs::TipOfTheDay::GetOffset()
{
	return offset;
}

S::Void S::GUI::Dialogs::TipOfTheDay::ButtonOK()
{
	dlgwnd->Close();
}

S::Void S::GUI::Dialogs::TipOfTheDay::ButtonNext()
{
	String	 tip;
	time_t	 timer;

	switch (mode)
	{
		case TIP_ORDERED:
			tip = tips.GetNth(offset++);

			if (offset == tips.GetNOfEntries()) offset = 0;

			break;
		case TIP_RANDOM:
			srand((unsigned) time(&timer));

			tip = tips.GetNth(rand() % tips.GetNOfEntries());

			break;
	}

	txt_tip->SetText(tip);
}

S::Void S::GUI::Dialogs::TipOfTheDay::Paint()
{
	dlgwnd->GetDrawSurface()->Box(Rect(Point(7, 38) + dlgwnd->GetMainLayer()->GetPosition(), Size(330, 184)), 0, Rect::Outlined);
}
