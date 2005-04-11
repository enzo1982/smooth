 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/editbox.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/i18n.h>
#include <smooth/misc/binary.h>
#include <smooth/misc/string.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/system/timer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/basic/scrollbar.h>

#include <imm.h>

const S::Int	 S::GUI::EditBox::classID = S::Object::RequestClassID();

S::GUI::EditBox::EditBox(String iText, Point iPos, Size iSize, Int iMaxSize)
{
	type		= classID;
	text		= iText;
	markStart	= -1;
	markEnd		= -1;
	maxSize		= iMaxSize;
	promptPos	= 0;
	promptVisible	= False;
	timer		= NIL;
	marking		= False;
	invisibleChars	= 0;
	scrollbarPos	= 0;

	dropDownList	= NIL;
	comboBox	= NIL;
	scrollbar	= NIL;

	font.SetColor(Setup::ClientTextColor);

	possibleContainers.AddEntry(Layer::classID);

	if (maxSize <= 0) maxSize = 32768;

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 19;

	onEnter.SetParentObject(this);
}

S::GUI::EditBox::~EditBox()
{
	if (comboBox != NIL)
	{
		if (container != NIL) container->UnregisterObject(comboBox);

		DeleteObject(comboBox);

		comboBox = NIL;
	}

	if (scrollbar != NIL)
	{
		if (container != NIL) container->UnregisterObject(scrollbar);

		DeleteObject(scrollbar);

		scrollbar = NIL;
	}
}

