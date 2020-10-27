 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/gui/application/application.h>
#include <smooth/misc/math.h>
#include <smooth/system/timer.h>
#include <smooth/system/screen.h>
#include <smooth/input/keyboard.h>
#include <smooth/foreach.h>

#ifdef __WIN32__
#	include <windows.h>

#	undef GetObject
#endif

const S::Short	 S::GUI::Widget::classID = S::Object::RequestClassID();

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

	realPosValid	= False;
	realSizeValid	= False;

	orientation	= OR_UPPERLEFT;

	text		= NIL;

	unscaledTextSize= Size();
	scaledTextSize	= Size();

	mouseOver	= False;

	leftButtonDown	= False;
	rightButtonDown	= False;

	mouseDragging	= False;

	alwaysActive	= False;
	independent	= False;

	tabstopCapable	= False;
	dropTarget	= False;

	tooltipText	= NIL;
	tooltipLayer	= NIL;

	tipTimer	= NIL;
	tooltip		= NIL;

	contextMenu	= NIL;

	font.SetColor(Setup::TextColor);

	backgroundColor	= Setup::BackgroundColor;
	backgroundColorSet = False;

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
}

S::Int S::GUI::Widget::EnableLocking(Bool enable)
{
	if (enable) widgets.EnableLocking();
	else	    widgets.DisableLocking();

	foreach (Widget *widget, widgets) widget->EnableLocking(enable);

	return Object::EnableLocking(enable);
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

		widget->EnableLocking(IsLockingEnabled());

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
	if (IsVisible()) return container->GetDrawSurface();
	else		 return Surface::GetNullSurface();
}

S::GUI::Widget *S::GUI::Widget::GetPreviousTabstopWidget(Int widgetHandle) const
{
	Widget	*lastTabstopObject = NIL;

	if (registered && widgetHandle != 0) lastTabstopObject = container->GetPreviousTabstopWidget(GetHandle());

	foreach (Widget *widget, widgets)
	{
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

	foreach (Widget *widget, widgets)
	{
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
	scaledTextSize.cx = font.GetScaledTextSizeX(text);
	scaledTextSize.cy = font.GetScaledTextSizeY(text);

	Float	 dpi = Surface().GetSurfaceDPI();

	if (Math::Abs(dpi - 96.0) < 0.1) unscaledTextSize = scaledTextSize;
	else				 unscaledTextSize = scaledTextSize * 96.0 / dpi;
}

S::Void S::GUI::Widget::InvalidateMetrics()
{
	realPosValid  = False;
	realSizeValid = False;

	foreach (Widget *widget, widgets) widget->InvalidateMetrics();
}

S::GUI::Point S::GUI::Widget::GetRealPosition() const
{
	if (!registered)  return pos;
	if (realPosValid) return realPos;

	Surface	*surface = GetDrawSurface();

	Point	 containerPos  = container->GetRealPosition();
	Size	 containerSize = container->GetRealSize();
	Point	 scaledPos     = pos * surface->GetSurfaceDPI() / 96.0;

	realPos	= containerPos + scaledPos;

	if (orientation == OR_UPPERRIGHT)
	{
		realPos.x = containerPos.x + containerSize.cx - scaledPos.x;
	}
	else if (orientation == OR_LOWERLEFT)
	{
		realPos.y = containerPos.y + containerSize.cy - scaledPos.y;
	}
	else if (orientation == OR_LOWERRIGHT)
	{
		realPos.x = containerPos.x + containerSize.cx - scaledPos.x;
		realPos.y = containerPos.y + containerSize.cy - scaledPos.y;
	}

	realPosValid = True;

	return realPos;
}

S::GUI::Size S::GUI::Widget::GetRealSize() const
{
	if (!registered)   return size;
	if (realSizeValid) return realSize;

	Surface	*surface = GetDrawSurface();

	realSize      = size * surface->GetSurfaceDPI() / 96.0;
	realSizeValid = True;

	return realSize;
}

S::Int S::GUI::Widget::SetBackgroundColor(const Color &nColor)
{
	if (backgroundColorSet && backgroundColor == nColor) return Success();

	backgroundColor	   = nColor;
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

	/* Show ourself.
	 */
	visible = True;

	if (!registered) return Success();

	if (IsVisible()) Paint(SP_PAINT);

	/* Show child widgets.
	 */
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = GetNthObject(i);

		if (object == NIL || !object->IsVisible()) continue;

		visible = False;
		object->Hide();

		visible = True;
		object->Show();
	}

	/* Notify listeners.
	 */
	onShow.Emit();

	return Success();
}

S::Int S::GUI::Widget::Hide()
{
	if (!visible) return Success();

	/* Hide child widgets.
	 */
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = GetNthObject(i);

		if (object == NIL || !object->IsVisible()) continue;

		object->Hide();
		visible = False;

		object->Show();
		visible = True;
	}

	/* Hide ourself.
	 */
	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	visible = False;

	if (!registered) return Success();

	if (surface)
	{
		if (focussed) { focussed = False; onLoseFocus.Emit(); }

		surface->Box(GetVisibleArea(), container->GetBackgroundColor(), Rect::Filled);

		DeactivateTooltip();
	}

	/* Reset mouse status.
	 */
	mouseOver	= False;

	leftButtonDown	= False;
	rightButtonDown	= False;

	mouseDragging	= False;

	/* Notify listeners.
	 */
	onHide.Emit();

	return Success();
}

