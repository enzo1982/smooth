 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_MESSAGEBOX_
#define __OBJSMOOTH_MESSAGEBOX_

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
#include <smooth/mathtools.h>
#include <smooth/objectproperties.h>

int SMOOTHMessageBoxApp::nOfMessageBoxes = 0;

SMOOTHInt SMOOTH::MessageBox(SMOOTHString text, SMOOTHString title, SMOOTHInt buttons, char *icon)
{
	SMOOTHInt		 rVal;
	SMOOTHMessageBoxApp	*app = new SMOOTHMessageBoxApp(text, title, buttons, (wchar_t *) icon);

	rVal = app->ShowMessageBox();

	SMOOTH::DeleteObject(app);

	return rVal;
}

SMOOTHInt SMOOTH::MessageBox(SMOOTHString text, SMOOTHString title, SMOOTHInt buttons, wchar_t *icon)
{
	SMOOTHInt		 rVal;
	SMOOTHMessageBoxApp	*app = new SMOOTHMessageBoxApp(text, title, buttons, icon);

	rVal = app->ShowMessageBox();

	SMOOTH::DeleteObject(app);

	return rVal;
}

SMOOTHMessageBoxApp::SMOOTHMessageBoxApp(SMOOTHString text, SMOOTHString title, int btns, wchar_t *icon)
{
	msgicon = icon;

	Create(text, title, btns);
}

void SMOOTHMessageBoxApp::Create(SMOOTHString text, SMOOTHString title, int btns)
{
	msgbox		= new SMOOTHWindow(title);
	titlebar	= new SMOOTHTitlebar(false, false, true);
	lay		= new SMOOTHLayer();
	okbutton	= NIL;
	yesbutton	= NIL;
	nobutton	= NIL;
	abortbutton	= NIL;
	cancelbutton	= NIL;
	retrybutton	= NIL;
	ignorebutton	= NIL;
	buttons		= btns;

	int		 actpos = 0;
	int		 thissize = 0;
	int		 maxsize = 0;
	HDC		 dc = GetContext(NIL);
	SMOOTHPoint	 bpos;
	SMOOTHSize	 bsize;
	int		 titlesize = 0;
	double		 oldmeasurement;
	SMOOTHInt	 buttonWidth = roundtoint(80 * SMOOTH::Setup::FontSize);
	SMOOTHInt	 buttonHeight = roundtoint(22 * SMOOTH::Setup::FontSize);

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
			thissize = GetTextSizeX(dc, line[j], I18N_DEFAULTFONT, -MulDiv(I18N_DEFAULTFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_NORMAL);
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

	titlesize = GetTextSizeX(dc, title, I18N_DEFAULTFONT, -MulDiv(I18N_DEFAULTFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_BOLD);
#endif

	if (msgbox->GetObjectProperties()->size.cx < titlesize+80) msgbox->GetObjectProperties()->size.cx = titlesize+80;

	RegisterObject(msgbox);

	msgbox->RegisterObject(titlebar);
	msgbox->RegisterObject(lay);

	msgbox->SetPaintProc(SMOOTHProc(SMOOTHMessageBoxApp, this, MessagePaintProc));
	msgbox->SetKillProc(SMOOTHKillProc(SMOOTHMessageBoxApp, this, MessageKillProc));

	bpos.y = 14 + buttonHeight;
	bsize.cy = 0;
	bsize.cx = 0;

	oldmeasurement = SMOOTH::Setup::FontSize;

	SMOOTHSetMeasurement(SMT_PIXELS);

	switch (buttons)
	{
#ifdef __WIN32__
		default:
		case MB_OK:
			if (msgbox->GetObjectProperties()->size.cx < (buttonWidth + 30)) msgbox->GetObjectProperties()->size.cx = buttonWidth + 30;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - buttonWidth) / 2 - 3;
			okbutton = new SMOOTHButton(TXT_OK, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageOK));
			okbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			break;
		case MB_OKCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			okbutton = new SMOOTHButton(TXT_OK, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageOK));
			okbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new SMOOTHButton(TXT_CANCEL, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageCancel));
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(okbutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_YESNO:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			yesbutton = new SMOOTHButton(TXT_YES, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageYes));
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new SMOOTHButton(TXT_NO, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageNo));
			nobutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			break;
		case MB_YESNOCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (3 * buttonWidth + 48)) msgbox->GetObjectProperties()->size.cx = 3 * buttonWidth + 48;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (3 * buttonWidth + 18)) / 2 - 3;
			yesbutton = new SMOOTHButton(TXT_YES, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageYes));
			yesbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			nobutton = new SMOOTHButton(TXT_NO, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageNo));
			nobutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new SMOOTHButton(TXT_CANCEL, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageCancel));
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(yesbutton);
			lay->RegisterObject(nobutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_RETRYCANCEL:
			if (msgbox->GetObjectProperties()->size.cx < (2 * buttonWidth + 39)) msgbox->GetObjectProperties()->size.cx = 2 * buttonWidth + 39;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (2 * buttonWidth + 9)) / 2 - 3;
			retrybutton = new SMOOTHButton(TXT_RETRY, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageRetry));
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			cancelbutton = new SMOOTHButton(TXT_CANCEL, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageCancel));
			cancelbutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(cancelbutton);
			break;
		case MB_ABORTRETRYIGNORE:
			if (msgbox->GetObjectProperties()->size.cx < (3 * buttonWidth + 48)) msgbox->GetObjectProperties()->size.cx = 3 * buttonWidth + 48;
			bpos.x = (msgbox->GetObjectProperties()->size.cx - (3 * buttonWidth + 18)) / 2 - 3;
			abortbutton = new SMOOTHButton(TXT_ABORT, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageAbort));
			abortbutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			retrybutton = new SMOOTHButton(TXT_RETRY, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageRetry));
			retrybutton->SetOrientation(OR_LOWERLEFT);
			bpos.x += buttonWidth + 9;
			ignorebutton = new SMOOTHButton(TXT_IGNORE, NIL, bpos, bsize, SMOOTHProc(SMOOTHMessageBoxApp, this, MessageIgnore));
			ignorebutton->SetOrientation(OR_LOWERLEFT);
			lay->RegisterObject(abortbutton);
			lay->RegisterObject(retrybutton);
			lay->RegisterObject(ignorebutton);
			break;
