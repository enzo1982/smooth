 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/foreach.h>

#if defined __WIN32__
#	include <windows.h>
#elif defined __HAIKU__
#	include <support/Beep.h>
#endif

S::Int	 S::GUI::Dialogs::MessageDlg::nOfMessageBoxes = 0;
S::Bool	 S::GUI::Dialogs::MessageDlg::defaultRightToLeft = False;

S::Int S::GUI::Dialogs::QuickMessage(const String &text, const String &title, Int buttons, const Bitmap &icon)
{
	MessageDlg	*app = new MessageDlg(text, title, buttons, icon);

	app->ShowDialog();

	Int		 rVal = app->GetButtonCode();

	Object::DeleteObject(app);

	return rVal;
}

S::Int S::GUI::Dialogs::QuickMessage(const String &text, const String &title, Int buttons, Int iconID)
{
	MessageDlg	*app = new MessageDlg(text, title, buttons, iconID);

	app->ShowDialog();

	Int		 rVal = app->GetButtonCode();

	Object::DeleteObject(app);

	return rVal;
}

S::GUI::Dialogs::MessageDlg::MessageDlg(const String &text, const String &title, Int buttons, const Bitmap &icon, const String &checkBoxText, Bool *cVar)
{
	InitializeWidgets(text, title, buttons, icon, checkBoxText, cVar);
}

S::GUI::Dialogs::MessageDlg::MessageDlg(const String &text, const String &title, Int buttons, Int iconID, const String &checkBoxText, Bool *cVar)
{
	Bitmap	 icon;

#ifdef __WIN32__
	icon = ImageLoader::Load(String("Icon:").Append(String::FromInt(iconID)));
#else
	String	 iconName;
	Int	 iconSize = 32;

	if	(iconID == Message::Icon::Error)       iconName = "dialog-error.png";
	else if (iconID == Message::Icon::Question)    iconName = "dialog-question.png";
	else if (iconID == Message::Icon::Warning)     iconName = "dialog-warning.png";
	else if (iconID == Message::Icon::Information) iconName = "dialog-information.png";

	static const char	*iconDirs[] = { "/usr/share/icons", "/usr/local/share/icons", NIL };

	for (Int i = 0; iconDirs[i] != NIL; i++)
	{
		if (!Directory(iconDirs[i]).Exists()) continue;

		String	 file = String(iconDirs[i]).Append("/gnome/").Append(String::FromInt(iconSize)).Append("x").Append(String::FromInt(iconSize)).Append("/status/").Append(iconName);

		if (!File(file).Exists()) continue;

		icon = ImageLoader::Load(file);

		break;
	}
#endif

	icon.SetBackgroundColor(Setup::BackgroundColor);

	InitializeWidgets(text, title, buttons, icon, checkBoxText, cVar);
}

