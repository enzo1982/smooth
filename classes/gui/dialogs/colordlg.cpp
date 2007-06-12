 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/colordlg.h>
#include <smooth/gui/application/application.h>
#include <smooth/gui/window/window.h>
#include <smooth/i18n/i18n.h>
#include <smooth/graphics/color.h>
#include <smooth/gui/widgets/basic/button.h>
#include <smooth/definitions.h>
#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/gui/widgets/basic/slider.h>
#include <smooth/gui/widgets/basic/text.h>
#include <smooth/gui/widgets/basic/editbox.h>
#include <smooth/misc/string.h>
#include <smooth/init.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/gui/widgets/basic/titlebar.h>

S::GUI::Dialogs::ColorSelection::ColorSelection()
{
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			colortable[x][y] = Color(x, y, 255, HSV).ConvertTo(RGBA);
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

	acthue = color.ConvertTo(HSV).GetRed();
	actsat = color.ConvertTo(HSV).GetGreen();
	actval = color.ConvertTo(HSV).GetBlue();
	actred = color.GetRed();
	actgreen = color.GetGreen();
	actblue = color.GetBlue();

	ColorDlgUpdateHexValue();

	bp.x = 87;
	bp.y = 29;
	bs.cx = 0;
	bs.cy = 0;

	dlgwnd		= new Window(I18n::Translator::defaultTranslator->TranslateString("Color selection"), Point(100, 100), Size(438, 288));

	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar		= new Divider(39, OR_HORZ | OR_BOTTOM);

	okbtn		= new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), NIL, bp, bs);
	okbtn->onAction.Connect(&ColorSelection::ColorDlgOK, this);
	okbtn->SetOrientation(OR_LOWERRIGHT);

	bp.x = 175;

	cancelbtn	= new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), NIL, bp, bs);
	cancelbtn->onAction.Connect(&ColorSelection::ColorDlgCancel, this);
	cancelbtn->SetOrientation(OR_LOWERRIGHT);

	bp.x = 174;
	bp.y = 35;
	bs.cx = 129;
	bs.cy = 0;

	hueslider	= new Slider(bp, bs, OR_HORZ, &acthue, 0, 255);
	hueslider->onValueChange.Connect(&ColorSelection::ColorDlgHueSlider, this);
	hueslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	satslider	= new Slider(bp, bs, OR_HORZ, &actsat, 0, 255);
	satslider->onValueChange.Connect(&ColorSelection::ColorDlgSatSlider, this);
	satslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	valslider	= new Slider(bp, bs, OR_HORZ, &actval, 0, 255);
	valslider->onValueChange.Connect(&ColorSelection::ColorDlgValSlider, this);
	valslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	redslider	= new Slider(bp, bs, OR_HORZ, &actred, 0, 255);
	redslider->onValueChange.Connect(&ColorSelection::ColorDlgRedSlider, this);
	redslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	greenslider	= new Slider(bp, bs, OR_HORZ, &actgreen, 0, 255);
	greenslider->onValueChange.Connect(&ColorSelection::ColorDlgGreenSlider, this);
	greenslider->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	blueslider	= new Slider(bp, bs, OR_HORZ, &actblue, 0, 255);
	blueslider->onValueChange.Connect(&ColorSelection::ColorDlgBlueSlider, this);
	blueslider->SetOrientation(OR_UPPERRIGHT);

	bp.x = 189;
	bp.y = 37;

	huetext		= new Text(I18n::Translator::defaultTranslator->TranslateString("H"), bp);
	huetext->SetOrientation(OR_UPPERRIGHT);
	huetext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Setup::TextColor));

	bp.y += 26;

	sattext		= new Text(I18n::Translator::defaultTranslator->TranslateString("S"), bp);
	sattext->SetOrientation(OR_UPPERRIGHT);
	sattext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Setup::TextColor));

	bp.y += 26;

	valtext		= new Text(I18n::Translator::defaultTranslator->TranslateString("V"), bp);
	valtext->SetOrientation(OR_UPPERRIGHT);
	valtext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Setup::TextColor));

	bp.y += 26;

	redtext		= new Text(I18n::Translator::defaultTranslator->TranslateString("R"), bp);
	redtext->SetOrientation(OR_UPPERRIGHT);
	redtext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Color(255, 0, 0)));

	bp.y += 26;

	greentext	= new Text(I18n::Translator::defaultTranslator->TranslateString("G"), bp);
	greentext->SetOrientation(OR_UPPERRIGHT);
	greentext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Color(0, 160, 0)));

	bp.y += 26;

	bluetext	= new Text(I18n::Translator::defaultTranslator->TranslateString("B"), bp);
	bluetext->SetOrientation(OR_UPPERRIGHT);
	bluetext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Color(0, 0, 255)));

	bp.y += 26;

	hextext		= new Text(I18n::Translator::defaultTranslator->TranslateString("HTML code"), bp);
	hextext->SetOrientation(OR_UPPERRIGHT);
	hextext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Setup::TextColor));

	bp.x = 37;
	bp.y = 34;
	bs.cx = 30;
	bs.cy = 0;

	hueedit		= new EditBox(String::FromInt(acthue), bp, bs, 3);
	hueedit->onInput.Connect(&ColorSelection::ColorDlgHueEdit, this);
	hueedit->SetFlags(EDB_NUMERIC);
	hueedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	satedit		= new EditBox(String::FromInt(actsat), bp, bs, 3);
	satedit->onInput.Connect(&ColorSelection::ColorDlgSatEdit, this);
	satedit->SetFlags(EDB_NUMERIC);
	satedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	valedit		= new EditBox(String::FromInt(actval), bp, bs, 3);
	valedit->onInput.Connect(&ColorSelection::ColorDlgValEdit, this);
	valedit->SetFlags(EDB_NUMERIC);
	valedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	rededit		= new EditBox(String::FromInt(actred), bp, bs, 3);
	rededit->onInput.Connect(&ColorSelection::ColorDlgRedEdit, this);
	rededit->SetFlags(EDB_NUMERIC);
	rededit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	greenedit	= new EditBox(String::FromInt(actgreen), bp, bs, 3);
	greenedit->onInput.Connect(&ColorSelection::ColorDlgGreenEdit, this);
	greenedit->SetFlags(EDB_NUMERIC);
	greenedit->SetOrientation(OR_UPPERRIGHT);

	bp.y += 26;

	blueedit	= new EditBox(String::FromInt(actblue), bp, bs, 3);
	blueedit->onInput.Connect(&ColorSelection::ColorDlgBlueEdit, this);
	blueedit->SetFlags(EDB_NUMERIC);
	blueedit->SetOrientation(OR_UPPERRIGHT);

	bp.x += 40;
	bp.y += 26;
	bs.cx += 40;

	hexedit		= new EditBox(hexval, bp, bs, 7);
	hexedit->onInput.Connect(&ColorSelection::ColorDlgHexEdit, this);
	hexedit->SetOrientation(OR_UPPERRIGHT);

	huecapt = false;
	vscapt = false;

	lasthue = -1;
	lastsat = -1;
	lastval = -1;

	Add(dlgwnd);

	dlgwnd->Add(okbtn);
	dlgwnd->Add(cancelbtn);
	dlgwnd->Add(hueslider);
	dlgwnd->Add(satslider);
	dlgwnd->Add(valslider);
	dlgwnd->Add(redslider);
	dlgwnd->Add(greenslider);
	dlgwnd->Add(blueslider);
	dlgwnd->Add(huetext);
	dlgwnd->Add(sattext);
	dlgwnd->Add(valtext);
	dlgwnd->Add(redtext);
	dlgwnd->Add(greentext);
	dlgwnd->Add(bluetext);
	dlgwnd->Add(hueedit);
	dlgwnd->Add(satedit);
	dlgwnd->Add(valedit);
	dlgwnd->Add(rededit);
	dlgwnd->Add(greenedit);
	dlgwnd->Add(blueedit);
	dlgwnd->Add(hextext);
	dlgwnd->Add(hexedit);
	dlgwnd->Add(titlebar);
	dlgwnd->Add(divbar);

	dlgwnd->onPaint.Connect(&ColorSelection::ColorDlgPaintProc, this);
	dlgwnd->onEvent.Connect(&ColorSelection::ColorDlgMessageProc, this);
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

