 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/colordlg.h>
#include <smooth/application.h>
#include <smooth/window.h>
#include <smooth/titlebar.h>
#include <smooth/i18n.h>
#include <smooth/toolkit.h>
#include <smooth/color.h>
#include <smooth/stk.h>
#include <smooth/button.h>
#include <smooth/definitions.h>
#include <smooth/divider.h>
#include <smooth/slider.h>
#include <smooth/text.h>
#include <smooth/editbox.h>
#include <smooth/string.h>
#include <smooth/metrics.h>
#include <smooth/loop.h>
#include <smooth/math.h>
#include <smooth/objectproperties.h>
#include <smooth/surface.h>

#include <picture.h>

using namespace smooth::GUI;

S::DialogColorSelection::DialogColorSelection()
{
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			colortable[x][y] = ConvertColor(HSV, RGB, RGB(x, y, 255));
		}
	}

	Point	 bp;
	Size	 bs;

	color = 0;

	forcehupdate = false;
	forcevsupdate = false;
	preventhupdate = false;
	preventvsupdate = false;
	updatetext = true;
	updatehextext = true;

	yoffset = Math::Round(32 * Setup::FontSize);
	huexoffset = Math::Round(219 * Setup::FontSize);
	ncxoffset = Math::Round(242 * Setup::FontSize);
	ocxoffset = Math::Round(338 * Setup::FontSize);
	crsizex = Math::Round(90 * Setup::FontSize);
	crsizey = Math::Round(22 * Setup::FontSize);

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

	dlgwnd = new Window(TXT_COLORSELECTION);

	titlebar	= new Titlebar(false, false, true);
	divbar		= new Divider(42, OR_HORZ | OR_BOTTOM);
	okbtn		= new Button(TXT_OK, NIL, bp, bs);
	okbtn->onClick.Connect(&DialogColorSelection::ColorDlgOK, this);
	okbtn->SetOrientation(OR_LOWERRIGHT);

	bp.x = 175;

	cancelbtn = new Button(TXT_CANCEL, NIL, bp, bs);
	cancelbtn->onClick.Connect(&DialogColorSelection::ColorDlgCancel, this);
	cancelbtn->SetOrientation(OR_LOWERRIGHT);

	bp.x = 174;
	bp.y = 35;
	bs.cx = 129;
	bs.cy = 0;

	hueslider = new Slider(bp, bs, OR_HORZ, &acthue, 0, 255);
	hueslider->onClick.Connect(&DialogColorSelection::ColorDlgHueSlider, this);
	hueslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	satslider = new Slider(bp, bs, OR_HORZ, &actsat, 0, 255);
	satslider->onClick.Connect(&DialogColorSelection::ColorDlgSatSlider, this);
	satslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	valslider = new Slider(bp, bs, OR_HORZ, &actval, 0, 255);
	valslider->onClick.Connect(&DialogColorSelection::ColorDlgValSlider, this);
	valslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	redslider = new Slider(bp, bs, OR_HORZ, &actred, 0, 255);
	redslider->onClick.Connect(&DialogColorSelection::ColorDlgRedSlider, this);
	redslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	greenslider = new Slider(bp, bs, OR_HORZ, &actgreen, 0, 255);
	greenslider->onClick.Connect(&DialogColorSelection::ColorDlgGreenSlider, this);
	greenslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	blueslider = new Slider(bp, bs, OR_HORZ, &actblue, 0, 255);
	blueslider->onClick.Connect(&DialogColorSelection::ColorDlgBlueSlider, this);
	blueslider->SetOrientation(OR_UPPERRIGHT);

	bp.x = 189;
	bp.y = 37;

	huetext = new Text(TXT_HUESHORT, bp);
	huetext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	huetext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, Setup::TextColor, FW_NORMAL);
#endif

	bp.y += 26;

	sattext = new Text(TXT_SATURATIONSHORT, bp);
	sattext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	sattext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, Setup::TextColor, FW_NORMAL);
#endif

	bp.y += 26;

	valtext = new Text(TXT_VALUESHORT, bp);
	valtext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32
	valtext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, Setup::TextColor, FW_NORMAL);
