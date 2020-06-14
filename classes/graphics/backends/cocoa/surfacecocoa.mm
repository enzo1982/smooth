 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cocoa/surfacecocoa.h>
#include <smooth/graphics/backends/cocoa/fontcocoa.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/gui/application/application.h>
#include <smooth/system/system.h>
#include <smooth/files/file.h>
#include <smooth/misc/math.h>
#include <smooth/backends/cocoa/backendcocoa.h>
#include <smooth/foreach.h>

S::GUI::SurfaceBackend *CreateSurfaceCocoa(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceCocoa(iSurface, maxSize);
}

S::Int		 surfaceCocoaTmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceCocoa);

namespace smooth
{
	namespace GUI
	{
		static NSColor	*NSColorForColor(const Color &color)
		{
			return [NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
							 green: color.GetGreen() / 255.0
							  blue: color.GetBlue()  / 255.0
							 alpha: 1.0];
		}
	}
}

S::Short	 S::GUI::SurfaceCocoa::surfaceDPI = -1;

S::GUI::SurfaceCocoa::SurfaceCocoa(Void *iWindow, const Size &maxSize)
{
	type		= SURFACE_COCOA;

	window		= (NSWindow *) iWindow;

	paintImage	= nil;
	paintBitmap	= nil;

	paintPending	= False;

	lockCount	= 0;

	if (window != NIL)
	{
		size = maxSize;

		if (maxSize == Size())
		{
			NSScreen	*screen = [NSScreen mainScreen];
			NSRect		 frame	= [screen visibleFrame];

			size.cx = frame.size.width;
			size.cy = frame.size.height;
		}

		fontSize.SetFontSize(GetSurfaceDPI());
		rightToLeft.SetSurfaceSize(size);

		paintImage  = [[NSImage alloc] initWithSize: NSMakeSize(size.cx, size.cy)];
		paintBitmap = [[[window contentView] bitmapImageRepForCachingDisplayInRect: NSMakeRect(0, 0, size.cx, size.cy)] retain];

		[paintImage addRepresentation: paintBitmap];

		paintRects.Add(Rect(Point(0, 0), size));

		[NSBezierPath setDefaultLineWidth: 0.0];

		allocSize = size;
	}
}

S::GUI::SurfaceCocoa::~SurfaceCocoa()
{
	if (window != NIL)
	{
		[paintBitmap release];
		[paintImage release];
	}
}

S::Int S::GUI::SurfaceCocoa::Lock()
{
	SurfaceBackend::Lock();

	if (++lockCount > 1 || window == NIL) return Success();

	/* Draw pending areas when this is the main thread.
	 */
	if (paintPending && [NSThread isMainThread]) PaintRect(paintPendingRect);

	/* Otherwise spin while waiting for painting to occur.
	 */
	while (paintPending)
	{
		lockCount--;

		SurfaceBackend::Release();

		Application::Lock::ResumeLock(Application::Lock::SuspendLock());

		SurfaceBackend::Lock();

		lockCount++;
	}

	/* Create autorelease pool.
	 */
	pool = [[NSAutoreleasePool alloc] init];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Release()
{
	if (--lockCount == 0 && window != NIL) [pool release];

	SurfaceBackend::Release();

	return Success();
}

S::Int S::GUI::SurfaceCocoa::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (window != NIL && !painting)
	{
		paintRects.RemoveAll();

		[paintBitmap release];
		[paintImage release];

		paintImage  = [[NSImage alloc] initWithSize: NSMakeSize(size.cx, size.cy)];
		paintBitmap = [[[window contentView] bitmapImageRepForCachingDisplayInRect: NSMakeRect(0, 0, size.cx, size.cy)] retain];

		[paintImage addRepresentation: paintBitmap];

		paintRects.Add(Rect(Point(0, 0), size));
	}

	allocSize = nSize;

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceCocoa::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceCocoa::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window == NIL || ![window isVisible]) return Success();

	NSView	*contentView = [window contentView];
	NSRect	 rect	     = NSMakeRect(pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top);

	if ([NSThread isMainThread])
	{
		if ([NSView focusView] == contentView)
		{
			/* Draw cached image if focussed.
			 */
			[paintImage drawInRect: rect
				      fromRect: rect
				     operation: NSCompositeCopy
				      fraction: 1.0];
		}
		else if (!Backends::BackendCocoa::IsOSXVersionAtLeast(10, 7, 0))
		{
			/* Lock content view and draw on older OS X versions.
			 */
			[contentView lockFocus];

			[paintImage drawInRect: rect
				      fromRect: rect
				     operation: NSCompositeCopy
				      fraction: 1.0];

			[window flushWindow];

			[contentView unlockFocus];
		}
		else
		{
			/* Otherwise call displayRect: if not focussed.
			 */
			[contentView displayRect: rect];
		}

		paintPending = False;
	}
	else
	{
		/* Call displayRect: on main thread if in another one.
		 */
		NSInvocation	*invocation = [NSInvocation invocationWithMethodSignature: [contentView methodSignatureForSelector: @selector(displayRect:)]];

		[invocation setTarget: contentView];
		[invocation setSelector: @selector(displayRect:)];
		[invocation setArgument: &rect
				atIndex: 2];
		[invocation retainArguments];

		[invocation performSelectorOnMainThread: @selector(invoke) withObject: nil waitUntilDone: NO];

		paintPending	 = True;
		paintPendingRect = pRect;
	}

	return Success();
}

