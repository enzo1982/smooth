 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SURFACEDIRECT2D
#define H_OBJSMOOTH_SURFACEDIRECT2D

namespace smooth
{
	namespace GUI
	{
		class SurfaceDirect2D;
	};
};

#include "../surfacebackend.h"
#include "../../../misc/array.h"

#include <windows.h>
#include <dxgi1_2.h>
#include <d2d1_1.h>

namespace smooth
{
	namespace GUI
	{
		const Short	 SURFACE_DIRECT2D = 8;

		/* Direct2D surface with DirectWrite text rendering.
		 *
		 * Drawing goes to a backbuffer bitmap in video memory. Changed
		 * areas are copied to a flip model swap chain and presented with
		 * dirty rectangles. While painting, all drawing operations are
		 * batched until the paint ends.
		 *
		 * Surface coordinates are window coordinates, while the swap
		 * chain covers the client area only.
		 */
		class SurfaceDirect2D : public SurfaceBackend
		{
			private:
				static Short		 surfaceDPI;

				HWND			 window;

				Size			 allocSize;
				Array<Rect>		 paintRects;

				ID2D1Device		*device;
				ID2D1DeviceContext	*context;

				ID2D1Bitmap1		*backbuffer;

				IDXGISwapChain1		*swapChain;
				ID2D1Bitmap1		*swapBuffer;

				ID2D1Effect		*whiteFlood;

				Point			 clientOffset;

				Rect			 lastDirty;
				Int			 fullCopies;

				Bool			 drawing;

				Bool			 CreateResources();
				Void			 DestroyResources();
				Void			 RecreateResources();

				Bool			 CreateSwapBuffer();
				Void			 UpdateClientOffset();

				Void			 CopyToSwapBuffer(const Rect &);

				Void			 SuspendDraw();

				Void			 ReplaceClip(const Rect &);

				Bool			 BeginDraw();
				Void			 EndDraw();

				ID2D1SolidColorBrush	*CreateBrush(const Color &);
			public:
							 SurfaceDirect2D(Void * = NIL, const Size & = Size());
							~SurfaceDirect2D();

				Int			 SetSize(const Size &);

				Int			 PaintRect(const Rect &);

				Int			 StartPaint(const Rect &);
				Int			 EndPaint();

				Void			*GetSystemSurface() const;

				Short			 GetSurfaceDPI() const;

				Int			 SetPixel(const Point &, const Color &);

				Int			 Line(const Point &, const Point &, const Color &);
				Int			 Box(const Rect &, const Color &, Int, const Size &);

				Int			 SetText(const String &, const Rect &, const Font &, Bool);

				Int			 Gradient(const Rect &, const Color &, const Color &, Int);

				Int			 BlitFromBitmap(const Bitmap &, const Rect &, const Rect &);
				Int			 BlitToBitmap(const Rect &, Bitmap &, const Rect &);
		};
	};
};

#endif
