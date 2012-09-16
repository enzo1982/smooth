 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/multimonitor.h>
#include <smooth/input/pointer.h>
#include <smooth/init.h>

#if defined __WIN32__
#	include <windows.h>

	HMODULE user32dll = NIL;

	BOOL	 (*ex_GetMonitorInfo)(HMONITOR, LPMONITORINFO)	= NIL;
	HMONITOR (*ex_MonitorFromPoint)(POINT, DWORD)		= NIL;
#elif defined __HAIKU__
#	include <Screen.h>
#else
#	include <smooth/backends/xlib/backendxlib.h>

	using namespace X11;
#endif

S::Int	 addMultiMonitorInitTmp = S::AddInitFunction(&S::System::MultiMonitor::Initialize);
S::Int	 addMultiMonitorFreeTmp = S::AddFreeFunction(&S::System::MultiMonitor::Free);

S::System::MultiMonitor::MultiMonitor()
{
}

S::System::MultiMonitor::MultiMonitor(const MultiMonitor &)
{
}

S::Int S::System::MultiMonitor::Initialize()
{
#ifdef __WIN32__
	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if ((vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT      && vInfo.dwMajorVersion >= 5) || 
	    (vInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && vInfo.dwMinorVersion >= 1))
	{
		user32dll = LoadLibraryA("user32.dll");

		ex_GetMonitorInfo = (BOOL (*)(HMONITOR, LPMONITORINFO)) GetProcAddress(user32dll, "GetMonitorInfo");
		ex_MonitorFromPoint = (HMONITOR (*)(POINT, DWORD)) GetProcAddress(user32dll, "MonitorFromPoint");
	}
#endif

	return Success();
}

S::Int S::System::MultiMonitor::Free()
{
#ifdef __WIN32__
	if (user32dll != NIL)
	{
//		FreeLibrary(user32dll);

		user32dll = NIL;
	}
#endif

	return Success();
}

S::GUI::Rect S::System::MultiMonitor::GetActiveMonitorMetrics()
{
#if defined __WIN32__
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
#elif defined __HAIKU__
	BScreen	 screen;
	BRect	 frame = screen.Frame();

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(frame.right - frame.left + 1, frame.bottom - frame.top - 1));
#else
	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(Backends::BackendXLib::GetDisplay())), XHeightOfScreen(XDefaultScreenOfDisplay(Backends::BackendXLib::GetDisplay()))));
#endif
}

S::GUI::Rect S::System::MultiMonitor::GetActiveMonitorWorkArea()
{
#if defined __WIN32__
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
#elif defined __HAIKU__
	BScreen	 screen;
	BRect	 frame = screen.Frame();

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(frame.right - frame.left + 1, frame.bottom - frame.top - 1));
#else
	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(Backends::BackendXLib::GetDisplay())), XHeightOfScreen(XDefaultScreenOfDisplay(Backends::BackendXLib::GetDisplay()))));
#endif
}

S::GUI::Rect S::System::MultiMonitor::GetVirtualScreenMetrics()
{
#if defined __WIN32__
	if (user32dll == NIL || GetSystemMetrics(SM_CMONITORS) == 1)
	{
		return GUI::Rect(GUI::Point(), GUI::Size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
	}
	else
	{
		return GUI::Rect(GUI::Point(GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN)), GUI::Size(GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)));
	}
#elif defined __HAIKU__
	BScreen	 screen;
	BRect	 frame = screen.Frame();

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(frame.right - frame.left + 1, frame.bottom - frame.top - 1));
#else
	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(Backends::BackendXLib::GetDisplay())), XHeightOfScreen(XDefaultScreenOfDisplay(Backends::BackendXLib::GetDisplay()))));
#endif
}