#endif

	bp.y += 26;

	redtext = new Text(TXT_REDSHORT, bp);
	redtext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	redtext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, RGB(255, 0, 0), FW_NORMAL);
#endif

	bp.y += 26;

	greentext = new Text(TXT_GREENSHORT, bp);
	greentext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	greentext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, RGB(0, 160, 0), FW_NORMAL);
#endif

	bp.y += 26;

	bluetext = new Text(TXT_BLUESHORT, bp);
	bluetext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	bluetext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, RGB(0, 0, 255), FW_NORMAL);
#endif

	bp.y += 26;

	hextext = new Text(TXT_HTMLCODE, bp);
	hextext->SetOrientation(OR_UPPERRIGHT);

#ifdef __WIN32__
	hextext->SetFont(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, Setup::TextColor, FW_NORMAL);
#endif

	bp.x = 37;
	bp.y = 34;
	bs.cx = 30;
	bs.cy = 0;

	hueedit = new EditBox(String::IntToString(acthue), bp, bs, EDB_NUMERIC, 3);
	hueedit->onClick.Connect(&DialogColorSelection::ColorDlgHueEdit, this);
	hueedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	satedit = new EditBox(String::IntToString(actsat), bp, bs, EDB_NUMERIC, 3);
	satedit->onClick.Connect(&DialogColorSelection::ColorDlgSatEdit, this);
	satedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	valedit = new EditBox(String::IntToString(actval), bp, bs, EDB_NUMERIC, 3);
	valedit->onClick.Connect(&DialogColorSelection::ColorDlgValEdit, this);
	valedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	rededit = new EditBox(String::IntToString(actred), bp, bs, EDB_NUMERIC, 3);
	rededit->onClick.Connect(&DialogColorSelection::ColorDlgRedEdit, this);
	rededit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	greenedit = new EditBox(String::IntToString(actgreen), bp, bs, EDB_NUMERIC, 3);
	greenedit->onClick.Connect(&DialogColorSelection::ColorDlgGreenEdit, this);
	greenedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	blueedit = new EditBox(String::IntToString(actblue), bp, bs, EDB_NUMERIC, 3);
	blueedit->onClick.Connect(&DialogColorSelection::ColorDlgBlueEdit, this);
	blueedit->SetOrientation(OR_UPPERRIGHT);

	bp.x += 40;
	bp.y += 26;
	bs.cx += 40;

	hexedit = new EditBox(hexval, bp, bs, EDB_ALPHANUMERIC, 7);
	hexedit->onClick.Connect(&DialogColorSelection::ColorDlgHexEdit, this);
	hexedit->SetOrientation(OR_UPPERRIGHT);

	huecapt = false;
	vscapt = false;

	lasthue = -1;
	lastsat = -1;
	lastval = -1;

	RegisterObject(dlgwnd);
	dlgwnd->RegisterObject(okbtn);
	dlgwnd->RegisterObject(cancelbtn);
	dlgwnd->RegisterObject(hueslider);
	dlgwnd->RegisterObject(satslider);
	dlgwnd->RegisterObject(valslider);
	dlgwnd->RegisterObject(redslider);
	dlgwnd->RegisterObject(greenslider);
	dlgwnd->RegisterObject(blueslider);
	dlgwnd->RegisterObject(huetext);
	dlgwnd->RegisterObject(sattext);
	dlgwnd->RegisterObject(valtext);
	dlgwnd->RegisterObject(redtext);
	dlgwnd->RegisterObject(greentext);
	dlgwnd->RegisterObject(bluetext);
	dlgwnd->RegisterObject(hueedit);
	dlgwnd->RegisterObject(satedit);
	dlgwnd->RegisterObject(valedit);
	dlgwnd->RegisterObject(rededit);
	dlgwnd->RegisterObject(greenedit);
	dlgwnd->RegisterObject(blueedit);
	dlgwnd->RegisterObject(hextext);
	dlgwnd->RegisterObject(hexedit);
	dlgwnd->RegisterObject(titlebar);
	dlgwnd->RegisterObject(divbar);

	dlgwnd->value = 0;

	dlgwnd->SetMetrics(Point(100, 100), Size(436, 286));

	dlgwnd->onPaint.Connect(&DialogColorSelection::ColorDlgPaintProc, this);
	dlgwnd->onEvent.Connect(&DialogColorSelection::ColorDlgMessageProc, this);
	dlgwnd->doQuit.Connect(&DialogColorSelection::ColorDlgKillProc, this);
}

