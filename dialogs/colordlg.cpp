 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_COLORDLG_
#define __OBJSMOOTH_COLORDLG_

#include <smooth/colordlg.h>
#include <smooth/application.h>
#include <smooth/window.h>
#include <smooth/titlebar.h>
#include <smooth/i18n.h>
#include <smooth/toolkit.h>
#include <smooth/color.h>
#include <smooth/stk.h>
#include <smooth/layer.h>
#include <smooth/button.h>
#include <smooth/definitions.h>
#include <smooth/divisionbar.h>
#include <smooth/slider.h>
#include <smooth/text.h>
#include <smooth/editbox.h>
#include <smooth/string.h>
#include <smooth/metrics.h>
#include <smooth/loop.h>
#include <smooth/mathtools.h>
#include <smooth/objectproperties.h>

#include <picture.h>

SMOOTHDialogColorSelection::SMOOTHDialogColorSelection()
{
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			colortable[x][y] = ConvertColor(HSV, RGB, RGB(x, y, 255));
		}
	}

	SMOOTHPoint	 bp;
	SMOOTHSize	 bs;

	color = 0;

	forcehupdate = false;
	forcevsupdate = false;
	preventhupdate = false;
	preventvsupdate = false;
	updatetext = true;
	updatehextext = true;

	yoffset = roundtoint(32 * SMOOTH::Setup::FontSize);
	huexoffset = roundtoint(219 * SMOOTH::Setup::FontSize);
	ncxoffset = roundtoint(242 * SMOOTH::Setup::FontSize);
	ocxoffset = roundtoint(338 * SMOOTH::Setup::FontSize);
	crsizex = roundtoint(90 * SMOOTH::Setup::FontSize);
	crsizey = roundtoint(22 * SMOOTH::Setup::FontSize);

	acthue = GetRed(ConvertColor(RGB, HSV, color));
	actsat = GetGreen(ConvertColor(RGB, HSV, color));
	actval = GetBlue(ConvertColor(RGB, HSV, color));
	actred = GetRed(color);
	actgreen = GetGreen(color);
	actblue = GetBlue(color);

	ColorDlgUpdateHexValue();

	bp.x = 87;
	bp.y = 29;
	bs.cx = 0;
	bs.cy = 0;

	dlgwnd = new SMOOTHWindow(TXT_COLORSELECTION);

	titlebar	= new SMOOTHTitlebar(false, false, true);
	divbar		= new SMOOTHDivisionbar(42, OR_HORZ | OR_BOTTOM);
	layer		= new SMOOTHLayer();
	okbtn		= new SMOOTHButton(TXT_OK, NIL, bp, bs, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgOK));
	okbtn->SetOrientation(OR_LOWERRIGHT);

	bp.x = 175;

	cancelbtn = new SMOOTHButton(TXT_CANCEL, NIL, bp, bs, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgCancel));
	cancelbtn->SetOrientation(OR_LOWERRIGHT);

	bp.x = 174;
	bp.y = 35;
	bs.cx = 129;
	bs.cy = 0;

	hueslider = new SMOOTHSlider(bp, bs, OR_HORZ, &acthue, 0, 255, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgHueSlider));
	hueslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	satslider = new SMOOTHSlider(bp, bs, OR_HORZ, &actsat, 0, 255, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgSatSlider));
	satslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	valslider = new SMOOTHSlider(bp, bs, OR_HORZ, &actval, 0, 255, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgValSlider));
	valslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	redslider = new SMOOTHSlider(bp, bs, OR_HORZ, &actred, 0, 255, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgRedSlider));
	redslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	greenslider = new SMOOTHSlider(bp, bs, OR_HORZ, &actgreen, 0, 255, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgGreenSlider));
	greenslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	blueslider = new SMOOTHSlider(bp, bs, OR_HORZ, &actblue, 0, 255, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgBlueSlider));
	blueslider->SetOrientation(OR_UPPERRIGHT);

	bp.x = 189;
	bp.y = 37;

	huetext = new SMOOTHText(TXT_HUESHORT, bp);
	huetext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	huetext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, SMOOTH::Setup::TextColor, FW_NORMAL);
#endif

	bp.y += 26;

	sattext = new SMOOTHText(TXT_SATURATIONSHORT, bp);
	sattext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	sattext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, SMOOTH::Setup::TextColor, FW_NORMAL);
