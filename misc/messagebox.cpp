 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/messagebox.h>
#include <smooth/array.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/toolkit.h>
#include <smooth/button.h>
#include <smooth/titlebar.h>
#include <smooth/layer.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/objectproperties.h>
#include <smooth/surface.h>

using namespace smooth::GUI;

#if defined MessageBox
#undef MessageBox
#endif

S::Int S::MessageBoxApp::nOfMessageBoxes = 0;

S::Int S::SMOOTH::MessageBox(String text, String title, Int buttons, char *icon)
{
	Int		 rVal;
	MessageBoxApp	*app = new MessageBoxApp(text, title, buttons, (wchar_t *) icon);

	rVal = app->ShowMessageBox();

	DeleteObject(app);

	return rVal;
}

S::Int S::SMOOTH::MessageBox(String text, String title, Int buttons, wchar_t *icon)
{
	Int		 rVal;
	MessageBoxApp	*app = new MessageBoxApp(text, title, buttons, icon);

	rVal = app->ShowMessageBox();

	DeleteObject(app);

	return rVal;
}

S::MessageBoxApp::MessageBoxApp(String text, String title, Int btns, wchar_t *icon)
{
	msgicon = icon;

	Create(text, title, btns);
}

S::Void S::MessageBoxApp::Create(String text, String title, Int btns)
{
	msgbox		= new Window(title);
	titlebar	= new Titlebar(false, false, true);
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
	HDC	 dc = GetContext(NIL);
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
#ifdef __WIN32__
			thissize = GetTextSizeX(line[j], I18N_DEFAULTFONT, -MulDiv(I18N_DEFAULTFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_NORMAL);
#endif

			if (thissize > maxsize) maxsize = thissize;
		}
	}
	else
	{
		lines = 0;
		thissize = 0;
	}

	msgbox->GetObjectProperties()->size.cx = maxsize+34;
	msgbox->GetObjectProperties()->size.cy = ((lines+1)*16) + 68 + buttonHeight;

	if (lines == 1 && msgicon != NIL) msgbox->GetObjectProperties()->size.cy = 116 + buttonHeight;

