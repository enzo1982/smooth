 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BACKENDDIRECT2D
#define H_OBJSMOOTH_BACKENDDIRECT2D

namespace smooth
{
	namespace Backends
	{
		class BackendDirect2D;
	};
};

#include "../backend.h"
#include "../../misc/string.h"

#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <d2d1_1.h>
#include <dwrite.h>

namespace smooth
{
	namespace Backends
	{
		const Short	 BACKEND_DIRECT2D = 7;

		class BackendDirect2D : public Backend
		{
			private:
				static ID2D1Factory1		*d2dFactory;
				static IDWriteFactory		*dwriteFactory;

				static ID3D11Device		*d3dDevice;
				static ID2D1Device		*d2dDevice;
				static IDXGIFactory2		*dxgiFactory;

				static Bool			 warpDevice;

				static Bool			 CreateDevices();
			public:
				static ID2D1Factory1		*GetD2DFactory()	{ return d2dFactory; }
				static IDWriteFactory		*GetDWriteFactory()	{ return dwriteFactory; }

				/* Devices shared by all surfaces, created on first use.
				 */
				static ID3D11Device		*GetD3DDevice()		{ if (d3dDevice == NIL) CreateDevices(); return d3dDevice; }
				static ID2D1Device		*GetD2DDevice()		{ if (d3dDevice == NIL) CreateDevices(); return d2dDevice; }
				static IDXGIFactory2		*GetDXGIFactory()	{ if (d3dDevice == NIL) CreateDevices(); return dxgiFactory; }

				/* Get the sub-pixel precision of the rasterizer in use. Direct3D
				 * hardware uses 8 bits, the WARP software rasterizer was found to
				 * resolve only 4 bits when filling geometry.
				 */
				static Float			 GetRasterizerPrecision() { return warpDevice ? 1.0f / 16 : 1.0f / 256; }

				/* Release the devices after a device loss, so they are
				 * created anew on next use. Does nothing if the given
				 * device has already been replaced.
				 */
				static Void			 ReleaseDevices(ID2D1Device *);

				/* Create a text format for a font with the given
				 * height in pixels (equal to DIPs at 96 DPI).
				 */
				static IDWriteTextFormat	*CreateTextFormat(const String &, Int, Bool, Bool);

								 BackendDirect2D();
				virtual				~BackendDirect2D();

				Int				 Init();
				Int				 Deinit();
		};
	};
};

#endif
