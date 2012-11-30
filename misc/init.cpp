 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/backend.h>
#include <smooth/templates/nonblocking.h>
#include <smooth/i18n/translator_internal.h>
#include <smooth/threads/thread.h>
#include <smooth/graphics/font.h>
#include <smooth/init.h>
#include <smooth/foreach.h>
#include <smooth/resources.h>

#if defined __WIN32__
#	include <smooth/init.win32.h>

#	include <wtypes.h>

#	include <smooth/backends/win32/backendwin32.h>
#elif defined __HAIKU__
#	include <interface/InterfaceDefs.h>
#else
#	include <unistd.h>
#endif

#include <iconv.h>

static S::Array<S::Int (*)(), S::Void *>	*initFunctions = NIL;
static S::Array<S::Int (*)(), S::Void *>	*freeFunctions = NIL;

#if defined __WIN32__
__declspec (dllexport) HINSTANCE	 S::hInstance	  = NIL;
__declspec (dllexport) HINSTANCE	 S::hPrevInstance = NIL;

__declspec (dllexport) HICON		 S::SMOOTHICON	  = NIL;

int CALLBACK EnumFontProcA(ENUMLOGFONTEXA *lpelfe, NEWTEXTMETRICEXA *lpntme, int fontType, LPARAM lParam)
{
	static S::String	 fontName = "Microsoft Sans Serif";

	if (S::String(lpelfe->elfLogFont.lfFaceName) == fontName) return 0;
	else							  return 1;
}

int CALLBACK EnumFontProcW(ENUMLOGFONTEXW *lpelfe, NEWTEXTMETRICEXW *lpntme, int fontType, LPARAM lParam)
{
	static S::String	 fontName = "Microsoft Sans Serif";

	if (S::String(lpelfe->elfLogFont.lfFaceName) == fontName) return 0;
	else							  return 1;
}
#endif

namespace smooth
{
	SMOOTHAPI S::Array<S::Caller *, Void *> S::NonBlocking::callers;
};

S::Bool	 S::initializing = S::True;

S::Int	 initCount = 0;

S::Int S::AddInitFunction(Int (*initFunction)())
{
	if (initFunction == NIL) return Error();

	if (initFunctions == NIL) initFunctions = new Array<Int (*)(), Void *>;

	initFunctions->Add(initFunction);

	return Success();
}

S::Int S::AddFreeFunction(Int (*freeFunction)())
{
	if (freeFunction == NIL) return Error();

	if (freeFunctions == NIL) freeFunctions = new Array<Int (*)(), Void *>;

	freeFunctions->Add(freeFunction);

	return Success();
}

S::Bool S::Init()
{
	if (initCount++) return True;

	/* Init system backends.
	 */
	if (Backends::Backend::InitBackends() == Error()) return False;

	/* Call registered init functions.
	 */
	foreach (Int (*initFunction)(), (*initFunctions))
	{
		if (initFunction() != Success())
		{
			/* Deinit system backends.
			 */
			Backends::Backend::DeinitBackends();

			return False;
		}
	}

#if defined __WIN32__
	if (hDllInstance == NIL) hDllInstance = hInstance;

	/* Decide if we want to use unicode.
	 */
	{
		OSVERSIONINFOA	 vInfo;

		vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

		GetVersionExA(&vInfo);

		if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			Setup::enableUnicode = True;
		}
		else
		{
			HMODULE	 hUnicows = LoadLibraryA("unicows.dll");

			if (hUnicows != NIL) Setup::enableUnicode = True;

			FreeLibrary(hUnicows);
		}
	}

	SMOOTHICON = (HICON) LoadImageA(hDllInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	if (LoadIconvDLL() == True) Setup::useIconv = True;

	use_iconv = (Setup::useIconv ? 1 : 0);
#endif

	/* Get default colors and fonts.
	 */
	GetColors();
	GetDefaultFont();

	/* Init internationalization system.
	 */
	I18n::Translator::defaultTranslator = new I18n::TranslatorInternal();
	I18n::Translator::defaultTranslator->SelectUserDefaultLanguage();

	Setup::rightToLeft = I18n::Translator::defaultTranslator->IsActiveLanguageRightToLeft();

	return True;
}

