 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/system/screen.h>
#include <smooth/files/file.h>
#include <smooth/files/directory.h>

#if defined __WIN32__
#	include <windows.h>
#elif defined __HAIKU__
#	include <support/Beep.h>
#endif

S::Int	 S::GUI::Dialogs::MessageDlg::nOfMessageBoxes = 0;
S::Bool	 S::GUI::Dialogs::MessageDlg::defaultRightToLeft = False;

S::Int S::GUI::Dialogs::QuickMessage(const String &text, const String &title, Int buttons, const char *icon)
{
	MessageDlg	*app = new MessageDlg(text, title, buttons, (const wchar_t *) icon);

	app->ShowDialog();

	Int		 rVal = app->GetButtonCode();

	Object::DeleteObject(app);

	return rVal;
}

S::Int S::GUI::Dialogs::QuickMessage(const String &text, const String &title, Int buttons, const wchar_t *icon)
{
	MessageDlg	*app = new MessageDlg(text, title, buttons, icon);

	app->ShowDialog();

	Int		 rVal = app->GetButtonCode();

	Object::DeleteObject(app);

	return rVal;
}

S::GUI::Dialogs::MessageDlg::MessageDlg(const String &text, const String &title, Int btns, const wchar_t *cIcon, const String &checkBoxText, Bool *iCVar)
{
	cVar = iCVar;

	msgbox		= new Window(title, Point(), Size());
	msgbox->SetRightToLeft(defaultRightToLeft);

	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	lay		= new Layer();

#ifdef __WIN32__
	Bitmap	 bitmap = ImageLoader::Load(String("Icon:").Append(String::FromInt((Int64) cIcon)));

	bitmap.SetBackgroundColor(Setup::BackgroundColor);

	icon		= new Image(bitmap, Point(14, 19), Size(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON)));
#else
	String	 iconName;
	Int	 iconSize = 32;

	icon		= NIL;

	if	(cIcon == Message::Icon::Error)	      iconName = "dialog-error.png";
	else if (cIcon == Message::Icon::Question)    iconName = "dialog-question.png";
	else if (cIcon == Message::Icon::Warning)     iconName = "dialog-warning.png";
	else if (cIcon == Message::Icon::Information) iconName = "dialog-information.png";

	static const char	*iconDirs[] = { "/usr/share/icons", "/usr/local/share/icons", NIL };

	for (int i = 0; iconDirs[i] != NIL; i++)
	{
		if (Directory(iconDirs[i]).Exists())
		{
			String	 file = String(iconDirs[i]).Append("/gnome/").Append(String::FromInt(iconSize)).Append("x").Append(String::FromInt(iconSize)).Append("/status/").Append(iconName);

			if (File(file).Exists())
			{
				Bitmap	 bitmap = ImageLoader::Load(file);

				bitmap.SetBackgroundColor(Setup::BackgroundColor);

				icon = new Image(bitmap, Point(14, 19), Size(iconSize, iconSize));

				break;
			}
		}
	}
