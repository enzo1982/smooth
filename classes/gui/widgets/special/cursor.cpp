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
	scrollPos	= 0;
	maxScrollPos	= 0;

	onLoseFocus.Connect(&Cursor::OnLoseFocus, this);

	onInput.SetParentObject(this);
	onEnter.SetParentObject(this);
	onScroll.SetParentObject(this);
}

S::GUI::Cursor::~Cursor()
{
	if (timer != NIL) DeleteObject(timer);
}

S::Int S::GUI::Cursor::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Int	 nMaxScrollPos = Math::Max(0, Math::Ceil((Float) (font.GetTextSizeY(text) - GetHeight()) / (font.GetTextSizeY("*") + 3)));

				if (nMaxScrollPos != maxScrollPos)
				{
					maxScrollPos = nMaxScrollPos;

					if (maxScrollPos == 0) scrollPos = 0;

					onScroll.Emit(scrollPos, maxScrollPos);
				}

				Surface	*surface = container->GetDrawSurface();
				Point	 realPos = GetRealPosition();
				Rect	 frame	 = Rect(realPos, GetSize());

				surface->StartPaint(frame);

				surface->Box(frame, GetBackgroundColor(), FILLED);

				String	 line;
				Int	 lineNumber = 0;
				Int	 lineStart = 0;

				for (Int i = 0; i <= text.Length(); i++)
				{
					line[i - lineStart] = text[i];

					if (text[i] == '\n' || text[i] == 0)
					{
						line[i - lineStart] = 0;

						if (lineNumber >= scrollPos)
						{
							if (!Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK))	surface->SetText(line, frame + Point(-visibleOffset, 1 + (lineNumber - scrollPos) * (font.GetTextSizeY("*") + 3)) + Size(visibleOffset, -2), font);
							else								surface->SetText(String().FillN('*', line.Length()), frame + Point(-visibleOffset, 1 + (lineNumber - scrollPos) * (font.GetTextSizeY("*") + 3)) + Size(visibleOffset, -2), font);

							if (markStart != markEnd && markStart >= 0 && markEnd >= 0)
							{
								Int	 lineMarkStart = Math::Max(0, Math::Min(markStart, markEnd) - lineStart);
								Int	 lineMarkEnd = Math::Min(line.Length(), Math::Max(markStart, markEnd) - lineStart);

								if (lineMarkStart < line.Length() && lineMarkEnd > 0)
								{
									Int	 bColor	 = GetSysColor(COLOR_HIGHLIGHT);
									Int	 tColor	 = GetSysColor(COLOR_HIGHLIGHTTEXT);

									String	 mText;
									String	 wText = line;

									if (Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK)) wText.FillN('*', wText.Length());

									for (Int j = lineMarkStart; j < lineMarkEnd; j++) mText[j - lineMarkStart] = wText[j];

									Rect	 markRect = Rect(realPos + Point(font.GetTextSizeX(wText, lineMarkStart) - visibleOffset, (lineNumber - scrollPos) * (font.GetTextSizeY("*") + 3)), Size(font.GetTextSizeX(mText), font.GetTextSizeY("*") + 3));

									surface->Box(markRect, bColor, FILLED);

									Font	 nFont = font;

									nFont.SetColor(tColor);

									surface->SetText(mText, markRect + Point(0, 1) - Size(0, 2), nFont);
								}
							}
						}

						lineStart = i + 1;
						lineNumber++;
					}
				}

				surface->EndPaint();
			}

			break;
		case SP_MOUSEIN:
			LiSASetMouseCursor((HWND) container->GetContainerWindow()->GetSystemWindow(), LiSA_MOUSE_TEXTEDIT);

			break;
		case SP_MOUSEOUT:
			LiSASetMouseCursor((HWND) container->GetContainerWindow()->GetSystemWindow(), LiSA_MOUSE_ARROW);

			break;
	}

	return Success();
}

