 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef	_H_OBJTEST_
#define _H_OBJTEST_

#include <smoothx.h>

class ObjTest : public SMOOTHApplication
{
	private:
		SMOOTHWindow		*wnd;
		SMOOTHLayer		*layer;
		SMOOTHLayer		*layer2;
		SMOOTHTitlebar		*title;
		SMOOTHMenubar		*menu;

		SMOOTHButton		*btn_setOrient;
		SMOOTHButton		*btn_setText;
		SMOOTHButton		*btn_setFont;
		SMOOTHButton		*btn_setPos;
		SMOOTHButton		*btn_setMetrics;
		SMOOTHButton		*btn_hide;
		SMOOTHButton		*btn_show;
		SMOOTHButton		*btn_activate;
		SMOOTHButton		*btn_deactivate;
		SMOOTHButton		*btn_hideLayer;
		SMOOTHButton		*btn_showLayer;

		SMOOTHTabRegister	*reg;
		SMOOTHLayer		*reg_layer;
		SMOOTHHyperlink		*object;
		SMOOTHButton		*button;

		SMOOTHBool		 bvar;
		SMOOTHInt		 ivar;
	public:
					 ObjTest();
					~ObjTest();
		SMOOTHVoid		 setOrientation();
		SMOOTHVoid		 setText();
		SMOOTHVoid		 setFont();
		SMOOTHVoid		 setPos();
		SMOOTHVoid		 setMetrics();
		SMOOTHVoid		 hide();
		SMOOTHVoid		 show();
		SMOOTHVoid		 hideLayer();
		SMOOTHVoid		 showLayer();
		SMOOTHVoid		 activate();
		SMOOTHVoid		 deactivate();
};

#endif
