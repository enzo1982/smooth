 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_COLORDLG_
#define _H_OBJSMOOTH_COLORDLG_

#define SDialogColorSelection SMOOTHDialogColorSelection

class SMOOTHDialogColorSelection;
class SMOOTHWindow;
class SMOOTHLayer;
class SMOOTHButton;
class SMOOTHEditBox;
class SMOOTHText;
class SMOOTHSlider;
class SMOOTHTitlebar;
class SMOOTHDivisionbar;

#include "definitions.h"
#include "dialog.h"

class SMOOTHAPI SMOOTHDialogColorSelection : public SMOOTHDialog
{
	private:
		int			 color;

		int			 yoffset;
		int			 huexoffset;
		int			 ncxoffset;
		int			 ocxoffset;
		int			 crsizex;
		int			 crsizey;

		bool			 huecapt;
		bool			 vscapt;

		bool			 forcehupdate;
		bool			 forcevsupdate;
		bool			 preventhupdate;
		bool			 preventvsupdate;
		bool			 updatetext;
		bool			 updatehextext;

		int			 lasthue;
		int			 lastsat;
		int			 lastval;

		SMOOTHInt		 acthue;
		SMOOTHInt		 actsat;
		SMOOTHInt		 actval;
		SMOOTHInt		 actred;
		SMOOTHInt		 actgreen;
		SMOOTHInt		 actblue;

		int			 colortable[256][256];

		SMOOTHString		 hexval;

		SMOOTHWindow		*dlgwnd;
		SMOOTHTitlebar		*titlebar;
		SMOOTHDivisionbar	*divbar;
		SMOOTHLayer		*layer;
		SMOOTHButton		*okbtn;
		SMOOTHButton		*cancelbtn;
		SMOOTHSlider		*hueslider;
		SMOOTHSlider		*satslider;
		SMOOTHSlider		*valslider;
		SMOOTHSlider		*redslider;
		SMOOTHSlider		*greenslider;
		SMOOTHSlider		*blueslider;
		SMOOTHText		*huetext;
		SMOOTHText		*sattext;
		SMOOTHText		*valtext;
		SMOOTHText		*redtext;
		SMOOTHText		*greentext;
		SMOOTHText		*bluetext;
		SMOOTHText		*hextext;
		SMOOTHEditBox		*hueedit;
		SMOOTHEditBox		*satedit;
		SMOOTHEditBox		*valedit;
		SMOOTHEditBox		*rededit;
		SMOOTHEditBox		*greenedit;
		SMOOTHEditBox		*blueedit;
		SMOOTHEditBox		*hexedit;

		void			 ColorDlgUpdateHexValue();
		void			 ColorDlgHexValueChanged();

		void			 ColorDlgPaintProc();
		void			 ColorDlgMessageProc(SMOOTHInt, SMOOTHInt, SMOOTHInt);
		void			 ColorDlgUpdatePickers();
		SMOOTHBool		 ColorDlgKillProc();
		void			 ColorDlgOK();
		void			 ColorDlgCancel();
		void			 ColorDlgHueSlider();
		void			 ColorDlgSatSlider();
		void			 ColorDlgValSlider();
		void			 ColorDlgRedSlider();
		void			 ColorDlgGreenSlider();
		void			 ColorDlgBlueSlider();
		void			 ColorDlgHueEdit();
		void			 ColorDlgSatEdit();
		void			 ColorDlgValEdit();
		void			 ColorDlgRedEdit();
		void			 ColorDlgGreenEdit();
		void			 ColorDlgBlueEdit();
		void			 ColorDlgHexEdit();
	public:
					 SMOOTHDialogColorSelection();
					~SMOOTHDialogColorSelection();

		SMOOTHInt		 ShowDialog();

		SMOOTHInt		 GetColor();
		SMOOTHInt		 SetColor(SMOOTHInt);
};

#endif