const Error &S::GUI::Dialogs::ColorSelection::ShowDialog()
{
	if (parentWindow != NIL)	dlgwnd->SetMetrics(parentWindow->GetPosition() + Point(25, 25), Size(436, 286));
	if (caption != NIL)		dlgwnd->SetText(caption);

	dlgwnd->Show();

	ColorDlgUpdatePickers();

	dlgwnd->Stay();

	return error;
}

S::Int S::GUI::Dialogs::ColorSelection::SetColor(const Color &newColor)
{
	color = newColor;

	acthue = color.ConvertTo(HSV).GetRed();
	actsat = color.ConvertTo(HSV).GetGreen();
	actval = color.ConvertTo(HSV).GetBlue();
	actred = color.GetRed();
	actgreen = color.GetGreen();
	actblue = color.GetBlue();

	ColorDlgUpdateHexValue();

	hueedit->SetText(String::FromInt(acthue));
	satedit->SetText(String::FromInt(actsat));
	valedit->SetText(String::FromInt(actval));
	rededit->SetText(String::FromInt(actred));
	greenedit->SetText(String::FromInt(actgreen));
	blueedit->SetText(String::FromInt(actblue));
	hexedit->SetText(hexval);

	return Success();
}

const S::GUI::Color &S::GUI::Dialogs::ColorSelection::GetColor() const
{
	return color;
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
	Int		 hssize = 205;
	double		 hue = 0;
	double		 huebias = 256 / (double) hssize;
	register Int	 xmin = 0;
	register Int	 ymin = 0;
	register Int	 xmax = hssize;
	register Int	 ymax = hssize;
	register int	 normrgb;
	register double	 rbias;
	register double	 gbias;
	register double	 bbias;
	register double  ared;
	register double  agreen;
	register double  ablue;

	rect.left	= huexoffset;
	rect.top	= yoffset;
	rect.right	= rect.left + 18;
	rect.bottom	= rect.top + hssize + 2;

	if (Rect::DoRectsOverlap(rect, urect))
	{
		surface->Frame(rect, FRAME_DOWN);

		for (int ypos = 0; ypos < hssize; ypos++)
		{
			p1.x = huexoffset + 1;
			p1.y = yoffset + 1 + ypos;
			p2.x = huexoffset + 17;
			p2.y = yoffset + 1 + ypos;

			surface->Line(p1, p2, Color(255 - Math::Round(hue), 255, 255, HSV).ConvertTo(RGBA));

			hue += huebias;
		}
	}

	rect.left	= 7;
	rect.right	= rect.left + hssize + 2;

	surface->Frame(rect, FRAME_DOWN);

	rect.left = ncxoffset;
	rect.top = yoffset;
	rect.right = rect.left + crsizex + 1;
	rect.bottom = rect.top + crsizey + 1;

	surface->Box(rect, Color(acthue, actsat, actval, HSV).ConvertTo(RGBA), Rect::Filled);
	surface->Frame(rect, FRAME_DOWN);

	rect.left = ocxoffset;
	rect.right = rect.left + crsizex + 1;

	surface->Box(rect, color, Rect::Filled);
	surface->Frame(rect, FRAME_DOWN);

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

		for (register int sat = Math::Max(0, ymin); sat < Math::Min(hssize, ymax); sat++)
		{
			normrgb = colortable[acthue][255 - Math::Round(sat * (256.0 / 205.0))];

			rbias = (double) Color(normrgb).GetRed() / (255.0 / (256.0 / 205.0));
			gbias = (double) Color(normrgb).GetGreen() / (255.0 / (256.0 / 205.0));
			bbias = (double) Color(normrgb).GetBlue() / (255.0 / (256.0 / 205.0));

			ared = -rbias + Math::Max(0, xmin) * rbias;
			agreen = -gbias + Math::Max(0, xmin) * gbias;
			ablue = -bbias + Math::Max(0, xmin) * bbias;

			for (register int val = Math::Max(0, xmin); val < Math::Min(hssize, xmax); val++)
			{
				bmp.SetPixel(val, sat, Color(Math::Round(ared += rbias), Math::Round(agreen += gbias), Math::Round(ablue += bbias)));
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

				acthue = color.ConvertTo(HSV).GetRed();
				actsat = color.ConvertTo(HSV).GetGreen();
				actval = color.ConvertTo(HSV).GetBlue();
				actred = color.GetRed();
				actgreen = color.GetGreen();
				actblue = color.GetBlue();

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

				newhue = 255 - Math::Round(Math::Max(Math::Min(dlgwnd->GetMousePosition().y - (yoffset + 1), hssize - 1), 0) * (256.0 / 205.0));

				if (newhue != acthue)
				{
					lasthue = acthue;
					acthue = newhue;

					dlgwnd->SetUpdateRect(Rect(Point(8, yoffset + 1), Size(hssize, hssize)));
					ColorDlgPaintProc();

					actred = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetRed();
					actgreen = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetGreen();
					actblue = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetBlue();

					ColorDlgUpdateHexValue();

					hueedit->SetText(String::FromInt(acthue));
					rededit->SetText(String::FromInt(actred));
					greenedit->SetText(String::FromInt(actgreen));
					blueedit->SetText(String::FromInt(actblue));
					hexedit->SetText(hexval);

					surface->Box(ncrect, Color(acthue, actsat, actval, HSV).ConvertTo(RGBA), Rect::Filled);
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

				Point	 mousePos = dlgwnd->GetMousePosition();

				newval = Math::Round(Math::Max(Math::Min(mousePos.x - 8, hssize - 1), 0) * (256.0 / 205.0));
				newsat = 255 - Math::Round(Math::Max(Math::Min(mousePos.y - (yoffset + 1), hssize - 1), 0) * (256.0 / 205.0));

				if ((newval != actval) || (newsat != actsat))
				{
					lastval = actval;
					lastsat = actsat;
					actval = newval;
					actsat = newsat;

					surface->StartPaint(Rect(Point(3, yoffset - 4), Size(hssize + 5, hssize + 5)));

					ColorDlgUpdatePickers();

					surface->EndPaint();

					actred = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetRed();
					actgreen = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetGreen();
					actblue = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetBlue();

					ColorDlgUpdateHexValue();

					valedit->SetText(String::FromInt(actval));
					satedit->SetText(String::FromInt(actsat));
					rededit->SetText(String::FromInt(actred));
					greenedit->SetText(String::FromInt(actgreen));
					blueedit->SetText(String::FromInt(actblue));
					hexedit->SetText(hexval);

					surface->Box(ncrect, Color(acthue, actsat, actval, HSV).ConvertTo(RGBA), Rect::Filled);
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
	int	 ahrgb = Color(acthue, 255, 255, HSV).ConvertTo(RGBA);
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

			surface->Line(p1, p2, Color(lasthue, 255, 255, HSV).ConvertTo(RGBA));
		}

		for (int x = huexoffset + 1; x < (huexoffset + 17); x++)
		{
			surface->SetPixel(x, yoffset + 1 + (int) ((255.0 - acthue) / (256.0 / 205.0)), Color(255 - Color(ahrgb).GetRed(), 255 - Color(ahrgb).GetGreen(), 255 - Color(ahrgb).GetBlue()));
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
				rgb = Color(acthue, lastsat, (int) (x * (256.0 / 205.0)), HSV).ConvertTo(RGBA);

				surface->SetPixel(x + 8, yoffset + 1 + (int) ((255.0 - lastsat) / (256.0 / 205.0)), rgb);
			}

			for (int y = 0; y < hssize; y++)
			{
				rgb = Color(acthue, (int) (255.0 - (y * (256.0 / 205.0))), lastval, HSV).ConvertTo(RGBA);

				surface->SetPixel(8 + (int) (lastval / (256.0 / 205.0)), y + yoffset + 1, rgb);
			}
		}

		for (int x = 0; x < hssize; x++)
		{
			rgb = Color(acthue, actsat, x, HSV).ConvertTo(RGBA);

			surface->SetPixel(x + 8, yoffset + 1 + (int) ((255.0 - actsat) / (256.0 / 205.0)), Color(255 - Color(rgb).GetRed(), 255 - Color(rgb).GetGreen(), 255 - Color(rgb).GetBlue()));
		}

		for (int y = 0; y < hssize; y++)
		{
			rgb = Color(acthue, 255 - y, actval, HSV).ConvertTo(RGBA);

			surface->SetPixel(8 + (int) (actval / (256.0 / 205.0)), y + yoffset + 1, Color(255 - Color(rgb).GetRed(), 255 - Color(rgb).GetGreen(), 255 - Color(rgb).GetBlue()));
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
	color = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA);

	dlgwnd->Close();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgCancel()
{
	dlgwnd->Close();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgHueSlider()
{
	int	 hssize = 205;

	actred = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetRed();
	actgreen = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetGreen();
	actblue = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetBlue();

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

	actred = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetRed();
	actgreen = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetGreen();
	actblue = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetBlue();

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

	surface->Box(Rect(Point(ncxoffset + 1, yoffset + 1), Size(crsizex - 1, crsizey - 1)), Color(acthue, actsat, actval, HSV).ConvertTo(RGBA), Rect::Filled);
}

void S::GUI::Dialogs::ColorSelection::ColorDlgValSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();

	ColorDlgUpdatePickers();

	actred = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetRed();
	actgreen = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetGreen();
	actblue = Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetBlue();

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

	surface->Box(Rect(Point(ncxoffset + 1, yoffset + 1), Size(crsizex - 1, crsizey - 1)), Color(acthue, actsat, actval, HSV).ConvertTo(RGBA), Rect::Filled);
}

void S::GUI::Dialogs::ColorSelection::ColorDlgRedSlider()
{
	int	 hssize = 205;

	acthue = Color(actred, actgreen, actblue).ConvertTo(HSV).GetRed();
	actsat = Color(actred, actgreen, actblue).ConvertTo(HSV).GetGreen();
	actval = Color(actred, actgreen, actblue).ConvertTo(HSV).GetBlue();

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

	acthue = Color(actred, actgreen, actblue).ConvertTo(HSV).GetRed();
	actsat = Color(actred, actgreen, actblue).ConvertTo(HSV).GetGreen();
	actval = Color(actred, actgreen, actblue).ConvertTo(HSV).GetBlue();

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

	acthue = Color(actred, actgreen, actblue).ConvertTo(HSV).GetRed();
	actsat = Color(actred, actgreen, actblue).ConvertTo(HSV).GetGreen();
	actval = Color(actred, actgreen, actblue).ConvertTo(HSV).GetBlue();

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
	int newhue = Math::Max(0, Math::Min(255, hueedit->GetText().ToInt()));

	if (newhue != acthue)
	{
		acthue = newhue;

		updatetext = false;

		ColorDlgHueSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgSatEdit()
{
	int newsat = Math::Max(0, Math::Min(255, satedit->GetText().ToInt()));

	if (newsat != actsat)
	{
		actsat = newsat;

		updatetext = false;

		ColorDlgSatSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgValEdit()
{
	int newval = Math::Max(0, Math::Min(255, valedit->GetText().ToInt()));

	if (newval != actval)
	{
		actval = newval;

		updatetext = false;

		ColorDlgValSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgRedEdit()
{
	int newred = Math::Max(0, Math::Min(255, rededit->GetText().ToInt()));

	if (newred != Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetRed())
	{
		actred = newred;

		updatetext = false;

		ColorDlgRedSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgGreenEdit()
{
	int newgreen = Math::Max(0, Math::Min(255, greenedit->GetText().ToInt()));

	if (newgreen != Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetGreen())
	{
		actgreen = newgreen;

		updatetext = false;

		ColorDlgGreenSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgBlueEdit()
{
	int newblue = Math::Max(0, Math::Min(255, blueedit->GetText().ToInt()));

	if (newblue != Color(acthue, actsat, actval, HSV).ConvertTo(RGBA).GetBlue())
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
