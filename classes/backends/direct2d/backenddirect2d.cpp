 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/direct2d/backenddirect2d.h>
#include <smooth/backends/win32/backendwin32.h>
#include <smooth/backends/platform.h>

/* Direct2D, DirectWrite and Direct3D 11 are loaded at runtime.
 */
typedef HRESULT (WINAPI *D2D1CREATEFACTORY)(D2D1_FACTORY_TYPE, REFIID, const D2D1_FACTORY_OPTIONS *, void **);
typedef HRESULT (WINAPI *DWRITECREATEFACTORY)(DWRITE_FACTORY_TYPE, REFIID, IUnknown **);
typedef HRESULT (WINAPI *D3D11CREATEDEVICE)(IDXGIAdapter *, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL *, UINT, UINT, ID3D11Device **, D3D_FEATURE_LEVEL *, ID3D11DeviceContext **);

static HMODULE			 d2d1dll		 = NIL;
static HMODULE			 dwritedll		 = NIL;
static HMODULE			 d3d11dll		 = NIL;

static D2D1CREATEFACTORY	 ex_D2D1CreateFactory	 = NIL;
static DWRITECREATEFACTORY	 ex_DWriteCreateFactory	 = NIL;
static D3D11CREATEDEVICE	 ex_D3D11CreateDevice	 = NIL;

static S::Void FreeDLLs()
{
	if (d2d1dll   != NIL) FreeLibrary(d2d1dll);
	if (dwritedll != NIL) FreeLibrary(dwritedll);
	if (d3d11dll  != NIL) FreeLibrary(d3d11dll);

	d2d1dll	  = NIL;
	dwritedll = NIL;
	d3d11dll  = NIL;

	ex_D2D1CreateFactory   = NIL;
	ex_DWriteCreateFactory = NIL;
	ex_D3D11CreateDevice   = NIL;
}

static S::Bool LoadDLLs()
{
	if (d2d1dll != NIL) return S::True;

	d2d1dll	  = LoadLibraryEx(L"d2d1.dll",	 NIL, LOAD_LIBRARY_SEARCH_SYSTEM32);
	dwritedll = LoadLibraryEx(L"dwrite.dll", NIL, LOAD_LIBRARY_SEARCH_SYSTEM32);
	d3d11dll  = LoadLibraryEx(L"d3d11.dll",	 NIL, LOAD_LIBRARY_SEARCH_SYSTEM32);

	if (d2d1dll   != NIL) ex_D2D1CreateFactory   = (D2D1CREATEFACTORY) GetProcAddress(d2d1dll, "D2D1CreateFactory");
	if (dwritedll != NIL) ex_DWriteCreateFactory = (DWRITECREATEFACTORY) GetProcAddress(dwritedll, "DWriteCreateFactory");
	if (d3d11dll  != NIL) ex_D3D11CreateDevice   = (D3D11CREATEDEVICE) GetProcAddress(d3d11dll, "D3D11CreateDevice");

	if (ex_D2D1CreateFactory == NIL || ex_DWriteCreateFactory == NIL || ex_D3D11CreateDevice == NIL) { FreeDLLs(); return S::False; }

	return S::True;
}

S::Backends::Backend *CreateBackendDirect2D()
{
	return new S::Backends::BackendDirect2D();
}

static S::Bool ProbeDirect2D()
{
	/* Flip model swap chains and Direct2D 1.1 require Windows 8 or later.
	 */
	if (!S::Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2)) return S::False;

	return LoadDLLs();
}

/* Prefer Direct2D over GDI and GDI+ where it is supported.
 */
S::Int	 backendDirect2DTmp	 = S::Backends::Backend::AddBackend(&CreateBackendDirect2D);
S::Int	 backendDirect2DProbeTmp = S::Backends::Platform::AddProbe(S::Backends::PLATFORM_DIRECT2D, 3, &ProbeDirect2D);