#endif

	bp.y += 26;

	valtext = new SMOOTHText(TXT_VALUESHORT, bp);
	valtext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32
	valtext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, SMOOTH::Setup::TextColor, FW_NORMAL);
#endif

	bp.y += 26;

	redtext = new SMOOTHText(TXT_REDSHORT, bp);
	redtext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	redtext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, RGB(255, 0, 0), FW_NORMAL);
#endif

	bp.y += 26;

	greentext = new SMOOTHText(TXT_GREENSHORT, bp);
	greentext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	greentext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, RGB(0, 160, 0), FW_NORMAL);
#endif

	bp.y += 26;

	bluetext = new SMOOTHText(TXT_BLUESHORT, bp);
	bluetext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	bluetext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, RGB(0, 0, 255), FW_NORMAL);
#endif

	bp.y += 26;

	hextext = new SMOOTHText(TXT_HTMLCODE, bp);
	hextext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	hextext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, SMOOTH::Setup::TextColor, FW_NORMAL);
#endif

	bp.x = 37;
	bp.y = 34;
	bs.cx = 30;
	bs.cy = 0;

	hueedit = new SMOOTHEditBox(SMOOTHString::IntToString(acthue), bp, bs, EDB_NUMERIC, 3, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgHueEdit));
	hueedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	satedit = new SMOOTHEditBox(SMOOTHString::IntToString(actsat), bp, bs, EDB_NUMERIC, 3, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgSatEdit));
	satedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	valedit = new SMOOTHEditBox(SMOOTHString::IntToString(actval), bp, bs, EDB_NUMERIC, 3, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgValEdit));
	valedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	rededit = new SMOOTHEditBox(SMOOTHString::IntToString(actred), bp, bs, EDB_NUMERIC, 3, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgRedEdit));
	rededit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	greenedit = new SMOOTHEditBox(SMOOTHString::IntToString(actgreen), bp, bs, EDB_NUMERIC, 3, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgGreenEdit));
	greenedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	blueedit = new SMOOTHEditBox(SMOOTHString::IntToString(actblue), bp, bs, EDB_NUMERIC, 3, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgBlueEdit));
	blueedit->SetOrientation(OR_UPPERRIGHT);

	bp.x += 40;
	bp.y += 26;
	bs.cx += 40;

	hexedit = new SMOOTHEditBox(hexval, bp, bs, EDB_ALPHANUMERIC, 7, SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgHexEdit));
	hexedit->SetOrientation(OR_UPPERRIGHT);

	huecapt = false;
	vscapt = false;

	lasthue = -1;
	lastsat = -1;
	lastval = -1;

	RegisterObject(dlgwnd);
	layer->RegisterObject(okbtn);
	layer->RegisterObject(cancelbtn);
	layer->RegisterObject(hueslider);
	layer->RegisterObject(satslider);
	layer->RegisterObject(valslider);
	layer->RegisterObject(redslider);
	layer->RegisterObject(greenslider);
	layer->RegisterObject(blueslider);
	layer->RegisterObject(huetext);
	layer->RegisterObject(sattext);
	layer->RegisterObject(valtext);
	layer->RegisterObject(redtext);
	layer->RegisterObject(greentext);
	layer->RegisterObject(bluetext);
	layer->RegisterObject(hueedit);
	layer->RegisterObject(satedit);
	layer->RegisterObject(valedit);
	layer->RegisterObject(rededit);
	layer->RegisterObject(greenedit);
	layer->RegisterObject(blueedit);
	layer->RegisterObject(hextext);
	layer->RegisterObject(hexedit);
	dlgwnd->RegisterObject(titlebar);
	dlgwnd->RegisterObject(layer);
	dlgwnd->RegisterObject(divbar);

	dlgwnd->value = 0;

	dlgwnd->SetMetrics(SMOOTHPoint(100, 100), SMOOTHSize(436, 286));
	dlgwnd->SetPaintProc(SMOOTHProc(SMOOTHDialogColorSelection, this, ColorDlgPaintProc));
	dlgwnd->SetMessageProc(SMOOTHMessageProc(SMOOTHDialogColorSelection, this, ColorDlgMessageProc));
	dlgwnd->SetKillProc(SMOOTHKillProc(SMOOTHDialogColorSelection, this, ColorDlgKillProc));
}

