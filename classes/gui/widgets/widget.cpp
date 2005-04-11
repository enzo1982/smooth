 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/widget.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/rect.h>
#include <smooth/gui/widgets/container.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/gui/window/window.h>
#include <smooth/system/timer.h>

const S::Int	 S::GUI::Widget::classID = S::Object::RequestClassID();

S::GUI::Widget::Widget()
{
	type		= classID;

	registered	= False;
	container	= NIL;

	visible		= False;
	active		= True;

	subtype		= 0;

	pos.x		= 0;
	pos.y		= 0;
	size.cx		= 100;
	size.cy		= 100;

	borderWidth	= 0;

	orientation	= OR_UPPERLEFT;

	text		= NIL;
	tooltipText	= NIL;

	textSize.cx	= 0;
	textSize.cy	= 0;

	checked		= False;
	clicked		= False;

	mouseOver	= False;

	leftButtonDown	= False;
	rightButtonDown	= False;

	tipTimer	= NIL;
	tooltip		= NIL;

	font.SetColor(Setup::TextColor);

	onShow.SetParentObject(this);
	onHide.SetParentObject(this);

	onClick.SetParentObject(this);

	onRegister.SetParentObject(this);
	onUnregister.SetParentObject(this);
}

S::GUI::Widget::~Widget()
{
	DeactivateTooltip();

	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Bool S::GUI::Widget::IsRegistered()
{
	return registered;
}

S::Int S::GUI::Widget::SetContainer(Container *newContainer)
{
	container = newContainer;

	return Success;
}

S::GUI::Container *S::GUI::Widget::GetContainer()
{
	return container;
}

S::Void S::GUI::Widget::SetRegisteredFlag(Bool flag)
{
	registered = flag;
}

S::Void S::GUI::Widget::GetTextSize()
{
	textSize.cx = font.GetTextSizeX(text);
	textSize.cy = font.GetTextSizeY(text);
}

S::GUI::Point S::GUI::Widget::GetRealPosition()
{
	if (!registered) return pos;

	Point	 realPos = pos;

	if (container->GetObjectType() == GUI::Layer::classID)
	{
		Point	 layerPos = container->GetRealPosition();

		realPos.x = layerPos.x + pos.x;
		realPos.y = layerPos.y + pos.y;

		if (orientation == OR_UPPERRIGHT)
		{
			realPos.x = layerPos.x + container->size.cx - pos.x;
		}
		else if (orientation == OR_LOWERLEFT)
		{
			realPos.y = layerPos.y + container->size.cy - pos.y;
		}
		else if (orientation == OR_LOWERRIGHT)
		{
			realPos.x = layerPos.x + container->size.cx - pos.x;
			realPos.y = layerPos.y + container->size.cy - pos.y;
		}
	}
	else
	{
		if (orientation == OR_UPPERRIGHT)
		{
			realPos.x = container->size.cx - pos.x;
		}
		else if (orientation == OR_LOWERLEFT)
		{
			realPos.y = container->size.cy - pos.y;
		}
		else if (orientation == OR_LOWERRIGHT)
		{
			realPos.x = container->size.cx - pos.x;
			realPos.y = container->size.cy - pos.y;
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

	onShow.Emit();

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
		Rect	 rect		= Rect(GetRealPosition(), size);
		Surface	*surface	= container->GetDrawSurface();

		surface->Box(rect, container->GetBackgroundColor() == -1 ? Setup::BackgroundColor : container->GetBackgroundColor(), FILLED);
	}

	onHide.Emit();

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
	if (!registered)	return Failure;
	if (!visible)		return Success;

	return Success;
}

S::Int S::GUI::Widget::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Failure;
	if (!active || !visible)	return Success;

	Window	*window	= container->GetContainerWindow();

	if (window == NIL) return Success;

	EnterProtectedRegion();

	Rect	 frame	= Rect(GetRealPosition() + Point(borderWidth, borderWidth), size - Size(2 * borderWidth, 2 * borderWidth));

	switch (message)
	{
		case SM_MOUSEMOVE:
			if (!mouseOver && window->IsMouseOn(frame))
			{
				mouseOver = True;

				Paint(SP_MOUSEIN);

				onMouseOver.Emit();

				if (tooltipText != NIL)
				{
					tipTimer = new System::Timer();

					tipTimer->onInterval.Connect(&Widget::ActivateTooltip, this);
					tipTimer->Start(500);
				}
			}
			else if (mouseOver && !window->IsMouseOn(frame))
			{
				mouseOver = False;

				leftButtonDown = False;
				leftButtonDown = False;

				Paint(SP_MOUSEOUT);

				onMouseOut.Emit();

				DeactivateTooltip();
			}
			else if (mouseOver && window->IsMouseOn(frame))
			{
				if (tipTimer != NIL && wParam == 0)
				{
					tipTimer->Stop();
					tipTimer->Start(500);
				}
			}

			break;
		case SM_LBUTTONDBLCLK:
			if (mouseOver)
			{
				onLeftButtonDoubleClick.Emit(Point(window->MouseX(), window->MouseY()));
			}
		case SM_LBUTTONDOWN:
			if (mouseOver)
			{
				leftButtonDown = True;

				Paint(SP_MOUSEDOWN);

				onLeftButtonDown.Emit(Point(window->MouseX(), window->MouseY()));

				DeactivateTooltip();
			}

			break;
		case SM_LBUTTONUP:
			if (leftButtonDown)
			{
				leftButtonDown = False;

				Paint(SP_MOUSEUP);

				onLeftButtonUp.Emit(Point(window->MouseX(), window->MouseY()));
				onLeftButtonClick.Emit(Point(window->MouseX(), window->MouseY()));

				onClick.Emit(window->MouseX(), window->MouseY());
			}

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Void S::GUI::Widget::ActivateTooltip()
{
	if (tooltip != NIL) return;

	tipTimer->Stop();

	DeleteObject(tipTimer);

	tipTimer = NIL;

	Window	*window	= container->GetContainerWindow();

	tooltip = new Tooltip();

	tooltip->SetText(tooltipText);
	tooltip->SetMetrics(Point(window->MouseX(), window->MouseY()), Size(0, 0));
	tooltip->SetTimeout(3000);

	window->RegisterObject(tooltip);
}

S::Void S::GUI::Widget::DeactivateTooltip()
{
	if (tipTimer != NIL)
	{
		tipTimer->Stop();

		DeleteObject(tipTimer);

		tipTimer = NIL;
	}

	if (tooltip != NIL)
	{
		tooltip->Hide();

		tooltip->GetContainer()->UnregisterObject(tooltip);

		DeleteObject(tooltip);

		tooltip = NIL;
	}
}

S::Bool S::GUI::Widget::IsVisible()
{
	if (!registered)	return visible;
	if (!visible)		return False;

	Bool	 isVisible = True;

	if (container->GetObjectType() == classID)
	{
		if (!container->IsVisible()) isVisible = False;
	}

	return isVisible;
}

S::Bool S::GUI::Widget::IsActive()
{
	return active;
}

S::Int S::GUI::Widget::SetText(const String &newText)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	text = newText;

	GetTextSize();

	checked = False;
	clicked = False;

	if (registered && prevVisible) Show();

	Process(SM_MOUSEMOVE, 0, 0);

	return Success;
}

S::String S::GUI::Widget::GetText()
{
	return text;
}

S::Int S::GUI::Widget::SetTooltipText(const String &nTooltipText)
{
	tooltipText = nTooltipText;

	return Success;
}

S::String S::GUI::Widget::GetTooltipText()
{
	return tooltipText;
}

S::Int S::GUI::Widget::SetFont(Font nFont)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	font = nFont;

	GetTextSize();

	if (registered && prevVisible) Show();

	return Success;
}

S::GUI::Font S::GUI::Widget::GetFont()
{
	return font;
}

S::Int S::GUI::Widget::SetOrientation(Int nOrientation)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	orientation = nOrientation;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::GetOrientation()
{
	return orientation;
}

S::Int S::GUI::Widget::SetPosition(Point nPos)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	pos	= nPos;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Widget::SetMetrics(Point nPos, Size nSize)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	pos	= nPos;
	size	= nSize;

	if (registered && prevVisible) Show();

	return Success;
}

S::Bool S::GUI::Widget::IsAffected(const Rect &uRect)
{
	Rect	 tRect;
	Point	 realpos = pos;

	if (type == GUI::Layer::classID || type == GUI::Divider::classID) return True;

	if (pos.x == 0 && pos.y == 0 && size.cx == 0 && size.cy == 0) return True;

	if (container != NIL)
	{
		if (container->GetObjectType() == GUI::Layer::classID) realpos = GetRealPosition();
	}

	tRect.left	= realpos.x - 10;
	tRect.top	= realpos.y - 10;
	tRect.right	= realpos.x + size.cx + 10;
	tRect.bottom	= realpos.y + size.cy + 10;

	if (!Rect::DoRectsOverlap(uRect, tRect)) return False;

	return True;
}

S::Bool S::GUI::Widget::IsTypeCompatible(Int compType)
{
	if (compType == Object::classID || compType == classID)	return True;
	else							return False;
}
