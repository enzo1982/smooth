 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/direct2d/surfacedirect2d.h>
#include <smooth/graphics/backends/direct2d/bitmapdirect2d.h>
#include <smooth/backends/direct2d/backenddirect2d.h>
#include <smooth/backends/platform.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/gui/application/application.h>
#include <smooth/basic/setup.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

S::GUI::SurfaceBackend *CreateSurfaceDirect2D(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceDirect2D(iSurface, maxSize);
}

static S::Int Register_surfaceDirect2DTmp() { return S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceDirect2D); }

S::Int	 surfaceDirect2DTmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_DIRECT2D, &Register_surfaceDirect2DTmp);

S::Short S::GUI::SurfaceDirect2D::surfaceDPI = -1;

S::GUI::SurfaceDirect2D::SurfaceDirect2D(Void *iWindow, const Size &maxSize)
{
	type	   = SURFACE_DIRECT2D;

	window	   = (HWND) iWindow;

	device	   = NIL;
	context	   = NIL;

	backbuffer = NIL;

	swapChain  = NIL;
	swapBuffer = NIL;

	whiteFlood = NIL;

	fullCopies = 0;
	drawing	   = False;

	if (window == NIL) return;

	/* Allocate the backbuffer at screen size by default,
	 * so it does not need to grow when resizing the window.
	 */
	size = maxSize;

	if (maxSize == Size())
	{
		HDC	 gdi_dc = GetWindowDC(window);

		size.cx	= GetDeviceCaps(gdi_dc, HORZRES) + 2;
		size.cy	= GetDeviceCaps(gdi_dc, VERTRES) + 2;

		ReleaseDC(window, gdi_dc);
	}

	fontSize.SetFontSize(GetSurfaceDPI());
	rightToLeft.SetSurfaceSize(size);

	paintRects.Add(Rect(Point(0, 0), size));

	allocSize = size;

	CreateResources();
}

S::GUI::SurfaceDirect2D::~SurfaceDirect2D()
{
	SuspendDraw();

	DestroyResources();
}

S::Bool S::GUI::SurfaceDirect2D::CreateResources()
{
	device = Backends::BackendDirect2D::GetD2DDevice();

	if (device == NIL) return False;

	/* Keep a reference, so the device stays identifiable after a device loss.
	 */
	device->AddRef();

	if (device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &context) != S_OK) { DestroyResources(); return False; }

	context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	/* Create the backbuffer and clear it to the background color.
	 */
	D2D1_BITMAP_PROPERTIES1	 properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));

	if (context->CreateBitmap(D2D1::SizeU(allocSize.cx, allocSize.cy), NIL, 0, properties, &backbuffer) != S_OK) { DestroyResources(); return False; }

	context->SetTarget(backbuffer);
	context->BeginDraw();
	context->Clear(D2D1::ColorF(Setup::BackgroundColor.GetRed() / 255.0f, Setup::BackgroundColor.GetGreen() / 255.0f, Setup::BackgroundColor.GetBlue() / 255.0f));
	context->EndDraw();

	/* Create a flip model swap chain for the window. Buffers have the
	 * size of the backbuffer and are not scaled to the client area.
	 */
	DXGI_SWAP_CHAIN_DESC1	 description = { 0 };

	description.Width	     = allocSize.cx;
	description.Height	     = allocSize.cy;
	description.Format	     = DXGI_FORMAT_B8G8R8A8_UNORM;
	description.SampleDesc.Count = 1;
	description.BufferUsage	     = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.BufferCount	     = 2;
	description.Scaling	     = DXGI_SCALING_NONE;
	description.SwapEffect	     = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	description.AlphaMode	     = DXGI_ALPHA_MODE_IGNORE;

	IDXGIFactory2	*factory = Backends::BackendDirect2D::GetDXGIFactory();

	if (factory->CreateSwapChainForHwnd(Backends::BackendDirect2D::GetD3DDevice(), window, &description, NIL, NIL, &swapChain) != S_OK) { DestroyResources(); return False; }

	factory->MakeWindowAssociation(window, DXGI_MWA_NO_WINDOW_CHANGES);

	if (!CreateSwapBuffer()) { DestroyResources(); return False; }

	/* Opaque white flood used to invert colors.
	 */
	if (context->CreateEffect(CLSID_D2D1Flood, &whiteFlood) != S_OK) { DestroyResources(); return False; }

	whiteFlood->SetValue(D2D1_FLOOD_PROP_COLOR, D2D1::Vector4F(1.0f, 1.0f, 1.0f, 1.0f));

	/* Copy the whole backbuffer to both swap chain buffers first.
	 */
	fullCopies = 2;
	lastDirty  = Rect();

	return True;
}