#endif
	}

	SMOOTH::Setup::FontSize = oldmeasurement;

	msgbox->GetObjectProperties()->pos.x = (LiSAGetDisplaySizeX() - msgbox->GetObjectProperties()->size.cx) / 2 + (nOfMessageBoxes - 1) * 25;
	msgbox->GetObjectProperties()->pos.y = (LiSAGetDisplaySizeY() - msgbox->GetObjectProperties()->size.cy) / 2 + (nOfMessageBoxes - 1) * 25;

	FreeContext(NIL, dc);
}

SMOOTHMessageBoxApp::~SMOOTHMessageBoxApp()
{
	switch (buttons)
	{
#ifdef __WIN32__
		default:
		case MB_OK:
			lay->UnregisterObject(okbutton);

			SMOOTH::DeleteObject(okbutton);

			break;
		case MB_OKCANCEL:
			lay->UnregisterObject(okbutton);
			lay->UnregisterObject(cancelbutton);

			SMOOTH::DeleteObject(okbutton);
			SMOOTH::DeleteObject(cancelbutton);

			break;
		case MB_YESNO:
			lay->UnregisterObject(yesbutton);
			lay->UnregisterObject(nobutton);

			SMOOTH::DeleteObject(yesbutton);
			SMOOTH::DeleteObject(nobutton);

			break;
		case MB_YESNOCANCEL:
			lay->UnregisterObject(yesbutton);
			lay->UnregisterObject(nobutton);
			lay->UnregisterObject(cancelbutton);

			SMOOTH::DeleteObject(yesbutton);
			SMOOTH::DeleteObject(nobutton);
			SMOOTH::DeleteObject(cancelbutton);

			break;
		case MB_RETRYCANCEL:
			lay->UnregisterObject(retrybutton);
			lay->UnregisterObject(cancelbutton);

			SMOOTH::DeleteObject(retrybutton);
			SMOOTH::DeleteObject(cancelbutton);

			break;
		case MB_ABORTRETRYIGNORE:
			lay->UnregisterObject(abortbutton);
			lay->UnregisterObject(retrybutton);
			lay->UnregisterObject(ignorebutton);

			SMOOTH::DeleteObject(abortbutton);
			SMOOTH::DeleteObject(retrybutton);
			SMOOTH::DeleteObject(ignorebutton);

			break;
#endif
	}

	msgbox->UnregisterObject(lay);
	msgbox->UnregisterObject(titlebar);

	UnregisterObject(msgbox);

	SMOOTH::DeleteObject(lay);
	SMOOTH::DeleteObject(titlebar);
	SMOOTH::DeleteObject(msgbox);
}

