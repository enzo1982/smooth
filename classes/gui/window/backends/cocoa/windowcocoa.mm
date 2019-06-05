 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/cocoa/windowcocoa.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/special/cursor.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>
#include <smooth/init.h>

S::GUI::WindowBackend *CreateWindowCocoa()
{
	return new S::GUI::WindowCocoa();
}

S::Int	 windowCocoaTmp = S::GUI::WindowBackend::SetBackend(&CreateWindowCocoa);

S::Int	 addWindowCocoaInitTmp = S::AddInitFunction(&S::GUI::WindowCocoa::Initialize);
S::Int	 addWindowCocoaFreeTmp = S::AddFreeFunction(&S::GUI::WindowCocoa::Free);

S::Array<S::GUI::WindowCocoa *, S::Void *>	 S::GUI::WindowCocoa::windowBackends;

const int	 NSApplicationPaint	 = 1;
const int	 NSApplicationMap	 = 2;
const int	 NSApplicationUnmap	 = 3;
const int	 NSApplicationMove	 = 4;
const int	 NSApplicationResize	 = 5;
const int	 NSApplicationFocus	 = 6;
const int	 NSApplicationUnfocus	 = 7;
const int	 NSApplicationInsertText = 8;
const int	 NSApplicationDropFiles	 = 9;

@interface CocoaView : NSView <NSTextInputClient>
{
	@private
		NSTrackingArea			*trackingArea;

		NSMutableAttributedString	*editString;

		NSRange				 markedRange;
		NSMutableDictionary		*markedAttributes;

		S::GUI::Cursor			*cursor;
		S::GUI::Point			 cursorPosition;
}

	/* NSView methods.
	 */
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
	- (void)	flagsChanged:	   (NSEvent *) event;

	- (BOOL)	isFlipped;
	- (void)	drawRect:	   (NSRect) rect;

	- (BOOL)	acceptsFirstResponder;

	/* NSTextInputClient methods.
	 */
	- (void)		 insertText:	(id) string;
	- (void)		 insertText:	(id) string replacementRange: (NSRange) replacementRange;
	- (void)		 setMarkedText:	(id) string selectedRange:    (NSRange) selectedRange replacementRange: (NSRange) replacementRange;
	- (BOOL)		 hasMarkedText;
	- (void)		 unmarkText;

	- (NSRange)		 selectedRange;
	- (NSRange)		 markedRange;

	- (NSArray *)		 validAttributesForMarkedText;
	- (NSUInteger)		 characterIndexForPoint:	      (NSPoint) aPoint;

	- (NSRect)		 firstRectForCharacterRange:	      (NSRange) aRange actualRange: (NSRangePointer) actualRange;
	- (NSAttributedString *) attributedSubstringForProposedRange: (NSRange) aRange actualRange: (NSRangePointer) actualRange;

	- (void)		 doCommandBySelector:		      (SEL)	aSelector;

	/* Helper methods.
	 */
	- (void)	setCursor:    (S::GUI::Cursor *) aCursor position: (const S::GUI::Point &) aPoint;
	- (void)	removeCursor: (S::GUI::Cursor *) aCursor;
@end

