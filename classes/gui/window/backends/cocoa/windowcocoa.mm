 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/cocoa/windowcocoa.h>
#include <smooth/gui/window/window.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>
#include <smooth/init.h>

S::GUI::WindowBackend *CreateWindowCocoa()
{
	return new S::GUI::WindowCocoa();
}

S::Int	 windowCocoaTmp = S::GUI::WindowBackend::SetBackend(&CreateWindowCocoa);

S::Array<S::GUI::WindowCocoa *, S::Void *>	 S::GUI::WindowCocoa::windowBackends;

const int	 NSApplicationPaint   = 1;
const int	 NSApplicationMap     = 2;
const int	 NSApplicationUnmap   = 3;
const int	 NSApplicationMove    = 4;
const int	 NSApplicationResize  = 5;
const int	 NSApplicationFocus   = 6;
const int	 NSApplicationUnfocus = 7;

@interface CocoaView : NSView
{
	@private
		NSTrackingArea	*trackingArea;
}

	- (id)		initWithFrame:	   (NSRect) frameRect;

	- (void)	updateTrackingAreas;

	- (void)	handleEvent:	   (NSEvent *) event;

	- (void)	mouseMoved:	   (NSEvent *) event;

	- (void)	mouseEntered:	   (NSEvent *) event;
	- (void)	mouseExited:	   (NSEvent *) event;

	- (void)	mouseDown:	   (NSEvent *) event;
	- (void)	mouseDragged:	   (NSEvent *) event;
	- (void)	mouseUp:	   (NSEvent *) event;

	- (void)	rightMouseDown:    (NSEvent *) event;
	- (void)	rightMouseDragged: (NSEvent *) event;
	- (void)	rightMouseUp:      (NSEvent *) event;

	- (void)	otherMouseDown:    (NSEvent *) event;
	- (void)	otherMouseDragged: (NSEvent *) event;
	- (void)	otherMouseUp:      (NSEvent *) event;

	- (void)	scrollWheel:	   (NSEvent *) event;

	- (void)	keyDown:	   (NSEvent *) event;
	- (void)	keyUp:		   (NSEvent *) event;

	- (void)	drawRect:	   (NSRect) rect;

	- (BOOL)	isFlipped;

	- (BOOL)	acceptsFirstResponder;
@end

@implementation CocoaView
	- (id) initWithFrame: (NSRect) frameRect
	{
		[super initWithFrame: frameRect];

		trackingArea = [[[NSTrackingArea alloc] initWithRect: [self visibleRect]
							     options: NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways
							       owner: self
							    userInfo: nil] autorelease];

		[self addTrackingArea: trackingArea];

		return self;
	}

	- (void) updateTrackingAreas
	{
		[self removeTrackingArea: trackingArea];

		trackingArea = [[[NSTrackingArea alloc] initWithRect: [self visibleRect]
							     options: NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways
							       owner: self
							    userInfo: nil] autorelease];

		[self addTrackingArea: trackingArea];
	}

	- (void) handleEvent: (NSEvent *) event
	{
		S::GUI::WindowCocoa	*backend = S::GUI::WindowCocoa::GetWindowBackend([event window]);

		if (backend != NIL) backend->ProcessSystemMessages(event);
	}

	- (void) mouseMoved:	    (NSEvent *) event { [self handleEvent: event]; }

	- (void) mouseEntered:	    (NSEvent *) event { [self handleEvent: event]; }
	- (void) mouseExited:	    (NSEvent *) event { [self handleEvent: event]; }

	- (void) mouseDown:	    (NSEvent *) event { [self handleEvent: event]; }
	- (void) mouseDragged:	    (NSEvent *) event { [self handleEvent: event]; }
	- (void) mouseUp:	    (NSEvent *) event { [self handleEvent: event]; }

	- (void) rightMouseDown:    (NSEvent *) event { [self handleEvent: event]; }
	- (void) rightMouseDragged: (NSEvent *) event { [self handleEvent: event]; }
	- (void) rightMouseUp:	    (NSEvent *) event { [self handleEvent: event]; }

	- (void) otherMouseDown:    (NSEvent *) event { [self handleEvent: event]; }
	- (void) otherMouseDragged: (NSEvent *) event { [self handleEvent: event]; }
	- (void) otherMouseUp:	    (NSEvent *) event { [self handleEvent: event]; }

	- (void) scrollWheel:	    (NSEvent *) event { [self handleEvent: event]; }

	- (void) keyDown:	    (NSEvent *) event { [self handleEvent: event]; }
	- (void) keyUp:		    (NSEvent *) event { [self handleEvent: event]; }

	- (void) drawRect: (NSRect) rect
	{
		S::GUI::WindowCocoa	*backend = S::GUI::WindowCocoa::GetWindowBackend([self window]);
		NSEvent			*event	 = [NSEvent otherEventWithType: NSApplicationDefined
								      location: NSMakePoint(0, 0)
								 modifierFlags: 0
								     timestamp: 0
								  windowNumber: [[self window] windowNumber]
								       context: [[self window] graphicsContext]
								       subtype: NSApplicationPaint
									 data1: (NSInteger) &rect
									 data2: nil];

		if (backend != NIL) backend->ProcessSystemMessages(event);
	}

	- (BOOL) isFlipped
	{
		return YES;
	}

	- (BOOL) acceptsFirstResponder
	{
		return YES;
	}
