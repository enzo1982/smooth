 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/dialogs/colordlg.h>
#include <smooth/application.h>
#include <smooth/window/window.h>
#include <smooth/misc/i18n.h>
#include <smooth/color.h>
#include <smooth/button.h>
#include <smooth/definitions.h>
#include <smooth/window/divider.h>
#include <smooth/slider.h>
#include <smooth/text.h>
#include <smooth/editbox.h>
#include <smooth/misc/string.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/window/titlebar.h>

S::GUI::Dialogs::ColorSelection::ColorSelection()
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

	yoffset = 32;
	huexoffset = 219;
	ncxoffset = 242;
	ocxoffset = 338;
	crsizex = 90;
	crsizey = 22;

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

	dlgwnd		= new Window(I18n::Translator::defaultTranslator->TranslateString("Color selection"));

	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar		= new Divider(42, OR_HORZ | OR_BOTTOM);

	okbtn		= new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, bp, bs);
	okbtn->onClick.Connect(&ColorSelection::ColorDlgOK, this);
	okbtn->SetOrientation(OR_LOWERRIGHT);

	bp.x = 175;

	cancelbtn	= new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bp, bs);
	cancelbtn->onClick.Connect(&ColorSelection::ColorDlgCancel, this);
	cancelbtn->SetOrientation(OR_LOWERRIGHT);

	bp.x = 174;
	bp.y = 35;
	bs.cx = 129;
	bs.cy = 0;

	hueslider	= new Slider(bp, bs, OR_HORZ, &acthue, 0, 255);
	hueslider->onClick.Connect(&ColorSelection::ColorDlgHueSlider, this);
	hueslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	satslider	= new Slider(bp, bs, OR_HORZ, &actsat, 0, 255);
	satslider->onClick.Connect(&ColorSelection::ColorDlgSatSlider, this);
	satslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	valslider	= new Slider(bp, bs, OR_HORZ, &actval, 0, 255);
	valslider->onClick.Connect(&ColorSelection::ColorDlgValSlider, this);
	valslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	redslider	= new Slider(bp, bs, OR_HORZ, &actred, 0, 255);
	redslider->onClick.Connect(&ColorSelection::ColorDlgRedSlider, this);
	redslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	greenslider	= new Slider(bp, bs, OR_HORZ, &actgreen, 0, 255);
	greenslider->onClick.Connect(&ColorSelection::ColorDlgGreenSlider, this);
	greenslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	blueslider	= new Slider(bp, bs, OR_HORZ, &actblue, 0, 255);
	blueslider->onClick.Connect(&ColorSelection::ColorDlgBlueSlider, this);
	blueslider->SetOrientation(OR_UPPERRIGHT);

	bp.x = 189;
	bp.y = 37;

	huetext		= new Text(I18n::Translator::defaultTranslator->TranslateString("H"), bp);
	huetext->SetOrientation(OR_UPPERRIGHT);
	huetext->SetFont(Font(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, Setup::TextColor));

	bp.y += 26;

	sattext		= new Text(I18n::Translator::defaultTranslator->TranslateString("S"), bp);
	sattext->SetOrientation(OR_UPPERRIGHT);
	sattext->SetFont(Font(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, Setup::TextColor));

	bp.y += 26;

	valtext		= new Text(I18n::Translator::defaultTranslator->TranslateString("V"), bp);
	valtext->SetOrientation(OR_UPPERRIGHT);
	valtext->SetFont(Font(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, Setup::TextColor));

	bp.y += 26;

	redtext		= new Text(I18n::Translator::defaultTranslator->TranslateString("R"), bp);
	redtext->SetOrientation(OR_UPPERRIGHT);
	redtext->SetFont(Font(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, RGB(255, 0, 0)));

	bp.y += 26;

	greentext	= new Text(I18n::Translator::defaultTranslator->TranslateString("G"), bp);
	greentext->SetOrientation(OR_UPPERRIGHT);
	greentext->SetFont(Font(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, RGB(0, 160, 0)));

	bp.y += 26;

	bluetext	= new Text(I18n::Translator::defaultTranslator->TranslateString("B"), bp);
	bluetext->SetOrientation(OR_UPPERRIGHT);
	bluetext->SetFont(Font(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, RGB(0, 0, 255)));

	bp.y += 26;

	hextext		= new Text(I18n::Translator::defaultTranslator->TranslateString("HTML code"), bp);
	hextext->SetOrientation(OR_UPPERRIGHT);
	hextext->SetFont(Font(I18N_DEFAULTFONT, I18N_DEFAULTFONTSIZE, Setup::TextColor));

	bp.x = 37;
	bp.y = 34;
	bs.cx = 30;
	bs.cy = 0;

	hueedit		= new EditBox(String::FromInt(acthue), bp, bs, 3);
	hueedit->onClick.Connect(&ColorSelection::ColorDlgHueEdit, this);
	hueedit->SetFlags(EDB_NUMERIC);
	hueedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	satedit		= new EditBox(String::FromInt(actsat), bp, bs, 3);
	satedit->onClick.Connect(&ColorSelection::ColorDlgSatEdit, this);
	satedit->SetFlags(EDB_NUMERIC);
	satedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	valedit		= new EditBox(String::FromInt(actval), bp, bs, 3);
	valedit->onClick.Connect(&ColorSelection::ColorDlgValEdit, this);
	valedit->SetFlags(EDB_NUMERIC);
	valedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	rededit		= new EditBox(String::FromInt(actred), bp, bs, 3);
	rededit->onClick.Connect(&ColorSelection::ColorDlgRedEdit, this);
	rededit->SetFlags(EDB_NUMERIC);
	rededit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	greenedit	= new EditBox(String::FromInt(actgreen), bp, bs, 3);
	greenedit->onClick.Connect(&ColorSelection::ColorDlgGreenEdit, this);
	greenedit->SetFlags(EDB_NUMERIC);
	greenedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	blueedit	= new EditBox(String::FromInt(actblue), bp, bs, 3);
	blueedit->onClick.Connect(&ColorSelection::ColorDlgBlueEdit, this);
	blueedit->SetFlags(EDB_NUMERIC);
	blueedit->SetOrientation(OR_UPPERRIGHT);

	bp.x += 40;
	bp.y += 26;
	bs.cx += 40;

	hexedit		= new EditBox(hexval, bp, bs, 7);
	hexedit->onClick.Connect(&ColorSelection::ColorDlgHexEdit, this);
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

	dlgwnd->onPaint.Connect(&ColorSelection::ColorDlgPaintProc, this);
	dlgwnd->onEvent.Connect(&ColorSelection::ColorDlgMessageProc, this);
	dlgwnd->doQuit.Connect(&ColorSelection::ColorDlgKillProc, this);
}