#ifdef __WIN32__
	if (msgicon != NIL) msgbox->GetObjectProperties()->size.cx += GetSystemMetrics(SM_CXICON) + 20;

	titlesize = GetTextSizeX(title, I18N_DEFAULTFONT, -MulDiv(I18N_DEFAULTFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_BOLD);
#endif

	if (msgbox->GetObjectProperties()->size.cx < titlesize+80) msgbox->GetObjectProperties()->size.cx = titlesize+80;

	RegisterObject(msgbox);

	msgbox->RegisterObject(titlebar);
	msgbox->RegisterObject(lay);

	msgbox->onPaint.Connect(&MessageBoxApp::MessagePaintProc, this);
	msgbox->doQuit.Connect(&MessageBoxApp::MessageKillProc, this);

	bpos.y = 14 + buttonHeight;
	bsize.cy = 0;
	bsize.cx = 0;

	oldmeasurement = Setup::FontSize;

	SetMeasurement(SMT_PIXELS);

	switch (buttons)
	{
#ifdef __WIN32__
		default:
		case MB_OK:
			if (msgbox->GetObjectProperties()->size.cx < (buttonWidth + 30)) msgbox->GetObjectProperties()->size.cx = buttonWidth + 30;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - buttonWidth) / 2 - 3;
			okbutton = new Button(TXT_OK, NIL, bpos, bsize);
			okbutton->onClick.Connect(&MessageBoxApp::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			break;
		case MB_OKCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			okbutton = new Button(TXT_OK, NIL, bpos, bsize);
			okbutton->onClick.Connect(&MessageBoxApp::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(TXT_CANCEL, NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageBoxApp::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_YESNO:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			yesbutton = new Button(TXT_YES, NIL, bpos, bsize);
			yesbutton->onClick.Connect(&MessageBoxApp::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new Button(TXT_NO, NIL, bpos, bsize);
			nobutton->onClick.Connect(&MessageBoxApp::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			break;
		case MB_YESNOCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (3 * buttonWidth + 48)) msgbox->GetObjectProperties()->size.cx = 3 * buttonWidth + 48;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (3 * buttonWidth + 18)) / 2 - 3;
			yesbutton = new Button(TXT_YES, NIL, bpos, bsize);
			yesbutton->onClick.Connect(&MessageBoxApp::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new Button(TXT_NO, NIL, bpos, bsize);
			nobutton->onClick.Connect(&MessageBoxApp::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(TXT_CANCEL, NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageBoxApp::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_RETRYCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			retrybutton = new Button(TXT_RETRY, NIL, bpos, bsize);
			retrybutton->onClick.Connect(&MessageBoxApp::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new Button(TXT_CANCEL, NIL, bpos, bsize);
			cancelbutton->onClick.Connect(&MessageBoxApp::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_ABORTRETRYIGNORE:
			if (msgbox->GetObjectProperties()->size.cx < (3 * buttonWidth + 48)) msgbox->GetObjectProperties()->size.cx = 3 * buttonWidth + 48;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (3 * buttonWidth + 18)) / 2 - 3;
			abortbutton = new Button(TXT_ABORT, NIL, bpos, bsize);
			abortbutton->onClick.Connect(&MessageBoxApp::MessageAbort, this);
			abortbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			retrybutton = new Button(TXT_RETRY, NIL, bpos, bsize);
			retrybutton->onClick.Connect(&MessageBoxApp::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			ignorebutton = new Button(TXT_IGNORE, NIL, bpos, bsize);
			ignorebutton->onClick.Connect(&MessageBoxApp::MessageIgnore, this);
			ignorebutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(abortbutton);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(ignorebutton);
			break;
#endif
	}

	Setup::FontSize = oldmeasurement;

	msgbox->GetObjectProperties()->pos.x = (LiSAGetDisplaySizeX() - msgbox->GetObjectProperties()->size.cx) / 2 + (nOfMessageBoxes - 1) * 25;
	msgbox->GetObjectProperties()->pos.y = (LiSAGetDisplaySizeY() - msgbox->GetObjectProperties()->size.cy) / 2 + (nOfMessageBoxes - 1) * 25;

	FreeContext(NIL, dc);
}

S::MessageBoxApp::~MessageBoxApp()
{
	switch (buttons)
	{
#ifdef __WIN32__
		default:
		case MB_OK:
			lay->UnregisterObject(okbutton);

			DeleteObject(okbutton);

			break;
		case MB_OKCANCEL:
			lay->UnregisterObject(okbutton);
			lay->UnregisterObject(cancelbutton);

			DeleteObject(okbutton);
			DeleteObject(cancelbutton);

			break;
		case MB_YESNO:
			lay->UnregisterObject(yesbutton);
			lay->UnregisterObject(nobutton);

			DeleteObject(yesbutton);
			DeleteObject(nobutton);

			break;
		case MB_YESNOCANCEL:
			lay->UnregisterObject(yesbutton);
			lay->UnregisterObject(nobutton);
			lay->UnregisterObject(cancelbutton);

			DeleteObject(yesbutton);
			DeleteObject(nobutton);
			DeleteObject(cancelbutton);

			break;
		case MB_RETRYCANCEL:
			lay->UnregisterObject(retrybutton);
			lay->UnregisterObject(cancelbutton);

			DeleteObject(retrybutton);
			DeleteObject(cancelbutton);

			break;
		case MB_ABORTRETRYIGNORE:
			lay->UnregisterObject(abortbutton);
			lay->UnregisterObject(retrybutton);
			lay->UnregisterObject(ignorebutton);

			DeleteObject(abortbutton);
			DeleteObject(retrybutton);
			DeleteObject(ignorebutton);

			break;
#endif
	}

	msgbox->UnregisterObject(lay);
	msgbox->UnregisterObject(titlebar);

	UnregisterObject(msgbox);

	DeleteObject(lay);
	DeleteObject(titlebar);
	DeleteObject(msgbox);
}

S::Int S::MessageBoxApp::ShowMessageBox()
{
	int	 rval;

	nOfMessageBoxes++;

#ifdef __WIN32__
	MessageBeep(MB_ICONASTERISK);
#endif

	rval = msgbox->Stay();

	return rval;
}

S::Void S::MessageBoxApp::MessagePaintProc()
{
	Surface	*surface = msgbox->GetDrawSurface();
	HDC	 dc = GetContext(msgbox);
	Rect	 txtrect;

	txtrect.left = 17;
	txtrect.top = 47;
	txtrect.right = txtrect.left + msgbox->GetObjectProperties()->size.cx - 32;

	if (lines == 1 && msgicon != NIL) txtrect.top = 55;

	txtrect.bottom = txtrect.top + 16;

	if (msgicon != NIL)
	{
#ifdef __WIN32__
		HICON	 icon = NIL;

		txtrect.left += GetSystemMetrics(SM_CXICON)+20;

		if (msgicon == MAKEINTRESOURCEW(32512) || msgicon == MAKEINTRESOURCEW(32516) || msgicon == MAKEINTRESOURCEW(32515) || msgicon == MAKEINTRESOURCEW(32513) || msgicon == MAKEINTRESOURCEW(32514) || msgicon == MAKEINTRESOURCEW(32517))
		{
			if (Setup::enableUnicode)	icon = LoadIconW(NIL, msgicon);
			else				icon = LoadIconA(NIL, (char *) msgicon);
		}
		else
		{
			if (Setup::enableUnicode)	icon = LoadIconW(hInstance, msgicon);
			else				icon = LoadIconA(hInstance, (char *) msgicon);
		}

		DrawIcon(dc, 17, 47, icon);
		DestroyIcon(icon);
#endif
	}

	FreeContext(msgbox, dc);

	for (int i = 0; i < lines; i++)
	{
#ifdef __WIN32__
		surface->SetText(line[i], txtrect, I18N_DEFAULTFONT, -MulDiv(I18N_DEFAULTFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), Setup::TextColor, FW_NORMAL);
#endif

		txtrect.top += 16;
		txtrect.bottom += 16;
	}
}

S::Bool S::MessageBoxApp::MessageKillProc()
{
	if (msgbox->value == 0) msgbox->value = IDCLOSE;

	nOfMessageBoxes--;

	return True;
}

S::Void S::MessageBoxApp::MessageOK()
{
	msgbox->value = IDOK;
	msgbox->Close();
}

S::Void S::MessageBoxApp::MessageCancel()
{
	msgbox->value = IDCANCEL;
	msgbox->Close();
}

S::Void S::MessageBoxApp::MessageYes()
{
	msgbox->value = IDYES;
	msgbox->Close();
}

S::Void S::MessageBoxApp::MessageNo()
{
	msgbox->value = IDNO;
	msgbox->Close();
}

S::Void S::MessageBoxApp::MessageRetry()
{
	msgbox->value = IDRETRY;
	msgbox->Close();
}

S::Void S::MessageBoxApp::MessageAbort()
{
	msgbox->value = IDABORT;
	msgbox->Close();
}

S::Void S::MessageBoxApp::MessageIgnore()
{
	msgbox->value = IDIGNORE;
	msgbox->Close();
}
