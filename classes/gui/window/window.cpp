 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/window.h>
#include <smooth/gui/window/backends/windowbackend.h>
#include <smooth/gui/application/application.h>
#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/widgets/basic/statusbar.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/modifiers/righttoleft.h>
#include <smooth/graphics/imageloader/imageloader.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>
#include <smooth/misc/binary.h>
#include <smooth/system/event.h>
#include <smooth/system/screen.h>
#include <smooth/system/system.h>
#include <smooth/resources.h>

#ifdef __WIN32__
#	include <smooth/backends/win32/backendwin32.h>

#	undef GetObject

#	if !defined SM_CXPADDEDBORDER
#		define SM_CXPADDEDBORDER 92
#	endif
#endif

S::Array<S::GUI::Window *, S::Void *>	 S::GUI::Window::windows;

const S::Short				 S::GUI::Window::classID = S::Object::RequestClassID();
S::Short				 S::GUI::Window::nOfActiveWindows = 0;

S::GUI::Window::Window(const String &title, const Point &iPos, const Size &iSize, Void *iWindow) : Widget(iPos, iSize)
{
	backend = WindowBackend::CreateBackendInstance();
	backend->onEvent.SetParentObject(this);
	backend->onEvent.Connect(&Window::Process, this);

	backend->onCreate.Connect(&Window::OnCreate, this);
	backend->onDestroy.Connect(&Window::OnDestroy, this);

	backend->onMinimize.Connect(&Window::OnMinimize, this);
	backend->onMaximize.Connect(&Window::OnMaximize, this);

	backend->onRestore.Connect(&Window::OnRestore, this);

	backend->doClose.Connect(&doClose);

	windows.Add(this, GetHandle());

	maximized	= False;
	minimized	= False;

	type		= classID;

	order		= 0;

	layoutDirection = LD_DEFAULT;

	if (title != NIL) text = title;
	else		  text = "smooth Application";

#ifdef __WIN32__
	frameWidth	= GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
#else
	frameWidth	= 1;

	backend->SetSizeModifier(Size(-6, -6));
#endif

	updateRect	= Rect(Point(-1, -1), Size(0, 0));

	icon		= NIL;

	created		= False;
	visible		= False;
	destroyed	= False;

	visibilitySet	= False;

	mainLayer	= new Layer();

	Add(mainLayer);

	doClose.Connect(True);

	onCreate.SetParentObject(this);
	onPaint.SetParentObject(this);
	onEvent.SetParentObject(this);

	handleSystemMessage.Connect(MessageUnknown);
}

S::GUI::Window::~Window()
{
	backend->onEvent.Disconnect(&Window::Process, this);

	if (created && !destroyed) backend->Close();

	Remove(mainLayer);
	DeleteObject(mainLayer);

	windows.Remove(GetHandle());

	delete backend;
}

S::Int S::GUI::Window::SetMetrics(const Point &nPos, const Size &nSize)
{
	if (created && visible) backend->SetMetrics(nPos, nSize);

	Bool	 resized     = (GetSize() != nSize);
	Bool	 prevVisible = visible;

	visible = False;

	Widget::SetMetrics(nPos, nSize);

	if (resized) CalculateOffsets();

	visible = prevVisible;

	return Success();
}

S::Int S::GUI::Window::SetIcon(const Bitmap &nIcon)
{
	Bitmap	 newIcon = nIcon;

#ifdef __WIN32__
	if (newIcon == NIL) newIcon = ImageLoader::Load(String("Icon:").Append(String::FromInt(IDI_ICON)));
#endif

	if (newIcon != NIL)
	{
		icon = newIcon;

		backend->SetIcon(icon);
	}

	return Success();
}

S::Int S::GUI::Window::SetIconDirect(Void *nIcon)
{
	backend->SetIconDirect(nIcon);

	return Success();
}

S::Int S::GUI::Window::SetText(const String &nTitle)
{
	text = nTitle;

	if (created && !destroyed)
	{
		Process(SM_WINDOWTITLECHANGED, 0, 0);

		backend->SetTitle(text);
	}

	return Success();
}