S::Int S::GUI::EditBox::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface	= container->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 textRect;
	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(GetRealPosition(), size);

	switch (message)
	{
		default:
		case SP_PAINT:
			if (scrollbar != NIL) frame.right -= 17;

			if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
			else		surface->Box(frame, Setup::BackgroundColor, FILLED);

			if (scrollbar != NIL) frame.right += 17;

			surface->Frame(frame, FRAME_DOWN);

			if (dropDownList != NIL && comboBox == NIL)
			{
				comboBox = new ComboBox(pos, size);
				comboBox->SetFlags(CB_HOTSPOTONLY);
				comboBox->SetOrientation(orientation);
				comboBox->onClick.Connect(&EditBox::DropDownListProc, this);

				if (!active) comboBox->Deactivate();

				for (Int i = 0; i < dropDownList->GetNOfObjects(); i++)
				{
					comboBox->AddEntry(dropDownList->GetNthObject(i)->GetText());
				}

				container->RegisterObject(comboBox);
			}
			else if (dropDownList != NIL)
			{
				comboBox->pos = pos;
				comboBox->size = size;

				if (!active) comboBox->Deactivate();
			}

			textRect.left	= frame.left + 3;
			textRect.top	= frame.top + 3;
			textRect.right	= textRect.left + size.cx - 6 - (dropDownList != NIL || scrollbar != NIL ? 16 : 0);
			textRect.bottom	= textRect.top + (flags | EDB_MULTILINE ? size.cy - 5 : font.GetLineSizeY(text));

			if ((flags | EDB_MULTILINE) && (font.GetTextSizeY(text) > size.cy - 6) && (scrollbar == NIL))
			{
				Layer	*layer = (Layer *) container;

				scrollbarPos = 0;

				scrollbar = new Scrollbar(Point(frame.right - layer->pos.x - 18, frame.top - layer->pos.y + 1), Size(0, size.cy - 2), OR_VERT, &scrollbarPos, 0, GetNOfInvisibleLines());
				scrollbar->onClick.Connect(&EditBox::ScrollbarProc, this);

				container->RegisterObject(scrollbar);

				scrollbar->Paint(SP_PAINT);
			}
			else if ((flags | EDB_MULTILINE) && (font.GetTextSizeY(text) > size.cy - 6) && (scrollbar != NIL))
			{
				Layer	*layer = (Layer *) container;

				scrollbar->pos = Point(frame.right - layer->pos.x - 18, frame.top - layer->pos.y + 1);
				scrollbar->size.cy = size.cy - 2;

				scrollbar->SetRange(0, GetNOfInvisibleLines());

				scrollbar->Paint(SP_PAINT);
			}
			else if ((flags | EDB_MULTILINE) && (font.GetTextSizeY(text) <= size.cy - 6) && (scrollbar != NIL))
			{
				container->UnregisterObject(scrollbar);

				surface->Box(Rect(Point(frame.right - 18, frame.top + 1), Size(18, size.cy - 2)), Setup::ClientColor, FILLED);

				DeleteObject(scrollbar);

				scrollbar = NIL;
				scrollbarPos = 0;
			}

			Font	 nFont = font;
			String	 visText = GetVisibleText();

			if (!active) nFont.SetColor(Setup::TextColor);

			surface->SetText(visText, textRect, nFont);

			if (comboBox != NIL) comboBox->Paint(SP_PAINT);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::EditBox::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())			return Failure;
	if (!IsActive() || !IsVisible())	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 frame;
	Int	 nOfChars = text.Length();
	Point	 p1;
	Int	 newpos = 0;
	Int	 leftButton = 0;
	String	 visText = GetVisibleText();

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx - (dropDownList == NIL ? 0 : 18);
	frame.bottom	= realPos.y + size.cy;

	if (scrollbar != NIL) frame.right -= 18;

	switch (message)
	{
		case SM_LOOSEFOCUS:
			if (clicked)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				markStart = -1;
				markEnd = -1;

				MarkText(prevMarkStart, prevMarkEnd);

				clicked = False;
				checked = False;

				if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
				else						p1.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

				p1.y = frame.top + 2;

				if (promptVisible) surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);

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
			if (clicked)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				markStart = -1;
				markEnd = -1;

				MarkText(prevMarkStart, prevMarkEnd);

				clicked = False;
				checked = False;

				if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
				else						p1.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

				p1.y = frame.top + 2;

				if (promptVisible) surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);

				promptVisible = False;
				promptPos = 0;

				{
					HIMC		 hImc = ImmGetContext((HWND) wnd->GetSystemWindow());
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
					checked = True;
					Process(SM_LBUTTONDOWN, 0, 0);
				}
				else
				{
					Process(SM_MOUSEMOVE, 0, 0);
				}
			}
			else if (checked)
			{
				clicked = True;

				for (Int i = 0; i <= text.Length() - invisibleChars + 1; i++)
				{
					if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	newpos = frame.left + 3 + font.GetTextSizeX(visText, i);
					else						newpos = frame.left + 3 + font.GetTextSizeX(String().FillN('*', i), i);

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

				if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
				else						p1.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

				p1.y = frame.top + 2;

				surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);

				promptVisible = True;

				{
					HIMC		 hImc = ImmGetContext((HWND) wnd->GetSystemWindow());
					COMPOSITIONFORM	 info;

					info.dwStyle = CFS_POINT;
					info.ptCurrentPos.x = p1.x - 4;
					info.ptCurrentPos.y = p1.y - 2;

					ImmSetCompositionWindow(hImc, &info);

					if (Setup::enableUnicode)
					{
						LOGFONTW	 lFont;

						lFont.lfHeight = -MulDiv(font.GetSize(), GetDeviceCaps((HDC) surface->GetSystemSurface(), LOGPIXELSY), 72);
						lFont.lfWidth = 0;
						lFont.lfEscapement = 0;
						lFont.lfOrientation = 0;
						lFont.lfWeight = font.GetWeight();
						lFont.lfItalic = false;
						lFont.lfUnderline = false;
						lFont.lfStrikeOut = false;
						lFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
						lFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
						lFont.lfCharSet = DEFAULT_CHARSET;
						lFont.lfQuality = DEFAULT_QUALITY;
						lFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
						wcscpy(lFont.lfFaceName, font.GetName());

						ImmSetCompositionFontW(hImc, &lFont);
					}
					else
					{
						LOGFONTA	 lFont;

						lFont.lfHeight = -MulDiv(font.GetSize(), GetDeviceCaps((HDC) surface->GetSystemSurface(), LOGPIXELSY), 72);
						lFont.lfWidth = 0;
						lFont.lfEscapement = 0;
						lFont.lfOrientation = 0;
						lFont.lfWeight = font.GetWeight();
						lFont.lfItalic = false;
						lFont.lfUnderline = false;
						lFont.lfStrikeOut = false;
						lFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
						lFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
						lFont.lfCharSet = DEFAULT_CHARSET;
						lFont.lfQuality = DEFAULT_QUALITY;
						lFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
						strcpy(lFont.lfFaceName, font.GetName());

						ImmSetCompositionFontA(hImc, &lFont);
					}

					ImmDestroyContext(hImc);
				}

				timer = new System::Timer();

				timer->onInterval.Connect(&EditBox::TimerProc, this);
				timer->Start(500);
			}

			break;
		case SM_LBUTTONUP:
			if (clicked && markStart != -1)
			{
				if (markStart == markEnd) { markStart = -1; markEnd = -1; }

				marking = False;
			}

			break;
		case SM_MOUSEMOVE:
			if (!checked && wnd->IsMouseOn(frame))
			{
				LiSASetMouseCursor((HWND) wnd->GetSystemWindow(), LiSA_MOUSE_TEXTEDIT);

				checked = True;
			}
			else if (checked && !wnd->IsMouseOn(frame))
			{
				LiSASetMouseCursor((HWND) wnd->GetSystemWindow(), LiSA_MOUSE_ARROW);

				checked = False;
			}

			if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
			else					leftButton = VK_LBUTTON;

			GetAsyncKeyState(leftButton);

			if (GetAsyncKeyState(leftButton) == 0 && marking) marking = False;

			if (clicked && markStart != -1 && marking)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				for (Int i = 0; i <= text.Length() - invisibleChars + 1; i++)
				{
					if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	newpos = frame.left + 3 + font.GetTextSizeX(visText, i);
					else						newpos = frame.left + 3 + font.GetTextSizeX(String().FillN('*', i), i);

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
			if (clicked) Process(SM_LBUTTONDOWN, 0, 0);

			break;
		case WM_KEYDOWN:
			if (clicked)
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
						if (clicked)
						{
							Process(SM_LBUTTONDOWN, 0, 1);

							onEnter.Emit();
						}

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
			if (clicked)
			{
				if (wParam == 3 && !(lParam & (1 << 30)))
				{
					if (markStart != markEnd)
					{
						String	 mText;

						for (int j = markStart; j < markEnd; j++) mText[j - markStart] = text[j];

						char	*bAmText = NIL;
						wchar_t	*bWmText = NIL;

						if (Setup::enableUnicode)	bWmText = new wchar_t [mText.Length() + 1];
						else				bAmText = new char [mText.Length() + 1];

						if (Setup::enableUnicode)	wcscpy(bWmText, mText);
						else				strcpy(bAmText, mText);

						OpenClipboard((HWND) wnd->GetSystemWindow());

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

					OpenClipboard((HWND) wnd->GetSystemWindow());

					if (Setup::enableUnicode && IsClipboardFormatAvailable(CF_UNICODETEXT))
					{
						insertText = (wchar_t *) GetClipboardData(CF_UNICODETEXT);
					}
					else if (IsClipboardFormatAvailable(CF_TEXT))
					{
						insertText = (char *) GetClipboardData(CF_TEXT);
					}

					CloseClipboard();

					if (insertText.Length() > 0 && (insertText.Length() + text.Length()) <= maxSize)
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
	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 p1;
	String	 visText = GetVisibleText();
	Int	 oInvisChars = invisibleChars;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx - (dropDownList == NIL ? 0 : 18);
	frame.bottom	= realPos.y + size.cy;

	if (scrollbar != NIL) frame.right -= 18;

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
	else						p1.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

	p1.y = frame.top + 2;

	if (promptVisible) surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);

	promptPos = newPos;

	if (promptPos <= invisibleChars && invisibleChars > 0) invisibleChars = promptPos;

	visText = GetVisibleText();

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
	else						p1.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

	while (p1.x >= (frame.right - 1))
	{
		invisibleChars++;

		visText = "";

		for (Int i = 0; i < text.Length() - invisibleChars; i++)
		{
			visText[i] = text[i + invisibleChars];
		}

		if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
		else						p1.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);
	}

	if (invisibleChars != oInvisChars) ModifyText(text);

	promptPos = newPos;

	surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);

	{
		HIMC		 hImc = ImmGetContext((HWND) wnd->GetSystemWindow());
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

	timer = new System::Timer();

	timer->onInterval.Connect(&EditBox::TimerProc, this);
	timer->Start(500);

	promptVisible = True;
}

S::Void S::GUI::EditBox::RemoveCursor()
{
	if (!promptVisible) return;

	Surface	*surface = container->GetDrawSurface();
	Point	 p = GetRealPosition();
	String	 visText = GetVisibleText();

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p.x += 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
	else						p.x += 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

	p.y += 2;

	surface->Box(Rect(p, Size(1, 15)), 0, INVERT);

	promptVisible = False;
}

S::Void S::GUI::EditBox::MarkText(Int prevMarkStart, Int prevMarkEnd)
{
	if (prevMarkStart == markStart && prevMarkEnd == markEnd) return;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 p1;
	String	 visText = GetVisibleText();

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx - (dropDownList == NIL ? 0 : 18);
	frame.bottom	= realPos.y + size.cy;

	if (scrollbar != NIL) frame.right -= 18;

	Int	 bColor = GetSysColor(COLOR_HIGHLIGHT);
	Int	 tColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
	String	 mText;

	if (prevMarkStart != prevMarkEnd && prevMarkStart >= 0 && prevMarkEnd >= 0)
	{
		if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
		else						p1.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

		p1.y = frame.top + 2;

		if (promptVisible) surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);

		for (int j = prevMarkStart; j < prevMarkEnd; j++) mText[j - prevMarkStart] = text[j];

		frame.left	= realPos.x + font.GetTextSizeX(visText, prevMarkStart - invisibleChars) + 3;
		frame.top	= realPos.y + 2;
		frame.right	= frame.left + font.GetTextSizeX(mText);
		frame.bottom	= frame.top + font.GetLineSizeY(mText) + 3;

		surface->Box(frame, Setup::ClientColor, FILLED);

		frame.top++;

		surface->SetText(mText, frame, font);

		if (promptVisible) surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);
	}

	mText = "";

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx - (dropDownList == NIL ? 0 : 18);
	frame.bottom	= realPos.y + size.cy;

	if (scrollbar != NIL) frame.right -= 18;

	if (markStart != markEnd && markStart >= 0 && markEnd >= 0)
	{
		if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
		else						p1.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

		p1.y = frame.top + 2;

		if (promptVisible) surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);

		for (int j = markStart; j < markEnd; j++) mText[j - markStart] = text[j];

		frame.left	= realPos.x + font.GetTextSizeX(visText, markStart - invisibleChars) + 3;
		frame.top	= realPos.y + 2;
		frame.right	= frame.left + font.GetTextSizeX(mText);
		frame.bottom	= frame.top + font.GetLineSizeY(mText) + 3;

		surface->Box(frame, bColor, FILLED);

		frame.top++;

		Font	 nFont = font;

		nFont.SetColor(tColor);

		surface->SetText(mText, frame, nFont);

		if (promptVisible) surface->Box(Rect(p1, Size(1, 15)), 0, INVERT);
	}
}

