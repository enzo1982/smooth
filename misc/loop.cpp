 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/resources.h>
#include <smooth/threads/thread.h>
#include <smooth/backends/backend.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/color.h>
#include <smooth/i18n/i18n.h>
#include <smooth/gui/application/background.h>
#include <smooth/system/event.h>
#include <smooth/system/multimonitor.h>

#ifdef __WIN32__
#include <wtypes.h>
#include <shlobj.h>

#include <smooth/backends/win32/backendwin32.h>
#else
#include <unistd.h>
#endif

using namespace smooth::Backends;

#ifdef __WIN32__
__declspec (dllexport) HINSTANCE	 S::hInstance		= NIL;
__declspec (dllexport) HINSTANCE	 S::hPrevInstance	= NIL;
__declspec (dllexport) S::String	 S::szCmdLine		= NIL;
__declspec (dllexport) int		 S::iCmdShow		= 0;

__declspec (dllexport) HICON	 S::SMOOTHICON = NIL;
#endif

bool	 S::loopActive		= false;
int	 S::peekLoop		= 0;
bool	 S::initializing	= true;

S::Int	 initCount = 0;

S::Void S::Init()
{
	if (initCount++) return;

	LiSAInit();

	Backend::InitBackends();

	System::MultiMonitor::Initialize();

#ifdef __WIN32__
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

	if (LoadIconvDLL() == True)	{ Setup::useIconv = True;  use_iconv = 1; }
	else				{ Setup::useIconv = False; use_iconv = 0; }
#else
	{ Setup::useIconv = True; use_iconv = 1; }
#endif

	GetColors();

	I18n::Translator::defaultTranslator = new I18n::Translator("smooth", True);
	I18n::Translator::defaultTranslator->SetInternalLanguageInfo("English", "Robert Kausch", "http://www.smooth-project.org/", False);

	Int	 codePage = 1252;

#ifdef __WIN32__
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

	String::SetInputFormat("ISO-8859-1");
	String::SetOutputFormat(String("CP").Append(String::FromInt(codePage)));

#ifdef __WIN32__
	GUI::backgroundApplication = new GUI::BackgroundApplication();
#endif
}

S::Void S::Free()
{
	if (--initCount) return;

	CoUninitialize();

#ifdef __WIN32__
	delete I18n::Translator::defaultTranslator;

	delete GUI::backgroundApplication;

	if (Setup::useIconv) FreeIconvDLL();
#endif

	System::MultiMonitor::Free();

	Backend::DeinitBackends();

	LiSADeinit();
}

S::Void S::GetColors()
{
#ifdef __WIN32__
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

S::Int S::Loop()
{
	if (!loopActive)
	{
		initializing = false;
		loopActive = true;

		for (Int i = 0; i < Object::GetNOfObjects(); i++)
		{
			Object	*object = Object::GetNthObject(i);

			if (object != NIL)
			{
				if (object->GetObjectType() == GUI::Window::classID)
				{
					if (!((GUI::Window *) object)->initshow) ((GUI::Window *) object)->Show();
				}
			}
		}

		// start waiting threads here

		for (Int j = 0; j < Object::GetNOfObjects(); j++)
		{
			Object	*object = Object::GetNthObject(j);

			if (object != NIL)
			{
				if (object->GetObjectType() == Threads::Thread::classID)
				{
					if (((Threads::Thread *) object)->GetStatus() == Threads::THREAD_STARTME) ((Threads::Thread *) object)->Start();
				}
			}
		}
	}

#ifdef __WIN32__
	if (GUI::Window::nOfActiveWindows == 0) PostQuitMessage(0);
#endif

	System::EventProcessor	*event = new System::EventProcessor();
	Bool			 quit = False;

	while (!quit)
	{
		Int	 result = Success();

		if (peekLoop > 0)	result = event->ProcessNextEvent(False);
		else			result = event->ProcessNextEvent(True);

		if (result == Break)			quit = True;
		if (GUI::Window::nOfActiveWindows == 0)	quit = True;
	}

	delete event;

	loopActive = false;

	for (int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object	*object = Object::GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Threads::Thread::classID)
			{
				if (!(((Threads::Thread *) object)->GetFlags() & Threads::THREAD_KILLFLAG_WAIT)) ((Threads::Thread *) object)->Stop();
			}
		}
	}

	while (Threads::Thread::GetNOfRunningThreads() > 0) LiSASleep(10);

	return Success();
}