S::Int S::GUI::Window::SetStatusText(const String &nStatus)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget *widget = GetNthObject(i);

		if (widget == NIL) continue;

		if (widget->GetObjectType() == Statusbar::classID)
		{
			widget->SetText(nStatus);

			return Success();
		}
	}

	return Error();
}

const S::String &S::GUI::Window::GetStatusText() const
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget *widget = GetNthObject(i);

		if (widget == NIL) continue;

		if (widget->GetObjectType() == Statusbar::classID) return widget->GetText();
	}

	return defaultStatus;
}

S::Int S::GUI::Window::SetDefaultStatusText(const String &nStatus)
{
	defaultStatus = nStatus;

	RestoreDefaultStatusText();

	return Success();
}

S::Int S::GUI::Window::RestoreDefaultStatusText()
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget *widget = GetNthObject(i);

		if (widget == NIL) continue;

		if (widget->GetObjectType() == Statusbar::classID)
		{
			widget->SetText(defaultStatus);

			break;
		}
	}

	return Success();
}

S::Int S::GUI::Window::SetProgressIndicator(ProgressIndicatorState state, Float value)
{
	return backend->SetProgressIndicator(state, value);
}

const S::Array<S::String> &S::GUI::Window::GetDroppedFiles() const
{
	return backend->GetDroppedFiles();
}

S::Int S::GUI::Window::SetRightToLeft(Bool nRightToLeft)
{
	Surface	*surface = GetDrawSurface();

	surface->SetRightToLeft(nRightToLeft);

	layoutDirection = (nRightToLeft ? LD_RIGHTTOLEFT : LD_LEFTTORIGHT);

	return Success();
}

S::Bool S::GUI::Window::IsRightToLeft() const
{
	return ((layoutDirection == LD_DEFAULT) ? Setup::rightToLeft : (layoutDirection == LD_RIGHTTOLEFT));
}

S::Int S::GUI::Window::Show()
{
	order = Object::RequestObjectHandle();

	if (!created) Create();

	backend->SetMetrics(GetPosition(), GetSize());

	onChangePosition.Emit(GetPosition());
	onChangeSize.Emit(GetSize());

	if (maximized && !visibilitySet)
	{
		backend->Hide();

		maximized = False;
		Maximize();
	}

	if (minimized && !visibilitySet)
	{
		backend->Hide();

		minimized = False;
		Minimize();
	}

	visible	      = True;
	visibilitySet = True;

	backend->Show();

	onShow.Emit();

	return Success();
}

S::Int S::GUI::Window::Hide()
{
	if (!visible) return Success();

	if (created) backend->Hide();

	visible	      = False;
	visibilitySet = True;

	onHide.Emit();

	return Success();
}

S::Int S::GUI::Window::Minimize()
{
	if (minimized) return Success();

	if (!created)
	{
		minimized = True;

		return Success();
	}

	backend->Minimize();

	return Success();
}

S::Int S::GUI::Window::Maximize()
{
	if (maximized) return Success();

	if (!created)
	{
		maximized = True;

		return Success();
	}

	backend->Maximize();

	return Success();
}

S::Int S::GUI::Window::Restore()
{
	if (!maximized && !minimized) return Success();

	if (!created)
	{
		maximized = False;
		minimized = False;

		return Success();
	}

	backend->Restore();

	return Success();
}

S::Int S::GUI::Window::Raise()
{
	if (created) backend->Raise();

	return Success();
}

S::GUI::Rect S::GUI::Window::GetWindowRect() const
{
	return Rect(GetPosition(), GetSize());
}

S::GUI::Rect S::GUI::Window::GetClientRect() const
{
	return Rect(Point(innerOffset.left, innerOffset.top), GetSize() - Size(innerOffset.left + innerOffset.right, innerOffset.top + innerOffset.bottom));
}

S::GUI::Rect S::GUI::Window::GetRestoredWindowRect() const
{
	if (IsMaximized()) return backend->GetRestoredWindowRect();
	else		   return GetWindowRect();
}

S::GUI::Rect S::GUI::Window::GetVisibleArea() const
{
	return Rect(Point(frameWidth, frameWidth), GetRealSize() - Size(frameWidth, frameWidth) * 2);
}

S::Int S::GUI::Window::SetMinimumSize(const Size &newMinSize)
{
	backend->SetMinimumSize(newMinSize);

	return Success();
}

