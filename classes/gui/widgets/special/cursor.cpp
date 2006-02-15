 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/special/cursor.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/gui/widgets/basic/editbox.h>
#include <smooth/misc/binary.h>
#include <smooth/misc/math.h>
#include <smooth/system/timer.h>
#include <smooth/graphics/surface.h>

#include <imm.h>

const S::Int	 S::GUI::Cursor::classID = S::Object::RequestClassID();

S::GUI::Cursor::Cursor(const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type		= classID;
	markStart	= -1;
	markEnd		= -1;
	maxSize		= 32768;
	promptPos	= 0;
	promptVisible	= False;
	timer		= NIL;
	marking		= False;
	visibleOffset	= 0;

	onLoseFocus.Connect(&Cursor::OnLoseFocus, this);

	onInput.SetParentObject(this);
}

S::GUI::Cursor::~Cursor()
{
	if (timer != NIL) DeleteObject(timer);
}

S::Int S::GUI::Cursor::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	EnterProtectedRegion();

	Surface	*surface = container->GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->StartPaint(frame);

			surface->Box(frame, GetBackgroundColor(), FILLED);
			surface->SetText(text, frame + Point(-visibleOffset, 1) + Size(visibleOffset, -2), font);

			surface->EndPaint();

			break;
		case SP_MOUSEIN:
			LiSASetMouseCursor((HWND) container->GetContainerWindow()->GetSystemWindow(), LiSA_MOUSE_TEXTEDIT);

			break;
		case SP_MOUSEOUT:
			LiSASetMouseCursor((HWND) container->GetContainerWindow()->GetSystemWindow(), LiSA_MOUSE_ARROW);

			break;
	}

	LeaveProtectedRegion();

	return Success();
}