S::Int S::GUI::SurfaceCocoa::StartPaint(const Rect &iPRect)
{
	if (window == NIL || ![window isVisible]) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), paintRects.GetLast());

	[NSGraphicsContext saveGraphicsState];

	if (painting == 0) [NSGraphicsContext setCurrentContext: [NSGraphicsContext graphicsContextWithBitmapImageRep: paintBitmap]];

	NSRectClip(NSMakeRect(pRect.left, pRect.top, pRect.GetWidth(), pRect.GetHeight()));

	paintRects.Add(pRect);

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceCocoa::EndPaint()
{
	if (!painting) return Error();

	painting--;

	Rect	 paintRect = paintRects.GetLast();

	paintRects.RemoveNth(paintRects.Length() - 1);

	[NSGraphicsContext restoreGraphicsState];

	if (painting == 0) PaintRect(paintRect);

	return Success();
}

S::Void *S::GUI::SurfaceCocoa::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceCocoa::GetSurfaceDPI() const
{
	if (Application::GetScaleFactor() != 0)	surfaceDPI = Math::Round(96.0 * Application::GetScaleFactor());

	if (surfaceDPI != -1) return surfaceDPI;

	Float	 dpi = 96.0;

	surfaceDPI = Math::Round(dpi * Setup::FontSize);

	return surfaceDPI;
}

S::Float S::GUI::SurfaceCocoa::GetScaleFactor() const
{
	/* Get screen information.
	 */
	static SEL	 scaleFactorSelector = @selector(backingScaleFactor);
	static Bool	 canQueryScaleFactor = [[NSScreen mainScreen] respondsToSelector: scaleFactorSelector];

	if (!canQueryScaleFactor || window == NIL || ![window isVisible]) return 1.0;

	NSScreen	*screen = [window screen];

	return ((CGFloat (*)(id, SEL)) [screen methodForSelector: scaleFactorSelector])(screen, scaleFactorSelector);
}