S::Void S::GUI::SurfaceDirect2D::DestroyResources()
{
	if (whiteFlood != NIL) whiteFlood->Release();
	if (swapBuffer != NIL) swapBuffer->Release();
	if (swapChain  != NIL) swapChain->Release();
	if (backbuffer != NIL) backbuffer->Release();
	if (context    != NIL) context->Release();
	if (device     != NIL) device->Release();

	whiteFlood = NIL;
	swapBuffer = NIL;
	swapChain  = NIL;
	backbuffer = NIL;
	context	   = NIL;
	device	   = NIL;

	drawing	   = False;
}

S::Void S::GUI::SurfaceDirect2D::RecreateResources()
{
	/* Replace the lost device and recreate all resources. The
	 * backbuffer content is lost, so repaint the whole window.
	 */
	Backends::BackendDirect2D::ReleaseDevices(device);

	DestroyResources();
	CreateResources();

	InvalidateRect(window, NIL, False);
}

S::Bool S::GUI::SurfaceDirect2D::CreateSwapBuffer()
{
	/* With flip model swap chains, buffer 0 always refers
	 * to the current back buffer.
	 */
	IDXGISurface	*surface = NIL;

	if (swapChain->GetBuffer(0, __uuidof(IDXGISurface), (void **) &surface) != S_OK) return False;

	D2D1_BITMAP_PROPERTIES1	 properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
	HRESULT			 result	    = context->CreateBitmapFromDxgiSurface(surface, properties, &swapBuffer);

	surface->Release();

	return (result == S_OK);
}

S::Void S::GUI::SurfaceDirect2D::UpdateClientOffset()
{
	/* Find the client area origin in window coordinates.
	 */
	RECT	 windowRect;
	POINT	 origin = { 0, 0 };

	GetWindowRect(window, &windowRect);
	ClientToScreen(window, &origin);

	clientOffset = Point(origin.x - windowRect.left, origin.y - windowRect.top);
}

S::Void S::GUI::SurfaceDirect2D::CopyToSwapBuffer(const Rect &iArea)
{
	/* Copy an area given in client coordinates from the backbuffer.
	 */
	Rect	 area = Rect::OverlapRect(Rect::OverlapRect(iArea, Rect(Point(0, 0), allocSize)), Rect(Point(0, 0), allocSize) - clientOffset);

	if (area.GetWidth() <= 0 || area.GetHeight() <= 0) return;

	D2D1_POINT_2U	 point	= D2D1::Point2U(area.left, area.top);
	D2D1_RECT_U	 source = D2D1::RectU(area.left + clientOffset.x, area.top + clientOffset.y, area.right + clientOffset.x, area.bottom + clientOffset.y);

	swapBuffer->CopyFromBitmap(&point, backbuffer, &source);
}

S::Bool S::GUI::SurfaceDirect2D::BeginDraw()
{
	if (context == NIL) return False;

	/* Start drawing if not already batching. While painting,
	 * clip to the current paint rectangle.
	 */
	if (!drawing)
	{
		context->BeginDraw();

		if (painting)
		{
			const Rect	&clip = paintRects.GetLast();

			context->PushAxisAlignedClip(D2D1::RectF(clip.left, clip.top, clip.right, clip.bottom), D2D1_ANTIALIAS_MODE_ALIASED);
		}

		drawing = True;
	}

	return True;
}

S::Void S::GUI::SurfaceDirect2D::EndDraw()
{
	/* Continue the batch while painting.
	 */
	if (!painting) SuspendDraw();
}

