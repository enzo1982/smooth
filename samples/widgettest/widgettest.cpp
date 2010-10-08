 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "widget/myactivearea.h"
#include "widgettest.h"

using namespace smooth::GUI::Dialogs;

Int smooth::Main()
{
	WidgetTest	*app = new WidgetTest();

	app->Loop();

	Object::DeleteObject(app);

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

	wnd		= new GUI::Window("smooth widget test", Point(200, 150), Size(250, 100));
	layer		= new Layer();
	title		= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	activearea	= new MyActiveArea(RGB(255, 255, 128), pos, size);
	activearea->onAction.Connect(&WidgetTest::activeAreaProc, this);
	
	Add(wnd);

	wnd->Add(title);
	wnd->Add(layer);

	layer->Add(activearea);
}

WidgetTest::~WidgetTest()
{
	DeleteObject(title);
	DeleteObject(wnd);
	DeleteObject(activearea);
	DeleteObject(layer);
}

Void WidgetTest::activeAreaProc()
{
	QuickMessage("This is a custom smooth widget!", "Info", MB_OK, IDI_INFORMATION);
}
