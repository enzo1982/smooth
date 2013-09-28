 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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

	paintBitmap = nil;

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

		paintBitmap = [[NSImage alloc] initWithSize: NSMakeSize(size.cx, size.cy)];

		paintRects.Add(new Rect(Point(0, 0), size));

		[NSBezierPath setDefaultLineWidth: 0.0];

		allocSize = size;
	}

	fontSize.SetFontSize(GetSurfaceDPI());
}

S::GUI::SurfaceCocoa::~SurfaceCocoa()
{
	if (window != NIL)
	{
		delete paintRects.GetFirst();

		[paintBitmap release];
	}
}

S::Int S::GUI::SurfaceCocoa::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (window != NIL && !painting)
	{
		delete paintRects.GetFirst();

		paintRects.RemoveAll();

		[paintBitmap setSize: NSMakeSize(size.cx, size.cy)];

		paintRects.Add(new Rect(Point(0, 0), size));
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

	if (window != NIL && [window isVisible])
	{
		[[window contentView] lockFocus];

		[paintBitmap drawInRect: NSMakeRect(pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top)
			       fromRect: NSMakeRect(pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top)
			      operation: NSCompositeCopy
			       fraction: 1.0];

		[[window contentView] unlockFocus];

		[window flushWindow];
	}

	return Success();
}

S::Int S::GUI::SurfaceCocoa::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), *(paintRects.GetLast()));

	if (painting == 0) [paintBitmap lockFocus];

	[NSGraphicsContext saveGraphicsState];

	[[NSBezierPath bezierPathWithRect: NSMakeRect(pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top)] addClip];

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

	if (painting == 0)
	{
		[paintBitmap unlockFocus];

		PaintRect(paintRect);
	}

	return Success();
}

S::Void *S::GUI::SurfaceCocoa::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceCocoa::GetSurfaceDPI() const
{
	if (surfaceDPI != -1) return surfaceDPI;

	Short	 dpi = 96;

	surfaceDPI = dpi;

	return dpi;
}