S::Void S::GUI::SurfaceDirect2D::SuspendDraw()
{
	/* Finish pending drawing operations.
	 */
	if (!drawing) return;

	if (painting) context->PopAxisAlignedClip();

	drawing = False;

	if (context->EndDraw() == D2DERR_RECREATE_TARGET) RecreateResources();
}

S::Void S::GUI::SurfaceDirect2D::ReplaceClip(const Rect &clip)
{
	context->PopAxisAlignedClip();
	context->PushAxisAlignedClip(D2D1::RectF(clip.left, clip.top, clip.right, clip.bottom), D2D1_ANTIALIAS_MODE_ALIASED);
}

S::Int S::GUI::SurfaceDirect2D::SetSize(const Size &nSize)
{
	SurfaceBackend::SetSize(nSize);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (window == NIL || painting || context == NIL) return Success();

	/* Grow the backbuffer, keeping its content.
	 */
	SuspendDraw();

	Size			 newSize    = Size(Math::Max(allocSize.cx, nSize.cx), Math::Max(allocSize.cy, nSize.cy));
	D2D1_BITMAP_PROPERTIES1	 properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
	ID2D1Bitmap1		*newBuffer  = NIL;

	if (context->CreateBitmap(D2D1::SizeU(newSize.cx, newSize.cy), NIL, 0, properties, &newBuffer) != S_OK) return Error();

	D2D1_POINT_2U	 point	= D2D1::Point2U(0, 0);
	D2D1_RECT_U	 source = D2D1::RectU(0, 0, allocSize.cx, allocSize.cy);

	newBuffer->CopyFromBitmap(&point, backbuffer, &source);

	context->SetTarget(newBuffer);

	backbuffer->Release();
	backbuffer = newBuffer;

	/* Resize the swap chain buffers.
	 */
	swapBuffer->Release();
	swapBuffer = NIL;

	if (swapChain->ResizeBuffers(0, newSize.cx, newSize.cy, DXGI_FORMAT_UNKNOWN, 0) != S_OK || !CreateSwapBuffer()) { RecreateResources(); return Error(); }

	fullCopies = 2;

	paintRects.RemoveAll();
	paintRects.Add(Rect(Point(0, 0), size));

	allocSize = newSize;

	return Success();
}

S::Int S::GUI::SurfaceDirect2D::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window == NIL || swapChain == NIL) return Success();

	UpdateClientOffset();

	/* The back buffer of the swap chain holds the frame before the last
	 * one, so update it with the areas changed in the last frame and now.
	 * Initially, copy the whole backbuffer to both swap chain buffers.
	 */
	Rect	 dirty = Rect::OverlapRect(pRect - clientOffset, Rect(Point(0, 0), allocSize));

	if (fullCopies > 0) { dirty = Rect(Point(0, 0), allocSize); fullCopies--; }

	if (dirty.GetWidth() <= 0 || dirty.GetHeight() <= 0) return Success();

	CopyToSwapBuffer(lastDirty);
	CopyToSwapBuffer(dirty);

	RECT			 dirtyRect  = { dirty.left, dirty.top, dirty.right, dirty.bottom };
	DXGI_PRESENT_PARAMETERS	 parameters = { 1, &dirtyRect, NIL, NIL };
	HRESULT			 result	    = swapChain->Present1(0, 0, &parameters);

	lastDirty = dirty;

	if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET) RecreateResources();

	return Success();
}

S::Int S::GUI::SurfaceDirect2D::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), paintRects.GetLast());

	paintRects.Add(pRect);

	painting++;

	/* Clip to the new paint rectangle if already drawing.
	 */
	if (drawing) ReplaceClip(pRect);

	return Success();
}

S::Int S::GUI::SurfaceDirect2D::EndPaint()
{
	if (!painting) return Error();

	/* Finish drawing at the end of the outermost paint, before
	 * presenting. Otherwise restore clipping to the enclosing
	 * paint rectangle.
	 */
	if	(painting == 1) SuspendDraw();
	else if (drawing)	ReplaceClip(paintRects.GetNth(paintRects.Length() - 2));

	painting--;

	if (painting == 0) PaintRect(paintRects.GetLast());

	paintRects.RemoveNth(paintRects.Length() - 1);

	return Success();
}

