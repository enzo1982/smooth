 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/special/cursor.h>
#include <smooth/gui/widgets/basic/editbox.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/gui/clipboard/clipboard.h>
#include <smooth/graphics/surface.h>
#include <smooth/input/pointer.h>
#include <smooth/i18n/translator.h>
#include <smooth/misc/binary.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>
#include <smooth/system/timer.h>

#include <fribidi.h>

#ifdef __WIN32__
#	include <windows.h>
#	include <imm.h>
#endif

const S::Short	 S::GUI::Cursor::classID = S::Object::RequestClassID();

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
	contextMenu	= NIL;

	SetTabstopCapable(True);

	onGetFocus.Connect(&Cursor::OnGetFocus, this);
	onGetFocusByKeyboard.Connect(&Cursor::OnGetFocusByKeyboard, this);
	onLoseFocus.Connect(&Cursor::OnLoseFocus, this);

	getContextMenu.Connect(&Cursor::GetContextMenu, this);

	onInput.SetParentObject(this);
	onEnter.SetParentObject(this);
	onScroll.SetParentObject(this);
}

S::GUI::Cursor::~Cursor()
{
	if (contextMenu != NIL) DeleteObject(contextMenu);

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
			DrawWidget();

			break;
		case SP_MOUSEIN:
			Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorTextEdit);

			break;
		case SP_MOUSEOUT:
			Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorArrow);

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
				String	 wText = text;
				Int	 wPromptPos = 0;

				if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
				{
					Int	 line = (window->GetMousePosition().y - frame.top) / (font.GetTextSizeY() + 3);
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

				Int	 newPromptPos = GetLogicalCursorPositionFromDisplay(wText, window->GetMousePosition().x - frame.left) + wPromptPos;

				marking = True;

				MarkText(newPromptPos, newPromptPos);

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
		case SM_RBUTTONDOWN:
		case SM_RBUTTONDBLCLK:
			if (mouseOver)
			{
				/* Activate this widget before opening the context menu.
				 */
				if (!focussed)
				{
					window->Process(SM_LBUTTONDOWN, 0, 0);
					window->Process(SM_LBUTTONUP, 0, 0);
				}

				/* Enable the context menu even for inactive widgets.
				 */
				if (!IsActive())
				{
					OpenContextMenu();

					/* Force mouseOut event.
					 */
					window->Process(SM_MOUSEMOVE, 0, 0);
				}
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
				String	 wText = text;
				Int	 wPromptPos = 0;

				if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
				{
					Int	 line = (window->GetMousePosition().y - frame.top) / (font.GetTextSizeY() + 3);
					Int	 lineCount = 0;

					if (window->GetMousePosition().y - frame.top < 0) line--;

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
					Int	 newMarkEnd = GetLogicalCursorPositionFromDisplay(wText, window->GetMousePosition().x - frame.left) + wPromptPos;

					MarkText(markStart, newMarkEnd);

					SetCursorPos(newMarkEnd);
				}
			}

			break;
		case SM_KEYDOWN:
			OnSpecialKey(wParam);

			break;
		case SM_CHAR:
			OnInput(wParam, lParam);

			break;
	}

	return Widget::Process(message, wParam, lParam);
}

