 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/messagebox.h>
#include <smooth/i18n/translator.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/imageloader/imageloader.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/gui/widgets/basic/button.h>
#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/gui/widgets/basic/image.h>
#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/window/window.h>

S::Int	 S::GUI::Dialogs::MessageDlg::nOfMessageBoxes = 0;
S::Bool	 S::GUI::Dialogs::MessageDlg::defaultRightToLeft = False;

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
	msgbox->SetRightToLeft(defaultRightToLeft);

	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	lay		= new Layer();

#ifdef __WIN32__
	icon		= new Image(ImageLoader::Load(String("Icon:").Append(String::FromInt((Int64) cIcon))), Point(14, 19), Size(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON)));
#else
	icon		= NIL;
#endif

	buttons		= btns;

	Int	 thissize = 0;
	Int	 maxsize = 0;
	Int	 titlesize = 0;
	Int	 buttonWidth = 80;
	Int	 buttonHeight = 22;

	lines = 1;

	if (text != NIL)
	{
		Int	 actpos = 0;

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

	msgbox->SetWidth(maxsize + 36);
	msgbox->SetHeight((((Int) Math::Max(2, lines) + 1) * 16) + 70 + buttonHeight);

	if (msgicon != NIL)
	{
#ifdef __WIN32__
		msgbox->SetWidth(maxsize + 36 + GetSystemMetrics(SM_CXICON) + 20);
#endif
	}


	titlesize = Font(Font::Default, Font::DefaultSize, Font::Bold).GetTextSizeX(title);

	msgbox->SetWidth(Math::Max(msgbox->GetWidth(), titlesize + 80));

	Add(msgbox);

	msgbox->Add(titlebar);
	msgbox->Add(lay);
	msgbox->Add(icon);

	msgbox->onPaint.Connect(&MessageDlg::MessagePaintProc, this);
	msgbox->doClose.Connect(&MessageDlg::MessageKillProc, this);

	if (cVar != NIL)
	{
		checkbox = new CheckBox(checkBoxText, Point(13, 46 + buttonHeight), Size(100, 0), cVar);
		checkbox->SetOrientation(OR_LOWERLEFT);
		checkbox->SetWidth(checkbox->textSize.cx + 21);

		msgbox->SetWidth(Math::Max(msgbox->GetWidth(), checkbox->textSize.cx + 54));
		msgbox->SetHeight(msgbox->GetHeight() + 22);

		if (msgicon != NIL)
		{
#ifdef __WIN32__
			checkbox->SetX(checkbox->GetX() + GetSystemMetrics(SM_CXICON) + 20);

			msgbox->SetWidth(Math::Max(msgbox->GetWidth(), checkbox->textSize.cx + 54 + GetSystemMetrics(SM_CXICON) + 20));
#endif
		}

		lay->Add(checkbox);
	}

	switch (buttons)
	{
		default:
		case MB_OK:
			buttonLabels.Add("OK");

			break;
		case MB_OKCANCEL:
			buttonLabels.Add("OK");
			buttonLabels.Add("Cancel");

			break;
		case MB_YESNO:
			buttonLabels.Add("Yes");
			buttonLabels.Add("No");

			break;
		case MB_YESNOCANCEL:
			buttonLabels.Add("Yes");
			buttonLabels.Add("No");
			buttonLabels.Add("Cancel");

			break;
		case MB_RETRYCANCEL:
			buttonLabels.Add("Retry");
			buttonLabels.Add("Cancel");

			break;
		case MB_ABORTRETRYIGNORE:
			buttonLabels.Add("Abort");
			buttonLabels.Add("Retry");
			buttonLabels.Add("Ignore");

			break;
	}

	for (Int i = 0; i < buttonLabels.Length(); i++)
	{
		Button	*button = new Button(I18n::Translator::defaultTranslator->TranslateString(buttonLabels.GetNth(i)), NIL, Point((msgbox->GetWidth() - (buttonLabels.Length() * (buttonWidth + 9) - 9)) / 2 - 3 + (i * (buttonWidth + 9)), 14 + buttonHeight), Size());

		if	(i == 0) button->onAction.Connect(&MessageDlg::MessageButton0, this);
		else if	(i == 1) button->onAction.Connect(&MessageDlg::MessageButton1, this);
		else if	(i == 2) button->onAction.Connect(&MessageDlg::MessageButton2, this);

		button->SetOrientation(OR_LOWERLEFT);

		buttonWidgets.Add(button);

		lay->Add(button);
	}

	msgbox->SetWidth(Math::Max(msgbox->GetWidth(), buttonLabels.Length() * (buttonWidth + 9) + 21));
	msgbox->SetPosition(Point((LiSAGetDisplaySizeX() - msgbox->GetWidth()) / 2, (LiSAGetDisplaySizeY() - msgbox->GetHeight()) / 2) + Point(nOfMessageBoxes * 25, nOfMessageBoxes * 25));
}

S::GUI::Dialogs::MessageDlg::~MessageDlg()
{
	for (Int i = 0; i < buttonWidgets.Length(); i++) DeleteObject(buttonWidgets.GetNth(i));

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
	if (buttonCode == 0) buttonCode = IDCLOSE;

	nOfMessageBoxes--;

	return True;
}

S::Void S::GUI::Dialogs::MessageDlg::MessageButton(Int buttonID)
{
	if	(buttonLabels.GetNth(buttonID) == "OK")		buttonCode = IDOK;
	else if	(buttonLabels.GetNth(buttonID) == "Cancel")	buttonCode = IDCANCEL;
	else if	(buttonLabels.GetNth(buttonID) == "Yes")	buttonCode = IDYES;
	else if	(buttonLabels.GetNth(buttonID) == "No")		buttonCode = IDNO;
	else if	(buttonLabels.GetNth(buttonID) == "Retry")	buttonCode = IDRETRY;
	else if	(buttonLabels.GetNth(buttonID) == "Abort")	buttonCode = IDABORT;
	else if	(buttonLabels.GetNth(buttonID) == "Ignore")	buttonCode = IDIGNORE;

	msgbox->Close();
}

S::Void S::GUI::Dialogs::MessageDlg::MessageButton0()
{
	MessageButton(0);
}

S::Void S::GUI::Dialogs::MessageDlg::MessageButton1()
{
	MessageButton(1);
}

S::Void S::GUI::Dialogs::MessageDlg::MessageButton2()
{
	MessageButton(2);
}

S::Int S::GUI::Dialogs::MessageDlg::GetButtonCode() const
{
	return buttonCode;
}

S::Void S::GUI::Dialogs::MessageDlg::SetRightToLeft(Bool rightToLeft)
{
	msgbox->SetRightToLeft(rightToLeft);
}

S::Bool S::GUI::Dialogs::MessageDlg::IsRightToLeft() const
{
	return msgbox->IsRightToLeft();
}

S::Void S::GUI::Dialogs::MessageDlg::SetDefaultRightToLeft(Bool nDefaultRightToLeft)
{
	defaultRightToLeft = nDefaultRightToLeft;
}

S::Bool S::GUI::Dialogs::MessageDlg::IsDefaultRightToLeft()
{
	return defaultRightToLeft;
}