SMOOTHDialogColorSelection::~SMOOTHDialogColorSelection()
{
	dlgwnd->UnregisterObject(titlebar);
	dlgwnd->UnregisterObject(layer);
	dlgwnd->UnregisterObject(divbar);
	layer->UnregisterObject(okbtn);
	layer->UnregisterObject(cancelbtn);
	layer->UnregisterObject(hueslider);
	layer->UnregisterObject(satslider);
	layer->UnregisterObject(valslider);
	layer->UnregisterObject(redslider);
	layer->UnregisterObject(greenslider);
	layer->UnregisterObject(blueslider);
	layer->UnregisterObject(huetext);
	layer->UnregisterObject(sattext);
	layer->UnregisterObject(valtext);
	layer->UnregisterObject(redtext);
	layer->UnregisterObject(greentext);
	layer->UnregisterObject(bluetext);
	layer->UnregisterObject(hueedit);
	layer->UnregisterObject(satedit);
	layer->UnregisterObject(valedit);
	layer->UnregisterObject(rededit);
	layer->UnregisterObject(greenedit);
	layer->UnregisterObject(blueedit);
	layer->UnregisterObject(hextext);
	layer->UnregisterObject(hexedit);
	UnregisterObject(dlgwnd);

	delete titlebar;
	delete divbar;
	delete dlgwnd;
	delete layer;
	delete okbtn;
	delete cancelbtn;
	delete hueslider;
	delete satslider;
	delete valslider;
	delete redslider;
	delete greenslider;
	delete blueslider;
	delete huetext;
	delete sattext;
	delete valtext;
	delete redtext;
	delete greentext;
	delete bluetext;
	delete hueedit;
	delete satedit;
	delete valedit;
	delete rededit;
	delete greenedit;
	delete blueedit;
	delete hextext;
	delete hexedit;
}

SMOOTHInt SMOOTHDialogColorSelection::ShowDialog()
{
	if (parentWindow != NIL)	dlgwnd->SetMetrics(SMOOTHPoint(parentWindow->GetObjectProperties()->pos.x + 25, parentWindow->GetObjectProperties()->pos.y + 25), SMOOTHSize(436, 286));
	if (caption != NIL)		dlgwnd->SetText(caption);

	dlgwnd->Show();

	ColorDlgUpdatePickers();

	color = dlgwnd->Stay();

	return color;
}

SMOOTHInt SMOOTHDialogColorSelection::GetColor()
{
	return color;
}

SMOOTHInt SMOOTHDialogColorSelection::SetColor(SMOOTHInt newColor)
{
	color = newColor;

	acthue = GetRed(ConvertColor(RGB, HSV, color));
	actsat = GetGreen(ConvertColor(RGB, HSV, color));
	actval = GetBlue(ConvertColor(RGB, HSV, color));
	actred = GetRed(color);
	actgreen = GetGreen(color);
	actblue = GetBlue(color);

	ColorDlgUpdateHexValue();

	hueedit->SetText(SMOOTHString::IntToString(acthue));
	satedit->SetText(SMOOTHString::IntToString(actsat));
	valedit->SetText(SMOOTHString::IntToString(actval));
	rededit->SetText(SMOOTHString::IntToString(actred));
	greenedit->SetText(SMOOTHString::IntToString(actgreen));
	blueedit->SetText(SMOOTHString::IntToString(actblue));
	hexedit->SetText(hexval);

	return SMOOTH::Success;
}

SMOOTHBool SMOOTHDialogColorSelection::ColorDlgKillProc()
{
	if (dlgwnd->value == 0) dlgwnd->value = color;

	return true;
}