S::Void *S::GUI::SurfaceDirect2D::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceDirect2D::GetSurfaceDPI() const
{
	if (Application::GetScaleFactor() != 0)	surfaceDPI = Math::Round(96.0 * Application::GetScaleFactor());

	if (surfaceDPI != -1) return surfaceDPI;

	surfaceDPI = Math::Round(96.0 * Setup::FontSize);

	return surfaceDPI;
}

ID2D1SolidColorBrush *S::GUI::SurfaceDirect2D::CreateBrush(const Color &color)
{
	ID2D1SolidColorBrush	*brush = NIL;

	context->CreateSolidColorBrush(D2D1::ColorF(color.GetRed() / 255.0f, color.GetGreen() / 255.0f, color.GetBlue() / 255.0f), &brush);

	return brush;
}

S::Int S::GUI::SurfaceDirect2D::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(iPoint);
	Rect	 rect  = Rect(point, Size(1, 1));

	if (BeginDraw())
	{
		ID2D1SolidColorBrush	*brush = CreateBrush(color);

		context->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush);

		brush->Release();

		EndDraw();
	}

	if (!painting) PaintRect(rect);

	return Success();
}

S::Int S::GUI::SurfaceDirect2D::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	Int	 dx    = Math::Abs(pos2.x - pos1.x);
	Int	 dy    = Math::Abs(pos2.y - pos1.y);
	Int	 major = Math::Max(dx, dy);

	if (major == 0) return Success();

	Rect	 rect  = Rect(Point(Math::Min(pos1.x, pos2.x), Math::Min(pos1.y, pos2.y)), Size(dx + 1, dy + 1));

	if (BeginDraw())
	{
		/* Aliased Direct2D strokes do not cover exactly one pixel per
		 * step along the major axis for arbitrary slopes. Instead, fill
		 * a parallelogram from the first to the last pixel along the
		 * major axis that is exactly one pixel wide along the minor
		 * axis. Windows GDI does not draw the end point of a line, so
		 * the last pixel is one step before it.
		 *
		 * Moving the parallelogram towards the top or left decides for
		 * pixel centers on its edges like GDI does. Other pixel centers
		 * are at least 1/(2 * major) pixels away from the edges, so move
		 * it by half of that, but not less than the rasterizer resolves.
		 *
		 * The parallelogram is drawn as a rectangle one pixel wide along
		 * the minor axis, skewed by the slope of the line.
		 */
		Bool	 xMajor = (dx >= dy);
		Int	 start	= xMajor ? pos1.x : pos1.y;
		Int	 end	= xMajor ? pos2.x : pos2.y;
		Int	 last	= end - (end > start ? 1 : -1);

		Float	 slope	= xMajor ? Float(pos2.y - pos1.y) / (pos2.x - pos1.x) : Float(pos2.x - pos1.x) / (pos2.y - pos1.y);
		Float	 shift	= Math::Max(0.25f / major, Backends::BackendDirect2D::GetRasterizerPrecision());

		Float	 low	= Math::Min(start, last);
		Float	 high	= Math::Max(start, last) + 1;

		/* Position of the top or left edge along the minor axis at major coordinate 0.
		 */
		Float	 offset = (xMajor ? pos1.y : pos1.x) - shift - (start + 0.5f) * slope;

		ID2D1SolidColorBrush	*brush = CreateBrush(color);

		context->SetTransform(xMajor ? D2D1::Matrix3x2F(1, slope, 0, 1, 0, offset) : D2D1::Matrix3x2F(1, 0, slope, 1, offset, 0));
		context->FillRectangle(xMajor ? D2D1::RectF(low, 0, high, 1) : D2D1::RectF(0, low, 1, high), brush);
		context->SetTransform(D2D1::Matrix3x2F::Identity());

		brush->Release();

		EndDraw();
	}

	if (!painting) PaintRect(rect);

	return Success();
}