@implementation CocoaView
	/* NSView methods.
	 */
	- (id) initWithFrame: (NSRect) frameRect
	{
		[super initWithFrame: frameRect];

		cursor		 = NIL;

		editString	 = [[[NSMutableAttributedString alloc] initWithString: @""] autorelease];

		markedRange	 = NSMakeRange(NSNotFound, 0);
		markedAttributes = [[[NSMutableDictionary alloc] init] autorelease];

		trackingArea	 = [[[NSTrackingArea alloc] initWithRect: [self visibleRect]
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

	- (void) keyDown: (NSEvent *) event
	{
		/* Collect queued key down events to process them all together.
		 */
		NSMutableArray	*keyEvents = [NSMutableArray arrayWithCapacity: 1];

		while (event != nil)
		{
			if (![self hasMarkedText]) [self handleEvent: event];

			/* Check if event is relevant for us.
			 */
			NSString	*characters    = [event charactersIgnoringModifiers];
			S::Int		 keySym	       = [characters length] >= 1 ? [characters characterAtIndex: 0] : 0;
			S::Int		 modifierFlags = [event modifierFlags];

			if ([self hasMarkedText] || (keySym != NSTabCharacter		 &&
						     keySym != NSBackTabCharacter	 &&
						     keySym != NSDeleteCharacter	 &&
						     keySym != NSEnterCharacter		 &&
						     keySym != NSNewlineCharacter	 &&
						     keySym != NSCarriageReturnCharacter &&
						     keySym != NSHomeFunctionKey	 &&
						     keySym != NSEndFunctionKey		 &&
						     keySym != NSDeleteFunctionKey	 &&
						     keySym != NSLeftArrowFunctionKey	 &&
						     keySym != NSUpArrowFunctionKey	 &&
						     keySym != NSRightArrowFunctionKey	 &&
						     keySym != NSDownArrowFunctionKey    &&
						     !(modifierFlags & NSControlKeyMask) &&
						     !(modifierFlags & NSCommandKeyMask))) [keyEvents addObject: event];

			/* Get next key down event.
			 */
			event = [[self window] nextEventMatchingMask: NSKeyDownMask
							   untilDate: [NSDate distantPast]
							      inMode: NSDefaultRunLoopMode
							     dequeue: YES];
		}

		/* Send events to input manager.
		 */
		if ([keyEvents count] == 0) return;

		S::GUI::WindowCocoa	*backend = S::GUI::WindowCocoa::GetWindowBackend([self window]);

		if (backend != NIL && cursor != NIL)
		{
			S::GUI::Surface	*surface = cursor->GetDrawSurface();

			surface->StartPaint(S::GUI::Rect(cursor->GetContainer()->GetRealPosition(), cursor->GetContainer()->GetRealSize()));

			[self interpretKeyEvents: keyEvents];

			surface->EndPaint();
		}
	}

	- (void) keyUp:		    (NSEvent *) event { [self handleEvent: event]; }
	- (void) flagsChanged:	    (NSEvent *) event { [self handleEvent: event]; }

	- (BOOL) isFlipped
	{
		return YES;
	}

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

		if (markedRange.location != NSNotFound && cursor != NIL)
		{
			[[NSGraphicsContext currentContext] setShouldAntialias: YES];

			[NSGraphicsContext saveGraphicsState];

			[[NSBezierPath bezierPathWithRect: NSMakeRect(cursor->GetRealPosition().x, [self frame].size.height - cursor->GetRealPosition().y - cursor->GetRealSize().cy, cursor->GetRealSize().cx, cursor->GetRealSize().cy)] addClip];

			NSAttributedString	*range = [editString attributedSubstringFromRange: markedRange];

			[range drawAtPoint: NSMakePoint(cursorPosition.x - 1,
							[self frame].size.height - cursorPosition.y - 1 - cursor->GetFont().GetScaledTextSizeY())];

			[NSGraphicsContext restoreGraphicsState];
		}
	}

	- (BOOL) acceptsFirstResponder
	{
		return YES;
	}

	/* NSTextInputClient methods.
	 */
	- (void) insertText: (id) string
	{
		S::GUI::WindowCocoa	*backend = S::GUI::WindowCocoa::GetWindowBackend([self window]);
		NSEvent			*event	 = [NSEvent otherEventWithType: NSApplicationDefined
								      location: NSMakePoint(0, 0)
								 modifierFlags: 0
								     timestamp: 0
								  windowNumber: [[self window] windowNumber]
								       context: [[self window] graphicsContext]
								       subtype: NSApplicationInsertText
									 data1: (NSInteger) string
									 data2: nil];

		if (backend != NIL && cursor != NIL) backend->ProcessSystemMessages(event);

		[self unmarkText];
	}

	- (void) insertText: (id) string replacementRange: (NSRange) replacementRange
	{
		[self insertText: string];
	}

	- (void) setMarkedText: (id) string selectedRange: (NSRange) selectedRange replacementRange: (NSRange) replacementRange
	{
		if (replacementRange.location == NSNotFound) replacementRange = NSMakeRange(0, 0);

		[editString beginEditing];

		if ([string length] == 0)
		{
			[self unmarkText];
		}
		else
		{
			markedRange = NSMakeRange(replacementRange.location, [string length]);

			if ([string isKindOfClass: [NSAttributedString class]]) [editString replaceCharactersInRange: replacementRange withAttributedString: string];
			else							[editString replaceCharactersInRange: replacementRange withString: string];

			[editString addAttributes: markedAttributes range: markedRange];

			if (cursor != NIL)
			{
				S::String	 string;

				string.ImportFrom("UTF-8", [[[editString attributedSubstringFromRange: markedRange] string] UTF8String]);

				cursor->SetIMEAdvance(cursor->GetFont().GetScaledTextSizeX(string));
			}
		}

		[editString endEditing];

		[self setNeedsDisplay: YES];
	}

	- (BOOL) hasMarkedText
	{
		return (markedRange.location == NSNotFound ? NO : YES);
	}

	- (void) unmarkText
	{
		markedRange = NSMakeRange(NSNotFound, 0);

		[editString setAttributedString: [[[NSAttributedString alloc] initWithString: @""] autorelease]];

		[[NSInputManager currentInputManager] markedTextAbandoned: self];

		if (cursor != NIL) cursor->SetIMEAdvance(0);
	}

	- (NSRange) selectedRange
	{
		return NSMakeRange(NSNotFound, 0);
	}

	- (NSRange) markedRange
	{
		return markedRange;
	}

	- (NSArray *) validAttributesForMarkedText
	{
		return [NSArray arrayWithObjects: NSMarkedClauseSegmentAttributeName, NSGlyphInfoAttributeName, nil];
	}

	- (NSUInteger) characterIndexForPoint: (NSPoint) aPoint
	{
		return 0;
	}

	- (NSRect) firstRectForCharacterRange: (NSRange) aRange actualRange: (NSRangePointer) actualRange
	{
		if (actualRange) *actualRange = aRange;

		NSRect	 rect = NSMakeRect(0, 0, 0, 0);

		if (cursor != NIL) rect = NSMakeRect(cursorPosition.x, [self frame].size.height - cursorPosition.y - cursor->GetFont().GetScaledTextSizeY(), 0, 0);

		rect	    = [self convertRectToBase: rect];
		rect.origin = [[self window] convertBaseToScreen: rect.origin];

		return rect;
	}

	- (NSAttributedString *) attributedSubstringForProposedRange: (NSRange) aRange actualRange: (NSRangePointer) actualRange
	{
		if (actualRange) *actualRange = aRange;

		return [editString attributedSubstringFromRange: aRange];
	}

	- (void) doCommandBySelector: (SEL) aSelector
	{
		[super doCommandBySelector: aSelector];
	}

	/* Helper methods.
	 */
	- (void) setCursor: (S::GUI::Cursor *) aCursor position: (const S::GUI::Point &) aPoint
	{
		const S::GUI::Font	&font = aCursor->GetFont();

		NSFont			*nsFont = [[NSFontManager sharedFontManager] fontWithFamily: [NSString stringWithUTF8String: font.GetName().ConvertTo("UTF-8")]
											     traits: (font.GetStyle() & S::GUI::Font::Italic ? NSItalicFontMask : 0) | (font.GetWeight() >= S::GUI::Font::Bold ? NSBoldFontMask : 0)
											     weight: 5
											       size: font.GetSize() * S::GUI::Surface().GetSurfaceDPI() / 72.0];

		[markedAttributes setObject: nsFont
				     forKey: NSFontAttributeName];

		[self unmarkText];
		[self setNeedsDisplay: YES];

		cursor	       = aCursor;
		cursorPosition = aPoint;
	}

	- (void) removeCursor: (S::GUI::Cursor *) aCursor
	{
		if (cursor != aCursor) return;

		[self unmarkText];
		[self setNeedsDisplay: YES];

		cursor = NIL;
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

			/* Return no in any case. Either the user decided not to
			 * close the window or we closed it on our own above.
			 */
			return NO;
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

@interface CocoaWindow : NSWindow
{
	@private
		NSUInteger	 styleMask;

		NSRect		 frameRect;
		NSRect		 contentRect;

		NSSize		 minSize;
		NSSize		 maxSize;
}

	/* NSWindow methods.
	 */
	- (BOOL)	    worksWhenModal;

	- (void)	    orderFront:		  (id) sender;
	- (void)	    orderOut:		  (id) sender;
	- (void)	    makeKeyAndOrderFront: (id) sender;

	/* Helper Methods.
	 */
	- (id)		    initWithContentRect:  (NSRect) rect styleMask: (NSUInteger) style;

	- (void)	    setFrame:		  (NSRect) rect;
	- (void)	    setMinSize:		  (NSSize) size;
	- (void)	    setMaxSize:		  (NSSize) size;

	- (void)	    processEvent:	  (int) type;
	- (void)	    processEvent:	  (int) type withData: (NSInteger) data;

	- (NSDragOperation) draggingEntered:	  (id) sender;
	- (BOOL)	    performDragOperation: (id) sender;

	/* Internal use only.
	 */
	- (id)		    internalInitWithAttributes;

	- (void)	    internalSetFrameWithAttributes;
	- (void)	    internalSetMinSizeWithAttributes;
	- (void)	    internalSetMaxSizeWithAttributes;

@end

@implementation CocoaWindow
	/* NSWindow methods.
	 */
	- (BOOL) worksWhenModal { return YES; }

	- (void) orderFront:	       (id) sender { [super orderFront: sender];	   [self processEvent: NSApplicationMap];   }
	- (void) orderOut:	       (id) sender { [super orderOut: sender];		   [self processEvent: NSApplicationUnmap]; }
	- (void) makeKeyAndOrderFront: (id) sender { [super makeKeyAndOrderFront: sender]; [self processEvent: NSApplicationMap];   }

	/* Helper Methods.
	 */
	- (id) initWithContentRect: (NSRect) rect styleMask: (NSUInteger) style
	{
		contentRect = rect;
		styleMask   = style;

		if ([NSThread isMainThread]) [self internalInitWithAttributes];
		else			     [self performSelectorOnMainThread: @selector(internalInitWithAttributes) withObject: NIL waitUntilDone: YES];

		return self;
	}

	- (void) setFrame: (NSRect) rect
	{
		frameRect = rect;

		if ([NSThread isMainThread]) [self internalSetFrameWithAttributes];
		else			     [self performSelectorOnMainThread: @selector(internalSetFrameWithAttributes) withObject: NIL waitUntilDone: YES];
	}

	- (void) setMinSize: (NSSize) size
	{
		minSize = size;

		if ([NSThread isMainThread]) [self internalSetMinSizeWithAttributes];
		else			     [self performSelectorOnMainThread: @selector(internalSetMinSizeWithAttributes) withObject: NIL waitUntilDone: YES];
	}

	- (void) setMaxSize: (NSSize) size
	{
		maxSize = size;

		if ([NSThread isMainThread]) [self internalSetMaxSizeWithAttributes];
		else			     [self performSelectorOnMainThread: @selector(internalSetMaxSizeWithAttributes) withObject: NIL waitUntilDone: YES];
	}

	- (void) processEvent: (int) type
	{
		[self processEvent: type
			  withData: nil];
	}

	- (void) processEvent: (int) type withData: (NSInteger) data
	{
		S::GUI::WindowCocoa	*backend = S::GUI::WindowCocoa::GetWindowBackend(self);
		NSEvent			*event	 = [NSEvent otherEventWithType: NSApplicationDefined
								      location: NSMakePoint(0, 0)
								 modifierFlags: 0
								     timestamp: 0
								  windowNumber: [self windowNumber]
								       context: [self graphicsContext]
								       subtype: type
									 data1: data
									 data2: nil];

		if (backend != NIL) backend->ProcessSystemMessages(event);
	}

	- (NSDragOperation) draggingEntered: (id) sender
	{
		NSPasteboard	*pasteBoard	= [sender draggingPasteboard];
		NSDragOperation	 sourceDragMask = [sender draggingSourceOperationMask];

		if ([[pasteBoard types] containsObject: NSFilenamesPboardType] && sourceDragMask & NSDragOperationGeneric)
		{
			return NSDragOperationGeneric;
		}

		return NSDragOperationNone;
	}

	- (BOOL) performDragOperation: (id) sender
	{
		[self processEvent: NSApplicationDropFiles
			  withData: (NSInteger) sender];

		return YES;
	}

	/* Internal use only.
	 */
	- (id) internalInitWithAttributes
	{
		[super initWithContentRect: contentRect
				 styleMask: styleMask
				   backing: NSBackingStoreBuffered
				     defer: YES];

		return self;
	}

	- (void) internalSetFrameWithAttributes	  { [self setFrame: frameRect display: YES]; }

	- (void) internalSetMinSizeWithAttributes { [self setContentMinSize: minSize]; }
	- (void) internalSetMaxSizeWithAttributes { [self setContentMaxSize: maxSize]; }

@end

@interface CocoaCircularProgress : NSView
{
	BOOL	 hidden;
	float	 progress;
}

	- (id)	 init;

	- (void) setHidden:   (BOOL) value;
	- (void) setProgress: (float) value;
@end

@implementation CocoaCircularProgress
	- (id) init
	{
		[super init];

		hidden	 = NO;
		progress = 0.0;

		return self;
	}

	- (void) drawRect: (NSRect) dirtyRect
	{
		[[NSApp applicationIconImage] drawInRect: [self bounds]
						fromRect: NSZeroRect
					       operation: NSCompositeSourceOver
						fraction: 1.0];

		if (hidden) return;

		NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

		NSRect	 badgeRect = [self bounds];

		badgeRect.origin.x    = NSWidth(badgeRect) - 0.4 * NSWidth(badgeRect) - 5.0;
		badgeRect.origin.y    = badgeRect.origin.y + 5.0;
		badgeRect.size.width  = 0.4 * NSWidth(badgeRect);
		badgeRect.size.height = (int) (0.4 * badgeRect.size.height);

		CGFloat	 badgeRadius = NSMidY(badgeRect);
		NSPoint	 badgeCenter = NSMakePoint(NSMidX(badgeRect), NSMidY(badgeRect));

		/* Draw the background.
		 */
		[NSGraphicsContext saveGraphicsState];

		NSColor		*bgrdColor	= [NSColor colorWithCalibratedRed: 0.85 green: 0.85 blue: 0.85 alpha: 1.0];
		NSColor		*bgrdHighlight	= [bgrdColor blendedColorWithFraction: 0.85 ofColor: [NSColor whiteColor]];
		NSGradient	*bgrdGradient	= [[[NSGradient alloc] initWithStartingColor: bgrdHighlight
										 endingColor: bgrdColor] autorelease];
		NSBezierPath	*badgeEdge	= [NSBezierPath bezierPathWithOvalInRect: badgeRect];

		[badgeEdge addClip];
		[bgrdGradient drawFromCenter: badgeCenter radius: 0.0
				    toCenter: badgeCenter radius: badgeRadius
				     options: 0];

		[NSGraphicsContext restoreGraphicsState];

		/* Compute the slice.
		 */
		NSColor		*sliceColor	= [NSColor colorWithCalibratedRed: 0.3 green: 0.55 blue: 0.8 alpha: 1.0];
		NSColor		*sliceHighlight	= [sliceColor blendedColorWithFraction: 0.4 ofColor: [NSColor whiteColor]];
		NSGradient	*sliceGradient	= [[[NSGradient alloc] initWithStartingColor: sliceHighlight
										 endingColor: sliceColor] autorelease];
		NSBezierPath	*progressSlice	= [NSBezierPath bezierPathWithOvalInRect: badgeRect];

		if (progress < 1.0)
		{
			CGFloat	 endAngle = 90.0 - 360.0 * progress;

			if (endAngle < 0.0) endAngle += 360.0;

			progressSlice = [NSBezierPath bezierPath];

			[progressSlice moveToPoint: badgeCenter];
			[progressSlice appendBezierPathWithArcWithCenter: badgeCenter
								  radius: badgeRadius
							      startAngle: 90.0
								endAngle: endAngle
							       clockwise: YES];
			[progressSlice closePath];
		}

		/* Draw the slice.
		 */
		[NSGraphicsContext saveGraphicsState];

		[progressSlice addClip];
		[sliceGradient drawFromCenter: badgeCenter radius: 0.0
				     toCenter: badgeCenter radius: badgeRadius
				      options: 0];

		[NSGraphicsContext restoreGraphicsState];

		/* Draw the edge.
		 */
		[NSGraphicsContext saveGraphicsState];

		[[NSColor whiteColor] set];

		NSShadow	*shadow = [[[NSShadow alloc] init] autorelease];

		[shadow setShadowOffset: NSMakeSize(0, -2)];
		[shadow setShadowBlurRadius: 2];
		[shadow set];

		[badgeEdge setLineWidth: 2];
		[badgeEdge stroke];

		[NSGraphicsContext restoreGraphicsState];

		[pool release];
	}

	- (void) setHidden:   (BOOL)  value { hidden   = value; }
	- (void) setProgress: (float) value { progress = value; }
@end

S::Int S::GUI::WindowCocoa::Initialize()
{
	/* Register for cursor events.
	 */
	Cursor::internalSetCursor.Connect(&WindowCocoa::SetCursor);
	Cursor::internalRemoveCursor.Connect(&WindowCocoa::RemoveCursor);

	return Success();
}

S::Int S::GUI::WindowCocoa::Free()
{
	/* Unregister cursor event handlers.
	 */
	Cursor::internalSetCursor.Disconnect(&WindowCocoa::SetCursor);
	Cursor::internalRemoveCursor.Disconnect(&WindowCocoa::RemoveCursor);

	return Success();
}

S::GUI::WindowCocoa::WindowCocoa(Void *iWindow)
{
	type		 = WINDOW_COCOA;

	wnd		 = nil;

	progressView	 = nil;

	wid		 = windowBackends.Add(this);

	minSize		 = Size(160, 24);
	maxSize		 = Size(32768, 32768);

	fontSize	 = Surface().GetSurfaceDPI() / 96.0;

	flags		 = 0;

	pasteBoard	 = nil;
	enableDropFiles	 = False;
}

S::GUI::WindowCocoa::~WindowCocoa()
{
	if (wnd != nil) [wnd release];

	if (progressView != nil) [progressView release];

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

S::Input::Keyboard::Key S::GUI::WindowCocoa::ConvertKey(Int keySym)
{
	Input::Keyboard::Key	 key = Input::Keyboard::KeyOther;

	switch (keySym)
	{
		case NSLeftArrowFunctionKey:	key = Input::Keyboard::KeyLeft;    break;
		case NSUpArrowFunctionKey:	key = Input::Keyboard::KeyUp;      break;
		case NSRightArrowFunctionKey:	key = Input::Keyboard::KeyRight;   break;
		case NSDownArrowFunctionKey:	key = Input::Keyboard::KeyDown;    break;

		case NSHomeFunctionKey:		key = Input::Keyboard::KeyHome;    break;
		case NSEndFunctionKey:		key = Input::Keyboard::KeyEnd;     break;
		case NSInsertFunctionKey:	key = Input::Keyboard::KeyInsert;  break;
		case NSDeleteFunctionKey:	key = Input::Keyboard::KeyDelete;  break;
		case NSPrevFunctionKey:		key = Input::Keyboard::KeyPrior;   break;
		case NSNextFunctionKey:		key = Input::Keyboard::KeyNext;    break;

		case NSEnterCharacter:
		case NSCarriageReturnCharacter: key = Input::Keyboard::KeyReturn;  break;

		case NSDeleteCharacter:		key = Input::Keyboard::KeyBack;    break;

		case NSTabCharacter:
		case NSBackTabCharacter:	key = Input::Keyboard::KeyTab;     break;

		case 0x20:			key = Input::Keyboard::KeySpace;   break;

		case 0x1B:			key = Input::Keyboard::KeyEscape;  break;

		case NSF1FunctionKey:		key = Input::Keyboard::KeyF1;      break;
		case NSF2FunctionKey:		key = Input::Keyboard::KeyF2;      break;
		case NSF3FunctionKey:		key = Input::Keyboard::KeyF3;      break;
		case NSF4FunctionKey:		key = Input::Keyboard::KeyF4;      break;
		case NSF5FunctionKey:		key = Input::Keyboard::KeyF5;      break;
		case NSF6FunctionKey:		key = Input::Keyboard::KeyF6;      break;
		case NSF7FunctionKey:		key = Input::Keyboard::KeyF7;      break;
		case NSF8FunctionKey:		key = Input::Keyboard::KeyF8;      break;
		case NSF9FunctionKey:		key = Input::Keyboard::KeyF9;      break;
		case NSF10FunctionKey:		key = Input::Keyboard::KeyF10;     break;
		case NSF11FunctionKey:		key = Input::Keyboard::KeyF11;     break;
		case NSF12FunctionKey:		key = Input::Keyboard::KeyF12;     break;
		case NSF13FunctionKey:		key = Input::Keyboard::KeyF13;     break;
		case NSF14FunctionKey:		key = Input::Keyboard::KeyF14;     break;
		case NSF15FunctionKey:		key = Input::Keyboard::KeyF15;     break;
		case NSF16FunctionKey:		key = Input::Keyboard::KeyF16;     break;
		case NSF17FunctionKey:		key = Input::Keyboard::KeyF17;     break;
		case NSF18FunctionKey:		key = Input::Keyboard::KeyF18;     break;
		case NSF19FunctionKey:		key = Input::Keyboard::KeyF19;     break;
		case NSF20FunctionKey:		key = Input::Keyboard::KeyF20;     break;
		case NSF21FunctionKey:		key = Input::Keyboard::KeyF21;     break;
		case NSF22FunctionKey:		key = Input::Keyboard::KeyF22;     break;
		case NSF23FunctionKey:		key = Input::Keyboard::KeyF23;     break;
		case NSF24FunctionKey:		key = Input::Keyboard::KeyF24;     break;
	}

	if	(keySym >= '0' && keySym <= '9') key = (Input::Keyboard::Key)  keySym;
	else if (keySym >= 'A' && keySym <= 'Z') key = (Input::Keyboard::Key)  keySym;
	else if (keySym >= 'a' && keySym <= 'z') key = (Input::Keyboard::Key) (keySym + ('A' - 'a'));

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
			if	([e type] == NSLeftMouseDown)			      onEvent.Call(SM_LBUTTONDOWN,   0, 0);
			else if ([e type] == NSLeftMouseUp)			      onEvent.Call(SM_LBUTTONUP,     0, 0);
			else if	([e type] == NSRightMouseDown)			      onEvent.Call(SM_RBUTTONDOWN,   0, 0);
			else if ([e type] == NSRightMouseUp)			      onEvent.Call(SM_RBUTTONUP,     0, 0);
			else							      onEvent.Call(SM_MOUSEMOVE,     0, 0);

			if	([e type] == NSLeftMouseDown  && [e clickCount] == 2) onEvent.Call(SM_LBUTTONDBLCLK, 0, 0);
			else if	([e type] == NSRightMouseDown && [e clickCount] == 2) onEvent.Call(SM_RBUTTONDBLCLK, 0, 0);

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
			if	([e type] == NSOtherMouseDown)			      { if ([e buttonNumber] == 2) onEvent.Call(SM_MBUTTONDOWN,	  0, 0); }
			else if ([e type] == NSOtherMouseUp)			      { if ([e buttonNumber] == 2) onEvent.Call(SM_MBUTTONUP,	  0, 0); }
			else											   onEvent.Call(SM_MOUSEMOVE,	  0, 0);

			if	([e type] == NSOtherMouseDown && [e clickCount] == 2) { if ([e buttonNumber] == 2) onEvent.Call(SM_MBUTTONDBLCLK, 0, 0); }

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
			if ([[e charactersIgnoringModifiers] length] >= 1)
			{
				NSString	*characters = [e charactersIgnoringModifiers];

				Input::Keyboard::UpdateKeyState(ConvertKey([characters characterAtIndex: 0]), True);

				onEvent.Call(SM_KEYDOWN, ConvertKey([characters characterAtIndex: 0]), 0);
			}

			break;

		case NSKeyUp:
			if ([[e charactersIgnoringModifiers] length] >= 1)
			{
				NSString	*characters = [e charactersIgnoringModifiers];

				Input::Keyboard::UpdateKeyState(ConvertKey([characters characterAtIndex: 0]), False);

				onEvent.Call(SM_KEYUP, ConvertKey([characters characterAtIndex: 0]), 0);
			}

			break;

		case NSFlagsChanged:
			Input::Keyboard::UpdateKeyState(Input::Keyboard::KeyShift,   [e modifierFlags] & NSShiftKeyMask);
			Input::Keyboard::UpdateKeyState(Input::Keyboard::KeyControl, [e modifierFlags] & NSControlKeyMask);
			Input::Keyboard::UpdateKeyState(Input::Keyboard::KeyCommand, [e modifierFlags] & NSCommandKeyMask);
			Input::Keyboard::UpdateKeyState(Input::Keyboard::KeyAlt,     [e modifierFlags] & NSAlternateKeyMask);

			if ([e modifierFlags] & NSShiftKeyMask)     onEvent.Call(SM_KEYDOWN, Input::Keyboard::KeyShift, 0);
			if ([e modifierFlags] & NSControlKeyMask)   onEvent.Call(SM_KEYDOWN, Input::Keyboard::KeyControl, 0);
			if ([e modifierFlags] & NSCommandKeyMask)   onEvent.Call(SM_KEYDOWN, Input::Keyboard::KeyCommand, 0);
			if ([e modifierFlags] & NSAlternateKeyMask) onEvent.Call(SM_KEYDOWN, Input::Keyboard::KeyAlt, 0);

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

			/* Keyboard events:
			 */
			if ([e subtype] == NSApplicationInsertText)
			{
				/* Call SM_CHAR event for each character in input string.
				 */
				NSString	*characters = (NSString *) [e data1];
				String		 string;

				string.ImportFrom("UTF-8", [characters UTF8String]);

				for (Int i = 0; i < string.Length(); i++) onEvent.Call(SM_CHAR, string[i], 0);
			}

			/* Drag & drop events:
			 */
			if ([e subtype] == NSApplicationDropFiles)
			{
				id	 info	  = (id) [e data1];
				Point	 position = Window::GetWindow(wnd)->GetMousePosition();

				pasteBoard = [info draggingPasteboard];

				onEvent.Call(SM_DROPFILES, position.x, position.y);
			}

			break;
	}

	/* Return from the active loop after processing an event.
	 */
	NSWindow	*modalWindow = [NSApp modalWindow];
	NSWindow	*keyWindow   = [NSApp keyWindow];

	if ((modalWindow == wnd && (keyWindow != nil && keyWindow != wnd && Window::GetWindow(keyWindow) != NIL)) ||
	    (keyWindow	 == wnd && (modalWindow == nil || ![modalWindow isVisible]))) [NSApp stop: nil];

	return Success();
}

S::Int S::GUI::WindowCocoa::Open(const String &title, const Point &pos, const Size &size, Int iFlags)
{
	flags = iFlags;

	NSRect		 contentRect = NSMakeRect(pos.x, [[NSScreen mainScreen] frame].size.height - (pos.y + Math::Round(size.cy * fontSize) + sizeModifier.cy), Math::Round(size.cx * fontSize) + sizeModifier.cx, Math::Round(size.cy * fontSize) + sizeModifier.cy);
	NSUInteger	 styleMask   = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;

	if (flags & WF_NORESIZE  ) styleMask ^= NSResizableWindowMask;
	if (flags & WF_THINBORDER) styleMask  = NSBorderlessWindowMask;

	wnd = [[CocoaWindow alloc] initWithContentRect: contentRect
					     styleMask: styleMask];

	if (wnd != nil)
	{
		/* We will explicitly release the window later.
		 */
		[wnd setReleasedWhenClosed: NO];

		/* Do not show the resize grip on older macOS versions.
		 */
		[wnd setShowsResizeIndicator: NO];

		/* Init content view and delegate.
		 */
		NSView	*contentView = [[[CocoaView alloc] initWithFrame: contentRect] autorelease];

		if ([NSThread isMainThread]) [wnd setContentView: contentView];
		else			     [wnd performSelectorOnMainThread: @selector(setContentView:) withObject: contentView waitUntilDone: YES];

		[wnd setDelegate: [[[CocoaWindowDelegate alloc] init] autorelease]];

		/* Create drawing surface.
		*/
		if ((flags & WF_THINBORDER) || (flags & WF_NORESIZE)) drawSurface = new Surface((Void *) wnd, size * fontSize + sizeModifier);
		else						      drawSurface = new Surface((Void *) wnd);

		drawSurface->SetSize(size * fontSize + sizeModifier);

		[wnd useOptimizedDrawing: YES];

		/* Set window title.
		 */
		SetTitle(title);

		/* Set metrics to actually allocated window.
		 */
		NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

		contentRect.origin.y = [[wnd screen] frame].size.height - (contentRect.origin.y + contentRect.size.height);

		onEvent.Call(SM_WINDOWMETRICS, ((	     Int(contentRect.origin.x)		     + 32768) << 16) | (	    Int(contentRect.origin.y)		     + 32768),
					       ((Math::Floor(Int(contentRect.size.width) / fontSize) + 32768) << 16) | (Math::Floor(Int(contentRect.size.height) / fontSize) + 32768));

		/* Set window level for topmost windows.
		 */
		if (flags & WF_TOPMOST) [wnd setLevel: NSPopUpMenuWindowLevel];

		/* Enable dropping files if requested.
		 */
		if (enableDropFiles) EnableDropFiles(True);

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

	/* Hide window.
	 */
	[wnd orderOut: nil];

	/* Emit onDestroy signal.
	 */
	onDestroy.Emit();

	/* Stop active loop after the window is closed.
	 */
	if	(flags & WF_MODAL)		[NSApp abortModal];
	else if (Window::nOfActiveWindows == 0) [NSApp stop: nil];

	return Success();
}

S::Int S::GUI::WindowCocoa::RequestClose()
{
	if (wnd == nil) return Success();

	if ([NSThread isMainThread]) [wnd performClose: nil];
	else			     [wnd performSelectorOnMainThread: @selector(performClose:) withObject: nil waitUntilDone: YES];

	return Success();
}

S::Int S::GUI::WindowCocoa::SetTitle(const String &nTitle)
{
	if (wnd == nil) return Error();

	NSString	*title = [NSString stringWithUTF8String: nTitle.ConvertTo("UTF-8")];

	if ([NSThread isMainThread]) [wnd setTitle: title];
	else			     [wnd performSelectorOnMainThread: @selector(setTitle:) withObject: title waitUntilDone: YES];

	return Success();
}

S::Int S::GUI::WindowCocoa::EnableDropFiles(Bool nEnableDropFiles)
{
	enableDropFiles = nEnableDropFiles;

	if (wnd != nil)
	{
		if (enableDropFiles) [wnd registerForDraggedTypes: [NSArray arrayWithObjects: NSFilenamesPboardType, nil]];
		else		     [wnd unregisterDraggedTypes];
	}

	return Success();
}

const S::Array<S::String> &S::GUI::WindowCocoa::GetDroppedFiles() const
{
	if (pasteBoard == nil) return WindowBackend::GetDroppedFiles();

	static Array<String>	 fileNames;

	fileNames.RemoveAll();

	/* Query dropped files.
	 */
	NSArray	*files = [pasteBoard propertyListForType: NSFilenamesPboardType];

	for (NSString *file in files)
	{
		String	 fileName;

		fileName.ImportFrom("UTF-8", [file UTF8String]);

		fileNames.Add(fileName);
	}

	return fileNames;
}

S::Int S::GUI::WindowCocoa::SetMinimumSize(const Size &nMinSize)
{
	minSize = nMinSize;

	if (wnd == nil) return Success();

	[(CocoaWindow *) wnd setMinSize: NSMakeSize(Math::Round(minSize.cx * fontSize) + sizeModifier.cx, Math::Round(minSize.cy * fontSize) + sizeModifier.cy)];

	NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

	if (contentRect.size.width  < Math::Round(minSize.cx * fontSize) + sizeModifier.cx || contentRect.size.height < Math::Round(minSize.cy * fontSize) + sizeModifier.cy)
	{
		[(CocoaWindow *) wnd setFrame: [wnd frameRectForContentRect: NSMakeRect(contentRect.origin.x, contentRect.origin.y + contentRect.size.height - Math::Max((Int) contentRect.size.height, (Int) Math::Round(minSize.cy * fontSize) + sizeModifier.cy), Math::Max((Int) contentRect.size.width, (Int) Math::Round(minSize.cx * fontSize) + sizeModifier.cx), Math::Max((Int) contentRect.size.height, (Int) Math::Round(minSize.cy * fontSize) + sizeModifier.cy))]];
	}

	return Success();
}

S::Int S::GUI::WindowCocoa::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize;

	if (wnd == nil) return Success();

	[(CocoaWindow *) wnd setMaxSize: NSMakeSize(Math::Round(maxSize.cx * fontSize) + sizeModifier.cx, Math::Round(maxSize.cy * fontSize) + sizeModifier.cy)];

	NSRect	 contentRect = [wnd contentRectForFrameRect: [wnd frame]];

	if (contentRect.size.width  > Math::Round(maxSize.cx * fontSize) + sizeModifier.cx || contentRect.size.height > Math::Round(maxSize.cy * fontSize) + sizeModifier.cy)
	{
		[(CocoaWindow *) wnd setFrame: [wnd frameRectForContentRect: NSMakeRect(contentRect.origin.x, contentRect.origin.y + contentRect.size.height - Math::Max((Int) contentRect.size.height, (Int) Math::Round(minSize.cy * fontSize) + sizeModifier.cy), Math::Max((Int) contentRect.size.width, (Int) Math::Round(minSize.cx * fontSize) + sizeModifier.cx), Math::Max((Int) contentRect.size.height, (Int) Math::Round(minSize.cy * fontSize) + sizeModifier.cy))]];
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
		SetMinimumSize(minSize);
		SetMaximumSize(maxSize);
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

	[(CocoaWindow *) wnd setFrame: [wnd frameRectForContentRect: NSMakeRect(nPos.x, [[wnd screen] frame].size.height - (nPos.y + Math::Round(nSize.cy * fontSize) + sizeModifier.cy), Math::Round(nSize.cx * fontSize) + sizeModifier.cx, Math::Round(nSize.cy * fontSize) + sizeModifier.cy)]];

	return Success();
}

S::Int S::GUI::WindowCocoa::Raise()
{
	if (wnd == nil) return Success();

	if ([NSThread isMainThread]) [wnd makeKeyAndOrderFront: nil];
	else			     [wnd performSelectorOnMainThread: @selector(makeKeyAndOrderFront:) withObject: nil waitUntilDone: YES];

	return Success();
}

S::Int S::GUI::WindowCocoa::SetProgressIndicator(Window::ProgressIndicatorState state, Float value)
{
	if (progressView == nil)
	{
		progressView = [[CocoaCircularProgress alloc] init];

		[[NSApp dockTile] setContentView: progressView];
	}

	CocoaCircularProgress	*circularProgress = (CocoaCircularProgress *) progressView;

	switch (state)
	{
		case Window::ProgressIndicatorNone:
			[circularProgress setHidden: YES];

			break;
		case Window::ProgressIndicatorNormal:
		case Window::ProgressIndicatorPaused:
			if (value >= 0) [circularProgress setProgress: value / 100];

			[circularProgress setHidden: NO];

			break;
	}

	if ([NSThread isMainThread]) [[NSApp dockTile] display];
	else			     [[NSApp dockTile] performSelectorOnMainThread: @selector(display) withObject: nil waitUntilDone: NO];

	return Success();
}

S::Void S::GUI::WindowCocoa::SetCursor(Cursor *cursor, const Point &point)
{
	CocoaWindow	*window = (CocoaWindow *) cursor->GetContainerWindow()->GetSystemWindow();

	if (window != NIL)
	{
		CocoaView	*view = (CocoaView *) [window contentView];

		[view setCursor: cursor position: point];
	}
}

S::Void S::GUI::WindowCocoa::RemoveCursor(Cursor *cursor)
{
	CocoaWindow	*window = (CocoaWindow *) cursor->GetContainerWindow()->GetSystemWindow();
	CocoaView	*view	= (CocoaView *) [window contentView];

	[view removeCursor: cursor];
}