S::Void S::Free()
{
	/* Delete all remaining callers left
	 * from nonblocking function calls.
	 */
	NonBlocking::CleanUp();

	/* Call ObjectCleanup() to make sure other
	 * DLLs are unloaded before we continue.
	 */
	Object::ObjectCleanup();

	if (--initCount) return;

	/* Stop running threads.
	 */
	if (Threads::Thread::GetNOfRunningThreads() > 0)
	{
		for (Int i = 0; i < Object::GetNOfObjects(); i++)
		{
			Object	*object = Object::GetNthObject(i);

			if (object == NIL) continue;

			if (object->GetObjectType() == Threads::Thread::classID &&
			    !(object->GetFlags() & Threads::THREAD_KILLFLAG_WAIT)) ((Threads::Thread *) object)->Stop();
		}
	}

	/* Wait for remaining threads to finish.
	 */
	while (Threads::Thread::GetNOfRunningThreads() > 0) System::System::Sleep(10);

	/* Delete translator object and free iconv library.
	 */
	delete I18n::Translator::defaultTranslator;

#if defined __WIN32__
	if (Setup::useIconv) FreeIconvDLL();
#endif

	/* Call registered free functions.
	 */
	foreach (Int (*freeFunction)(), (*freeFunctions))
	{
		freeFunction();
	}

	/* Deinit system backends.
	 */
	Backends::Backend::DeinitBackends();

	/* All pending objects should be marked for
	 * deletion now, so let's call ObjectCleanup()
	 * again to actually free them.
	 */
	Object::ObjectCleanup();
}

