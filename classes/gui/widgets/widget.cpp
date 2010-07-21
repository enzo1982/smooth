 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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

	textSize.cx	= 0;
	textSize.cy	= 0;

	mouseOver	= False;

	leftButtonDown	= False;
	rightButtonDown	= False;

	mouseDragging	= False;

	alwaysActive	= False;
	independent	= False;
	tabstopCapable	= False;

	tooltipText	= NIL;
	tooltipLayer	= NIL;

	tipTimer	= NIL;
	tooltip		= NIL;

	contextMenu	= NIL;

	font.SetColor(Setup::TextColor);

	backgroundColor	= Setup::BackgroundColor;
	backgroundColorSet = False;

	nullSurface	= new GUI::Surface();
	drawSurface	= nullSurface;

	hitTest.Connect(&Widget::DefaultHitTest, this);
	getContextMenu.Connect((PopupMenu *) NIL);

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

	onOpenContextMenu.SetParentObject(this);
	onCloseContextMenu.SetParentObject(this);

	onGetFocus.SetParentObject(this);
	onLoseFocus.SetParentObject(this);

	onClickInFocus.SetParentObject(this);

	onRegister.SetParentObject(this);
	onUnregister.SetParentObject(this);
}

S::GUI::Widget::~Widget()
{
	/* Clean up everything in case this widget
	 * is not deleted using DeleteObject.
	 */
	DeactivateTooltip();

	CloseContextMenu();

	while (widgets.Length()) Remove(widgets.GetFirst());

	widgets.RemoveAll();

	if (registered && container != NIL) container->Remove(this);

	delete nullSurface;
}

S::Void S::GUI::Widget::EnqueueForDeletion()
{
	onEnqueueForDeletion.Emit();

	DeactivateTooltip();

	CloseContextMenu();

	if (registered && container != NIL) container->Remove(this);
}

