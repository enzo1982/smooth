 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/editbox.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/i18n.h>
#include <smooth/binary.h>
#include <smooth/string.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/timer.h>
#include <smooth/graphics/surface.h>

#include <imm.h>

const S::Int	 S::GUI::EditBox::classID = S::Object::RequestClassID();

S::GUI::EditBox::EditBox(String text, Point pos, Size size, Int iMaxSize)
{
	type				= classID;
	objectProperties->text		= text;
	markStart			= -1;
	markEnd				= -1;
	maxSize				= iMaxSize;
	promptPos			= 0;
	promptVisible			= False;
	timer				= NIL;
	marking				= False;
	invisibleChars			= 0;

	dropDownList			= NIL;
	comboBox			= NIL;

	objectProperties->font.SetColor(Setup::ClientTextColor);

	possibleContainers.AddEntry(Layer::classID);

	if (maxSize <= 0) maxSize = 32768;

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(19 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
}

S::GUI::EditBox::~EditBox()
{
	if (comboBox != NIL)
	{
		if (myContainer != NIL) myContainer->UnregisterObject(comboBox);

		DeleteObject(comboBox);
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::EditBox::Paint(Int message)
{
	if (!IsRegistered())	return Error;
	if (!IsVisible())	return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame;
	Rect	 textRect;
	Point	 realPos = GetRealPosition();
	String	 visText = objectProperties->text;

	switch (message)
	{
		default:
		case SP_PAINT:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx - 1;
			frame.bottom	= realPos.y + objectProperties->size.cy - 1;

			if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
			else		surface->Box(frame, Setup::BackgroundColor, FILLED);

			surface->Frame(frame, FRAME_DOWN);

			if (dropDownList != NIL && comboBox == NIL)
			{
				comboBox = new ComboBox(objectProperties->pos, objectProperties->size);
				comboBox->SetFlags(CB_HOTSPOTONLY);
				comboBox->SetOrientation(objectProperties->orientation);
				comboBox->onClick.Connect(&EditBox::DropDownListProc, this);

				if (!active) comboBox->Deactivate();

				for (Int i = 0; i < dropDownList->GetNOfEntries(); i++)
				{
					comboBox->AddEntry(dropDownList->GetNthEntry(i)->name);
				}

				myContainer->RegisterObject(comboBox);
			}
			else if (dropDownList != NIL)
			{
				comboBox->GetObjectProperties()->pos = objectProperties->pos;
				comboBox->GetObjectProperties()->size = objectProperties->size;

				if (!active) comboBox->Deactivate();
			}

			textRect.left	= frame.left + 3;
			textRect.top	= frame.top + 3;
			textRect.right	= textRect.left + objectProperties->size.cx - 6 - (dropDownList == NIL ? 0 : METRIC_COMBOBOXARROWOFFSETX + 4);
			textRect.bottom	= textRect.top + 16;

			if (invisibleChars > 0)
			{
				visText = "";

				for (Int i = 0; i < objectProperties->text.Length() - invisibleChars; i++)
				{
					visText[i] = objectProperties->text[i + invisibleChars];
				}
			}

			Font	 font = objectProperties->font;

			if (!active) font.SetColor(Setup::TextColor);

			surface->SetText(visText, textRect, font);

			if (comboBox != NIL) comboBox->Paint(SP_PAINT);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::EditBox::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())			return Error;
	if (!IsActive() || !IsVisible())	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 frame;
	Int	 nOfChars = objectProperties->text.Length();
	Point	 p1;
	Int	 newpos = 0;
	Int	 leftButton = 0;
	String	 visText = objectProperties->text;

	if (invisibleChars > 0)
	{
		visText = "";

		for (Int i = 0; i < objectProperties->text.Length() - invisibleChars; i++)
		{
			visText[i] = objectProperties->text[i + invisibleChars];
		}
	}

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1 - (dropDownList == NIL ? 0 : METRIC_COMBOBOXOFFSETX + 2);
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	switch (message)
	{
		case SM_LOOSEFOCUS:
			if (objectProperties->clicked)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				markStart = -1;
				markEnd = -1;

				MarkText(prevMarkStart, prevMarkEnd);

				objectProperties->clicked = False;
				objectProperties->checked = False;

				if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
				else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

				p1.y = frame.top + 2;

				if (promptVisible) surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

				promptVisible = False;
				promptPos = 0;

				if (timer != NIL)
				{
					timer->Stop();

					delete timer;

					timer = NIL;
				}

				Process(SM_MOUSEMOVE, 0, 0);
			}

			break;
		case SM_LBUTTONDOWN:
			if (objectProperties->clicked)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				markStart = -1;
				markEnd = -1;

				MarkText(prevMarkStart, prevMarkEnd);

				objectProperties->clicked = False;
				objectProperties->checked = False;

				if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
				else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

				p1.y = frame.top + 2;

				if (promptVisible) surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

				promptVisible = False;
				promptPos = 0;

				{
					HIMC		 hImc = ImmGetContext(wnd->hwnd);
					COMPOSITIONFORM	 info;

					info.dwStyle = CFS_POINT;
					info.ptCurrentPos.x = p1.x - 3;
					info.ptCurrentPos.y = p1.y - 2;

					ImmSetCompositionWindow(hImc, &info);

					ImmDestroyContext(hImc);
				}

				if (timer != NIL)
				{
					timer->Stop();

					delete timer;

					timer = NIL;
				}

				if (wnd->IsMouseOn(frame) && lParam != 1)
				{
					objectProperties->checked = True;
					Process(SM_LBUTTONDOWN, 0, 0);
				}
				else
				{
					Process(SM_MOUSEMOVE, 0, 0);
				}
			}
			else if (objectProperties->checked)
			{
				objectProperties->clicked = True;

				for (Int i = 0; i <= objectProperties->text.Length() - invisibleChars + 1; i++)
				{
					if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	newpos = frame.left + 3 + GetTextSizeX(visText, i, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
					else						newpos = frame.left + 3 + GetTextSizeX(String().FillN('*', i), i, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

					if (i > 0 && wnd->MouseX() < (p1.x + newpos) / 2)
					{
						promptPos = i + invisibleChars - 1;

						break;
					}
					else
					{
						p1.x = newpos;

						if (i == nOfChars) promptPos = i + invisibleChars;
					}
				}

				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				markStart = promptPos;
				oMarkStart = promptPos;
				markEnd = -1;
				marking = True;

				MarkText(prevMarkStart, prevMarkEnd);

				if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
				else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

				p1.y = frame.top + 2;

				surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

				promptVisible = True;

				{
					HIMC		 hImc = ImmGetContext(wnd->hwnd);
					COMPOSITIONFORM	 info;

					info.dwStyle = CFS_POINT;
					info.ptCurrentPos.x = p1.x - 4;
					info.ptCurrentPos.y = p1.y - 2;

					ImmSetCompositionWindow(hImc, &info);

					if (Setup::enableUnicode)
					{
						LOGFONTW	 font;

						font.lfHeight = objectProperties->font.GetSize();
						font.lfWidth = 0;
						font.lfEscapement = 0;
						font.lfOrientation = 0;
						font.lfWeight = objectProperties->font.GetWeight();
						font.lfItalic = false;
						font.lfUnderline = false;
						font.lfStrikeOut = false;
						font.lfOutPrecision = OUT_DEFAULT_PRECIS;
						font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
						font.lfCharSet = DEFAULT_CHARSET;
						font.lfQuality = DEFAULT_QUALITY;
						font.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
						wcscpy(font.lfFaceName, objectProperties->font.GetName());

						ImmSetCompositionFontW(hImc, &font);
					}
					else
					{
						LOGFONTA	 font;

						font.lfHeight = objectProperties->font.GetSize();
						font.lfWidth = 0;
						font.lfEscapement = 0;
						font.lfOrientation = 0;
						font.lfWeight = objectProperties->font.GetWeight();
						font.lfItalic = false;
						font.lfUnderline = false;
						font.lfStrikeOut = false;
						font.lfOutPrecision = OUT_DEFAULT_PRECIS;
						font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
						font.lfCharSet = DEFAULT_CHARSET;
						font.lfQuality = DEFAULT_QUALITY;
						font.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
						strcpy(font.lfFaceName, objectProperties->font.GetName());

						ImmSetCompositionFontA(hImc, &font);
					}

					ImmDestroyContext(hImc);
				}

				timer = new Timer();

				timer->onInterval.Connect(&EditBox::TimerProc, this);
				timer->Start(500);
			}

			break;
		case SM_LBUTTONUP:
			if (objectProperties->clicked && markStart != -1)
			{
				if (markStart == markEnd) { markStart = -1; markEnd = -1; }

				marking = False;
			}

			break;
		case SM_MOUSEMOVE:
			if (!objectProperties->checked && wnd->IsMouseOn(frame))
			{
				LiSASetMouseCursor(wnd->hwnd, LiSA_MOUSE_TEXTEDIT);

				objectProperties->checked = True;
			}
			else if (objectProperties->checked && !wnd->IsMouseOn(frame))
			{
				LiSASetMouseCursor(wnd->hwnd, LiSA_MOUSE_ARROW);

				objectProperties->checked = False;
			}

			if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
			else					leftButton = VK_LBUTTON;

			GetAsyncKeyState(leftButton);

			if (GetAsyncKeyState(leftButton) == 0 && marking) marking = False;

			if (objectProperties->clicked && markStart != -1 && marking)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				for (Int i = 0; i <= objectProperties->text.Length() - invisibleChars + 1; i++)
				{
					if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	newpos = frame.left + 3 + GetTextSizeX(visText, i, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
					else						newpos = frame.left + 3 + GetTextSizeX(String().FillN('*', i), i, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

					if (i > 0 && wnd->MouseX() < (p1.x + newpos) / 2)
					{
						markEnd = i + invisibleChars - 1;

						break;
					}
					else
					{
						p1.x = newpos;

						if (i == nOfChars) markEnd = i + invisibleChars;
					}
				}

				if (markEnd < oMarkStart)
				{
					markStart = markEnd;
					markEnd = oMarkStart;
					newpos = markStart;
				}
				else
				{
					markStart = oMarkStart;
					newpos = markEnd;
				}

				if (prevMarkStart != markStart || prevMarkEnd != markEnd)
				{
					SetCursor(newpos);

					MarkText(prevMarkStart, prevMarkEnd);
				}
			}

			break;
		case WM_KILLFOCUS:
		case WM_ACTIVATEAPP:
			if (objectProperties->clicked) Process(SM_LBUTTONDOWN, 0, 0);

			break;
		case WM_KEYDOWN:
			if (objectProperties->clicked)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				switch (wParam)
				{
					case VK_LEFT:
					case VK_RIGHT:
					case VK_HOME:
					case VK_END:
						markStart = -1;
						markEnd = -1;

						MarkText(prevMarkStart, prevMarkEnd);

						if (wParam == VK_LEFT && promptPos == 0)	break;
						if (wParam == VK_HOME && promptPos == 0)	break;
						if (wParam == VK_RIGHT && promptPos >= nOfChars)break;
						if (wParam == VK_END && promptPos >= nOfChars)	break;

						if (wParam == VK_LEFT)	newpos = promptPos - 1;
						if (wParam == VK_RIGHT)	newpos = promptPos + 1;
						if (wParam == VK_HOME)	newpos = 0;
						if (wParam == VK_END)	newpos = nOfChars;

						SetCursor(newpos);

						break;
					case VK_RETURN:
						if (objectProperties->clicked) Process(SM_LBUTTONDOWN, 0, 1);

						break;
					case VK_BACK:
					case VK_DELETE:
						if (markStart != markEnd) { DeleteSelectedText(); break; }

						if (promptPos == 0 && wParam == VK_BACK)		break;
						if (promptPos == nOfChars && wParam == VK_DELETE)	break;

						if (wParam == VK_BACK)
						{
							markStart = promptPos - 1;
							markEnd = promptPos;

							DeleteSelectedText();
						}
						else
						{
							markStart = promptPos;
							markEnd = promptPos + 1;

							DeleteSelectedText();
						}

						break;
					default:
						break;
				}

				retVal = Break;
			}

			break;
		case WM_CHAR:
			if (objectProperties->clicked)
			{
				if (wParam == 3 && !(lParam & (1 << 30)))
				{
					if (markStart != markEnd)
					{
						String	 mText;

						for (int j = markStart; j < markEnd; j++) mText[j - markStart] = objectProperties->text[j];

						char	*bAmText = NIL;
						wchar_t	*bWmText = NIL;

						if (Setup::enableUnicode)	bWmText = new wchar_t [mText.Length() + 1];
						else				bAmText = new char [mText.Length() + 1];

						if (Setup::enableUnicode)	wcscpy(bWmText, mText);
						else				strcpy(bAmText, mText);

						OpenClipboard(wnd->hwnd);

						if (Setup::enableUnicode)	SetClipboardData(CF_UNICODETEXT, (HANDLE) bWmText);
						else				SetClipboardData(CF_TEXT, (HANDLE) bAmText);

						CloseClipboard();
					}
				}

				if (wParam == 24 && !(lParam & (1 << 30)))
				{
					Process(WM_CHAR, 3, 0);

					DeleteSelectedText();
				}

				if (wParam == 22)
				{
					DeleteSelectedText();

					String	 insertText;

					OpenClipboard(wnd->hwnd);

					if (Setup::enableUnicode && IsClipboardFormatAvailable(CF_UNICODETEXT))
					{
						insertText = (wchar_t *) GetClipboardData(CF_UNICODETEXT);
					}
					else if (IsClipboardFormatAvailable(CF_TEXT))
					{
						insertText = (char *) GetClipboardData(CF_TEXT);
					}

					CloseClipboard();

					if (insertText.Length() > 0 && (insertText.Length() + objectProperties->text.Length()) <= maxSize)
					{
						if (Binary::IsFlagSet(flags, EDB_NUMERIC) && (insertText.ToInt() == 0 && insertText[0] != '0')) break;

						InsertText(insertText);
					}
				}

				if (nOfChars == maxSize && markStart == markEnd) break;

				if (wParam >= 32)
				{
					DeleteSelectedText();

					if (Binary::IsFlagSet(flags, EDB_NUMERIC) && (wParam < '0' || wParam > '9') && wParam != 45 && wParam != '.') break;

					String	 insertText;

					insertText[0] = wParam;

					InsertText(insertText);
				}
			}

			break;
	}

	return retVal;
}

S::Void S::GUI::EditBox::SetCursor(Int newPos)
{
	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 p1;
	String	 visText = objectProperties->text;
	Int	 oInvisChars = invisibleChars;

	if (invisibleChars > 0)
	{
		visText = "";

		for (Int i = 0; i < objectProperties->text.Length() - invisibleChars; i++)
		{
			visText[i] = objectProperties->text[i + invisibleChars];
		}
	}

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1 - (dropDownList == NIL ? 0 : METRIC_COMBOBOXOFFSETX + 2);
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
	else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

	p1.y = frame.top + 2;

	if (promptVisible) surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

	promptPos = newPos;

	if (promptPos <= invisibleChars && invisibleChars > 0) invisibleChars = promptPos;

	visText = objectProperties->text;

	if (invisibleChars > 0)
	{
		visText = "";

		for (Int i = 0; i < objectProperties->text.Length() - invisibleChars; i++)
		{
			visText[i] = objectProperties->text[i + invisibleChars];
		}
	}

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
	else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

	while (p1.x >= (frame.right - 1))
	{
		invisibleChars++;

		visText = "";

		for (Int i = 0; i < objectProperties->text.Length() - invisibleChars; i++)
		{
			visText[i] = objectProperties->text[i + invisibleChars];
		}

		if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
		else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
	}

	if (invisibleChars != oInvisChars) SetText(objectProperties->text);

	promptPos = newPos;

	surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

	{
		HIMC		 hImc = ImmGetContext(wnd->hwnd);
		COMPOSITIONFORM	 info;

		info.dwStyle = CFS_POINT;
		info.ptCurrentPos.x = p1.x - 3;
		info.ptCurrentPos.y = p1.y - 2;

		ImmSetCompositionWindow(hImc, &info);

		ImmDestroyContext(hImc);
	}

	if (timer != NIL)
	{
		timer->Stop();

		delete timer;

		timer = NIL;
	}

	timer = new Timer();

	timer->onInterval.Connect(&EditBox::TimerProc, this);
	timer->Start(500);

	promptVisible = True;
}

S::Void S::GUI::EditBox::RemoveCursor()
{
	if (!promptVisible) return;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 p = GetRealPosition();
	String	 visText = objectProperties->text;

	if (invisibleChars > 0)
	{
		visText = "";

		for (Int i = 0; i < objectProperties->text.Length() - invisibleChars; i++)
		{
			visText[i] = objectProperties->text[i + invisibleChars];
		}
	}

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p.x += 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
	else						p.x += 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

	p.y += 2;

	surface->Box(Rect(p, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

	promptVisible = False;
}

S::Void S::GUI::EditBox::MarkText(Int prevMarkStart, Int prevMarkEnd)
{
	if (prevMarkStart == markStart && prevMarkEnd == markEnd) return;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 p1;
	String	 visText = objectProperties->text;

	if (invisibleChars > 0)
	{
		visText = "";

		for (Int i = 0; i < objectProperties->text.Length() - invisibleChars; i++)
		{
			visText[i] = objectProperties->text[i + invisibleChars];
		}
	}

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1 - (dropDownList == NIL ? 0 : METRIC_COMBOBOXOFFSETX + 2);
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	Int	 bColor = GetSysColor(COLOR_HIGHLIGHT);
	Int	 tColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
	String	 mText;

	if (prevMarkStart != prevMarkEnd && prevMarkStart >= 0 && prevMarkEnd >= 0)
	{
		if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
		else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

		p1.y = frame.top + 2;

		if (promptVisible) surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

		for (int j = prevMarkStart; j < prevMarkEnd; j++) mText[j - prevMarkStart] = objectProperties->text[j];

		frame.left = realPos.x + GetTextSizeX(visText, prevMarkStart - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight()) + 4;
		frame.top = realPos.y + 2;
		frame.right = frame.left + GetTextSizeX(mText, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
		frame.bottom = realPos.y + objectProperties->size.cy - 2;

		surface->Box(frame, Setup::ClientColor, FILLED);

		frame.left--;
		frame.top++;

		surface->SetText(mText, frame, objectProperties->font);

		if (promptVisible) surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);
	}

	mText = "";

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1 - (dropDownList == NIL ? 0 : METRIC_COMBOBOXOFFSETX + 2);
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (markStart != markEnd && markStart >= 0 && markEnd >= 0)
	{
		if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
		else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

		p1.y = frame.top + 2;

		if (promptVisible) surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

		for (int j = markStart; j < markEnd; j++) mText[j - markStart] = objectProperties->text[j];

		frame.left = realPos.x + GetTextSizeX(visText, markStart - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight()) + 4;
		frame.top = realPos.y + 2;
		frame.right = frame.left + GetTextSizeX(mText, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
		frame.bottom = realPos.y + objectProperties->size.cy - 2;

		surface->Box(frame, bColor, FILLED);

		frame.left--;
		frame.top++;

		Font	 font = objectProperties->font;

		font.SetColor(tColor);

		surface->SetText(mText, frame, font);

		if (promptVisible) surface->Box(Rect(p1, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);
	}
}

S::Void S::GUI::EditBox::DeleteSelectedText()
{
	if (markStart == markEnd || markStart < 0 || markEnd < 0) return;

	Int	 bMarkStart = markStart;
	Int	 bMarkEnd = markEnd;

	markStart = -1;
	markEnd = -1;

	MarkText(bMarkStart, bMarkEnd);

	Int	 nOfChars = objectProperties->text.Length();
	String	 newText;
	Int	 prevPromptPos = promptPos;

	for (Int i = 0; i < bMarkStart; i++)		newText[i] = objectProperties->text[i];
	for (Int j = bMarkEnd; j <= nOfChars; j++)	newText[j - (bMarkEnd - bMarkStart)] = objectProperties->text[j];

	SetText(newText);

	promptPos = prevPromptPos;

	SetCursor(bMarkStart);
}

S::Void S::GUI::EditBox::InsertText(String insertText)
{
	Int	 nOfChars = objectProperties->text.Length();
	String	 newText;
	Int	 prevPromptPos = promptPos;

	for (Int i = 0; i < promptPos; i++)					newText[i] = objectProperties->text[i];
	for (Int j = promptPos; j < promptPos + insertText.Length(); j++)	newText[j] = insertText[j - promptPos];
	for (Int k = promptPos; k <= nOfChars; k++)				newText[k + insertText.Length()] = objectProperties->text[k];

	SetText(newText);

	promptPos = prevPromptPos;

	SetCursor(promptPos + insertText.Length());
}

S::Int S::GUI::EditBox::Show()
{
	if (comboBox != NIL)
	{
		comboBox->SetMetrics(objectProperties->pos, objectProperties->size);
		comboBox->Show();
	}

	return Widget::Show();
}

S::Int S::GUI::EditBox::Hide()
{
	if (comboBox != NIL) comboBox->Hide();

	return Widget::Hide();
}

S::Int S::GUI::EditBox::Activate()
{
	if (comboBox != NIL) comboBox->Activate();

	return Widget::Activate();
}

S::Int S::GUI::EditBox::Deactivate()
{
	if (objectProperties->clicked)
	{
		if (timer != NIL)
		{
			timer->Stop();

			delete timer;

			timer = NIL;
		}

		objectProperties->clicked = False;
	}

	if (comboBox != NIL) comboBox->Deactivate();

	return Widget::Deactivate();
}

S::Int S::GUI::EditBox::SetText(String txt)
{
	RemoveCursor();

	promptPos = 0;

	if (objectProperties->text == txt)
	{
		Paint(SP_PAINT);

		return Success;
	}

	objectProperties->text = txt;
	invisibleChars = 0;

	Paint(SP_PAINT);

	if (registered) onClick.Emit(0, 0);

	return Success;
}

S::Int S::GUI::EditBox::SetDropDownList(List *nDropDownList)
{
	dropDownList = nDropDownList;

	return Success;
}

S::Void S::GUI::EditBox::DropDownListProc()
{
	if (comboBox->GetSelectedEntry() == NIL) return;

	SetText(comboBox->GetSelectedEntry()->name);

	comboBox->GetSelectedEntry()->clicked = False;

	objectProperties->checked = True;

	Process(SM_LBUTTONDOWN, 0, 0);
	SetCursor(objectProperties->text.Length());

	markStart = 0;
	markEnd = objectProperties->text.Length();

	MarkText(0, 0);
	SetCursor(objectProperties->text.Length());
}

S::Void S::GUI::EditBox::TimerProc()
{
	if (!registered)		return;
	if (!active || !visible)	return;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 lineStart;
	String	 visText = objectProperties->text;

	if (invisibleChars > 0)
	{
		visText = "";

		for (Int i = 0; i < objectProperties->text.Length() - invisibleChars; i++)
		{
			visText[i] = objectProperties->text[i + invisibleChars];
		}
	}

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	lineStart.x = frame.left + 3 + GetTextSizeX(visText, promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
	else						lineStart.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

	lineStart.y = frame.top + 2;

	surface->Box(Rect(lineStart, Size(1, METRIC_EDITBOXLINEHEIGHT)), 0, INVERT);

	promptVisible = !promptVisible;
}

S::Int S::GUI::EditBox::GetCursorPos()
{
	if (objectProperties->clicked)	return promptPos;
	else				return -1;
}