S::Int S::GUI::SurfaceDirect2D::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	if (BeginDraw())
	{
		ID2D1SolidColorBrush	*brush = CreateBrush(color);

		if (style & Rect::Filled)
		{
			if (style & Rect::Rounded)
			{
				context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
				context->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), ellipse.cx / 2.0f, ellipse.cy / 2.0f), brush);
				context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
			}
			else
			{
				context->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush);
			}
		}
		else if (style == Rect::Outlined)
		{
			context->FillRectangle(D2D1::RectF(rect.left,	 rect.top,	  rect.right,	  rect.top + 1), brush);
			context->FillRectangle(D2D1::RectF(rect.left,	 rect.bottom - 1, rect.right,	  rect.bottom),	 brush);
			context->FillRectangle(D2D1::RectF(rect.left,	 rect.top,	  rect.left + 1,  rect.bottom),	 brush);
			context->FillRectangle(D2D1::RectF(rect.right - 1, rect.top,	  rect.right,	  rect.bottom),	 brush);
		}
		else if (style & Rect::Inverted)
		{
			/* Composing an opaque image in mask invert mode inverts the destination.
			 */
			D2D1_POINT_2F	 corner = D2D1::Point2F(rect.left, rect.top);
			D2D1_RECT_F	 area	= D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);

			context->DrawImage(whiteFlood, &corner, &area, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1_COMPOSITE_MODE_MASK_INVERT);
		}
		else if (style & Rect::Dotted)
		{
			for (Int x = rect.left						    + 1;  x <  rect.right;  x += 2) context->FillRectangle(D2D1::RectF(x, rect.top, x + 1, rect.top + 1), brush);
			for (Int y = rect.top	 - (rect.GetWidth()		      ) % 2 + 2;  y <  rect.bottom; y += 2) context->FillRectangle(D2D1::RectF(rect.right - 1, y, rect.right, y + 1), brush);
			for (Int x = rect.right	 - (rect.GetWidth() + rect.GetHeight()) % 2 - 2;  x >= rect.left;   x -= 2) context->FillRectangle(D2D1::RectF(x, rect.bottom - 1, x + 1, rect.bottom), brush);
			for (Int y = rect.bottom - (		      rect.GetHeight()) % 2 - 1;  y >= rect.top;    y -= 2) context->FillRectangle(D2D1::RectF(rect.left, y, rect.left + 1, y + 1), brush);
		}

		brush->Release();

		EndDraw();
	}

	if (!painting) PaintRect(rect);

	return Success();
}

S::Int S::GUI::SurfaceDirect2D::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (window == NIL) return Success();

	if (string == NIL) return Error();
	if (shadow)	   return SurfaceBackend::SetText(string, iRect, font, shadow);

	Rect	 rect	    = iRect;
	Int	 lineHeight = 0;

	/* Set up DirectWrite text format with the same pixel height GDI would use.
	 */
	Int			 height = Math::Round(font.GetSize() * fontSize.TranslateY(96) / 72.0);
	IDWriteTextFormat	*format = Backends::BackendDirect2D::CreateTextFormat(font.GetName(), height, font.GetWeight() >= Font::Bold, font.GetStyle() & Font::Italic);

	if (format == NIL) return Error();

	/* Draw text line by line.
	 */
	const Array<String>	&lines = string.Explode("\n");

	if (lines.Length() > 1) lineHeight = font.GetScaledTextSizeY() + 3;

	Rect	 updateRect = rightToLeft.TranslateRect(Rect(rect.GetPosition(), Size(rect.GetWidth(), rect.GetHeight() + lineHeight * (lines.Length() - 1))));

	if (BeginDraw())
	{
		ID2D1SolidColorBrush	*brush = CreateBrush(font.GetColor());

		foreach (const String &line, lines)
		{
			Int	 lineLength = line.Length();

			/* Check for right to left characters in text.
			 */
			Bool	 rtlCharacters = False;

			for (Int i = 0; i < lineLength; i++) if (line[i] >= 0x0590 && line[i] <= 0x08FF) { rtlCharacters = True; break; }

			/* Create text layout and draw text.
			 */
			Rect			 tRect	= rightToLeft.TranslateRect(rect);
			IDWriteTextLayout	*layout = NIL;

			Backends::BackendDirect2D::GetDWriteFactory()->CreateGdiCompatibleTextLayout(line, lineLength, format, tRect.GetWidth(), tRect.GetHeight(), 1.0, NIL, True, &layout);

			if (layout != NIL)
			{
				DWRITE_TEXT_RANGE	 range = { 0, (UINT32) lineLength };

				/* Align text to the right on right to left surfaces.
				 */
				layout->SetReadingDirection(rtlCharacters ? DWRITE_READING_DIRECTION_RIGHT_TO_LEFT : DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
				layout->SetTextAlignment(rightToLeft.GetRightToLeft() == rtlCharacters ? DWRITE_TEXT_ALIGNMENT_LEADING : DWRITE_TEXT_ALIGNMENT_TRAILING);

				layout->SetUnderline(font.GetStyle() & Font::Underline, range);
				layout->SetStrikethrough(font.GetStyle() & Font::StrikeOut, range);

				context->DrawTextLayout(D2D1::Point2F(tRect.left, tRect.top), layout, brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);

				layout->Release();
			}

			rect.top += lineHeight;
		}

		brush->Release();

		EndDraw();
	}

	format->Release();

	if (!painting) PaintRect(updateRect);

	return Success();
}

