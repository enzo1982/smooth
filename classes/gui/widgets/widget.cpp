 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/widget.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/misc/math.h>
#include <smooth/system/timer.h>

const S::Int	 S::GUI::Widget::classID = S::Object::RequestClassID();

S::GUI::Widget::Widget(const Point &iPos, const Size &iSize)
{
	type		= classID;

	registered	= False;
	container	= NIL;

	visible		= True;
	active		= True;
	focussed	= False;

	subtype		= 0;

	pos		= iPos;
	size		= iSize;

	orientation	= OR_UPPERLEFT;

	text		= NIL;
	tooltipText	= NIL;

	textSize.cx	= 0;
	textSize.cy	= 0;

	mouseOver	= False;

	leftButtonDown	= False;
	rightButtonDown	= False;

	mouseDragging	= False;

	alwaysActive	= False;
	tabstopCapable	= False;

	tipTimer	= NIL;
	tooltip		= NIL;

	font.SetColor(Setup::TextColor);

	backgroundColor	= -1;

	nullSurface	= new GUI::Surface();
	drawSurface	= nullSurface;

	hitTest.Connect(&Widget::DefaultHitTest, this);

	onShow.SetParentObject(this);
	onHide.SetParentObject(this);

	onChangePosition.SetParentObject(this);
	onChangeSize.SetParentObject(this);

	onMouseOver.SetParentObject(this);
	onMouseOut.SetParentObject(this);

	onLeftButtonDown.SetParentObject(this);
	onLeftButtonUp.SetParentObject(this);
	onLeftButtonClick.SetParentObject(this);
	onLeftButtonDoubleClick.SetParentObject(this);

	onRightButtonDown.SetParentObject(this);
	onRightButtonUp.SetParentObject(this);
	onRightButtonClick.SetParentObject(this);
	onRightButtonDoubleClick.SetParentObject(this);

	onMouseDragStart.SetParentObject(this);
	onMouseDrag.SetParentObject(this);
	onMouseDragEnd.SetParentObject(this);

	onMouseWheel.SetParentObject(this);

	onAction.SetParentObject(this);

	onGetFocus.SetParentObject(this);
	onLoseFocus.SetParentObject(this);

	onClickInFocus.SetParentObject(this);

	onRegister.SetParentObject(this);
	onUnregister.SetParentObject(this);
}

S::GUI::Widget::~Widget()
{
	DeactivateTooltip();

	while (widgets.GetNOfEntries()) UnregisterObject(widgets.GetFirstEntry());

	widgets.RemoveAll();

	if (registered && container != NIL) container->UnregisterObject(this);

	delete nullSurface;
}

S::Int S::GUI::Widget::RegisterObject(Widget *widget)
{
	if (widget == NIL) return Error();

	if (!widget->IsRegistered())
	{
		widgets.AddEntry(widget, widget->GetHandle());

		widget->SetContainer(this);
		widget->SetRegisteredFlag(True);

		widget->onRegister.Emit(this);

		if (widget->visible)
		{
			widget->visible = False;

			widget->Show();
		}

		return Success();
	}

	return Error();
}

S::Int S::GUI::Widget::UnregisterObject(Widget *widget)
{
	if (widget == NIL) return Error();

	if (widget->IsRegistered())
	{
		if (widgets.RemoveEntry(widget->GetHandle()) == True)
		{
			widget->Hide();
			widget->onUnregister.Emit(this);

			widget->SetRegisteredFlag(False);
			widget->SetContainer(NIL);

			widget->visible = True;

			return Success();
		}
	}

	return Error();
}

S::Int S::GUI::Widget::GetNOfObjects()
{
	return widgets.GetNOfEntries();
}

S::GUI::Widget *S::GUI::Widget::GetNthObject(Int n)
{
	return widgets.GetNthEntry(n);
}

S::GUI::Window *S::GUI::Widget::GetContainerWindow()
{
	Widget	*widget = this;

	while (widget->GetObjectType() != Window::classID)
	{
		if (widget->GetContainer() == NIL) return NIL;

		widget = widget->GetContainer();
	}

	return (Window *) widget;
}

