 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

/* TODO: rewrite SMOOTHEditBox from scratch */

#ifndef __OBJSMOOTH_EDITBOX_
#define __OBJSMOOTH_EDITBOX_

#include <smooth/editbox.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/i18n.h>
#include <smooth/binary.h>
#include <smooth/string.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/timer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_EDITBOX = SMOOTH::RequestObjectID();

SMOOTHEditBox::SMOOTHEditBox(SMOOTHString text, SMOOTHPoint pos, SMOOTHSize size, SMOOTHInt subType, SMOOTHInt iMaxSize, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_EDITBOX;
	objectProperties->text		= text;
	isRight				= SMOOTH::False;
	isAsterisk			= SMOOTH::False;
	markStart			= 0;
	markEnd				= 0;
	leftCut				= 0;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	objectProperties->fontColor	= SMOOTH::Setup::ClientTextColor;
	subtype				= subType;
	maxSize				= iMaxSize;
	currLine			= 0;
	nOfLines			= CountLines();
	promptPos			= 0;
	linePromptPos			= 0;
	timer				= NIL;

	visText = NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	for (int i = 0; i < objectProperties->text.Length(); i++)
	{
		if (objectProperties->text[i] == 13 && objectProperties->text[i+1] != 10)	break;
		else if (objectProperties->text[i] == 10 && objectProperties->text[i+1] != 13)	break;

		visText[i] = objectProperties->text[i];
	}

	if (maxSize <= 0) maxSize = 32768;

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(19 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
}

SMOOTHEditBox::~SMOOTHEditBox()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHEditBox::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHSurface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	SMOOTHRect	 frame;
	SMOOTHRect	 textRect;
	SMOOTHPoint	 realPos = GetRealPosition();

	switch (message)
	{
		default:
		case SP_PAINT:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx - 1;
			frame.bottom	= realPos.y + objectProperties->size.cy - 1;

			if (active)	surface->Box(frame, SMOOTH::Setup::ClientColor, FILLED);
			else		surface->Box(frame, SMOOTH::Setup::BackgroundColor, FILLED);

			surface->Frame(frame, FRAME_DOWN);

			textRect.left	= frame.left + 3;
			textRect.top	= frame.top + 3;
			textRect.right	= textRect.left + objectProperties->size.cx - 6;

			if (IsBitSet(subtype, EDB_MULTILINE))	textRect.bottom	= textRect.top + objectProperties->size.cy - 6;
			else					textRect.bottom = textRect.top + 16;

			for (SMOOTHInt i = 0; i < nOfLines; i++)
			{
				if (active)	surface->SetText(GetLine(i), textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
				else		surface->SetText(GetLine(i), textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::TextColor, objectProperties->fontWeight);

				textRect.top += METRIC_EDITBOXLINEHEIGHT;

				if (textRect.top >= textRect.bottom) break;
			}

			break;
	}

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHEditBox::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHInt	 retVal = SMOOTH::Success;
	SMOOTHRect	 frame;
	HDC		 dc;
	int		 nOfChars = objectProperties->text.Length();
	SMOOTHPoint	 p1;
	SMOOTHPoint	 p2;
	SMOOTHString	 newtext;
	SMOOTHString	 lrcaddtext;
	int		 newpos;
	int		 i;

#ifdef __WIN32__
	int		 prevPromptPos;
	int		 prevLinePromptPos;
	int		 prevCurrLine;
#endif

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	switch (message)
	{
		case SM_LOOSEFOCUS:
			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				objectProperties->clicked = SMOOTH::False;
				objectProperties->checked = SMOOTH::False;

				if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
				else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

				p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
				p2.x = p1.x;
				p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

				Line(dc, p1, p2, SMOOTH::Setup::ClientColor, PS_SOLID, 1);

				promptPos = 0;
				linePromptPos = 0;

				if (timer != NIL)
				{
					timer->Stop();

					wnd->UnregisterObject(timer);

					delete timer;

					timer = NIL;
				}

				Process(SM_MOUSEMOVE, 0, 0);
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONDOWN:
			if (!active) break;

			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				objectProperties->clicked = SMOOTH::False;
				objectProperties->checked = SMOOTH::False;

				if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
				else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

				p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
				p2.x = p1.x;
				p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

				Line(dc, p1, p2, SMOOTH::Setup::ClientColor, PS_SOLID, 1);

				promptPos = 0;
				linePromptPos = 0;

				if (timer != NIL)
				{
					timer->Stop();

					wnd->UnregisterObject(timer);

					delete timer;

					timer = NIL;
				}

				if (IsMouseOn(wnd->hwnd, frame, WINDOW) && lParam != 1)
				{
					objectProperties->checked = SMOOTH::True;
					Process(SM_LBUTTONDOWN, 0, 0);
				}
				else
				{
					Process(SM_MOUSEMOVE, 0, 0);
				}
			}
			else if (objectProperties->checked)
			{
				objectProperties->clicked = SMOOTH::True;
				Frame(dc, frame, FRAME_DOWN);

				if (!IsBitSet(subtype, EDB_MULTILINE))
				{
					for (int i = 0; i <= visText.Length(); i++)
					{
						if (!IsBitSet(subtype, EDB_ASTERISK))	newpos = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, i, objectProperties->font,objectProperties->fontSize, objectProperties->fontWeight);
						else					newpos = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', i), i, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						if (i > 0 && MouseX(wnd->hwnd, WINDOW) < (p1.x + newpos) / 2)
						{
							promptPos = i - 1 + leftCut;
							linePromptPos = i - 1 + leftCut;

							break;
						}
						else
						{
							p1.x = newpos;

							if (i == nOfChars)
							{
								promptPos = i + leftCut;
								linePromptPos = i + leftCut;
							}
						}
					}
				}
				else
				{
// insert multiline code here
				}

				if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
				else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

				p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
				p2.x = p1.x;
				p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

				Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

				promptVisible = SMOOTH::True;

				timer = new SMOOTHTimer();

				wnd->RegisterObject(timer);

				timer->SetProc(SMOOTHProc(SMOOTHEditBox, this, TimerProc));
				timer->Start(500);
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			if (!active) break;

			dc = GetContext(wnd);

			if (!objectProperties->checked && IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				wnd->cursorset = SMOOTH::True;

				LiSASetMouseCursor(LiSA_MOUSE_TEXTEDIT);

				objectProperties->checked = SMOOTH::True;

				if (!objectProperties->clicked)
				{
					Frame(dc, frame, FRAME_UP);
				}
			}
			else if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				wnd->cursorset = SMOOTH::False;

				LiSASetMouseCursor(LiSA_MOUSE_ARROW);

				objectProperties->checked = SMOOTH::False;

				if (!objectProperties->clicked)
				{
					Frame(dc, frame, FRAME_DOWN);
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSELEAVE:
			if (!active) break;

			dc = GetContext(wnd);

			if (objectProperties->checked || objectProperties->clicked)
			{
				wnd->cursorset = SMOOTH::False;

				LiSASetMouseCursor(LiSA_MOUSE_ARROW);

				objectProperties->checked = SMOOTH::False;

				if (!objectProperties->clicked) Frame(dc, frame, FRAME_DOWN);
			}

			FreeContext(wnd, dc);

			break;
#ifdef __WIN32__
		case WM_KILLFOCUS:
		case WM_ACTIVATEAPP:
			if (objectProperties->clicked) Process(SM_LBUTTONDOWN, 0, 0);

			break;
		case WM_KEYDOWN:
			if (!active) break;

			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				switch (wParam)
				{
					case VK_LEFT:
						if (promptPos == 0) break;

						if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						Line(dc, p1, p2, SMOOTH::Setup::ClientColor, PS_SOLID, 1);

						if (!IsBitSet(subtype, EDB_MULTILINE))
						{
							promptPos--;
							linePromptPos--;
						}

						if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p2.x = p1.x;

						Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

						promptVisible = SMOOTH::True;

						break;
					case VK_RIGHT:
						if (promptPos >= nOfChars) break;

						if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						Line(dc, p1, p2, SMOOTH::Setup::ClientColor, PS_SOLID, 1);

						if (!IsBitSet(subtype, EDB_MULTILINE))
						{
							promptPos++;
							linePromptPos++;
						}

						if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p2.x = p1.x;

						Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

						promptVisible = SMOOTH::True;

						break;
					case VK_UP:
						if (currLine >= 1) currLine--;

						break;
					case VK_DOWN:
						if (IsBitSet(subtype, EDB_MULTILINE) && (nOfLines > (currLine + 1))) currLine++;

						break;
					case VK_RETURN:
						if (IsBitSet(subtype, EDB_MULTILINE))
						{
// insert multiline code here
						}
						else
						{
							if (objectProperties->clicked) Process(SM_LBUTTONDOWN, 0, 1);
						}

						break;
					case VK_BACK:
					case VK_DELETE:
						if (promptPos == 0 && wParam == VK_BACK) break;
						if (promptPos == nOfChars && wParam == VK_DELETE) break;

						if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						Line(dc, p1, p2, SMOOTH::Setup::ClientColor, PS_SOLID, 1);

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
							linePromptPos--;
						}

						prevPromptPos = promptPos;
						prevLinePromptPos = linePromptPos;
						prevCurrLine = currLine;

						SetText(newtext);

						promptPos = prevPromptPos;
						linePromptPos = prevLinePromptPos;
						currLine = prevCurrLine;

						if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
						else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

						p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
						p2.x = p1.x;
						p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

						Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

						promptVisible = SMOOTH::True;

						break;
					default:
						break;
				}

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case WM_CHAR:
			if (!active) break;

			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				if (nOfChars + leftCut == maxSize) break;

				if (wParam >= 32)
				{
					if (IsBitSet(subtype, EDB_NUMERIC) && (wParam < '0' || wParam > '9') && wParam != 45 && wParam != '.') break;

					for (i = 0; i < promptPos; i++)
					{
						newtext[i] = objectProperties->text[i];
					}

					newtext[(int) promptPos] = wParam;

					for (i = promptPos; i <= nOfChars; i++)
					{
						newtext[i+1] = objectProperties->text[i];
					}

					if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
					else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

					p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
					p2.x = p1.x;
					p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

					Line(dc, p1, p2, SMOOTH::Setup::ClientColor, PS_SOLID, 1);

					promptPos++;
					linePromptPos++;

					prevPromptPos = promptPos;
					prevLinePromptPos = linePromptPos;
					prevCurrLine = currLine;

					SetText(newtext);

					promptPos = prevPromptPos;
					linePromptPos = prevLinePromptPos;
					currLine = prevCurrLine;

					if (!IsBitSet(subtype, EDB_ASTERISK))	p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
					else					p1.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

					p1.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
					p2.x = p1.x;
					p2.y = p1.y + METRIC_EDITBOXLINEHEIGHT;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

					promptVisible = SMOOTH::True;
				}
			}

			FreeContext(wnd, dc);

			break;
#endif
	}

	return retVal;
}