S::GUI::Dialogs::ColorSelection::~ColorSelection()
{
	DeleteObject(titlebar);
	DeleteObject(divbar);
	DeleteObject(dlgwnd);
	DeleteObject(okbtn);
	DeleteObject(cancelbtn);
	DeleteObject(hueslider);
	DeleteObject(satslider);
	DeleteObject(valslider);
	DeleteObject(redslider);
	DeleteObject(greenslider);
	DeleteObject(blueslider);
	DeleteObject(huetext);
	DeleteObject(sattext);
	DeleteObject(valtext);
	DeleteObject(redtext);
	DeleteObject(greentext);
	DeleteObject(bluetext);
	DeleteObject(hueedit);
	DeleteObject(satedit);
	DeleteObject(valedit);
	DeleteObject(rededit);
	DeleteObject(greenedit);
	DeleteObject(blueedit);
	DeleteObject(hextext);
	DeleteObject(hexedit);
}

S::Int S::GUI::Dialogs::ColorSelection::ShowDialog()
{
	if (parentWindow != NIL)	dlgwnd->SetMetrics(Point(parentWindow->GetObjectProperties()->pos.x + 25, parentWindow->GetObjectProperties()->pos.y + 25), Size(436, 286));
	if (caption != NIL)		dlgwnd->SetText(caption);

	dlgwnd->Show();

	ColorDlgUpdatePickers();

	color = dlgwnd->Stay();

	return color;
}

S::Int S::GUI::Dialogs::ColorSelection::GetColor()
{
	return color;
}

