 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/widget.h>
#include <smooth/surface.h>
#include <smooth/rect.h>
#include <smooth/container.h>
#include <smooth/objectproperties.h>
#include <smooth/toolkit.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_WIDGET = S::Object::RequestObjectID();

S::GUI::Widget::Widget()
{
	visible	= False;
	active	= True;

	subtype	= 0;
}

S::GUI::Widget::~Widget()
{
}

S::Int S::GUI::Widget::Show()
{
	if (visible)		return Success;

	visible = True;

	if (!registered)	return Success;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::Widget::Hide()
{
	if (!visible)		return Success;

	visible = False;

	if (!registered)	return Success;

	Rect	 rect;
	Point	 realPos = GetRealPosition();
	Surface	*surface = myContainer->GetDrawSurface();

	rect.left	= realPos.x;
	rect.top	= realPos.y;
	rect.right	= realPos.x + objectProperties->size.cx + 1;
	rect.bottom	= realPos.y + objectProperties->size.cy + 1;

	surface->Box(rect, Setup::BackgroundColor, FILLED);

	return Success;
}

S::Int S::GUI::Widget::Activate()
{
	if (active) return Success;

	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	active = True;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::Deactivate()
{
	if (!active) return Success;

	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	active = False;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	return Success;
}

S::Int S::GUI::Widget::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	return Success;
}

S::Bool S::GUI::Widget::IsVisible()
{
	return visible;
}

S::Bool S::GUI::Widget::IsActive()
{
	return active;
}

S::Int S::GUI::Widget::SetText(String newText)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->text = newText;

	GetTextSize();

	objectProperties->checked = False;
	objectProperties->clicked = False;

	if (registered && prevVisible) Show();

	Process(SM_MOUSEMOVE, 0, 0);

	return Success;
}

S::String S::GUI::Widget::GetText()
{
	return objectProperties->text;
}

S::Int S::GUI::Widget::SetTooltip(String newTooltip)
{
	objectProperties->tooltip = newTooltip;

	GetTextSize();

	return Success;
}

S::String S::GUI::Widget::GetTooltip()
{
	return objectProperties->tooltip;
}

S::Int S::GUI::Widget::SetFont(String newFont, Int newFontSize, Int newFontColor, Int newFontWeight)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->font = newFont;
	objectProperties->fontColor = newFontColor;

	HDC	 dc = GetContext(0);

	objectProperties->fontSize = -MulDiv(newFontSize, GetDeviceCaps(dc, LOGPIXELSY), 72);

	FreeContext(0, dc);

	objectProperties->fontWeight = newFontWeight;

	GetTextSize();

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::SetOrientation(Int newOrientation)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->orientation = newOrientation;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::SetPosition(Point newPos)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->pos = newPos;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::SetMetrics(Point newPos, Size newSize)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->pos	= newPos;
	objectProperties->size	= newSize;

	if (registered && prevVisible) Show();

	return Success;
}

S::Bool S::GUI::Widget::IsTypeCompatible(Int compType)
{
	if (compType == OBJ_OBJECT || compType == OBJ_WIDGET)	return True;
	else							return False;
}
