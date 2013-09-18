 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/multiedit.h>
#include <smooth/gui/widgets/special/cursor.h>
#include <smooth/gui/widgets/basic/scrollbar.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/misc/binary.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Short	 S::GUI::MultiEdit::classID = S::Object::RequestClassID();

S::GUI::MultiEdit::MultiEdit(const String &iText, const Point &iPos, const Size &iSize, Int maxSize) : Widget(iPos, iSize)
{
	type		= classID;

	scrollbar	= NIL;
	scrollbarPos	= 0;

	font.SetColor(Setup::ClientTextColor);

	if (GetWidth() == 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(19);

	cursor = new Cursor(Point(3, 2), GetSize() - Size(6, 4));
	cursor->onScroll.Connect(&MultiEdit::OnCursorScroll, this);
	cursor->SetMaxSize(maxSize);
	cursor->SetBackgroundColor(Color(255, 255, 255));
	cursor->SetFlags(CF_MULTILINE);
	cursor->SetFont(font);
	cursor->SetText(iText);
	cursor->onInput.Connect(&onInput);

	Add(cursor);

	onInput.SetParentObject(this);

	onChangeSize.Connect(&MultiEdit::OnChangeSize, this);
	onLoseFocus.Connect(&cursor->onLoseFocus);
}

S::GUI::MultiEdit::~MultiEdit()
{
	DeleteObject(cursor);

	if (scrollbar != NIL) DeleteObject(scrollbar);
}

S::Int S::GUI::MultiEdit::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

				surface->StartPaint(GetVisibleArea());

				if (IsActive())
				{
					cursor->SetBackgroundColor(Setup::ClientColor);

					surface->Box(frame, Setup::ClientColor, Rect::Filled);
				}
				else
				{
					cursor->SetBackgroundColor(Setup::BackgroundColor);

					surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				}

				surface->Frame(frame, FRAME_DOWN);

				Widget::Paint(message);

				surface->EndPaint();
			}

			return Success();
	}

	return Widget::Paint(message);
}

S::Int S::GUI::MultiEdit::GetNOfLines()
{
	Int	 lines = 1;

	for (Int i = 0; i < text.Length(); i++)
	{
		if (text[i] == '\n') lines++;
	}

	return lines;
}

S::Int S::GUI::MultiEdit::GetNOfInvisibleLines()
{
	static Int	 lineSize = font.GetUnscaledTextSizeY() + 1;

	return 1 + GetNOfLines() - Math::Floor((GetHeight() - 6) / lineSize);
}

S::Int S::GUI::MultiEdit::SetText(const String &newText)
{
	scrollbarPos = 0;

	if (!IsVisible()) return cursor->SetText(newText);

	Surface	*surface = GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

	surface->StartPaint(frame);

	cursor->SetText(newText);

	surface->EndPaint();

	return Success();
}

S::Void S::GUI::MultiEdit::OnScroll()
{
	cursor->Scroll(scrollbarPos);
}

S::Void S::GUI::MultiEdit::OnCursorScroll(Int scrollPos, Int maxScrollPos)
{
	if (maxScrollPos == NIL && scrollbar != NIL)
	{
		DeleteObject(scrollbar);

		scrollbar = NIL;

		cursor->SetWidth(cursor->GetWidth() + 17);

		Paint(SP_PAINT);
	}
	else if (scrollbar == NIL)
	{
		cursor->SetWidth(cursor->GetWidth() - 17);

		scrollbar = new Scrollbar(Point(18, 1), Size(0, GetHeight() - 2), OR_VERT, &scrollbarPos, 0, maxScrollPos);
		scrollbar->onValueChange.Connect(&MultiEdit::OnScroll, this);
		scrollbar->SetOrientation(OR_UPPERRIGHT);
		scrollbar->SetAlwaysActive(True);

		Add(scrollbar);

		Paint(SP_PAINT);
	}
	else
	{
		scrollbar->SetRange(0, maxScrollPos);
		scrollbar->SetValue(scrollPos);
	}
}

S::Void S::GUI::MultiEdit::OnChangeSize(const Size &nSize)
{
	if (scrollbar != NIL) scrollbar->SetHeight(nSize.cy - 2);

	cursor->SetSize(nSize - Size(6 + (scrollbar != NIL ? 17 : 0), 4));
}
