 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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

const S::Int	 S::GUI::Shortcut::classID = S::Object::RequestClassID();

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
	if (!IsRegistered())	return Error();
	if (!IsActive())	return Success();

	if (ref != NIL && !ref->IsActive())	return Success();

	Int	 retVal = Success();

	EnterProtectedRegion();

	switch (message)
	{
#ifdef __WIN32__
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (wParam == key)
			{
				BYTE	 state[256];

				if (GetKeyboardState(state))
				{
					if ((((flags & SC_ALT) 	 && (lParam & 536870912))      || (!(flags & SC_ALT)   && !(lParam & 536870912)))      &&
					    (((flags & SC_CTRL)  && (state[VK_CONTROL] & 128)) || (!(flags & SC_CTRL)  && !(state[VK_CONTROL] & 128))) &&
					    (((flags & SC_SHIFT) && (state[VK_SHIFT] & 128))   || (!(flags & SC_SHIFT) && !(state[VK_SHIFT] & 128))))
					{
						onKeyDown.Emit(param);

						retVal = Break;
					}
				}
			}

			break;
#endif
	}

	LeaveProtectedRegion();

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

S::Int S::GUI::Shortcut::GetKey()
{
	return key;
}

S::String S::GUI::Shortcut::ToString() const
{
	String	 keyString;

#ifdef __WIN32__
	if (key >= VK_F1 && key <= VK_F24)	keyString = String("F").Append(String::FromInt(1 + (key - VK_F1)));
	else if (key >= '0'  && key <= '9')	keyString[0] = key;
	else if (key >= 'A'  && key <= 'Z')	keyString[0] = key;
	else if (key == VK_BACK)		keyString = I18n::Translator::defaultTranslator->TranslateString("Backspace");
	else if (key == VK_TAB)			keyString = I18n::Translator::defaultTranslator->TranslateString("Tab");
	else if (key == VK_RETURN)		keyString = I18n::Translator::defaultTranslator->TranslateString("Return");
	else if (key == VK_ESCAPE)		keyString = I18n::Translator::defaultTranslator->TranslateString("Esc");
	else if (key == VK_SPACE)		keyString = I18n::Translator::defaultTranslator->TranslateString("Space");
	else if (key == VK_PRIOR)		keyString = I18n::Translator::defaultTranslator->TranslateString("PgUp");
	else if (key == VK_NEXT)		keyString = I18n::Translator::defaultTranslator->TranslateString("PgDown");
	else if (key == VK_END)			keyString = I18n::Translator::defaultTranslator->TranslateString("End");
	else if (key == VK_HOME)		keyString = I18n::Translator::defaultTranslator->TranslateString("Home");
	else if (key == VK_LEFT)		keyString = I18n::Translator::defaultTranslator->TranslateString("Left");
	else if (key == VK_UP)			keyString = I18n::Translator::defaultTranslator->TranslateString("Up");
	else if (key == VK_RIGHT)		keyString = I18n::Translator::defaultTranslator->TranslateString("Right");
	else if (key == VK_DOWN)		keyString = I18n::Translator::defaultTranslator->TranslateString("Down");
	else if (key == VK_INSERT)		keyString = I18n::Translator::defaultTranslator->TranslateString("Ins");
	else if (key == VK_DELETE)		keyString = I18n::Translator::defaultTranslator->TranslateString("Del");
#endif

	return	(Binary::IsFlagSet(flags, SC_CTRL) ? String(I18n::Translator::defaultTranslator->TranslateString("Ctrl")).Append("+") : String())
         .Append(Binary::IsFlagSet(flags, SC_ALT) ? String(I18n::Translator::defaultTranslator->TranslateString("Alt")).Append("+") : String())
	 .Append(Binary::IsFlagSet(flags, SC_SHIFT) ? String(I18n::Translator::defaultTranslator->TranslateString("Shift")).Append("+") : String())
	 .Append(keyString);
}
