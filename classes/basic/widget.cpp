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
#include <smooth/layer.h>

const S::Int	 S::GUI::Widget::classID = S::Object::RequestClassID();

S::GUI::Widget::Widget()
{
	type = classID;

	visible	= False;
	active	= True;

	subtype	= 0;

	objectProperties->font.SetColor(Setup::TextColor);

	onClick.SetParentObject(this);
	onRegister.SetParentObject(this);
	onUnregister.SetParentObject(this);
}

S::GUI::Widget::~Widget()
{
}

S::Void S::GUI::Widget::GetTextSize()
{
	objectProperties->textSize.cx = GetTextSizeX(objectProperties->text, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());
	objectProperties->textSize.cy = GetTextSizeY(objectProperties->text, objectProperties->font.GetName(), objectProperties->font.GetSize(), objectProperties->font.GetWeight());

	objectProperties->tooltipSize.cx = GetTextSizeX(objectProperties->tooltip, I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, FW_NORMAL);
	objectProperties->tooltipSize.cy = GetTextSizeY(objectProperties->tooltip, I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, FW_NORMAL);
}

S::Point S::GUI::Widget::GetRealPosition()
{
	if (!registered) return objectProperties->pos;

	Point	 realPos = objectProperties->pos;

	if (myContainer->GetContainerObject()->GetObjectType() == GUI::Layer::classID)
	{
		Layer	*layer = (Layer *) myContainer->GetContainerObject();
		Point	 layerPos = layer->GetRealPosition();

		realPos.x = layerPos.x + objectProperties->pos.x;
		realPos.y = layerPos.y + objectProperties->pos.y;

		if (objectProperties->orientation == OR_UPPERRIGHT)
		{
			realPos.x = layerPos.x + layer->GetObjectProperties()->size.cx - objectProperties->pos.x;
		}
		else if (objectProperties->orientation == OR_LOWERLEFT)
		{
			realPos.y = layerPos.y + layer->GetObjectProperties()->size.cy - objectProperties->pos.y;
		}
		else if (objectProperties->orientation == OR_LOWERRIGHT)
		{
			realPos.x = layerPos.x + layer->GetObjectProperties()->size.cx - objectProperties->pos.x;
			realPos.y = layerPos.y + layer->GetObjectProperties()->size.cy - objectProperties->pos.y;
		}
	}
	else
	{
		Object	*container = myContainer->GetContainerObject();

		if (objectProperties->orientation == OR_UPPERRIGHT)
		{
			realPos.x = container->GetObjectProperties()->size.cx - objectProperties->pos.x;
		}
		else if (objectProperties->orientation == OR_LOWERLEFT)
		{
			realPos.y = container->GetObjectProperties()->size.cy - objectProperties->pos.y;
		}
		else if (objectProperties->orientation == OR_LOWERRIGHT)
		{
			realPos.x = container->GetObjectProperties()->size.cx - objectProperties->pos.x;
			realPos.y = container->GetObjectProperties()->size.cy - objectProperties->pos.y;
		}
	}

	return realPos;
}

S::Int S::GUI::Widget::Show()
{
	if (visible) return Success;

	visible = True;

	if (!registered) return Success;

	if (IsVisible()) Paint(SP_SHOW);

	return Success;
}

S::Int S::GUI::Widget::Hide()
{
	if (!visible) return Success;

	Bool	 wasVisible = IsVisible();

	visible = False;

	if (!registered) return Success;

	if (wasVisible)
	{
		Rect	 rect;
		Point	 realPos = GetRealPosition();
		Surface	*surface = myContainer->GetDrawSurface();

		rect.left	= realPos.x;
		rect.top	= realPos.y;
		rect.right	= realPos.x + objectProperties->size.cx;
		rect.bottom	= realPos.y + objectProperties->size.cy;

		surface->Box(rect, Setup::BackgroundColor, FILLED);
	}

	return Success;
}

S::Int S::GUI::Widget::Activate()
{
	if (active) return Success;

	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	active = True;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::Deactivate()
{
	if (!active) return Success;

	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

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
	if (!registered)	return visible;
	if (!visible)		return False;

	Bool	 isVisible = True;
	Object	*container = myContainer->GetContainerObject();

	if (container->GetObjectType() == classID)
	{
		if (!((Widget *) container)->IsVisible()) isVisible = False;
	}

	return isVisible;
}

S::Bool S::GUI::Widget::IsActive()
{
	return active;
}

S::Int S::GUI::Widget::SetText(String newText)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

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

S::Int S::GUI::Widget::SetFont(Font newFont)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	objectProperties->font = newFont;

	GetTextSize();

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::SetOrientation(Int newOrientation)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	objectProperties->orientation = newOrientation;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::SetPosition(Point newPos)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	objectProperties->pos = newPos;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::SetMetrics(Point newPos, Size newSize)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	objectProperties->pos	= newPos;
	objectProperties->size	= newSize;

	if (registered && prevVisible) Show();

	return Success;
}

S::Bool S::GUI::Widget::IsTypeCompatible(Int compType)
{
	if (compType == Object::classID || compType == classID)	return True;
	else							return False;
}
