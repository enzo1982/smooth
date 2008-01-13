 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/init.h>
#include <smooth/resources.h>
#include <smooth/backends/backend.h>
#include <smooth/gui/application/background.h>
#include <smooth/system/multimonitor.h>
#include <smooth/templates/nonblocking.h>
#include <smooth/i18n/translator.h>

#if defined __WIN32__
#	include <wtypes.h>
#	include <shlobj.h>

#	include <smooth/backends/win32/backendwin32.h>
#else
#	include <unistd.h>
#endif

#include <iconv.h>

using namespace smooth::Backends;

#if defined __WIN32__
__declspec (dllexport) HINSTANCE	 S::hInstance		= NIL;
__declspec (dllexport) HINSTANCE	 S::hPrevInstance	= NIL;
__declspec (dllexport) S::String	 S::szCmdLine		= NIL;
__declspec (dllexport) int		 S::iCmdShow		= 0;

__declspec (dllexport) HICON	 S::SMOOTHICON = NIL;

int CALLBACK EnumFontProcA(ENUMLOGFONTEXA *lpelfe, NEWTEXTMETRICEXA *lpntme, int fontType, LPARAM lParam)
{
	if (S::String(lpelfe->elfLogFont.lfFaceName) == "Microsoft Sans Serif")	return 0;
	else									return 1;
}

int CALLBACK EnumFontProcW(ENUMLOGFONTEXW *lpelfe, NEWTEXTMETRICEXW *lpntme, int fontType, LPARAM lParam)
{
	if (S::String(lpelfe->elfLogFont.lfFaceName) == "Microsoft Sans Serif")	return 0;
	else									return 1;
}
#endif

namespace smooth
{
	SMOOTHAPI S::Array<S::Caller *, Void *> S::NonBlocking::callers;
};

S::Bool	 S::initializing = S::True;

S::Int	 initCount = 0;

S::Void S::Init()
{
	if (initCount++) return;

	LiSAInit();

	Backend::InitBackends();

	System::MultiMonitor::Initialize();

#if defined __WIN32__
	if (hDllInstance == NIL) hDllInstance = hInstance;

	// decide if we want to use unicode:
	{
		OSVERSIONINFOA	 vInfo;

		vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

		GetVersionExA(&vInfo);

		HMODULE		 hUnicows = LoadLibraryA("unicows.dll");

		if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT || hUnicows != NIL)	Setup::enableUnicode = True;
		else									Setup::enableUnicode = False;

		FreeLibrary(hUnicows);
	}

	CoInitialize(NIL);

	SMOOTHICON = (HICON) LoadImageA(hDllInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	if (LoadIconvDLL() == True)	Setup::useIconv = True;
	else				Setup::useIconv = False;
#else
	Setup::useIconv = True;
#endif

#if defined __WIN32__ && !defined __WINE__
	use_iconv = (Setup::useIconv ? 1 : 0);
#endif

	GetColors();
	GetDefaultFont();

	I18n::Translator::defaultTranslator = new I18n::Translator("smooth", True);
	I18n::Translator::defaultTranslator->SetInternalLanguageInfo("English", "Robert Kausch", "http://www.smooth-project.org/", False);

	Int	 codePage = 1252;

#if defined __WIN32__
	switch (PRIMARYLANGID(GetUserDefaultLangID()))
	{
		default:
		case LANG_ENGLISH:
			I18n::Translator::defaultTranslator->ActivateLanguage("internal");
			break;
		case LANG_GERMAN:
			I18n::Translator::defaultTranslator->ActivateLanguage("smooth_de");
			break;
		case LANG_ITALIAN:
			I18n::Translator::defaultTranslator->ActivateLanguage("smooth_it");
			break;
	}

	codePage = GetACP();
#endif

	Setup::rightToLeft = I18n::Translator::defaultTranslator->IsActiveLanguageRightToLeft();

	String::SetInputFormat(String("CP").Append(String::FromInt(codePage)));
	String::SetOutputFormat(String("CP").Append(String::FromInt(codePage)));

	GUI::backgroundApplication = new GUI::BackgroundApplication();
}

S::Void S::Free()
{
	if (--initCount) return;

#if defined __WIN32__
	CoUninitialize();
#endif

	delete I18n::Translator::defaultTranslator;

	delete GUI::backgroundApplication;

#if defined __WIN32__
	if (Setup::useIconv) FreeIconvDLL();
#endif

	System::MultiMonitor::Free();

	Backend::DeinitBackends();

	LiSADeinit();
}

S::Void S::GetColors()
{
#if defined __WIN32__
	Setup::BackgroundColor			= GetSysColor(COLOR_3DFACE);
	Setup::ClientColor			= GetSysColor(COLOR_WINDOW);
	Setup::ClientTextColor			= GetSysColor(COLOR_WINDOWTEXT);
	Setup::TextColor			= GetSysColor(COLOR_BTNTEXT);
	Setup::GrayTextColor			= GetSysColor(COLOR_GRAYTEXT);
	Setup::GradientStartColor		= GetSysColor(COLOR_ACTIVECAPTION);
	Setup::GradientEndColor			= GetSysColor(27);
	Setup::GradientTextColor		= GetSysColor(COLOR_CAPTIONTEXT);
	Setup::InactiveGradientStartColor	= GetSysColor(COLOR_INACTIVECAPTION);
	Setup::InactiveGradientEndColor		= GetSysColor(28);
	Setup::InactiveGradientTextColor	= GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	Setup::DividerLightColor		= RGB(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
	Setup::DividerDarkColor			= GetSysColor(COLOR_3DSHADOW);
	Setup::LightGrayColor			= RGB(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
	Setup::TooltipColor			= GetSysColor(COLOR_INFOBK);
	Setup::TooltipTextColor			= GetSysColor(COLOR_INFOTEXT);
#endif
}

S::Void S::GetDefaultFont()
{
#if defined __WIN32__
	HDC		 dc = GetWindowDC(0);
	LOGFONTA	 fontInfoA;
	LOGFONTW	 fontInfoW;

	fontInfoA.lfCharSet = DEFAULT_CHARSET;
	fontInfoA.lfFaceName[0] = 0;
	fontInfoA.lfPitchAndFamily = 0;

	fontInfoW.lfCharSet = DEFAULT_CHARSET;
	fontInfoW.lfFaceName[0] = 0;
	fontInfoW.lfPitchAndFamily = 0;

	int	 result;

	if (Setup::enableUnicode)	result = EnumFontFamiliesExW(dc, &fontInfoW, (FONTENUMPROCW) &EnumFontProcW, 0, 0);
	else				result = EnumFontFamiliesExA(dc, &fontInfoA, (FONTENUMPROCA) &EnumFontProcA, 0, 0);

	if (result == 0)	GUI::Font::Default = "Microsoft Sans Serif";
	else			GUI::Font::Default = "MS Sans Serif";

	ReleaseDC(0, dc);
#else
	GUI::Font::Default = "Helvetica";
#endif
}