S::Int S::GUI::Widget::Add(Widget *widget)
{
	if (widget == NIL) return Error();

	if (!widget->IsRegistered())
	{
		widgets.Add(widget, widget->GetHandle());

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

S::Int S::GUI::Widget::Remove(Widget *widget)
{
	if (widget == NIL) return Error();

	if (widget->IsRegistered())
	{
		if (widgets.Remove(widget->GetHandle()) == True)
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

S::Int S::GUI::Widget::GetNOfObjects() const
{
	return widgets.Length();
}

S::GUI::Widget *S::GUI::Widget::GetNthObject(Int n) const
{
	return widgets.GetNth(n);
}

S::GUI::Window *S::GUI::Widget::GetContainerWindow() const
{
	Widget	*widget = const_cast<Widget *>(this);

	while (widget->GetObjectType() != Window::classID)
	{
		if (widget->GetContainer() == NIL) return NIL;

		widget = widget->GetContainer();
	}

	return (Window *) widget;
}

S::GUI::Surface *S::GUI::Widget::GetDrawSurface() const
{
	if (IsRegistered())	return container->GetDrawSurface();
	else			return drawSurface;
}

S::Int S::GUI::Widget::SetContainer(Widget *newContainer)
{
	container = newContainer;

	return Success();
}

S::GUI::Widget *S::GUI::Widget::GetContainer() const
{
	return container;
}

S::Void S::GUI::Widget::SetRegisteredFlag(Bool flag)
{
	registered = flag;
}

S::GUI::Widget *S::GUI::Widget::GetPreviousTabstopWidget(Int widgetHandle) const
{
	Widget	*lastTabstopObject = NIL;

	if (registered && widgetHandle != 0) lastTabstopObject = container->GetPreviousTabstopWidget(GetHandle());

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		if (widget->GetHandle() == widgetHandle) return lastTabstopObject;

		if	(widget->IsTabstopCapable() && widget->IsActive()) lastTabstopObject = widget;
		else if (widget->GetPreviousTabstopWidget(0) != NIL)	   lastTabstopObject = widget->GetPreviousTabstopWidget(0);
	}

	if (widgetHandle == 0) return lastTabstopObject;

	return NIL;
}

S::GUI::Widget *S::GUI::Widget::GetNextTabstopWidget(Int widgetHandle) const
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

		if	(found && widget->IsTabstopCapable() && widget->IsActive()) return widget;
		else if (found && widget->GetNextTabstopWidget(0) != NIL)	    return widget->GetNextTabstopWidget(0);
	}

	if (registered && widgetHandle != 0) return container->GetNextTabstopWidget(GetHandle());

	return NIL;
}

S::Void S::GUI::Widget::ComputeTextSize()
{
	textSize.cx = font.GetTextSizeX(text);
	textSize.cy = font.GetTextSizeY(text);
}

S::GUI::Point S::GUI::Widget::GetRealPosition() const
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

S::Int S::GUI::Widget::SetBackgroundColor(const Color &nColor)
{
	backgroundColor = nColor;
	backgroundColorSet = True;

	Paint(SP_PAINT);

	return Success();
}

const S::GUI::Color &S::GUI::Widget::GetBackgroundColor() const
{
	if (backgroundColorSet)	 return backgroundColor;
	else if (IsRegistered()) return container->GetBackgroundColor();
	else			 return Setup::BackgroundColor;
}

S::Bool S::GUI::Widget::IsBackgroundColorSet() const
{
	if (backgroundColorSet)	 return True;
	else if (IsRegistered()) return container->IsBackgroundColorSet();
	else			 return False;
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
		if (focussed)
		{
			focussed = False;

			onLoseFocus.Emit();
		}

		Rect	 rect		= GetVisibleArea();
		Surface	*surface	= GetDrawSurface();

		surface->Box(rect, container->GetBackgroundColor(), Rect::Filled);

		DeactivateTooltip();
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

	Window	*window	= container->GetContainerWindow();

	if (window == NIL) return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			for (Int i = 0; i < GetNOfObjects(); i++)
			{
				Widget	*widget = GetNthObject(i);

				if (widget->IsAffected(window->GetUpdateRect())) widget->Paint(message);
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
	Rect	 visibleArea	= GetVisibleArea();
	Point	 mousePos	= window->GetMousePosition();
	Int	 returnValue	= Success();

	switch (message)
	{
		case SM_MOUSEMOVE:
			if (!mouseOver && window->IsMouseOn(visibleArea) && hitTest.Call(mousePos - realPosition))
			{
				mouseOver = True;

				if (statusText != NIL) window->SetStatusText(statusText);

				if ((tooltipText != NIL || tooltipLayer != NIL) && window->IsFocussed())
				{
					tipTimer = new System::Timer();

					tipTimer->onInterval.Connect(&Widget::ActivateTooltip, this);
					tipTimer->Start(500);
				}

				Paint(SP_MOUSEIN);

				onMouseOver.Emit();
			}
			else if (mouseOver && !(window->IsMouseOn(visibleArea) && hitTest.Call(mousePos - realPosition)))
			{
				mouseOver = False;

				leftButtonDown = False;
				rightButtonDown = False;

				if (statusText != NIL && window->GetStatusText() == statusText) window->RestoreDefaultStatusText();

				DeactivateTooltip();

				Paint(SP_MOUSEOUT);

				onMouseOut.Emit();
			}
			else if (mouseOver && window->IsMouseOn(visibleArea) && hitTest.Call(mousePos - realPosition))
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

				OpenContextMenu();

				/* Force mouseOut event.
				 */
				window->Process(SM_MOUSEMOVE, 0, 0);
			}

			break;
		case SM_MOUSEWHEEL:
			{
				UnsignedInt	 scrollLines = 0;

#ifdef __WIN32__
				if (Setup::enableUnicode)	SystemParametersInfoW(104, NIL, &scrollLines, NIL);
				else				SystemParametersInfoA(104, NIL, &scrollLines, NIL);
#endif

				if (scrollLines <= 0) scrollLines = 3;

				onMouseWheel.Emit(wParam / 120 * scrollLines);
			}

			break;
		case SM_KEYDOWN:
			if (!focussed || !tabstopCapable) break;

			if (wParam == VK_TAB)
			{
#ifdef __WIN32__
				BYTE	 state[256];

				if (GetKeyboardState(state))
				{
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
#endif
			}

			break;
#ifdef __WIN32__
		case WM_KILLFOCUS:
			{
				Window	*windowGetFocus = Window::GetWindow((HWND) wParam);

				if (windowGetFocus != NIL)
				{
					if (windowGetFocus->GetObjectType() == ToolWindow::classID && windowGetFocus->GetOrder() >= window->GetOrder()) break;
				}
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

	if (tooltipText != NIL)
	{
		tooltip->SetText(tooltipText);
		tooltip->SetMetrics(window->GetMousePosition() - Point(Math::Round(0.2 * tooltip->textSize.cx), 1), Size(0, 0));
		tooltip->SetTimeout(3000);
	}
	else if (tooltipLayer != NIL)
	{
		tooltip->SetLayer(tooltipLayer);
		tooltip->SetPosition(window->GetMousePosition() - Point(Math::Round(0.2 * tooltip->textSize.cx), 1));
		tooltip->SetTimeout(3000);
	}

	PopupMenu::internalOnOpenPopupMenu.Connect(&Widget::DeactivateTooltip, this);

	window->Add(tooltip);
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
		PopupMenu::internalOnOpenPopupMenu.Disconnect(&Widget::DeactivateTooltip, this);

		tooltip->Hide();

		DeleteObject(tooltip);

		tooltip = NIL;
	}
}

S::Void S::GUI::Widget::OpenContextMenu()
{
	if (contextMenu != NIL) CloseContextMenu();

	contextMenu = getContextMenu.Call();

	if (contextMenu != NIL)
	{
		onOpenContextMenu.Emit();

		Window	*window	= container->GetContainerWindow();
		Point	 mousePos = window->GetMousePosition();

		contextMenu->CalculateSize();

		contextMenu->SetPosition(mousePos);
		contextMenu->SetAlwaysActive(True);
		contextMenu->internalRequestClose.Connect(&Widget::CloseContextMenu, this);

		Add(contextMenu);
	}
}

S::Void S::GUI::Widget::CloseContextMenu()
{
	if (contextMenu != NIL)
	{
		contextMenu->internalRequestClose.Disconnect(&Widget::CloseContextMenu, this);

		Remove(contextMenu);

		contextMenu = NIL;

		onCloseContextMenu.Emit();
	}
}

S::Int S::GUI::Widget::SetFocus()
{
	if (!focussed)
	{
		focussed = True;

		onGetFocus.Emit();

		if (GetNOfObjects() > 0) GetNthObject(0)->SetFocus();
	}

	return Success();
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
	Bool	 prevFocussed = focussed;

	if (registered && prevFocussed) focussed = False;
	if (registered && prevVisible) Hide();

	text = newText;

	ComputeTextSize();

	if (registered && prevVisible) Show();
	if (registered && prevFocussed) focussed = True;

	Process(SM_MOUSEMOVE, 0, 0);

	return Success();
}

const S::String &S::GUI::Widget::GetText() const
{
	return text;
}

S::Int S::GUI::Widget::SetTooltipText(const String &nTooltipText)
{
	tooltipText = nTooltipText;
	tooltipLayer = NIL;

	return Success();
}

const S::String &S::GUI::Widget::GetTooltipText() const
{
	return tooltipText;
}

S::Int S::GUI::Widget::SetTooltipLayer(Layer *nTooltipLayer)
{
	tooltipText = NIL;
	tooltipLayer = nTooltipLayer;

	return Success();
}

S::GUI::Layer *S::GUI::Widget::GetTooltipLayer() const
{
	return tooltipLayer;
}

S::Int S::GUI::Widget::SetStatusText(const String &nStatusText)
{
	statusText = nStatusText;

	return Success();
}

const S::String &S::GUI::Widget::GetStatusText() const
{
	return statusText;
}

S::Int S::GUI::Widget::SetFont(const Font &nFont)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	font = nFont;

	ComputeTextSize();

	if (registered && prevVisible) Show();

	return Success();
}

const S::GUI::Font &S::GUI::Widget::GetFont() const
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

S::Int S::GUI::Widget::GetOrientation() const
{
	return orientation;
}

S::GUI::Rect S::GUI::Widget::GetVisibleArea() const
{
	if (!IsRegistered()) return Rect();

	if (IsIndependent()) return Rect(GetRealPosition(), GetSize());
	else		     return Rect::OverlapRect(Rect(GetRealPosition(), GetSize()), container->GetVisibleArea());
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

S::Bool S::GUI::Widget::IsAffected(const Rect &uRect) const
{
	return Rect::DoRectsOverlap(uRect, Rect(GetRealPosition() - Point(10, 10), size + Size(20, 20)));
}

S::Bool S::GUI::Widget::DefaultHitTest(const Point &mousePos)
{
	return (mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x <= size.cx - 1 && mousePos.y <= size.cy - 1);
}