@end

#if defined MAC_OS_X_VERSION_10_6 && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface CocoaWindowDelegate : NSObject <NSWindowDelegate> { }
#else
@interface CocoaWindowDelegate : NSObject { }
#endif
	- (void) handleNotification: (NSNotification *) note as: (int) type;

	- (BOOL) windowShouldClose:  (id) sender;

	- (void) windowDidMove:	     (NSNotification *) note;
	- (void) windowDidResize:    (NSNotification *) note;

	- (void) windowDidBecomeKey: (NSNotification *) note;
	- (void) windowDidResignKey: (NSNotification *) note;
@end

@implementation CocoaWindowDelegate
	- (BOOL) windowShouldClose: (id) sender
	{
		S::GUI::WindowCocoa	*backend = S::GUI::WindowCocoa::GetWindowBackend(sender);

		if (backend != NIL)
		{
			if (backend->doClose.Call()) backend->Close();

			if (backend->GetSystemWindow() == NIL) return YES;
			else				       return NO;
		}

		return YES;
	}

	- (void) handleNotification: (NSNotification *) note as: (int) type
	{
		S::GUI::WindowCocoa	*backend = S::GUI::WindowCocoa::GetWindowBackend([note object]);
		NSEvent			*event	 = [NSEvent otherEventWithType: NSApplicationDefined
								      location: NSMakePoint(0, 0)
								 modifierFlags: 0
								     timestamp: 0
								  windowNumber: [[note object] windowNumber]
								       context: [[note object] graphicsContext]
								       subtype: type
									 data1: nil
									 data2: nil];

		if (backend != NIL) backend->ProcessSystemMessages(event);
	}

	- (void) windowDidMove:	     (NSNotification *) note { [self handleNotification: note as: NSApplicationMove];	 }
	- (void) windowDidResize:    (NSNotification *) note { [self handleNotification: note as: NSApplicationResize];	 }
	- (void) windowDidBecomeKey: (NSNotification *) note { [self handleNotification: note as: NSApplicationFocus];	 }
	- (void) windowDidResignKey: (NSNotification *) note { [self handleNotification: note as: NSApplicationUnfocus]; }
@end

@interface CocoaWindow : NSWindow { }
	- (BOOL) worksWhenModal;

	- (void) processEvent:	       (int) type;

	- (void) orderFront:	       (id) sender;
	- (void) orderOut:	       (id) sender;
	- (void) makeKeyAndOrderFront: (id) sender;
@end

@implementation CocoaWindow
	- (BOOL) worksWhenModal { return YES; }

	- (void) processEvent: (int) type
	{
		S::GUI::WindowCocoa	*backend = S::GUI::WindowCocoa::GetWindowBackend(self);
		NSEvent			*event	 = [NSEvent otherEventWithType: NSApplicationDefined
								      location: NSMakePoint(0, 0)
								 modifierFlags: 0
								     timestamp: 0
								  windowNumber: [self windowNumber]
								       context: [self graphicsContext]
								       subtype: type
									 data1: nil
									 data2: nil];

		if (backend != NIL) backend->ProcessSystemMessages(event);
	}

	- (void) orderFront:	       (id) sender { [super orderFront: sender];	   [self processEvent: NSApplicationMap];   }
	- (void) orderOut:	       (id) sender { [super orderOut: sender];		   [self processEvent: NSApplicationUnmap]; }
	- (void) makeKeyAndOrderFront: (id) sender { [super makeKeyAndOrderFront: sender]; [self processEvent: NSApplicationMap];   }
