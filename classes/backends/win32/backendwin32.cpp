 /* The smooth Class Library
  * Copyright (C) 1998-2023 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/win32/backendwin32.h>
#include <smooth/files/directory.h>
#include <smooth/gui/application/application.h>

#include <shlobj.h>
#include <iconv.h>

#define MAX_EXT_PATH 32768

size_t	 (*iconv)(iconv_t, char **, size_t *, char **, size_t *) = NIL;
iconv_t	 (*iconv_open)(const char *, const char *)		 = NIL;
int	 (*iconv_close)(iconv_t)				 = NIL;
int	 (*iconvctl)(iconv_t, int, void *)			 = NIL;

HINSTANCE	 iconvDLL     = NIL;
HINSTANCE	 hDllInstance = NIL;

S::Bool S::LoadIconvDLL()
{
	iconvDLL	= LoadLibrary(GUI::Application::GetApplicationDirectory().Append("iconv"));

	iconv		= (size_t (*)(iconv_t, char **, size_t *, char **, size_t *)) GetProcAddress(iconvDLL, "libiconv");
	iconv_open	= (iconv_t (*)(const char *, const char *)) GetProcAddress(iconvDLL, "libiconv_open");
	iconv_close	= (int (*)(iconv_t)) GetProcAddress(iconvDLL, "libiconv_close");
	iconvctl	= (int (*)(iconv_t, int, void *)) GetProcAddress(iconvDLL, "libiconvctl");

	if (iconv	== NIL ||
	    iconv_open	== NIL ||
	    iconv_close == NIL ||
	    iconvctl	== NIL)	{ FreeIconvDLL(); return False; }

	return True;
}

S::Void S::FreeIconvDLL()
{
	FreeLibrary(iconvDLL);

	iconv		= NIL;
	iconv_open	= NIL;
	iconv_close	= NIL;
	iconvctl	= NIL;
}

#ifndef SMOOTH_STATIC

extern "C"
{
	BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
	{
		switch (reason)
		{
			case DLL_PROCESS_ATTACH:
				hDllInstance = hInst;

				break;
			case DLL_PROCESS_DETACH:
				break;
			case DLL_THREAD_ATTACH:
				break;
			case DLL_THREAD_DETACH:
				break;
		}

		return true;
	}
}

#endif

static HMODULE	 user32dll = NIL;

typedef BOOL (WINAPI* SETPROCESSDPIAWARE)();

static SETPROCESSDPIAWARE	 ex_SetProcessDPIAware = NIL;

S::Backends::Backend *CreateBackendWin32()
{
	return new S::Backends::BackendWin32();
}

S::Int	 backendWin32Tmp = S::Backends::Backend::AddBackend(&CreateBackendWin32);

S::Backends::BackendWin32::BackendWin32()
{
	type = BACKEND_WIN32;
}

S::Backends::BackendWin32::~BackendWin32()
{
}

S::Int S::Backends::BackendWin32::Init()
{
	/* Init COM library.
	 */
	OleInitialize(NIL);

	/* Init Windows sockets.
	 */
	WORD	 wVersionRequested = MAKEWORD(2,2);
	WSADATA	 wsaData;

	WSAStartup(wVersionRequested, &wsaData);

	/* Declare the process DPI aware.
	 */
	user32dll = LoadLibrary(L"user32.dll");

	ex_SetProcessDPIAware = (SETPROCESSDPIAWARE) GetProcAddress(user32dll, "SetProcessDPIAware");

	if (ex_SetProcessDPIAware != NIL) ex_SetProcessDPIAware();

	/* Get default colors.
	 */
	UpdateColors();

	/* Get mouse hover time and tolerance.
	 */
	SystemParametersInfo(SPI_GETMOUSEHOVERTIME, 0, &Setup::HoverTime, 0);
	SystemParametersInfo(SPI_GETMOUSEHOVERWIDTH, 0, &Setup::HoverWidth, 0);
	SystemParametersInfo(SPI_GETMOUSEHOVERHEIGHT, 0, &Setup::HoverHeight, 0);

	return Success();
}