S::Int S::GUI::Cursor::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Window	*window		= container->GetContainerWindow();

	if (window == NIL) return Success();

	Rect	 frame		= Rect(GetRealPosition(), GetSize());
	Int	 nOfChars	= text.Length();

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (mouseOver)
			{
				Int	 newPromptPos = 0;

				Int	 newPos	 = 0;
				Int	 lastPos = 0;

				for (Int i = 0; i <= nOfChars; i++)
				{
					if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	newPos = frame.left + font.GetTextSizeX(text, i) - visibleOffset;
					else						newPos = frame.left + font.GetTextSizeX(String().FillN('*', i), i) - visibleOffset;

					if (i > 0 && window->MouseX() < (lastPos + newPos) / 2)  { newPromptPos	= i - 1; break; }
					else if (i == nOfChars)					   newPromptPos	= nOfChars;
					else							   lastPos	= newPos;
				}

				marking = True;

				MarkText(newPromptPos, -1);

				SetCursorPos(newPromptPos);

				if (!focussed)
				{
					focussed = True;

					onGetFocus.Emit();
				}
				else
				{
					onClickInFocus.Emit();
				}
			}
			else
			{
				if (focussed)
				{
					focussed = False;

					onLoseFocus.Emit();
				}
			}

			break;
		case SM_LBUTTONUP:
			if (focussed && markStart != -1)
			{
				if (markStart == markEnd) { markStart = -1; markEnd = -1; }

				marking = False;
			}

			break;
		case SM_MOUSEMOVE:
			if (!mouseOver && window->IsMouseOn(frame))
			{
				mouseOver = True;

				Paint(SP_MOUSEIN);
			}
			else if (mouseOver && !window->IsMouseOn(frame))
			{
				mouseOver = False;

				Paint(SP_MOUSEOUT);
			}

			if (focussed && markStart != -1 && marking)
			{
				Int	 newMarkEnd = 0;

				Int	 newPos	 = 0;
				Int	 lastPos = 0;

				for (Int i = 0; i <= nOfChars; i++)
				{
					if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	newPos = frame.left + font.GetTextSizeX(text, i) - visibleOffset;
					else						newPos = frame.left + font.GetTextSizeX(String().FillN('*', i), i) - visibleOffset;

					if (i > 0 && window->MouseX() < (lastPos + newPos) / 2)  { newMarkEnd	= i - 1; break; }
					else if (i == nOfChars)					   newMarkEnd	= nOfChars;
					else							   lastPos	= newPos;
				}

				MarkText(markStart, newMarkEnd);

				SetCursorPos(newMarkEnd);
			}

			break;
		case WM_KEYDOWN:
			if (focussed)
			{
				Int	 newPos = 0;

				switch (wParam)
				{
					case VK_LEFT:
					case VK_RIGHT:
					case VK_HOME:
					case VK_END:
						MarkText(-1, -1);

						if (wParam == VK_LEFT && promptPos == 0)	 break;
						if (wParam == VK_HOME && promptPos == 0)	 break;
						if (wParam == VK_RIGHT && promptPos >= nOfChars) break;
						if (wParam == VK_END && promptPos >= nOfChars)	 break;

						if (wParam == VK_LEFT)	newPos = promptPos - 1;
						if (wParam == VK_RIGHT)	newPos = promptPos + 1;
						if (wParam == VK_HOME)	newPos = 0;
						if (wParam == VK_END)	newPos = nOfChars;

						SetCursorPos(newPos);

						break;
					case VK_RETURN:
						if (!IsActive()) break;

						onEnter.Emit(text);

						break;
					case VK_BACK:
					case VK_DELETE:
						if (!IsActive()) break;

						if (markStart != markEnd) { DeleteSelectedText(); break; }

						if (promptPos == 0 && wParam == VK_BACK)		break;
						if (promptPos == nOfChars && wParam == VK_DELETE)	break;

						if (wParam == VK_BACK)
						{
							markStart	= promptPos - 1;
							markEnd		= promptPos;
						}
						else
						{
							markStart	= promptPos;
							markEnd		= promptPos + 1;
						}

						DeleteSelectedText();

						break;
					default:
						break;
				}
			}

			break;
		case WM_CHAR:
			if (focussed)
			{
				if (wParam == 3 && !(lParam & (1 << 30)))
				{
					if (markStart != markEnd)
					{
						String	 mText;

						for (int j = markStart; j < markEnd; j++) mText[j - markStart] = text[j];

						OpenClipboard((HWND) window->GetSystemWindow());

						HGLOBAL	 memory = NIL;

						if (Setup::enableUnicode)
						{
							memory = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, 2 * (mText.Length() + 1));

							wcscpy((wchar_t *) GlobalLock(memory), mText);

							SetClipboardData(CF_UNICODETEXT, memory);
						}
						else
						{
							memory = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, mText.Length() + 1);

							strcpy((char *) GlobalLock(memory), mText);

							SetClipboardData(CF_TEXT, memory);
						}

						CloseClipboard();
					}
				}

				if (wParam == 24 && !(lParam & (1 << 30)) && IsActive())
				{
					Process(WM_CHAR, 3, 0);

					DeleteSelectedText();
				}

				if (wParam == 22 && IsActive())
				{
					DeleteSelectedText();

					String	 insertText;

					OpenClipboard((HWND) window->GetSystemWindow());

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

				if (wParam >= 32 && IsActive())
				{
					DeleteSelectedText();

					if (Binary::IsFlagSet(flags, EDB_NUMERIC) && (wParam < '0' || wParam > '9') && wParam != 45 && wParam != '.') break;

					String	 insertText;

					insertText[0] = wParam;

					InsertText(insertText);
				}
			}

			break;
#ifdef __WIN32__
		case WM_KILLFOCUS:
			if (Window::GetWindow((HWND) wParam) != NIL)
			{
				if (Window::GetWindow((HWND) wParam)->GetObjectType() == ToolWindow::classID || Window::GetWindow((HWND) wParam) == window) break;
			}

			if (focussed)
			{
				focussed = False;

				onLoseFocus.Emit();
			}

			break;
#endif
	}

	return Widget::Process(message, wParam, lParam);
}

S::Void S::GUI::Cursor::ShowCursor(Bool visible)
{
	if (promptVisible == visible) return;

	Surface	*surface = container->GetDrawSurface();
	Point	 point	 = GetRealPosition();

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	point.x += font.GetTextSizeX(text, promptPos) - visibleOffset;
	else						point.x += font.GetTextSizeX(String().FillN('*', promptPos), promptPos) - visibleOffset;

	surface->Box(Rect(point, Size(1, GetHeight())), 0, INVERT);

	promptVisible = visible;
}