S::Int S::GUI::SurfaceCocoa::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL || ![window isVisible]) return Success();

	Point	 point	  = rightToLeft.TranslatePoint(iPoint);
	Bool	 endPaint = painting ? False : StartPaint(Rect(point, Size(1, 1))) == Success();

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	[NSColorForColor(color) set];

	NSRectFill(NSMakeRect(point.x, point.y, 1, 1));

	if (endPaint) EndPaint();

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL || ![window isVisible]) return Success();

	/* Convert coordinates.
	 */
	Point	 pos1	= rightToLeft.TranslatePoint(iPos1);
	Point	 pos2	= rightToLeft.TranslatePoint(iPos2);

	NSPoint	 point1 = NSMakePoint(pos1.x + 0.5, pos1.y + 0.5);
	NSPoint	 point2 = NSMakePoint(pos2.x + 0.5, pos2.y + 0.5);

	/* Adjust to Windows GDI behavior for diagonal lines.
	 */
	Float	 scaleFactor = GetScaleFactor();

	if (Math::Abs(pos2.x - pos1.x) == Math::Abs(pos2.y - pos1.y) && scaleFactor == 1.0)
	{
		if (pos1.x < pos2.x && pos1.y < pos2.y) { point2.x--; point2.y--; }
		if (pos1.x > pos2.x && pos1.y < pos2.y) { point2.x++; point2.y--; }
	}

	/* Adjust to Windows GDI behaviour for horizontal and vertical lines.
	 */
	if (point1.x < point2.x) { point1.x -= 0.5; point2.x -= 0.5; }
	if (point1.x > point2.x) { point1.x += 0.5; point2.x += 0.5; }

	if (point1.y < point2.y) { point1.y -= 0.5; point2.y -= 0.5; }
	if (point1.y > point2.y) { point1.y += 0.5; point2.y += 0.5; }

	Rect     rect;

	rect.left   = Math::Min(point1.x, point2.x);
	rect.top    = Math::Min(point1.y, point2.y);
	rect.right  = Math::Max(point1.x, point2.x) + 1;
	rect.bottom = Math::Max(point1.y, point2.y) + 1;

	Bool	 endPaint = painting ? False : StartPaint(rect) == Success();

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	[NSColorForColor(color) set];

	NSBezierPath	*path	= [NSBezierPath bezierPath];

	[path setLineWidth: 1.0];

	[path moveToPoint: point1];
	[path lineToPoint: point2];

	[path stroke];

	if (endPaint) EndPaint();

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL || ![window isVisible]) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			/* ToDo: Implement drawing of rounded boxes.
			 */
		}
		else
		{
			Bool	 endPaint = painting ? False : StartPaint(rect) == Success();

			[[NSGraphicsContext currentContext] setShouldAntialias: NO];

			[NSColorForColor(color) set];

			NSRectFill(NSMakeRect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight()));

			if (endPaint) EndPaint();
		}
	}
	else if (style == Rect::Outlined)
	{
		Bool	 endPaint = painting ? False : StartPaint(rect) == Success();

		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		[NSColorForColor(color) set];

		NSFrameRect(NSMakeRect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight()));

		if (endPaint) EndPaint();
	}
	else if (style & Rect::Inverted)
	{
		Float	 scaleFactor = GetScaleFactor();
		Bitmap	 area(rect.GetSize() * scaleFactor);

		BlitToBitmap(iRect, area, Rect(Point(0, 0), area.GetSize()));

		area.InvertColors();

		BlitFromBitmap(area, Rect(Point(0, 0), area.GetSize()), iRect);
	}
	else if (style & Rect::Dotted)
	{
		Bool	 endPaint = painting ? False : StartPaint(rect) == Success();

		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		[NSColorForColor(color) set];

		NSBezierPath	*path	    = [NSBezierPath bezierPathWithRect: NSMakeRect(rect.left + 0.5, rect.top + 0.5, rect.GetWidth() - 1, rect.GetHeight() - 1)];
		CGFloat		 pattern[2] = { 1.0, 1.0 };

		[path setLineDash: pattern count: 2 phase: 1];
		[path setLineWidth: 1.0];

		[path stroke];

		if (endPaint) EndPaint();
	}

	return Success();
}