@end

S::GUI::WindowCocoa::WindowCocoa(Void *iWindow)
{
	type	 = WINDOW_COCOA;

	wnd	 = nil;

	wid	 = windowBackends.Add(this);

	minSize	 = Size(160, 24);
	maxSize	 = Size(32768, 32768);

	fontSize = Surface().GetSurfaceDPI() / 96.0;

	flags	 = 0;
}

S::GUI::WindowCocoa::~WindowCocoa()
{
	if (wnd != nil) [wnd release];

	windowBackends.Remove(wid);
}

S::Void *S::GUI::WindowCocoa::GetSystemWindow() const
{
	return (Void *) wnd;
}

S::GUI::WindowCocoa *S::GUI::WindowCocoa::GetWindowBackend(NSWindow *wnd)
{
	if (wnd == nil) return NIL;

	for (Int i = 0; i < windowBackends.Length(); i++)
	{
		WindowCocoa	*window = windowBackends.GetNth(i);

		if (window != NIL)
		{
			if (window->wnd == wnd) return window;
		}
	}

	return NIL;
}

S::Input::Keyboard::Key S::GUI::WindowCocoa::ConvertKey(Int keyCode)
{
	Input::Keyboard::Key	 key = Input::Keyboard::KeyOther;

	switch (keyCode)
	{
		case 123: key = Input::Keyboard::KeyLeft;    break;
		case 124: key = Input::Keyboard::KeyRight;   break;
		case 125: key = Input::Keyboard::KeyDown;    break;
		case 126: key = Input::Keyboard::KeyUp;      break;

		case 115: key = Input::Keyboard::KeyHome;    break;
		case 116: key = Input::Keyboard::KeyNext;    break;
		case 117: key = Input::Keyboard::KeyDelete;  break;
		case 119: key = Input::Keyboard::KeyEnd;     break;
		case 121: key = Input::Keyboard::KeyPrior;   break;

		case 36:  key = Input::Keyboard::KeyReturn;  break;
		case 51:  key = Input::Keyboard::KeyBack;    break;
		case 48:  key = Input::Keyboard::KeyTab;     break;

		case 49:  key = Input::Keyboard::KeySpace;   break;

		case 53:  key = Input::Keyboard::KeyEscape;  break;

		case 122: key = Input::Keyboard::KeyF1;      break;
		case 120: key = Input::Keyboard::KeyF2;      break;
		case 99:  key = Input::Keyboard::KeyF3;      break;
		case 118: key = Input::Keyboard::KeyF4;      break;
		case 96:  key = Input::Keyboard::KeyF5;      break;
		case 97:  key = Input::Keyboard::KeyF6;      break;
		case 98:  key = Input::Keyboard::KeyF7;      break;
		case 100: key = Input::Keyboard::KeyF8;      break;
	}

	return key;
}

