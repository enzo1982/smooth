 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TIPODAYDLG_
#define _H_OBJSMOOTH_TIPODAYDLG_

namespace smooth
{
	class DialogTipOfTheDay;

	namespace GUI
	{
		class Window;
		class Button;
		class CheckBox;
		class Text;
		class Titlebar;
		class Divider;
		class Layer;
	};
};

#include "../definitions.h"
#include "dialog.h"

namespace smooth
{
	class SMOOTHAPI DialogTipOfTheDay : public Dialog
	{
		private:
			GUI::Window	*dlgwnd;
			GUI::Titlebar	*titlebar;
			GUI::Divider	*divbar;
			GUI::Button	*btn_ok;
			GUI::Button	*btn_next;
			GUI::CheckBox	*check_showtips;
			GUI::Text	*txt_didyouknow;
			GUI::Text	*txt_tip;
			GUI::Layer	*layer_inner;

			Bool		 showTips;

			Array<String>	 tips;
			Int		 mode;
			Int		 offset;
		public:
					 DialogTipOfTheDay();
					~DialogTipOfTheDay();

			Int		 ShowDialog();

			Int		 AddTip(const String &);
			Int		 SetMode(Int, Int = 0);
		slots:
			Void		 ButtonOK();
			Void		 ButtonNext();

			Void		 Paint();
	};

	const Int	 TIP_ORDERED	= 0;
	const Int	 TIP_RANDOM	= 1;
};

#endif
