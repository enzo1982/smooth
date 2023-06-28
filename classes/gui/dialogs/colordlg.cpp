 /* The smooth Class Library
  * Copyright (C) 1998-2023 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/application/application.h>
#include <smooth/gui/dialogs/colordlg.h>
#include <smooth/gui/widgets/basic/activearea.h>
#include <smooth/gui/widgets/basic/button.h>
#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/gui/widgets/basic/editbox.h>
#include <smooth/gui/widgets/basic/slider.h>
#include <smooth/gui/widgets/basic/text.h>
#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/gui/window/window.h>
#include <smooth/i18n/translator.h>
#include <smooth/misc/math.h>
#include <smooth/misc/string.h>
#include <smooth/init.h>

S::GUI::Dialogs::ColorSelection::ColorSelection()
{
	for (Int y = 0; y < 256; y++)
	{
		for (Int x = 0; x < 256; x++)
		{
			colortable[x][y] = Color(x, y, 255, Color::HSV).ConvertTo(Color::RGB);
		}
	}

	color = 0;

	forcehupdate	= False;
	forcevsupdate	= False;
	preventhupdate	= False;
	preventvsupdate	= False;
	updatetext	= True;
	updatehextext	= True;

	acthue	 = color.ConvertTo(Color::HSV).GetRed();
	actsat	 = color.ConvertTo(Color::HSV).GetGreen();
	actval	 = color.ConvertTo(Color::HSV).GetBlue();
	actred	 = color.GetRed();
	actgreen = color.GetGreen();
	actblue	 = color.GetBlue();

	dlgwnd		= new Window(I18n::Translator::defaultTranslator->TranslateString("Color selection"), Point(100, 100), Size(438, 288));

	titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar		= new Divider(39, OR_HORZ | OR_BOTTOM);

	okbtn		= new Button(I18n::Translator::defaultTranslator->TranslateString("OK"), Point(87, 29), Size());
	okbtn->onAction.Connect(&ColorSelection::OnOK, this);
	okbtn->SetOrientation(OR_LOWERRIGHT);

	cancelbtn	= new Button(I18n::Translator::defaultTranslator->TranslateString("Cancel"), Point(175, 29), Size());
	cancelbtn->onAction.Connect(&ColorSelection::OnCancel, this);
	cancelbtn->SetOrientation(OR_LOWERRIGHT);

	ncarea		= new ActiveArea(Color(), Point(238, 3), Size(91, 23));
	ocarea		= new ActiveArea(Color(), Point(334, 3), Size(91, 23));
	ocarea->onAction.Connect(&ColorSelection::OnSelectOriginalColor, this);

	hueslider	= new Slider(Point(174, 35), Size(129, 0), OR_HORZ, &acthue, 0, 255);
	hueslider->onValueChange.Connect(&ColorSelection::ColorDlgHueSlider, this);
	hueslider->SetOrientation(OR_UPPERRIGHT);

	satslider	= new Slider(Point(174, hueslider->GetY() + 26), Size(129, 0), OR_HORZ, &actsat, 0, 255);
	satslider->onValueChange.Connect(&ColorSelection::ColorDlgSatSlider, this);
	satslider->SetOrientation(OR_UPPERRIGHT);

	valslider	= new Slider(Point(174, satslider->GetY() + 26), Size(129, 0), OR_HORZ, &actval, 0, 255);
	valslider->onValueChange.Connect(&ColorSelection::ColorDlgValSlider, this);
	valslider->SetOrientation(OR_UPPERRIGHT);

	redslider	= new Slider(Point(174, valslider->GetY() + 26), Size(129, 0), OR_HORZ, &actred, 0, 255);
	redslider->onValueChange.Connect(&ColorSelection::ColorDlgRedSlider, this);
	redslider->SetOrientation(OR_UPPERRIGHT);

	greenslider	= new Slider(Point(174, redslider->GetY() + 26), Size(129, 0), OR_HORZ, &actgreen, 0, 255);
	greenslider->onValueChange.Connect(&ColorSelection::ColorDlgGreenSlider, this);
	greenslider->SetOrientation(OR_UPPERRIGHT);

	blueslider	= new Slider(Point(174, greenslider->GetY() + 26), Size(129, 0), OR_HORZ, &actblue, 0, 255);
	blueslider->onValueChange.Connect(&ColorSelection::ColorDlgBlueSlider, this);
	blueslider->SetOrientation(OR_UPPERRIGHT);

	huetext		= new Text(I18n::Translator::defaultTranslator->TranslateString("H"), Point(189, 37));
	huetext->SetOrientation(OR_UPPERRIGHT);
	huetext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Setup::TextColor));

	sattext		= new Text(I18n::Translator::defaultTranslator->TranslateString("S"), Point(189, huetext->GetY() + 26));
	sattext->SetOrientation(OR_UPPERRIGHT);
	sattext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Setup::TextColor));

	valtext		= new Text(I18n::Translator::defaultTranslator->TranslateString("V"), Point(189, sattext->GetY() + 26));
	valtext->SetOrientation(OR_UPPERRIGHT);
	valtext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Setup::TextColor));

	redtext		= new Text(I18n::Translator::defaultTranslator->TranslateString("R"), Point(189, valtext->GetY() + 26));
	redtext->SetOrientation(OR_UPPERRIGHT);
	redtext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Color(255, 0, 0)));

	greentext	= new Text(I18n::Translator::defaultTranslator->TranslateString("G"), Point(189, redtext->GetY() + 26));
	greentext->SetOrientation(OR_UPPERRIGHT);
	greentext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Color(0, 160, 0)));

	bluetext	= new Text(I18n::Translator::defaultTranslator->TranslateString("B"), Point(189, greentext->GetY() + 26));
	bluetext->SetOrientation(OR_UPPERRIGHT);
	bluetext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Color(0, 0, 255)));

	hextext		= new Text(I18n::Translator::defaultTranslator->TranslateString("HTML code"), Point(189, bluetext->GetY() + 26));
	hextext->SetOrientation(OR_UPPERRIGHT);
	hextext->SetFont(Font(Font::Default, Font::DefaultSize, Font::Normal, Font::Normal, Setup::TextColor));

	hueedit		= new EditBox(String::FromInt(acthue), Point(37, 34), Size(30, 0), 3);
	hueedit->onInput.Connect(&ColorSelection::ColorDlgHueEdit, this);
	hueedit->SetFlags(EDB_NUMERIC);
	hueedit->SetOrientation(OR_UPPERRIGHT);

	satedit		= new EditBox(String::FromInt(actsat), Point(37, hueedit->GetY() + 26), Size(30, 0), 3);
	satedit->onInput.Connect(&ColorSelection::ColorDlgSatEdit, this);
	satedit->SetFlags(EDB_NUMERIC);
	satedit->SetOrientation(OR_UPPERRIGHT);

	valedit		= new EditBox(String::FromInt(actval), Point(37, satedit->GetY() + 26), Size(30, 0), 3);
	valedit->onInput.Connect(&ColorSelection::ColorDlgValEdit, this);
	valedit->SetFlags(EDB_NUMERIC);
	valedit->SetOrientation(OR_UPPERRIGHT);

	rededit		= new EditBox(String::FromInt(actred), Point(37, valedit->GetY() + 26), Size(30, 0), 3);
	rededit->onInput.Connect(&ColorSelection::ColorDlgRedEdit, this);
	rededit->SetFlags(EDB_NUMERIC);
	rededit->SetOrientation(OR_UPPERRIGHT);

	greenedit	= new EditBox(String::FromInt(actgreen), Point(37, rededit->GetY() + 26), Size(30, 0), 3);
	greenedit->onInput.Connect(&ColorSelection::ColorDlgGreenEdit, this);
	greenedit->SetFlags(EDB_NUMERIC);
	greenedit->SetOrientation(OR_UPPERRIGHT);

	blueedit	= new EditBox(String::FromInt(actblue), Point(37, greenedit->GetY() + 26), Size(30, 0), 3);
	blueedit->onInput.Connect(&ColorSelection::ColorDlgBlueEdit, this);
	blueedit->SetFlags(EDB_NUMERIC);
	blueedit->SetOrientation(OR_UPPERRIGHT);

	ColorDlgUpdateHexValue();

	hexedit		= new EditBox(hexval, Point(77, blueedit->GetY() + 26), Size(70, 0), 7);
	hexedit->onInput.Connect(&ColorSelection::ColorDlgHexEdit, this);
	hexedit->SetOrientation(OR_UPPERRIGHT);

	huecapt = False;
	vscapt	= False;

	lasthue = -1;
	lastsat = -1;
	lastval = -1;

	Add(dlgwnd);

	dlgwnd->Add(okbtn);
	dlgwnd->Add(cancelbtn);
	dlgwnd->Add(ncarea);
	dlgwnd->Add(ocarea);
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

	dlgwnd->SetFlags(dlgwnd->GetFlags() | WF_MODAL);

	xoffset = Math::Round((dlgwnd->GetMainLayer()->GetX() - 1.0) * Surface().GetSurfaceDPI() / 96.0);
	yoffset = Math::Round((dlgwnd->GetMainLayer()->GetY() - 1.0) * Surface().GetSurfaceDPI() / 96.0);
}

S::GUI::Dialogs::ColorSelection::~ColorSelection()
{
	DeleteObject(titlebar);
	DeleteObject(divbar);
	DeleteObject(dlgwnd);
	DeleteObject(okbtn);
	DeleteObject(cancelbtn);
	DeleteObject(ncarea);
	DeleteObject(ocarea);
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
	if (parentWindow != NIL) dlgwnd->SetMetrics(parentWindow->GetPosition() + Point(25, 25), Size(436, 286));
	if (caption != NIL)	 dlgwnd->SetText(caption);

	ColorDlgUpdatePickers();

	dlgwnd->WaitUntilClosed();

	return error;
}

S::Int S::GUI::Dialogs::ColorSelection::SetColor(const Color &newColor)
{
	color = newColor;

	acthue = color.ConvertTo(Color::HSV).GetRed();
	actsat = color.ConvertTo(Color::HSV).GetGreen();
	actval = color.ConvertTo(Color::HSV).GetBlue();
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
	Surface		*surface    = dlgwnd->GetDrawSurface();
	Rect		 rect;
	Rect		 urect	    = dlgwnd->GetUpdateRect();
	Int		 hssize	    = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);
	Int		 huexoffset = hssize + Math::Round(14.0 * surface->GetSurfaceDPI() / 96.0);
	double		 huebias    = 256 / (double) hssize;
	Bitmap		 bmp(Size(hssize, hssize));

	rect.left	= huexoffset;
	rect.top	= yoffset;
	rect.right	= rect.left + Math::Round(16.0 * surface->GetSurfaceDPI() / 96.0) + 2;
	rect.bottom	= rect.top + hssize + 2;

	if (Rect::DoRectsOverlap(rect, urect))
	{
		double	 hue = 0;

		surface->Frame(rect, FRAME_DOWN);

		for (Int ypos = 0; ypos < hssize; ypos++)
		{
			Point	 p1(huexoffset							     + 1, yoffset + 1 + ypos);
			Point	 p2(huexoffset + Math::Round(16.0 * surface->GetSurfaceDPI() / 96.0) + 1, yoffset + 1 + ypos);

			surface->Line(p1, p2, Color(255 - Math::Round(hue), 255, 255, Color::HSV).ConvertTo(Color::RGB));

			hue += huebias;
		}
	}

	rect.left	= xoffset;
	rect.right	= rect.left + hssize + 2;

	surface->Frame(rect, FRAME_DOWN);

	rect.left	= xoffset - 4;
	rect.top	= yoffset - 4;
	rect.right	= rect.left + hssize;
	rect.bottom	= rect.top + hssize;

	surface->StartPaint(Rect(Point(xoffset - 4, yoffset - 4), Size(hssize + Math::Round(30.0 * surface->GetSurfaceDPI() / 96.0) + 5, hssize + 5)));

	forcehupdate	= True;
	preventvsupdate = True;

	ColorDlgUpdatePickers();

	if (Rect::DoRectsOverlap(rect, urect))
	{
		Rect	 irect = Rect::OverlapRect(rect, urect);

		irect.right += 5;
		irect.bottom += 5;

		Int	 xmin = irect.left - (xoffset + 1);
		Int	 ymin = irect.top - (yoffset + 1);
		Int	 xmax = irect.right - (xoffset + 1);
		Int	 ymax = irect.bottom - (yoffset + 1);

		rect.left   = xmin;
		rect.top    = ymin;
		rect.right  = xmax;
		rect.bottom = ymax;

		for (int sat = Math::Max(0, ymin); sat < Math::Min(hssize, ymax); sat++)
		{
			int	 normrgb = colortable[acthue][255 - Math::Round(sat * (256.0 / hssize))];

			double	 rbias	 = (double) Color(normrgb).GetRed() / (255.0 / (256.0 / hssize));
			double	 gbias	 = (double) Color(normrgb).GetGreen() / (255.0 / (256.0 / hssize));
			double	 bbias	 = (double) Color(normrgb).GetBlue() / (255.0 / (256.0 / hssize));

			double	 ared	 = -rbias + Math::Max(0, xmin) * rbias;
			double	 agreen	 = -gbias + Math::Max(0, xmin) * gbias;
			double	 ablue	 = -bbias + Math::Max(0, xmin) * bbias;

			for (int val = Math::Max(0, xmin); val < Math::Min(hssize, xmax); val++)
			{
				bmp.SetPixel(Point(val, sat), Color(Math::Round(ared += rbias), Math::Round(agreen += gbias), Math::Round(ablue += bbias)));
			}
		}

		surface->BlitFromBitmap(bmp, rect, irect);
	}

	forcevsupdate  = True;
	preventhupdate = True;

	ColorDlgUpdatePickers();

	surface->EndPaint();

	ncarea->SetColor(Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB));
}

void S::GUI::Dialogs::ColorSelection::ColorDlgMessageProc(Int message, Int wparam, Int lparam)
{
	Surface	*surface    = dlgwnd->GetDrawSurface();
	Rect	 huerect;
	Rect	 vsrect;
	Int	 hssize	    = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);
	Int	 huexoffset = hssize + Math::Round(14.0 * surface->GetSurfaceDPI() / 96.0);

	huerect.left	= huexoffset;
	huerect.top	= yoffset;
	huerect.right	= huerect.left + Math::Round(16.0 * surface->GetSurfaceDPI() / 96.0) + 1;
	huerect.bottom	= huerect.top + hssize + 1;

	vsrect.left	= xoffset + 1;
	vsrect.top	= yoffset + 1;
	vsrect.right	= vsrect.left + hssize;
	vsrect.bottom	= vsrect.top + hssize;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (dlgwnd->IsMouseOn(huerect))
			{
				huecapt = True;

				ColorDlgMessageProc(SM_MOUSEMOVE, 0, 0);
			}
			else if (dlgwnd->IsMouseOn(vsrect))
			{
				vscapt = True;

				ColorDlgMessageProc(SM_MOUSEMOVE, 0, 0);
			}

			break;
		case SM_LBUTTONUP:
			if (huecapt)
			{
				huecapt = False;

				dlgwnd->SetUpdateRect(vsrect);
				ColorDlgPaintProc();
			}
			else if (vscapt)
			{
				vscapt = False;
			}

			break;
		case SM_MOUSEMOVE:
			if (huecapt)
			{
				Int	 newhue = 255 - Math::Round(Math::Max(Math::Min(dlgwnd->GetMousePosition().y - (yoffset + 1), hssize - 1), 0) * (256.0 / hssize));

				if (newhue != acthue)
				{
					lasthue	= acthue;
					acthue	= newhue;

					dlgwnd->SetUpdateRect(Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize)));
					ColorDlgPaintProc();

					actred	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetRed();
					actgreen = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetGreen();
					actblue	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetBlue();

					ColorDlgUpdateHexValue();

					hueedit->SetText(String::FromInt(acthue));
					rededit->SetText(String::FromInt(actred));
					greenedit->SetText(String::FromInt(actgreen));
					blueedit->SetText(String::FromInt(actblue));
					hexedit->SetText(hexval);
				}
			}
			else if (vscapt)
			{
				Point	 mousePos = dlgwnd->GetMousePosition();

				Int	 newval = 	Math::Round(Math::Max(Math::Min(mousePos.x - (xoffset + 1), hssize - 1), 0) * (256.0 / hssize));
				Int	 newsat = 255 - Math::Round(Math::Max(Math::Min(mousePos.y - (yoffset + 1), hssize - 1), 0) * (256.0 / hssize));

				if ((newval != actval) || (newsat != actsat))
				{
					lastval	= actval;
					lastsat	= actsat;
					actval	= newval;
					actsat	= newsat;

					ncarea->SetColor(Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB));

					surface->StartPaint(Rect(Point(xoffset - 4, yoffset - 4), Size(hssize + 5, hssize + 5)));

					ColorDlgUpdatePickers();

					surface->EndPaint();

					actred	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetRed();
					actgreen = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetGreen();
					actblue	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetBlue();

					ColorDlgUpdateHexValue();

					valedit->SetText(String::FromInt(actval));
					satedit->SetText(String::FromInt(actsat));
					rededit->SetText(String::FromInt(actred));
					greenedit->SetText(String::FromInt(actgreen));
					blueedit->SetText(String::FromInt(actblue));
					hexedit->SetText(hexval);
				}
			}

			break;
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgUpdatePickers()
{
	Surface	*surface    = dlgwnd->GetDrawSurface();
	Int	 hssize	    = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);
	Int	 huexoffset = hssize + Math::Round(14.0 * surface->GetSurfaceDPI() / 96.0);

	if (((lasthue != acthue) || forcehupdate) && !preventhupdate)
	{
		/* Update hue picker.
		 */
		if (lasthue != -1)
		{
			Point	 p1(huexoffset							     + 1, yoffset + 1 + (Int) ((255.0 - lasthue) / (256.0 / hssize)));
			Point	 p2(huexoffset + Math::Round(16.0 * surface->GetSurfaceDPI() / 96.0) + 1, yoffset + 1 + (Int) ((255.0 - lasthue) / (256.0 / hssize)));

			surface->Line(p1, p2, Color(lasthue, 255, 255, Color::HSV).ConvertTo(Color::RGB));
		}

		Color	 ahrgb = Color(acthue, 255, 255, Color::HSV).ConvertTo(Color::RGB);

		for (Int x = huexoffset + 1; x < (huexoffset + Math::Round(16.0 * surface->GetSurfaceDPI() / 96.0) + 1); x++)
		{
			surface->SetPixel(Point(x, yoffset + 1 + (Int) ((255.0 - acthue) / (256.0 / hssize))), Color(255 - ahrgb.GetRed(), 255 - ahrgb.GetGreen(), 255 - ahrgb.GetBlue()));
		}

		lasthue = acthue;
	}

	if (((lastval != actval) || (lastsat != actsat) || forcevsupdate) && !preventvsupdate)
	{
		/* Update val/sat picker.
		 */
		surface->StartPaint(Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize)));

		if ((lastval != -1) && (lastsat != -1))
		{
			for (Int x = 0; x < hssize; x++)
			{
				Color	 rgb = Color(acthue, lastsat, (Int) (x * (256.0 / hssize)), Color::HSV).ConvertTo(Color::RGB);

				surface->SetPixel(Point(x + xoffset + 1, yoffset + 1 + (Int) ((255.0 - lastsat) / (256.0 / hssize))), rgb);
			}

			for (Int y = 0; y < hssize; y++)
			{
				Color	 rgb = Color(acthue, (Int) (255.0 - (y * (256.0 / hssize))), lastval, Color::HSV).ConvertTo(Color::RGB);

				surface->SetPixel(Point(xoffset + 1 + (Int) (lastval / (256.0 / hssize)), y + yoffset + 1), rgb);
			}
		}

		for (Int x = 0; x < hssize; x++)
		{
			Color	 rgb = Color(acthue, actsat, (Int) (x * (256.0 / hssize)), Color::HSV).ConvertTo(Color::RGB);

			surface->SetPixel(Point(x + xoffset + 1, yoffset + 1 + (Int) ((255.0 - actsat) / (256.0 / hssize))), Color(255 - Color(rgb).GetRed(), 255 - Color(rgb).GetGreen(), 255 - Color(rgb).GetBlue()));
		}

		for (Int y = 0; y < hssize; y++)
		{
			Color	 rgb = Color(acthue, (Int) (255.0 - (y * (256.0 / hssize))), actval, Color::HSV).ConvertTo(Color::RGB);

			surface->SetPixel(Point(xoffset + 1 + (Int) (actval / (256.0 / hssize)), y + yoffset + 1), Color(255 - Color(rgb).GetRed(), 255 - Color(rgb).GetGreen(), 255 - Color(rgb).GetBlue()));
		}

		surface->EndPaint();

		lastval = actval;
		lastsat = actsat;
	}

	ncarea->SetColor(Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB));

	forcehupdate	= False;
	preventhupdate	= False;
	forcevsupdate	= False;
	preventvsupdate	= False;
}