S::Int S::GUI::Cursor::DrawWidget()
{
	Int	 nMaxScrollPos = Math::Max(0, (Int) Math::Ceil((Float) (font.GetTextSizeY(text) - GetHeight()) / (font.GetTextSizeY() + 3)));

	if (nMaxScrollPos != maxScrollPos)
	{
		maxScrollPos = nMaxScrollPos;

		if (maxScrollPos == 0) scrollPos = 0;

		onScroll.Emit(scrollPos, maxScrollPos);
	}

	Surface	*surface = GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame	 = Rect(realPos, GetSize());

	surface->StartPaint(GetVisibleArea());

	surface->Box(frame, GetBackgroundColor(), Rect::Filled);

	String	 line;
	Bool	 fillLineIndices = (lineIndices.Length() == 0);
	Int	 lineNumber = (fillLineIndices ? 0 : scrollPos);
	Int	 lineStart = (fillLineIndices ? 0 : lineIndices.GetNth(scrollPos));

	lineIndices.Add(0);

	for (Int i = (fillLineIndices ? 0 : lineIndices.GetNth(scrollPos)); i <= text.Length(); i++)
	{
		/* Check if the line is above the first visible line due to scrolling
		 */
		if (lineNumber < scrollPos)
		{
			if (text[i] == '\n' || text[i] == 0)
			{
				lineStart = i + 1;
				lineNumber++;

				lineIndices.Add(lineStart);
			}

			continue;
		}

		line[i - lineStart] = text[i];

		if (text[i] == '\n' || text[i] == 0)
		{
			line[i - lineStart] = 0;

			if ((lineNumber - scrollPos) * (font.GetTextSizeY() + 3) >= frame.bottom - frame.top && !fillLineIndices) break;

			if (!Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK))	surface->SetText(line, frame + Point(-visibleOffset, 1 + (lineNumber - scrollPos) * (font.GetTextSizeY() + 3)) + Size(visibleOffset, -2), font);
			else								surface->SetText(String().FillN('*', line.Length()), frame + Point(-visibleOffset, 1 + (lineNumber - scrollPos) * (font.GetTextSizeY() + 3)) + Size(visibleOffset, -2), font);

			if (markStart != markEnd && markStart >= 0 && markEnd >= 0)
			{
				Int	 lineMarkStart = Math::Max(0, Math::Min(markStart, markEnd) - lineStart);
				Int	 lineMarkEnd = Math::Min(line.Length(), Math::Max(markStart, markEnd) - lineStart);

				if (lineMarkStart < line.Length() && lineMarkEnd > 0)
				{
#ifdef __WIN32__
					Int	 bColor = GetSysColor(COLOR_HIGHLIGHT);
					Int	 tColor	= GetSysColor(COLOR_HIGHLIGHTTEXT);
#else
					Int	 bColor	= Color(55, 115, 215);
					Int	 tColor = Color(255, 255, 255);
#endif
					Array<Int>	 markRegionStarts;
					Array<Int>	 markRegionEnds;

					if (Setup::useIconv && ContainsRTLCharacters(line))
					{
						FriBidiStrIndex	 length = line.Length();

						/* Get visual positions.
						 */
						FriBidiStrIndex	*positions = new FriBidiStrIndex [length + 1];
						FriBidiParType	 type = (IsRightToLeft() ? FRIBIDI_PAR_RTL : FRIBIDI_PAR_LTR);

						fribidi_log2vis((FriBidiChar *) line.ConvertTo("UCS-4LE"), length, &type, NIL, positions, NIL, NIL);

						/* Find marked regions in visual string.
						 */
						for (Int i = lineMarkStart; i < lineMarkEnd; i++)
						{
							Bool	 done = False;

							for (Int j = 0; j < markRegionStarts.Length(); j++)
							{
								if	(markRegionStarts.GetNth(j) == positions[i] + 1) { markRegionStarts.SetNth(j, markRegionStarts.GetNth(j) - 1); done = True; break; }
								else if (markRegionEnds.GetNth(j)   == positions[i]    ) { markRegionEnds.SetNth(j, markRegionEnds.GetNth(j) + 1);     done = True; break; }
							}

							if (!done)
							{
								markRegionStarts.Add(positions[i]);
								markRegionEnds.Add(positions[i] + 1);
							}
						}

						delete [] positions;

						/* Consolidate adjacent regions into one.
						 */
						for (Int i = 0; i < markRegionStarts.Length() - 1; i++)
						{
							for (Int j = i + 1; j < markRegionStarts.Length(); j++)
							{
								if (markRegionStarts.GetNth(i) < markRegionStarts.GetNth(j) &&
								    markRegionEnds.GetNth(i) >= markRegionStarts.GetNth(j))
								{
									markRegionEnds.SetNth(i, markRegionEnds.GetNth(j));

									markRegionStarts.RemoveNth(j);
									markRegionEnds.RemoveNth(j);

									j--;
								}
								else if (markRegionStarts.GetNth(j) < markRegionStarts.GetNth(i) &&
								    	 markRegionEnds.GetNth(j) >= markRegionStarts.GetNth(i))
								{
									markRegionStarts.SetNth(i, markRegionStarts.GetNth(j));

									markRegionStarts.RemoveNth(j);
									markRegionEnds.RemoveNth(j);

									j--;
								}
							}
						}
					}
					else
					{
						markRegionStarts.Add(lineMarkStart);
						markRegionEnds.Add(lineMarkEnd);
					}

					/* Draw marked regions.
					 */
					for (Int i = 0; i < markRegionStarts.Length(); i++)
					{
						Int	 markRegionStart = GetDisplayCursorPositionFromVisual(line, markRegionStarts.GetNth(i));
						Int	 markRegionEnd	 = GetDisplayCursorPositionFromVisual(line, markRegionEnds.GetNth(i));

						Rect	 markRect = Rect(realPos + Point(markRegionStart, (lineNumber - scrollPos) * (font.GetTextSizeY() + 3)), Size(markRegionEnd - markRegionStart, font.GetTextSizeY() + 3));
						Font	 nFont = font;

						nFont.SetColor(tColor);

						surface->StartPaint(markRect);

						surface->Box(markRect, bColor, Rect::Filled);

						if (!Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK))	surface->SetText(line, frame + Point(-visibleOffset, 1 + (lineNumber - scrollPos) * (font.GetTextSizeY() + 3)) + Size(visibleOffset, -2), nFont);
						else								surface->SetText(String().FillN('*', line.Length()), frame + Point(-visibleOffset, 1 + (lineNumber - scrollPos) * (font.GetTextSizeY() + 3)) + Size(visibleOffset, -2), nFont);

						surface->EndPaint();
					}
				}
			}

			lineStart = i + 1;
			lineNumber++;

			lineIndices.Add(lineStart);
		}
	}

	surface->EndPaint();

	return Success();
}

