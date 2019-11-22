 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/xlib/windowxlib.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/special/cursor.h>
#include <smooth/gui/application/application.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>
#include <smooth/misc/encoding/urlencode.h>
#include <smooth/io/instream.h>
#include <smooth/system/system.h>
#include <smooth/foreach.h>
#include <smooth/init.h>
#include <smooth/backends/xlib/backendxlib.h>

#include <stdio.h>
#include <stdlib.h>

using namespace X11;

S::GUI::WindowBackend *CreateWindowXLib()
{
	return new S::GUI::WindowXLib();
}

S::Int	 windowXLibTmp = S::GUI::WindowBackend::SetBackend(&CreateWindowXLib);

S::Int	 addWindowXLibInitTmp = S::AddInitFunction(&S::GUI::WindowXLib::Initialize);
S::Int	 addWindowXLibFreeTmp = S::AddFreeFunction(&S::GUI::WindowXLib::Free);

S::Array<S::GUI::WindowXLib *, S::Void *>	 S::GUI::WindowXLib::windowBackends;

S::GUI::Cursor	*S::GUI::WindowXLib::activeCursor = NIL;
S::GUI::Point	 S::GUI::WindowXLib::activeCursorPos;

namespace smooth
{
	namespace GUI
	{
		static int OnXIMPreeditStart(XIC ic, XPointer backend, XPointer dummy)
		{
			((WindowXLib *) backend)->OnXIMPreeditStart();

			return -1;
		}

		static void OnXIMPreeditDone(XIC ic, XPointer backend, XPointer dummy)
		{
			((WindowXLib *) backend)->OnXIMPreeditDone();
		}

		static void OnXIMPreeditDraw(XIC ic, XPointer backend, XIMPreeditDrawCallbackStruct *data)
		{
			((WindowXLib *) backend)->OnXIMPreeditDraw(data);
		}

		static void OnXIMPreeditCaret(XIC ic, XPointer backend, XIMPreeditCaretCallbackStruct *data)
		{
			((WindowXLib *) backend)->OnXIMPreeditCaret(data);
		}
	};
};

S::Int S::GUI::WindowXLib::Initialize()
{
	/* Register for cursor events.
	 */
	Cursor::internalSetCursor.Connect(&WindowXLib::SetCursor);
	Cursor::internalRemoveCursor.Connect(&WindowXLib::RemoveCursor);

	return Success();
}

S::Int S::GUI::WindowXLib::Free()
{
	/* Unregister cursor event handlers.
	 */
	Cursor::internalSetCursor.Disconnect(&WindowXLib::SetCursor);
	Cursor::internalRemoveCursor.Disconnect(&WindowXLib::RemoveCursor);

	return Success();
}

S::GUI::WindowXLib::WindowXLib(Void *iWindow)
{
	type		= WINDOW_XLIB;

	display		= Backends::BackendXLib::GetDisplay();
	im		= Backends::BackendXLib::GetIM();

	if (display == NIL)
	{
		fprintf(stderr, "Error: Unable to open display.\n");

		exit(EXIT_FAILURE);
	}

	wnd		= NIL;
	oldwnd		= NIL;

	ic		= NIL;
	iwnd		= NIL;

	id		= windowBackends.Add(this);

	minSize		= Size(160, 24);

	fontSize	= Surface().GetSurfaceDPI() / 96.0;

	flags		= 0;

	sysIcon		= NIL;
	sysIconSize	= 0;

	xdndTimeStamp	= 0;
	acceptDrop	= False;
	enableDropFiles	= False;
}

S::GUI::WindowXLib::~WindowXLib()
{
	if (sysIcon != NIL)
	{
		delete [] sysIcon;

		sysIcon	    = NIL;
		sysIconSize = 0;
	}

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

	normal.width	   = Math::Round(size.cx * fontSize) + sizeModifier.cx;
	normal.height	   = Math::Round(size.cy * fontSize) + sizeModifier.cy;

	normal.min_width   = Math::Round(minSize.cx * fontSize) + sizeModifier.cx;
	normal.min_height  = Math::Round(minSize.cy * fontSize) + sizeModifier.cy;

	normal.max_width   = Math::Round(maxSize.cx * fontSize) + sizeModifier.cx;
	normal.max_height  = Math::Round(maxSize.cy * fontSize) + sizeModifier.cy;

	XSetWMNormalHints(display, wnd, &normal);
}

S::Void *S::GUI::WindowXLib::GetSystemWindow() const
{
	return (Void *) wnd;
}

S::GUI::WindowXLib *S::GUI::WindowXLib::GetWindowBackend(X11::Window wnd)
{
	if (wnd == 0) return NIL;

	foreach (WindowXLib *window, windowBackends)
	{
		if (window == NIL) continue;

		if (window->wnd    == wnd ||
		    window->iwnd   == wnd ||
		    window->oldwnd == wnd) return window;
	}

	return NIL;
}