void SMOOTHDialogColorSelection::ColorDlgPaintProc()
{
	if (dlgwnd->hwnd == NIL) return;

	HDC		 dc = GetContext(dlgwnd);
	SMOOTHRect	 rect;
	SMOOTHPoint	 p1;
	SMOOTHPoint	 p2;
	picture		*pic = new picture(256, 256, 24);
	SMOOTHRect	 urect = dlgwnd->GetUpdateRect();
	SMOOTHRect	 irect;
	int		 hssize = roundtoint(205 * SMOOTH::Setup::FontSize);
	double		 hue = 0;
	double		 huebias = 256 / (double) hssize;
	register int	 xmin = 0;
	register int	 ymin = 0;
	register int	 xmax = hssize;
	register int	 ymax = hssize;
	register int	 normrgb;
	register double	 rbias;
	register double	 gbias;
	register double	 bbias;
	register double  ared;
	register double  agreen;
	register double  ablue;

	rect.left	= huexoffset;
	rect.top	= yoffset;
	rect.right	= rect.left + 17;
	rect.bottom	= rect.top + hssize + 1;

	if (DoRectsOverlap(rect, urect))
	{
		Frame(dc, rect, FRAME_DOWN);

		for (int ypos = 0; ypos < hssize; ypos++)
		{
			p1.x = huexoffset + 1;
			p1.y = yoffset + 1 + ypos;
			p2.x = huexoffset + 17;
			p2.y = yoffset + 1 + ypos;

			Line(dc, p1, p2, ConvertColor(HSV, RGB, RGB(255 - roundtoint(hue), 255, 255)), PS_SOLID, 1);

			hue += huebias;
		}
	}

	rect.left	= 7;
	rect.right	= rect.left + hssize + 1;

	Frame(dc, rect, FRAME_DOWN);

	rect.left = ncxoffset;
	rect.top = yoffset;
	rect.right = rect.left + crsizex;
	rect.bottom = rect.top + crsizey;

	Frame(dc, rect, FRAME_DOWN);

	rect.left++;
	rect.top++;

	Box(dc, rect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);

	rect.left = ocxoffset;
	rect.right = rect.left + crsizex;
	rect.top--;

	Frame(dc, rect, FRAME_DOWN);

	rect.left++;
	rect.top++;

	Box(dc, rect, color, FILLED);

	rect.left	= 3;
	rect.top	= yoffset - 4;
	rect.right	= rect.left + hssize;
	rect.bottom	= rect.top + hssize;

	forcehupdate = true;
	preventvsupdate = true;

	ColorDlgUpdatePickers();

	if (DoRectsOverlap(rect, urect))
	{
		irect = OverlapRect(rect, urect);

		irect.right += 5;
		irect.bottom += 5;

		xmin = irect.left - 8;
		xmax = irect.right - 8;
		ymin = irect.top - (yoffset + 1);
		ymax = irect.bottom - (yoffset + 1);

		rect.left	= xmin;
		rect.top	= ymin;
		rect.right	= xmax;
		rect.bottom	= ymax;

		for (register int sat = max(0, ymin); sat < min(hssize, ymax); sat++)
		{
			normrgb = colortable[acthue][255 - roundtoint(sat * (256 / (205 * SMOOTH::Setup::FontSize)))];

			rbias = (double) GetRed(normrgb) / (255 / (256 / (205 * SMOOTH::Setup::FontSize)));
			gbias = (double) GetGreen(normrgb) / (255 / (256 / (205 * SMOOTH::Setup::FontSize)));
			bbias = (double) GetBlue(normrgb) / (255 / (256 / (205 * SMOOTH::Setup::FontSize)));

			ared = -rbias + max(0, xmin) * rbias;
			agreen = -gbias + max(0, xmin) * gbias;
			ablue = -bbias + max(0, xmin) * bbias;

			for (register int val = max(0, xmin); val < min(hssize, xmax); val++)
			{
				pic->SetPixel(val, sat, RGB(roundtoint(ared += rbias), roundtoint(agreen += gbias), roundtoint(ablue += bbias)));
			}
		}

#ifdef __WIN32__
		{
			RECT	 iRect = irect;
			RECT	 Rect = rect;

			pic->BlitToDC(dc, &iRect, &Rect);

			rect = Rect;
			irect = iRect;
		}
#endif
	}

	forcevsupdate = true;
	preventhupdate = true;

	ColorDlgUpdatePickers();

	FreeContext(dlgwnd, dc);

	delete pic;
}

