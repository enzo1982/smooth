 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/xlib/windowxlib.h>
#include <smooth/gui/window/window.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>
#include <smooth/init.h>
#include <smooth/backends/xlib/backendxlib.h>

using namespace X11;

S::GUI::WindowBackend *CreateWindowXLib()
{
	return new S::GUI::WindowXLib();
}

S::Int	 windowXLibTmp = S::GUI::WindowBackend::SetBackend(&CreateWindowXLib);

S::Array<S::GUI::WindowXLib *, S::Void *>	 S::GUI::WindowXLib::windowBackends;

S::GUI::WindowXLib::WindowXLib(Void *iWindow)
{
	type	= WINDOW_XLIB;

	display	= Backends::BackendXLib::GetDisplay();
	im	= Backends::BackendXLib::GetIM();

	wnd	= NIL;
	oldwnd	= NIL;

	ic	= NIL;

	id	= windowBackends.Add(this);

	minSize	= Size(160, 24);

	flags	= 0;
}

S::GUI::WindowXLib::~WindowXLib()
{
	windowBackends.Remove(id);
}

S::Void S::GUI::WindowXLib::UpdateWMNormalHints()
{
	if (wnd == NIL) return;

	XSizeHints	 normal;

	normal.flags	   = (minSize != Size() ? PMinSize : 0) |
			     (maxSize != Size() ? PMaxSize : 0) |
			     (flags & WF_NORESIZE ? PPosition | PSize : 0);

	normal.x	   = pos.x;
	normal.y	   = pos.y;

	normal.width	   = size.cx;
	normal.height	   = size.cy;

	normal.min_width   = minSize.cx;
	normal.min_height  = minSize.cy;

	normal.max_width   = maxSize.cx;
	normal.max_height  = maxSize.cy;

	XSetWMNormalHints(display, wnd, &normal);
}

S::Void *S::GUI::WindowXLib::GetSystemWindow() const
{
	return (Void *) wnd;
}

S::GUI::WindowXLib *S::GUI::WindowXLib::GetWindowBackend(::Window wnd)
{
	if (wnd == 0) return NIL;

	for (Int i = 0; i < windowBackends.Length(); i++)
	{
		WindowXLib	*window = windowBackends.GetNth(i);

		if (window != NIL)
		{
			if (window->wnd    == wnd ||
			    window->oldwnd == wnd) return window;
		}
	}

	return NIL;
}

S::Key S::GUI::WindowXLib::ConvertKey(Int keySym)
{
	Key	 key = SK_OTHER;

	switch (keySym)
	{
		case XK_Left:	   key = SK_LEFT;    break;
		case XK_Up:	   key = SK_UP;	     break;
		case XK_Right:	   key = SK_RIGHT;   break;
		case XK_Down:	   key = SK_DOWN;    break;

		case XK_Home:	   key = SK_HOME;    break;
		case XK_End:	   key = SK_END;     break;
		case XK_Insert:	   key = SK_INSERT;  break;
		case XK_Delete:	   key = SK_DELETE;  break;
		case XK_Prior:	   key = SK_PRIOR;   break;
		case XK_Next:	   key = SK_NEXT;    break;

		case XK_Return:	   key = SK_RETURN;  break;
		case XK_BackSpace: key = SK_BACK;    break;
		case XK_Tab:	   key = SK_TAB;     break;

		case XK_space:	   key = SK_SPACE;   break;

		case XK_Shift_L:
		case XK_Shift_R:   key = SK_SHIFT;   break;

		case XK_Control_L:
		case XK_Control_R: key = SK_CONTROL; break;

		case XK_Alt_L:
		case XK_Alt_R:	   key = SK_ALT;     break;

		case XK_Escape:	   key = SK_ESCAPE;  break;

		case XK_F1:	   key = SK_F1;	     break;
		case XK_F2:	   key = SK_F2;	     break;
		case XK_F3:	   key = SK_F3;	     break;
		case XK_F4:	   key = SK_F4;	     break;
		case XK_F5:	   key = SK_F5;	     break;
		case XK_F6:	   key = SK_F6;	     break;
		case XK_F7:	   key = SK_F7;	     break;
		case XK_F8:	   key = SK_F8;	     break;
		case XK_F9:	   key = SK_F9;	     break;
		case XK_F10:	   key = SK_F10;     break;
		case XK_F11:	   key = SK_F11;     break;
		case XK_F12:	   key = SK_F12;     break;
		case XK_F13:	   key = SK_F13;     break;
		case XK_F14:	   key = SK_F14;     break;
		case XK_F15:	   key = SK_F15;     break;
		case XK_F16:	   key = SK_F16;     break;
		case XK_F17:	   key = SK_F17;     break;
		case XK_F18:	   key = SK_F18;     break;
		case XK_F19:	   key = SK_F19;     break;
		case XK_F20:	   key = SK_F20;     break;
		case XK_F21:	   key = SK_F21;     break;
		case XK_F22:	   key = SK_F22;     break;
		case XK_F23:	   key = SK_F23;     break;
		case XK_F24:	   key = SK_F24;     break;
	}

	if	(keySym >= '0' && keySym <= '9') key = (Key)  keySym;
	else if	(keySym >= 'A' && keySym <= 'Z') key = (Key)  keySym;
	else if	(keySym >= 'a' && keySym <= 'z') key = (Key) (keySym + ('A' - 'a'));

	return key;
}