S::Void S::GUI::Cursor::ShowCursor(Bool visible)
{
	if (promptVisible == visible) return;

	Surface	*surface = GetDrawSurface();
	Point	 point	 = GetRealPosition();

	Int	 line = 0;

	if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
	{
		for (Int j = promptPos - 1; j >= 0; j--)
		{
			if (text[j] == '\n') line++;
		}
	}

	point.x += GetDisplayCursorPositionFromLogical(promptPos);
	point.y += (font.GetTextSizeY() + 3) * (line - scrollPos);

	if (!(line - scrollPos < 0 || (font.GetTextSizeY() + 3) * (line - scrollPos + 1) > GetHeight()))
	{
		if (IsVisible()) surface->Box(Rect(point, Size(1, font.GetTextSizeY() + 3)), 0, Rect::Inverted);

		promptVisible = visible;
	}
}

S::Int S::GUI::Cursor::SetText(const String &newText)
{
	if (text == newText) return Success();

	lineIndices.RemoveAll();

	OnLoseFocus();

	promptPos	= 0;
	visibleOffset	= 0;
	scrollPos	= 0;

	return Widget::SetText(newText);
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
	MarkText(0, text.Length());

	SetCursorPos(text.Length());

	return Success();
}

S::Void S::GUI::Cursor::InsertText(const String &insertText)
{
	ShowCursor(False);

	String	 newText;

	for (Int i = 0;		i < promptPos;			     i++) newText[i] = text[i];
	for (Int j = promptPos; j < promptPos + insertText.Length(); j++) newText[j] = insertText[j - promptPos];
	for (Int k = promptPos; k <= text.Length();		     k++) newText[k + insertText.Length()] = text[k];

	Bool	 prevVisible = IsVisible();

	visible = False;

	lineIndices.RemoveAll();

	Widget::SetText(newText);

	visible = prevVisible;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(Rect(container->GetRealPosition(), container->GetSize()));

	container->Paint(SP_PAINT);

	surface->EndPaint();

	SetCursorPos(promptPos + insertText.Length());

	onInput.Emit(newText);
}

S::Void S::GUI::Cursor::CopyToClipboard()
{
	if (markStart != markEnd)
	{
		String	 mText;

		for (int j = Math::Min(markStart, markEnd); j < Math::Max(markStart, markEnd); j++) mText[j - Math::Min(markStart, markEnd)] = text[j];

		Clipboard(container->GetContainerWindow()).SetClipboardText(mText);
	}
}

S::Void S::GUI::Cursor::InsertFromClipboard()
{
	DeleteSelectedText();

	String	 insertText = Clipboard(container->GetContainerWindow()).GetClipboardText();

	if (insertText.Length() > 0 && (insertText.Length() + text.Length()) <= maxSize)
	{
		if (Binary::IsFlagSet(container->GetFlags(), EDB_NUMERIC) && (insertText.ToInt() == 0 && insertText[0] != '0')) return;

		InsertText(insertText);
	}
}

