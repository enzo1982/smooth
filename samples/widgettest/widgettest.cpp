 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "myactivearea.h"
#include "widgettest.h"

using namespace smooth::GUI::Dialogs;

Int smooth::Main()
{
	WidgetTest	*app = new WidgetTest();

	Loop();

	delete app;

	return 0;
}

WidgetTest::WidgetTest()
{
	Point	 pos;
	Size	 size;

	pos.x	= 15;
	pos.y	= 15;
	size.cx	= 50;
	size.cy	= 30;

	wnd		= new Window("smooth widget test", Point(200, 150), Size(250, 100));
	layer		= new Layer();
	title		= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	activearea	= new MyActiveArea(RGB(255, 255, 128), pos, size);
	activearea->onAction.Connect(&WidgetTest::activeAreaProc, this);
	
	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(layer);

	layer->RegisterObject(activearea);
}

WidgetTest::~WidgetTest()
{
	layer->UnregisterObject(activearea);

	wnd->UnregisterObject(title);
	wnd->UnregisterObject(layer);

	UnregisterObject(wnd);

	delete title;
	delete wnd;
	delete activearea;
	delete layer;
}

Void WidgetTest::activeAreaProc()
{
	QuickMessage("This is a custom smooth widget!", "Info", MB_OK, IDI_INFORMATION);
}