void SMOOTHDialogColorSelection::ColorDlgMessageProc(SMOOTHInt message, SMOOTHInt wparam, SMOOTHInt lparam)
{
	if (dlgwnd->hwnd == NIL) return;

	SMOOTHRect	 huerect;
	SMOOTHRect	 vsrect;
	SMOOTHRect	 ncrect;
	SMOOTHRect	 ocrect;
	HDC		 dc = GetContext(dlgwnd);
	int		 newval;
	int		 newsat;

	int		 newhue;

#ifdef __WIN32__
	int		 leftbutton;
#endif

	int		 hssize = roundtoint(205 * SMOOTH::Setup::FontSize);

	huerect.left	= huexoffset;
	huerect.top	= yoffset;
	huerect.right	= huerect.left + 17;
	huerect.bottom	= huerect.top + hssize + 1;

	vsrect.left	= 8;
	vsrect.top	= yoffset + 1;
	vsrect.right	= vsrect.left + hssize;
	vsrect.bottom	= vsrect.top + hssize;

	ncrect.left = ncxoffset + 1;
	ncrect.top = yoffset + 1;
	ncrect.right = ncrect.left + (crsizex - 1);
	ncrect.bottom = ncrect.top + (crsizey - 1);

	ocrect.left = ocxoffset + 1;
	ocrect.top = yoffset + 1;
	ocrect.right = ocrect.left + (crsizex - 1);
	ocrect.bottom = ocrect.top + (crsizey - 1);

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (IsMouseOn(dlgwnd->hwnd, huerect, WINDOW))
			{
				huecapt = true;

				ColorDlgMessageProc(SM_MOUSEMOVE, 0, 0);
			}
			else if (IsMouseOn(dlgwnd->hwnd, vsrect, WINDOW))
			{
				vscapt = true;

				ColorDlgMessageProc(SM_MOUSEMOVE, 0, 0);
			}
			else if (IsMouseOn(dlgwnd->hwnd, ocrect, WINDOW))
			{
				lasthue = acthue;
				lastsat = actsat;
				lastval = actval;

				acthue = GetRed(ConvertColor(RGB, HSV, color));
				actsat = GetGreen(ConvertColor(RGB, HSV, color));
				actval = GetBlue(ConvertColor(RGB, HSV, color));
				actred = GetRed(color);
				actgreen = GetGreen(color);
				actblue = GetBlue(color);

				ColorDlgUpdateHexValue();

				hueedit->SetText(SMOOTHString::IntToString(acthue));
				satedit->SetText(SMOOTHString::IntToString(actsat));
				valedit->SetText(SMOOTHString::IntToString(actval));
				rededit->SetText(SMOOTHString::IntToString(actred));
				greenedit->SetText(SMOOTHString::IntToString(actgreen));
				blueedit->SetText(SMOOTHString::IntToString(actblue));
				hexedit->SetText(hexval);

				dlgwnd->SetUpdateRect(vsrect);
				ColorDlgPaintProc();
			}
			break;
		case SM_LBUTTONUP:
			if (huecapt)
			{
				huecapt = false;

				dlgwnd->SetUpdateRect(vsrect);
				ColorDlgPaintProc();
			}
			else if (vscapt)
			{
				vscapt = false;
			}
			break;
		case SM_MOUSEMOVE:
			if (huecapt)
			{
#ifdef __WIN32__
				if (GetSystemMetrics(SM_SWAPBUTTON))	leftbutton = VK_RBUTTON;
				else					leftbutton = VK_LBUTTON;

				if (GetAsyncKeyState(leftbutton) == 0)
				{
					ColorDlgMessageProc(SM_LBUTTONUP, 0, 0);
					break;
				}
#endif

				newhue = 255 - roundtoint(max(min(MouseY(dlgwnd->hwnd, WINDOW) - (yoffset + 1), hssize - 1), 0) * (255 / (204 * SMOOTH::Setup::FontSize)));

				if (newhue != acthue)
				{
					lasthue = acthue;
					acthue = newhue;

					ColorDlgUpdatePickers();

					actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

					ColorDlgUpdateHexValue();

					hueedit->SetText(SMOOTHString::IntToString(acthue));
					rededit->SetText(SMOOTHString::IntToString(actred));
					greenedit->SetText(SMOOTHString::IntToString(actgreen));
					blueedit->SetText(SMOOTHString::IntToString(actblue));
					hexedit->SetText(hexval);

					Box(dc, ncrect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);
				}
			}
			else if (vscapt)
			{
#ifdef __WIN32__
				if (GetSystemMetrics(SM_SWAPBUTTON))	leftbutton = VK_RBUTTON;
				else					leftbutton = VK_LBUTTON;

				if (GetAsyncKeyState(leftbutton) == 0)
				{
					ColorDlgMessageProc(SM_LBUTTONUP, 0, 0);
					break;
				}
#endif

				newval = roundtoint(max(min(MouseX(dlgwnd->hwnd, WINDOW) - 8, hssize - 1), 0) * (255 / (204 * SMOOTH::Setup::FontSize)));
				newsat = 255 - roundtoint(max(min(MouseY(dlgwnd->hwnd, WINDOW) - (yoffset + 1), hssize - 1), 0) * (255 / (204 * SMOOTH::Setup::FontSize)));

				if ((newval != actval) || (newsat != actsat))
				{
					lastval = actval;
					lastsat = actsat;
					actval = newval;
					actsat = newsat;

					ColorDlgUpdatePickers();

					actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

					ColorDlgUpdateHexValue();

					valedit->SetText(SMOOTHString::IntToString(actval));
					satedit->SetText(SMOOTHString::IntToString(actsat));
					rededit->SetText(SMOOTHString::IntToString(actred));
					greenedit->SetText(SMOOTHString::IntToString(actgreen));
					blueedit->SetText(SMOOTHString::IntToString(actblue));
					hexedit->SetText(hexval);

					Box(dc, ncrect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);
				}
			}
			break;
	}

	FreeContext(dlgwnd, dc);
}