S::Input::Keyboard::Key S::GUI::WindowXLib::ConvertKey(Int keySym)
{
	Input::Keyboard::Key	 key = Input::Keyboard::KeyOther;

	switch (keySym)
	{
		case XK_Left:	   key = Input::Keyboard::KeyLeft;    break;
		case XK_Up:	   key = Input::Keyboard::KeyUp;      break;
		case XK_Right:	   key = Input::Keyboard::KeyRight;   break;
		case XK_Down:	   key = Input::Keyboard::KeyDown;    break;

		case XK_Home:	   key = Input::Keyboard::KeyHome;    break;
		case XK_End:	   key = Input::Keyboard::KeyEnd;     break;
		case XK_Insert:	   key = Input::Keyboard::KeyInsert;  break;
		case XK_Delete:	   key = Input::Keyboard::KeyDelete;  break;
		case XK_Prior:	   key = Input::Keyboard::KeyPrior;   break;
		case XK_Next:	   key = Input::Keyboard::KeyNext;    break;

		case XK_Return:	   key = Input::Keyboard::KeyReturn;  break;
		case XK_BackSpace: key = Input::Keyboard::KeyBack;    break;
		case XK_Tab:	   key = Input::Keyboard::KeyTab;     break;

		case XK_space:	   key = Input::Keyboard::KeySpace;   break;

		case XK_Shift_L:
		case XK_Shift_R:   key = Input::Keyboard::KeyShift;   break;

		case XK_Control_L:
		case XK_Control_R: key = Input::Keyboard::KeyControl; break;

		case XK_Alt_L:
		case XK_Alt_R:	   key = Input::Keyboard::KeyAlt;     break;

		case XK_Escape:	   key = Input::Keyboard::KeyEscape;  break;

		case XK_F1:	   key = Input::Keyboard::KeyF1;      break;
		case XK_F2:	   key = Input::Keyboard::KeyF2;      break;
		case XK_F3:	   key = Input::Keyboard::KeyF3;      break;
		case XK_F4:	   key = Input::Keyboard::KeyF4;      break;
		case XK_F5:	   key = Input::Keyboard::KeyF5;      break;
		case XK_F6:	   key = Input::Keyboard::KeyF6;      break;
		case XK_F7:	   key = Input::Keyboard::KeyF7;      break;
		case XK_F8:	   key = Input::Keyboard::KeyF8;      break;
		case XK_F9:	   key = Input::Keyboard::KeyF9;      break;
		case XK_F10:	   key = Input::Keyboard::KeyF10;     break;
		case XK_F11:	   key = Input::Keyboard::KeyF11;     break;
		case XK_F12:	   key = Input::Keyboard::KeyF12;     break;
		case XK_F13:	   key = Input::Keyboard::KeyF13;     break;
		case XK_F14:	   key = Input::Keyboard::KeyF14;     break;
		case XK_F15:	   key = Input::Keyboard::KeyF15;     break;
		case XK_F16:	   key = Input::Keyboard::KeyF16;     break;
		case XK_F17:	   key = Input::Keyboard::KeyF17;     break;
		case XK_F18:	   key = Input::Keyboard::KeyF18;     break;
		case XK_F19:	   key = Input::Keyboard::KeyF19;     break;
		case XK_F20:	   key = Input::Keyboard::KeyF20;     break;
		case XK_F21:	   key = Input::Keyboard::KeyF21;     break;
		case XK_F22:	   key = Input::Keyboard::KeyF22;     break;
		case XK_F23:	   key = Input::Keyboard::KeyF23;     break;
		case XK_F24:	   key = Input::Keyboard::KeyF24;     break;
	}

	if	(keySym >= '0' && keySym <= '9') key = (Input::Keyboard::Key)  keySym;
	else if	(keySym >= 'A' && keySym <= 'Z') key = (Input::Keyboard::Key)  keySym;
	else if	(keySym >= 'a' && keySym <= 'z') key = (Input::Keyboard::Key) (keySym + ('A' - 'a'));

	return key;
}