S::Int S::GUI::EditBox::MarkAll()
{
	RemoveCursor();
	SetCursor(text.Length());

	Int	 prevMarkStart = markStart;
	Int	 prevMarkEnd = markEnd;

	markStart = 0;
	markEnd = text.Length();

	MarkText(prevMarkStart, prevMarkEnd);

	clicked = True;

	return Success;
}

S::Void S::GUI::EditBox::DeleteSelectedText()
{
	if (markStart == markEnd || markStart < 0 || markEnd < 0) return;

	Int	 bMarkStart = markStart;
	Int	 bMarkEnd = markEnd;

	markStart = -1;
	markEnd = -1;

	MarkText(bMarkStart, bMarkEnd);

	Int	 nOfChars = text.Length();
	String	 newText;
	Int	 prevPromptPos = promptPos;

	for (Int i = 0; i < bMarkStart; i++)		newText[i] = text[i];
	for (Int j = bMarkEnd; j <= nOfChars; j++)	newText[j - (bMarkEnd - bMarkStart)] = text[j];

	ModifyText(newText);

	promptPos = prevPromptPos;

	SetCursor(bMarkStart);
}

S::Void S::GUI::EditBox::InsertText(String insertText)
{
	Int	 nOfChars = text.Length();
	String	 newText;
	Int	 prevPromptPos = promptPos;

	for (Int i = 0; i < promptPos; i++)					newText[i] = text[i];
	for (Int j = promptPos; j < promptPos + insertText.Length(); j++)	newText[j] = insertText[j - promptPos];
	for (Int k = promptPos; k <= nOfChars; k++)				newText[k + insertText.Length()] = text[k];

	ModifyText(newText);

	promptPos = prevPromptPos;

	SetCursor(promptPos + insertText.Length());
}

