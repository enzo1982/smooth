 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/messagebox.h>
#include <smooth/array.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/button.h>
#include <smooth/layer.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/window.h>
#include <smooth/titlebar.h>

using namespace smooth::GUI;

S::Int S::MessageDialog::nOfMessageBoxes = 0;

S::Int S::QuickMessage(String text, String title, Int buttons, char *icon)
{
	Int		 rVal;
	MessageDialog	*app = new MessageDialog(text, title, buttons, (wchar_t *) icon);

	rVal = app->ShowDialog();

	Object::DeleteObject(app);

	return rVal;
}

S::Int S::QuickMessage(String text, String title, Int buttons, wchar_t *icon)
{
	Int		 rVal;
	MessageDialog	*app = new MessageDialog(text, title, buttons, icon);

	rVal = app->ShowDialog();

	Object::DeleteObject(app);

	return rVal;
}

S::MessageDialog::MessageDialog(String text, String title, Int btns, wchar_t *icon)
{
	msgicon = icon;

	Create(text, title, btns);
}

S::Void S::MessageDialog::Create(String text, String title, Int btns)
{
	msgbox		= new Window(title);
	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	lay		= new Layer();
	okbutton	= NIL;
	yesbutton	= NIL;
	nobutton	= NIL;
	abortbutton	= NIL;
	cancelbutton	= NIL;
	retrybutton	= NIL;
	ignorebutton	= NIL;
	buttons		= btns;

	int	 actpos = 0;
	int	 thissize = 0;
	int	 maxsize = 0;
	Point	 bpos;
	Size	 bsize;
	int	 titlesize = 0;
	double	 oldmeasurement;
	Int	 buttonWidth = Math::Round(80 * Setup::FontSize);
	Int	 buttonHeight = Math::Round(22 * Setup::FontSize);

	lines = 1;
	msgbox->GetObjectProperties()->size.cx = 0;
	msgbox->GetObjectProperties()->size.cy = 0;

	if (text != NIL)
	{
		if (text[0] == 0) lines = 0;

		for (int i = 0; i < 256; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (text[actpos] != 10)
				{
					if (text[actpos] == 0) { line[i][j] = 0; break; }
					line[i][j] = text[actpos];
					actpos++;
				}
				else
				{
					lines++;
					line[i][j] = 0;
					actpos++;
					break;
				}
			}
		}

		for (int j = 0; j < lines; j++)
		{
			GUI::Font	 font;

			thissize = font.GetTextSizeX(line[j]);

			if (thissize > maxsize) maxsize = thissize;
		}
	}
	else
	{
		lines = 0;
		thissize = 0;
	}

	msgbox->GetObjectProperties()->size.cx = maxsize + 34;
	msgbox->GetObjectProperties()->size.cy = (((Int) Math::Max(2, lines) + 1) * 16) + 68 + buttonHeight;

	if (lines == 1 && msgicon != NIL) msgbox->GetObjectProperties()->size.cy = 116 + buttonHeight;

	if (msgicon != NIL) msgbox->GetObjectProperties()->size.cx += GetSystemMetrics(SM_CXICON) + 20;

	Font	 tbFont(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, 0, FW_BOLD);

	titlesize = tbFont.GetTextSizeX(title);

	if (msgbox->GetObjectProperties()->size.cx < titlesize + 80) msgbox->GetObjectProperties()->size.cx = titlesize + 80;

	RegisterObject(msgbox);

	msgbox->RegisterObject(titlebar);
	msgbox->RegisterObject(lay);

	msgbox->onPaint.Connect(&MessageDialog::MessagePaintProc, this);
	msgbox->doQuit.Connect(&MessageDialog::MessageKillProc, this);

	bpos.y = 14 + buttonHeight;
	bsize.cy = 0;
	bsize.cx = 0;

	oldmeasurement = Setup::FontSize;

	SetMeasurement(SMT_PIXELS);

	switch (buttons)
	{
		default:
		case MB_OK:
			if (msgbox->GetObjectProperties()->size.cx < (buttonWidth + 30)) msgbox->GetObjectProperties()->size.cx = buttonWidth + 30;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - buttonWidth) / 2 - 3;
			okbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, bpos, bsize);
			okbutton->onClick.Connect(&MessageDialog::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			break;
		case MB_OKCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			okbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, bpos, bsize);
			okbutton->onClick.Connect(&MessageDialog::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageDialog::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_YESNO:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			yesbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Yes"), NIL, bpos, bsize);
			yesbutton->onClick.Connect(&MessageDialog::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new Button(I18n::Translator::defaultTranslator->TranslateString("No"), NIL, bpos, bsize);
			nobutton->onClick.Connect(&MessageDialog::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			break;
		case MB_YESNOCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (3 * buttonWidth + 48)) msgbox->GetObjectProperties()->size.cx = 3 * buttonWidth + 48;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (3 * buttonWidth + 18)) / 2 - 3;
			yesbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Yes"), NIL, bpos, bsize);
			yesbutton->onClick.Connect(&MessageDialog::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new Button(I18n::Translator::defaultTranslator->TranslateString("No"), NIL, bpos, bsize);
			nobutton->onClick.Connect(&MessageDialog::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageDialog::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_RETRYCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			retrybutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Retry"), NIL, bpos, bsize);
			retrybutton->onClick.Connect(&MessageDialog::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageDialog::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_ABORTRETRYIGNORE:
			if (msgbox->GetObjectProperties()->size.cx < (3 * buttonWidth + 48)) msgbox->GetObjectProperties()->size.cx = 3 * buttonWidth + 48;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (3 * buttonWidth + 18)) / 2 - 3;
			abortbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Abort"), NIL, bpos, bsize);
			abortbutton->onClick.Connect(&MessageDialog::MessageAbort, this);
			abortbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			retrybutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Retry"), NIL, bpos, bsize);
			retrybutton->onClick.Connect(&MessageDialog::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			ignorebutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Ignore"), NIL, bpos, bsize);
			ignorebutton->onClick.Connect(&MessageDialog::MessageIgnore, this);
			ignorebutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(abortbutton);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(ignorebutton);
			break;
	}

	Setup::FontSize = oldmeasurement;

	msgbox->GetObjectProperties()->pos.x = (LiSAGetDisplaySizeX() - msgbox->GetObjectProperties()->size.cx) / 2 + (nOfMessageBoxes - 1) * 25;
	msgbox->GetObjectProperties()->pos.y = (LiSAGetDisplaySizeY() - msgbox->GetObjectProperties()->size.cy) / 2 + (nOfMessageBoxes - 1) * 25;
}

S::MessageDialog::~MessageDialog()
{
	switch (buttons)
	{
		default:
		case MB_OK:
			DeleteObject(okbutton);

			break;
		case MB_OKCANCEL:
			DeleteObject(okbutton);
			DeleteObject(cancelbutton);

			break;
		case MB_YESNO:
			DeleteObject(yesbutton);
			DeleteObject(nobutton);

			break;
		case MB_YESNOCANCEL:
			DeleteObject(yesbutton);
			DeleteObject(nobutton);
			DeleteObject(cancelbutton);

			break;
		case MB_RETRYCANCEL:
			DeleteObject(retrybutton);
			DeleteObject(cancelbutton);

			break;
		case MB_ABORTRETRYIGNORE:
			DeleteObject(abortbutton);
			DeleteObject(retrybutton);
			DeleteObject(ignorebutton);

			break;
	}

	DeleteObject(lay);
	DeleteObject(titlebar);
	DeleteObject(msgbox);
}

S::Int S::MessageDialog::ShowDialog()
{
	int	 rval;

	nOfMessageBoxes++;

	MessageBeep(MB_ICONASTERISK);

	rval = msgbox->Stay();

	return rval;
}

S::Void S::MessageDialog::MessagePaintProc()
{
	Surface	*surface = msgbox->GetDrawSurface();
	HDC	 dc = (HDC) surface->GetContext();
	Rect	 txtrect;

	txtrect.left = 17;
	txtrect.top = 47;
	txtrect.right = txtrect.left + msgbox->GetObjectProperties()->size.cx - 32;

	if (lines == 1 && msgicon != NIL) txtrect.top = 55;

	txtrect.bottom = txtrect.top + 16;

	if (msgicon != NIL)
	{
		HICON	 icon = NIL;

		txtrect.left += GetSystemMetrics(SM_CXICON) + 20;

		if (msgicon == MAKEINTRESOURCEW(32512) || msgicon == MAKEINTRESOURCEW(32516) || msgicon == MAKEINTRESOURCEW(32515) || msgicon == MAKEINTRESOURCEW(32513) || msgicon == MAKEINTRESOURCEW(32514) || msgicon == MAKEINTRESOURCEW(32517))
		{
			if (Setup::enableUnicode)	icon = (HICON) LoadImageW(NIL, msgicon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
			else				icon = (HICON) LoadImageA(NIL, (char *) msgicon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		}
		else
		{
			if (Setup::enableUnicode)	icon = (HICON) LoadImageW(hInstance, msgicon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
			else				icon = (HICON) LoadImageA(hInstance, (char *) msgicon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		}

		DrawIcon(dc, Setup::rightToLeft ? msgbox->GetObjectProperties()->size.cx - 17 - GetSystemMetrics(SM_CXICON) : 17, 47, icon);
	}

	for (int i = 0; i < lines; i++)
	{
		surface->SetText(line[i], txtrect, Font());

		txtrect.top += 16;
		txtrect.bottom += 16;
	}
}

S::Bool S::MessageDialog::MessageKillProc()
{
	if (msgbox->value == 0) msgbox->value = IDCLOSE;

	nOfMessageBoxes--;

	return True;
}

S::Void S::MessageDialog::MessageOK()
{
	msgbox->value = IDOK;
	msgbox->Close();
}

S::Void S::MessageDialog::MessageCancel()
{
	msgbox->value = IDCANCEL;
	msgbox->Close();
}

S::Void S::MessageDialog::MessageYes()
{
	msgbox->value = IDYES;
	msgbox->Close();
}

S::Void S::MessageDialog::MessageNo()
{
	msgbox->value = IDNO;
	msgbox->Close();
}

S::Void S::MessageDialog::MessageRetry()
{
	msgbox->value = IDRETRY;
	msgbox->Close();
}

S::Void S::MessageDialog::MessageAbort()
{
	msgbox->value = IDABORT;
	msgbox->Close();
}

S::Void S::MessageDialog::MessageIgnore()
{
	msgbox->value = IDIGNORE;
	msgbox->Close();
}