S::Int S::GUI::WindowXLib::ProcessSystemMessages(XEvent *e)
{
	static Int	 focusWndID = -1;

	static Atom	 mimeURIListAtom       = XInternAtom(display, "text/uri-list", False);

	static Atom	 wmProtocolsAtom       = XInternAtom(display, "WM_PROTOCOLS", False);
	static Atom	 wmDeleteWindowAtom    = XInternAtom(display, "WM_DELETE_WINDOW", False);

	static Atom	 wmPingAtom	       = XInternAtom(display, "_NET_WM_PING", False);

	static Atom	 xaTargets	       = XInternAtom(display, "TARGETS", True);
	static Atom	 xaClipboard	       = XInternAtom(display, "CLIPBOARD", True);
	static Atom	 xaUtf8String	       = XInternAtom(display, "UTF8_STRING", True);

	static Atom	 xdndEnterAtom	       = XInternAtom(display, "XdndEnter", False);
	static Atom	 xdndTypeListAtom      = XInternAtom(display, "XdndTypeList", False);
	static Atom	 xdndPositionAtom      = XInternAtom(display, "XdndPosition", False);
	static Atom	 xdndStatusAtom	       = XInternAtom(display, "XdndStatus", False);
	static Atom	 xdndDropAtom	       = XInternAtom(display, "XdndDrop", False);
	static Atom	 xdndLeaveAtom	       = XInternAtom(display, "XdndLeave", False);
	static Atom	 xdndFinishedAtom      = XInternAtom(display, "XdndFinished", False);

	static Atom	 xdndActionPrivateAtom = XInternAtom(display, "XdndActionPrivate", False);

	/* Lock application while processing messages.
	 */
	Application::Lock	 lock;

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
			/* Set internal focus window.
			 */
			focusWndID = id;

			/* Initialize window metrics here to handle X servers
			 * that do not send a ConfigureNotify to new windows.
			 */
			if (wnd == e->xany.window)
			{
				XWindowAttributes	 attributes;

				XGetWindowAttributes(display, wnd, &attributes);

				/* Get screen coordinates for window.
				 */
				X11::Window	 child = 0;

				XTranslateCoordinates(display, wnd, RootWindow(display, DefaultScreen(display)), 0, 0, &attributes.x, &attributes.y, &child);

				/* Set metrics and emit window metrics event.
				 */
				pos  =  Point(attributes.x, attributes.y);
				size = (Size(attributes.width, attributes.height) - sizeModifier) / fontSize;

				if (drawSurface != NIL) drawSurface->SetSize(Size(attributes.width, attributes.height));

				onEvent.Call(SM_WINDOWMETRICS, ((	     attributes.x		  + 32768) << 16) | (		 attributes.y		       + 32768),
							       ((Math::Round(attributes.width / fontSize) + 32768) << 16) | (Math::Round(attributes.height / fontSize) + 32768));

				onEvent.Call(SM_GETFOCUS, 0, 0);
			}

			break;

		case UnmapNotify:
			/* Clear internal focus window.
			 */
			if (focusWndID == id) focusWndID = -1;

			break;

		case SelectionRequest:
			{
				String	 text;

				if	(e->xselectionrequest.selection == XA_PRIMARY)  text = selection;
				else if (e->xselectionrequest.selection == xaClipboard) text = clipboard;

				if (text == NIL) break;

				XEvent	 respond;

				respond.xselection.type	     = SelectionNotify;
				respond.xselection.display   = e->xselectionrequest.display;
				respond.xselection.requestor = e->xselectionrequest.requestor;
				respond.xselection.selection = e->xselectionrequest.selection;
				respond.xselection.target    = e->xselectionrequest.target;
				respond.xselection.property  = e->xselectionrequest.property;
				respond.xselection.time	     = e->xselectionrequest.time;

				if (e->xselectionrequest.target == xaTargets)
				{
					Atom	 targets[] = { XA_STRING, xaUtf8String };

					XChangeProperty(display, e->xselectionrequest.requestor, e->xselectionrequest.property, XA_ATOM, 32, PropModeReplace, (unsigned char *) targets, 2);
				}
				else if (e->xselectionrequest.target == XA_STRING)
				{
					XChangeProperty(display, e->xselectionrequest.requestor, e->xselectionrequest.property, XA_STRING, 8, PropModeReplace, (unsigned char *) (char *) text, text.Length());
				}
				else if (e->xselectionrequest.target == xaUtf8String)
				{
					XChangeProperty(display, e->xselectionrequest.requestor, e->xselectionrequest.property, xaUtf8String, 8, PropModeReplace, (unsigned char *) text.ConvertTo("UTF-8"), strlen(text.ConvertTo("UTF-8")));
				}
				else
				{
					/* Strings only please!
					 */
					respond.xselection.property = None;
				}

				XSendEvent(display, e->xselectionrequest.requestor, 0, 0, &respond);
				XFlush(display);
			}

			break;

		case SelectionClear:
			if	(e->xselectionclear.selection == XA_PRIMARY)  selection = NIL;
			else if (e->xselectionclear.selection == xaClipboard) clipboard = NIL;

			break;
	}

	/* Check if window still matches the event.
	 */
	if (e->xany.window != wnd &&
	    e->xany.window != iwnd) return Error();

	/* Convert Xlib events to smooth messages.
	 */
	switch (e->type)
	{
		/* Mouse events:
		 */
		case MotionNotify:
			/* Update pointer position in Input::Pointer.
			 */
			Input::Pointer::UpdatePosition(Window::GetWindow((Void *) e->xmotion.window), e->xmotion.x_root, e->xmotion.y_root);

			onEvent.Call(SM_MOUSEMOVE, 0, 0);

			break;

		case EnterNotify:
			/* Update pointer position in Input::Pointer.
			 */
			Input::Pointer::UpdatePosition(Window::GetWindow((Void *) e->xcrossing.window), e->xcrossing.x_root, e->xcrossing.y_root);

			onEvent.Call(SM_MOUSEMOVE, 0, 0);

			break;

		case LeaveNotify:
			/* Update pointer position in Input::Pointer.
			 */
			Input::Pointer::UpdatePosition(NIL, e->xcrossing.x_root, e->xcrossing.y_root);

			onEvent.Call(SM_MOUSEMOVE, 0, 0);

			break;

		case ButtonPress:
			/* Update pointer position in Input::Pointer.
			 */
			Input::Pointer::UpdatePosition(Window::GetWindow((Void *) e->xbutton.window), e->xbutton.x_root, e->xbutton.y_root);

			/* Reject if a modal window is active.
			 */
			if (IsModalWindowActive()) break;

			/* Reject button messages if we do not have the focus and
			 * we are not an utility window.
			 *
			 * This usually means that some modal window is blocking.
			 */
			if (!(flags & WF_TOPMOST && flags & WF_NOTASKBUTTON && flags & WF_THINBORDER))
			{
				X11::Window	 focusWnd = None;
				int		 revertTo = RevertToNone;

				if (XGetInputFocus(display, &focusWnd, &revertTo) == 0)
				{
					if (focusWnd != wnd) break;
				}
			}

			/* Pass message to smooth window.
			 */
			{
				static XButtonEvent	 prevButtonEvent;
				static Bool		 prevButtonEventInitialized = False;

				if (!prevButtonEventInitialized)
				{
					prevButtonEvent.button = 0;
					prevButtonEvent.time   = 0;
					prevButtonEvent.x      = 0;
					prevButtonEvent.y      = 0;

					prevButtonEventInitialized = True;
				}

				if ((e->xbutton.button == Button1 ||
				     e->xbutton.button == Button2 ||
				     e->xbutton.button == Button3)		     &&
				    e->xbutton.button == prevButtonEvent.button	     &&
				    e->xbutton.time   <= prevButtonEvent.time + 500  &&
				    Math::Abs(e->xbutton.x - prevButtonEvent.x) <= 5 &&
				    Math::Abs(e->xbutton.y - prevButtonEvent.y) <= 5)
				{
					if	(e->xbutton.button == Button1) onEvent.Call(SM_LBUTTONDBLCLK, 0, 0);
					else if (e->xbutton.button == Button2) onEvent.Call(SM_MBUTTONDBLCLK, 0, 0);
					else if (e->xbutton.button == Button3) onEvent.Call(SM_RBUTTONDBLCLK, 0, 0);
				}
				else
				{
					if	(e->xbutton.button == Button1) onEvent.Call(SM_LBUTTONDOWN, 0, 0);
					else if (e->xbutton.button == Button2) onEvent.Call(SM_MBUTTONDOWN, 0, 0);
					else if (e->xbutton.button == Button3) onEvent.Call(SM_RBUTTONDOWN, 0, 0);
					else if (e->xbutton.button == Button4) onEvent.Call(SM_MOUSEWHEEL, 120, 0);
					else if (e->xbutton.button == Button5) onEvent.Call(SM_MOUSEWHEEL, -120, 0);
				}

				prevButtonEvent = e->xbutton;
			}

			/* Grab the keyboard focus if we don't have it already.
			 */
			if (wnd != NIL && focusWndID != id && e->xbutton.button <= 3)
			{
				WindowXLib	*focusWnd = windowBackends.Get(focusWndID);

				if (focusWnd != NIL) focusWnd->onEvent.Call(SM_LOSEFOCUS, Window::GetWindow((Void *) wnd)->GetHandle(), 0);

				focusWndID = id;

				onEvent.Call(SM_GETFOCUS, 0, 0);
			}

			/* Send an event to update widget under cursor if necessary.
			 */
			onEvent.Call(SM_MOUSEMOVE, 0, 0);

			break;

		case ButtonRelease:
			/* Update pointer position in Input::Pointer.
			 */
			Input::Pointer::UpdatePosition(Window::GetWindow((Void *) e->xbutton.window), e->xbutton.x_root, e->xbutton.y_root);

			/* Pass message to smooth window.
			 */
			if	(e->xbutton.button == Button1) onEvent.Call(SM_LBUTTONUP, 0, 0);
			else if (e->xbutton.button == Button2) onEvent.Call(SM_MBUTTONUP, 0, 0);
			else if (e->xbutton.button == Button3) onEvent.Call(SM_RBUTTONUP, 0, 0);

			break;

		/* Keyboard events:
		 */
		case KeymapNotify:
			XRefreshKeyboardMapping(&e->xmapping);

			break;

		case KeyPress:
			Input::Keyboard::UpdateKeyState(ConvertKey(XkbKeycodeToKeysym(display, e->xkey.keycode, 0, 0)), True);

			onEvent.Call(SM_KEYDOWN, ConvertKey(XkbKeycodeToKeysym(display, e->xkey.keycode, 0, 0)), 0);

			/* Convert keyboard event to input string and
			 * call SM_CHAR event for each character.
			 */
			if (ic != NIL)
			{
				Status	 status;
				char	 text[32];
				Int	 numChars = Xutf8LookupString(ic, &e->xkey, text, 32, NIL, &status);

				text[numChars] = 0;

				if (status == XLookupChars)
				{
					String	 string;

					string.ImportFrom("UTF-8", text);

					for (Int i = 0; i < string.Length(); i++) onEvent.Call(SM_CHAR, string[i], 0);
				}
			}
			else
			{
				char	 text[32];
				Int	 numChars = XLookupString(&e->xkey, text, 32, NIL, NIL);

				for (Int i = 0; i < numChars; i++) onEvent.Call(SM_CHAR, text[i], 0);
			}

			break;

		case KeyRelease:
			Input::Keyboard::UpdateKeyState(ConvertKey(XkbKeycodeToKeysym(display, e->xkey.keycode, 0, 0)), False);

			onEvent.Call(SM_KEYUP, ConvertKey(XkbKeycodeToKeysym(display, e->xkey.keycode, 0, 0)), 0);

			break;

		/* Paint events:
		 */
		case Expose:
			/* Fill update rect.
			 */
			if (updateRect == Rect())
			{
				updateRect.left	  = e->xexpose.x;
				updateRect.top	  = e->xexpose.y;
				updateRect.right  = e->xexpose.x + e->xexpose.width;
				updateRect.bottom = e->xexpose.y + e->xexpose.height;
			}
			else
			{
				updateRect.left	  = Math::Min(updateRect.left, e->xexpose.x);
				updateRect.top	  = Math::Min(updateRect.top, e->xexpose.y);
				updateRect.right  = Math::Max(updateRect.right, e->xexpose.x + e->xexpose.width);
				updateRect.bottom = Math::Max(updateRect.bottom, e->xexpose.y + e->xexpose.height);
			}

			/* Emit paint event if this was the
			 * last queued expose notification.
			 */
			if (e->xexpose.count == 0)
			{
				onEvent.Call(SM_PAINT, 0, 0);

				updateRect = Rect();
			}

			break;

		/* Window state change events:
		 */
		case ConfigureNotify:
			/* Process window metrics changes.
			 */
			{
				static Size	 prevSize;

				/* Get screen coordinates for window.
				 */
				if (wnd != NIL && !e->xconfigure.send_event)
				{
					X11::Window	 child = 0;

					XTranslateCoordinates(display, wnd, RootWindow(display, DefaultScreen(display)), 0, 0, &e->xconfigure.x, &e->xconfigure.y, &child);
				}

				/* Set metrics and emit window metrics event.
				 */
				pos  =  Point(e->xconfigure.x, e->xconfigure.y);
				size = (Size(e->xconfigure.width, e->xconfigure.height) - sizeModifier) / fontSize;

				if (drawSurface != NIL) drawSurface->SetSize(Size(e->xconfigure.width, e->xconfigure.height));

				onEvent.Call(SM_WINDOWMETRICS, ((	     e->xconfigure.x		     + 32768) << 16) | (	    e->xconfigure.y		     + 32768),
							       ((Math::Round(e->xconfigure.width / fontSize) + 32768) << 16) | (Math::Round(e->xconfigure.height / fontSize) + 32768));

				/* Update window rect and emit paint event if necessary.
				 */
				if (size != prevSize)
				{
					updateRect.left	  = Math::Min(updateRect.left, 0);
					updateRect.top	  = Math::Min(updateRect.top, 0);
					updateRect.right  = Math::Max(updateRect.right, e->xconfigure.width);
					updateRect.bottom = Math::Max(updateRect.bottom, e->xconfigure.height);

					onEvent.Call(SM_PAINT, 0, 0);

					updateRect = Rect();
				}

				prevSize = size;
			}

			break;

		case FocusIn:
			focusWndID = id;

			onEvent.Call(SM_GETFOCUS, 0, 0);

			break;

		case FocusOut:
			Input::Keyboard::ResetKeyState();

			/* Get the window that now has the focus.
			 */
			{
				X11::Window	 focusWnd = None;
				int		 revertTo = RevertToNone;

				if (XGetInputFocus(display, &focusWnd, &revertTo) == 0)
				{
					Window	*window = Window::GetWindow((Void *) focusWnd);

					onEvent.Call(SM_LOSEFOCUS, window != NIL ? window->GetHandle() : -1, 0);
				}
			}

			break;

		/* Client message events:
		 */
		case ClientMessage:
			/* Handle window manager request.
			 */
			if (e->xclient.message_type == wmProtocolsAtom)
			{
				if ((Atom) e->xclient.data.l[0] == wmDeleteWindowAtom)
				{
					if (doClose.Call()) Close();
				}
				else if ((Atom) e->xclient.data.l[0] == wmPingAtom)
				{
					e->xclient.window = RootWindow(display, DefaultScreen(display));

					XSendEvent(display, e->xclient.window, 0, SubstructureNotifyMask | SubstructureRedirectMask, e);
					XFlush(display);
				}
			}

			/* Handle XdndEnter message.
			 */
			if (e->xclient.message_type == xdndEnterAtom)
			{
				/* Check if the dropped item is a file.
				 */
				X11::Window	 sourceWnd = e->xclient.data.l[0];

				if (e->xclient.data.l[1] & 1)
				{
					Int		 typesRead	= 0;
					unsigned long	 bytesRemaining = 4;

					while (bytesRemaining)
					{
						Atom		 actualType;
						int		 actualFormat;
						unsigned long	 actualItems;
						Atom		*xdndType;

						XGetWindowProperty(display, sourceWnd, xdndTypeListAtom, typesRead++, 1, False, XA_ATOM, &actualType, &actualFormat, &actualItems, &bytesRemaining, (unsigned char **) &xdndType);

						if (*xdndType == mimeURIListAtom) acceptDrop = True;

						XFree(xdndType);
					}
				}
				else if ((Atom) e->xclient.data.l[2] == mimeURIListAtom ||
					 (Atom) e->xclient.data.l[3] == mimeURIListAtom ||
					 (Atom) e->xclient.data.l[4] == mimeURIListAtom)
				{
					acceptDrop = True;
				}
			}

			/* Handle XdndPosition message.
			 */
			if (e->xclient.message_type == xdndPositionAtom)
			{
				/* Send XdndStatus message.
				 */
				XEvent		 status;
				X11::Window	 sourceWnd = e->xclient.data.l[0];

				status.xclient.type	    = ClientMessage;
				status.xclient.display	    = display;
				status.xclient.window	    = sourceWnd;
				status.xclient.message_type = xdndStatusAtom;
				status.xclient.format	    = 32;

				status.xclient.data.l[0]    = wnd;
				status.xclient.data.l[1]    = acceptDrop;
				status.xclient.data.l[2]    = (pos.x   << 16) | pos.y;
				status.xclient.data.l[3]    = (size.cx << 16) | size.cy;
				status.xclient.data.l[4]    = xdndActionPrivateAtom;

				XSendEvent(display, sourceWnd, 0, 0, &status);
				XFlush(display);
			}

			/* Handle XdndDrop message.
			 */
			if (e->xclient.message_type == xdndDropAtom)
			{
				/* Are we interested at all?
				 */
				if (acceptDrop)
				{
					Point	 position = Window::GetWindow((Void *) wnd)->GetMousePosition();

					xdndTimeStamp = e->xclient.data.l[2];

					onEvent.Call(SM_DROPFILES, position.x, position.y);
				}

				/* Send XdndFinished message.
				 */
				XEvent		 finished;
				X11::Window	 sourceWnd = e->xclient.data.l[0];

				finished.xclient.type	      = ClientMessage;
				finished.xclient.display      = display;
				finished.xclient.window	      = sourceWnd;
				finished.xclient.message_type = xdndFinishedAtom;
				finished.xclient.format	      = 32;

				finished.xclient.data.l[0]    = wnd;
				finished.xclient.data.l[1]    = acceptDrop;
				finished.xclient.data.l[2]    = xdndActionPrivateAtom;

				XSendEvent(display, sourceWnd, 0, 0, &finished);
				XFlush(display);
			}

			/* Clean up after XdndDrop or on XdndLeave message.
			 */
			if (e->xclient.message_type == xdndDropAtom ||
			    e->xclient.message_type == xdndLeaveAtom)
			{
				xdndTimeStamp = 0;
				acceptDrop    = False;
			}

			break;
	}

	return Success();
}