S::Int S::GUI::Widget::Activate()
{
	if (active) return Success();

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) { surface->StartPaint(GetVisibleArea()); Hide(); }

	active = True;

	if (surface) { Show(); Process(SM_MOUSEMOVE, 0, 0); surface->EndPaint(); }

	onActivate.Emit();

	return Success();
}

S::Int S::GUI::Widget::Deactivate()
{
	if (!active) return Success();

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) { surface->StartPaint(GetVisibleArea()); Hide(); }

	active = False;

	foreach (Widget *widget, widgets)
	{
		widget->mouseOver	= False;

		widget->leftButtonDown	= False;
		widget->rightButtonDown	= False;

		widget->mouseDragging	= False;
	}

	if (surface) { Show(); surface->EndPaint(); }

	onDeactivate.Emit();

	return Success();
}

S::Int S::GUI::Widget::Paint(Int message)
{
	if (!registered) return Error();
	if (!visible)	 return Success();

	Window	*window	= container->GetContainerWindow();

	if (window == NIL) return Success();

	switch (message)
	{
		case SP_PAINT:
			widgets.LockForRead();

			foreach (Widget *widget, widgets)
			{
				if (widget->IsAffected(window->GetUpdateRect())) widget->Paint(message);
			}

			widgets.Unlock();

			break;
	}

	return Success();
}