S::Void S::GUI::Cursor::MarkText(Int newMarkStart, Int newMarkEnd)
{
	if (newMarkStart == markStart && newMarkEnd == markEnd) return;

	ShowCursor(False);

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();

	Int	 bColor	 = GetSysColor(COLOR_HIGHLIGHT);
	Int	 tColor	 = GetSysColor(COLOR_HIGHLIGHTTEXT);

	if (markStart != markEnd && markStart >= 0 && markEnd >= 0)
	{
		String	 mText;

		for (Int i = Math::Min(markStart, markEnd); i < Math::Max(markStart, markEnd); i++) mText[i - Math::Min(markStart, markEnd)] = text[i];

		Rect	 markRect = Rect(realPos + Point(font.GetTextSizeX(text, Math::Min(markStart, markEnd)) - visibleOffset, 0), Size(font.GetTextSizeX(mText), GetHeight()));

		surface->Box(markRect, GetBackgroundColor(), FILLED);
		surface->SetText(mText, markRect + Point(0, 1) - Size(0, 2), font);
	}

	markStart	= newMarkStart;
	markEnd		= newMarkEnd;

	if (newMarkStart != newMarkEnd && newMarkStart >= 0 && newMarkEnd >= 0)
	{
		String	 mText;

		for (Int i = Math::Min(markStart, markEnd); i < Math::Max(markStart, markEnd); i++) mText[i - Math::Min(markStart, markEnd)] = text[i];

		Rect	 markRect = Rect(realPos + Point(font.GetTextSizeX(text, Math::Min(markStart, markEnd)) - visibleOffset, 0), Size(font.GetTextSizeX(mText), GetHeight()));

		surface->Box(markRect, bColor, FILLED);

		Font	 nFont = font;

		nFont.SetColor(tColor);

		surface->SetText(mText, markRect + Point(0, 1) - Size(0, 2), nFont);
	}
}

S::Int S::GUI::Cursor::MarkAll()
{
	focussed = True;

	MarkText(0, text.Length());

	SetCursorPos(text.Length());

	return Success();
}

S::Void S::GUI::Cursor::DeleteSelectedText()
{
	if (markStart == markEnd || markStart < 0 || markEnd < 0) return;

	ShowCursor(False);

	Int	 bMarkStart	= Math::Min(markStart, markEnd);
	Int	 bMarkEnd	= Math::Max(markStart, markEnd);

	MarkText(-1, -1);

	Int	 nOfChars = text.Length();
	String	 newText;

	for (Int i = 0; i < bMarkStart; i++)		newText[i] = text[i];
	for (Int j = bMarkEnd; j <= nOfChars; j++)	newText[j - (bMarkEnd - bMarkStart)] = text[j];

	SetText(newText);

	SetCursorPos(bMarkStart);

	onInput.Emit(newText);
}

S::Void S::GUI::Cursor::InsertText(const String &insertText)
{
	ShowCursor(False);

	Int	 nOfChars = text.Length();
	String	 newText;

	for (Int i = 0; i < promptPos; i++)					newText[i] = text[i];
	for (Int j = promptPos; j < promptPos + insertText.Length(); j++)	newText[j] = insertText[j - promptPos];
	for (Int k = promptPos; k <= nOfChars; k++)				newText[k + insertText.Length()] = text[k];

	SetText(newText);

	SetCursorPos(promptPos + insertText.Length());

	onInput.Emit(newText);
}

S::Void S::GUI::Cursor::OnTimer()
{
	ShowCursor(!promptVisible);
}

S::Void S::GUI::Cursor::OnLoseFocus()
{
	ShowCursor(False);

	MarkText(-1, -1);

	if (timer != NIL)
	{
		DeleteObject(timer);

		timer = NIL;
	}
}

S::Int S::GUI::Cursor::SetCursorPos(Int newPos)
{
	ShowCursor(False);

	Window	*wnd	 = container->GetContainerWindow();
	Surface	*surface = container->GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetSize());
	Point	 p1	 = GetRealPosition();

	promptPos = newPos;

	if (!Binary::IsFlagSet(flags, EDB_ASTERISK))	p1.x += font.GetTextSizeX(text, promptPos) - visibleOffset;
	else						p1.x += font.GetTextSizeX(String().FillN('*', promptPos), promptPos) - visibleOffset;

	{
		HIMC		 hImc = ImmGetContext((HWND) wnd->GetSystemWindow());
		COMPOSITIONFORM	 info;

		info.dwStyle = CFS_POINT;
		info.ptCurrentPos.x = p1.x - 3;
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

	if (timer != NIL) DeleteObject(timer);

	timer = new System::Timer();

	timer->onInterval.Connect(&Cursor::OnTimer, this);
	timer->Start(500);

	ShowCursor(True);

	return Success();
}

S::Int S::GUI::Cursor::GetCursorPos()
{
	if (focussed)	return promptPos;
	else		return -1;
}

S::Int S::GUI::Cursor::SetMaxSize(Int newMaxSize)
{
	if (newMaxSize <= 0)	maxSize = 32768;
	else			maxSize = newMaxSize;

	return Success();
}