S::Int S::GUI::WindowCocoa::ProcessSystemMessages(NSEvent *e)
{
	static Int	 focusWndID = -1;

	/* Convert Cocoa events to smooth messages.
	 */
	switch ([e type])
	{
		/* Mouse events:
		 */
		case NSMouseMoved:
		case NSMouseEntered:
		case NSMouseExited:

		case NSLeftMouseDown:
		case NSLeftMouseDragged:
		case NSLeftMouseUp:

		case NSRightMouseDown:
		case NSRightMouseDragged:
		case NSRightMouseUp:
			/* Update pointer position in Input::Pointer.
			 */
			if ([e type] == NSMouseExited) Input::Pointer::UpdatePosition(NIL,				      [NSEvent mouseLocation].x, [[wnd screen] frame].size.height - [NSEvent mouseLocation].y);
			else			       Input::Pointer::UpdatePosition(Window::GetWindow((Void *) [e window]), [NSEvent mouseLocation].x, [[wnd screen] frame].size.height - [NSEvent mouseLocation].y);

			/* Reject if a modal window is active.
			 */
			if (([e type] == NSLeftMouseDown ||
			     [e type] == NSRightMouseDown) && !(flags & WF_TOPMOST) && ( [NSApp modalWindow] != nil &&
											 [NSApp modalWindow] != wnd &&
											[[NSApp modalWindow] isVisible])) break;

			/* Grab the keyboard focus if we don't have it already.
			 */
			if (focusWndID != wid && ([e type] == NSLeftMouseDown ||
						  [e type] == NSRightMouseDown))
			{
				WindowCocoa	*focusWnd = windowBackends.Get(focusWndID);

				if (focusWnd != NIL) focusWnd->onEvent.Call(SM_LOSEFOCUS, Window::GetWindow((Void *) wnd)->GetHandle(), 0);

				focusWndID = wid;

				onEvent.Call(SM_GETFOCUS, 0, 0);
			}

			/* Pass message to smooth window.
			 */
			if	([e type] == NSLeftMouseDown)  onEvent.Call(SM_LBUTTONDOWN, 0, 0);
			else if ([e type] == NSLeftMouseUp)    onEvent.Call(SM_LBUTTONUP,   0, 0);
			else if	([e type] == NSRightMouseDown) onEvent.Call(SM_RBUTTONDOWN, 0, 0);
			else if ([e type] == NSRightMouseUp)   onEvent.Call(SM_RBUTTONUP,   0, 0);
			else				       onEvent.Call(SM_MOUSEMOVE,   0, 0);

			break;

		case NSOtherMouseDown:
		case NSOtherMouseDragged:
		case NSOtherMouseUp:
			/* Update pointer position in Input::Pointer.
			 */
			Input::Pointer::UpdatePosition(Window::GetWindow((Void *) [e window]), [NSEvent mouseLocation].x, [[wnd screen] frame].size.height - [NSEvent mouseLocation].y);

			/* Reject if a modal window is active.
			 */
			if ([e type] == NSOtherMouseDown && !(flags & WF_TOPMOST) && ( [NSApp modalWindow] != nil &&
										       [NSApp modalWindow] != wnd &&
										      [[NSApp modalWindow] isVisible])) break;

			/* Grab the keyboard focus if we don't have it already.
			 */
			if (focusWndID != wid && [e type] == NSOtherMouseDown)
			{
				WindowCocoa	*focusWnd = windowBackends.Get(focusWndID);

				if (focusWnd != NIL) focusWnd->onEvent.Call(SM_LOSEFOCUS, Window::GetWindow((Void *) wnd)->GetHandle(), 0);

				focusWndID = wid;

				onEvent.Call(SM_GETFOCUS, 0, 0);
			}

			/* Pass message to smooth window.
			 */
			if	([e type] == NSOtherMouseDown) { if ([e buttonNumber] == 2) onEvent.Call(SM_MBUTTONDOWN, 0, 0); }
			else if ([e type] == NSOtherMouseUp)   { if ([e buttonNumber] == 2) onEvent.Call(SM_MBUTTONUP,   0, 0); }
			else				       				    onEvent.Call(SM_MOUSEMOVE,   0, 0);

			break;

		case NSScrollWheel:
			/* Pass message to smooth window.
			 */
			onEvent.Call(SM_MOUSEWHEEL, [e deltaY] * 1200, 0);

			/* Send an event to update widget under cursor if necessary.
			 */
			onEvent.Call(SM_MOUSEMOVE, 0, 0);

			break;

		/* Keyboard events:
		 */
		case NSKeyDown:
			Input::Keyboard::UpdateKeyState(ConvertKey([e keyCode]), True);

			onEvent.Call(SM_KEYDOWN, ConvertKey([e keyCode]), 0);

			/* Convert keyboard event to input string and
			 * call SM_CHAR event for each character.
			 */
			{
				NSString	*characters = [e characters];
				String		 string;

				string.ImportFrom("UTF-8", [characters UTF8String]);

				for (Int i = 0; i < string.Length(); i++) onEvent.Call(SM_CHAR, string[i], 0);
			}

			break;

		case NSKeyUp:
			Input::Keyboard::UpdateKeyState(ConvertKey([e keyCode]), False);

			onEvent.Call(SM_KEYUP, ConvertKey([e keyCode]), 0);

			break;

		/* Custom events:
		 */
		case NSApplicationDefined:
			/* Paint events:
			 */
			if ([e subtype] == NSApplicationPaint)
			{
				/* Get drawing area.
				 */
				NSRect	*rect = (NSRect *) [e data1];

				/* Fill update rect.
				 */
				if (updateRect == Rect())
				{
					updateRect.left	  = rect->origin.x;
					updateRect.top	  = rect->origin.y;
					updateRect.right  = rect->origin.x + rect->size.width;
					updateRect.bottom = rect->origin.x + rect->size.height;
				}
				else
				{
					updateRect.left	  = Math::Min(updateRect.left, Int(rect->origin.x));
					updateRect.top	  = Math::Min(updateRect.top, Int(rect->origin.y));
					updateRect.right  = Math::Max(updateRect.right, Int(rect->origin.x + rect->size.width));
					updateRect.bottom = Math::Max(updateRect.bottom, Int(rect->origin.x + rect->size.height));
				}

				onEvent.Call(SM_PAINT, 0, 0);

				updateRect = Rect();
			}

			/* Window state change events:
			 */
			if ([e subtype] == NSApplicationMap)
			{
				/* Set internal focus window.
				 */
				focusWndID = wid;

				onEvent.Call(SM_GETFOCUS, 0, 0);
			}

			if ([e subtype] == NSApplicationUnmap)
			{
				/* Clear internal focus window.
				 */
				if (focusWndID == wid) focusWndID = -1;
			}

			if ([e subtype] == NSApplicationMove)
			{
				/* Set metrics and emit window metrics event.
				 */
				NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

				contentRect.origin.y = [[wnd screen] frame].size.height - (contentRect.origin.y + contentRect.size.height);

				onEvent.Call(SM_WINDOWMETRICS, ((	     Int(contentRect.origin.x)		     + 32768) << 16) | (	    Int(contentRect.origin.y)		     + 32768),
							       ((Math::Floor(Int(contentRect.size.width) / fontSize) + 32768) << 16) | (Math::Floor(Int(contentRect.size.height) / fontSize) + 32768));
			}

			if ([e subtype] == NSApplicationResize)
			{
				/* Set metrics and emit window metrics event.
				 */
				NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

				contentRect.origin.y = [[wnd screen] frame].size.height - (contentRect.origin.y + contentRect.size.height);

				if (drawSurface != NIL) drawSurface->SetSize(Size(contentRect.size.width, contentRect.size.height));

				onEvent.Call(SM_WINDOWMETRICS, ((	     Int(contentRect.origin.x)		     + 32768) << 16) | (	    Int(contentRect.origin.y)		     + 32768),
							       ((Math::Floor(Int(contentRect.size.width) / fontSize) + 32768) << 16) | (Math::Floor(Int(contentRect.size.height) / fontSize) + 32768));
			}

			/* Focus events:
			 */
			if ([e subtype] == NSApplicationFocus)
			{
				focusWndID = wid;

				onEvent.Call(SM_GETFOCUS, 0, 0);
			}

			if ([e subtype] == NSApplicationUnfocus)
			{
				Input::Keyboard::ResetKeyState();

				/* Get the window that now has the focus.
				 */
				{
					NSWindow	*cocoaFocusWnd = [NSApp keyWindow];
					Window		*focusWnd      = Window::GetWindow((Void *) cocoaFocusWnd);

					onEvent.Call(SM_LOSEFOCUS, focusWnd != NIL ? focusWnd->GetHandle() : -1, 0);
				 }
			}

			break;
	}

	/* Return from the active loop after processing an event.
	 */
	if ([wnd isKeyWindow] && (  [NSApp modalWindow] == nil ||
				    [NSApp modalWindow] == wnd ||
				  ![[NSApp modalWindow] isVisible])) [NSApp stop: nil];

	return Success();
}