S::Void S::GUI::Dialogs::ColorSelection::OnOK()
{
	color = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB);

	dlgwnd->Close();
}

S::Void S::GUI::Dialogs::ColorSelection::OnCancel()
{
	dlgwnd->Close();
}

S::Void S::GUI::Dialogs::ColorSelection::OnSelectOriginalColor()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Int	 hssize	 = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);
	Rect	 vsrect	 = Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize));

	lasthue	 = acthue;
	lastsat	 = actsat;
	lastval	 = actval;

	acthue	 = color.ConvertTo(Color::HSV).GetRed();
	actsat	 = color.ConvertTo(Color::HSV).GetGreen();
	actval	 = color.ConvertTo(Color::HSV).GetBlue();
	actred	 = color.GetRed();
	actgreen = color.GetGreen();
	actblue	 = color.GetBlue();

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

void S::GUI::Dialogs::ColorSelection::ColorDlgHueSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Int	 hssize	 = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);

	actred	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetRed();
	actgreen = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetGreen();
	actblue	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetBlue();

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

	updatehextext = True;
	updatetext    = True;

	dlgwnd->SetUpdateRect(Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgSatSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Int	 hssize	 = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);

	ColorDlgUpdatePickers();

	actred	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetRed();
	actgreen = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetGreen();
	actblue	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetBlue();

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

	updatehextext = True;
	updatetext    = True;

	dlgwnd->SetUpdateRect(Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgValSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Int	 hssize	 = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);

	ColorDlgUpdatePickers();

	actred	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetRed();
	actgreen = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetGreen();
	actblue	 = Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetBlue();

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

	updatehextext = True;
	updatetext    = True;

	dlgwnd->SetUpdateRect(Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgRedSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Int	 hssize	 = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);

	acthue = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetRed();
	actsat = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetGreen();
	actval = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetBlue();

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

	updatehextext = True;
	updatetext    = True;

	dlgwnd->SetUpdateRect(Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgGreenSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Int	 hssize	 = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);

	acthue = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetRed();
	actsat = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetGreen();
	actval = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetBlue();

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

	updatehextext = True;
	updatetext    = True;

	dlgwnd->SetUpdateRect(Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgBlueSlider()
{
	Surface	*surface = dlgwnd->GetDrawSurface();
	Int	 hssize	 = Math::Round(205.0 * surface->GetSurfaceDPI() / 96.0);

	acthue = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetRed();
	actsat = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetGreen();
	actval = Color(actred, actgreen, actblue).ConvertTo(Color::HSV).GetBlue();

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

	updatehextext = True;
	updatetext    = True;

	dlgwnd->SetUpdateRect(Rect(Point(xoffset + 1, yoffset + 1), Size(hssize, hssize)));
	ColorDlgPaintProc();
}

void S::GUI::Dialogs::ColorSelection::ColorDlgHueEdit()
{
	Int	 newhue = Math::Max(0, Math::Min(255, (Int) hueedit->GetText().ToInt()));

	if (newhue != acthue)
	{
		acthue = newhue;

		updatetext = False;

		ColorDlgHueSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgSatEdit()
{
	Int	 newsat = Math::Max(0, Math::Min(255, (Int) satedit->GetText().ToInt()));

	if (newsat != actsat)
	{
		actsat = newsat;

		updatetext = False;

		ColorDlgSatSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgValEdit()
{
	Int	 newval = Math::Max(0, Math::Min(255, (Int) valedit->GetText().ToInt()));

	if (newval != actval)
	{
		actval = newval;

		updatetext = False;

		ColorDlgValSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgRedEdit()
{
	Int	 newred = Math::Max(0, Math::Min(255, (Int) rededit->GetText().ToInt()));

	if (newred != Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetRed())
	{
		actred = newred;

		updatetext = False;

		ColorDlgRedSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgGreenEdit()
{
	Int	 newgreen = Math::Max(0, Math::Min(255, (Int) greenedit->GetText().ToInt()));

	if (newgreen != Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetGreen())
	{
		actgreen = newgreen;

		updatetext = False;

		ColorDlgGreenSlider();
	}
}

void S::GUI::Dialogs::ColorSelection::ColorDlgBlueEdit()
{
	Int	 newblue = Math::Max(0, Math::Min(255, (Int) blueedit->GetText().ToInt()));

	if (newblue != Color(acthue, actsat, actval, Color::HSV).ConvertTo(Color::RGB).GetBlue())
	{
		actblue = newblue;

		updatetext = False;

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

	for (Int i = 1; i < 7; i++)
	{
		if (hexval[i] > 57) hexval[i] = 'a' + (hexval[i] - 58);
	}

	hueslider->SetValue(acthue);
	satslider->SetValue(actsat);
	valslider->SetValue(actval);

	redslider->SetValue(actred);
	greenslider->SetValue(actgreen);
	blueslider->SetValue(actblue);
}

void S::GUI::Dialogs::ColorSelection::ColorDlgHexValueChanged()
{
}

void S::GUI::Dialogs::ColorSelection::ColorDlgHexEdit()
{
}