S::Int S::GUI::WindowXLib::Open(const String &title, const Point &pos, const Size &size, Int iFlags)
{
	static Atom	 wmDeleteWindowAtom    = XInternAtom(display, "WM_DELETE_WINDOW", False);

	static Atom	 wmPingAtom	       = XInternAtom(display, "_NET_WM_PING", False);
	static Atom	 wmIconAtom	       = XInternAtom(display, "_NET_WM_ICON", False);

	static Atom	 windowStateAtom       = XInternAtom(display, "_NET_WM_STATE", False);
	static Atom	 windowStateModalAtom  = XInternAtom(display, "_NET_WM_STATE_MODAL", False);

	static Atom	 windowTypeAtom	       = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
	static Atom	 windowTypeNormalAtom  = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	static Atom	 windowTypeDialogAtom  = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
	static Atom	 windowTypeUtilityAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", False);

	flags = iFlags;

	XSetWindowAttributes	 attributes;

	attributes.background_pixel  = Setup::BackgroundColor;
	attributes.bit_gravity	     = NorthWestGravity;
	attributes.save_under	     = False;
	attributes.override_redirect = False;

	if (flags & WF_NOTITLE	   ) attributes.override_redirect = True;
	if (flags & WF_TOPMOST	    &&
	    flags & WF_NOTASKBUTTON &&
	    flags & WF_THINBORDER  ) attributes.save_under = True;

	wnd = XCreateWindow(display, RootWindow(display, 0), pos.x, pos.y, Math::Round(size.cx * fontSize) + sizeModifier.cx, Math::Round(size.cy * fontSize) + sizeModifier.cy, 0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWBitGravity | CWSaveUnder | CWOverrideRedirect, &attributes);

	if (wnd != NIL)
	{
		/* Create input window.
		 */
		if (im != NIL)
		{
			XLockDisplay(display);

			iwnd = XCreateWindow(display, wnd, 0, 0, 1, 1, 0, CopyFromParent, InputOnly, CopyFromParent, 0, NIL);

			XMapWindow(display, iwnd);

			XUnlockDisplay(display);
		}

		/* Select event types we want to receive.
		 */
		XSelectInput(display, wnd, ExposureMask | FocusChangeMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask);

		/* Opt in to the WM_DELETE_WINDOW and _NET_WM_PING protocols.
		 */
		Atom	 protocols[2] = { wmDeleteWindowAtom, wmPingAtom };

		XSetWMProtocols(display, wnd, protocols, 2);

		/* Process the CreateNotify event again, as GetWindowBackend
		 * cannot find the correct backend until wnd is set.
		 */
		XEvent	 e;

		e.type = CreateNotify;

		ProcessSystemMessages(&e);

		/* Create drawing surface.
		 */
		if ((flags & WF_THINBORDER) || (flags & WF_NORESIZE)) drawSurface = new Surface((Void *) wnd, size * fontSize + sizeModifier);
		else						      drawSurface = new Surface((Void *) wnd);

		drawSurface->SetSize(size * fontSize + sizeModifier);

		/* Set application name.
		 */
		for (Int i = 0; i < Object::GetNOfObjects(); i++)
		{
			if (Object::GetNthObject(i)->GetObjectType() != Application::classID) continue;

			Application	*application = (Application *) Object::GetNthObject(i);
			XClassHint	*classHint   = XAllocClassHint();

			classHint->res_name  = application->GetText();
			classHint->res_class = application->GetText();

			XSetClassHint(display, wnd, classHint);
			XFree(classHint);

			break;
		}

		/* Set window title.
		 */
		SetTitle(title);

		/* Set icon.
		 */
		if (sysIcon != NIL)
		{
			XChangeProperty(display, wnd, wmIconAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) sysIcon, sysIconSize);
		}

		/* Set minimum and maximum size.
		 */
		if (flags & WF_NORESIZE)
		{
			minSize = size;
			maxSize = size;
		}

		UpdateWMNormalHints();

		/* Configure window type.
		 */
		if ((flags & WF_TOPMOST) && (flags & WF_NOTASKBUTTON) && (flags & WF_THINBORDER))
		{
			XChangeProperty(display, wnd, windowTypeAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *) &windowTypeUtilityAtom, 1);
		}
		else if (flags & WF_MODAL)
		{
			XChangeProperty(display, wnd, windowTypeAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *) &windowTypeDialogAtom, 1);
			XChangeProperty(display, wnd, windowStateAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *) &windowStateModalAtom, 1);
		}
		else
		{
			XChangeProperty(display, wnd, windowTypeAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *) &windowTypeNormalAtom, 1);
		}

		/* Set transient hint.
		 */
		Widget	*container = Window::GetWindow((Void *) wnd)->GetContainer();

		if (container != NIL)
		{
			Window		*parent = container->GetContainerWindow();
			WindowXLib	*leader = FindLeaderWindow();

			if	(parent != NIL) XSetTransientForHint(display, wnd, (X11::Window) parent->GetSystemWindow());
			else if (leader != NIL) XSetTransientForHint(display, wnd, leader->wnd);
		}

		/* Enable dropping files if requested.
		 */
		if (enableDropFiles) EnableDropFiles(True);

		return Success();
	}

	return Error();
}

