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
#include "objtest.h"

SMOOTHVoid SMOOTH::Main()
{
	ObjTest	*app = new ObjTest();

	SMOOTH::Loop();

	delete app;
}

ObjTest::ObjTest()
{
	SetText("ObjTest");

	SMOOTHPoint	 bp;
	SMOOTHSize	 bs;

	bvar = false;
	ivar = 0;

	wnd	= new SMOOTHWindow("SMOOTH Object Test");
	layer	= new SMOOTHLayer();
	layer2	= new SMOOTHLayer();
	title	= new SMOOTHTitlebar(true, false, true);
	menu	= new SMOOTHMenubar();

	bp.x = 200;
	bp.y = 120;
	bs.cx = 150;
	bs.cy = 40;

	layer2->SetMetrics(bp, bs);
	layer2->SetColor(RGB(0, 0, 192));

	bp.x = 10;
	bp.y = 12;
	bs.cx = 0;
	bs.cy = 0;

	btn_setOrient = new SMOOTHButton("Orientation", NIL, bp, bs, SMOOTHProc(ObjTest, this, setOrientation));
	bp.x += 90;
	btn_setText = new SMOOTHButton("Text", NIL, bp, bs, SMOOTHProc(ObjTest, this, setText));
	bp.x += 90;
	btn_setFont = new SMOOTHButton("Font", NIL, bp, bs, SMOOTHProc(ObjTest, this, setFont));
	bp.x += 90;
	btn_setPos = new SMOOTHButton("Position", NIL, bp, bs, SMOOTHProc(ObjTest, this, setPos));
	bp.x += 90;
	btn_setMetrics = new SMOOTHButton("Metrics", NIL, bp, bs, SMOOTHProc(ObjTest, this, setMetrics));
	bp.x = 10;
	bp.y += 30;
	btn_hide = new SMOOTHButton("Hide", NIL, bp, bs, SMOOTHProc(ObjTest, this, hide));
	bp.x += 90;
	btn_show = new SMOOTHButton("Show", NIL, bp, bs, SMOOTHProc(ObjTest, this, show));
	bp.x += 90;
	btn_activate = new SMOOTHButton("Activate", NIL, bp, bs, SMOOTHProc(ObjTest, this, activate));
	bp.x += 90;
	btn_deactivate = new SMOOTHButton("Deactivate", NIL, bp, bs, SMOOTHProc(ObjTest, this, deactivate));
	bp.x += 90;
	btn_hideLayer = new SMOOTHButton("Hide layer", NIL, bp, bs, SMOOTHProc(ObjTest, this, hideLayer));
	bp.x += 90;
	btn_showLayer = new SMOOTHButton("Show layer", NIL, bp, bs, SMOOTHProc(ObjTest, this, showLayer));

	bp.x = 100;
	bp.y = 100;
	bs.cx = 300;
	bs.cy = 150;

	reg_layer = new SMOOTHLayer("Reg");

	reg = new SMOOTHTabRegister(bp, bs);

	bp.x = 80;
	bp.y = 22;
	bs.cx = 0;
	bs.cy = 0;

	object = new SMOOTHHyperlink("Click me!", NIL, "http://www.gnu.org", bp);

	bp.y = 50;

	button = new SMOOTHButton("Test", NIL, bp, bs, NULLPROC);

	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(layer2);
	wnd->RegisterObject(layer);
	wnd->RegisterObject(menu);

	layer->RegisterObject(btn_setOrient);
	layer->RegisterObject(btn_setText);
	layer->RegisterObject(btn_setFont);
	layer->RegisterObject(btn_setPos);
	layer->RegisterObject(btn_setMetrics);
	layer->RegisterObject(btn_hide);
	layer->RegisterObject(btn_show);
	layer->RegisterObject(btn_hideLayer);
	layer->RegisterObject(btn_showLayer);
	layer->RegisterObject(btn_activate);
	layer->RegisterObject(btn_deactivate);

	layer->RegisterObject(reg);
	reg_layer->RegisterObject(object);
	reg_layer->RegisterObject(button);
	reg->RegisterObject(reg_layer);

	wnd->SetMetrics(SMOOTHPoint(80, 80), SMOOTHSize(600, 300));
}

ObjTest::~ObjTest()
{
	layer->UnregisterObject(btn_setOrient);
	layer->UnregisterObject(btn_setText);
	layer->UnregisterObject(btn_setFont);
	layer->UnregisterObject(btn_setPos);
	layer->UnregisterObject(btn_setMetrics);
	layer->UnregisterObject(btn_hide);
	layer->UnregisterObject(btn_show);
	layer->UnregisterObject(btn_hideLayer);
	layer->UnregisterObject(btn_showLayer);
	layer->UnregisterObject(btn_activate);
	layer->UnregisterObject(btn_deactivate);

	layer->UnregisterObject(reg);

	reg_layer->UnregisterObject(object);
	reg_layer->UnregisterObject(button);
	reg->UnregisterObject(reg_layer);

	wnd->UnregisterObject(title);
	wnd->UnregisterObject(layer);
	wnd->UnregisterObject(layer2);
	wnd->UnregisterObject(menu);

	UnregisterObject(wnd);

	delete title;
	delete menu;
	delete wnd;
	delete layer;
	delete layer2;
	delete btn_setOrient;
	delete btn_setText;
	delete btn_setFont;
	delete btn_setPos;
	delete btn_setMetrics;
	delete btn_hide;
	delete btn_show;
	delete btn_hideLayer;
	delete btn_showLayer;
	delete btn_activate;
	delete btn_deactivate;
	delete object;
	delete button;
	delete reg;
	delete reg_layer;
}

SMOOTHVoid ObjTest::setOrientation()
{
	if (object->GetObjectProperties()->orientation == OR_UPPERLEFT)	object->SetOrientation(OR_LOWERRIGHT);
	else								object->SetOrientation(OR_UPPERLEFT);
}

SMOOTHVoid ObjTest::setText()
{
	if (object->GetObjectProperties()->text == "Test")	object->SetText("Hallo");
	else							object->SetText("Test");
}

SMOOTHVoid ObjTest::setFont()
{
	if (object->GetObjectProperties()->font == "Microsoft Sans Serif")	object->SetFont("Verdana", 14, RGB(255, 0, 0), FW_NORMAL);
	else									object->SetFont("Microsoft Sans Serif", I18N_DEFAULTFONTSIZE, RGB(0, 0, 0), FW_NORMAL);
}

SMOOTHVoid ObjTest::setPos()
{
	SMOOTHPoint	 pos;

	if (object->GetObjectProperties()->pos.x == 150)
	{
		pos.x = 200;
		pos.y = 100;
	}
	else
	{
		pos.x = 150;
		pos.y = 150;
	}

	object->SetPosition(pos);
}

SMOOTHVoid ObjTest::setMetrics()
{
	SMOOTHSize	 size;

	size.cy = 30;

	if (object->GetObjectProperties()->size.cx == 80)	{ size.cx = 100; object->SetMetrics(object->GetObjectProperties()->pos, size); }
	else							{ size.cx = 80; object->SetMetrics(object->GetObjectProperties()->pos, size); }
}

SMOOTHVoid ObjTest::hide()
{
	object->Hide();
}

SMOOTHVoid ObjTest::show()
{
	object->Show();
}

SMOOTHVoid ObjTest::hideLayer()
{
	layer->Hide();
}

SMOOTHVoid ObjTest::showLayer()
{
	layer->Show();
}

SMOOTHVoid ObjTest::activate()
{
	object->Activate();
}

SMOOTHVoid ObjTest::deactivate()
{
	object->Deactivate();
}