S::Int S::GUI::SurfaceDirect2D::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	/* Setup colors.
	 */
	Color	 c1   = (style == OR_HORZ && rightToLeft.GetRightToLeft()) ? color2 : color1;
	Color	 c2   = (style == OR_HORZ && rightToLeft.GetRightToLeft()) ? color1 : color2;

	if (BeginDraw())
	{
		/* Setup Direct2D objects and draw gradient.
		 */
		D2D1_GRADIENT_STOP		 stops[2]   = { { 0.0f, D2D1::ColorF(c1.GetRed() / 255.0f, c1.GetGreen() / 255.0f, c1.GetBlue() / 255.0f) },
							    { 1.0f, D2D1::ColorF(c2.GetRed() / 255.0f, c2.GetGreen() / 255.0f, c2.GetBlue() / 255.0f) } };
		ID2D1GradientStopCollection	*collection = NIL;
		ID2D1LinearGradientBrush	*brush	    = NIL;

		context->CreateGradientStopCollection(stops, 2, &collection);
		context->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(style == OR_HORZ ? rect.right  : rect.left,
																	       style == OR_VERT ? rect.bottom : rect.top)), collection, &brush);

		context->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush);

		brush->Release();
		collection->Release();

		EndDraw();
	}

	if (!painting) PaintRect(rect);

	return Success();
}

S::Int S::GUI::SurfaceDirect2D::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	BitmapBackend	*backend = GetBitmapBackend(bitmap);

	if (backend->GetBitmapType() != BITMAP_DIRECT2D) return Error();

	/* Scale the bitmap ourselves for better quality.
	 */
	Size	 bitmapSize = bitmap.GetSize();
	Point	 offset	    = srcRect.GetPosition();

	if (destRect.GetSize() != srcRect.GetSize())
	{
		Float	 scaleFactorX = Float(srcRect.GetWidth()) / Float(destRect.GetWidth());
		Float	 scaleFactorY = Float(srcRect.GetHeight()) / Float(destRect.GetHeight());

		bitmapSize = Size(Float(bitmapSize.cx) / scaleFactorX, Float(bitmapSize.cy) / scaleFactorY);
		offset	   = Point(Float(srcRect.left) / scaleFactorX, Float(srcRect.top) / scaleFactorY);
	}

	if (BeginDraw())
	{
		/* Draw the part of the cached device bitmap that lies within
		 * the bitmap. Pixels outside of the bitmap are transparent.
		 */
		ID2D1Bitmap	*d2dBitmap = ((BitmapDirect2D *) backend)->GetDeviceBitmap(context, bitmapSize);
		Rect		 source	   = Rect::OverlapRect(Rect(offset, destRect.GetSize()), Rect(Point(0, 0), bitmapSize));

		if (d2dBitmap != NIL && source.GetWidth() > 0 && source.GetHeight() > 0)
		{
			Rect	 target = source + (destRect.GetPosition() - offset);

			context->DrawBitmap(d2dBitmap, D2D1::RectF(target.left, target.top, target.right, target.bottom), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(source.left, source.top, source.right, source.bottom));
		}

		EndDraw();
	}

	if (!painting) PaintRect(destRect);

	return Success();
}