S::Int S::GUI::WindowXLib::Close()
{
	if (wnd == NIL) return Success();

	/* Delete surface.
	 */
	if (drawSurface != NIL) delete drawSurface;

	drawSurface = NIL;

	/* Destroy input context.
	 */
	DestroyInputContext();

	/* Destroy input window.
	 */
	if (iwnd != NIL)
	{
		XDestroyWindow(display, iwnd);

		iwnd = NIL;
	}

	/* Destroy window.
	 */
	XDestroyWindow(display, wnd);

	oldwnd	= wnd;
	wnd	= NIL;

	return Success();
}

S::Int S::GUI::WindowXLib::RequestClose()
{
	if (doClose.Call()) return Close();

	return Success();
}

S::GUI::WindowXLib *S::GUI::WindowXLib::FindLeaderWindow()
{
	/* The leader window is the newest non topmost window.
	 */
	foreachreverse (WindowXLib *backend, windowBackends)
	{
		if (  backend->id    <	id  &&
		      backend->wnd   != NIL &&
		    !(backend->flags &	WF_TOPMOST)) return backend;
	}

	return NIL;
}

S::Bool S::GUI::WindowXLib::IsModalWindowActive()
{
	/* Look for modal windows opened after ourselves.
	 */
	foreachreverse (WindowXLib *backend, windowBackends)
	{
		if	(backend == this)				   return False;
		else if (backend->wnd != NIL && backend->flags & WF_MODAL) return True;
	}

	return False;
}