S::Int S::GUI::WindowCocoa::Open(const String &title, const Point &pos, const Size &size, Int iFlags)
{
	flags = iFlags;

	NSRect		 frame	   = NSMakeRect(pos.x, [[NSScreen mainScreen] frame].size.height - (pos.y + Math::Round(size.cy * fontSize) + sizeModifier.cy), Math::Round(size.cx * fontSize) + sizeModifier.cx, Math::Round(size.cy * fontSize) + sizeModifier.cy);
	NSUInteger	 styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;

	if (!(flags & WF_NORESIZE  )) styleMask |= NSResizableWindowMask;
	if (  flags & WF_THINBORDER ) styleMask  = NSBorderlessWindowMask;

	wnd = [[[CocoaWindow alloc] initWithContentRect: frame
					      styleMask: styleMask
						backing: NSBackingStoreBuffered
						  defer: YES] retain];

	if (wnd != nil)
	{
		/* Init content view and delegate.
		 */
		[wnd setContentView: [[CocoaView alloc] initWithFrame: frame]];
		[wnd setDelegate: [[CocoaWindowDelegate alloc] init]];

		/* Create drawing surface.
		*/
		if ((flags & WF_THINBORDER) || (flags & WF_NORESIZE)) drawSurface = new Surface((Void *) wnd, size * fontSize + sizeModifier);
		else						      drawSurface = new Surface((Void *) wnd);

		drawSurface->SetSize(size * fontSize + sizeModifier);
		
		[wnd useOptimizedDrawing: YES];

		/* Set window title.
		 */
		[wnd setTitle: [NSString stringWithUTF8String: title.ConvertTo("UTF-8")]];

		/* Adjust minimum and maximum size for current size modifier.
		 */
		if (minSize != Size(160, 24))	   minSize = minSize + Size(6, 6) + sizeModifier;
		if (maxSize != Size(32768, 32768)) maxSize = maxSize + Size(6, 6) + sizeModifier;

		/* Set metrics to actually allocated window.
		 */
		NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

		contentRect.origin.y = [[wnd screen] frame].size.height - (contentRect.origin.y + contentRect.size.height);

		onEvent.Call(SM_WINDOWMETRICS, ((	     Int(contentRect.origin.x)		     + 32768) << 16) | (	    Int(contentRect.origin.y)		     + 32768),
					       ((Math::Floor(Int(contentRect.size.width) / fontSize) + 32768) << 16) | (Math::Floor(Int(contentRect.size.height) / fontSize) + 32768));

		/* Set window level for topmost windows.
		 */
		if (flags & WF_TOPMOST) [wnd setLevel: NSPopUpMenuWindowLevel];

		/* Emit onCreate signal.
		 */
		onCreate.Emit();

		return Success();
	}

	return Error();
}

