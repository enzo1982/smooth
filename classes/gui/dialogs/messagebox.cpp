 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/messagebox.h>
#include <smooth/i18n/i18n.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/imageloader/imageloader.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/gui/widgets/basic/button.h>
#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/gui/widgets/basic/image.h>
#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/window/window.h>

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

S::GUI::Dialogs::MessageDlg::MessageDlg(const String &text, const String &title, Int btns, wchar_t *cIcon, const String &checkBoxText, Bool *iCVar)
{
	msgicon = cIcon;
	cVar = iCVar;

	msgbox		= new Window(title, Point(), Size());
	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	lay		= new Layer();

#ifdef __WIN32__
	icon		= new Image(ImageLoader::Load(String("Icon:").Append(String::FromInt((Int) cIcon))), Point(14, 19), Size(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON)));
#else
	icon		= NIL;
#endif

	okbutton	= NIL;
	yesbutton	= NIL;
	nobutton	= NIL;
	abortbutton	= NIL;
	cancelbutton	= NIL;
	retrybutton	= NIL;
	ignorebutton	= NIL;
	buttons		= btns;

	Int	 actpos = 0;
	Int	 thissize = 0;
	Int	 maxsize = 0;
	Int	 titlesize = 0;
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

#ifdef __WIN32__
	msgbox->SetSize(Size(maxsize + 36 + (msgicon != NIL ? GetSystemMetrics(SM_CXICON) + 20 : 0), (((Int) Math::Max(2, lines) + 1) * 16) + 70 + buttonHeight));
#endif

	titlesize = Font(Font::Default, Font::DefaultSize, Font::Bold).GetTextSizeX(title);

	msgbox->SetWidth(Math::Max(msgbox->GetWidth(), titlesize + 80));

	Add(msgbox);

	msgbox->Add(titlebar);
	msgbox->Add(lay);
	msgbox->Add(icon);

	msgbox->onPaint.Connect(&MessageDlg::MessagePaintProc, this);
	msgbox->doQuit.Connect(&MessageDlg::MessageKillProc, this);

	if (cVar != NIL)
	{
#ifdef __WIN32__
		checkbox = new CheckBox(checkBoxText, Point(13 + (msgicon != NIL ? GetSystemMetrics(SM_CXICON) + 20 : 0), 46 + buttonHeight), Size(100, 0), cVar);
		checkbox->SetOrientation(OR_LOWERLEFT);
		checkbox->SetWidth(checkbox->textSize.cx + 21);

		msgbox->SetWidth(Math::Max(msgbox->GetWidth(), checkbox->textSize.cx + 54 + (msgicon != NIL ? GetSystemMetrics(SM_CXICON) + 20 : 0)));
		msgbox->SetHeight(msgbox->GetHeight() + 22);
#endif

		lay->Add(checkbox);
	}