S::Int S::GUI::SurfaceCocoa::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(iPoint);

	if (!painting && [window isVisible])
	{
		[[window contentView] lockFocus];

		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
					   green: color.GetGreen() / 255.0
					    blue: color.GetBlue()  / 255.0
					   alpha: 1.0] set];

		[NSBezierPath fillRect: NSMakeRect(point.x, point.y, 1, 1)];

		[[window contentView] unlockFocus];

		[window flushWindow];
	}

	if (!painting) [paintBitmap lockFocus];

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
				   green: color.GetGreen() / 255.0
				    blue: color.GetBlue()  / 255.0
				   alpha: 1.0] set];

	[NSBezierPath fillRect: NSMakeRect(point.x, point.y, 1, 1)];

	if (!painting) [paintBitmap unlockFocus];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	/* Adjust to Windows GDI behavior for diagonal lines.
	 */
	if (Math::Abs(pos2.x - pos1.x) == Math::Abs(pos2.y - pos1.y))
	{
		if (pos1.x < pos2.x && pos1.y < pos2.y) { pos1.x++; pos1.y++; }
		if (pos1.x > pos2.x && pos1.y < pos2.y) { pos1.x--; pos1.y++; }
		if (pos1.x < pos2.x && pos1.y > pos2.y) { pos2.x--; pos2.y++; }
		if (pos1.x > pos2.x && pos1.y > pos2.y) { pos2.x++; pos2.y++; }
	}

	/* Adjust to Windows GDI behaviour for horizontal and vertical lines.
	 */
	if (pos1.y == pos2.y) { pos1.y++; pos2.y++; }
	if (pos1.x >  pos2.x) { pos1.x++; pos2.x++; }
	if (pos1.y >  pos2.y) { pos1.y++; pos2.y++; }

	if (!painting && [window isVisible])
	{
		[[window contentView] lockFocus];

		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
					   green: color.GetGreen() / 255.0
					    blue: color.GetBlue()  / 255.0
					   alpha: 1.0] set];

		NSBezierPath	*path	= [NSBezierPath bezierPath];

		[path setLineWidth: 0.0];

		[path moveToPoint: NSMakePoint(pos1.x, pos1.y)];
		[path lineToPoint: NSMakePoint(pos2.x, pos2.y)];

		[path stroke];

		[[window contentView] unlockFocus];

		[window flushWindow];
	}

	pos1 = rightToLeft.TranslatePoint(iPos1);
	pos2 = rightToLeft.TranslatePoint(iPos2);

	/* Adjust to Windows GDI behavior for diagonal lines.
	 */
	if (Math::Abs(pos2.x - pos1.x) == Math::Abs(pos2.y - pos1.y))
	{
		if (pos1.x < pos2.x && pos1.y < pos2.y) { pos2.x--; pos2.y--; }
		if (pos1.x > pos2.x && pos1.y < pos2.y) { pos2.x++; pos2.y--; }
	}

	/* Adjust to Windows GDI behaviour for horizontal and vertical lines.
	 */
	if (pos1.x >  pos2.x) { pos1.x++; pos2.x++; }
	if (pos1.y >  pos2.y) { pos1.y++; pos2.y++; }

	if (!painting) [paintBitmap lockFocus];

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
				   green: color.GetGreen() / 255.0
				    blue: color.GetBlue()  / 255.0
				   alpha: 1.0] set];

	NSBezierPath	*path	= [NSBezierPath bezierPath];

	[path setLineWidth: 0.0];

	[path moveToPoint: NSMakePoint(pos1.x, pos1.y)];
	[path lineToPoint: NSMakePoint(pos2.x, pos2.y)];

	[path stroke];

	if (!painting) [paintBitmap unlockFocus];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			if (!painting)
			{
/*				cairo_move_to(context, rect.left + ellipse.cx, rect.top);
				cairo_line_to(context, rect.right - ellipse.cx, rect.top);
				cairo_curve_to(context, rect.right, rect.top, rect.right, rect.top, rect.right, rect.top + ellipse.cy);
				cairo_line_to(context, rect.right, rect.bottom - ellipse.cy);
				cairo_curve_to(context, rect.right, rect.bottom, rect.right, rect.bottom, rect.right - ellipse.cx, rect.bottom);
				cairo_line_to(context, rect.left + ellipse.cx, rect.bottom);
				cairo_curve_to(context, rect.left, rect.bottom, rect.left, rect.bottom, rect.left, rect.bottom - ellipse.cy);
				cairo_line_to(context, rect.left, rect.top + ellipse.cy);
				cairo_curve_to(context, rect.left, rect.top, rect.left, rect.top, rect.left + ellipse.cx, rect.top);

				cairo_fill(context);
*/			}