S::DialogColorSelection::~DialogColorSelection()
{
	dlgwnd->UnregisterObject(titlebar);
	dlgwnd->UnregisterObject(divbar);
	dlgwnd->UnregisterObject(okbtn);
	dlgwnd->UnregisterObject(cancelbtn);
	dlgwnd->UnregisterObject(hueslider);
	dlgwnd->UnregisterObject(satslider);
	dlgwnd->UnregisterObject(valslider);
	dlgwnd->UnregisterObject(redslider);
	dlgwnd->UnregisterObject(greenslider);
	dlgwnd->UnregisterObject(blueslider);
	dlgwnd->UnregisterObject(huetext);
	dlgwnd->UnregisterObject(sattext);
	dlgwnd->UnregisterObject(valtext);
	dlgwnd->UnregisterObject(redtext);
	dlgwnd->UnregisterObject(greentext);
	dlgwnd->UnregisterObject(bluetext);
	dlgwnd->UnregisterObject(hueedit);
	dlgwnd->UnregisterObject(satedit);
	dlgwnd->UnregisterObject(valedit);
	dlgwnd->UnregisterObject(rededit);
	dlgwnd->UnregisterObject(greenedit);
	dlgwnd->UnregisterObject(blueedit);
	dlgwnd->UnregisterObject(hextext);
	dlgwnd->UnregisterObject(hexedit);
	UnregisterObject(dlgwnd);

	delete titlebar;
	delete divbar;
	delete dlgwnd;
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

S::Int S::DialogColorSelection::ShowDialog()
{
	if (parentWindow != NIL)	dlgwnd->SetMetrics(Point(parentWindow->GetObjectProperties()->pos.x + 25, parentWindow->GetObjectProperties()->pos.y + 25), Size(436, 286));
	if (caption != NIL)		dlgwnd->SetText(caption);

	dlgwnd->Show();

	ColorDlgUpdatePickers();

	color = dlgwnd->Stay();

	return color;
}

S::Int S::DialogColorSelection::GetColor()
{
	return color;
}

S::Int S::DialogColorSelection::SetColor(Int newColor)
{
	color = newColor;

	acthue = GetRed(ConvertColor(RGB, HSV, color));
	actsat = GetGreen(ConvertColor(RGB, HSV, color));
	actval = GetBlue(ConvertColor(RGB, HSV, color));
	actred = GetRed(color);
	actgreen = GetGreen(color);
	actblue = GetBlue(color);

	ColorDlgUpdateHexValue();

	hueedit->SetText(String::IntToString(acthue));
	satedit->SetText(String::IntToString(actsat));
	valedit->SetText(String::IntToString(actval));
	rededit->SetText(String::IntToString(actred));
	greenedit->SetText(String::IntToString(actgreen));
	blueedit->SetText(String::IntToString(actblue));
	hexedit->SetText(hexval);

	return Success;
}

S::Bool S::DialogColorSelection::ColorDlgKillProc()
{
	if (dlgwnd->value == 0) dlgwnd->value = color;

	return True;
}

void S::DialogColorSelection::ColorDlgPaintProc()
{
	if (dlgwnd->hwnd == NIL) return;

	Surface		*surface = dlgwnd->GetDrawSurface();
	HDC		 dc = GetContext(dlgwnd);
	Rect		 rect;
	Point		 p1;
	Point		 p2;
	picture		*pic = new picture(256, 256, 24);
	Rect		 urect = dlgwnd->GetUpdateRect();
	Rect		 irect;
	int		 hssize = Math::Round(205 * Setup::FontSize);
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
		surface->Frame(rect, FRAME_DOWN);

		for (int ypos = 0; ypos < hssize; ypos++)
		{
			p1.x = huexoffset + 1;
			p1.y = yoffset + 1 + ypos;
			p2.x = huexoffset + 17;
			p2.y = yoffset + 1 + ypos;

			surface->Line(p1, p2, ConvertColor(HSV, RGB, RGB(255 - Math::Round(hue), 255, 255)));

			hue += huebias;
		}
	}

	rect.left	= 7;
	rect.right	= rect.left + hssize + 1;

	surface->Frame(rect, FRAME_DOWN);

	rect.left = ncxoffset;
	rect.top = yoffset;
	rect.right = rect.left + crsizex;
	rect.bottom = rect.top + crsizey;

	surface->Frame(rect, FRAME_DOWN);

	rect.left++;
	rect.top++;

	surface->Box(rect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);

	rect.left = ocxoffset;
	rect.right = rect.left + crsizex;
	rect.top--;

	surface->Frame(rect, FRAME_DOWN);

	rect.left++;
	rect.top++;

	surface->Box(rect, color, FILLED);

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
			normrgb = colortable[acthue][255 - Math::Round(sat * (256 / (205 * Setup::FontSize)))];

			rbias = (double) GetRed(normrgb) / (255 / (256 / (205 * Setup::FontSize)));
			gbias = (double) GetGreen(normrgb) / (255 / (256 / (205 * Setup::FontSize)));
			bbias = (double) GetBlue(normrgb) / (255 / (256 / (205 * Setup::FontSize)));

			ared = -rbias + max(0, xmin) * rbias;
			agreen = -gbias + max(0, xmin) * gbias;
			ablue = -bbias + max(0, xmin) * bbias;

			for (register int val = max(0, xmin); val < min(hssize, xmax); val++)
			{
				pic->SetPixel(val, sat, RGB(Math::Round(ared += rbias), Math::Round(agreen += gbias), Math::Round(ablue += bbias)));
			}
		}

		surface->BlitFromBitmap(pic->GetBitmap(), rect, irect);
	}

	forcevsupdate = true;
	preventhupdate = true;

	ColorDlgUpdatePickers();

	FreeContext(dlgwnd, dc);

	delete pic;
}