S::Void S::GetColors()
{
#if defined __WIN32__
	Setup::BackgroundColor		  = GetSysColor(COLOR_3DFACE);
	Setup::TextColor		  = GetSysColor(COLOR_BTNTEXT);
	Setup::GrayTextColor		  = GetSysColor(COLOR_GRAYTEXT);
	Setup::ClientColor		  = GetSysColor(COLOR_WINDOW);
	Setup::ClientTextColor		  = GetSysColor(COLOR_WINDOWTEXT);
	Setup::GradientStartColor	  = GetSysColor(COLOR_ACTIVECAPTION);
	Setup::GradientEndColor		  = GetSysColor(27);
	Setup::GradientTextColor	  = GetSysColor(COLOR_CAPTIONTEXT);
	Setup::InactiveGradientStartColor = GetSysColor(COLOR_INACTIVECAPTION);
	Setup::InactiveGradientEndColor	  = GetSysColor(28);
	Setup::InactiveGradientTextColor  = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	Setup::DividerLightColor	  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
	Setup::DividerDarkColor		  = GetSysColor(COLOR_3DSHADOW);
	Setup::LightGrayColor		  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
	Setup::TooltipColor		  = GetSysColor(COLOR_INFOBK);
	Setup::TooltipTextColor		  = GetSysColor(COLOR_INFOTEXT);
#elif defined __APPLE__
	Setup::BackgroundColor		  = GUI::Color(216, 216, 216);
#elif defined __HAIKU__
	rgb_color	 panelBackground    = ui_color(B_PANEL_BACKGROUND_COLOR);
	rgb_color	 panelText	    = ui_color(B_PANEL_TEXT_COLOR);
	rgb_color	 documentBackground = ui_color(B_DOCUMENT_BACKGROUND_COLOR);
	rgb_color	 documentText	    = ui_color(B_DOCUMENT_TEXT_COLOR);
	rgb_color	 windowTab	    = ui_color(B_WINDOW_TAB_COLOR);
	rgb_color	 windowText	    = ui_color(B_WINDOW_TEXT_COLOR);
	rgb_color	 windowInactiveTab  = ui_color(B_WINDOW_INACTIVE_TAB_COLOR);
	rgb_color	 windowInactiveText = ui_color(B_WINDOW_INACTIVE_TEXT_COLOR);
	rgb_color	 tooltipBackground  = ui_color(B_TOOL_TIP_BACKGROUND_COLOR);
	rgb_color	 tooltipText	    = ui_color(B_TOOL_TIP_TEXT_COLOR);

	Setup::BackgroundColor		  = GUI::Color(panelBackground.red, panelBackground.green, panelBackground.blue);
	Setup::TextColor		  = GUI::Color(panelText.red, panelText.green, panelText.blue);
	Setup::ClientColor		  = GUI::Color(documentBackground.red, documentBackground.green, documentBackground.blue);
	Setup::ClientTextColor		  = GUI::Color(documentText.red, documentText.green, documentText.blue);
	Setup::GradientStartColor	  = GUI::Color(windowTab.red, windowTab.green, windowTab.blue);
	Setup::GradientEndColor		  = GUI::Color(Setup::GradientStartColor.GetRed() + (255 - Setup::GradientStartColor.GetRed()) * 0.6, Setup::GradientStartColor.GetGreen() + (255 - Setup::GradientStartColor.GetGreen()) * 0.6, Setup::GradientStartColor.GetBlue() + (255 - Setup::GradientStartColor.GetBlue()) * 0.6);
	Setup::GradientTextColor	  = GUI::Color(windowText.red, windowText.green, windowText.blue);
	Setup::InactiveGradientStartColor = GUI::Color(windowInactiveTab.red, windowInactiveTab.green, windowInactiveTab.blue);
	Setup::InactiveGradientEndColor	  = GUI::Color(Setup::InactiveGradientStartColor.GetRed() + (255 - Setup::InactiveGradientStartColor.GetRed()) * 0.6, Setup::InactiveGradientStartColor.GetGreen() + (255 - Setup::InactiveGradientStartColor.GetGreen()) * 0.6, Setup::InactiveGradientStartColor.GetBlue() + (255 - Setup::InactiveGradientStartColor.GetBlue()) * 0.6);
	Setup::InactiveGradientTextColor  = GUI::Color(windowInactiveText.red, windowInactiveText.green, windowInactiveText.blue);
	Setup::DividerLightColor	  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
	Setup::LightGrayColor		  = GUI::Color(Setup::BackgroundColor.GetRed() + (255 - Setup::BackgroundColor.GetRed()) * 0.6, Setup::BackgroundColor.GetGreen() + (255 - Setup::BackgroundColor.GetGreen()) * 0.6, Setup::BackgroundColor.GetBlue() + (255 - Setup::BackgroundColor.GetBlue()) * 0.6);
	Setup::TooltipColor		  = GUI::Color(tooltipBackground.red, tooltipBackground.green, tooltipBackground.blue);
	Setup::TooltipTextColor		  = GUI::Color(tooltipText.red, tooltipText.green, tooltipText.blue);
#endif
}

S::Void S::GetDefaultFont()
{
#if defined __WIN32__
	HDC		 dc = GetWindowDC(0);
	LOGFONTA	 fontInfoA;
	LOGFONTW	 fontInfoW;

	fontInfoA.lfCharSet	   = DEFAULT_CHARSET;
	fontInfoA.lfFaceName[0]	   = 0;
	fontInfoA.lfPitchAndFamily = 0;

	fontInfoW.lfCharSet	   = DEFAULT_CHARSET;
	fontInfoW.lfFaceName[0]	   = 0;
	fontInfoW.lfPitchAndFamily = 0;

	int	 result;

	if (Setup::enableUnicode) result = EnumFontFamiliesExW(dc, &fontInfoW, (FONTENUMPROCW) &EnumFontProcW, 0, 0);
	else			  result = EnumFontFamiliesExA(dc, &fontInfoA, (FONTENUMPROCA) &EnumFontProcA, 0, 0);

	if (result == 0) GUI::Font::Default = "Microsoft Sans Serif";
	else		 GUI::Font::Default = "MS Sans Serif";

	ReleaseDC(0, dc);
#elif defined __APPLE__
	GUI::Font::Default = "Arial";
#elif defined __HAIKU__
	GUI::Font::Default = "DejaVu Sans";
#else
	GUI::Font::Default = "Helvetica";
#endif
}