namespace smooth
{
	namespace GUI
	{
		/* Bitmaps are GDI DIB sections, stored bottom-up with 24 or 32 bits
		 * per pixel in BGR(A) order and rows aligned to 4 bytes.
		 */
		static UnsignedByte *GetBitmapRow(const Bitmap &bitmap, Int y)
		{
			const Size	&size	= bitmap.GetSize();
			Int		 stride = (size.cx * (bitmap.GetDepth() / 8) + 3) & ~3;

			return bitmap.GetBytes() + (size.cy - y - 1) * stride;
		}

		/* Write a row of BGRA pixels as opaque pixels, skipping
		 * pixels outside of the bitmap.
		 */
		static Void WriteBitmapRow(Bitmap &bitmap, Int x, Int y, const UnsignedInt32 *pixels, Int count)
		{
			const Size	&size		= bitmap.GetSize();
			Int		 bytesPerPixel	= bitmap.GetDepth() / 8;

			if (y >= size.cy || bitmap.GetBytes() == NIL) return;

			UnsignedByte	*row = GetBitmapRow(bitmap, y);

			for (Int i = 0; i < count; i++, x++)
			{
				if (x < 0 || x >= size.cx) continue;

				UnsignedByte	*pixel = row + x * bytesPerPixel;

				pixel[0] = pixels[i]	   & 255;
				pixel[1] = pixels[i] >>  8 & 255;
				pixel[2] = pixels[i] >> 16 & 255;

				if (bytesPerPixel == 4) pixel[3] = 255;
			}
		}
	};
};

S::Int S::GUI::SurfaceDirect2D::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	if (context == NIL) return Error();

	Rect	 area = Rect::OverlapRect(srcRect, paintRects.GetFirst());

	if (area.GetWidth() <= 0 || area.GetHeight() <= 0) return Success();

	/* Finish pending drawing operations and copy
	 * the area to a CPU readable bitmap.
	 */
	SuspendDraw();

	ID2D1Bitmap1	*copy	= NIL;
	HRESULT		 result = context->CreateBitmap(D2D1::SizeU(area.GetWidth(), area.GetHeight()), NIL, 0, D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &copy);

	if (result == S_OK)
	{
		D2D1_POINT_2U	 origin = D2D1::Point2U(0, 0);
		D2D1_RECT_U	 source = D2D1::RectU(area.left, area.top, area.right, area.bottom);

		result = copy->CopyFromBitmap(&origin, backbuffer, &source);
	}

	D2D1_MAPPED_RECT	 mapped;

	if (result == S_OK) result = copy->Map(D2D1_MAP_OPTIONS_READ, &mapped);

	if (result != S_OK)
	{
		if (copy != NIL) copy->Release();

		return Error();
	}

	/* Convert pixels, scaling them if necessary. The alpha channel of
	 * the backbuffer is undefined, so make all pixels opaque.
	 */
	Bool	 scale	= (destRect.GetSize() != srcRect.GetSize());
	Bitmap	 temp	= scale ? Bitmap(srcRect.GetSize(), 32) : Bitmap();
	Bitmap	&target = scale ? temp : bitmap;
	Point	 offset = (scale ? Point() : destRect.GetPosition()) + (area.GetPosition() - srcRect.GetPosition());

	for (Int y = 0; y < area.GetHeight(); y++) WriteBitmapRow(target, offset.x, offset.y + y, (UnsignedInt32 *) (mapped.bits + y * mapped.pitch), area.GetWidth());

	copy->Unmap();
	copy->Release();

	/* The scaled bitmap has 32 bits per pixel like
	 * the temporary one, so its rows are BGRA.
	 */
	if (scale)
	{
		Bitmap	 scaled = target.Scale(destRect.GetSize());

		for (Int y = 0; y < destRect.GetHeight(); y++) WriteBitmapRow(bitmap, destRect.left, destRect.top + y, (UnsignedInt32 *) GetBitmapRow(scaled, y), destRect.GetWidth());
	}

	return Success();
}
