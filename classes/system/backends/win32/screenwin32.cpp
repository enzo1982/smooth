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

S::System::ScreenWin32::ScreenWin32()
{
	type = SCREEN_WIN32;
}

S::System::ScreenWin32::~ScreenWin32()
{
}

S::GUI::Rect S::System::ScreenWin32::GetActiveScreenMetrics()
{
	if (GetSystemMetrics(SM_CMONITORS) == 1)
	{
		return GUI::Rect(GUI::Point(0, 0), GUI::Size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
	}

	GUI::Point	 position = Input::Pointer::GetPosition();
	POINT		 pos	  = { position.x, position.y };
	HMONITOR	 monitor  = MonitorFromPoint(pos, 2);
	MONITORINFO	 info;

	info.cbSize = sizeof(MONITORINFO);

	GetMonitorInfo(monitor, &info);

	return GUI::Rect(GUI::Point(info.rcMonitor.left, info.rcMonitor.top), GUI::Size(info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top));
}

S::GUI::Rect S::System::ScreenWin32::GetActiveScreenWorkArea()
{
	if (GetSystemMetrics(SM_CMONITORS) == 1)
	{
		RECT rect;

		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

		return GUI::Rect(GUI::Point(rect.left, rect.top), GUI::Size(rect.right - rect.left, rect.bottom - rect.top));
	}

	GUI::Point	 position = Input::Pointer::GetPosition();
	POINT		 pos	  = { position.x, position.y };
	HMONITOR	 monitor  = MonitorFromPoint(pos, 2);
	MONITORINFO	 info;

	info.cbSize = sizeof(MONITORINFO);

	GetMonitorInfo(monitor, &info);

	return GUI::Rect(GUI::Point(info.rcWork.left, info.rcWork.top), GUI::Size(info.rcWork.right - info.rcWork.left, info.rcWork.bottom - info.rcWork.top));
}

S::GUI::Rect S::System::ScreenWin32::GetVirtualScreenMetrics()
{
	if (GetSystemMetrics(SM_CMONITORS) == 1)
	{
		return GUI::Rect(GUI::Point(), GUI::Size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
	}

	return GUI::Rect(GUI::Point(GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN)), GUI::Size(GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)));
}