S::Int S::GUI::Dialogs::ColorSelection::SetColor(Int newColor)
{
	color = newColor;

	acthue = GetRed(ConvertColor(RGB, HSV, color));
	actsat = GetGreen(ConvertColor(RGB, HSV, color));
	actval = GetBlue(ConvertColor(RGB, HSV, color));
	actred = GetRed(color);
	actgreen = GetGreen(color);
	actblue = GetBlue(color);

	ColorDlgUpdateHexValue();

	hueedit->SetText(String::FromInt(acthue));
	satedit->SetText(String::FromInt(actsat));
	valedit->SetText(String::FromInt(actval));
	rededit->SetText(String::FromInt(actred));
	greenedit->SetText(String::FromInt(actgreen));
	blueedit->SetText(String::FromInt(actblue));
	hexedit->SetText(hexval);

	return Success;
}

S::Bool S::GUI::Dialogs::ColorSelection::ColorDlgKillProc()
{
	if (dlgwnd->value == 0) dlgwnd->value = color;

	return True;
}

void S::GUI::Dialogs::ColorSelection::ColorDlgPaintProc()
{
	Surface		*surface = dlgwnd->GetDrawSurface();
	Rect		 rect;
	Point		 p1;
	Point		 p2;
	Bitmap		 bmp(256, 256, 24);
	Rect		 urect = dlgwnd->GetUpdateRect();
	Rect		 irect;
	int		 hssize = 205;
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

	if (Rect::DoRectsOverlap(rect, urect))
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

	surface->StartPaint(Rect(Point(3, yoffset - 4), Size(hssize + 35, hssize + 5)));

	forcehupdate = true;
	preventvsupdate = true;

	ColorDlgUpdatePickers();

	if (Rect::DoRectsOverlap(rect, urect))
	{
		irect = Rect::OverlapRect(rect, urect);

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
			normrgb = colortable[acthue][255 - Math::Round(sat * (256.0 / 205.0))];

			rbias = (double) GetRed(normrgb) / (255.0 / (256.0 / 205.0));
			gbias = (double) GetGreen(normrgb) / (255.0 / (256.0 / 205.0));
			bbias = (double) GetBlue(normrgb) / (255.0 / (256.0 / 205.0));

			ared = -rbias + max(0, xmin) * rbias;
			agreen = -gbias + max(0, xmin) * gbias;
			ablue = -bbias + max(0, xmin) * bbias;

			for (register int val = max(0, xmin); val < min(hssize, xmax); val++)
			{
				bmp.SetPixel(val, sat, CombineColor(Math::Round(ared += rbias), Math::Round(agreen += gbias), Math::Round(ablue += bbias)));
			}
		}

		surface->BlitFromBitmap(bmp, rect, irect);
	}

	forcevsupdate = true;
	preventhupdate = true;

	ColorDlgUpdatePickers();

	surface->EndPaint();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgMessageProc(Int message, Int wparam, Int lparam)
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Rect	 huerect;
	Rect	 vsrect;
	Rect	 ncrect;
	Rect	 ocrect;
	int	 newval;
	int	 newsat;
	int	 newhue;
	int	 leftbutton;
	int	 hssize = 205;

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

				hueedit->SetText(String::FromInt(acthue));
				satedit->SetText(String::FromInt(actsat));
				valedit->SetText(String::FromInt(actval));
				rededit->SetText(String::FromInt(actred));
				greenedit->SetText(String::FromInt(actgreen));
				blueedit->SetText(String::FromInt(actblue));
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
				if (GetSystemMetrics(SM_SWAPBUTTON))	leftbutton = VK_RBUTTON;
				else					leftbutton = VK_LBUTTON;

				if (GetAsyncKeyState(leftbutton) == 0)
				{
					ColorDlgMessageProc(SM_LBUTTONUP, 0, 0);
					break;
				}

				newhue = 255 - Math::Round(max(min(dlgwnd->MouseY() - (yoffset + 1), hssize - 1), 0) * (256.0 / 205.0));

				if (newhue != acthue)
				{
					lasthue = acthue;
					acthue = newhue;

					int	 hssize = 205;

					dlgwnd->SetUpdateRect(Rect(Point(8, yoffset + 1), Size(hssize, hssize)));
					ColorDlgPaintProc();

					actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

					ColorDlgUpdateHexValue();

					hueedit->SetText(String::FromInt(acthue));
					rededit->SetText(String::FromInt(actred));
					greenedit->SetText(String::FromInt(actgreen));
					blueedit->SetText(String::FromInt(actblue));
					hexedit->SetText(hexval);

					surface->Box(ncrect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);
				}
			}
			else if (vscapt)
			{
				if (GetSystemMetrics(SM_SWAPBUTTON))	leftbutton = VK_RBUTTON;
				else					leftbutton = VK_LBUTTON;

				if (GetAsyncKeyState(leftbutton) == 0)
				{
					ColorDlgMessageProc(SM_LBUTTONUP, 0, 0);
					break;
				}

				newval = Math::Round(max(min(dlgwnd->MouseX() - 8, hssize - 1), 0) * (256.0 / 205.0));
				newsat = 255 - Math::Round(max(min(dlgwnd->MouseY() - (yoffset + 1), hssize - 1), 0) * (256.0 / 205.0));

				if ((newval != actval) || (newsat != actsat))
				{
					lastval = actval;
					lastsat = actsat;
					actval = newval;
					actsat = newsat;

					surface->StartPaint(Rect(Point(3, yoffset - 4), Size(hssize + 5, hssize + 5)));

					ColorDlgUpdatePickers();

					surface->EndPaint();

					actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
					actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

					ColorDlgUpdateHexValue();

					valedit->SetText(String::FromInt(actval));
					satedit->SetText(String::FromInt(actsat));
					rededit->SetText(String::FromInt(actred));
					greenedit->SetText(String::FromInt(actgreen));
					blueedit->SetText(String::FromInt(actblue));
					hexedit->SetText(hexval);

					surface->Box(ncrect, ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)), FILLED);
				}
			}
			break;
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgUpdatePickers()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Point	 p1;
	Point	 p2;
	int	 ahrgb = ConvertColor(HSV, RGB, RGB(acthue, 255, 255));
	int	 rgb;
	int	 hssize = 205;

	if (((lasthue != acthue) || forcehupdate) && !preventhupdate)
	{
		// update hue picker
		if (lasthue != -1)
		{
			p1.x = huexoffset + 1;
			p1.y = yoffset + 1 + (int) ((255.0 - lasthue) / (256.0 / 205.0));
			p2.x = huexoffset + 17;
			p2.y = yoffset + 1 + (int) ((255.0 - lasthue) / (256.0 / 205.0));

			surface->Line(p1, p2, ConvertColor(HSV, RGB, RGB(lasthue, 255, 255)));
		}

		for (int x = huexoffset + 1; x < (huexoffset + 17); x++)
		{
			surface->SetPixel(x, yoffset + 1 + (int) ((255.0 - acthue) / (256.0 / 205.0)), RGB(255-GetRed(ahrgb), 255-GetGreen(ahrgb), 255-GetBlue(ahrgb)));
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
				rgb = ConvertColor(HSV, RGB, RGB(acthue, lastsat, (int) (x * (256.0 / 205.0))));

				surface->SetPixel(x + 8, yoffset + 1 + (int) ((255.0 - lastsat) / (256.0 / 205.0)), rgb);
			}

			for (int y = 0; y < hssize; y++)
			{
				rgb = ConvertColor(HSV, RGB, RGB(acthue, (int) (255.0 - (y * (256.0 / 205.0))), lastval));

				surface->SetPixel(8 + (int) (lastval / (256.0 / 205.0)), y + yoffset + 1, rgb);
			}
		}

		for (int x = 0; x < hssize; x++)
		{
			rgb = ConvertColor(HSV, RGB, RGB(acthue, actsat, x));

			surface->SetPixel(x + 8, yoffset + 1 + (int) ((255.0 - actsat) / (256.0 / 205.0)), RGB(255-GetRed(rgb), 255-GetGreen(rgb), 255-GetBlue(rgb)));
		}

		for (int y = 0; y < hssize; y++)
		{
			rgb = ConvertColor(HSV, RGB, RGB(acthue, 255.0 - y, actval));

			surface->SetPixel(8 + (int) (actval / (256.0 / 205.0)), y + yoffset + 1, RGB(255-GetRed(rgb), 255-GetGreen(rgb), 255-GetBlue(rgb)));
		}

		lastval = actval;
		lastsat = actsat;
	}

	forcehupdate = false;
	preventhupdate = false;
	forcevsupdate = false;
	preventvsupdate = false;
}