S::Void S::GUI::Cursor::DeleteSelectedText()
{
	if (markStart == markEnd || markStart < 0 || markEnd < 0) return;

	ShowCursor(False);

	Int	 bMarkStart	= Math::Min(markStart, markEnd);
	Int	 bMarkEnd	= Math::Max(markStart, markEnd);

	Bool	 prevVisible = IsVisible();

	visible = False;

	MarkText(-1, -1);

	String	 newText;

	for (Int i = 0; i < bMarkStart; i++)		newText[i] = text[i];
	for (Int j = bMarkEnd; j <= text.Length(); j++)	newText[j - (bMarkEnd - bMarkStart)] = text[j];

	lineIndices.RemoveAll();

	Widget::SetText(newText);

	visible = prevVisible;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(Rect(container->GetRealPosition(), container->GetSize()));

	container->Paint(SP_PAINT);

	surface->EndPaint();

	SetCursorPos(bMarkStart);

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
	if (!IsVisible())
	{
		onLoseFocus.Emit();

		return;
	}

	ShowCursor(!promptVisible);
}

S::Void S::GUI::Cursor::OnGetFocus()
{
	SetCursorPos(promptPos);
}

S::Void S::GUI::Cursor::OnGetFocusByKeyboard()
{
	MarkAll();
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

S::Void S::GUI::Cursor::OnSpecialKey(Int keyCode)
{
	/* Called when a special key such as
	 * return or an arrow key is hit.
	 */
	if (!focussed) return;

	Int	 newPos = 0;
	Int	 linePos = 0;

	Int	 i = 0;

	switch (keyCode)
	{
		case SK_LEFT:
		case SK_RIGHT:
		case SK_HOME:
		case SK_END:
			MarkText(-1, -1);

			{
				Bool	 isLeft	 = ((keyCode == SK_LEFT  && !IsRightToLeft()) || (keyCode == SK_RIGHT && IsRightToLeft()));
				Bool	 isRight = ((keyCode == SK_RIGHT && !IsRightToLeft()) || (keyCode == SK_LEFT  && IsRightToLeft()));

				if	(isLeft)		{ if (promptPos == 0)		  break; newPos = promptPos - 1; }
				else if (isRight)		{ if (promptPos >= text.Length()) break; newPos = promptPos + 1; }
				else if (keyCode == SK_HOME)	{ if (promptPos == 0)		  break; newPos = 0;		 }
				else if (keyCode == SK_END)	{ if (promptPos >= text.Length()) break; newPos = text.Length(); }
			}

			SetCursorPos(newPos);

			break;
		case SK_UP:
		case SK_DOWN:
			if (!Binary::IsFlagSet(GetFlags(), CF_MULTILINE)) break;

			linePos = promptPos;
			newPos = promptPos;

			for (i = promptPos - 1; i >= 0; i--)
			{
				if (text[i] == '\n') { linePos -= (i + 1); break; }
			}

			if (keyCode == SK_UP)
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

			break;
		case SK_RETURN:
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
		case SK_BACK:
		case SK_DELETE:
			if (!IsActive()) break;

			if (markStart != markEnd) { DeleteSelectedText(); break; }

			if (promptPos == 0	       && keyCode == SK_BACK)	break;
			if (promptPos == text.Length() && keyCode == SK_DELETE)	break;

			if (keyCode == SK_BACK)
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

S::Void S::GUI::Cursor::OnInput(Int character, Int flags)
{
	/* Called when a character is entered.
	 */
	if (!focussed) return;

	/* CTRL + C
	 */
	if (character == 0x03 && !(flags & (1 << 30)))
	{
		CopyToClipboard();
	}

	/* CTRL + X
	 */
	if (character == 0x18 && !(flags & (1 << 30)) && IsActive())
	{
		CopyToClipboard();

		DeleteSelectedText();
	}

	/* CTRL + V
	 */
	if (character == 0x16 && IsActive())
	{
		InsertFromClipboard();
	}

	if (text.Length() == maxSize && markStart == markEnd) return;

	if (character >= 0x20 && character != 0x7F && IsActive())
	{
		DeleteSelectedText();

		if (Binary::IsFlagSet(container->GetFlags(), EDB_NUMERIC) && (character < '0' || character > '9') && character != '-' && character != '.') return;

		String	 insertText;

		if (Setup::enableUnicode)
		{
			insertText[0] = character;
		}
		else
		{
			/* Non Unicode Windows puts ANSI character codes into wParam.
			 * Import these using the current character input format.
			 */
			char  ansiText[2] = { character, 0 };

			insertText = ansiText;
		}

		InsertText(insertText);
	}
}

S::Void S::GUI::Cursor::OnCut()
{
	CopyToClipboard();
	DeleteSelectedText();
}

S::Void S::GUI::Cursor::OnInsert()
{
	InsertFromClipboard();
}

S::GUI::PopupMenu *S::GUI::Cursor::GetContextMenu()
{
	if (!IsActive() && text == NIL) return NIL;

	if (contextMenu == NIL) contextMenu = new PopupMenu();

	contextMenu->RemoveAllEntries();

	MenuEntry	*entryCut     = contextMenu->AddEntry(I18n::Translator::defaultTranslator->TranslateString("Cut"));
	MenuEntry	*entryCopy    = contextMenu->AddEntry(I18n::Translator::defaultTranslator->TranslateString("Copy"));
	MenuEntry	*entryInsert  = contextMenu->AddEntry(I18n::Translator::defaultTranslator->TranslateString("Paste"));
	MenuEntry	*entryDelete  = contextMenu->AddEntry(I18n::Translator::defaultTranslator->TranslateString("Clear"));

	contextMenu->AddEntry();

	MenuEntry	*entryMarkAll = contextMenu->AddEntry(I18n::Translator::defaultTranslator->TranslateString("Select all"));

	entryCut->onAction.Connect(&Cursor::OnCut, this);
	entryCopy->onAction.Connect(&Cursor::CopyToClipboard, this);
	entryInsert->onAction.Connect(&Cursor::OnInsert, this);
	entryDelete->onAction.Connect(&Cursor::DeleteSelectedText, this);

	entryMarkAll->onAction.Connect(&Cursor::MarkAll, this);

	/* Disable modification if inactive.
	 */
	if (!IsActive())
	{
		entryCut->Deactivate();
		entryInsert->Deactivate();
		entryDelete->Deactivate();
	}

	/* Disable copy and delete if nothing is selected.
	 */
	if (markStart == markEnd)
	{
		entryCut->Deactivate();
		entryCopy->Deactivate();
		entryDelete->Deactivate();
	}

	/* Disable "Select all" if everything is selected already.
	 */
	if ((markStart == 0 && markEnd == text.Length()) || text == NIL)
	{
		entryMarkAll->Deactivate();
	}

	return contextMenu;
}

S::Int S::GUI::Cursor::SetCursorPos(Int newPos)
{
	focussed = True;

	ShowCursor(False);

	Rect	 frame	 = Rect(GetRealPosition(), GetSize());
	Point	 p1	 = GetRealPosition();

	promptPos = newPos;

	Int	 line = 0;

	if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
	{
		for (Int j = promptPos - 1; j >= 0; j--)
		{
			if (text[j] == '\n') line++;
		}
	}

	p1.x += GetDisplayCursorPositionFromLogical(promptPos);

	while (p1.x > frame.right || p1.x < frame.left)
	{
		if (p1.x > frame.right)     { visibleOffset +=  5; p1.x -=  5; }
		else if (p1.x < frame.left) { visibleOffset -= 20; p1.x += 20; }
	}

	if (visibleOffset < 0) { p1.x -= visibleOffset; visibleOffset = 0; }

	p1.y += (font.GetTextSizeY() + 3) * (line - scrollPos);

	if (line - scrollPos < 0 || (font.GetTextSizeY() + 3) * (line - scrollPos + 1) > GetHeight())
	{
		while (line - scrollPos < 0)						 scrollPos--;
		while ((font.GetTextSizeY() + 3) * (line - scrollPos + 1) > GetHeight()) scrollPos++;

		onScroll.Emit(scrollPos, maxScrollPos);

		System::System::Sleep(100);

		return Success();
	}

#ifdef __WIN32__
	{
		Window	*window	 = GetContainerWindow();
		Surface	*surface = GetDrawSurface();

		HWND	 hwnd	 = (HWND) surface->GetSystemSurface();
		HDC	 dc	 = GetWindowDC(hwnd);

		HIMC		 hImc = ImmGetContext((HWND) window->GetSystemWindow());
		COMPOSITIONFORM	 info;

		info.dwStyle = CFS_POINT;
		info.ptCurrentPos.x = p1.x - 3;
		info.ptCurrentPos.y = p1.y - 2;

		ImmSetCompositionWindow(hImc, &info);

		if (Setup::enableUnicode)
		{
			LOGFONTW	 lFont;

			lFont.lfHeight		= -Math::Round(font.GetSize() * 128.0 / GetDeviceCaps(dc, LOGPIXELSY));
			lFont.lfWidth		= 0;
			lFont.lfEscapement	= 0;
			lFont.lfOrientation	= 0;
			lFont.lfWeight		= (font.GetWeight() == Font::Bold) ? FW_BOLD : FW_NORMAL;
			lFont.lfItalic		= false;
			lFont.lfUnderline	= false;
			lFont.lfStrikeOut	= false;
			lFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
			lFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
			lFont.lfCharSet		= DEFAULT_CHARSET;
			lFont.lfQuality		= DEFAULT_QUALITY;
			lFont.lfPitchAndFamily	= DEFAULT_PITCH | FF_ROMAN;

			memcpy(lFont.lfFaceName, (wchar_t *) font.GetName(), sizeof(wchar_t) * (font.GetName().Length() + 1));

			ImmSetCompositionFontW(hImc, &lFont);
		}
		else
		{
			LOGFONTA	 lFont;

			lFont.lfHeight		= -Math::Round(font.GetSize() * 128.0 / GetDeviceCaps(dc, LOGPIXELSY));
			lFont.lfWidth		= 0;
			lFont.lfEscapement	= 0;
			lFont.lfOrientation	= 0;
			lFont.lfWeight		= (font.GetWeight() == Font::Bold) ? FW_BOLD : FW_NORMAL;
			lFont.lfItalic		= false;
			lFont.lfUnderline	= false;
			lFont.lfStrikeOut	= false;
			lFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
			lFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;

			lFont.lfCharSet		= DEFAULT_CHARSET;
			lFont.lfQuality		= DEFAULT_QUALITY;
			lFont.lfPitchAndFamily	= DEFAULT_PITCH | FF_ROMAN;

			strcpy(lFont.lfFaceName, font.GetName());

			ImmSetCompositionFontA(hImc, &lFont);
		}

		ImmDestroyContext(hImc);

		ReleaseDC(hwnd, dc);
	}
#endif

	if (timer != NIL) DeleteObject(timer);

	timer = new System::Timer();

	timer->onInterval.Connect(&Cursor::OnTimer, this);
	timer->Start(500);

	Paint(SP_PAINT);

	ShowCursor(True);

	return Success();
}

S::Int S::GUI::Cursor::GetCursorPos() const
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

S::Int S::GUI::Cursor::GetMaxSize() const
{
	return maxSize;
}

/* Returns the display cursor position
 * for a given logical cursor position.
 */
S::Int S::GUI::Cursor::GetDisplayCursorPositionFromLogical(Int promptPos) const
{
	String	 wText = text;
	Int	 wPromptPos = promptPos;

	if (Binary::IsFlagSet(GetFlags(), CF_MULTILINE))
	{
		Int	 lineStart  = promptPos;
		Int	 lineLength = 0;

		for (Int i = promptPos - 1; i >= 0;		i--) { if (text[i] == '\n')		    break; lineStart--;	 }
		for (Int i = lineStart;	    i <  text.Length();	i++) { if (text[i] == '\n' || text[i] == 0) break; lineLength++; }

		wText = text.SubString(lineStart, lineLength);
		wPromptPos = promptPos - lineStart;
	}

	return GetDisplayCursorPositionFromLogical(wText, wPromptPos);
}

/* Returns the display cursor position
 * for a given logical cursor position.
 */
S::Int S::GUI::Cursor::GetDisplayCursorPositionFromLogical(const String &line, Int promptPos) const
{
	return GetDisplayCursorPositionFromVisual(line, GetVisualCursorPositionFromLogical(line, promptPos));
}

/* Returns the display cursor position
 * for a given visual cursor position.
 */
S::Int S::GUI::Cursor::GetDisplayCursorPositionFromVisual(const String &line, Int promptPos) const
{
	FriBidiStrIndex	 length = line.Length();

	if (length == 0) return 0;

	Int		 position = 0;

	if (!Binary::IsFlagSet(container->GetFlags(), EDB_ASTERISK))
	{
		String	 vText = line;

		if (Setup::useIconv && ContainsRTLCharacters(line))
		{
			FriBidiChar	*visual = new FriBidiChar [length + 1];
			FriBidiParType	 type = (IsRightToLeft() ? FRIBIDI_PAR_RTL : FRIBIDI_PAR_LTR);

			fribidi_log2vis((FriBidiChar *) vText.ConvertTo("UCS-4LE"), length, &type, visual, NIL, NIL, NIL);

			visual[length] = 0;

			vText.ImportFrom("UCS-4LE", (char *) visual);

			delete [] visual;
		}

		if (!IsRightToLeft())	position += font.GetTextSizeX(vText.Head(promptPos)) - visibleOffset;
		else			position += font.GetTextSizeX(vText.Tail(length - promptPos)) - visibleOffset;
	}
	else
	{
		position += font.GetTextSizeX(String().FillN('*', promptPos)) - visibleOffset;
	}

	return position;
}

/* Return the logical cursor position
 * for a given display cursor position.
 */
S::Int S::GUI::Cursor::GetLogicalCursorPositionFromDisplay(const String &line, Int displayPos) const
{
	FriBidiStrIndex	 length = line.Length();

	if (length == 0) return 0;

	Int		 bestPos = 0;
	Int		 bestPosValue = 100000;

	for (Int i = 0; i <= length; i++)
	{
		Int	 pos = GetDisplayCursorPositionFromLogical(line, i);

		if (Math::Abs(pos - displayPos) < bestPosValue)
		{
			bestPos = i;
			bestPosValue = Math::Abs(pos - displayPos);

			if (bestPosValue == 0) return i;
		}
	}

	return bestPos;
}

/* Returns the cursor position in the visual string
 * for a given logical cursor position.
 */
S::Int S::GUI::Cursor::GetVisualCursorPositionFromLogical(const String &line, Int n) const
{
	FriBidiStrIndex	 length = line.Length();

	if (length == 0) return 0;

	Int		 position = n;

	/* Check if the input string contains RTL characters.
	 */
	if (Setup::useIconv && ContainsRTLCharacters(line))
	{
		/* Get BiDi types for input string.
		 */
		FriBidiCharType	*types = new FriBidiCharType [length + 1];

		fribidi_get_bidi_types((FriBidiChar *) line.ConvertTo("UCS-4LE"), length, types);

		/* Get visual cursor position.
		 */
		FriBidiStrIndex	*positions = new FriBidiStrIndex [length + 1];
		FriBidiParType	 type = (IsRightToLeft() ? FRIBIDI_PAR_RTL : FRIBIDI_PAR_LTR);

		fribidi_log2vis((FriBidiChar *) line.ConvertTo("UCS-4LE"), length, &type, NIL, positions, NIL, NIL);

		if (n == length)
		{
			if	( FRIBIDI_IS_RTL(types[n - 1])) position = positions[n - 1];
			else					position = positions[n - 1] + 1;
		}
		else if (n == 0)
		{
			if	( FRIBIDI_IS_RTL(types[n    ]))	position = positions[n    ] + 1;
			else					position = positions[n    ];
		}
		else
		{
			if	( FRIBIDI_IS_RTL(types[n    ]))	position = positions[n    ] + 1;
			else if ( FRIBIDI_IS_RTL(types[n - 1])) position = positions[n - 1];
			else if ( FRIBIDI_IS_SPACE(types[n]) &&
				 !FRIBIDI_IS_RTL(types[n - 1])) position = positions[n - 1] + 1;
			else					position = positions[n    ];
		}

		delete [] types;
		delete [] positions;
	}

	return position;
}

/* Check if the string contains right-to-left characters.
 */
S::Bool S::GUI::Cursor::ContainsRTLCharacters(const String &line) const
{
	FriBidiStrIndex	 length = line.Length();

	if (length == 0) return False;

	Bool		 rtlCharacters = False;

	if (Setup::useIconv)
	{
		/* Get BiDi types for input string.
		 */
		FriBidiCharType	*types = new FriBidiCharType [length + 1];

		fribidi_get_bidi_types((FriBidiChar *) line.ConvertTo("UCS-4LE"), length, types);

		/* Check if the input string contains RTL characters.
		 */
		for (Int i = 0; i < length; i++)
		{
			if (FRIBIDI_IS_RTL(types[i])) { rtlCharacters = True; break; }
		}

		delete [] types;
	}
	else
	{
		for (Int i = 0; i < length; i++)
		{
			if (line[i] >= 0x0590 && line[i] <= 0x07BF) { rtlCharacters = True; break; }
		}
	}

	return rtlCharacters;
}
