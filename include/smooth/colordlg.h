 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_COLORDLG_
#define _H_OBJSMOOTH_COLORDLG_

namespace smooth
{
	class DialogColorSelection;
	class Window;
	class Button;
	class EditBox;
	class Text;
	class Slider;
	class Titlebar;
	class Divider;
};

#include "definitions.h"
#include "dialog.h"

namespace smooth
{
	class SMOOTHAPI DialogColorSelection : public Dialog
	{
		private:
			int		 color;

			int		 yoffset;
			int		 huexoffset;
			int		 ncxoffset;
			int		 ocxoffset;
			int		 crsizex;
			int		 crsizey;

			bool		 huecapt;
			bool		 vscapt;

			bool		 forcehupdate;
			bool		 forcevsupdate;
			bool		 preventhupdate;
			bool		 preventvsupdate;
			bool		 updatetext;
			bool		 updatehextext;

			int		 lasthue;
			int		 lastsat;
			int		 lastval;

			Int		 acthue;
			Int		 actsat;
			Int		 actval;
			Int		 actred;
			Int		 actgreen;
			Int		 actblue;

			int		 colortable[256][256];

			String		 hexval;

			Window		*dlgwnd;
			Titlebar	*titlebar;
			Divider		*divbar;
			Button		*okbtn;
			Button		*cancelbtn;
			Slider		*hueslider;
			Slider		*satslider;
			Slider		*valslider;
			Slider		*redslider;
			Slider		*greenslider;
			Slider		*blueslider;
			Text		*huetext;
			Text		*sattext;
			Text		*valtext;
			Text		*redtext;
			Text		*greentext;
			Text		*bluetext;
			Text		*hextext;
			EditBox		*hueedit;
			EditBox		*satedit;
			EditBox		*valedit;
			EditBox		*rededit;
			EditBox		*greenedit;
			EditBox		*blueedit;
			EditBox		*hexedit;

			void		 ColorDlgUpdateHexValue();
			void		 ColorDlgHexValueChanged();

			void		 ColorDlgPaintProc();
			void		 ColorDlgMessageProc(Int, Int, Int);
			void		 ColorDlgUpdatePickers();
			Bool		 ColorDlgKillProc();
			void		 ColorDlgOK();
			void		 ColorDlgCancel();
			void		 ColorDlgHueSlider();
			void		 ColorDlgSatSlider();
			void		 ColorDlgValSlider();
			void		 ColorDlgRedSlider();
			void		 ColorDlgGreenSlider();
			void		 ColorDlgBlueSlider();
			void		 ColorDlgHueEdit();
			void		 ColorDlgSatEdit();
			void		 ColorDlgValEdit();
			void		 ColorDlgRedEdit();
			void		 ColorDlgGreenEdit();
			void		 ColorDlgBlueEdit();
			void		 ColorDlgHexEdit();
		public:
					 DialogColorSelection();
					~DialogColorSelection();

			Int		 ShowDialog();

			Int		 GetColor();
			Int		 SetColor(Int);
	};
};

#endif