S::Int S::GUI::WindowXLib::SetTitle(const String &nTitle)
{
	if (wnd == NIL) return Error();

	static Atom	 wmNameAtom	= XInternAtom(display, "_NET_WM_NAME", False);
	static Atom	 wmIconNameAtom = XInternAtom(display, "_NET_WM_ICON_NAME", False);

	XTextProperty	 titleProp;
	XTextProperty	 titlePropUTF8;

	const char	*title	   = nTitle.ConvertTo("ISO-8859-1");
	const char	*titleUTF8 = nTitle.ConvertTo("UTF-8");

	/* Set ISO encoded window name.
	 */
	if (XStringListToTextProperty((char **) &title, 1, &titleProp) != 0)
	{
		XSetWMName(display, wnd, &titleProp);
		XSetWMIconName(display, wnd, &titleProp);

		XFree(titleProp.value);
	}

	/* Set UTF-8 encoded window name.
	 */
	if (Xutf8TextListToTextProperty(display, (char **) &titleUTF8, 1, XUTF8StringStyle, &titlePropUTF8) == 0)
	{
		XSetTextProperty(display, wnd, &titlePropUTF8, wmNameAtom);
		XSetTextProperty(display, wnd, &titlePropUTF8, wmIconNameAtom);

		XFree(titlePropUTF8.value);
	}

	return Success();
}