void S::DialogColorSelection::ColorDlgMessageProc(Int message, Int wparam, Int lparam)
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Rect	 huerect;
	Rect	 vsrect;
	Rect	 ncrect;
	Rect	 ocrect;
	int	 newval;
	int	 newsat;

	int	 newhue;

#ifdef __WIN32__
	int	 leftbutton;
#endif

	int	 hssize = Math::Round(205 * Setup::FontSize);

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
			if (dlgwnd->IsMouseOn(huerect))
			{
				huecapt = true;

				ColorDlgMessageProc(SM_MOUSEMOVE, 0, 0);
			}
			else if (dlgwnd->IsMouseOn(vsrect))
			{
				vscapt = true;

				ColorDlgMessageProc(SM_MOUSEMOVE, 0, 0);
			}
			else if (dlgwnd->IsMouseOn(ocrect))
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

				hueedit->SetText(String::IntToString(acthue));
				satedit->SetText(String::IntToString(actsat));
				valedit->SetText(String::IntToString(actval));
				rededit->SetText(String::IntToString(actred));
				greenedit->SetText(String::IntToString(actgreen));
				blueedit->SetText(String::IntToString(actblue));
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

				newhue = 255 - Math::Round(max(min(dlgwnd->MouseY() - (yoffset + 1), hssize - 1), 0) * (255 / (204 * Setup::FontSize)));

				if (newhue != acthue)
				{
					lasthue = acthue;
					acthue = newhue;

					int	 hssize = Math::Round(205 * Setup::FontSize);

					dlgwnd->SetUpdateRect(Rect(Point(8, yoffset + 1), Size(hssize, hssize)));
					ColorDlgPaintProc();

					actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

					ColorDlgUpdateHexValue();

					hueedit->SetText(String::IntToString(acthue));
					rededit->SetText(String::IntToString(actred));
					greenedit->SetText(String::IntToString(actgreen));
					blueedit->SetText(String::IntToString(actblue));
					hexedit->SetText(hexval);

					surface->Box(ncrect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);
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

				newval = Math::Round(max(min(dlgwnd->MouseX() - 8, hssize - 1), 0) * (255 / (204 * Setup::FontSize)));
				newsat = 255 - Math::Round(max(min(dlgwnd->MouseY() - (yoffset + 1), hssize - 1), 0) * (255 / (204 * Setup::FontSize)));

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

					valedit->SetText(String::IntToString(actval));
					satedit->SetText(String::IntToString(actsat));
					rededit->SetText(String::IntToString(actred));
					greenedit->SetText(String::IntToString(actgreen));
					blueedit->SetText(String::IntToString(actblue));
					hexedit->SetText(hexval);

					surface->Box(ncrect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);
				}
			}
			break;
	}
}