#ifdef __WIN32__
	switch (buttons)
	{
		default:
		case MB_OK:
			msgbox->SetWidth(Math::Max(msgbox->GetWidth(), buttonWidth + 30));

			okbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, Point((msgbox->GetWidth() - buttonWidth) / 2 - 3, 14 + buttonHeight), Size());
			okbutton->onAction.Connect(&MessageDlg::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);

			lay->Add(okbutton);

			break;
		case MB_OKCANCEL:
			msgbox->SetWidth(Math::Max(msgbox->GetWidth(), 2 * buttonWidth + 39));

			okbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, Point((msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3, 14 + buttonHeight), Size());
			okbutton->onAction.Connect(&MessageDlg::MessageOK, this);
			okbutton->SetOrientation(OR_LOWERLEFT);

			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, Point((msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3 + buttonWidth + 9, 14 + buttonHeight), Size());
			cancelbutton->onAction.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);

			lay->Add(okbutton);
			lay->Add(cancelbutton);

			break;
		case MB_YESNO:
			msgbox->SetWidth(Math::Max(msgbox->GetWidth(), 2 * buttonWidth + 39));

			yesbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Yes"), NIL, Point((msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3, 14 + buttonHeight), Size());
			yesbutton->onAction.Connect(&MessageDlg::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);

			nobutton = new Button(I18n::Translator::defaultTranslator->TranslateString("No"), NIL, Point((msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3 + buttonWidth + 9, 14 + buttonHeight), Size());
			nobutton->onAction.Connect(&MessageDlg::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);

			lay->Add(yesbutton);
			lay->Add(nobutton);

			break;
		case MB_YESNOCANCEL:
			msgbox->SetWidth(Math::Max(msgbox->GetWidth(), 3 * buttonWidth + 48));

			yesbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Yes"), NIL, Point((msgbox->GetWidth() - (3 * buttonWidth + 18)) / 2 - 3, 14 + buttonHeight), Size());
			yesbutton->onAction.Connect(&MessageDlg::MessageYes, this);
			yesbutton->SetOrientation(OR_LOWERLEFT);

			nobutton = new Button(I18n::Translator::defaultTranslator->TranslateString("No"), NIL, Point((msgbox->GetWidth() - (3 * buttonWidth + 18)) / 2 - 3 + buttonWidth + 9, 14 + buttonHeight), Size());
			nobutton->onAction.Connect(&MessageDlg::MessageNo, this);
			nobutton->SetOrientation(OR_LOWERLEFT);

			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, Point((msgbox->GetWidth() - (3 * buttonWidth + 18)) / 2 - 3 + 2 * buttonWidth + 18, 14 + buttonHeight), Size());
			cancelbutton->onAction.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);

			lay->Add(yesbutton);
			lay->Add(nobutton);
			lay->Add(cancelbutton);

			break;
		case MB_RETRYCANCEL:
			msgbox->SetWidth(Math::Max(msgbox->GetWidth(), 2 * buttonWidth + 39));

			retrybutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Retry"), NIL, Point((msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3, 14 + buttonHeight), Size());
			retrybutton->onAction.Connect(&MessageDlg::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);

			cancelbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, Point((msgbox->GetWidth() - (2 * buttonWidth + 9)) / 2 - 3 + buttonWidth + 9, 14 + buttonHeight), Size());
			cancelbutton->onAction.Connect(&MessageDlg::MessageCancel, this);
			cancelbutton->SetOrientation(OR_LOWERLEFT);

			lay->Add(retrybutton);
			lay->Add(cancelbutton);

			break;
		case MB_ABORTRETRYIGNORE:
			msgbox->SetWidth(Math::Max(msgbox->GetWidth(), 3 * buttonWidth + 48));

			abortbutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Abort"), NIL, Point((msgbox->GetWidth() - (3 * buttonWidth + 18)) / 2 - 3, 14 + buttonHeight), Size());
			abortbutton->onAction.Connect(&MessageDlg::MessageAbort, this);
			abortbutton->SetOrientation(OR_LOWERLEFT);

			retrybutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Retry"), NIL, Point((msgbox->GetWidth() - (3 * buttonWidth + 18)) / 2 - 3 + buttonWidth + 9, 14 + buttonHeight), Size());
			retrybutton->onAction.Connect(&MessageDlg::MessageRetry, this);
			retrybutton->SetOrientation(OR_LOWERLEFT);

			ignorebutton = new Button(I18n::Translator::defaultTranslator->TranslateString("Ignore"), NIL, Point((msgbox->GetWidth() - (3 * buttonWidth + 18)) / 2 - 3 + 2 * buttonWidth + 18, 14 + buttonHeight), Size());
			ignorebutton->onAction.Connect(&MessageDlg::MessageIgnore, this);
			ignorebutton->SetOrientation(OR_LOWERLEFT);

			lay->Add(abortbutton);
			lay->Add(retrybutton);
			lay->Add(ignorebutton);

			break;
	}
#endif

	msgbox->SetPosition(Point((LiSAGetDisplaySizeX() - msgbox->GetWidth()) / 2, (LiSAGetDisplaySizeY() - msgbox->GetHeight()) / 2) + Point(nOfMessageBoxes * 25, nOfMessageBoxes * 25));
}

S::GUI::Dialogs::MessageDlg::~MessageDlg()
{
#ifdef __WIN32__
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
#endif

	DeleteObject(lay);
	DeleteObject(icon);
	DeleteObject(titlebar);
	DeleteObject(msgbox);

	if (cVar != NIL) DeleteObject(checkbox);
}

const Error &S::GUI::Dialogs::MessageDlg::ShowDialog()
{
	nOfMessageBoxes++;

#ifdef __WIN32__
	MessageBeep(MB_ICONASTERISK);
#endif

	msgbox->Stay();

	return error;
}

S::Void S::GUI::Dialogs::MessageDlg::MessagePaintProc()
{
	Surface	*surface = msgbox->GetDrawSurface();
	Rect	 txtrect;

	txtrect.left = 18;
	txtrect.top = 48;
	txtrect.right = txtrect.left + msgbox->GetWidth() - 32;

	if (lines == 1 && msgicon != NIL) txtrect.top = 56;

	txtrect.bottom = txtrect.top + 16;

#ifdef __WIN32__
	if (msgicon != NIL) txtrect.left += GetSystemMetrics(SM_CXICON) + 20;
#endif

	for (int i = 0; i < lines; i++)
	{
		surface->SetText(line[i], txtrect, Font());

		txtrect.top += 16;
		txtrect.bottom += 16;
	}
}

S::Bool S::GUI::Dialogs::MessageDlg::MessageKillProc()
{
#ifdef __WIN32__
	if (buttonCode == 0) buttonCode = IDCLOSE;
#endif

	nOfMessageBoxes--;

	return True;
}

S::Void S::GUI::Dialogs::MessageDlg::MessageOK()
{
#ifdef __WIN32__
	buttonCode = IDOK;
#endif

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageCancel()
{
#ifdef __WIN32__
	buttonCode = IDCANCEL;
#endif

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageYes()
{
#ifdef __WIN32__
	buttonCode = IDYES;
#endif

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageNo()
{
#ifdef __WIN32__
	buttonCode = IDNO;
#endif

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageRetry()
{
#ifdef __WIN32__
	buttonCode = IDRETRY;
#endif

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageAbort()
{
#ifdef __WIN32__
	buttonCode = IDABORT;
#endif

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageIgnore()
{
#ifdef __WIN32__
	buttonCode = IDIGNORE;
#endif

	msgbox->Close();
}

S::Int S::GUI::Dialogs::MessageDlg::GetButtonCode()
{
	return buttonCode;
}