void SMOOTHDialogColorSelection::ColorDlgUpdatePickers()
{
	if (dlgwnd->hwnd == NIL) return;

	SMOOTHPoint	 p1;
	SMOOTHPoint	 p2;
	HDC		 dc = GetContext(dlgwnd);
	int		 ahrgb = ConvertColor(HSV, RGB, RGB(acthue, 255, 255));
	int		 rgb;
	int		 hssize = roundtoint(205 * SMOOTH::Setup::FontSize);

	if (((lasthue != acthue) || forcehupdate) && !preventhupdate)
	{
		// update hue picker
		if (lasthue != -1)
		{
			p1.x = huexoffset + 1;
			p1.y = yoffset + 1 + (int) ((255 - lasthue) / (256 / (205 * SMOOTH::Setup::FontSize)));
			p2.x = huexoffset + 17;
			p2.y = yoffset + 1 + (int) ((255 - lasthue) / (256 / (205 * SMOOTH::Setup::FontSize)));

			Line(dc, p1, p2, ConvertColor(HSV, RGB, RGB(lasthue, 255, 255)), PS_SOLID, 1);
		}

		for (int x = huexoffset + 1; x < (huexoffset + 17); x++)
		{
			PaintPixel(dc, SMOOTHPoint(x, yoffset + 1 + (int) ((255 - acthue) / (256 / (205 * SMOOTH::Setup::FontSize)))), RGB(255-GetRed(ahrgb), 255-GetGreen(ahrgb), 255-GetBlue(ahrgb)));
		}

		lasthue = acthue;
	}

	if (((lastval != actval) || (lastsat != actsat) || forcevsupdate) && !preventvsupdate)
	{
		// update val/sat picker
		if ((lastval != -1) && (lastsat != -1))
		{
			for (int x = 0; x < hssize; x++)
			{
				rgb = ConvertColor(HSV, RGB, RGB(acthue, lastsat, (int) (x * (256 / (205 * SMOOTH::Setup::FontSize)))));

				PaintPixel(dc, SMOOTHPoint(x + 8, yoffset + 1 + (int) ((255 - lastsat) / (256 / (205 * SMOOTH::Setup::FontSize)))), rgb);
			}

			for (int y = 0; y < hssize; y++)
			{
				rgb = ConvertColor(HSV, RGB, RGB(acthue, (int) (255 - (y * (256 / (205 * SMOOTH::Setup::FontSize)))), lastval));

				PaintPixel(dc, SMOOTHPoint(8 + (int) (lastval / (256 / (205 * SMOOTH::Setup::FontSize))), y + yoffset + 1), rgb);
			}
		}

		for (int x = 0; x < hssize; x++)
		{
			rgb = ConvertColor(HSV, RGB, RGB(acthue, actsat, x));

			PaintPixel(dc, SMOOTHPoint(x + 8, yoffset + 1 + (int) ((255 - actsat) / (256 / (205 * SMOOTH::Setup::FontSize)))), RGB(255-GetRed(rgb), 255-GetGreen(rgb), 255-GetBlue(rgb)));
		}

		for (int y = 0; y < hssize; y++)
		{
			rgb = ConvertColor(HSV, RGB, RGB(acthue, 255 - y, actval));

			PaintPixel(dc, SMOOTHPoint(8 + (int) (actval / (256 / (205 * SMOOTH::Setup::FontSize))), y + yoffset + 1), RGB(255-GetRed(rgb), 255-GetGreen(rgb), 255-GetBlue(rgb)));
		}

		lastval = actval;
		lastsat = actsat;
	}

	forcehupdate = false;
	preventhupdate = false;
	forcevsupdate = false;
	preventvsupdate = false;

	FreeContext(dlgwnd, dc);
}