SMOOTHInt SMOOTHEditBox::Deactivate()
{
	if (!active) return SMOOTH::Success;

	if (!registered)
	{
		active = SMOOTH::False;

		return SMOOTH::Success;
	}

	if (objectProperties->clicked)
	{
		if (timer != NIL)
		{
			SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
			SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

			if (wnd == NIL) return SMOOTHObject::Deactivate();
			if (wnd->hwnd == NIL) return SMOOTHObject::Deactivate();

			timer->Stop();

			wnd->UnregisterObject(timer);

			delete timer;

			timer = NIL;
		}

		objectProperties->clicked = SMOOTH::False;
	}

	return SMOOTHObject::Deactivate();
}

SMOOTHInt SMOOTHEditBox::SetText(SMOOTHString txt)
{
	objectProperties->text = txt;

	promptPos		= 0;
	linePromptPos		= 0;
	currLine		= 0;
	nOfLines		= CountLines();

	visText = NIL;

	for (int k = 0; k < objectProperties->text.Length(); k++)
	{
		if (objectProperties->text[k] == 13 && objectProperties->text[k + 1] != 10)		break;
		else if (objectProperties->text[k] == 10 && objectProperties->text[k + 1] != 13)	break;

		visText[k] = objectProperties->text[k];
	}

	if (registered && visible)
	{
		Paint(SP_PAINT);

		SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHEditBox::CountLines()
{
	SMOOTHInt	 retVal = 1;
	SMOOTHString	 currLine;
	SMOOTHInt	 currPos = 0;

	lines.DeleteAll();

	for (int i = 0; i < objectProperties->text.Length(); i++)
	{
		if (objectProperties->text[i] == 13)
		{
			lines.AddEntry(currLine, retVal - 1);

			currLine	= NIL;
			currPos		= 0;
			retVal++;

			if (objectProperties->text[i+1] == 10) i++;
			i++;
		}
		else if (objectProperties->text[i] == 10)
		{
			lines.AddEntry(currLine, retVal - 1);

			currLine	= NIL;
			currPos		= 0;
			retVal++;

			if (objectProperties->text[i+1] == 13) i++;
			i++;
		}

		if (objectProperties->text[i] == 13 || objectProperties->text[i] == 10)	i--;
		else									currLine[(int) currPos++] = objectProperties->text[i];
	}

	return retVal;
}

SMOOTHString SMOOTHEditBox::GetLine(SMOOTHInt number)
{
	if (number >= nOfLines) return NIL;

	if (!IsBitSet(subtype, EDB_ASTERISK))
	{
		if (number == currLine) return visText;

		return lines.GetEntry(number);
	}
	else
	{
		if (number == currLine) return SMOOTHString().FillN('*', visText.Length());

		return SMOOTHString().FillN('*', lines.GetEntry(number).Length());
	}
}

SMOOTHVoid SMOOTHEditBox::TimerProc()
{
	if (!registered)		return;
	if (!active || !visible)	return;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return;
	if (wnd->hwnd == NIL) return;

	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 frame;
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;
	HDC		 dc;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	dc = GetContext(wnd);

	if (!IsBitSet(subtype, EDB_ASTERISK))	lineStart.x = frame.left + 4 + GetTextSizeXNoExtend(dc, visText, linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
	else					lineStart.x = frame.left + 4 + GetTextSizeXNoExtend(dc, SMOOTHString().FillN('*', linePromptPos - leftCut), linePromptPos - leftCut, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

	lineStart.y = frame.top + 2 + currLine * METRIC_EDITBOXLINEHEIGHT;
	lineEnd.x = lineStart.x;
	lineEnd.y = lineStart.y + METRIC_EDITBOXLINEHEIGHT;

	if (promptVisible)	Line(dc, lineStart, lineEnd, SMOOTH::Setup::ClientColor, PS_SOLID, 1);
	else			Line(dc, lineStart, lineEnd, SMOOTH::Setup::TextColor, PS_SOLID, 1);

	promptVisible = !promptVisible;

	FreeContext(wnd, dc);
}

#endif
