 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/keyboard.h>
#include <smooth/init.h>

S::Array<S::Bool>	*S::Input::Keyboard::keyState = NIL;

S::Int	 addKeyboardFreeTmp = S::AddFreeFunction(&S::Input::Keyboard::FreeKeyState);

S::Input::Keyboard::Keyboard()
{
}

S::Input::Keyboard::Keyboard(const Keyboard &)
{
}

S::Int S::Input::Keyboard::InitKeyState()
{
	if (keyState != NIL) return Success();

	keyState = new Array<Bool>();

	keyState->Add(False, KeyOther);

	keyState->Add(False, KeyBack);
	keyState->Add(False, KeyTab);

	keyState->Add(False, KeyReturn);

	keyState->Add(False, KeyShift);
	keyState->Add(False, KeyControl);
	keyState->Add(False, KeyAlt);

	keyState->Add(False, KeyEscape);

	keyState->Add(False, KeySpace);

	keyState->Add(False, KeyPrior);
	keyState->Add(False, KeyNext);
	keyState->Add(False, KeyEnd);
	keyState->Add(False, KeyHome);

	keyState->Add(False, KeyLeft);
	keyState->Add(False, KeyUp);
	keyState->Add(False, KeyRight);
	keyState->Add(False, KeyDown);

	keyState->Add(False, KeyInsert);
	keyState->Add(False, KeyDelete);

	keyState->Add(False, Key0);
	keyState->Add(False, Key1);
	keyState->Add(False, Key2);
	keyState->Add(False, Key3);
	keyState->Add(False, Key4);
	keyState->Add(False, Key5);
	keyState->Add(False, Key6);
	keyState->Add(False, Key7);
	keyState->Add(False, Key8);
	keyState->Add(False, Key9);

	keyState->Add(False, KeyA);
	keyState->Add(False, KeyB);
	keyState->Add(False, KeyC);
	keyState->Add(False, KeyD);
	keyState->Add(False, KeyE);
	keyState->Add(False, KeyF);
	keyState->Add(False, KeyG);
	keyState->Add(False, KeyH);
	keyState->Add(False, KeyI);
	keyState->Add(False, KeyJ);
	keyState->Add(False, KeyK);
	keyState->Add(False, KeyL);
	keyState->Add(False, KeyM);
	keyState->Add(False, KeyN);
	keyState->Add(False, KeyO);
	keyState->Add(False, KeyP);
	keyState->Add(False, KeyQ);
	keyState->Add(False, KeyR);
	keyState->Add(False, KeyS);
	keyState->Add(False, KeyT);
	keyState->Add(False, KeyU);
	keyState->Add(False, KeyV);
	keyState->Add(False, KeyW);
	keyState->Add(False, KeyX);
	keyState->Add(False, KeyY);
	keyState->Add(False, KeyZ);

	keyState->Add(False, KeyCommand);

	keyState->Add(False, KeyF1);
	keyState->Add(False, KeyF2);
	keyState->Add(False, KeyF3);
	keyState->Add(False, KeyF4);
	keyState->Add(False, KeyF5);
	keyState->Add(False, KeyF6);
	keyState->Add(False, KeyF7);
	keyState->Add(False, KeyF8);
	keyState->Add(False, KeyF9);
	keyState->Add(False, KeyF10);
	keyState->Add(False, KeyF11);
	keyState->Add(False, KeyF12);
	keyState->Add(False, KeyF13);
	keyState->Add(False, KeyF14);
	keyState->Add(False, KeyF15);
	keyState->Add(False, KeyF16);
	keyState->Add(False, KeyF17);
	keyState->Add(False, KeyF18);
	keyState->Add(False, KeyF19);
	keyState->Add(False, KeyF20);
	keyState->Add(False, KeyF21);
	keyState->Add(False, KeyF22);
	keyState->Add(False, KeyF23);
	keyState->Add(False, KeyF24);

	return Success();
}

S::Int S::Input::Keyboard::FreeKeyState()
{
	if (keyState == NIL) return Success();

	delete keyState;

	keyState = NIL;

	return Success();
}

S::Bool S::Input::Keyboard::GetKeyState(Key key)
{
	if (keyState == NIL) InitKeyState();

	return keyState->Get(key);
}

S::Void S::Input::Keyboard::UpdateKeyState(Key key, Bool status)
{
	if (keyState == NIL) InitKeyState();

	keyState->Set(key, status);
}

S::Void S::Input::Keyboard::ResetKeyState()
{
	FreeKeyState();
	InitKeyState();
}
