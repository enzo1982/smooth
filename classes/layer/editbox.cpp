 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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
#include <smooth/surface.h>

#include <imm.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_EDITBOX = S::Object::RequestObjectID();

S::GUI::EditBox::EditBox(String text, Point pos, Size size, Int subType, Int iMaxSize)
{
	type				= OBJ_EDITBOX;
	objectProperties->text		= text;
	isAsterisk			= False;
	markStart			= -1;
	markEnd				= -1;
	objectProperties->fontColor	= Setup::ClientTextColor;
	subtype				= subType;
	maxSize				= iMaxSize;
	promptPos			= 0;
	timer				= NIL;
	marking				= False;

	possibleContainers.AddEntry(OBJ_LAYER);

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
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::EditBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame;
	Rect	 textRect;
	Point	 realPos = GetRealPosition();

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

			textRect.left	= frame.left + 3;
			textRect.top	= frame.top + 3;
			textRect.right	= textRect.left + objectProperties->size.cx - 6;
			textRect.bottom	= textRect.top + 16;

			if (active)	surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
			else		surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, Setup::TextColor, objectProperties->fontWeight);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::EditBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 frame;
	Int	 nOfChars = objectProperties->text.Length();
	Point	 p1;
	Point	 p2;
	String	 newtext;
	Int	 newpos;
	Int	 i;
	Int	 prevPromptPos;
	Int	 leftButton = 0;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
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

				if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
				else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

				p1.y = frame.top + 2;
				p2.x = p1.x;
				p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

				surface->Line(p1, p2, Setup::ClientColor);

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
			if (!active) break;

			if (objectProperties->clicked)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				markStart = -1;
				markEnd = -1;

				MarkText(prevMarkStart, prevMarkEnd);

				objectProperties->clicked = False;
				objectProperties->checked = False;

				if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
				else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

				p1.y = frame.top + 2;
				p2.x = p1.x;
				p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

				surface->Line(p1, p2, Setup::ClientColor);

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
				surface->Frame(frame, FRAME_DOWN);

				for (Int i = 0; i <= objectProperties->text.Length(); i++)
				{
					if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	newpos = frame.left + 3 + GetTextSizeX(objectProperties->text, i, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
					else						newpos = frame.left + 3 + GetTextSizeX(String().FillN('*', i), i, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

					if (i > 0 && wnd->MouseX() < (p1.x + newpos) / 2)
					{
						promptPos = i - 1;

						break;
					}
					else
					{
						p1.x = newpos;

						if (i == nOfChars) promptPos = i;
					}
				}

				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				markStart = promptPos;
				oMarkStart = promptPos;
				markEnd = -1;
				marking = True;

				MarkText(prevMarkStart, prevMarkEnd);

				if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
				else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

				p1.y = frame.top + 2;
				p2.x = p1.x;
				p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

				surface->Line(p1, p2, Setup::TextColor);

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

						font.lfHeight = objectProperties->fontSize;
						font.lfWidth = 0;
						font.lfEscapement = 0;
						font.lfOrientation = 0;
						font.lfWeight = objectProperties->fontWeight;
						font.lfItalic = false;
						font.lfUnderline = false;
						font.lfStrikeOut = false;
						font.lfOutPrecision = OUT_DEFAULT_PRECIS;
						font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
						font.lfCharSet = DEFAULT_CHARSET;
						font.lfQuality = DEFAULT_QUALITY;
						font.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
						wcscpy(font.lfFaceName, objectProperties->font);

						ImmSetCompositionFontW(hImc, &font);
					}
					else
					{
						LOGFONTA	 font;

						font.lfHeight = objectProperties->fontSize;
						font.lfWidth = 0;
						font.lfEscapement = 0;
						font.lfOrientation = 0;
						font.lfWeight = objectProperties->fontWeight;
						font.lfItalic = false;
						font.lfUnderline = false;
						font.lfStrikeOut = false;
						font.lfOutPrecision = OUT_DEFAULT_PRECIS;
						font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
						font.lfCharSet = DEFAULT_CHARSET;
						font.lfQuality = DEFAULT_QUALITY;
						font.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
						strcpy(font.lfFaceName, objectProperties->font);

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
			if (!active) break;

			if (objectProperties->clicked && markStart != -1)
			{
				if (markStart == markEnd) { markStart = -1; markEnd = -1; }

				marking = False;
			}

			break;
		case SM_MOUSEMOVE:
		case SM_MOUSELEAVE:
			if (!active) break;

			if (!objectProperties->checked && wnd->IsMouseOn(frame))
			{
				wnd->cursorset = True;

				LiSASetMouseCursor(LiSA_MOUSE_TEXTEDIT);

				objectProperties->checked = True;

				if (!objectProperties->clicked)
				{
					surface->Frame(frame, FRAME_UP);
				}
			}
			else if (objectProperties->checked && !wnd->IsMouseOn(frame))
			{
				wnd->cursorset = False;

				LiSASetMouseCursor(LiSA_MOUSE_ARROW);

				objectProperties->checked = False;

				if (!objectProperties->clicked)
				{
					surface->Frame(frame, FRAME_DOWN);
				}
			}

			if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
			else					leftButton = VK_LBUTTON;

			GetAsyncKeyState(leftButton);

			if (GetAsyncKeyState(leftButton) == 0 && marking) marking = False;

			if (objectProperties->clicked && markStart != -1 && marking)
			{
				Int	 prevMarkStart = markStart;
				Int	 prevMarkEnd = markEnd;

				for (Int i = 0; i <= objectProperties->text.Length(); i++)
				{
					if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	newpos = frame.left + 3 + GetTextSizeX(objectProperties->text, i, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
					else						newpos = frame.left + 3 + GetTextSizeX(String().FillN('*', i), i, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

					if (i > 0 && wnd->MouseX() < (p1.x + newpos) / 2)
					{
						markEnd = i - 1;

						break;
					}
					else
					{
						p1.x = newpos;

						if (i == nOfChars) markEnd = i;
					}
				}

				Int	 oPromptPos = promptPos;

				if (markEnd < oMarkStart)
				{
					markStart = markEnd;
					markEnd = oMarkStart;
					promptPos = markStart;
				}
				else
				{
					markStart = oMarkStart;
					promptPos = markEnd;
				}

				if (prevMarkStart != markStart || prevMarkEnd != markEnd)
				{
					if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, oPromptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
					else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', oPromptPos), oPromptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

					p1.y = frame.top + 2;
					p2.x = p1.x;
					p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

					surface->Line(p1, p2, Setup::ClientColor);

					MarkText(prevMarkStart, prevMarkEnd);

					if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
					else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

					p2.x = p1.x;

					surface->Line(p1, p2, Setup::TextColor);

					{
						HIMC		 hImc = ImmGetContext(wnd->hwnd);
						COMPOSITIONFORM	 info;

						info.dwStyle = CFS_POINT;
						info.ptCurrentPos.x = p1.x - 3;
						info.ptCurrentPos.y = p1.y - 2;

						ImmSetCompositionWindow(hImc, &info);

						ImmDestroyContext(hImc);
					}

					promptVisible = True;
				}
			}

			break;
		case WM_KILLFOCUS:
		case WM_ACTIVATEAPP:
			if (objectProperties->clicked) Process(SM_LBUTTONDOWN, 0, 0);

			break;
		case WM_KEYDOWN:
			if (!active) break;

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

						if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						surface->Line(p1, p2, Setup::ClientColor);

						if (wParam == VK_LEFT)	promptPos--;
						if (wParam == VK_RIGHT)	promptPos++;
						if (wParam == VK_HOME)	promptPos = 0;
						if (wParam == VK_END)	promptPos = nOfChars;

						if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p2.x = p1.x;

						surface->Line(p1, p2, Setup::TextColor);

						{
							HIMC		 hImc = ImmGetContext(wnd->hwnd);
							COMPOSITIONFORM	 info;

							info.dwStyle = CFS_POINT;
							info.ptCurrentPos.x = p1.x - 3;
							info.ptCurrentPos.y = p1.y - 2;

							ImmSetCompositionWindow(hImc, &info);

							ImmDestroyContext(hImc);
						}

						promptVisible = True;

						break;
					case VK_RETURN:
						if (objectProperties->clicked) Process(SM_LBUTTONDOWN, 0, 1);

						break;
					case VK_BACK:
					case VK_DELETE:
						if (markStart != markEnd)
						{
							DeleteSelectedText();

							break;
						}

						if (promptPos == 0 && wParam == VK_BACK) break;
						if (promptPos == nOfChars && wParam == VK_DELETE) break;

						if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						surface->Line(p1, p2, Setup::ClientColor);

						if (wParam == VK_BACK)
						{
							for (i = 0; i < promptPos - 1; i++)
							{
								newtext[i] = objectProperties->text[i];
							}

							for (i = promptPos - 1; i < nOfChars; i++)
							{
								newtext[i] = objectProperties->text[i + 1];
							}
						}
						else
						{
							for (i = 0; i < promptPos; i++)
							{
								newtext[i] = objectProperties->text[i];
							}

							for (i = promptPos; i < nOfChars; i++)
							{
								newtext[i] = objectProperties->text[i + 1];
							}
						}

						if (wParam == VK_BACK)
						{
							promptPos--;
						}

						prevPromptPos = promptPos;

						SetText(newtext);

						promptPos = prevPromptPos;

						if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						surface->Line(p1, p2, Setup::TextColor);

						{
							HIMC		 hImc = ImmGetContext(wnd->hwnd);
							COMPOSITIONFORM	 info;

							info.dwStyle = CFS_POINT;
							info.ptCurrentPos.x = p1.x - 3;
							info.ptCurrentPos.y = p1.y - 2;

							ImmSetCompositionWindow(hImc, &info);

							ImmDestroyContext(hImc);
						}

						promptVisible = True;

						break;
					default:
						break;
				}

				retVal = Break;
			}

			break;
		case WM_CHAR:
			if (!active) break;

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
						if (Binary::IsFlagSet(subtype, EDB_NUMERIC) && (insertText.ToInt() == 0 && insertText[0] != '0')) break;

						for (i = 0; i < promptPos; i++)
						{
							newtext[i] = objectProperties->text[i];
						}

						for (i = promptPos; i < promptPos + insertText.Length(); i++)
						{
							newtext[i] = insertText[i - promptPos];
						}

						for (i = promptPos; i <= nOfChars; i++)
						{
							newtext[i + insertText.Length()] = objectProperties->text[i];
						}

						if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						surface->Line(p1, p2, Setup::ClientColor);

						promptPos += insertText.Length();

						prevPromptPos = promptPos;

						SetText(newtext);

						promptPos = prevPromptPos;

						if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						surface->Line(p1, p2, Setup::TextColor);

						{
							HIMC		 hImc = ImmGetContext(wnd->hwnd);
							COMPOSITIONFORM	 info;

							info.dwStyle = CFS_POINT;
							info.ptCurrentPos.x = p1.x - 3;
							info.ptCurrentPos.y = p1.y - 2;

							ImmSetCompositionWindow(hImc, &info);

							ImmDestroyContext(hImc);
						}

						promptVisible = True;
					}
				}

				if (nOfChars == maxSize) break;

				if (wParam >= 32)
				{
					if (markStart != markEnd)
					{
						DeleteSelectedText();
					}

					if (Binary::IsFlagSet(subtype, EDB_NUMERIC) && (wParam < '0' || wParam > '9') && wParam != 45 && wParam != '.') break;

					for (i = 0; i < promptPos; i++)
					{
						newtext[i] = objectProperties->text[i];
					}

					newtext[(int) promptPos] = wParam;

					for (i = promptPos; i <= nOfChars; i++)
					{
						newtext[i+1] = objectProperties->text[i];
					}

					if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
					else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

					p1.y = frame.top + 2;
					p2.x = p1.x;
					p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

					surface->Line(p1, p2, Setup::ClientColor);

					promptPos++;

					prevPromptPos = promptPos;

					SetText(newtext);

					promptPos = prevPromptPos;

					if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
					else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

					p1.y = frame.top + 2;
					p2.x = p1.x;
					p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

					surface->Line(p1, p2, Setup::TextColor);

					{
						HIMC		 hImc = ImmGetContext(wnd->hwnd);
						COMPOSITIONFORM	 info;

						info.dwStyle = CFS_POINT;
						info.ptCurrentPos.x = p1.x - 3;
						info.ptCurrentPos.y = p1.y - 2;

						ImmSetCompositionWindow(hImc, &info);

						ImmDestroyContext(hImc);
					}

					promptVisible = True;
				}
			}

			break;
	}

	return retVal;
}