void S::GUI::Dialogs::ColorSelection::ColorDlgOK()
{
	dlgwnd->value = ConvertColor(HSV, RGB, RGB(acthue, actsat, actval));
	dlgwnd->Close();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgCancel()
{
	dlgwnd->value = color;
	dlgwnd->Close();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgHueSlider()
{
	int	 hssize = 205;

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		hueedit->SetText(String::FromInt(acthue));
		rededit->SetText(String::FromInt(actred));
		greenedit->SetText(String::FromInt(actgreen));
		blueedit->SetText(String::FromInt(actblue));
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

void S::GUI::Dialogs::ColorSelection::ColorDlgSatSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();

	ColorDlgUpdatePickers();

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		satedit->SetText(String::FromInt(actsat));
		rededit->SetText(String::FromInt(actred));
		greenedit->SetText(String::FromInt(actgreen));
		blueedit->SetText(String::FromInt(actblue));
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

void S::GUI::Dialogs::ColorSelection::ColorDlgValSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();

	ColorDlgUpdatePickers();

	actred = GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actgreen = GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));
	actblue = GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval)));

	if (updatetext)
	{
		valedit->SetText(String::FromInt(actval));
		rededit->SetText(String::FromInt(actred));
		greenedit->SetText(String::FromInt(actgreen));
		blueedit->SetText(String::FromInt(actblue));
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

void S::GUI::Dialogs::ColorSelection::ColorDlgRedSlider()
{
	int	 hssize = 205;

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(String::FromInt(acthue));
		satedit->SetText(String::FromInt(actsat));
		valedit->SetText(String::FromInt(actval));
		rededit->SetText(String::FromInt(actred));
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

void S::GUI::Dialogs::ColorSelection::ColorDlgGreenSlider()
{
	int	 hssize = 205;

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(String::FromInt(acthue));
		satedit->SetText(String::FromInt(actsat));
		valedit->SetText(String::FromInt(actval));
		greenedit->SetText(String::FromInt(actgreen));
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

void S::GUI::Dialogs::ColorSelection::ColorDlgBlueSlider()
{
	int	 hssize = 205;

	acthue = GetRed(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actsat = GetGreen(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));
	actval = GetBlue(ConvertColor(RGB, HSV, RGB(actred, actgreen, actblue)));

	if (updatetext)
	{
		hueedit->SetText(String::FromInt(acthue));
		satedit->SetText(String::FromInt(actsat));
		valedit->SetText(String::FromInt(actval));
		blueedit->SetText(String::FromInt(actblue));
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

void S::GUI::Dialogs::ColorSelection::ColorDlgHueEdit()
{
	int newhue = max(0, min(255, hueedit->GetText().ToInt()));

	if (newhue != acthue)
	{
		acthue = newhue;

		updatetext = false;

		ColorDlgHueSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgSatEdit()
{
	int newsat = max(0, min(255, satedit->GetText().ToInt()));

	if (newsat != actsat)
	{
		actsat = newsat;

		updatetext = false;

		ColorDlgSatSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgValEdit()
{
	int newval = max(0, min(255, valedit->GetText().ToInt()));

	if (newval != actval)
	{
		actval = newval;

		updatetext = false;

		ColorDlgValSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgRedEdit()
{
	int newred = max(0, min(255, rededit->GetText().ToInt()));

	if (newred != GetRed(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actred = newred;

		updatetext = false;

		ColorDlgRedSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgGreenEdit()
{
	int newgreen = max(0, min(255, greenedit->GetText().ToInt()));

	if (newgreen != GetGreen(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actgreen = newgreen;

		updatetext = false;

		ColorDlgGreenSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgBlueEdit()
{
	int newblue = max(0, min(255, blueedit->GetText().ToInt()));

	if (newblue != GetBlue(ConvertColor(HSV, RGB, RGB(acthue, actsat, actval))))
	{
		actblue = newblue;

		updatetext = false;

		ColorDlgBlueSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgUpdateHexValue()
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

void S::GUI::Dialogs::ColorSelection::ColorDlgHexValueChanged()
{
}

void S::GUI::Dialogs::ColorSelection::ColorDlgHexEdit()
{
}
