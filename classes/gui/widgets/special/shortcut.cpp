 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/special/shortcut.h>
#include <smooth/misc/binary.h>
#include <smooth/i18n/translator.h>
#include <smooth/input/keyboard.h>

const S::Short	 S::GUI::Shortcut::classID = S::Object::RequestClassID();

S::GUI::Shortcut::Shortcut(Int iFlags, Int iKey, Widget *iRef, Int iParam) : Widget(Point(), Size())
{
	type		= classID;
	orientation	= OR_FREE;

	key		= iKey;
	flags		= iFlags;
	param		= iParam;
	ref		= iRef;

	onKeyDown.SetParentObject(this);
}

S::GUI::Shortcut::~Shortcut()
{
}

S::Int S::GUI::Shortcut::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered()) return Error();
	if (!IsActive())     return Success();

	if (ref != NIL && !ref->IsActive()) return Success();

	Int	 retVal = Success();

	switch (message)
	{
		case SM_KEYDOWN:
			if (wParam == key)
			{
				if ((((flags & SC_ALT) 	 && Input::Keyboard::GetKeyState(Input::Keyboard::KeyAlt))     || (!(flags & SC_ALT)   && !Input::Keyboard::GetKeyState(Input::Keyboard::KeyAlt)))     &&
				    (((flags & SC_CTRL)  && Input::Keyboard::GetKeyState(Input::Keyboard::KeyControl)) || (!(flags & SC_CTRL)  && !Input::Keyboard::GetKeyState(Input::Keyboard::KeyControl))) &&
				    (((flags & SC_CMD)   && Input::Keyboard::GetKeyState(Input::Keyboard::KeyCommand)) || (!(flags & SC_CMD)   && !Input::Keyboard::GetKeyState(Input::Keyboard::KeyCommand))) &&
				    (((flags & SC_SHIFT) && Input::Keyboard::GetKeyState(Input::Keyboard::KeyShift))   || (!(flags & SC_SHIFT) && !Input::Keyboard::GetKeyState(Input::Keyboard::KeyShift))))
				{
					EnterProtectedRegion();

					onKeyDown.Emit(param);

					LeaveProtectedRegion();

					retVal = Break;
				}
			}

			break;
	}

	return retVal;
}

S::Int S::GUI::Shortcut::SetShortcut(Int nFlags, Int nKey, Widget *nRef, Int nParam)
{
	key	= nKey;
	flags	= nFlags;
	param	= nParam;
	ref	= nRef;

	return Success();
}

S::String S::GUI::Shortcut::ToString() const
{
	String	 keyString;

	if	(key >= '0'  && key <= '9')	keyString[0] = key;
	else if (key >= 'A'  && key <= 'Z')	keyString[0] = key;

	if	(key >= Input::Keyboard::KeyF1 &&
		 key <= Input::Keyboard::KeyF24)    keyString = String("F").Append(String::FromInt(1 + (key - Input::Keyboard::KeyF1)));
	else if (key == Input::Keyboard::KeyBack)   keyString = I18n::Translator::defaultTranslator->TranslateString("Backspace");
	else if (key == Input::Keyboard::KeyTab)    keyString = I18n::Translator::defaultTranslator->TranslateString("Tab");
	else if (key == Input::Keyboard::KeyReturn) keyString = I18n::Translator::defaultTranslator->TranslateString("Return");
	else if (key == Input::Keyboard::KeyEscape) keyString = I18n::Translator::defaultTranslator->TranslateString("Esc");
	else if (key == Input::Keyboard::KeySpace)  keyString = I18n::Translator::defaultTranslator->TranslateString("Space");
	else if (key == Input::Keyboard::KeyPrior)  keyString = I18n::Translator::defaultTranslator->TranslateString("PgUp");
	else if (key == Input::Keyboard::KeyNext)   keyString = I18n::Translator::defaultTranslator->TranslateString("PgDown");
	else if (key == Input::Keyboard::KeyEnd)    keyString = I18n::Translator::defaultTranslator->TranslateString("End");
	else if (key == Input::Keyboard::KeyHome)   keyString = I18n::Translator::defaultTranslator->TranslateString("Home");
	else if (key == Input::Keyboard::KeyLeft)   keyString = I18n::Translator::defaultTranslator->TranslateString("Left");
	else if (key == Input::Keyboard::KeyUp)	    keyString = I18n::Translator::defaultTranslator->TranslateString("Up");
	else if (key == Input::Keyboard::KeyRight)  keyString = I18n::Translator::defaultTranslator->TranslateString("Right");
	else if (key == Input::Keyboard::KeyDown)   keyString = I18n::Translator::defaultTranslator->TranslateString("Down");
	else if (key == Input::Keyboard::KeyInsert) keyString = I18n::Translator::defaultTranslator->TranslateString("Ins");
	else if (key == Input::Keyboard::KeyDelete) keyString = I18n::Translator::defaultTranslator->TranslateString("Del");

	return	(Binary::IsFlagSet(flags, SC_CTRL)  ? I18n::Translator::defaultTranslator->TranslateString("Ctrl").Append("+")	: String())
         .Append(Binary::IsFlagSet(flags, SC_CMD)   ? I18n::Translator::defaultTranslator->TranslateString("Cmd").Append("+")	: String())
         .Append(Binary::IsFlagSet(flags, SC_ALT)   ? I18n::Translator::defaultTranslator->TranslateString("Alt").Append("+")	: String())
	 .Append(Binary::IsFlagSet(flags, SC_SHIFT) ? I18n::Translator::defaultTranslator->TranslateString("Shift").Append("+")	: String())
	 .Append(keyString);
}
