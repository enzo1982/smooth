 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cocoa/surfacecocoa.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/gui/application/application.h>
#include <smooth/system/system.h>
#include <smooth/files/file.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

S::GUI::SurfaceBackend *CreateSurfaceCocoa(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceCocoa(iSurface, maxSize);
}

S::Int		 surfaceCocoaTmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceCocoa);

S::Short	 S::GUI::SurfaceCocoa::surfaceDPI = -1;

S::GUI::SurfaceCocoa::SurfaceCocoa(Void *iWindow, const Size &maxSize)
{
	type	    = SURFACE_COCOA;

	window	    = (NSWindow *) iWindow;

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

		rightToLeft.SetSurfaceSize(size);
		upsideDown.SetSurfaceSize(size);

		paintRects.Add(new Rect(Point(0, 0), size));

		[NSBezierPath setDefaultLineWidth: 0.0];
	}

	fontSize.SetFontSize(GetSurfaceDPI());
}

S::GUI::SurfaceCocoa::~SurfaceCocoa()
{
	if (window != NIL) delete paintRects.GetFirst();
}

S::Int S::GUI::SurfaceCocoa::Lock()
{
	if (window != NIL && [window isVisible])
	{
		while ([[window contentView] inLiveResize] && ![NSThread isMainThread]) S::System::System::Sleep(10);

		[[window contentView] lockFocus];
	}

	return SurfaceBackend::Lock();
}

S::Int S::GUI::SurfaceCocoa::Release()
{
	if (window != NIL && [window isVisible])
	{
		[[window contentView] unlockFocus];
	}

	return SurfaceBackend::Release();
}