S::Int S::GUI::WindowCocoa::Close()
{
	if (wnd == nil) return Success();

	/* Delete surface.
	 */
	if (drawSurface != NIL) delete drawSurface;

	drawSurface = NIL;

	/* Destroy input context and window.
	 */
	[wnd close];

	[[wnd contentView] release];
	[[wnd delegate] release];

	/* Emit onDestroy signal.
	 */
	onDestroy.Emit();

	/* Stop active loop after the window is closed.
	 */
	[NSApp stop: nil];

	return Success();
}

S::Int S::GUI::WindowCocoa::RequestClose()
{
	if (wnd == nil) return Success();

	[wnd performClose: nil];

	return Success();
}

S::GUI::WindowCocoa *S::GUI::WindowCocoa::FindLeaderWindow()
{
	/* The leader window is the newest non topmost window.
	 */
	for (Int i = windowBackends.Length() - 1; i >= 0; i--)
	{
		WindowCocoa	*backend = windowBackends.GetNth(i);

		if (backend != this && backend->wnd != NIL && !(backend->flags & WF_TOPMOST)) return backend;
	}

	return NIL;
}

S::Bool S::GUI::WindowCocoa::IsModalWindowActive()
{
	/* Look for modal windows opened after ourselves.
	 */
	for (Int i = windowBackends.Length() - 1; i >= 0; i--)
	{
		WindowCocoa	*backend = windowBackends.GetNth(i);

		if	(backend == this)				   return False;
		else if (backend->wnd != NIL && backend->flags & WF_MODAL) return True;
	}

	return False;
}