S::GUI::Surface *S::GUI::Widget::GetDrawSurface()
{
	if (IsRegistered())	return container->GetDrawSurface();
	else			return drawSurface;
}

S::Int S::GUI::Widget::SetContainer(Widget *newContainer)
{
	container = newContainer;

	return Success();
}

S::GUI::Widget *S::GUI::Widget::GetContainer()
{
	return container;
}

S::Void S::GUI::Widget::SetRegisteredFlag(Bool flag)
{
	registered = flag;
}

S::GUI::Widget *S::GUI::Widget::GetPreviousTabstopWidget(Int widgetHandle)
{
	Widget	*lastTabstopObject = NIL;

	if (registered && widgetHandle != 0) lastTabstopObject = container->GetPreviousTabstopWidget(GetHandle());

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		if (widget->GetHandle() == widgetHandle) return lastTabstopObject;

		if (widget->IsTabstopCapable() && widget->IsActive()) lastTabstopObject = widget;
		else if (widget->GetPreviousTabstopWidget(0) != NIL) lastTabstopObject = widget->GetPreviousTabstopWidget(0);
	}

	if (widgetHandle == 0) return lastTabstopObject;

	return NIL;
}

S::GUI::Widget *S::GUI::Widget::GetNextTabstopWidget(Int widgetHandle)
{
	Bool	 found = False;

	if (widgetHandle == 0) found = True;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		if (widget->GetHandle() == widgetHandle)
		{
			found = True;
			continue;
		}

		if (found && widget->IsTabstopCapable() && widget->IsActive()) return widget;
		else if (found && widget->GetNextTabstopWidget(0) != NIL) return widget->GetNextTabstopWidget(0);
	}

	if (registered && widgetHandle != 0) return container->GetNextTabstopWidget(GetHandle());

	return NIL;
}

S::Void S::GUI::Widget::GetTextSize()
{
	textSize.cx = font.GetTextSizeX(text);
	textSize.cy = font.GetTextSizeY(text);
}

S::GUI::Point S::GUI::Widget::GetRealPosition()
{
	if (!registered) return pos;

	Point	 realPos	= pos;
	Point	 containerPos	= container->GetRealPosition();

	realPos.x = containerPos.x + pos.x;
	realPos.y = containerPos.y + pos.y;

	if (orientation == OR_UPPERRIGHT)
	{
		realPos.x = containerPos.x + container->size.cx - pos.x;
	}
	else if (orientation == OR_LOWERLEFT)
	{
		realPos.y = containerPos.y + container->size.cy - pos.y;
	}
	else if (orientation == OR_LOWERRIGHT)
	{
		realPos.x = containerPos.x + container->size.cx - pos.x;
		realPos.y = containerPos.y + container->size.cy - pos.y;
	}

	return realPos;
}

const S::GUI::Color &S::GUI::Widget::GetBackgroundColor()
{
	return backgroundColor;
}

S::Int S::GUI::Widget::SetBackgroundColor(const Color &nColor)
{
	backgroundColor = nColor;

	Paint(SP_PAINT);

	return Success();
}

S::Int S::GUI::Widget::Show()
{
	if (visible) return Success();

	visible = True;

	if (!registered) return Success();

	if (IsVisible()) Paint(SP_SHOW);

	onShow.Emit();

	return Success();
}

S::Int S::GUI::Widget::Hide()
{
	if (!visible) return Success();

	Bool	 wasVisible = IsVisible();

	visible = False;

	if (!registered) return Success();

	if (wasVisible)
	{
		Rect	 rect		= Rect(GetRealPosition(), size);
		Surface	*surface	= container->GetDrawSurface();

		if (container->GetBackgroundColor() != -1)	surface->Box(rect, container->GetBackgroundColor(), FILLED);
		else						surface->Box(rect, Setup::BackgroundColor, FILLED);
	}

	onHide.Emit();

	return Success();
}

S::Int S::GUI::Widget::Activate()
{
	if (active) return Success();

	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	active = True;

	if (registered && prevVisible) Show();

	onActivate.Emit();

	return Success();
}

S::Int S::GUI::Widget::Deactivate()
{
	if (!active) return Success();

	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	active = False;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		widget->mouseOver	= False;

		widget->leftButtonDown	= False;
		widget->rightButtonDown	= False;

		widget->mouseDragging	= False;
	}

	if (registered && prevVisible) Show();

	onDeactivate.Emit();

	return Success();
}