S::Bool S::GUI::Dialogs::MessageDlg::InitializeWidgets(const String &iText, const String &title, Int iButtons, const Bitmap &bitmap, const String &checkBoxText, Bool *iCVar)
{
	text	   = iText;
	cVar	   = iCVar;

	buttons	   = iButtons;
	buttonCode = -1;

	msgbox		= new Window(title, Point(), Size());
	msgbox->SetRightToLeft(defaultRightToLeft);
	msgbox->SetFlags(msgbox->GetFlags() | WF_MODAL);

	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	layer		= new Layer();
	icon		= new Image(bitmap, Point(14, 19), Size(32, 32));

	/* Compute message box size.
	 */
	Int	 maxSize      = 0;
	Int	 buttonWidth  = 80;
	Int	 buttonHeight = 22;

	GUI::Font		 font;
	const Array<String>	&lines = text.Explode("\n");

	foreach (const String &line, lines)
	{
		Int	 textSize = font.GetUnscaledTextSizeX(line);

		if (textSize > maxSize) maxSize = textSize;
	}

	msgbox->SetWidth(maxSize + 36);
	msgbox->SetHeight((((Int) Math::Max(2, lines.Length()) + 1) * 16) + 70 + buttonHeight);

	if (icon != NIL) msgbox->SetWidth(maxSize + 36 + icon->GetWidth() + 20);

	Int	 titleSize = Font(Font::Default, Font::DefaultSize, Font::Bold).GetUnscaledTextSizeX(title);

	msgbox->SetWidth(Math::Max(msgbox->GetWidth(), titleSize + 80));

	/* Add widgets.
	 */
	Add(msgbox);

	msgbox->Add(titlebar);
	msgbox->Add(layer);
	msgbox->Add(icon);

	msgbox->onPaint.Connect(&MessageDlg::MessagePaintProc, this);
	msgbox->doClose.Connect(&MessageDlg::MessageKillProc, this);

	/* Add optional checkbox.
	 */
	checkbox = NIL;

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

		layer->Add(checkbox);
	}

	/* Find button labels.
	 */
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

	/* Add buttons.
	 */
	foreach (const String &buttonLabel, buttonLabels)
	{
		Button	*button = new Button(I18n::Translator::defaultTranslator->TranslateString(buttonLabel), Point((msgbox->GetWidth() - (buttonLabels.Length() * (buttonWidth + 9) - 9)) / 2 - 3 + (foreachindex * (buttonWidth + 9)), 14 + buttonHeight), Size());

		if	(foreachindex == 0) button->onAction.Connect(&MessageDlg::MessageButton0, this);
		else if	(foreachindex == 1) button->onAction.Connect(&MessageDlg::MessageButton1, this);
		else if	(foreachindex == 2) button->onAction.Connect(&MessageDlg::MessageButton2, this);

		button->SetOrientation(OR_LOWERLEFT);

		buttonWidgets.Add(button);

		layer->Add(button);
	}

	/* Position message box on screen.
	 */
	Rect	 workArea    = System::Screen::GetActiveScreenWorkArea();
	Float	 scaleFactor = Surface().GetSurfaceDPI() / 96.0;

	msgbox->SetWidth(Math::Max(msgbox->GetWidth(), buttonLabels.Length() * (buttonWidth + 9) + 21));
	msgbox->SetPosition(workArea.GetPosition() + Point((workArea.GetWidth()  - Math::Round(msgbox->GetWidth()  * scaleFactor)) / 2,
							   (workArea.GetHeight() - Math::Round(msgbox->GetHeight() * scaleFactor)) / 2) + Point(nOfMessageBoxes * 25, nOfMessageBoxes * 25));

	return True;
}

S::GUI::Dialogs::MessageDlg::~MessageDlg()
{
	foreach (Button *button, buttonWidgets) DeleteObject(button);

	DeleteObject(layer);
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

	msgbox->WaitUntilClosed();

	return error;
}

S::Void S::GUI::Dialogs::MessageDlg::MessagePaintProc()
{
	const Array<String>	&lines = text.Explode("\n");

	/* Get text bounds.
	 */
	Layer	*mainLayer = msgbox->GetMainLayer();
	Surface	*surface   = msgbox->GetDrawSurface();

	Rect	 textRect  = Rect(mainLayer->GetRealPosition() + Point(14, 19) * surface->GetSurfaceDPI() / 96.0, mainLayer->GetRealSize() - Size(28, 19) * surface->GetSurfaceDPI() / 96.0);

	if (icon != NIL)
	{
		if (lines.Length() == 1) textRect.top += Math::Round(8 * surface->GetSurfaceDPI() / 96.0);

		textRect.left += icon->GetRealSize().cx + Math::Round(20 * surface->GetSurfaceDPI() / 96.0);
	}

	/* Draw message text.
	 */
	Font	 font;

	font.SetColor(Setup::TextColor);

	foreach (const String &line, lines)
	{
		surface->SetText(line, textRect, font);

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
	if	(buttonLabels.GetNth(buttonID) == "OK")	    buttonCode = Message::Button::Ok;
	else if	(buttonLabels.GetNth(buttonID) == "Cancel") buttonCode = Message::Button::Cancel;
	else if	(buttonLabels.GetNth(buttonID) == "Yes")    buttonCode = Message::Button::Yes;
	else if	(buttonLabels.GetNth(buttonID) == "No")	    buttonCode = Message::Button::No;
	else if	(buttonLabels.GetNth(buttonID) == "Retry")  buttonCode = Message::Button::Retry;
	else if	(buttonLabels.GetNth(buttonID) == "Abort")  buttonCode = Message::Button::Abort;
	else if	(buttonLabels.GetNth(buttonID) == "Ignore") buttonCode = Message::Button::Ignore;

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