S::Int S::GUI::Window::SetMaximumSize(const Size &newMaxSize)
{
	backend->SetMaximumSize(newMaxSize);

	return Success();
}

S::Bool S::GUI::Window::Create()
{
	if (!IsRegistered() || created) return False;

	Rect	 monitorRect = System::Screen::GetActiveScreenMetrics();

	if (GetPosition().x >= 0 && GetPosition().y >= 0 && GetPosition().x < monitorRect.GetSize().cx && GetPosition().y < monitorRect.GetSize().cy)
	{
		SetPosition(Point(monitorRect.left, monitorRect.top) + GetPosition());
	}

#ifdef __WIN32__
	if (flags & WF_NORESIZE) frameWidth = 4;
#endif

	/* Check if there is a titlebar.
	 */
	flags |= WF_NOTITLE;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		if (widget->GetObjectType() == Titlebar::classID) { flags ^= WF_NOTITLE; break; }
	}

	/* Open the new window.
	 */
	if (backend->Open(text, GetPosition(), GetSize(), flags) == Success())
	{
		Surface	*surface = GetDrawSurface();

		surface->SetRightToLeft(IsRightToLeft());

		visible = False;

		return True;
	}

	return False;
}

S::Int S::GUI::Window::WaitUntilClosed()
{
	if (!IsRegistered()) return Error();

	if (!created) Create();
	if (!visible) Show();

	/* Wait here until window is closed.
	 */
	System::EventProcessor	 event;

	while (!destroyed) event.ProcessNextEvent();

	/* Sleep while Window is still in use.
	 */
	while (IsObjectInUse()) S::System::System::Sleep(10);

	return Success();
}

S::Int S::GUI::Window::Close()
{
	backend->RequestClose();

	return Success();
}

S::Void S::GUI::Window::OnCreate()
{
	if (GetObjectType() != ToolWindow::classID) nOfActiveWindows++;

	created = True;

	CalculateOffsets();

	onCreate.Emit();
}

S::Void S::GUI::Window::OnDestroy()
{
	destroyed = True;
	visible	  = False;

	if (GetObjectType() != ToolWindow::classID) nOfActiveWindows--;
}

S::Void S::GUI::Window::OnMinimize()
{
	minimized = True;
}

S::Void S::GUI::Window::OnMaximize()
{
	maximized = True;
}

S::Void S::GUI::Window::OnRestore()
{
	if	(minimized) minimized = False;
	else if (maximized) maximized = False;
}

S::Int S::GUI::Window::Process(Int message, Int wParam, Int lParam)
{
	if (!created) return MessageUnknown;

	EnterProtectedRegion();

	/* Forward event to subscribers.
	 */
	onEvent.Emit(message, wParam, lParam);

	/* Process events we are interested in.
	 */
	Int	 rVal = MessageUnknown;

	switch (message)
	{
		case SM_GETFOCUS:
			if (!focussed)
			{
				focussed = True;

				onGetFocus.Emit();
			}

			break;
		case SM_LOSEFOCUS:
			if (focussed)
			{
				Window	*focusWnd = (Window *) Object::GetObject(wParam, Window::classID);

				if (focusWnd != NIL)
				{
					if (focusWnd->GetObjectType() == ToolWindow::classID && focusWnd->GetOrder() >= GetOrder())
					{
						LeaveProtectedRegion();

						return MessageProcessed;
					}
				}

				focussed = False;

				onLoseFocus.Emit();
			}

			break;
		case SM_WINDOWMETRICS:
			{
				Point	 nPos((unsigned(wParam) >> 16) - 32768, (unsigned(wParam) & 65535) - 32768);
				Size	 nSize((unsigned(lParam) >> 16) - 32768, (unsigned(lParam) & 65535) - 32768);

				Bool	 resized	= (GetSize() != nSize);
				Bool	 prevVisible	= visible;

				visible = False;

				Widget::SetMetrics(nPos, nSize);

				if (resized) CalculateOffsets();

				visible = prevVisible;

				rVal = MessageProcessed;
			}

			break;
		case SM_PAINT:
			updateRect = backend->GetUpdateRect();

			{
				Size	 realSize = GetRealSize();

				if (Math::Abs(updateRect.GetWidth() - realSize.cx) < 20 && Math::Abs(updateRect.GetHeight() - realSize.cy) < 20) Paint(SP_PAINT);
				else														 Paint(SP_UPDATE);

				updateRect = Rect(Point(0, 0), realSize);
			}

			rVal = MessageProcessed;

			break;
	}

	if (rVal != MessageUnknown)
	{
		LeaveProtectedRegion();

		return rVal;
	}

	/* Delegate event to main layer.
	 */
	if (mainLayer->Process(message, wParam, lParam) == MessageProcessed)
	{
		LeaveProtectedRegion();

		return MessageProcessed;
	}

	/* Delegate event to other widgets.
	 */
	for (Int i = GetNOfObjects() - 1; i >= 0; i--)
	{
		Widget	*object = GetNthObject(i);

		if (object == NIL || object == mainLayer) continue;

		if (object->Process(message, wParam, lParam) == MessageProcessed)
		{
			rVal = MessageProcessed;

			break;
		}
	}

	/* Invoke callback for application message handling.
	 */
	if (rVal == MessageUnknown) rVal = handleSystemMessage.Call(message, wParam, lParam);

	LeaveProtectedRegion();

	return rVal;
}