void SMOOTHDialogColorSelection::ColorDlgOK()
{
	dlgwnd->value = ConvertColor(HSV, RGB, RGB(acthue, actsat, actval));

	SMOOTH::CloseWindow(dlgwnd);
}

void SMOOTHDialogColorSelection::ColorDlgCancel()
{
	dlgwnd->value = color;

	SMOOTH::CloseWindow(dlgwnd);
}

void SMOOTHDialogColorSelection::ColorDlgHueSlider()
{
	SMOOTHRect	 vsrect;
	HDC		 dc = GetContext(dlgwnd);
	int		 hssize = roundtoint(205 * SMOOTH::Setup::FontSize);

	vsrect.left	= 8;
	vsrect.top	= yoffset + 1;
	vsrect.right	= vsrect.left + hssize;
	vsrect.bottom	= vsrect.top + hssize;

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		hueedit->SetText(SMOOTHString::IntToString(acthue));
		rededit->SetText(SMOOTHString::IntToString(actred));
		greenedit->SetText(SMOOTHString::IntToString(actgreen));
		blueedit->SetText(SMOOTHString::IntToString(actblue));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	dlgwnd->SetUpdateRect(vsrect);
	ColorDlgPaintProc();

	FreeContext(dlgwnd, dc);
}

void SMOOTHDialogColorSelection::ColorDlgSatSlider()
{
	SMOOTHRect	 ncrect;
	HDC		 dc = GetContext(dlgwnd);

	ncrect.left = ncxoffset + 1;
	ncrect.top = yoffset + 1;
	ncrect.right = ncrect.left + (crsizex - 1);
	ncrect.bottom = ncrect.top + (crsizey - 1);

	ColorDlgUpdatePickers();

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		satedit->SetText(SMOOTHString::IntToString(actsat));
		rededit->SetText(SMOOTHString::IntToString(actred));
		greenedit->SetText(SMOOTHString::IntToString(actgreen));
		blueedit->SetText(SMOOTHString::IntToString(actblue));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	Box(dc, ncrect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);

	FreeContext(dlgwnd, dc);
}

void SMOOTHDialogColorSelection::ColorDlgValSlider()
{
	SMOOTHRect	 ncrect;
	HDC		 dc = GetContext(dlgwnd);

	ncrect.left = ncxoffset + 1;
	ncrect.top = yoffset + 1;
	ncrect.right = ncrect.left + (crsizex - 1);
	ncrect.bottom = ncrect.top + (crsizey - 1);

	ColorDlgUpdatePickers();

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		valedit->SetText(SMOOTHString::IntToString(actval));
		rededit->SetText(SMOOTHString::IntToString(actred));
		greenedit->SetText(SMOOTHString::IntToString(actgreen));
		blueedit->SetText(SMOOTHString::IntToString(actblue));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	Box(dc, ncrect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);

	FreeContext(dlgwnd, dc);
}

void SMOOTHDialogColorSelection::ColorDlgRedSlider()
{
	SMOOTHRect	 vsrect;
	HDC		 dc = GetContext(dlgwnd);
	int		 hssize = roundtoint(205 * SMOOTH::Setup::FontSize);

	vsrect.left	= 8;
	vsrect.top	= yoffset + 1;
	vsrect.right	= vsrect.left + hssize;
	vsrect.bottom	= vsrect.top + hssize;

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(SMOOTHString::IntToString(acthue));
		satedit->SetText(SMOOTHString::IntToString(actsat));
		valedit->SetText(SMOOTHString::IntToString(actval));
		rededit->SetText(SMOOTHString::IntToString(actred));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	dlgwnd->SetUpdateRect(vsrect);
	ColorDlgPaintProc();

	FreeContext(dlgwnd, dc);
}