ID2D1Factory1	*S::Backends::BackendDirect2D::d2dFactory    = NIL;
IDWriteFactory	*S::Backends::BackendDirect2D::dwriteFactory = NIL;

ID3D11Device	*S::Backends::BackendDirect2D::d3dDevice     = NIL;
ID2D1Device	*S::Backends::BackendDirect2D::d2dDevice     = NIL;
IDXGIFactory2	*S::Backends::BackendDirect2D::dxgiFactory   = NIL;

S::Bool		 S::Backends::BackendDirect2D::warpDevice    = S::False;

S::Bool S::Backends::BackendDirect2D::CreateDevices()
{
	if (d2dFactory == NIL) return False;

	/* Create a hardware Direct3D device or fall back to WARP.
	 */
	warpDevice = False;

	if (ex_D3D11CreateDevice(NIL, D3D_DRIVER_TYPE_HARDWARE, NIL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NIL, 0, D3D11_SDK_VERSION, &d3dDevice, NIL, NIL) != S_OK)
	{
		if (ex_D3D11CreateDevice(NIL, D3D_DRIVER_TYPE_WARP, NIL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NIL, 0, D3D11_SDK_VERSION, &d3dDevice, NIL, NIL) != S_OK) return False;

		warpDevice = True;
	}

	/* Create the Direct2D device and get the DXGI factory for swap chains.
	 */
	IDXGIDevice	*dxgiDevice = NIL;
	IDXGIAdapter	*adapter    = NIL;

	if (d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **) &dxgiDevice) == S_OK)
	{
		d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);

		if (dxgiDevice->GetAdapter(&adapter) == S_OK)
		{
			adapter->GetParent(__uuidof(IDXGIFactory2), (void **) &dxgiFactory);

			adapter->Release();
		}

		dxgiDevice->Release();
	}

	if (d2dDevice == NIL || dxgiFactory == NIL) { ReleaseDevices(d2dDevice); return False; }

	return True;
}

S::Void S::Backends::BackendDirect2D::ReleaseDevices(ID2D1Device *device)
{
	if (device != d2dDevice) return;

	if (dxgiFactory != NIL) dxgiFactory->Release();
	if (d2dDevice	!= NIL) d2dDevice->Release();
	if (d3dDevice	!= NIL) d3dDevice->Release();

	dxgiFactory = NIL;
	d2dDevice   = NIL;
	d3dDevice   = NIL;
}

IDWriteTextFormat *S::Backends::BackendDirect2D::CreateTextFormat(const String &name, Int height, Bool bold, Bool italic)
{
	IDWriteTextFormat	*format = NIL;

	if (dwriteFactory == NIL) return NIL;

	dwriteFactory->CreateTextFormat(name, NIL, bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL, italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, height, L"", &format);

	if (format != NIL) format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	return format;
}

S::Backends::BackendDirect2D::BackendDirect2D()
{
	type = BACKEND_DIRECT2D;
}

S::Backends::BackendDirect2D::~BackendDirect2D()
{
}

S::Int S::Backends::BackendDirect2D::Init()
{
	if (!Platform::IsSelected(PLATFORM_DIRECT2D)) return Success();

	if (!LoadDLLs()) return Error();

	/* Create Direct2D and DirectWrite factories.
	 */
	if (ex_D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1), NIL, (void **) &d2dFactory) != S_OK) return Error();
	if (ex_DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **) &dwriteFactory) != S_OK) return Error();

	return Success();
}

S::Int S::Backends::BackendDirect2D::Deinit()
{
	if (!Platform::IsSelected(PLATFORM_DIRECT2D)) return Success();

	ReleaseDevices(d2dDevice);

	if (dwriteFactory != NIL) dwriteFactory->Release();
	if (d2dFactory	  != NIL) d2dFactory->Release();

	dwriteFactory = NIL;
	d2dFactory    = NIL;

	FreeDLLs();

	return Success();
}