S::Void S::GUI::EditBox::MarkText(Int prevMarkStart, Int prevMarkEnd)
{
	if (prevMarkStart == markStart && prevMarkEnd == markEnd) return;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	Int	 bColor = GetSysColor(COLOR_HIGHLIGHT);
	Int	 tColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
	String	 mText;

	if (prevMarkStart != prevMarkEnd && prevMarkStart >= 0 && prevMarkEnd >= 0)
	{
		for (int j = prevMarkStart; j < prevMarkEnd; j++) mText[j - prevMarkStart] = objectProperties->text[j];

		frame.left = realPos.x + GetTextSizeX(objectProperties->text, prevMarkStart, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight) + 4;
		frame.top = realPos.y + 2;
		frame.right = frame.left + GetTextSizeX(mText, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
		frame.bottom = realPos.y + objectProperties->size.cy - 2;

		surface->Box(frame, Setup::ClientColor, FILLED);

		frame.left--;
		frame.top++;

		surface->SetText(mText, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
	}

	mText = "";

	if (markStart != markEnd && markStart >= 0 && markEnd >= 0)
	{
		for (int j = markStart; j < markEnd; j++) mText[j - markStart] = objectProperties->text[j];

		frame.left = realPos.x + GetTextSizeX(objectProperties->text, markStart, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight) + 4;
		frame.top = realPos.y + 2;
		frame.right = frame.left + GetTextSizeX(mText, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
		frame.bottom = realPos.y + objectProperties->size.cy - 2;

		surface->Box(frame, bColor, FILLED);

		frame.left--;
		frame.top++;

		surface->SetText(mText, frame, objectProperties->font, objectProperties->fontSize, tColor, objectProperties->fontWeight);
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

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Int	 i = 0;
	Int	 nOfChars = objectProperties->text.Length();
	Point	 p1;
	Point	 p2;
	String	 newtext;
	Int	 prevPromptPos;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	for (i = 0; i < bMarkStart; i++)
	{
		newtext[i] = objectProperties->text[i];
	}

	for (i = bMarkEnd; i <= nOfChars; i++)
	{
		newtext[i - (bMarkEnd - bMarkStart)] = objectProperties->text[i];
	}



	if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
	else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

	p1.y = frame.top + 2;
	p2.x = p1.x;
	p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

	surface->Line(p1, p2, Setup::ClientColor);

	promptPos = bMarkStart;

	prevPromptPos = promptPos;

	SetText(newtext);

	promptPos = prevPromptPos;

	if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
	else						p1.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

	p1.y = frame.top + 2;
	p2.x = p1.x;
	p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

	surface->Line(p1, p2, Setup::TextColor);

	{
		HIMC		 hImc = ImmGetContext(wnd->hwnd);
		COMPOSITIONFORM	 info;

		info.dwStyle = CFS_POINT;
		info.ptCurrentPos.x = p1.x - 3;
		info.ptCurrentPos.y = p1.y - 2;

		ImmSetCompositionWindow(hImc, &info);

		ImmDestroyContext(hImc);
	}

	promptVisible = True;
}

S::Int S::GUI::EditBox::Deactivate()
{
	if (!active) return Success;

	if (!registered)
	{
		active = False;

		return Success;
	}

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

	return Widget::Deactivate();
}

S::Int S::GUI::EditBox::SetText(String txt)
{
	objectProperties->text	= txt;
	promptPos		= 0;

	if (registered && visible)
	{
		Paint(SP_PAINT);

		onClick.Emit();
	}

	return Success;
}

S::Void S::GUI::EditBox::TimerProc()
{
	if (!registered)		return;
	if (!active || !visible)	return;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 lineStart;
	Point	 lineEnd;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (!Binary::IsFlagSet(subtype, EDB_ASTERISK))	lineStart.x = frame.left + 3 + GetTextSizeX(objectProperties->text, promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
	else						lineStart.x = frame.left + 3 + GetTextSizeX(String().FillN('*', promptPos), promptPos, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

	lineStart.y = frame.top + 2;
	lineEnd.x = lineStart.x;
	lineEnd.y = lineStart.y + METRIC_EDITBOXLINEHEIGHT;

	if (promptVisible)	surface->Line(lineStart, lineEnd, Setup::ClientColor);
	else			surface->Line(lineStart, lineEnd, Setup::TextColor);

	promptVisible = !promptVisible;
}