#endif

	buttons	   = btns;
	buttonCode = -1;

	Int	 thissize     = 0;
	Int	 maxsize      = 0;
	Int	 titlesize    = 0;
	Int	 buttonWidth  = 80;
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
				if (text[actpos] != '\n')
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

			thissize = font.GetUnscaledTextSizeX(line[j]);

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

	if (icon != NIL) msgbox->SetWidth(maxsize + 36 + icon->GetWidth() + 20);

	titlesize = Font(Font::Default, Font::DefaultSize, Font::Bold).GetUnscaledTextSizeX(title);

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
		checkbox->SetWidth(checkbox->GetUnscaledTextWidth() + 21);

		msgbox->SetWidth(Math::Max(msgbox->GetWidth(), checkbox->GetUnscaledTextWidth() + 54));
		msgbox->SetHeight(msgbox->GetHeight() + 22);

		if (icon !=  NIL)
		{
			checkbox->SetX(checkbox->GetX() + icon->GetWidth() + 20);

			msgbox->SetWidth(Math::Max(msgbox->GetWidth(), checkbox->GetUnscaledTextWidth() + 54 + icon->GetWidth() + 20));
		}

		lay->Add(checkbox);
	}
	else
	{
		checkbox = NIL;
	}

	switch (buttons)
	{
		default:
		case Message::Buttons::Ok:
			buttonLabels.Add("OK");

			break;
		case Message::Buttons::OkCancel:
			buttonLabels.Add("OK");
			buttonLabels.Add("Cancel");

			break;
		case Message::Buttons::YesNo:
			buttonLabels.Add("Yes");
			buttonLabels.Add("No");

			break;
		case Message::Buttons::YesNoCancel:
			buttonLabels.Add("Yes");
			buttonLabels.Add("No");
			buttonLabels.Add("Cancel");

			break;
		case Message::Buttons::RetryCancel:
			buttonLabels.Add("Retry");
			buttonLabels.Add("Cancel");

			break;
		case Message::Buttons::AbortRetryIgnore:
			buttonLabels.Add("Abort");
			buttonLabels.Add("Retry");
			buttonLabels.Add("Ignore");

			break;
	}

	Rect	 workArea = System::Screen::GetActiveScreenWorkArea();

	msgbox->SetWidth(Math::Max(msgbox->GetWidth(), buttonLabels.Length() * (buttonWidth + 9) + 21));
	msgbox->SetPosition(workArea.GetPosition() + Point((workArea.GetSize().cx - msgbox->GetWidth()) / 2, (workArea.GetSize().cy - msgbox->GetHeight()) / 2) + Point(nOfMessageBoxes * 25, nOfMessageBoxes * 25));

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
}

S::GUI::Dialogs::MessageDlg::~MessageDlg()
{
	for (Int i = 0; i < buttonWidgets.Length(); i++) DeleteObject(buttonWidgets.GetNth(i));

	DeleteObject(lay);
	DeleteObject(icon);
	DeleteObject(titlebar);
	DeleteObject(msgbox);

	if (checkbox != NIL) DeleteObject(checkbox);
}

const Error &S::GUI::Dialogs::MessageDlg::ShowDialog()
{
	nOfMessageBoxes++;

#if defined __WIN32__
	MessageBeep(MB_ICONASTERISK);
#elif defined __HAIKU__
	beep();
#endif

	msgbox->Stay();

	return error;
}

S::Void S::GUI::Dialogs::MessageDlg::MessagePaintProc()
{
	Layer	*mainLayer = msgbox->GetMainLayer();
	Surface	*surface   = msgbox->GetDrawSurface();

	Rect	 textRect  = Rect(mainLayer->GetRealPosition() + Point(14, 19) * surface->GetSurfaceDPI() / 96.0, mainLayer->GetRealSize() - Size(28, 19) * surface->GetSurfaceDPI() / 96.0);

	if (icon != NIL)
	{
		if (lines == 1) textRect.top += Math::Round(8 * surface->GetSurfaceDPI() / 96.0);

		textRect.left += icon->GetRealSize().cx + Math::Round(20 * surface->GetSurfaceDPI() / 96.0);
	}

	for (int i = 0; i < lines; i++)
	{
		surface->SetText(line[i], textRect, Font());

		textRect.top += Math::Round(16 * surface->GetSurfaceDPI() / 96.0);
	}
}

S::Bool S::GUI::Dialogs::MessageDlg::MessageKillProc()
{
	if (buttonCode == 0) buttonCode = Message::Button::Close;

	nOfMessageBoxes--;

	return True;
}

S::Void S::GUI::Dialogs::MessageDlg::MessageButton(Int buttonID)
{
	if	(buttonLabels.GetNth(buttonID) == "OK")		buttonCode = Message::Button::Ok;
	else if	(buttonLabels.GetNth(buttonID) == "Cancel")	buttonCode = Message::Button::Cancel;
	else if	(buttonLabels.GetNth(buttonID) == "Yes")	buttonCode = Message::Button::Yes;
	else if	(buttonLabels.GetNth(buttonID) == "No")		buttonCode = Message::Button::No;
	else if	(buttonLabels.GetNth(buttonID) == "Retry")	buttonCode = Message::Button::Retry;
	else if	(buttonLabels.GetNth(buttonID) == "Abort")	buttonCode = Message::Button::Abort;
	else if	(buttonLabels.GetNth(buttonID) == "Ignore")	buttonCode = Message::Button::Ignore;

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