/*			cairo_move_to(paintContextCairo, rect.left + ellipse.cx, rect.top);
			cairo_line_to(paintContextCairo, rect.right - ellipse.cx, rect.top);
			cairo_curve_to(paintContextCairo, rect.right, rect.top, rect.right, rect.top, rect.right, rect.top + ellipse.cy);
			cairo_line_to(paintContextCairo, rect.right, rect.bottom - ellipse.cy);
			cairo_curve_to(paintContextCairo, rect.right, rect.bottom, rect.right, rect.bottom, rect.right - ellipse.cx, rect.bottom);
			cairo_line_to(paintContextCairo, rect.left + ellipse.cx, rect.bottom);
			cairo_curve_to(paintContextCairo, rect.left, rect.bottom, rect.left, rect.bottom, rect.left, rect.bottom - ellipse.cy);
			cairo_line_to(paintContextCairo, rect.left, rect.top + ellipse.cy);
			cairo_curve_to(paintContextCairo, rect.left, rect.top, rect.left, rect.top, rect.left + ellipse.cx, rect.top);

			cairo_fill(paintContextCairo);
*/		}
		else
		{
			if (!painting && [window isVisible])
			{
				[[window contentView] lockFocus];

				[[NSGraphicsContext currentContext] setShouldAntialias: NO];

				[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
							   green: color.GetGreen() / 255.0
							    blue: color.GetBlue()  / 255.0
							   alpha: 1.0] set];

				[NSBezierPath fillRect: NSMakeRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top)];

				[[window contentView] unlockFocus];

				[window flushWindow];
			}

			if (!painting) [paintBitmap lockFocus];

			[[NSGraphicsContext currentContext] setShouldAntialias: NO];

			[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
						   green: color.GetGreen() / 255.0
						    blue: color.GetBlue()  / 255.0
						   alpha: 1.0] set];

			[NSBezierPath fillRect: NSMakeRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top)];

			if (!painting) [paintBitmap unlockFocus];
		}
	}
	else if (style == Rect::Outlined)
	{
		if (!painting && [window isVisible])
		{
			[[window contentView] lockFocus];

			[[NSGraphicsContext currentContext] setShouldAntialias: NO];

			[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
						   green: color.GetGreen() / 255.0
						    blue: color.GetBlue()  / 255.0
						   alpha: 1.0] set];

			[NSBezierPath strokeRect: NSMakeRect(rect.left, rect.top + 1, rect.right - rect.left - 1, rect.bottom - rect.top - 1)];

			[[window contentView] unlockFocus];

			[window flushWindow];
		}

		if (!painting) [paintBitmap lockFocus];

		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
					   green: color.GetGreen() / 255.0
					    blue: color.GetBlue()  / 255.0
					   alpha: 1.0] set];

		[NSBezierPath strokeRect: NSMakeRect(rect.left, rect.top, rect.right - rect.left - 1, rect.bottom - rect.top - 1)];

		if (!painting) [paintBitmap unlockFocus];
	}
	else if (style & Rect::Inverted)
	{
		Bitmap	 area(rect.right - rect.left, rect.bottom - rect.top);

		BlitToBitmap(iRect, area, Rect(Point(0, 0), area.GetSize()));

		area.InvertColors();

		BlitFromBitmap(area, Rect(Point(0, 0), area.GetSize()), iRect);
	}
	else if (style & Rect::Dotted)
	{
		if (!painting && [window isVisible])
		{
			[[window contentView] lockFocus];

			[[NSGraphicsContext currentContext] setShouldAntialias: NO];

			[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
						   green: color.GetGreen() / 255.0
						    blue: color.GetBlue()  / 255.0
						   alpha: 1.0] set];

			NSBezierPath	*path	    = [NSBezierPath bezierPathWithRect: NSMakeRect(rect.left, rect.top + 1, rect.right - rect.left - 1, rect.bottom - rect.top - 1)];
			CGFloat		 pattern[2] = { 1.0, 1.0 };

			[path setLineDash: pattern count: 2 phase: 1];

			[path stroke];

			[[window contentView] unlockFocus];

			[window flushWindow];
		}

		if (!painting) [paintBitmap lockFocus];

		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		[[NSColor colorWithCalibratedRed: color.GetRed()   / 255.0
					   green: color.GetGreen() / 255.0
					    blue: color.GetBlue()  / 255.0
					   alpha: 1.0] set];

		NSBezierPath	*path	    = [NSBezierPath bezierPathWithRect: NSMakeRect(rect.left, rect.top, rect.right - rect.left - 1, rect.bottom - rect.top - 1)];
		CGFloat		 pattern[2] = { 1.0, 1.0 };

		[path setLineDash: pattern count: 2 phase: 1];

		[path stroke];

		if (!painting) [paintBitmap unlockFocus];
	}

	return Success();
}