S::Int S::GUI::Widget::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Window	*window	= container->GetContainerWindow();

	if (window == NIL)   return Success();

	widgets.LockForWrite();

	foreach (Widget *widget, widgets)
	{
		if (widget->Process(message, wParam, lParam) == MessageProcessed) { widgets.Unlock(); return MessageProcessed; }
	}

	widgets.Unlock();

	if (!IsVisible())    return Success();
	if (!IsActive())     return Success();

	EnterProtectedRegion();

	Int	 returnValue = Success();
	Point	 mousePos    = window->GetMousePosition();

	switch (message)
	{
		case SM_MOUSEMOVE:
			{
				Point	 realPosition = GetRealPosition();
				Rect	 visibleArea  = GetVisibleArea();

				if (!mouseOver && window->IsMouseOn(visibleArea) && hitTest.Call(mousePos - realPosition))
				{
					mouseOver = True;

					if (statusText != NIL) window->SetStatusText(statusText);

					if ((tooltipText != NIL || tooltipLayer != NIL) && window->IsFocussed())
					{
						Application::Lock	 lock;

						tipPos	 = mousePos;
						tipTimer = new System::Timer();

						tipTimer->onInterval.Connect(&Widget::ActivateTooltip, this);
						tipTimer->Start(Setup::HoverTime);
					}

					Paint(SP_MOUSEIN);

					onMouseOver.Emit();
				}
				else if (mouseOver && !(window->IsMouseOn(visibleArea) && hitTest.Call(mousePos - realPosition)))
				{
					mouseOver = False;

					leftButtonDown	= False;
					rightButtonDown	= False;

					if (statusText != NIL && window->GetStatusText() == statusText) window->RestoreDefaultStatusText();

					DeactivateTooltip();

					Paint(SP_MOUSEOUT);

					onMouseOut.Emit();
				}
				else if (mouseOver && window->IsMouseOn(visibleArea) && hitTest.Call(mousePos - realPosition))
				{
					Application::Lock	 lock;

					if (tipTimer != NIL && wParam == 0 && (Math::Abs(tipPos.x - mousePos.x) > Setup::HoverWidth / 2 || Math::Abs(tipPos.y - mousePos.y) > Setup::HoverHeight / 2))
					{
						tipPos = mousePos;

						tipTimer->Stop();
						tipTimer->Start(Setup::HoverTime);
					}
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
			if (mouseOver)
			{
				Paint(SP_MOUSEUP);

				onLeftButtonUp.Emit(mousePos);

				if (leftButtonDown)
				{
					leftButtonDown = False;

					onLeftButtonClick.Emit(mousePos);
				}
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
				SystemParametersInfo(104, NIL, &scrollLines, NIL);
#endif

				if (scrollLines <= 0) scrollLines = 3;

				onMouseWheel.Emit(Float(wParam) / 120.0 * scrollLines);
			}

			break;
		case SM_KEYDOWN:
			if (!focussed || !tabstopCapable) break;

			if (wParam == Input::Keyboard::KeyTab)
			{
				Widget	*widget = NIL;

				if (Input::Keyboard::GetKeyState(Input::Keyboard::KeyShift)) widget = container->GetPreviousTabstopWidget(GetHandle());
				else							     widget = container->GetNextTabstopWidget(GetHandle());

				if (widget != NIL)
				{
					focussed = False;

					onLoseFocus.Emit();

					widget->SetFocusByKeyboard();
				}

				returnValue = MessageProcessed;
			}

			break;
		case SM_LOSEFOCUS:
			if (focussed)
			{
				Window	*focusWnd = (Window *) Object::GetObject(wParam, Window::classID);

				if (focusWnd != NIL)
				{
					if ((focusWnd->GetObjectType() == ToolWindow::classID && focusWnd->GetOrder() >= window->GetOrder()) || focusWnd == window) break;
				}

				focussed = False;

				onLoseFocus.Emit();
			}

			break;
	}

	LeaveProtectedRegion();

	return returnValue;
}

S::Void S::GUI::Widget::ActivateTooltip()
{
	Application::Lock	 lock;

	if (tipTimer == NIL || tooltip != NIL) return;

	tipTimer->Stop();

	DeleteObject(tipTimer);

	tipTimer = NIL;

	if (IsVisible())
	{
		Window	*window	= container->GetContainerWindow();

		tooltip = new Tooltip();

		if (tooltipText != NIL)
		{
			tooltip->SetText(tooltipText);
			tooltip->SetMetrics(window->GetMousePosition() - Point(tooltip->GetUnscaledTextWidth() > 0 ? Math::Round(0.2 * tooltip->GetUnscaledTextWidth()) : 20, Setup::HoverHeight / 2 + 1), Size(0, 0));
			tooltip->SetTimeout(3000);
		}
		else if (tooltipLayer != NIL)
		{
			tooltipLayer->Show();

			tooltip->SetLayer(tooltipLayer);
			tooltip->SetPosition(window->GetMousePosition() - Point(Math::Round(0.2 * tooltipLayer->GetWidth()), Setup::HoverHeight / 2 + 1));
			tooltip->SetTimeout(3000);
		}

		PopupMenu::internalOnOpenPopupMenu.Connect(&Widget::DeactivateTooltip, this);

		window->Add(tooltip);
	}
}

S::Void S::GUI::Widget::DeactivateTooltip()
{
	Application::Lock	 lock;

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

		Window	*window	     = container->GetContainerWindow();
		Surface	*surface     = GetDrawSurface();

		Rect	 monitor     = System::Screen::GetActiveScreenWorkArea();
		Float	 scaleFactor = surface->GetSurfaceDPI() / 96.0;

		contextMenu->CalculateSize();

		Point	 popupPos  = window->GetMousePosition();
		Size	 popupSize = contextMenu->GetSize();

		if (!IsRightToLeft()) { if (window->GetX() + popupPos.x			       + Math::Round(popupSize.cx * scaleFactor) >= monitor.right) popupPos.x = popupPos.x - Math::Round(popupSize.cx * scaleFactor); }
		else		      { if (window->GetX() + (window->GetWidth() - popupPos.x) - Math::Round(popupSize.cx * scaleFactor) <  monitor.left)  popupPos.x = popupPos.x - Math::Round(popupSize.cx * scaleFactor); }

		if (window->GetY() + popupPos.y + Math::Round(popupSize.cy * scaleFactor) >= monitor.bottom) popupPos.y = popupPos.y - Math::Round(popupSize.cy * scaleFactor);

		contextMenu->SetPosition(popupPos);
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

S::Bool S::GUI::Widget::IsDropTarget() const
{
	if (dropTarget) return True;

	foreach (Widget *widget, widgets)
	{
		if (widget->IsDropTarget()) return True;
	}

	return False;
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

S::Int S::GUI::Widget::SetText(const String &nText)
{
	if (text == nText) return Success();

	Surface	*surface      = GetDrawSurface();

	Bool	 prevVisible  = IsVisible();
	Bool	 prevFocussed = focussed;

	if (registered && prevFocussed) { focussed = False; }
	if (registered && prevVisible)	{ surface->StartPaint(GetVisibleArea()); Hide(); }

	text = nText;

	ComputeTextSize();

	if (registered && prevVisible)  { Show(); Process(SM_MOUSEMOVE, 0, 0); surface->EndPaint(); }
	if (registered && prevFocussed) { focussed = True; }

	return Success();
}

S::Int S::GUI::Widget::SetTooltipText(const String &nTooltipText)
{
	tooltipText  = nTooltipText;
	tooltipLayer = NIL;

	return Success();
}

S::Int S::GUI::Widget::SetTooltipLayer(Layer *nTooltipLayer)
{
	tooltipText  = NIL;
	tooltipLayer = nTooltipLayer;

	return Success();
}

S::Int S::GUI::Widget::SetFont(const Font &nFont)
{
	if (font == nFont) return Success();

	Surface	*surface      = GetDrawSurface();

	Bool	 prevVisible  = IsVisible();
	Bool	 prevFocussed = focussed;

	if (registered && prevFocussed) { focussed = False; }
	if (registered && prevVisible)	{ surface->StartPaint(GetVisibleArea()); Hide(); }

	font = nFont;

	ComputeTextSize();

	if (registered && prevVisible)  { Show(); Process(SM_MOUSEMOVE, 0, 0); surface->EndPaint(); }
	if (registered && prevFocussed) { focussed = True; }

	return Success();
}

S::Int S::GUI::Widget::SetOrientation(Int nOrientation)
{
	if (orientation == nOrientation) return Success();

	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	orientation = nOrientation;

	if (registered && prevVisible) Show();

	return Success();
}

S::GUI::Rect S::GUI::Widget::GetVisibleArea() const
{
	if (!IsRegistered()) return Rect();

	if (IsIndependent()) return Rect(GetRealPosition(), GetRealSize());
	else		     return Rect::OverlapRect(Rect(GetRealPosition(), GetRealSize()), container->GetVisibleArea());
}

S::Int S::GUI::Widget::SetMetrics(const Point &nPos, const Size &nSize)
{
	if (nPos.x == pos.x && nPos.y == pos.y && nSize.cx == size.cx && nSize.cy == size.cy) return Success();

	DeactivateTooltip();

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface && orientation != OR_FREE) surface->StartPaint(container->GetVisibleArea());
	if (surface			     ) Hide();

	InvalidateMetrics();

	if (nPos.x   != pos.x   || nPos.y   != pos.y  )	{ pos  = nPos;  onChangePosition.Emit(pos); }
	if (nSize.cx != size.cx || nSize.cy != size.cy)	{ size = nSize; onChangeSize.Emit(size);    }

	if (surface			     ) { Show(); Process(SM_MOUSEMOVE, 0, 0); }
	if (surface && orientation != OR_FREE) surface->EndPaint();

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
	return Rect::DoRectsOverlap(uRect, Rect(GetRealPosition() - Point(10, 10), GetRealSize() + Size(20, 20)));
}

S::Bool S::GUI::Widget::DefaultHitTest(const Point &mousePos)
{
	Size	 realSize = GetRealSize();

	return (mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x <= realSize.cx - 1 && mousePos.y <= realSize.cy - 1);
}
