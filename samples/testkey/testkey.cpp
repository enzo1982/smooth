 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "testkey.h"

Int smooth::Main()
{
	TestKey	*app = new TestKey();

	app->Loop();

	Object::DeleteObject(app);

	return 0;
}

TestKey::TestKey()
{
	Point	 pos;

	pos.x = 85;
	pos.y = 28;

	wnd	= new Window("TestKey - by Robert Kausch 2000-2008", Point(80, 80), Size(300, 160));
	title	= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	text	= new Text("ASCII-Code: 000\nUnicode character: 00000\n\nScanCode: 000\nVirtual keycode: 000", pos);

	Add(wnd);

	wnd->Add(title);
	wnd->Add(text);

	wnd->onEvent.Connect(&TestKey::EventProc, this);
}

TestKey::~TestKey()
{
	DeleteObject(title);
	DeleteObject(wnd);
	DeleteObject(text);
}

Void TestKey::EventProc(Int message, Int wParam, Int lParam)
{
	Byte		 newText[77]	= "ASCII-Code: 000\nUnicode character: 00000\n\nScanCode: 000\nVirtual keycode: 000";
	UnsignedByte	 asciiCode	= 0;
	wchar_t		 unicode	= 0;
	Int		 scanCode	= 0;
	Int		 vkCode		= 0;
	Byte		 keys[256];

	if (message == SM_KEYDOWN)
	{
#ifdef __WIN32__
		GetKeyboardState((UnsignedByte *) &keys);
		ToAscii(wParam, GetBits(lParam, 16, 23), (UnsignedByte *) &keys, (UnsignedShort *) &asciiCode, 0);

		if (Setup::enableUnicode) ToUnicode(wParam, GetBits(lParam, 16, 23), (UnsignedByte *) &keys, &unicode, 1, 0);
#endif

		scanCode	= GetBits(lParam, 16, 23);
		vkCode		= wParam;

		newText[12] = asciiCode / 100 + 48;
		newText[13] = asciiCode % 100 / 10 + 48;
		newText[14] = asciiCode % 100 % 10 + 48;

		newText[35] = unicode / 10000 + 48;
		newText[36] = unicode % 10000 / 1000 + 48;
		newText[37] = unicode % 10000 % 1000 / 100 + 48;
		newText[38] = unicode % 10000 % 1000 % 100 / 10 + 48;
		newText[39] = unicode % 10000 % 1000 % 100 % 10 + 48;

		newText[52] = scanCode / 100 + 48;
		newText[53] = scanCode % 100 / 10 + 48;
		newText[54] = scanCode % 100 % 10 + 48;

		newText[73] = vkCode / 100 + 48;
		newText[74] = vkCode % 100 / 10 + 48;
		newText[75] = vkCode % 100 % 10 + 48;

		text->SetText(newText);
	}
}

Int TestKey::GetBits(Int number, UnsignedInt startBit, UnsignedInt endBit)
{
	Int	retVal = 0;

	if (startBit >= 64 || endBit >= 64) return -1;

	for (UnsignedInt i = startBit; i <= endBit; i++)
	{
		retVal += Math::Pow(2l, i - startBit) * ((number >> i) % 2);
	}

	return retVal;
}