S::Int S::Backends::BackendWin32::Deinit()
{
	/* Free dynamically loaded user32 library.
	 */
	FreeLibrary(user32dll);

	/* Cleanup Windows sockets.
	 */
	WSACleanup();

	/* Uninit COM library.
	 */
	OleUninitialize();

	return Success();
}

S::Void S::Backends::BackendWin32::UpdateColors()
{
	/* Query dark mode and accent color on Windows 10.
	 */
	Bool		 darkModeEnabled = False;

	GUI::Color	 colorizationColor(-1);

	GUI::Color	 accentColor(-1);
	Bool		 accentColorPrevalence = False;

	HKEY		 key;

	if (RegOpenKey(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", &key) == ERROR_SUCCESS)
	{
		DWORD	 value = 1;
		DWORD	 size  = sizeof(value);

		if (RegQueryValueEx(key, L"AppsUseLightTheme", 0, NULL, (BYTE *) &value, &size) == ERROR_SUCCESS) darkModeEnabled	= !value;

		RegCloseKey(key);
	}

	if (RegOpenKey(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\DWM", &key) == ERROR_SUCCESS)
	{
		DWORD	 value = 0;
		DWORD	 size  = sizeof(value);

		if (RegQueryValueEx(key, L"ColorizationColor", 0, NULL, (BYTE *) &value, &size) == ERROR_SUCCESS) colorizationColor	= GUI::Color((value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF);

		if (RegQueryValueEx(key, L"AccentColor",       0, NULL, (BYTE *) &value, &size) == ERROR_SUCCESS) accentColor		= GUI::Color(value & 0xFF, (value >> 8) & 0xFF, (value >> 16) & 0xFF);
		if (RegQueryValueEx(key, L"ColorPrevalence",   0, NULL, (BYTE *) &value, &size) == ERROR_SUCCESS) accentColorPrevalence = value;

		RegCloseKey(key);
	}

	/* Set theme colors.
	 */
	if (darkModeEnabled)
	{
		Setup::BackgroundColor	  = GUI::Color(64, 64, 70);
		Setup::LightGrayColor	  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);

		Setup::ClientColor	  = GUI::Color(32, 32, 35);
		Setup::ClientTextColor	  = GUI::Color(255, 255, 255);

		Setup::DividerLightColor  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
		Setup::DividerDarkColor	  = GetSysColor(COLOR_3DSHADOW);

		Setup::TextColor	  = GUI::Color(245, 245, 245);
		Setup::InactiveTextColor  = GUI::Color(160, 160, 160);

		Setup::LinkColor	  = GUI::Color(100, 160, 225);
		Setup::LinkHighlightColor = GUI::Color(100, 160, 225);
	}
	else
	{
		Setup::BackgroundColor	  = GetSysColor(COLOR_3DFACE);
		Setup::LightGrayColor	  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);

		Setup::ClientColor	  = GetSysColor(COLOR_WINDOW);
		Setup::ClientTextColor	  = GetSysColor(COLOR_WINDOWTEXT);

		Setup::DividerLightColor  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
		Setup::DividerDarkColor	  = GetSysColor(COLOR_3DSHADOW);

		Setup::TextColor	  = GetSysColor(COLOR_BTNTEXT);
		Setup::InactiveTextColor  = GetSysColor(COLOR_GRAYTEXT);

		Setup::LinkColor	  = GetSysColor(COLOR_HOTLIGHT);
		Setup::LinkHighlightColor = GetSysColor(COLOR_HOTLIGHT);
	}

	/* Set accent colors.
	 */
	if (accentColor != GUI::Color(-1))
	{
		Setup::HighlightColor		  = accentColor.Grayscale().GetRed() < 245 ? accentColor : GUI::Color(accentColor.GetRed() - accentColor.GetRed() * 0.15, accentColor.GetGreen() - accentColor.GetGreen() * 0.15, accentColor.GetBlue() - accentColor.GetBlue() * 0.15);
		Setup::HighlightTextColor	  = Setup::HighlightColor.Grayscale().GetRed() > 190 ? GUI::Color(0) : GUI::Color(255, 255, 255);

		if (darkModeEnabled)
		{
			Setup::GradientStartColor = GUI::Color(accentColor.GetRed() - accentColor.GetRed() * 0.45, accentColor.GetGreen() - accentColor.GetGreen() * 0.45, accentColor.GetBlue() - accentColor.GetBlue() * 0.45);
			Setup::GradientEndColor	  = GUI::Color(accentColor.GetRed() - accentColor.GetRed() * 0.30, accentColor.GetGreen() - accentColor.GetGreen() * 0.30, accentColor.GetBlue() - accentColor.GetBlue() * 0.30);
			Setup::GradientTextColor  = GUI::Color(245, 245, 245);
		}
		else
		{
			Setup::GradientStartColor = GUI::Color(accentColor.GetRed() - accentColor.GetRed() * 0.15, accentColor.GetGreen() - accentColor.GetGreen() * 0.15, accentColor.GetBlue() - accentColor.GetBlue() * 0.15);
			Setup::GradientEndColor	  = GUI::Color(accentColor.GetRed() + (255 - accentColor.GetRed()) * 0.25, accentColor.GetGreen() + (255 - accentColor.GetGreen()) * 0.25, accentColor.GetBlue() + (255 - accentColor.GetBlue()) * 0.25);
			Setup::GradientTextColor  = Setup::GradientStartColor.Average(Setup::GradientEndColor).Grayscale().GetRed() > 190 ? GUI::Color(0) : GUI::Color(255, 255, 255);
		}

		if (accentColorPrevalence)
		{
			Setup::TitlebarStartColor = accentColor;
			Setup::TitlebarEndColor	  = Setup::TitlebarStartColor;
			Setup::TitlebarTextColor  = Setup::TitlebarStartColor.Grayscale().GetRed() > 190 ? GUI::Color(0) : GUI::Color(255, 255, 255);
		}
		else
		{
			Setup::TitlebarStartColor = Setup::GradientStartColor;
			Setup::TitlebarEndColor	  = Setup::GradientEndColor;
			Setup::TitlebarTextColor  = Setup::GradientTextColor;
		}

		Setup::InactiveTitlebarStartColor = Setup::TitlebarStartColor.Grayscale();
		Setup::InactiveTitlebarEndColor	  = Setup::TitlebarEndColor.Grayscale();
		Setup::InactiveTitlebarTextColor  = Setup::TitlebarTextColor.Grayscale();
	}
	else if (colorizationColor != GUI::Color(-1))
	{
		Setup::HighlightColor		  = colorizationColor.Grayscale().GetRed() < 245 ? colorizationColor : GUI::Color(colorizationColor.GetRed() - colorizationColor.GetRed() * 0.15, colorizationColor.GetGreen() - colorizationColor.GetGreen() * 0.15, colorizationColor.GetBlue() - colorizationColor.GetBlue() * 0.15);
		Setup::HighlightTextColor	  = Setup::HighlightColor.Grayscale().GetRed() > 190 ? GUI::Color(0) : GUI::Color(255, 255, 255);

		Setup::GradientStartColor	  = GUI::Color(colorizationColor.GetRed() - colorizationColor.GetRed() * 0.15, colorizationColor.GetGreen() - colorizationColor.GetGreen() * 0.15, colorizationColor.GetBlue() - colorizationColor.GetBlue() * 0.15);
		Setup::GradientEndColor		  = GUI::Color(colorizationColor.GetRed() + (255 - colorizationColor.GetRed()) * 0.25, colorizationColor.GetGreen() + (255 - colorizationColor.GetGreen()) * 0.25, colorizationColor.GetBlue() + (255 - colorizationColor.GetBlue()) * 0.25);
		Setup::GradientTextColor	  = Setup::GradientStartColor.Average(Setup::GradientEndColor).Grayscale().GetRed() > 190 ? GUI::Color(0) : GUI::Color(255, 255, 255);

		if (Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2))
		{
			Setup::TitlebarStartColor	  = colorizationColor.Grayscale().GetRed() < 250 ? colorizationColor : GUI::Color(colorizationColor.GetRed() - colorizationColor.GetRed() * 0.02, colorizationColor.GetGreen() - colorizationColor.GetGreen() * 0.02, colorizationColor.GetBlue() - colorizationColor.GetBlue() * 0.02);
			Setup::TitlebarEndColor		  = Setup::TitlebarStartColor;
			Setup::TitlebarTextColor	  = Setup::TitlebarStartColor.Grayscale().GetRed() < 120 ? GUI::Color(245, 245, 245) : GUI::Color(0);

			Setup::InactiveTitlebarStartColor = GUI::Color(235, 235, 235);
			Setup::InactiveTitlebarEndColor	  = GUI::Color(235, 235, 235);
			Setup::InactiveTitlebarTextColor  = GUI::Color(0);
		}
		else
		{
			Setup::TitlebarStartColor	  = Setup::GradientStartColor;
			Setup::TitlebarEndColor		  = Setup::GradientEndColor;
			Setup::TitlebarTextColor	  = Setup::TitlebarStartColor.Grayscale().GetRed() < 120 ? GUI::Color(245, 245, 245) : GUI::Color(0);

			Setup::InactiveTitlebarStartColor = Setup::TitlebarStartColor.Grayscale();
			Setup::InactiveTitlebarEndColor	  = Setup::TitlebarEndColor.Grayscale();
			Setup::InactiveTitlebarTextColor  = Setup::TitlebarTextColor.Grayscale();
		}
	}
	else
	{
		Setup::HighlightColor		  = GetSysColor(COLOR_HIGHLIGHT);
		Setup::HighlightTextColor	  = GetSysColor(COLOR_HIGHLIGHTTEXT);

		Setup::GradientStartColor	  = GetSysColor(COLOR_ACTIVECAPTION);
		Setup::GradientEndColor		  = GetSysColor(COLOR_GRADIENTACTIVECAPTION);
		Setup::GradientTextColor	  = GetSysColor(COLOR_CAPTIONTEXT);

		Setup::TitlebarStartColor	  = GetSysColor(COLOR_ACTIVECAPTION);
		Setup::TitlebarEndColor		  = GetSysColor(COLOR_GRADIENTACTIVECAPTION);
		Setup::TitlebarTextColor	  = GetSysColor(COLOR_CAPTIONTEXT);

		Setup::InactiveTitlebarStartColor = GetSysColor(COLOR_INACTIVECAPTION);
		Setup::InactiveTitlebarEndColor	  = GetSysColor(COLOR_GRADIENTINACTIVECAPTION);
		Setup::InactiveTitlebarTextColor  = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	}

	/* Set tooltip and link colors.
	 */
	Setup::TooltipColor	= GetSysColor(COLOR_INFOBK);
	Setup::TooltipTextColor	= GetSysColor(COLOR_INFOTEXT);
}

S::Bool S::Backends::BackendWin32::IsWindowsVersionAtLeast(UnsignedInt platformId, UnsignedInt majorVersion, UnsignedInt minorVersion)
{
	static OSVERSIONINFO	 versionInfo = { 0 };

	if (versionInfo.dwOSVersionInfoSize == 0)
	{
		versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		GetVersionEx(&versionInfo);
	}

	if (versionInfo.dwPlatformId == platformId && ( versionInfo.dwMajorVersion >  majorVersion ||
						       (versionInfo.dwMajorVersion == majorVersion && versionInfo.dwMinorVersion >= minorVersion))) return True;

	return False;
}

S::String S::Backends::BackendWin32::GetFullPathName(const String &shortPath)
{
	static const String	 extendedPathPrefix = "\\\\?\\";

	if (shortPath.StartsWith(extendedPathPrefix)) return shortPath;

	wchar_t	 buffer[MAX_EXT_PATH] = { 0 };

	if (GetLongPathName(Directory::MakeExtendedPath(shortPath), buffer, MAX_EXT_PATH) == 0) return shortPath;

	return Directory::StripExtendedPathPrefix(buffer);
}