S::Int S::GUI::Window::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!created)	     return Success();
	if (!IsVisible())    return Success();

	if (updateRect.GetWidth() == 0 || updateRect.GetHeight() == 0) return Success();

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

#ifdef __WIN32__
	static Bool	 flatStyle = Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2);
#else
	static Bool	 flatStyle = False;
#endif

	Size	 realSize = GetRealSize();
	Float	 fontSize = surface->GetSurfaceDPI() / 96.0;

	if (updateRect.left < frameWidth)		  updateRect.left   = frameWidth - 1;
	if (updateRect.top  < frameWidth)		  updateRect.top    = frameWidth - 1;
	if (realSize.cx - updateRect.right  < frameWidth) updateRect.right  = realSize.cx - frameWidth + 1;
	if (realSize.cy - updateRect.bottom < frameWidth) updateRect.bottom = realSize.cy - frameWidth + 1;

	Rect	 workArea = System::Screen::GetVirtualScreenMetrics();

	if (message == SP_UPDATE				 &&
	    GetPosition().x		   > workArea.left   - 2 &&
	    GetPosition().y		   > workArea.top    - 2 &&
	    GetPosition().x + GetSize().cx < workArea.right  + 2 &&
	    GetPosition().y + GetSize().cy < workArea.bottom + 2)
	{
		surface->PaintRect(updateRect);
	}
	else
	{
		RightToLeftModifier	 rightToLeft;

		rightToLeft.SetSurfaceSize(surface->GetSize());
		rightToLeft.SetRightToLeft(IsRightToLeft());

		surface->StartPaint(rightToLeft.TranslateRect(updateRect));

		surface->Box(updateRect, Setup::BackgroundColor, Rect::Filled);

		if (type != ToolWindow::classID)
		{
			Widget	*lastTopWidget = NIL;
			Int	 topoffset = frameWidth;
			Int	 positions = 0;

			for (Int i = 0; i < GetNOfObjects(); i++)
			{
				Widget	*object = GetNthObject(i);

				positions |= object->GetOrientation();

				if (object->GetOrientation() == OR_TOP)
				{
					lastTopWidget = object;

					if (object->subtype == WO_SEPARATOR)
					{
						topoffset += object->GetSize().cy + 3;

						Point	 p1 = Point(frameWidth * fontSize, topoffset * fontSize - 2);
						Point	 p2 = Point(realSize.cx - frameWidth, p1.y);

#ifndef __APPLE__
						if (!flatStyle && icon != NIL) p1.x += Math::Round(18 * fontSize);
#endif

						surface->Bar(p1, p2, OR_HORZ);
					}
					else
					{
						topoffset += object->GetSize().cy;
					}
				}
			}

			if (positions & OR_TOP)
			{
				Point	 p1 = Point(frameWidth, topoffset * fontSize - 2);
				Point	 p2 = Point(realSize.cx - frameWidth, p1.y);

				if (lastTopWidget != NIL && lastTopWidget->subtype == WO_NOSEPARATOR)
				{
					p1.y += Math::Round(3 * surface->GetSurfaceDPI() / 96.0);
					p2.y += Math::Round(3 * surface->GetSurfaceDPI() / 96.0);
				}

				surface->Bar(p1, p2, OR_HORZ);
				surface->Bar(p1 + Point(0, 2), p2 + Point(0, 2), OR_HORZ);
			}

			if (positions & OR_BOTTOM)
			{
				Point	 p1 = Point(frameWidth, realSize.cy - innerOffset.bottom * fontSize + 1);
				Point	 p2 = Point(realSize.cx - frameWidth, p1.y);

				surface->Bar(p1, p2, OR_HORZ);
				surface->Bar(p1 + Point(0, 2), p2 + Point(0, 2), OR_HORZ);
			}

			if (positions & OR_LEFT)
			{
				Point	 p1 = Point(innerOffset.left * fontSize - 3, innerOffset.top);
				Point	 p2 = Point(p1.x, realSize.cy - innerOffset.bottom - 1);

				surface->Bar(p1, p2, OR_VERT);
			}

			if (positions & OR_RIGHT)
			{
				Point	 p1 = Point(realSize.cx - innerOffset.right * fontSize + 1, innerOffset.top);
				Point	 p2 = Point(p1.x, realSize.cy - innerOffset.bottom - 1);

				surface->Bar(p1, p2, OR_VERT);
			}
		}

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			Widget	*widget = GetNthObject(i);

			if (widget->GetObjectType() != Layer::classID && widget->IsAffected(updateRect)) widget->Paint(SP_PAINT);
		}

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			Widget	*widget = GetNthObject(i);

			if (widget->GetObjectType() == Layer::classID && widget->IsAffected(updateRect)) widget->Paint(SP_PAINT);
		}

		onPaint.Emit();

		surface->EndPaint();
	}

	LeaveProtectedRegion();

	return Success();
}

