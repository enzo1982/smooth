 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
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
	return TestKey().Loop();
}

TestKey::TestKey()
{
	Point	 pos;

	pos.x = 85;
	pos.y = 28;

	wnd	= new GUI::Window("TestKey - by Robert Kausch 2000-2020", Point(80, 80), Size(300, 160));
	title	= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	text	= new Text("ASCII code: 000\nUnicode character: 00000\n\nScan code: 000\nVirtual keycode: 000", pos);

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
	String		 newText	= "ASCII code: %1\nUnicode character: %2\n\nScan code: %3\nVirtual keycode: %4";
	UnsignedByte	 asciiCode	= 0;
	wchar_t		 unicode	= 0;
	Int		 scanCode	= 0;
	Int		 vkCode		= 0;

	if (message == SM_KEYDOWN)
	{
#ifdef __WIN32__
		Byte	 keys[256];

		GetKeyboardState((UnsignedByte *) &keys);
		ToAscii(wParam, Binary::GetBits(lParam, 16, 23), (UnsignedByte *) &keys, (UnsignedShort *) &asciiCode, 0);
		ToUnicode(wParam, Binary::GetBits(lParam, 16, 23), (UnsignedByte *) &keys, &unicode, 1, 0);
#endif

		scanCode = Binary::GetBits(lParam, 16, 23);
		vkCode	 = wParam;

		newText.Replace("%1", String().FillN('0', 2 - Math::Floor(Math::Log10(asciiCode))).Append(String::FromInt(asciiCode)));
		newText.Replace("%2", String().FillN('0', 4 - Math::Floor(Math::Log10(unicode))).Append(String::FromInt(unicode)));
		newText.Replace("%3", String().FillN('0', 2 - Math::Floor(Math::Log10(scanCode))).Append(String::FromInt(scanCode)));
		newText.Replace("%4", String().FillN('0', 2 - Math::Floor(Math::Log10(vkCode))).Append(String::FromInt(vkCode)));

		text->SetText(newText);
	}
}
