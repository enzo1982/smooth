 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/win32/screenwin32.h>
#include <smooth/backends/win32/backendwin32.h>
#include <smooth/input/pointer.h>
#include <smooth/init.h>

S::System::ScreenBackend *CreateScreenWin32()
{
	return new S::System::ScreenWin32();
}

S::Int	 screenWin32Tmp = S::System::ScreenBackend::SetBackend(&CreateScreenWin32);

S::Int	 addScreenWin32InitTmp = S::AddInitFunction(&S::System::ScreenWin32::Initialize);
S::Int	 addScreenWin32FreeTmp = S::AddFreeFunction(&S::System::ScreenWin32::Free);

static HMODULE	 user32dll = NIL;

static BOOL	 (*ex_GetMonitorInfo)(HMONITOR, LPMONITORINFO);
static HMONITOR	 (*ex_MonitorFromPoint)(POINT, DWORD);

S::Int S::System::ScreenWin32::Initialize()
{
	if (Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT,	5) ||
	    Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_WINDOWS, 4, 1))
	{
		user32dll = LoadLibraryA("user32.dll");

		ex_GetMonitorInfo = (BOOL (*)(HMONITOR, LPMONITORINFO)) GetProcAddress(user32dll, "GetMonitorInfo");
		ex_MonitorFromPoint = (HMONITOR (*)(POINT, DWORD)) GetProcAddress(user32dll, "MonitorFromPoint");
	}

	return Success();
}

S::Int S::System::ScreenWin32::Free()
{
	if (user32dll != NIL)
	{
//		FreeLibrary(user32dll);

		user32dll = NIL;
	}

	return Success();
}

S::System::ScreenWin32::ScreenWin32()
{
	type = SCREEN_WIN32;
}

S::System::ScreenWin32::~ScreenWin32()
{
}

S::GUI::Rect S::System::ScreenWin32::GetActiveScreenMetrics()
{
	if (user32dll == NIL || GetSystemMetrics(SM_CMONITORS) == 1)
	{
		return GUI::Rect(GUI::Point(0, 0), GUI::Size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
	}
	else
	{
		GUI::Point	 position = Input::Pointer::GetPosition();
		POINT		 pos	  = { position.x, position.y };
		HMONITOR	 monitor  = ex_MonitorFromPoint(pos, 2);
		MONITORINFO	 info;

		info.cbSize = sizeof(MONITORINFO);

		GetMonitorInfo(monitor, &info);

		return GUI::Rect(GUI::Point(info.rcMonitor.left, info.rcMonitor.top), GUI::Size(info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top));
	}
}

S::GUI::Rect S::System::ScreenWin32::GetActiveScreenWorkArea()
{
	if (user32dll == NIL || GetSystemMetrics(SM_CMONITORS) == 1)
	{
		RECT rect;

		if (Setup::enableUnicode) SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
		else			  SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

		return GUI::Rect(GUI::Point(rect.left, rect.top), GUI::Size(rect.right - rect.left, rect.bottom - rect.top));
	}
	else
	{
		GUI::Point	 position = Input::Pointer::GetPosition();
		POINT		 pos	  = { position.x, position.y };
		HMONITOR	 monitor  = ex_MonitorFromPoint(pos, 2);
		MONITORINFO	 info;

		info.cbSize = sizeof(MONITORINFO);

		GetMonitorInfo(monitor, &info);

		return GUI::Rect(GUI::Point(info.rcWork.left, info.rcWork.top), GUI::Size(info.rcWork.right - info.rcWork.left, info.rcWork.bottom - info.rcWork.top));
	}
}

S::GUI::Rect S::System::ScreenWin32::GetVirtualScreenMetrics()
{
	if (user32dll == NIL || GetSystemMetrics(SM_CMONITORS) == 1)
	{
		return GUI::Rect(GUI::Point(), GUI::Size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
	}
	else
	{
		return GUI::Rect(GUI::Point(GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN)), GUI::Size(GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)));
	}
}