int SMOOTHMessageBoxApp::ShowMessageBox()
{
	int	 rval;

	nOfMessageBoxes++;

#ifdef __WIN32__
	MessageBeep(MB_ICONASTERISK);
#endif

	rval = msgbox->Stay();

	return rval;
}

void SMOOTHMessageBoxApp::MessagePaintProc()
{
	HDC		 dc = GetContext(msgbox);
	SMOOTHRect	 txtrect;

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
			if (SMOOTH::Setup::enableUnicode)	icon = LoadIconW(NIL, msgicon);
			else					icon = LoadIconA(NIL, (char *) msgicon);
		}
		else
		{
			if (SMOOTH::Setup::enableUnicode)	icon = LoadIconW(hInstance, msgicon);
			else					icon = LoadIconA(hInstance, (char *) msgicon);
		}

		DrawIcon(dc, 17, 47, icon);
		DestroyIcon(icon);
#endif
	}

	for (int i = 0; i < lines; i++)
	{
#ifdef __WIN32__
		::SetText(dc, line[i], txtrect, I18N_DEFAULTFONT, -MulDiv(I18N_DEFAULTFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), SMOOTH::Setup::TextColor, FW_NORMAL);
#endif

		txtrect.top += 16;
		txtrect.bottom += 16;
	}

	FreeContext(msgbox, dc);
}

bool SMOOTHMessageBoxApp::MessageKillProc()
{
#ifdef __WIN32__
	if (msgbox->value == 0) msgbox->value = IDCLOSE;
#endif

	nOfMessageBoxes--;

	return true;
}

void SMOOTHMessageBoxApp::MessageOK()
{
#ifdef __WIN32__
	msgbox->value = IDOK;
#endif

	SMOOTH::CloseWindow(msgbox);
}

void SMOOTHMessageBoxApp::MessageCancel()
{
#ifdef __WIN32__
	msgbox->value = IDCANCEL;
#endif

	SMOOTH::CloseWindow(msgbox);
}

void SMOOTHMessageBoxApp::MessageYes()
{
#ifdef __WIN32__
	msgbox->value = IDYES;
#endif

	SMOOTH::CloseWindow(msgbox);
}

void SMOOTHMessageBoxApp::MessageNo()
{
#ifdef __WIN32__
	msgbox->value = IDNO;
#endif

	SMOOTH::CloseWindow(msgbox);
}

void SMOOTHMessageBoxApp::MessageRetry()
{
#ifdef __WIN32__
	msgbox->value = IDRETRY;
#endif

	SMOOTH::CloseWindow(msgbox);
}

void SMOOTHMessageBoxApp::MessageAbort()
{
#ifdef __WIN32__
	msgbox->value = IDABORT;
#endif

	SMOOTH::CloseWindow(msgbox);
}

void SMOOTHMessageBoxApp::MessageIgnore()
{
#ifdef __WIN32__
	msgbox->value = IDIGNORE;
#endif

	SMOOTH::CloseWindow(msgbox);
}

#endif
