 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

const S::Int	 S::GUI::MultiEdit::classID = S::Object::RequestClassID();

S::GUI::MultiEdit::MultiEdit(const String &iText, const Point &iPos, const Size &iSize, Int maxSize) : Widget(iPos, iSize)
{
	type		= classID;

	scrollbar	= NIL;
	scrollbarPos	= 0;

	font.SetColor(Setup::ClientTextColor);

	if (GetWidth() == 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(19);

	cursor = new Cursor(Point(3, 2), GetSize() - Size(6, 4));
	cursor->SetMaxSize(maxSize);
	cursor->SetBackgroundColor(Color(255, 255, 255));
	cursor->SetFont(font);
	cursor->SetText(iText);

	RegisterObject(cursor);

	onChangeSize.Connect(&MultiEdit::OnChangeSize, this);
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

	EnterProtectedRegion();

	Surface	*surface	= container->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (scrollbar != NIL) frame.right -= 17;

			if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
			else		surface->Box(frame, Setup::BackgroundColor, FILLED);

			if (scrollbar != NIL) frame.right += 17;

			surface->Frame(frame, FRAME_DOWN);

			if ((font.GetTextSizeY(text) > GetHeight() - 6) && (scrollbar == NIL))
			{
				Layer	*layer = (Layer *) container;

				scrollbarPos = 0;

				scrollbar = new Scrollbar(Point(frame.right - layer->GetX() - 18, frame.top - layer->GetY() + 1), Size(0, GetHeight() - 2), OR_VERT, &scrollbarPos, 0, GetNOfInvisibleLines());
				scrollbar->onValueChange.Connect(&MultiEdit::ScrollbarProc, this);

				container->RegisterObject(scrollbar);

				scrollbar->Paint(SP_PAINT);
			}
			else if ((font.GetTextSizeY(text) > GetHeight() - 6) && (scrollbar != NIL))
			{
				Layer	*layer = (Layer *) container;

				scrollbar->SetPosition(Point(frame.right - layer->GetX() - 18, frame.top - layer->GetY() + 1));
				scrollbar->SetHeight(GetHeight() - 2);

				scrollbar->SetRange(0, GetNOfInvisibleLines());

				scrollbar->Paint(SP_PAINT);
			}
			else if ((font.GetTextSizeY(text) <= GetHeight() - 6) && (scrollbar != NIL))
			{
				container->UnregisterObject(scrollbar);

				surface->Box(Rect(Point(frame.right - 18, frame.top + 1), Size(18, GetHeight() - 2)), Setup::ClientColor, FILLED);

				DeleteObject(scrollbar);

				scrollbar = NIL;
				scrollbarPos = 0;
			}

			break;
	}

	LeaveProtectedRegion();

	return Widget::Paint(message);
}

S::Int S::GUI::MultiEdit::MarkAll()
{
	return cursor->MarkAll();
}

S::Int S::GUI::MultiEdit::Activate()
{
	cursor->SetBackgroundColor(Setup::ClientColor);

	return Widget::Activate();
}

S::Int S::GUI::MultiEdit::Deactivate()
{
	cursor->SetBackgroundColor(Setup::BackgroundColor);

	return Widget::Deactivate();
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
	return 1 + GetNOfLines() - Math::Floor((GetHeight() - 6) / (font.GetLineSizeY(text) + 1));
}

S::Int S::GUI::MultiEdit::SetText(const String &newText)
{
	scrollbarPos = 0;

	cursor->SetText(newText);

	return Paint(SP_PAINT);
}

const S::String &S::GUI::MultiEdit::GetText()
{
	return cursor->GetText();
}

S::Void S::GUI::MultiEdit::ScrollbarProc()
{
	Paint(SP_PAINT);
}

S::Int S::GUI::MultiEdit::GetCursorPos()
{
	return cursor->GetCursorPos();
}

S::Void S::GUI::MultiEdit::OnChangeSize(const Size &nSize)
{
	cursor->SetSize(nSize - Size(6, 4));
}
