 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <math.h>
#include "testkey.h"

void SMOOTH::Main()
{
	TestKey	*app = new TestKey();

	SMOOTH::Loop();

	delete app;
}

TestKey::TestKey()
{
	SetText("TestKey");

	SMOOTHPoint	 pos;

	pos.x = 69;
	pos.y = 43;

	wnd	= new SWindow("TestKey - by Robert Kausch 2000-2002");
	layer	= new SLayer();
	title	= new STitlebar(true, false, true);
	text	= new SText("ASCII-Code: 000\nScanCode: 000\nVirtual keycode: 000", pos);

	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(layer);

	layer->RegisterObject(text);

	wnd->SetMessageProc(SMessageProc(TestKey, this, MessageProc));
	wnd->SetMetrics(SPoint(80, 80), SSize(240, 160));
}

TestKey::~TestKey()
{
	layer->UnregisterObject(text);

	wnd->UnregisterObject(title);
	wnd->UnregisterObject(layer);

	UnregisterObject(wnd);

	delete title;
	delete wnd;
	delete text;
	delete layer;
}

SVoid TestKey::MessageProc(SInt message, SInt wParam, SInt lParam)
{
	SByte	 newText[51]	= "ASCII-Code: 000\nScanCode: 000\nVirtual keycode: 000";
	SByte	 asciiCode	= 0;
	SInt	 scanCode	= 0;
	SInt	 vkCode		= 0;
	SByte	 keys[256];

	if (message == WM_KEYDOWN)
	{
		GetKeyboardState((SUByte *) &keys);
		ToAscii(wParam, GetBits(lParam, 16, 23), (SUByte *) &keys, (SUShort *) &asciiCode, 0);

		scanCode	= GetBits(lParam, 16, 23);
		vkCode		= wParam;

		newText[12] = asciiCode / 100 + 48;
		newText[13] = asciiCode % 100 / 10 + 48;
		newText[14] = asciiCode % 100 % 10 + 48;

		newText[26] = scanCode / 100 + 48;
		newText[27] = scanCode % 100 / 10 + 48;
		newText[28] = scanCode % 100 % 10 + 48;

		newText[47] = vkCode / 100 + 48;
		newText[48] = vkCode % 100 / 10 + 48;
		newText[49] = vkCode % 100 % 10 + 48;

		text->SetText(newText);
	}
}

SInt TestKey::GetBits(SInt number, SUInt startBit, SUInt endBit)
{
	SInt	retVal = 0;

	if (startBit >= 64 || endBit >= 64) return -1;

	for (SUInt i = startBit; i <= endBit; i++)
	{
		retVal += (SInt) pow(2, i - startBit) * ((number >> i) % 2);
	}

	return retVal;
}
