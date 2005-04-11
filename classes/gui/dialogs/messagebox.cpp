 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/messagebox.h>
#include <smooth/array.h>
#include <smooth/misc/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/gui/widgets/basic/button.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/widgets/basic/checkbox.h>

S::Int S::GUI::Dialogs::MessageDlg::nOfMessageBoxes = 0;

S::Int S::GUI::Dialogs::QuickMessage(String text, String title, Int buttons, char *icon)
{
	Int		 rVal;
	MessageDlg	*app = new MessageDlg(text, title, buttons, (wchar_t *) icon);

	rVal = app->ShowDialog();

	Object::DeleteObject(app);

	return rVal;
}

S::Int S::GUI::Dialogs::QuickMessage(String text, String title, Int buttons, wchar_t *icon)
{
	Int		 rVal;
	MessageDlg	*app = new MessageDlg(text, title, buttons, icon);

	rVal = app->ShowDialog();

	Object::DeleteObject(app);

	return rVal;
}

S::GUI::Dialogs::MessageDlg::MessageDlg(String text, String title, Int btns, wchar_t *icon, String checkBoxText, Bool *iCVar)
{
	msgicon = icon;
	cVar = iCVar;

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
	Int	 buttonWidth = 80;
	Int	 buttonHeight = 22;

	lines = 1;
	msgbox->size.cx = 0;
	msgbox->size.cy = 0;

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

	msgbox->size.cx = maxsize + 34;
	msgbox->size.cy = (((Int) Math::Max(2, lines) + 1) * 16) + 68 + buttonHeight;

	if (lines == 1 && msgicon != NIL) msgbox->size.cy = 116 + buttonHeight;

	if (msgicon != NIL) msgbox->size.cx += GetSystemMetrics(SM_CXICON) + 20;

	Font	 tbFont(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, 0, FW_BOLD);

	titlesize = tbFont.GetTextSizeX(title);

	if (msgbox->size.cx < titlesize + 80) msgbox->size.cx = titlesize + 80;

	RegisterObject(msgbox);

	msgbox->RegisterObject(titlebar);
	msgbox->RegisterObject(lay);

	msgbox->onPaint.Connect(&MessageDlg::MessagePaintProc, this);
	msgbox->doQuit.Connect(&MessageDlg::MessageKillProc, this);

	bpos.y = 14 + buttonHeight;
	bsize.cy = 0;
	bsize.cx = 0;

	switch (buttons)
	{
		default:
		case MB_OK:
			if (msgbox->size.cx < (buttonWidth + 30)) msgbox->size.cx = buttonWidth + 30;
			bpos.x = (msgbox->size.cx - buttonWidth) / 2 - 3;
			okbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, bpos, bsize);
			okbutton->onClick.Connect(&MessageDlg::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			break;
		case MB_OKCANCEL:
			if (msgbox->size.cx < (2 * buttonWidth + 39)) msgbox->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			okbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, bpos, bsize);
			okbutton->onClick.Connect(&MessageDlg::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_YESNO:
			if (msgbox->size.cx < (2 * buttonWidth + 39)) msgbox->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			yesbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Yes"), NIL, bpos, bsize);
			yesbutton->onClick.Connect(&MessageDlg::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new Button(I18n::Translator::defaultTranslator->TranslateString("No"), NIL, bpos, bsize);
			nobutton->onClick.Connect(&MessageDlg::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			break;
		case MB_YESNOCANCEL:
			if (msgbox->size.cx < (3 * buttonWidth + 48)) msgbox->size.cx = 3 * buttonWidth + 48;
			bpos.x = (msgbox->size.cx - (3 * buttonWidth + 18)) / 2 - 3;
			yesbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Yes"), NIL, bpos, bsize);
			yesbutton->onClick.Connect(&MessageDlg::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new Button(I18n::Translator::defaultTranslator->TranslateString("No"), NIL, bpos, bsize);
			nobutton->onClick.Connect(&MessageDlg::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_RETRYCANCEL:
			if (msgbox->size.cx < (2 * buttonWidth + 39)) msgbox->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			retrybutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Retry"), NIL, bpos, bsize);
			retrybutton->onClick.Connect(&MessageDlg::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_ABORTRETRYIGNORE:
			if (msgbox->size.cx < (3 * buttonWidth + 48)) msgbox->size.cx = 3 * buttonWidth + 48;
			bpos.x = (msgbox->size.cx - (3 * buttonWidth + 18)) / 2 - 3;
			abortbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Abort"), NIL, bpos, bsize);
			abortbutton->onClick.Connect(&MessageDlg::MessageAbort, this);
			abortbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			retrybutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Retry"), NIL, bpos, bsize);
			retrybutton->onClick.Connect(&MessageDlg::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			ignorebutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Ignore"), NIL, bpos, bsize);
			ignorebutton->onClick.Connect(&MessageDlg::MessageIgnore, this);
			ignorebutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(abortbutton);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(ignorebutton);
			break;
	}

	if (cVar != NIL)
	{
		Point	 pos(13, 46 + buttonHeight);
		Size	 size(100, 0);

		if (msgicon != NIL) pos.x += GetSystemMetrics(SM_CXICON) + 20;

		checkbox = new CheckBox(checkBoxText, pos, size, cVar);
		checkbox->SetOrientation(OR_LOWERLEFT);
		checkbox->SetMetrics(checkbox->pos, Size(checkbox->textSize.cx + 21, checkbox->size.cy));

		msgbox->size.cy += 22;

		lay->RegisterObject(checkbox);
	}

	msgbox->pos.x = (LiSAGetDisplaySizeX() - msgbox->size.cx) / 2 + (nOfMessageBoxes - 1) * 25;
	msgbox->pos.y = (LiSAGetDisplaySizeY() - msgbox->size.cy) / 2 + (nOfMessageBoxes - 1) * 25;
}

S::GUI::Dialogs::MessageDlg::~MessageDlg()
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

	if (cVar != NIL) DeleteObject(checkbox);
}

S::Int S::GUI::Dialogs::MessageDlg::ShowDialog()
{
	nOfMessageBoxes++;

	MessageBeep(MB_ICONASTERISK);

	return msgbox->Stay();
}

S::Void S::GUI::Dialogs::MessageDlg::MessagePaintProc()
{
	Surface	*surface = msgbox->GetDrawSurface();
	HDC	 dc = (HDC) surface->GetSystemSurface();
	Rect	 txtrect;

	txtrect.left = 17;
	txtrect.top = 47;
	txtrect.right = txtrect.left + msgbox->size.cx - 32;

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

		DrawIcon(dc, Setup::rightToLeft ? msgbox->size.cx - 17 - GetSystemMetrics(SM_CXICON) : 17, 47, icon);
	}

	for (int i = 0; i < lines; i++)
	{
		surface->SetText(line[i], txtrect, Font());

		txtrect.top += 16;
		txtrect.bottom += 16;
	}
}

S::Bool S::GUI::Dialogs::MessageDlg::MessageKillProc()
{
	if (msgbox->value == 0) msgbox->value = IDCLOSE;

	nOfMessageBoxes--;

	return True;
}

S::Void S::GUI::Dialogs::MessageDlg::MessageOK()
{
	msgbox->value = IDOK;
	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageCancel()
{
	msgbox->value = IDCANCEL;
	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageYes()
{
	msgbox->value = IDYES;
	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageNo()
{
	msgbox->value = IDNO;
	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageRetry()
{
	msgbox->value = IDRETRY;
	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageAbort()
{
	msgbox->value = IDABORT;
	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageIgnore()
{
	msgbox->value = IDIGNORE;
	msgbox->Close();
}