void SMOOTHDialogColorSelection::ColorDlgGreenSlider()
{
	SMOOTHRect	 vsrect;
	HDC		 dc = GetContext(dlgwnd);
	int		 hssize = roundtoint(205 * SMOOTH::Setup::FontSize);

	vsrect.left	= 8;
	vsrect.top	= yoffset + 1;
	vsrect.right	= vsrect.left + hssize;
	vsrect.bottom	= vsrect.top + hssize;

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(SMOOTHString::IntToString(acthue));
		satedit->SetText(SMOOTHString::IntToString(actsat));
		valedit->SetText(SMOOTHString::IntToString(actval));
		greenedit->SetText(SMOOTHString::IntToString(actgreen));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	dlgwnd->SetUpdateRect(vsrect);
	ColorDlgPaintProc();

	FreeContext(dlgwnd, dc);
}

void SMOOTHDialogColorSelection::ColorDlgBlueSlider()
{
	SMOOTHRect	 vsrect;
	HDC		 dc = GetContext(dlgwnd);
	int		 hssize = roundtoint(205 * SMOOTH::Setup::FontSize);

	vsrect.left	= 8;
	vsrect.top	= yoffset + 1;
	vsrect.right	= vsrect.left + hssize;
	vsrect.bottom	= vsrect.top + hssize;

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(SMOOTHString::IntToString(acthue));
		satedit->SetText(SMOOTHString::IntToString(actsat));
		valedit->SetText(SMOOTHString::IntToString(actval));
		blueedit->SetText(SMOOTHString::IntToString(actblue));
	}

	updatetext = true;

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;

	dlgwnd->SetUpdateRect(vsrect);
	ColorDlgPaintProc();

	FreeContext(dlgwnd, dc);
}

void SMOOTHDialogColorSelection::ColorDlgHueEdit()
{
	int newhue = max(0, min(255, hueedit->GetText().ToInt()));

	if (newhue != acthue)
	{
		acthue = newhue;

		updatetext = false;

		ColorDlgHueSlider();
	}
}

void SMOOTHDialogColorSelection::ColorDlgSatEdit()
{
	int newsat = max(0, min(255, satedit->GetText().ToInt()));

	if (newsat != actsat)
	{
		actsat = newsat;

		updatetext = false;

		ColorDlgSatSlider();
	}
}

void SMOOTHDialogColorSelection::ColorDlgValEdit()
{
	int newval = max(0, min(255, valedit->GetText().ToInt()));

	if (newval != actval)
	{
		actval = newval;

		updatetext = false;

		ColorDlgValSlider();
	}
}

void SMOOTHDialogColorSelection::ColorDlgRedEdit()
{
	int newred = max(0, min(255, rededit->GetText().ToInt()));

	if (newred != GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actred = newred;

		updatetext = false;

		ColorDlgRedSlider();
	}
}

void SMOOTHDialogColorSelection::ColorDlgGreenEdit()
{
	int newgreen = max(0, min(255, greenedit->GetText().ToInt()));

	if (newgreen != GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actgreen = newgreen;

		updatetext = false;

		ColorDlgGreenSlider();
	}
}

void SMOOTHDialogColorSelection::ColorDlgBlueEdit()
{
	int newblue = max(0, min(255, blueedit->GetText().ToInt()));

	if (newblue != GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actblue = newblue;

		updatetext = false;

		ColorDlgBlueSlider();
	}
}

void SMOOTHDialogColorSelection::ColorDlgUpdateHexValue()
{
	hexval[0] = '#';
	hexval[1] = 48 + (actred / 16);
	hexval[2] = 48 + (actred % 16);
	hexval[3] = 48 + (actgreen / 16);
	hexval[4] = 48 + (actgreen % 16);
	hexval[5] = 48 + (actblue / 16);
	hexval[6] = 48 + (actblue % 16);

	for (int i = 1; i < 7; i++)
	{
		if (hexval[i] > 57) hexval[i] = 'a' + (hexval[i] - 58);
	}
}

void SMOOTHDialogColorSelection::ColorDlgHexValueChanged()
{
}

void SMOOTHDialogColorSelection::ColorDlgHexEdit()
{
}

#endif