S::Int S::GUI::WindowXLib::ProcessSystemMessages(XEvent *e)
{
	/* Process system events not relevant
	 * to portable Window implementation.
	 */
	switch (e->type)
	{
		case CreateNotify:
			onCreate.Emit();

			break;
		case DestroyNotify:
			onDestroy.Emit();

			oldwnd = NIL;

			break;
		case MapNotify:
			/* Grab the keyboard focus.
			 */
			XSetInputFocus(display, wnd, RevertToParent, CurrentTime);

			onEvent.Call(SM_PAINT, 0, 0);

			XFlush(display);

			break;
		case UnmapNotify:
			break;
		case SelectionRequest:
			if (selection != NIL)
			{
				XEvent	 respond;

				if (e->xselectionrequest.target == XA_STRING)
				{
					XChangeProperty(display, e->xselectionrequest.requestor, e->xselectionrequest.property, XA_STRING, 8, PropModeReplace, (unsigned char *) (char *) selection, selection.Length());

					respond.xselection.property = e->xselectionrequest.property;
				}
				else if (e->xselectionrequest.target == XA_UTF8_STRING(display))
				{
					XChangeProperty(display, e->xselectionrequest.requestor, e->xselectionrequest.property, XA_UTF8_STRING(display), 8, PropModeReplace, (unsigned char *) selection.ConvertTo("UTF-8"), strlen(selection.ConvertTo("UTF-8")));

					respond.xselection.property = e->xselectionrequest.property;
				}
				else
				{
					/* Strings only please!
					 */
					respond.xselection.property = None;
				}

				respond.xselection.type	     = SelectionNotify;
				respond.xselection.display   = e->xselectionrequest.display;
				respond.xselection.requestor = e->xselectionrequest.requestor;
				respond.xselection.selection = e->xselectionrequest.selection;
				respond.xselection.target    = e->xselectionrequest.target;
				respond.xselection.time	     = e->xselectionrequest.time;

				XSendEvent(display, e->xselectionrequest.requestor, 0, 0, &respond);
				XFlush(display);
			}

			break;
		case SelectionClear:
			selection = NIL;

			break;
	}

	/* Convert Xlib events to smooth messages.
	 */
	switch (e->type)
	{
		/* Mouse events:
		 */
		case MotionNotify:
			Input::Pointer::UpdatePosition(e->xmotion.x_root, e->xmotion.y_root);

			onEvent.Call(SM_MOUSEMOVE, 0, 0);

			break;
		case ButtonPress:
			Input::Pointer::UpdatePosition(e->xmotion.x_root, e->xmotion.y_root);

			if	(e->xbutton.button == Button1) onEvent.Call(SM_LBUTTONDOWN, 0, 0);
			else if (e->xbutton.button == Button3) onEvent.Call(SM_RBUTTONDOWN, 0, 0);
			else if (e->xbutton.button == Button4) onEvent.Call(SM_MOUSEWHEEL, 120, 0);
			else if (e->xbutton.button == Button5) onEvent.Call(SM_MOUSEWHEEL, -120, 0);

			/* Grab the keyboard focus if we don't have it already.
			 */
			if (wnd != NIL && e->xbutton.button <= 3)
			{
				::Window	 wndFocus;
				int		 revertTo;

				XGetInputFocus(display, &wndFocus, &revertTo);

				if (wndFocus != wnd) XSetInputFocus(display, wnd, RevertToParent, CurrentTime);
			}

			break;
		case ButtonRelease:
			Input::Pointer::UpdatePosition(e->xmotion.x_root, e->xmotion.y_root);

			if	(e->xbutton.button == Button1) onEvent.Call(SM_LBUTTONUP, 0, 0);
			else if (e->xbutton.button == Button3) onEvent.Call(SM_RBUTTONUP, 0, 0);

			break;

		/* Keyboard events:
		 */
		case KeymapNotify:
			XRefreshKeyboardMapping(&e->xmapping);

			break;
		case KeyPress:
			onEvent.Call(SM_KEYDOWN, ConvertKey(XKeycodeToKeysym(display, e->xkey.keycode, 0)), 0);

			/* Convert keyboard event to input string and
			 * call SM_CHAR event for each character.
			 */
			{
				Status	 status;
				char	 text[32];
				Int	 numChars = Xutf8LookupString(ic, &e->xkey, text, 32, NIL, &status);

				text[numChars] = 0;

				if (status & XLookupChars)
				{
					String	 string;

					string.ImportFrom("UTF-8", text);

					for (Int i = 0; i < string.Length(); i++) onEvent.Call(SM_CHAR, string[i], 0);
				}
			}

			break;
		case KeyRelease:
			onEvent.Call(SM_KEYUP, ConvertKey(XKeycodeToKeysym(display, e->xkey.keycode, 0)), 0);

			break;

		/* Paint events:
		 */
		case Expose:
			updateRect.left	  = Math::Min(updateRect.left, e->xexpose.x);
			updateRect.top	  = Math::Min(updateRect.top, e->xexpose.y);
			updateRect.right  = Math::Max(updateRect.right, e->xexpose.x + e->xexpose.width);
			updateRect.bottom = Math::Max(updateRect.bottom, e->xexpose.y + e->xexpose.height);

			if (e->xexpose.count == 0)
			{
				onEvent.Call(SM_PAINT, 0, 0);

				updateRect = Rect();
			}

			break;

		/* Window state change events:
		 */
		case ConfigureNotify:
			/* Get screen coordinates for window.
			 */
			{
				::Window	 child = 0;

				XTranslateCoordinates(display, wnd, RootWindow(display, DefaultScreen(display)), 0, 0, &e->xconfigure.x, &e->xconfigure.y, &child);
			}

			pos  = Point(e->xconfigure.x, e->xconfigure.y);
			size = Size(e->xconfigure.width, e->xconfigure.height);

			onEvent.Call(SM_WINDOWMETRICS, ((e->xconfigure.x + 32768) << 16) | (e->xconfigure.y + 32768), ((e->xconfigure.width + 32768) << 16) | (e->xconfigure.height + 32768));

			break;
		case FocusIn:
			onEvent.Call(SM_GETFOCUS, 0, 0);

			break;
		case FocusOut:
			/* Get the window that now has the focus.
			 */
			{
				::Window	 xWnd;
				int		 revertTo;

				XGetInputFocus(display, &xWnd, &revertTo);

				Window		*focusWnd = Window::GetWindow((Void *) xWnd);

				onEvent.Call(SM_LOSEFOCUS, focusWnd != NIL ? focusWnd->GetHandle() : -1, 0);
			}

			break;

		/* Window manager events:
		 */
		case ClientMessage:
			if (e->xclient.message_type == XInternAtom(display, "WM_PROTOCOLS", True))
			{
				if ((Atom) e->xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", True))
				{
					if (doClose.Call()) Close();
				}
			}

			break;
	}

	return Success();
}

S::Int S::GUI::WindowXLib::Open(const String &title, const Point &pos, const Size &size, Int iFlags)
{
	flags = iFlags;

	XSetWindowAttributes	 attributes;

	attributes.background_pixmap	= None;
	attributes.background_pixel	= None;
	attributes.border_pixmap	= None;
	attributes.border_pixel		= BlackPixel(display, 0);

	attributes.bit_gravity		= ForgetGravity;
	attributes.win_gravity		= NorthWestGravity;
	attributes.backing_store	= NotUseful;
	attributes.backing_planes	= 0;
	attributes.backing_pixel	= 0;

	if (flags & WF_NOTASKBUTTON && flags & WF_THINBORDER)
	{
		attributes.save_under		= True;
		attributes.override_redirect	= True;
	}
	else
	{
		attributes.save_under		= False;
		attributes.override_redirect	= False;
	}

	attributes.event_mask		 = 0;
	attributes.do_not_propagate_mask = 0;
	attributes.colormap		 = CopyFromParent;
	attributes.cursor		 = None;

	wnd = XCreateWindow(display, RootWindow(display, 0), pos.x, pos.y, size.cx, size.cy, 0, CopyFromParent, InputOutput, CopyFromParent, CWBorderPixel | CWOverrideRedirect | CWSaveUnder, &attributes);

	if (wnd != NIL)
	{
		ic = XCreateIC(im, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, wnd, NULL);

		/* Get mask of filter events for IC.
		 */
		long	 filterEvents = 0;

		XGetICValues(ic, XNFilterEvents, &filterEvents, NULL);

		/* Select event types we want to receive.
		 */
		XSelectInput(display, wnd, ExposureMask | FocusChangeMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask | filterEvents);

		/* Opt in to receive WM_DELETE_WINDOW messages.
		 */
		Atom	 atomKill = XInternAtom(display, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(display, wnd, &atomKill, 1);

		/* Process the CreateNotify event again, as GetWindowBackend
		 * cannot find the correct backend until wnd is set.
		 */
		XEvent	 e;

		e.type = CreateNotify;

		ProcessSystemMessages(&e);

		/* Create drawing surface.
		 */
		if (flags & WF_THINBORDER || flags & WF_NORESIZE) drawSurface = new Surface((Void *) wnd, size);
		else						  drawSurface = new Surface((Void *) wnd);

		drawSurface->SetSize(size);

		/* Set window title.
		 */
		SetTitle(title);

		/* Set minimum and maximum size.
		 */
		if (flags & WF_NORESIZE)
		{
			minSize = size;
			maxSize = size;
		}

		UpdateWMNormalHints();

		return Success();
	}

	return Error();
}

S::Int S::GUI::WindowXLib::Close()
{
	if (wnd == NIL) return Success();

	if (drawSurface != nullSurface) delete drawSurface;

	drawSurface = nullSurface;

	XDestroyIC(ic);
	XDestroyWindow(display, wnd);

	oldwnd	= wnd;
	wnd	= NIL;

	ic	= NIL;

	return Success();
}

S::Int S::GUI::WindowXLib::SetTitle(const String &nTitle)
{
	XTextProperty	 titleProp;
	const char	*title = nTitle.ConvertTo("UTF-8");

	Xutf8TextListToTextProperty(display, (char **) &title, 1, XUTF8StringStyle, &titleProp);

	XSetWMName(display, wnd, &titleProp);
	XSetWMIconName(display, wnd, &titleProp);

	return Success();
}

S::Int S::GUI::WindowXLib::SetMinimumSize(const Size &nMinSize)
{
	minSize = nMinSize;

	UpdateWMNormalHints();

	return Success();
}

S::Int S::GUI::WindowXLib::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize;

	UpdateWMNormalHints();

	return Success();
}

S::Int S::GUI::WindowXLib::Show()
{
	if (wnd == NIL) return Success();

	XMapRaised(display, wnd);
	XFlush(display);

	return Success();
}

S::Int S::GUI::WindowXLib::Hide()
{
	if (wnd == NIL) return Success();

	XUnmapWindow(display, wnd);
	XFlush(display);

	return Success();
}

S::Int S::GUI::WindowXLib::SetMetrics(const Point &nPos, const Size &nSize)
{
	if (nPos == pos && nSize == size) return Success();

	XMoveResizeWindow(display, wnd, nPos.x, nPos.y, nSize.cx, nSize.cy);
	XFlush(display);

	drawSurface->SetSize(nSize);

	return Success();
}

S::Int S::GUI::WindowXLib::Raise()
{
	if (wnd == NIL) return Success();

	XSetInputFocus(display, wnd, RevertToParent, CurrentTime);
	XFlush(display);

	return Success();
}