S::Int S::GUI::Cursor::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Window	*window	= container->GetContainerWindow();

	if (window == NIL) return Success();

	Rect	 frame	= Rect(GetRealPosition(), GetSize());

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (mouseOver)
			{
				Int	 newPromptPos = 0;

				String	 wText = text;
				Int	 wPromptPos = 0;
				Int	 line = 0;

				if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
				{
					line = (window->MouseY() - frame.top) / (font.GetTextSizeY("*") + 3);

					Int	 lineCount = 0;

					for (Int i = 0; i <= text.Length(); i++)
					{
						if (text[i] == '\n' || text[i] == 0)
						{
							wText[i - wPromptPos] = 0;

							if (lineCount - scrollPos == line) break;

							wPromptPos = i;
							lineCount++;
						}

						wText[i - wPromptPos] = text[i];
					}
				}

				Int	 newPos	 = 0;
				Int	 lastPos = 0;

				for (Int i = 0; i <= wText.Length(); i++)
				{
					if (!Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK))	newPos = frame.left + font.GetTextSizeX(wText, i) - visibleOffset;
					else								newPos = frame.left + font.GetTextSizeX(String().FillN('*', i), i) - visibleOffset;

					if (i > 0 && window->MouseX() < (lastPos + newPos) / 2)  { newPromptPos	= i - 1; break; }
					else if (i == wText.Length())				   newPromptPos	= wText.Length();
					else							   lastPos	= newPos;
				}

				newPromptPos += wPromptPos;

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

				String	 wText = text;
				Int	 wPromptPos = 0;
				Int	 line = 0;

				if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
				{
					line = (window->MouseY() - frame.top) / (font.GetTextSizeY("*") + 3);

					if (window->MouseY() - frame.top < 0) line--;

					Int	 lineCount = 0;

					for (Int i = 0; i <= text.Length(); i++)
					{
						if (text[i] == '\n' || text[i] == 0)
						{
							wText[i - wPromptPos] = 0;

							if (lineCount - scrollPos == line) break;

							wPromptPos = i;
							lineCount++;
						}

						wText[i - wPromptPos] = text[i];
					}

					if (lineCount - scrollPos != line) wPromptPos = -1;
				}

				if (wPromptPos >= 0)
				{
					Int	 newPos	 = 0;
					Int	 lastPos = 0;

					for (Int i = 0; i <= wText.Length(); i++)
					{
						if (!Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK))	newPos = frame.left + font.GetTextSizeX(wText, i) - visibleOffset;
						else								newPos = frame.left + font.GetTextSizeX(String().FillN('*', i), i) - visibleOffset;

						if (i > 0 && window->MouseX() < (lastPos + newPos) / 2)  { newMarkEnd	= i - 1; break; }
						else if (i == wText.Length())				   newMarkEnd	= wText.Length();
						else							   lastPos	= newPos;
					}

					newMarkEnd += wPromptPos;

					MarkText(markStart, newMarkEnd);

					SetCursorPos(newMarkEnd);
				}
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

						if (wParam == VK_LEFT && promptPos == 0)	      break;
						if (wParam == VK_HOME && promptPos == 0)	      break;
						if (wParam == VK_RIGHT && promptPos >= text.Length()) break;
						if (wParam == VK_END && promptPos >= text.Length())   break;

						if (wParam == VK_LEFT)	newPos = promptPos - 1;
						if (wParam == VK_RIGHT)	newPos = promptPos + 1;
						if (wParam == VK_HOME)	newPos = 0;
						if (wParam == VK_END)	newPos = text.Length();

						SetCursorPos(newPos);

						break;
					case VK_UP:
					case VK_DOWN:
						if (!Binary::IsFlagSet(GetFlags(), CF_MULTILINE)) break;

						{
							Int	 linePos = promptPos;
							Int	 newPos = promptPos;

							Int	 i = 0;

							for (i = promptPos - 1; i >= 0; i--)
							{
								if (text[i] == '\n') { linePos -= (i + 1); break; }
							}

							if (wParam == VK_UP)
							{
								if (i == 0) newPos = 0;

								for (Int j = i - 1; j >= 0; j--)
								{
									if (text[j] == '\n' || j == 0) { newPos = Math::Min(i, j + linePos + (j == 0 ? 0 : 1)); break; }
								}
							}
							else
							{
								for (Int j = promptPos; j < text.Length(); j++)
								{
									if (text[j] == '\n')
									{
										for (Int k = j + 1; k <= j + 1 + linePos; k++)
										{
											newPos = k;

											if (text[k] == '\n' || text[k] == 0) break;
										}

										break;
									}
								}
							}

							SetCursorPos(newPos);
						}

						break;
					case VK_RETURN:
						if (!IsActive()) break;

						if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
						{
							DeleteSelectedText();

							String	 insertText;

							insertText[0] = '\n';

							InsertText(insertText);
						}
						else
						{
							focussed = False;

							onLoseFocus.Emit();

							onEnter.Emit(text);
						}

						break;
					case VK_BACK:
					case VK_DELETE:
						if (!IsActive()) break;

						if (markStart != markEnd) { DeleteSelectedText(); break; }

						if (promptPos == 0 && wParam == VK_BACK)		break;
						if (promptPos == text.Length() && wParam == VK_DELETE)	break;

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

						for (int j = Math::Min(markStart, markEnd); j < Math::Max(markStart, markEnd); j++) mText[j - Math::Min(markStart, markEnd)] = text[j];

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
						if (Binary::IsFlagSet(container->GetFlags(), EDB_NUMERIC) && (insertText.ToInt() == 0 && insertText[0] != '0')) break;

						InsertText(insertText);
					}
				}

				if (text.Length() == maxSize && markStart == markEnd) break;

				if (wParam >= 32 && IsActive())
				{
					DeleteSelectedText();

					if (Binary::IsFlagSet(container->GetFlags(), EDB_NUMERIC) && (wParam < '0' || wParam > '9') && wParam != 45 && wParam != '.') break;

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

	String	 wText = text;
	Int	 wPromptPos = promptPos;
	Int	 line = 0;

	if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
	{
		for (Int i = promptPos - 1; i >= 0; i--)
		{
			if (text[i] == '\n')
			{
				for (Int j = i + 1; j < promptPos; j++) wText[j - i - 1] = text[j];

				wText[promptPos - i - 1] = 0;
				wPromptPos = promptPos - i - 1;

				break;
			}
		}

		for (Int j = promptPos - 1; j >= 0; j--)
		{
			if (text[j] == '\n') line++;
		}
	}

	if (!Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK))	point.x += font.GetTextSizeX(wText, wPromptPos) - visibleOffset;
	else								point.x += font.GetTextSizeX(String().FillN('*', wPromptPos), wPromptPos) - visibleOffset;

	point.y += (font.GetTextSizeY("*") + 3) * (line - scrollPos);

	if (!(line - scrollPos < 0 || (font.GetTextSizeY("*") + 3) * (line - scrollPos + 1) > GetHeight()))
	{
		surface->Box(Rect(point, Size(1, font.GetTextSizeY("*") + 3)), 0, INVERT);

		promptVisible = visible;
	}
}