S::Void S::GUI::Window::CalculateOffsets()
{
	Widget	*lastTopWidget	= NIL;
	Int	 positions	= 0;

	innerOffset = Rect(Point(frameWidth, frameWidth), Size(0, 0));

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		positions |= widget->GetOrientation();

		if (widget->GetOrientation() == OR_TOP)
		{
			lastTopWidget = widget;

			widget->SetMetrics(Point(innerOffset.left, innerOffset.top), Size(GetWidth() - innerOffset.left - innerOffset.right, widget->GetHeight()));

			innerOffset.top += widget->GetHeight();

			if (widget->subtype == WO_SEPARATOR) innerOffset.top += 3;
		}
		else if (widget->GetOrientation() == OR_BOTTOM)
		{
			widget->SetMetrics(Point(innerOffset.left, GetHeight() - innerOffset.bottom - widget->GetHeight()), Size(GetWidth() - innerOffset.left - innerOffset.right, widget->GetHeight()));

			innerOffset.bottom += widget->GetHeight();
		}
	}

	if (positions & OR_TOP)	   innerOffset.top    += 3;
	if (positions & OR_BOTTOM) innerOffset.bottom += 5;

	if (lastTopWidget != NIL)  innerOffset.top    += (lastTopWidget->subtype == WO_NOSEPARATOR ? 3 : 0);

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		if (widget->GetOrientation() == OR_LEFT)
		{
			widget->SetMetrics(Point(innerOffset.left, innerOffset.top), Size(widget->GetWidth(), GetHeight() - innerOffset.top - innerOffset.bottom));

			innerOffset.left += widget->GetWidth();
		}
		else if (widget->GetOrientation() == OR_RIGHT)
		{
			widget->SetMetrics(Point(GetWidth() - innerOffset.right - widget->GetWidth(), innerOffset.top), Size(widget->GetWidth(), GetHeight() - innerOffset.top - innerOffset.bottom));

			innerOffset.right += widget->GetWidth();
		}
	}

	if (positions & OR_LEFT)  innerOffset.left  += 3;
	if (positions & OR_RIGHT) innerOffset.right += 3;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		if (widget->GetOrientation() == OR_CENTER)
		{
			widget->SetMetrics(Point(innerOffset.left, innerOffset.top), Size(GetWidth() - innerOffset.left - innerOffset.right, GetHeight() - innerOffset.top - innerOffset.bottom));
			widget->SetOrientation(OR_CENTER);
		}
	}
}

