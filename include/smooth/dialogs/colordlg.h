 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_COLORDLG_
#define _H_OBJSMOOTH_COLORDLG_

namespace smooth
{
	class DialogColorSelection;

	namespace GUI
	{
		class Window;
		class Button;
		class EditBox;
		class Text;
		class Slider;
		class Titlebar;
		class Divider;
	};
};

#include "../definitions.h"
#include "dialog.h"

namespace smooth
{
	class SMOOTHAPI DialogColorSelection : public Dialog
	{
		private:
			Int		 color;

			Int		 yoffset;
			Int		 huexoffset;
			Int		 ncxoffset;
			Int		 ocxoffset;
			Int		 crsizex;
			Int		 crsizey;

			Bool		 huecapt;
			Bool		 vscapt;

			Bool		 forcehupdate;
			Bool		 forcevsupdate;
			Bool		 preventhupdate;
			Bool		 preventvsupdate;
			Bool		 updatetext;
			Bool		 updatehextext;

			Int		 lasthue;
			Int		 lastsat;
			Int		 lastval;

			Int		 acthue;
			Int		 actsat;
			Int		 actval;
			Int		 actred;
			Int		 actgreen;
			Int		 actblue;

			Int		 colortable[256][256];

			String		 hexval;

			GUI::Window	*dlgwnd;
			GUI::Titlebar	*titlebar;
			GUI::Divider	*divbar;
			GUI::Button	*okbtn;
			GUI::Button	*cancelbtn;
			GUI::Slider	*hueslider;
			GUI::Slider	*satslider;
			GUI::Slider	*valslider;
			GUI::Slider	*redslider;
			GUI::Slider	*greenslider;
			GUI::Slider	*blueslider;
			GUI::Text	*huetext;
			GUI::Text	*sattext;
			GUI::Text	*valtext;
			GUI::Text	*redtext;
			GUI::Text	*greentext;
			GUI::Text	*bluetext;
			GUI::Text	*hextext;
			GUI::EditBox	*hueedit;
			GUI::EditBox	*satedit;
			GUI::EditBox	*valedit;
			GUI::EditBox	*rededit;
			GUI::EditBox	*greenedit;
			GUI::EditBox	*blueedit;
			GUI::EditBox	*hexedit;

			Void		 ColorDlgUpdateHexValue();
			Void		 ColorDlgHexValueChanged();

			Void		 ColorDlgPaintProc();
			Void		 ColorDlgMessageProc(Int, Int, Int);
			Void		 ColorDlgUpdatePickers();
			Bool		 ColorDlgKillProc();
			Void		 ColorDlgOK();
			Void		 ColorDlgCancel();
			Void		 ColorDlgHueSlider();
			Void		 ColorDlgSatSlider();
			Void		 ColorDlgValSlider();
			Void		 ColorDlgRedSlider();
			Void		 ColorDlgGreenSlider();
			Void		 ColorDlgBlueSlider();
			Void		 ColorDlgHueEdit();
			Void		 ColorDlgSatEdit();
			Void		 ColorDlgValEdit();
			Void		 ColorDlgRedEdit();
			Void		 ColorDlgGreenEdit();
			Void		 ColorDlgBlueEdit();
			Void		 ColorDlgHexEdit();
		public:
					 DialogColorSelection();
					~DialogColorSelection();

			Int		 ShowDialog();

			Int		 GetColor();
			Int		 SetColor(Int);
	};
};

#endif