S::Int S::GUI::Widget::Paint(Int message)
{
	if (!registered)	return Error();
	if (!visible)		return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			for (Int i = 0; i < GetNOfObjects(); i++)
			{
				Widget	*widget = GetNthObject(i);

				widget->Paint(message);
			}

			break;
	}

	return Success();
}

S::Int S::GUI::Widget::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Window	*window	= container->GetContainerWindow();

	if (window == NIL) return Success();

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = GetNthObject(i);

		if (object->Process(message, wParam, lParam) == Break) return Break;
	}

	if (!IsActive())	return Success();

	EnterProtectedRegion();

	Point	 realPosition	= GetRealPosition();
	Rect	 frame		= Rect(realPosition, size);
	Point	 mousePos	= Point(window->MouseX(), window->MouseY());
	Int	 returnValue	= Success();

	switch (message)
	{
		case SM_MOUSEMOVE:
			if (!mouseOver && window->IsMouseOn(frame) && hitTest.Call(mousePos - realPosition))
			{
				mouseOver = True;

				if (statusText != NIL) window->SetStatusText(statusText);

				if (tooltipText != NIL)
				{
					tipTimer = new System::Timer();

					tipTimer->onInterval.Connect(&Widget::ActivateTooltip, this);
					tipTimer->Start(500);
				}

				Paint(SP_MOUSEIN);

				onMouseOver.Emit();
			}
			else if (mouseOver && !(window->IsMouseOn(frame) && hitTest.Call(mousePos - realPosition)))
			{
				mouseOver = False;

				leftButtonDown = False;
				rightButtonDown = False;

				if (statusText != NIL && window->GetStatusText() == statusText) window->RestoreDefaultStatusText();

				DeactivateTooltip();

				Paint(SP_MOUSEOUT);

				onMouseOut.Emit();
			}
			else if (mouseOver && window->IsMouseOn(frame) && hitTest.Call(mousePos - realPosition))
			{
				if (tipTimer != NIL && wParam == 0)
				{
					tipTimer->Stop();
					tipTimer->Start(500);
				}
			}

			if (mouseDragging)
			{
				onMouseDrag.Emit(mousePos);
			}

			break;
		case SM_LBUTTONDOWN:
			if (mouseOver)
			{
				leftButtonDown = True;

				DeactivateTooltip();

				Paint(SP_MOUSEDOWN);

				if (!focussed)
				{
					focussed = True;

					onGetFocus.Emit();
				}
				else
				{
					onClickInFocus.Emit();
				}

				onLeftButtonDown.Emit(mousePos);

				if (!mouseDragging)
				{
					mouseDragging = True;

					onMouseDragStart.Emit(mousePos);
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
		case SM_LBUTTONDBLCLK:
			if (mouseOver)
			{
				leftButtonDown = True;

				Paint(SP_MOUSEDOWN);

				onLeftButtonDown.Emit(mousePos);
				onLeftButtonDoubleClick.Emit(mousePos);
			}

			break;
		case SM_LBUTTONUP:
			if (leftButtonDown)
			{
				leftButtonDown = False;

				Paint(SP_MOUSEUP);

				onLeftButtonUp.Emit(mousePos);
				onLeftButtonClick.Emit(mousePos);
			}

			if (mouseDragging)
			{
				mouseDragging = False;

				onMouseDragEnd.Emit(mousePos);
			}

			break;
		case SM_MOUSEWHEEL:
			{
				UnsignedInt	 scrollLines = 0;

				if (Setup::enableUnicode)	SystemParametersInfoW(104, NIL, &scrollLines, NIL);
				else				SystemParametersInfoA(104, NIL, &scrollLines, NIL);

				if (scrollLines <= 0) scrollLines = 3;

				onMouseWheel.Emit(wParam / 120 * scrollLines);
			}

			break;
#ifdef __WIN32__
		case WM_KEYDOWN:
			if (focussed && tabstopCapable)
			{
				switch (wParam)
				{
					case VK_TAB:
						{
							BYTE	 state[256];

							GetKeyboardState(state);

							Widget	*widget = NIL;

							if (state[VK_SHIFT] & 128) widget = container->GetPreviousTabstopWidget(GetHandle());
							else			   widget = container->GetNextTabstopWidget(GetHandle());

							if (widget != NIL)
							{
								focussed = False;

								onLoseFocus.Emit();

								widget->SetFocusByKeyboard();
							}

							returnValue = Break;
						}

						break;
				}
			}

			break;
		case WM_KILLFOCUS:
			if (Window::GetWindow((HWND) wParam) != NIL)
			{
				if (Window::GetWindow((HWND) wParam)->GetObjectType() == ToolWindow::classID && Window::GetWindow((HWND) wParam)->GetOrder() >= window->GetOrder()) break;
			}
		case WM_ACTIVATEAPP:
			if (focussed)
			{
				focussed = False;

				onLoseFocus.Emit();
			}

			break;
#endif
	}

	LeaveProtectedRegion();

	return returnValue;
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
	tooltip->SetMetrics(Point(window->MouseX() - Math::Round(0.2 * tooltip->textSize.cx), window->MouseY() - 1), Size(0, 0));
	tooltip->SetTimeout(3000);

	PopupMenu::internalOnOpenPopupMenu.Connect(&Widget::DeactivateTooltip, this);

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

		DeleteObject(tooltip);

		tooltip = NIL;
	}
}

