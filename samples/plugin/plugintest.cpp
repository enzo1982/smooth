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
#include "plugin.h"
#include "plugintest.h"

SVoid SMOOTH::Main()
{
	PlugInTest	*app = new PlugInTest();

	SMOOTH::Loop();

	delete app;
}

PlugInTest::PlugInTest()
{
	SetText("SMOOTH PlugIn Test");

	SPoint	 pos;
	SSize	 size;

	pos.x	= 15;
	pos.y	= 15;
	size.cx	= 50;
	size.cy	= 30;

	wnd		= new SWindow("SMOOTH plug-in test");
	layer		= new SLayer();
	title		= new STitlebar(true, false, true);
	activearea	= new SActiveAreaPlugin(RGB(255, 255, 128), pos, size, SProc(PlugInTest, this, activeAreaProc));
	
	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(layer);

	layer->RegisterObject(activearea);

	wnd->SetMetrics(SPoint(200, 150), SSize(250, 100));
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

SVoid PlugInTest::activeAreaProc()
{
	SMOOTH::MessageBox("This is a SMOOTH plug-in!", "Info", MB_OK, IDI_INFORMATION);
}