S::Int S::GUI::WindowCocoa::SetTitle(const String &nTitle)
{
	if (wnd == nil) return Error();

	[wnd setTitle: [NSString stringWithUTF8String: nTitle.ConvertTo("UTF-8")]];

	return Success();
}

S::Int S::GUI::WindowCocoa::SetMinimumSize(const Size &nMinSize)
{
	minSize = nMinSize + sizeModifier;

	if (wnd == nil) return Success();

	[wnd setContentMinSize: NSMakeSize(minSize.cx, minSize.cy)];

	NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

	if (contentRect.size.width < minSize.cx || contentRect.size.height < minSize.cy)
	{
		[wnd setFrame: [wnd frameRectForContentRect: NSMakeRect(contentRect.origin.x, contentRect.origin.y + contentRect.size.height - Math::Max((Int) contentRect.size.height, minSize.cy), Math::Max((Int) contentRect.size.width, minSize.cx), Math::Max((Int) contentRect.size.height, minSize.cy))]
		      display: YES];
	}

	return Success();
}

S::Int S::GUI::WindowCocoa::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize + sizeModifier;

	if (wnd == nil) return Success();

	[wnd setContentMaxSize: NSMakeSize(maxSize.cx, maxSize.cy)];

	NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

	if (contentRect.size.width > maxSize.cx || contentRect.size.height > maxSize.cy)
	{
		[wnd setFrame: [wnd frameRectForContentRect: NSMakeRect(contentRect.origin.x, contentRect.origin.y + contentRect.size.height - Math::Min((Int) contentRect.size.height, maxSize.cy), Math::Min((Int) contentRect.size.width, maxSize.cx), Math::Min((Int) contentRect.size.height, maxSize.cy))]
		      display: YES];
	}

	return Success();
}

S::Int S::GUI::WindowCocoa::Show()
{
	if (wnd == nil) return Success();

	/* Show window.
	 */
	if ([NSThread isMainThread]) [wnd makeKeyAndOrderFront: nil];
	else			     [wnd performSelectorOnMainThread: @selector(makeKeyAndOrderFront:) withObject: nil waitUntilDone: YES];

	/* Set minimum and maximum size.
	 */
	if (!(flags & WF_THINBORDER))
	{
		SetMinimumSize(minSize - sizeModifier);
		SetMaximumSize(maxSize - sizeModifier);
	}

	/* Update window metrics.
	 */
	NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

	contentRect.origin.y = [[wnd screen] frame].size.height - (contentRect.origin.y + contentRect.size.height);

	onEvent.Call(SM_WINDOWMETRICS, ((	     Int(contentRect.origin.x)		     + 32768) << 16) | (	    Int(contentRect.origin.y)		     + 32768),
				       ((Math::Floor(Int(contentRect.size.width) / fontSize) + 32768) << 16) | (Math::Floor(Int(contentRect.size.height) / fontSize) + 32768));

	/* Set update rect and send paint event for new window.
	 */
	updateRect = Rect(Point(0, 0), Size(contentRect.size.width, contentRect.size.height));

	onEvent.Call(SM_PAINT, 0, 0);

	return Success();
}

S::Int S::GUI::WindowCocoa::Hide()
{
	if (wnd == nil) return Success();

	if ([NSThread isMainThread]) [wnd orderOut: nil];
	else			     [wnd performSelectorOnMainThread: @selector(orderOut:) withObject: nil waitUntilDone: YES];

	return Success();
}

S::Int S::GUI::WindowCocoa::SetMetrics(const Point &nPos, const Size &nSize)
{
	if (wnd == nil) return Success();

	[wnd setFrame: [wnd frameRectForContentRect: NSMakeRect(nPos.x, [[wnd screen] frame].size.height - (nPos.y + Math::Round(nSize.cy * fontSize) + sizeModifier.cy), Math::Round(nSize.cx * fontSize) + sizeModifier.cx, Math::Round(nSize.cy * fontSize) + sizeModifier.cy)]
	      display: YES];

	return Success();
}

S::Int S::GUI::WindowCocoa::Raise()
{
	if (wnd == nil) return Success();

	[wnd makeKeyAndOrderFront: nil];

	return Success();
}