S::Int S::GUI::WindowXLib::SetIcon(const Bitmap &newIcon)
{
	if (sysIcon != NIL)
	{
		delete [] sysIcon;

		sysIcon	    = NIL;
		sysIconSize = 0;
	}

	Int	 index = 0;
	Size	 size  = newIcon.GetSize();
	Int	 depth = newIcon.GetDepth();

	sysIconSize = 2 + size.cx * size.cy;
	sysIcon	    = new UnsignedLong [sysIconSize];

	sysIcon[index++] = size.cx;
	sysIcon[index++] = size.cy;

	Color	 backgroundColor16 = ((Setup::BackgroundColor.GetBlue() >> 3) << 11) | ((Setup::BackgroundColor.GetGreen() >> 2) << 5) | (Setup::BackgroundColor.GetRed() >> 3);
	Color	 backgroundColor24 = Setup::BackgroundColor;

	XImage	*image = (XImage *) newIcon.GetSystemBitmap();

	for (Int y = 0; y < size.cy; y++)
	{
		for (Int x = 0; x < size.cx; x++)
		{
			Long	 value = XGetPixel(image, x, y);

			if	(depth == 16) sysIcon[index++] = (value == backgroundColor16 ? 0 : 255) << 24 | (((value >> 11) & 0x1F) << 19) | (((value >> 5) & 0x3F) << 10) | ((value & 0x1F) << 3);
			else if (depth == 24) sysIcon[index++] = (value == backgroundColor24 ? 0 : 255) << 24 | value;
			else if (depth == 32) sysIcon[index++] = 						value;
		}
	}

	return Success();
}

S::Int S::GUI::WindowXLib::EnableDropFiles(Bool nEnableDropFiles)
{
	enableDropFiles = nEnableDropFiles;

	if (wnd == NIL) return Success();

	static Atom	 xdndAwareAtom = XInternAtom(display, "XdndAware", False);

	Int32	 xdndVersion = 5;

	if (enableDropFiles) XChangeProperty(display, wnd, xdndAwareAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *) &xdndVersion, 1);
	else		     XDeleteProperty(display, wnd, xdndAwareAtom);

	return Success();
}

const S::Array<S::String> &S::GUI::WindowXLib::GetDroppedFiles() const
{
	if (xdndTimeStamp == 0) return WindowBackend::GetDroppedFiles();

	static Atom	 mimeURIListAtom   = XInternAtom(display, "text/uri-list", False);

	static Atom	 xdndSelectionAtom = XInternAtom(display, "XdndSelection", False);

	static Array<String>	 fileNames;

	fileNames.RemoveAll();

	XConvertSelection(display, xdndSelectionAtom, mimeURIListAtom, XA_PRIMARY, wnd, xdndTimeStamp);
	XFlush(display);

	/* Wait for SelectionNotify event to be sent.
	 */
	XEvent	 event;

	do
	{
		XNextEvent(display, &event);

		WindowXLib	*backend = GUI::WindowXLib::GetWindowBackend(event.xany.window);

		if (backend != NIL) backend->ProcessSystemMessages(&event);
	}
	while (event.type != SelectionNotify);

	Atom		 type;
	int		 format;
	unsigned long	 items, bytes;
	unsigned char	*data = NIL;

	/* Do not get any data yet, see how much data is there.
	 */
	if (XGetWindowProperty(display, wnd, XA_PRIMARY, 0, 0, 0, AnyPropertyType, &type, &format, &items, &bytes, &data) == 0)
	{
		if (data != NIL) { XFree(data); data = NIL; }

		/* Data is there!
		 */
		if (bytes > 0) XGetWindowProperty(display, wnd, XA_PRIMARY, 0, bytes, 0, AnyPropertyType, &type, &format, &items, &bytes, &data);
	}

	/* Query data.
	 */
	IO::InStream	 in(IO::STREAM_BUFFER, data, strlen((char *) data) + 1);

	while (in.GetPos() < in.Size())
	{
		String	 line = in.InputLine();

		if (line.StartsWith("#")) continue;

		if (line.StartsWith("file://"))
		{
			String	 host = line.SubString(7, line.Tail(line.Length() - 7).Find("/"));

			fileNames.Add(Encoding::URLEncode::Decode(line.Tail(line.Length() - 7 - host.Length())));
		}
	}

	in.Close();

	XFree(data);

	return fileNames;
}

S::Int S::GUI::WindowXLib::SetMinimumSize(const Size &nMinSize)
{
	minSize = nMinSize;

	UpdateWMNormalHints();

	if (wnd != NIL)
	{
		XResizeWindow(display, wnd, Math::Round(Math::Max(size.cx, minSize.cx) * fontSize) + sizeModifier.cx,
					    Math::Round(Math::Max(size.cy, minSize.cy) * fontSize) + sizeModifier.cy);
		XFlush(display);
	}

	return Success();
}

S::Int S::GUI::WindowXLib::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize;

	UpdateWMNormalHints();

	if (wnd != NIL)
	{
		XResizeWindow(display, wnd, Math::Round(Math::Min(size.cx, maxSize.cx) * fontSize) + sizeModifier.cx,
					    Math::Round(Math::Min(size.cy, maxSize.cy) * fontSize) + sizeModifier.cy);
		XFlush(display);
	}

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
	if (wnd == NIL) return Success();

	XMoveResizeWindow(display, wnd, nPos.x, nPos.y, Math::Round(nSize.cx * fontSize) + sizeModifier.cx, Math::Round(nSize.cy * fontSize) + sizeModifier.cy);
	XFlush(display);

	return Success();
}

S::Int S::GUI::WindowXLib::Raise()
{
	if (wnd == NIL) return Success();

	/* FixMe: Are there cases where we need to do anything here?
	 */
	XFlush(display);

	return Success();
}

S::Void S::GUI::WindowXLib::OnXIMPreeditStart()
{
}

