 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "plugin.h"
#include "plugintest.h"

Int smooth::Main()
{
	PlugInTest	*app = new PlugInTest();

	Loop();

	delete app;

	return 0;
}

PlugInTest::PlugInTest()
{
	Point	 pos;
	Size	 size;

	pos.x	= 15;
	pos.y	= 15;
	size.cx	= 50;
	size.cy	= 30;

	wnd		= new Window("smooth plug-in test");
	layer		= new Layer();
	title		= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	activearea	= new ActiveAreaPlugin(RGB(255, 255, 128), pos, size);
	activearea->onClick.Connect(&PlugInTest::activeAreaProc, this);
	
	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(layer);

	layer->RegisterObject(activearea);

	wnd->SetMetrics(Point(200, 150), Size(250, 100));
}

PlugInTest::~PlugInTest()
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

Void PlugInTest::activeAreaProc()
{
	QuickMessage("This is a smooth plug-in!", "Info", MB_OK, IDI_INFORMATION);
}