void S::DialogColorSelection::ColorDlgUpdatePickers()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Point	 p1;
	Point	 p2;
	int	 ahrgb = ConvertColor(HSV, RGB, RGB(acthue, 255, 255));
	int	 rgb;
	int	 hssize = Math::Round(205 * Setup::FontSize);

	if (((lasthue != acthue) || forcehupdate) && !preventhupdate)
	{
		// update hue picker
		if (lasthue != -1)
		{
			p1.x = huexoffset + 1;
			p1.y = yoffset + 1 + (int) ((255 - lasthue) / (256 / (205 * Setup::FontSize)));
			p2.x = huexoffset + 17;
			p2.y = yoffset + 1 + (int) ((255 - lasthue) / (256 / (205 * Setup::FontSize)));

			surface->Line(p1, p2, ConvertColor(HSV, RGB, RGB(lasthue, 255, 255)));
		}

		for (int x = huexoffset + 1; x < (huexoffset + 17); x++)
		{
			surface->SetPixel(x, yoffset + 1 + (int) ((255 - acthue) / (256 / (205 * Setup::FontSize))), RGB(255-GetRed(ahrgb), 255-GetGreen(ahrgb), 255-GetBlue(ahrgb)));
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
				rgb = ConvertColor(HSV, RGB, RGB(acthue, lastsat, (int) (x * (256 / (205 * Setup::FontSize)))));

				surface->SetPixel(x + 8, yoffset + 1 + (int) ((255 - lastsat) / (256 / (205 * Setup::FontSize))), rgb);
			}

			for (int y = 0; y < hssize; y++)
			{
				rgb = ConvertColor(HSV, RGB, RGB(acthue, (int) (255 - (y * (256 / (205 * Setup::FontSize)))), lastval));

				surface->SetPixel(8 + (int) (lastval / (256 / (205 * Setup::FontSize))), y + yoffset + 1, rgb);
			}
		}

		for (int x = 0; x < hssize; x++)
		{
			rgb = ConvertColor(HSV, RGB, RGB(acthue, actsat, x));

			surface->SetPixel(x + 8, yoffset + 1 + (int) ((255 - actsat) / (256 / (205 * Setup::FontSize))), RGB(255-GetRed(rgb), 255-GetGreen(rgb), 255-GetBlue(rgb)));
		}

		for (int y = 0; y < hssize; y++)
		{
			rgb = ConvertColor(HSV, RGB, RGB(acthue, 255 - y, actval));

			surface->SetPixel(8 + (int) (actval / (256 / (205 * Setup::FontSize))), y + yoffset + 1, RGB(255-GetRed(rgb), 255-GetGreen(rgb), 255-GetBlue(rgb)));
		}

		lastval = actval;
		lastsat = actsat;
	}

	forcehupdate = false;
	preventhupdate = false;
	forcevsupdate = false;
	preventvsupdate = false;
}

void S::DialogColorSelection::ColorDlgOK()
{
	dlgwnd->value = ConvertColor(HSV, RGB, RGB(acthue, actsat, actval));
	dlgwnd->Close();
}

void S::DialogColorSelection::ColorDlgCancel()
{
	dlgwnd->value = color;
	dlgwnd->Close();
}