S::Int S::GUI::SurfaceCocoa::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);
	upsideDown.SetSurfaceSize(size);

	if (window != NIL && !painting)
	{
		delete paintRects.GetFirst();

		paintRects.RemoveAll();

		paintRects.Add(new Rect(Point(0, 0), size));
	}

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceCocoa::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceCocoa::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window != NIL && [window isVisible]) [window flushWindowIfNeeded];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::StartPaint(const Rect &iPRect)
{
	if (window == NIL || ![window isVisible]) return Success();

	Rect	 pRect = Rect::OverlapRect(upsideDown.TranslateRect(rightToLeft.TranslateRect(iPRect)), *(paintRects.GetLast()));

	[window disableFlushWindow];

	[NSGraphicsContext saveGraphicsState];

	[[NSBezierPath bezierPathWithRect: NSMakeRect(pRect.left, pRect.top, pRect.GetWidth(), pRect.GetHeight())] addClip];

	paintRects.Add(new Rect(pRect));

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceCocoa::EndPaint()
{
	if (!painting) return Error();

	painting--;

	Rect	 paintRect = *(paintRects.GetLast());

	delete paintRects.GetLast();

	paintRects.RemoveNth(paintRects.Length() - 1);

	[NSGraphicsContext restoreGraphicsState];

	[window enableFlushWindow];

	if (painting == 0) PaintRect(paintRect);

	return Success();
}

S::Void *S::GUI::SurfaceCocoa::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceCocoa::GetSurfaceDPI() const
{
	if (Application::GetScaleFactor() != 0)	surfaceDPI = 96 * Application::GetScaleFactor();

	if (surfaceDPI != -1) return surfaceDPI;

	Short	 dpi = 96;

	surfaceDPI = dpi;

	return dpi;
}

S::Int S::GUI::SurfaceCocoa::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL || ![window isVisible]) return Success();

	Point	 point = upsideDown.TranslatePoint(rightToLeft.TranslatePoint(iPoint));

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
				   green: color.GetGreen() / 255.0
				    blue: color.GetBlue()  / 255.0
				   alpha: 1.0] set];

	[NSBezierPath fillRect: NSMakeRect(point.x, point.y, 1, 1)];

	[window flushWindow];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL || ![window isVisible]) return Success();

	/* Get screen information.
	 */
	static SEL	 selScaleFactor	     = @selector(backingScaleFactor);
	static Bool	 canQueryScaleFactor = [[NSScreen mainScreen] respondsToSelector: selScaleFactor];

	NSScreen	*screen	     = [window screen];
	Float		 scaleFactor = canQueryScaleFactor ? ((CGFloat (*)(id, SEL)) [screen methodForSelector: selScaleFactor])(screen, selScaleFactor) : 1.0;

	/* Convert coordinates.
	 */
	Point	 pos1	= upsideDown.TranslatePoint(rightToLeft.TranslatePoint(iPos1));
	Point	 pos2	= upsideDown.TranslatePoint(rightToLeft.TranslatePoint(iPos2));

	NSPoint	 point1 = NSMakePoint(pos1.x + 0.5, pos1.y + 0.5);
	NSPoint	 point2 = NSMakePoint(pos2.x + 0.5, pos2.y + 0.5);

	/* Adjust to Windows GDI behavior for diagonal lines.
	 */
	if (Math::Abs(pos2.x - pos1.x) == Math::Abs(pos2.y - pos1.y) && scaleFactor == 1.0)
	{
		if (pos1.x < pos2.x && pos1.y < pos2.y) { point2.x--; point2.y--; }
		if (pos1.x > pos2.x && pos1.y < pos2.y) { point2.x++; point2.y--; }
		if (pos1.x < pos2.x && pos1.y > pos2.y) { point1.x++; point1.y--; }
		if (pos1.x > pos2.x && pos1.y > pos2.y) { point1.x--; point1.y--; }
	}

	/* Adjust to Windows GDI behaviour for horizontal and vertical lines.
	 */
	if (point1.x < point2.x) { point1.x -= 0.5; point2.x -= 0.5; }
	if (point1.x > point2.x) { point1.x += 0.5; point2.x += 0.5; }

	if (point1.y < point2.y) { point1.y -= 0.5; point2.y -= 0.5; }
	if (point1.y > point2.y) { point1.y += 0.5; point2.y += 0.5; }

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
				   green: color.GetGreen() / 255.0
				    blue: color.GetBlue()  / 255.0
				   alpha: 1.0] set];

	NSBezierPath	*path	= [NSBezierPath bezierPath];

	[path setLineWidth: 0.0];

	[path moveToPoint: point1];
	[path lineToPoint: point2];

	[path stroke];

	[window flushWindow];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL || ![window isVisible]) return Success();

	Rect	 rect = upsideDown.TranslateRect(rightToLeft.TranslateRect(iRect));

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			/* ToDo: Implement drawing of rounded boxes.
			 */
		}
		else
		{
			[[NSGraphicsContext currentContext] setShouldAntialias: NO];

			[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
						   green: color.GetGreen() / 255.0
						    blue: color.GetBlue()  / 255.0
						   alpha: 1.0] set];

			[NSBezierPath fillRect: NSMakeRect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight())];

			[window flushWindow];
		}
	}
	else if (style == Rect::Outlined)
	{
		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
					   green: color.GetGreen() / 255.0
					    blue: color.GetBlue()  / 255.0
					   alpha: 1.0] set];

		[NSBezierPath strokeRect: NSMakeRect(rect.left + 0.5, rect.top + 0.5, rect.GetWidth() - 1, rect.GetHeight() - 1)];

		[window flushWindow];
	}
	else if (style & Rect::Inverted)
	{
		Bitmap	 area(rect.GetSize());

		BlitToBitmap(iRect, area, Rect(Point(0, 0), area.GetSize()));

		area.InvertColors();

		BlitFromBitmap(area, Rect(Point(0, 0), area.GetSize()), iRect);
	}
	else if (style & Rect::Dotted)
	{
		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
					   green: color.GetGreen() / 255.0
					    blue: color.GetBlue()  / 255.0
					   alpha: 1.0] set];

		NSBezierPath	*path	    = [NSBezierPath bezierPathWithRect: NSMakeRect(rect.left + 0.5, rect.top + 0.5, rect.GetWidth() - 1, rect.GetHeight() - 1)];
		CGFloat		 pattern[2] = { 1.0, 1.0 };

		[path setLineDash: pattern count: 2 phase: 1];

		[path stroke];

		[window flushWindow];
	}

	return Success();
}

