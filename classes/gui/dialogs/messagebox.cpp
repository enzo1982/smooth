 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/messagebox.h>
#include <smooth/i18n/i18n.h>
#include <smooth/gui/widgets/basic/button.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/widgets/basic/checkbox.h>

S::Int S::GUI::Dialogs::MessageDlg::nOfMessageBoxes = 0;

S::Int S::GUI::Dialogs::QuickMessage(const String &text, const String &title, Int buttons, char *icon)
{
	MessageDlg	*app = new MessageDlg(text, title, buttons, (wchar_t *) icon);

	app->ShowDialog();

	Int		 rVal = app->GetButtonCode();

	Object::DeleteObject(app);

	return rVal;
}

S::Int S::GUI::Dialogs::QuickMessage(const String &text, const String &title, Int buttons, wchar_t *icon)
{
	MessageDlg	*app = new MessageDlg(text, title, buttons, icon);

	app->ShowDialog();

	Int		 rVal = app->GetButtonCode();

	Object::DeleteObject(app);

	return rVal;
}

S::GUI::Dialogs::MessageDlg::MessageDlg(const String &text, const String &title, Int btns, wchar_t *icon, const String &checkBoxText, Bool *iCVar)
{
	msgicon = icon;
	cVar = iCVar;

	msgbox		= new Window(title, Point(), Size());
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

	msgbox->SetSize(Size(maxsize + 36 + (msgicon != NIL ? GetSystemMetrics(SM_CXICON) + 20 : 0), (((Int) Math::Max(2, lines) + 1) * 16) + 70 + buttonHeight));

	Font	 tbFont(Font::Default, Font::DefaultSize, Font::Bold);

	titlesize = tbFont.GetTextSizeX(title);

	if (msgbox->GetWidth() < titlesize + 80) msgbox->SetWidth(titlesize + 80);

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
			if (msgbox->GetWidth() < (buttonWidth + 30)) msgbox->SetWidth(buttonWidth + 30);
			bpos.x = (msgbox->GetWidth() - buttonWidth) / 2 - 3;
			okbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, bpos, bsize);
			okbutton->onAction.Connect(&MessageDlg::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			break;
		case MB_OKCANCEL:
			if (msgbox->GetWidth() < (2 * buttonWidth + 39)) msgbox->SetWidth(2 * buttonWidth + 39);
			bpos.x = (msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3;
			okbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, bpos, bsize);
			okbutton->onAction.Connect(&MessageDlg::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onAction.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_YESNO:
			if (msgbox->GetWidth() < (2 * buttonWidth + 39)) msgbox->SetWidth(2 * buttonWidth + 39);
			bpos.x = (msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3;
			yesbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Yes"), NIL, bpos, bsize);
			yesbutton->onAction.Connect(&MessageDlg::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new Button(I18n::Translator::defaultTranslator->TranslateString("No"), NIL, bpos, bsize);
			nobutton->onAction.Connect(&MessageDlg::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			break;
		case MB_YESNOCANCEL:
			if (msgbox->GetWidth() < (3 * buttonWidth + 48)) msgbox->SetWidth(3 * buttonWidth + 48);
			bpos.x = (msgbox->GetWidth() - (3 * buttonWidth + 18)) / 2 - 3;
			yesbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Yes"), NIL, bpos, bsize);
			yesbutton->onAction.Connect(&MessageDlg::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new Button(I18n::Translator::defaultTranslator->TranslateString("No"), NIL, bpos, bsize);
			nobutton->onAction.Connect(&MessageDlg::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onAction.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_RETRYCANCEL:
			if (msgbox->GetWidth() < (2 * buttonWidth + 39)) msgbox->SetWidth(2 * buttonWidth + 39);
			bpos.x = (msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3;
			retrybutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Retry"), NIL, bpos, bsize);
			retrybutton->onAction.Connect(&MessageDlg::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bpos, bsize);
			cancelbutton->onAction.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_ABORTRETRYIGNORE:
			if (msgbox->GetWidth() < (3 * buttonWidth + 48)) msgbox->SetWidth(3 * buttonWidth + 48);
			bpos.x = (msgbox->GetWidth() - (3 * buttonWidth + 18)) / 2 - 3;
			abortbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Abort"), NIL, bpos, bsize);
			abortbutton->onAction.Connect(&MessageDlg::MessageAbort, this);
			abortbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			retrybutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Retry"), NIL, bpos, bsize);
			retrybutton->onAction.Connect(&MessageDlg::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			ignorebutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Ignore"), NIL, bpos, bsize);
			ignorebutton->onAction.Connect(&MessageDlg::MessageIgnore, this);
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
		checkbox->SetWidth(checkbox->textSize.cx + 21);

		msgbox->SetHeight(msgbox->GetHeight() + 22);

		lay->RegisterObject(checkbox);
	}

	msgbox->SetPosition(Point((LiSAGetDisplaySizeX() - msgbox->GetWidth()) / 2, (LiSAGetDisplaySizeY() - msgbox->GetHeight()) / 2) + Point(nOfMessageBoxes * 25, nOfMessageBoxes * 25));
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

const Error &S::GUI::Dialogs::MessageDlg::ShowDialog()
{
	nOfMessageBoxes++;

	MessageBeep(MB_ICONASTERISK);

	msgbox->Stay();

	return error;
}

S::Void S::GUI::Dialogs::MessageDlg::MessagePaintProc()
{
	Surface	*surface = msgbox->GetDrawSurface();
	Rect	 txtrect;

	txtrect.left = 17;
	txtrect.top = 47;
	txtrect.right = txtrect.left + msgbox->GetWidth() - 32;

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

		HWND	 window	= (HWND) surface->GetSystemSurface();
		HDC	 dc	= GetWindowDC(window);

		DrawIcon(dc, msgbox->IsRightToLeft() ? msgbox->GetWidth() - 17 - GetSystemMetrics(SM_CXICON) : 17, 47, icon);

		ReleaseDC(window, dc);
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
	if (buttonCode == 0) buttonCode = IDCLOSE;

	nOfMessageBoxes--;

	return True;
}

S::Void S::GUI::Dialogs::MessageDlg::MessageOK()
{
	buttonCode = IDOK;

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageCancel()
{
	buttonCode = IDCANCEL;

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageYes()
{
	buttonCode = IDYES;

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageNo()
{
	buttonCode = IDNO;

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageRetry()
{
	buttonCode = IDRETRY;

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageAbort()
{
	buttonCode = IDABORT;

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageIgnore()
{
	buttonCode = IDIGNORE;

	msgbox->Close();
}

S::Int S::GUI::Dialogs::MessageDlg::GetButtonCode()
{
	return buttonCode;
}