S::Int S::GUI::SurfaceCocoa::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (window == NIL)	return Success();

	if (string == NIL)	return Error();
	if (shadow)		return SurfaceBackend::SetText(string, iRect, font, shadow);

	NSMutableDictionary	*attributes = [NSMutableDictionary dictionaryWithCapacity: 2];

	NSFont			*nsFont	    = [[NSFontManager sharedFontManager] fontWithFamily: [NSString stringWithUTF8String: font.GetName().ConvertTo("UTF-8")]
											 traits: (font.GetStyle() & Font::Italic ? NSItalicFontMask : 0) | (font.GetWeight() >= Font::Bold ? NSBoldFontMask : 0)
											 weight: 5
											   size: font.GetSize() * fontSize.TranslateY(96) / 72.0];
	[attributes setObject: nsFont
		       forKey: NSFontAttributeName];

	NSColor			*nsColor    = [NSColor colorWithCalibratedRed: font.GetColor().GetRed()   / 255.0
									green: font.GetColor().GetGreen() / 255.0
									 blue: font.GetColor().GetBlue()  / 255.0
									alpha: 1.0];
	[attributes setObject: nsColor
		       forKey: NSForegroundColorAttributeName];

	Rect			 rect	    = iRect;
	Int			 lineHeight = font.GetScaledTextSizeY() + 3;

	const Array<String>	&lines	    = string.Explode("\n");

	/* Draw to window.
	 */
	if (!painting && [window isVisible])
	{
		Rect	 tRect = rightToLeft.TranslateRect(rect);

		[[window contentView] lockFocus];

		[[NSGraphicsContext currentContext] setShouldAntialias: YES];

		[NSGraphicsContext saveGraphicsState];

		[[NSBezierPath bezierPathWithRect: NSMakeRect(tRect.left, tRect.top, tRect.right - tRect.left, tRect.bottom - tRect.top)] addClip];

		foreach (const String &line, lines)
		{
			tRect	   = rightToLeft.TranslateRect(rect);
			tRect.left = rightToLeft.GetRightToLeft() ? tRect.right - font.GetScaledTextSizeX(line) : tRect.left;

			NSAttributedString	*string	= [[NSAttributedString alloc] initWithString: [NSString stringWithUTF8String: (line == NIL ? "" : line.ConvertTo("UTF-8"))]
											  attributes: attributes];

			[string drawAtPoint: NSMakePoint(tRect.left, tRect.top - 1)];

			[string release];

			rect.top += lineHeight;
		}

		[NSGraphicsContext restoreGraphicsState];

		[[window contentView] unlockFocus];

		[window flushWindow];
	}

	/* Draw to buffer.
	 */
	rect = iRect;

	Rect	 tRect = rightToLeft.TranslateRect(rect);

	if (!painting) [paintBitmap lockFocus];

	[[NSGraphicsContext currentContext] setShouldAntialias: YES];

	[NSGraphicsContext saveGraphicsState];

	[[NSBezierPath bezierPathWithRect: NSMakeRect(tRect.left, tRect.top, tRect.right - tRect.left, tRect.bottom - tRect.top)] addClip];

	foreach (const String &line, lines)
	{
		tRect	   = rightToLeft.TranslateRect(rect);
		tRect.left = rightToLeft.GetRightToLeft() ? tRect.right - font.GetScaledTextSizeX(line) : tRect.left;

		NSAttributedString	*string	   = [[NSAttributedString alloc] initWithString: [NSString stringWithUTF8String: (line == NIL ? "" : line.ConvertTo("UTF-8"))]
										     attributes: attributes];
		NSAffineTransform	*transform = [NSAffineTransform transform];

		[transform scaleXBy:	 1.0 yBy: -1.0];
		[transform translateXBy: 0.0 yBy: -2.0 * tRect.top - lineHeight];

		[transform set];

		[string drawAtPoint: NSMakePoint(tRect.left, tRect.top + 1)];

		[string release];

		rect.top += lineHeight;
	}

	[NSGraphicsContext restoreGraphicsState];

	if (!painting) [paintBitmap unlockFocus];

	String::ExplodeFinish();

	return Success();
}