S::Int S::GUI::SurfaceCocoa::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (window == NIL || ![window isVisible]) return Success();

	if (string == NIL)			  return Error();
	if (shadow)				  return SurfaceBackend::SetText(string, iRect, font, shadow);

	NSFont			*nsFont	    = [[NSFontManager sharedFontManager] fontWithFamily: [NSString stringWithUTF8String: font.GetName().ConvertTo("UTF-8")]
											 traits: (font.GetStyle() & Font::Italic ? NSItalicFontMask : 0) | (font.GetWeight() >= Font::Bold ? NSBoldFontMask : 0)
											 weight: 5
											   size: font.GetSize() * fontSize.TranslateY(96) / 72.0];

	NSColor			*nsColor    = [NSColor colorWithCalibratedRed: font.GetColor().GetRed()   / 255.0
									green: font.GetColor().GetGreen() / 255.0
									 blue: font.GetColor().GetBlue()  / 255.0
									alpha: 1.0];

	NSMutableDictionary	*attributes = [[NSMutableDictionary alloc] initWithObjectsAndKeys: nsFont,  NSFontAttributeName,
												   nsColor, NSForegroundColorAttributeName, nil];

	Rect			 rect	    = iRect;
	Int			 lineHeight = font.GetScaledTextSizeY() + 3;

	const Array<String>	&lines	    = string.Explode("\n");

	/* Draw to window.
	 */
	Rect	 tRect = upsideDown.TranslateRect(rightToLeft.TranslateRect(rect));

	[[NSGraphicsContext currentContext] setShouldAntialias: YES];

	[NSGraphicsContext saveGraphicsState];

	[[NSBezierPath bezierPathWithRect: NSMakeRect(tRect.left, tRect.top, tRect.GetWidth(), tRect.GetHeight())] addClip];

	foreach (const String &line, lines)
	{
		tRect	   = upsideDown.TranslateRect(rightToLeft.TranslateRect(rect));
		tRect.left = rightToLeft.GetRightToLeft() ? tRect.right - font.GetScaledTextSizeX(line) : tRect.left;
		tRect.top  = tRect.bottom - lineHeight + 1;

		NSAttributedString	*string	= [[NSAttributedString alloc] initWithString: [NSString stringWithUTF8String: (line == NIL ? "" : line.ConvertTo("UTF-8"))]
										  attributes: attributes];

		[string drawAtPoint: NSMakePoint(tRect.left, tRect.top)];

		[string release];

		rect.top += lineHeight;
	}

	[NSGraphicsContext restoreGraphicsState];
	
	[window flushWindow];

	String::ExplodeFinish();

	[attributes release];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (window == NIL || ![window isVisible]) return Success();

	Rect	 rect = upsideDown.TranslateRect(rightToLeft.TranslateRect(iRect));

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	NSGradient	*gradient = [[NSGradient alloc] initWithStartingColor: [NSColor colorWithCalibratedRed: color1.GetRed()   / 255.0
													 green: color1.GetGreen() / 255.0
													  blue: color1.GetBlue()  / 255.0
													 alpha: 1.0]
								  endingColor: [NSColor colorWithCalibratedRed: color2.GetRed()   / 255.0
													 green: color2.GetGreen() / 255.0
													  blue: color2.GetBlue()  / 255.0
													 alpha: 1.0]];

	[gradient drawInRect: NSMakeRect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight())
		       angle: (style == OR_HORZ ? (rightToLeft.GetRightToLeft() ? 180 : 0.0) : 270)];

	[gradient release];

	[window flushWindow];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL || ![window isVisible]) return Success();
	if (bitmap == NIL)			  return Error();

	Rect	 destRect = upsideDown.TranslateRect(rightToLeft.TranslateRect(iDestRect));

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	NSImage	*image = (NSImage *) bitmap.GetSystemBitmap();

	[image drawInRect: NSMakeRect(destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight())
		 fromRect: NSMakeRect(srcRect.left, srcRect.top, srcRect.GetWidth(), srcRect.GetHeight())
		operation: NSCompositeCopy
		 fraction: 1.0];

	[window flushWindow];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL || ![window isVisible]) return Success();
	if (bitmap == NIL)			  return Error();

	Rect	 srcRect = upsideDown.TranslateRect(rightToLeft.TranslateRect(iSrcRect));

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Create image representing the area to be copied.
	 */
	NSBitmapImageRep	*imageRep = [[NSBitmapImageRep alloc] initWithFocusedViewRect: NSMakeRect(srcRect.left, srcRect.top, srcRect.GetWidth(), srcRect.GetHeight())];
	NSImage			*image	  = [[NSImage alloc] initWithSize: NSMakeSize([imageRep pixelsWide], [imageRep pixelsHigh])];

	[image addRepresentation: imageRep];

	/* Copy the image.
	 */
	NSGraphicsContext	*context  = [NSGraphicsContext graphicsContextWithBitmapImageRep: [[(NSImage *) bitmap.GetSystemBitmap() representations] objectAtIndex: 0]];

	[NSGraphicsContext saveGraphicsState];
	[NSGraphicsContext setCurrentContext: context];

	[image drawInRect: NSMakeRect(destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight())
		 fromRect: NSMakeRect(0, 0, srcRect.GetWidth(), srcRect.GetHeight())
		operation: NSCompositeCopy
		 fraction: 1.0];

	[(NSImage *) bitmap.GetSystemBitmap() recache];

	[NSGraphicsContext restoreGraphicsState];

	/* Free image and image representation.
	 */
	[image release];
	[imageRep release];

	return Success();
}