S::Int S::GUI::SurfaceCocoa::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (window == NIL || ![window isVisible]) return Success();

	if (string == NIL)			  return Error();
	if (shadow)				  return SurfaceBackend::SetText(string, iRect, font, shadow);

	NSMutableDictionary	*attributes = [[NSMutableDictionary alloc] initWithObjectsAndKeys: FontCocoa::GetNativeFont(font),   NSFontAttributeName,
												   NSColorForColor(font.GetColor()), NSForegroundColorAttributeName, nil];

	if (font.GetStyle() & Font::Underline) [attributes setObject: [NSNumber numberWithInt: NSUnderlineStyleSingle] forKey: NSUnderlineStyleAttributeName];
	if (font.GetStyle() & Font::StrikeOut) [attributes setObject: [NSNumber numberWithInt: NSUnderlineStyleSingle] forKey: NSStrikethroughStyleAttributeName];

	Rect			 rect	    = iRect;
	Int			 lineHeight = font.GetScaledTextSizeY() + 3;

	const Array<String>	&lines	    = string.Explode("\n");

	/* Draw to window.
	 */
	Rect			 tRect	    = rightToLeft.TranslateRect(rect);
	NSAffineTransform	*transform  = [NSAffineTransform transform];

	Bool	 endPaint = painting ? False : StartPaint(rect) == Success();

	[[NSGraphicsContext currentContext] setShouldAntialias: YES];

	[NSGraphicsContext saveGraphicsState];

	NSRectClip(NSMakeRect(tRect.left, tRect.top, tRect.GetWidth(), tRect.GetHeight()));

	[transform scaleXBy: 1.0 yBy: -1.0];
	[transform concat];

	foreach (const String &line, lines)
	{
		tRect.left  = rightToLeft.GetRightToLeft() ? tRect.right - font.GetScaledTextSizeX(line) : tRect.left;
		tRect.top  += lineHeight;

		if (line != NIL)
		{
			NSString	*string = [NSString stringWithUTF8String: line.ConvertTo("UTF-8")];

			[string drawAtPoint: NSMakePoint(tRect.left, -1.0 * (tRect.top - 1))
				withAttributes: attributes];
		}
	}

	[NSGraphicsContext restoreGraphicsState];

	if (endPaint) EndPaint();

	[attributes release];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (window == NIL || ![window isVisible]) return Success();

	Rect	 rect	  = rightToLeft.TranslateRect(iRect);
	Bool	 endPaint = painting ? False : StartPaint(rect) == Success();

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	NSGradient	*gradient = [[NSGradient alloc] initWithStartingColor: NSColorForColor(color1)
								  endingColor: NSColorForColor(color2)];

	[gradient drawInRect: NSMakeRect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight())
		       angle: (style == OR_HORZ ? (rightToLeft.GetRightToLeft() ? 180 : 0.0) : 270)];

	[gradient release];

	if (endPaint) EndPaint();

	return Success();
}

S::Int S::GUI::SurfaceCocoa::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL || ![window isVisible]) return Success();
	if (bitmap == NIL)			  return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	NSImage	*image	  = (NSImage *) bitmap.GetSystemBitmap();
	Bool	 endPaint = painting ? False : StartPaint(destRect) == Success();

	[image drawInRect: NSMakeRect(destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight())
		 fromRect: NSMakeRect(srcRect.left, srcRect.top, srcRect.GetWidth(), srcRect.GetHeight())
		operation: NSCompositeCopy
		 fraction: 1.0];

	if (endPaint) EndPaint();

	return Success();
}

S::Int S::GUI::SurfaceCocoa::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL || ![window isVisible]) return Success();
	if (bitmap == NIL)			  return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Create context for destination bitmap.
	 */
	NSGraphicsContext	*context  = [NSGraphicsContext graphicsContextWithBitmapImageRep: (NSBitmapImageRep *) [[(NSImage *) bitmap.GetSystemBitmap() representations] objectAtIndex: 0]];

	[NSGraphicsContext saveGraphicsState];
	[NSGraphicsContext setCurrentContext: context];

	/* Transform to adjust for flipped destination context.
	 */
	NSAffineTransform	*transform = [NSAffineTransform transform];

	[transform scaleXBy:     1.0 yBy: -1.0];
	[transform translateXBy: 0.0 yBy: -1.0 * destRect.GetHeight()];

	[transform set];

	/* Copy drawing cache contents to destination context.
	 */
	[paintImage drawInRect: NSMakeRect(destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight())
		      fromRect: NSMakeRect(srcRect.left, srcRect.top, srcRect.GetWidth(), srcRect.GetHeight())
		     operation: NSCompositeCopy
		      fraction: 1.0];

	[(NSImage *) bitmap.GetSystemBitmap() recache];

	[NSGraphicsContext restoreGraphicsState];

	return Success();
}