S::Int S::GUI::SurfaceCocoa::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	if (!painting && [window isVisible])
	{
		[[window contentView] lockFocus];

		[[NSGraphicsContext currentContext] setShouldAntialias: NO];

		NSGradient	*gradient = [[NSGradient alloc] initWithStartingColor: [NSColor colorWithCalibratedRed: color1.GetRed()   / 255.0
														 green: color1.GetGreen() / 255.0
														  blue: color1.GetBlue()  / 255.0
														 alpha: 1.0]
									  endingColor: [NSColor colorWithCalibratedRed: color2.GetRed()   / 255.0
														 green: color2.GetGreen() / 255.0
														  blue: color2.GetBlue()  / 255.0
														 alpha: 1.0]];

		[gradient drawInRect: NSMakeRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top)
			       angle: (style == OR_HORZ ? (rightToLeft.GetRightToLeft() ? 180 : 0.0) : 270)];

		[gradient release];

		[[window contentView] unlockFocus];

		[window flushWindow];
	}

	if (!painting) [paintBitmap lockFocus];

	[[NSGraphicsContext currentContext] setShouldAntialias: NO];

	NSGradient	*gradient = [[NSGradient alloc] initWithStartingColor: [NSColor colorWithCalibratedRed: color1.GetRed()   / 255.0
													 green: color1.GetGreen() / 255.0
													  blue: color1.GetBlue()  / 255.0
													 alpha: 1.0]
								  endingColor: [NSColor colorWithCalibratedRed: color2.GetRed()   / 255.0
													 green: color2.GetGreen() / 255.0
													  blue: color2.GetBlue()  / 255.0
													 alpha: 1.0]];

	[gradient drawInRect: NSMakeRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top)
		       angle: (style == OR_HORZ ? (rightToLeft.GetRightToLeft() ? 180 : 0.0) : 270)];

	[gradient release];

	if (!painting) [paintBitmap unlockFocus];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);

	/* Create NSImage from bitmap.
	 */
	NSBitmapImageRep	*imageRep = (NSBitmapImageRep *) bitmap.GetSystemBitmap();
	NSImage			*image	  = [[NSImage alloc] initWithSize: NSMakeSize([imageRep pixelsWide], [imageRep pixelsHigh])];

	[image addRepresentation: imageRep];

	/* Draw the image.
	 */
	if (!painting && [window isVisible])
	{
		[[window contentView] lockFocus];

		[image drawInRect: NSMakeRect(destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top)
			 fromRect: NSMakeRect(srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top)
			operation: NSCompositeCopy
			 fraction: 1.0];

		[[window contentView] unlockFocus];

		[window flushWindow];
	}

	if (!painting) [paintBitmap lockFocus];

	[image drawInRect: NSMakeRect(destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top)
		 fromRect: NSMakeRect(srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top)
		operation: NSCompositeCopy
		 fraction: 1.0];

	if (!painting) [paintBitmap unlockFocus];

	/* Free NSImage.
	 */
	[image release];

	return Success();
}

S::Int S::GUI::SurfaceCocoa::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	/* Create NSBitmapImageRep from background bitmap.
	 */
	[paintBitmap lockFocus];

	NSBitmapImageRep	*destRep = (NSBitmapImageRep *) bitmap.GetSystemBitmap();
	NSBitmapImageRep	*srcRep	 = [[NSBitmapImageRep alloc] initWithFocusedViewRect: NSMakeRect(srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top)];

	[paintBitmap unlockFocus];

	/* Copy the image.
	 */
	NSAutoreleasePool	*pool	 = [[NSAutoreleasePool alloc] init];

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		for (Int x = 0; x < srcRect.GetWidth(); x++)
		{
			for (Int y = 0; y < srcRect.GetHeight(); y++)
			{
				[destRep setColor: [srcRep colorAtX: x y: y] atX: destRect.left + x y: destRect.top + y];
			}
		}
	}
	else
	{
		/* ToDo: Allow copying to bitmaps of different
		 *	 size than original.
		 */
	}

	[pool release];

	/* Free NSBitmapImageRep.
	 */
	[srcRep release];

	return Success();
}