S::Void S::GUI::WindowXLib::OnXIMPreeditDone()
{
}

S::Void S::GUI::WindowXLib::OnXIMPreeditDraw(XIMPreeditDrawCallbackStruct *data)
{
	if (activeCursor == NIL) return;

	/* Get preedit string if any.
	 */
	String	 string;

	if (data->text != NIL)
	{
		if (data->text->encoding_is_wchar) string = data->text->string.wide_char;
		else				   string.ImportFrom("UTF-8", data->text->string.multi_byte);
	}

	/* Draw string at cursor position.
	 */
	Rect	 rect = Rect::OverlapRect(Rect(activeCursorPos - Point(1, 1), activeCursor->GetRealSize()), Rect(activeCursor->GetRealPosition(), activeCursor->GetRealSize()));

	activeCursor->SetIMECursor(True);

	drawSurface->StartPaint(rect);
	drawSurface->Box(rect, activeCursor->GetBackgroundColor(), Rect::Filled);

	if (data->text != NIL)
	{
		for (Int i = 0; i < data->text->length; i++)
		{
			Font	 font = activeCursor->GetFont();

			if (data->text->feedback != NIL)
			{
				if (data->text->feedback[i] & XIMUnderline) font.SetStyle(Font::Underline);
			}

			drawSurface->SetText(string.SubString(i, 1), rect + Point(font.GetScaledTextSizeX(string.Head(i)), 0), font);
		}
	}

	drawSurface->EndPaint();

	activeCursor->SetIMEAdvance(activeCursor->GetFont().GetScaledTextSizeX(string.Head(data->caret)));
	activeCursor->SetIMECursor(False);
}

S::Void S::GUI::WindowXLib::OnXIMPreeditCaret(XIMPreeditCaretCallbackStruct *data)
{
}

S::Void S::GUI::WindowXLib::CreateInputContext()
{
	XLockDisplay(display);

	/* Set up callbacks.
	 */
	XIMCallback	 cbPeStart  = { (XPointer) this, (XIMProc) S::GUI::OnXIMPreeditStart };
	XIMCallback	 cbPeDone   = { (XPointer) this, (XIMProc) S::GUI::OnXIMPreeditDone  };
	XIMCallback	 cbPeDraw   = { (XPointer) this, (XIMProc) S::GUI::OnXIMPreeditDraw  };
	XIMCallback	 cbPeCaret  = { (XPointer) this, (XIMProc) S::GUI::OnXIMPreeditCaret };

	XVaNestedList	 preeditCbs = XVaCreateNestedList(0, XNPreeditStartCallback, &cbPeStart,
							     XNPreeditDoneCallback,  &cbPeDone,
							     XNPreeditDrawCallback,  &cbPeDraw,
							     XNPreeditCaretCallback, &cbPeCaret, NULL);

	/* Create input context.
	 */
	ic = XCreateIC(im, XNClientWindow,	wnd,
			   XNInputStyle,	XIMPreeditCallbacks | XIMStatusNothing,
			   XNPreeditAttributes,	preeditCbs, NULL);

	XFree(preeditCbs);

	/* Configure input context.
	 */
	if (ic != NIL)
	{
		/* Set focus window.
		 */
		XSetICValues(ic, XNFocusWindow, iwnd, NULL);

		/* Get mask of filter events for IC.
		 */
		long	 filterEvents = 0;

		XGetICValues(ic, XNFilterEvents, &filterEvents, NULL);

		XSelectInput(display, iwnd, filterEvents);
	}

	XUnlockDisplay(display);
}

S::Void S::GUI::WindowXLib::DestroyInputContext()
{
	if (ic == NIL) return;

	/* Destroy input context.
	 */
	XDestroyIC(ic);

	ic = NIL;
}

S::Void S::GUI::WindowXLib::SetCursor(Cursor *cursor, const Point &point)
{
	WindowXLib	*window = GetWindowBackend((X11::Window) cursor->GetContainerWindow()->GetSystemWindow());

	if (window != NIL && window->ic != NIL)
	{
		/* Remove active cursor.
		 */
		if (cursor != activeCursor && activeCursor != NIL) RemoveCursor(activeCursor);
	}

	if (window != NIL)
	{
		/* Create input context.
		 */
		window->CreateInputContext();

		/* Move input window to cursor position.
		 */
		XMoveWindow(window->display, window->iwnd, point.x - 3, point.y + cursor->GetFont().GetScaledTextSizeY() + 1);

		/* Set input focus on input window.
		 */
		X11::Window	 focusWnd = None;
		int		 revertTo = RevertToNone;

		XGetInputFocus(window->display, &focusWnd, &revertTo);

		if (focusWnd != window->iwnd)
		{
			XSetInputFocus(window->display, window->iwnd, RevertToParent, CurrentTime);
			XFlush(window->display);
		}

		/* Set current input context.
		 */
		XSetICFocus(window->ic);
	}

	activeCursor	= cursor;
	activeCursorPos = point;
}

S::Void S::GUI::WindowXLib::RemoveCursor(Cursor *cursor)
{
	if (activeCursor != cursor) return;

	WindowXLib	*window = GetWindowBackend((X11::Window) cursor->GetContainerWindow()->GetSystemWindow());

	if (window != NIL && window->ic != NIL)
	{
		/* Destroy input context.
		 */
		window->DestroyInputContext();

		/* Clear preediting area.
		 */
		if (activeCursor->IsVisible())
		{
			Rect	 rect = Rect::OverlapRect(Rect(activeCursorPos - Point(1, 1), activeCursor->GetRealSize()), Rect(activeCursor->GetRealPosition(), activeCursor->GetRealSize()));

			activeCursor->SetIMECursor(True);

			window->drawSurface->StartPaint(rect);
			window->drawSurface->Box(rect, activeCursor->GetBackgroundColor(), Rect::Filled);

			activeCursor->Paint(SP_PAINT);

			window->drawSurface->EndPaint();

			activeCursor->SetIMECursor(False);
		}
	}

	activeCursor	= NIL;
	activeCursorPos = Point();
}