const S::GUI::Size &S::GUI::Window::GetSizeModifier() const
{
	return backend->GetSizeModifier();
}

S::GUI::Point S::GUI::Window::GetMousePosition() const
{
	Point	 position = Input::Pointer::GetPosition();

	if (IsRightToLeft()) position = Point(GetRealSize().cx - (position.x - GetX()) - 1, position.y - GetY());
	else		     position -= GetPosition();

	return position;
}

S::Bool S::GUI::Window::IsMouseOn(const Rect &rect) const
{
	Surface	*surface = GetDrawSurface();

	if (surface->GetSystemSurface() == NIL) return False;

	Point	 mousePos = GetMousePosition();

	if ((mousePos.x >= rect.left) && (mousePos.x < rect.right) && (mousePos.y >= rect.top) && (mousePos.y < rect.bottom))
	{
		if (Input::Pointer::GetPointerWindow() != this)	return False;
		else						return True;
	}

	return False;
}

S::GUI::Surface *S::GUI::Window::GetDrawSurface() const
{
	return backend->GetDrawSurface();
}

S::Void *S::GUI::Window::GetSystemWindow() const
{
	return backend->GetSystemWindow();
}

S::Int S::GUI::Window::Add(Widget *widget)
{
	if (widget == NIL) return Error();

	if (widget->GetOrientation() == OR_UPPERLEFT || widget->GetOrientation() == OR_UPPERRIGHT || widget->GetOrientation() == OR_LOWERLEFT || widget->GetOrientation() == OR_LOWERRIGHT)
	{
		if (mainLayer->Add(widget) == Success())
		{
			/* Enable drag & drop if the added widget is a target.
			 */
			if (IsDropTarget()) backend->EnableDropFiles(True);

			return Success();
		}

		return Error();
	}

	if (Widget::Add(widget) == Success())
	{
		/* Special handling for title and status bars.
		 */
		if (widget->GetObjectType() == Titlebar::classID)
		{
			if (!Binary::IsFlagSet(widget->GetFlags(), TB_MAXBUTTON)) flags |= WF_NORESIZE;

			if (widget->GetHeight() == 0) backend->SetSizeModifier(backend->GetSizeModifier() - Size(0, 19) * Surface().GetSurfaceDPI() / 96.0);
		}
		else if (widget->GetObjectType() == Statusbar::classID)
		{
			SetDefaultStatusText(widget->GetText());
		}

		/* Recalculate offsets unless the added widget has free orientation.
		 */
		if (widget->GetOrientation() != OR_FREE) CalculateOffsets();

		/* Enable drag & drop if the added widget is a target.
		 */
		if (IsDropTarget()) backend->EnableDropFiles(True);

		return Success();
	}

	return Error();
}

S::Int S::GUI::Window::Remove(Widget *widget)
{
	if (widget == NIL) return Error();

	if (widget->GetOrientation() == OR_UPPERLEFT || widget->GetOrientation() == OR_UPPERRIGHT || widget->GetOrientation() == OR_LOWERLEFT || widget->GetOrientation() == OR_LOWERRIGHT)
	{
		if (mainLayer->Remove(widget) == Success())
		{
			/* Disable drag & drop if the removed widget was a target.
			 */
			if (!IsDropTarget()) backend->EnableDropFiles(False);

			return Success();
		}

		return Error();
	}

	if (Widget::Remove(widget) == Success())
	{
		/* Recalculate offsets unless the removed widget had free orientation.
		 */
		if (widget->GetOrientation() != OR_FREE) CalculateOffsets();

		/* Disable drag & drop if the removed widget was a target.
		 */
		if (!IsDropTarget()) backend->EnableDropFiles(False);

		return Success();
	}

	return Error();
}

S::GUI::Window *S::GUI::Window::GetWindow(Void *sysWindow)
{
	if (sysWindow == NIL) return NIL;

	for (Int i = 0; i < windows.Length(); i++)
	{
		Window		*window = windows.GetNth(i);

		if (window->GetSystemWindow() == sysWindow) return window;
	}

	return NIL;
}