void S::DialogColorSelection::ColorDlgHueSlider()
{
	int	 hssize = Math::Round(205 * Setup::FontSize);

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		hueedit->SetText(String::IntToString(acthue));
		rededit->SetText(String::IntToString(actred));
		greenedit->SetText(String::IntToString(actgreen));
		blueedit->SetText(String::IntToString(actblue));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	dlgwnd->SetUpdateRect(Rect(Point(8, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::DialogColorSelection::ColorDlgSatSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();

	ColorDlgUpdatePickers();

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		satedit->SetText(String::IntToString(actsat));
		rededit->SetText(String::IntToString(actred));
		greenedit->SetText(String::IntToString(actgreen));
		blueedit->SetText(String::IntToString(actblue));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	surface->Box(Rect(Point(ncxoffset + 1, yoffset + 1), Size(crsizex - 1, crsizey - 1)), ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);
}

void S::DialogColorSelection::ColorDlgValSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();

	ColorDlgUpdatePickers();

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		valedit->SetText(String::IntToString(actval));
		rededit->SetText(String::IntToString(actred));
		greenedit->SetText(String::IntToString(actgreen));
		blueedit->SetText(String::IntToString(actblue));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	surface->Box(Rect(Point(ncxoffset + 1, yoffset + 1), Size(crsizex - 1, crsizey - 1)), ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);
}

void S::DialogColorSelection::ColorDlgRedSlider()
{
	int	 hssize = Math::Round(205 * Setup::FontSize);

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(String::IntToString(acthue));
		satedit->SetText(String::IntToString(actsat));
		valedit->SetText(String::IntToString(actval));
		rededit->SetText(String::IntToString(actred));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	dlgwnd->SetUpdateRect(Rect(Point(8, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::DialogColorSelection::ColorDlgGreenSlider()
{
	int	 hssize = Math::Round(205 * Setup::FontSize);

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(String::IntToString(acthue));
		satedit->SetText(String::IntToString(actsat));
		valedit->SetText(String::IntToString(actval));
		greenedit->SetText(String::IntToString(actgreen));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	dlgwnd->SetUpdateRect(Rect(Point(8, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::DialogColorSelection::ColorDlgBlueSlider()
{
	int	 hssize = Math::Round(205 * Setup::FontSize);

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(String::IntToString(acthue));
		satedit->SetText(String::IntToString(actsat));
		valedit->SetText(String::IntToString(actval));
		blueedit->SetText(String::IntToString(actblue));
	}

	if (updatehextext)
	{
		ColorDlgUpdateHexValue();

		hexedit->SetText(hexval);
	}

	updatehextext = true;
	updatetext = true;

	dlgwnd->SetUpdateRect(Rect(Point(8, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::DialogColorSelection::ColorDlgHueEdit()
{
	int newhue = max(0, min(255, hueedit->GetText().ToInt()));

	if (newhue != acthue)
	{
		acthue = newhue;

		updatetext = false;

		ColorDlgHueSlider();
	}
}

void S::DialogColorSelection::ColorDlgSatEdit()
{
	int newsat = max(0, min(255, satedit->GetText().ToInt()));

	if (newsat != actsat)
	{
		actsat = newsat;

		updatetext = false;

		ColorDlgSatSlider();
	}
}

void S::DialogColorSelection::ColorDlgValEdit()
{
	int newval = max(0, min(255, valedit->GetText().ToInt()));

	if (newval != actval)
	{
		actval = newval;

		updatetext = false;

		ColorDlgValSlider();
	}
}

void S::DialogColorSelection::ColorDlgRedEdit()
{
	int newred = max(0, min(255, rededit->GetText().ToInt()));

	if (newred != GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actred = newred;

		updatetext = false;

		ColorDlgRedSlider();
	}
}

void S::DialogColorSelection::ColorDlgGreenEdit()
{
	int newgreen = max(0, min(255, greenedit->GetText().ToInt()));

	if (newgreen != GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actgreen = newgreen;

		updatetext = false;

		ColorDlgGreenSlider();
	}
}

void S::DialogColorSelection::ColorDlgBlueEdit()
{
	int newblue = max(0, min(255, blueedit->GetText().ToInt()));

	if (newblue != GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actblue = newblue;

		updatetext = false;

		ColorDlgBlueSlider();
	}
}

void S::DialogColorSelection::ColorDlgUpdateHexValue()
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

void S::DialogColorSelection::ColorDlgHexValueChanged()
{
}

void S::DialogColorSelection::ColorDlgHexEdit()
{
}