S::Void S::GUI::Cursor::MarkText(Int newMarkStart, Int newMarkEnd)
{
	if (newMarkStart == markStart && newMarkEnd == markEnd) return;

	ShowCursor(False);

	markStart	= newMarkStart;
	markEnd		= newMarkEnd;

	Paint(SP_PAINT);
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

	String	 newText;

	for (Int i = 0; i < bMarkStart; i++)		newText[i] = text[i];
	for (Int j = bMarkEnd; j <= text.Length(); j++)	newText[j - (bMarkEnd - bMarkStart)] = text[j];

	SetText(newText);

	SetCursorPos(bMarkStart);

	onInput.Emit(newText);
}

S::Void S::GUI::Cursor::InsertText(const String &insertText)
{
	ShowCursor(False);

	String	 newText;

	for (Int i = 0; i < promptPos; i++)					newText[i] = text[i];
	for (Int j = promptPos; j < promptPos + insertText.Length(); j++)	newText[j] = insertText[j - promptPos];
	for (Int k = promptPos; k <= text.Length(); k++)			newText[k + insertText.Length()] = text[k];

	Surface	*surface = container->GetDrawSurface();

	surface->StartPaint(Rect(container->GetRealPosition(), container->GetSize()));

	SetText(newText);

	surface->EndPaint();

	SetCursorPos(promptPos + insertText.Length());

	onInput.Emit(newText);
}

S::Int S::GUI::Cursor::Scroll(Int nScrollPos)
{
	ShowCursor(False);

	scrollPos = Math::Max(0, Math::Min(nScrollPos, maxScrollPos));

	Paint(SP_PAINT);

	onScroll.Emit(scrollPos, maxScrollPos);

	return Success();
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

	String	 wText = text;
	Int	 wPromptPos = promptPos;
	Int	 line = 0;

	if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
	{
		for (Int i = promptPos - 1; i >= 0; i--)
		{
			if (text[i] == '\n')
			{
				for (Int j = i + 1; j < promptPos; j++) wText[j - i - 1] = text[j];

				wText[promptPos - i - 1] = 0;
				wPromptPos = promptPos - i - 1;

				break;
			}
		}

		for (Int j = promptPos - 1; j >= 0; j--)
		{
			if (text[j] == '\n') line++;
		}
	}

	if (!Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK))	p1.x += font.GetTextSizeX(wText, wPromptPos) - visibleOffset;
	else								p1.x += font.GetTextSizeX(String().FillN('*', wPromptPos), wPromptPos) - visibleOffset;

	while (p1.x > frame.right || p1.x < frame.left)
	{
		if (p1.x > frame.right)     { visibleOffset +=  5; p1.x -=  5; }
		else if (p1.x < frame.left) { visibleOffset -= 20; p1.x += 20; }
	}

	if (visibleOffset < 0) { p1.x -= visibleOffset; visibleOffset = 0; }

	p1.y += (font.GetTextSizeY("*") + 3) * (line - scrollPos);

	if (line - scrollPos < 0 || (font.GetTextSizeY("*") + 3) * (line - scrollPos + 1) > GetHeight())
	{
		while (line - scrollPos < 0)						    scrollPos--;
		while ((font.GetTextSizeY("*") + 3) * (line - scrollPos + 1) > GetHeight()) scrollPos++;

		onScroll.Emit(scrollPos, maxScrollPos);

		Sleep(100);

		return Success();
	}

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

	Paint(SP_PAINT);

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

S::Int S::GUI::Cursor::GetMaxSize()
{
	return maxSize;
}