S::Void S::GUI::Widget::SetFocusByKeyboard()
{
	if (!focussed)
	{
		focussed = True;

		onGetFocus.Emit();
		onGetFocusByKeyboard.Emit();
	}
}

S::Int S::GUI::Widget::SetText(const String &newText)
{
	if (text == newText) return Success();

	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	text = newText;

	GetTextSize();

	if (registered && prevVisible) Show();

	Process(SM_MOUSEMOVE, 0, 0);

	return Success();
}

const S::String &S::GUI::Widget::GetText()
{
	return text;
}

S::Int S::GUI::Widget::SetTooltipText(const String &nTooltipText)
{
	tooltipText = nTooltipText;

	return Success();
}

const S::String &S::GUI::Widget::GetTooltipText()
{
	return tooltipText;
}

S::Int S::GUI::Widget::SetStatusText(const String &nStatusText)
{
	statusText = nStatusText;

	return Success();
}

const S::String &S::GUI::Widget::GetStatusText()
{
	return statusText;
}

S::Int S::GUI::Widget::SetFont(const Font &nFont)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	font = nFont;

	GetTextSize();

	if (registered && prevVisible) Show();

	return Success();
}

const S::GUI::Font &S::GUI::Widget::GetFont()
{
	return font;
}

S::Int S::GUI::Widget::SetOrientation(Int nOrientation)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	orientation = nOrientation;

	if (registered && prevVisible) Show();

	return Success();
}

S::Int S::GUI::Widget::GetOrientation()
{
	return orientation;
}

S::Int S::GUI::Widget::SetMetrics(const Point &nPos, const Size &nSize)
{
	if (nPos.x == pos.x && nPos.y == pos.y && nSize.cx == size.cx && nSize.cy == size.cy) return Success();

	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	if (nPos.x   != pos.x   || nPos.y   != pos.y  )	{ pos  = nPos;  onChangePosition.Emit(pos); }
	if (nSize.cx != size.cx || nSize.cy != size.cy)	{ size = nSize; onChangeSize.Emit(size);    }

	if (registered && prevVisible) Show();

	return Success();
}

S::Bool S::GUI::Widget::IsRightToLeft() const
{
	if (IsRegistered())
	{
		Window	*containerWindow = container->GetContainerWindow();

		if (containerWindow != NIL) return containerWindow->IsRightToLeft();
	}

	return Setup::rightToLeft;
}

S::Bool S::GUI::Widget::IsAffected(const Rect &uRect)
{
	return Rect::DoRectsOverlap(uRect, Rect(GetRealPosition() - Point(10, 10), size + Size(20, 20)));
}

S::Bool S::GUI::Widget::DefaultHitTest(const Point &mousePos)
{
	return (mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x <= size.cx - 1 && mousePos.y <= size.cy - 1);
}