S::Int S::GUI::EditBox::Show()
{
	if (comboBox != NIL)
	{
		comboBox->SetMetrics(pos, size);
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
	if (clicked)
	{
		if (timer != NIL)
		{
			timer->Stop();

			delete timer;

			timer = NIL;
		}

		clicked = False;
	}

	if (comboBox != NIL) comboBox->Deactivate();

	return Widget::Deactivate();
}

S::Int S::GUI::EditBox::SetText(const String &txt)
{
	scrollbarPos = 0;

	return ModifyText(txt);
}

S::Int S::GUI::EditBox::ModifyText(String txt)
{
	RemoveCursor();

	promptPos = 0;

	if (text == txt)
	{
		Paint(SP_PAINT);

		return Success;
	}

	text = txt;
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

	ModifyText(comboBox->GetSelectedEntry()->GetText());

	checked = True;

	Process(SM_LBUTTONDOWN, 0, 0);
	SetCursor(text.Length());

	markStart = 0;
	markEnd = text.Length();

	MarkText(0, 0);
	SetCursor(text.Length());
}

S::Void S::GUI::EditBox::TimerProc()
{
	if (!registered)		return;
	if (!active || !visible)	return;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 lineStart;
	String	 visText = GetVisibleText();

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx;
	frame.bottom	= realPos.y + size.cy;

	if (scrollbar != NIL) frame.right -= 18;

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	lineStart.x = frame.left + 3 + font.GetTextSizeX(visText, promptPos - invisibleChars);
	else						lineStart.x = frame.left + 3 + font.GetTextSizeX(String().FillN('*', promptPos - invisibleChars), promptPos - invisibleChars);

	lineStart.y = frame.top + 2;

	surface->Box(Rect(lineStart, Size(1, 15)), 0, INVERT);

	promptVisible = !promptVisible;
}

S::Void S::GUI::EditBox::ScrollbarProc()
{
	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx;
	frame.bottom	= realPos.y + size.cy;

	surface->StartPaint(frame);
	Paint(SP_PAINT);
	surface->EndPaint();
}

S::Int S::GUI::EditBox::GetCursorPos()
{
	if (clicked)	return promptPos;
	else		return -1;
}

S::Int S::GUI::EditBox::GetNOfLines()
{
	Int	 lines = 1;

	for (Int i = 0; i < text.Length(); i++)
	{
		if (text[i] == '\n') lines++;
	}

	return lines;
}

S::Int S::GUI::EditBox::GetNOfInvisibleLines()
{
	return 1 + GetNOfLines() - Math::Floor((size.cy - 6) / (font.GetLineSizeY(text) + 1));
}

S::String S::GUI::EditBox::GetVisibleText()
{
	String	 visibleText = text;

	if (invisibleChars > 0)
	{
		visibleText = "";

		for (Int i = 0; i < text.Length() - invisibleChars; i++)
		{
			visibleText[i] = text[i + invisibleChars];
		}
	}

	if (scrollbar != NIL)
	{
		visibleText = "";

		Int	 invisibleLines = scrollbarPos;
		Int	 i;

		for (i = 0; i < text.Length(); i++)
		{
			if (text[i] == '\n') { invisibleLines--; i++; }

			if (invisibleLines == 0) break;
		}

		for (Int j = 0; j < text.Length() - i; j++)
		{
			visibleText[j] = text[j + i];
		}
	}

	return visibleText;
}
