 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/dllmain.h>
#include <smooth/resources.h>
#include <smooth/threads/thread.h>
#include <smooth/backends/backend.h>
#include <smooth/graphics/window.h>
#include <smooth/metrics.h>
#include <smooth/objectmanager.h>
#include <smooth/color.h>

#ifdef __WIN32__
#include <smooth/i18n.h>
#include <smooth/background.h>

#include <winsock.h>
#else
#include <sys/socket.h>
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

	Int	 codePage = 1252;

#ifdef __WIN32__
	if (LoadIconvDLL() == True)	Setup::useIconv = True;
	else				Setup::useIconv = False;
#else
	Setup::useIconv = True;
#endif

	Backend::InitBackends();

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

	codePage = GetACP();

	SMOOTHICON = (HICON) LoadImageA(hDllInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	mainObjectManager	= new ObjectManager();
#endif

	GetColors();

#ifdef __WIN32__
	SetMetrics();
	SetMeasurement(SMT_UNITS);

	I18n::Translator::defaultTranslator = new I18n::Translator(True);

	switch (PRIMARYLANGID(GetUserDefaultLangID()))
	{
		default:
		case LANG_ENGLISH:
			I18n::Translator::defaultTranslator->ActivateLanguage("internal");
			break;
		case LANG_GERMAN:
			I18n::Translator::defaultTranslator->ActivateLanguage("smooth_de");
			break;
	}

#endif

	String::SetInputFormat("ISO-8859-1");
	String::SetOutputFormat(String("CP").Append(String::FromInt(codePage)));

#ifdef __WIN32__
	backgroundApplication = new BackgroundApplication();
#endif
}

S::Void S::Free()
{
	if (--initCount) return;

#ifdef __WIN32__
	delete I18n::Translator::defaultTranslator;

	delete backgroundApplication;

	delete mainObjectManager;

	if (Setup::useIconv) FreeIconvDLL();
#endif

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
	Setup::DividerLightColor		= RGB(GetRed(Setup::BackgroundColor) + (255 - GetRed(Setup::BackgroundColor)) * 0.6, GetGreen(Setup::BackgroundColor) + (255 - GetGreen(Setup::BackgroundColor)) * 0.6, GetBlue(Setup::BackgroundColor) + (255 - GetBlue(Setup::BackgroundColor)) * 0.6);
	Setup::DividerDarkColor			= GetSysColor(COLOR_3DSHADOW);
	Setup::LightGrayColor			= RGB(GetRed(Setup::BackgroundColor) + (255 - GetRed(Setup::BackgroundColor)) * 0.6, GetGreen(Setup::BackgroundColor) + (255 - GetGreen(Setup::BackgroundColor)) * 0.6, GetBlue(Setup::BackgroundColor) + (255 - GetBlue(Setup::BackgroundColor)) * 0.6);
	Setup::TooltipColor			= GetSysColor(COLOR_INFOBK);
	Setup::TooltipTextColor			= GetSysColor(COLOR_INFOTEXT);
#endif
}

S::Int S::Loop()
{
#ifdef __WIN32__
	MSG		 msg;
#endif

	if (!loopActive)
	{
#ifdef __WIN32__
		if (GUI::Window::nOfActiveWindows == 0) PostQuitMessage(0);
#endif

		initializing = false;
		loopActive = true;

		for (int i = 0; i < mainObjectManager->GetNOfObjects(); i++)
		{
			Object	*object = mainObjectManager->GetNthObject(i);

			if (object != NIL)
			{
				if (object->GetObjectType() == GUI::Window::classID)
				{
					if (!((GUI::Window *) object)->initshow) ((GUI::Window *) object)->Show();
				}
			}
		}

		// start waiting threads here

		for (int j = 0; j < mainObjectManager->GetNOfObjects(); j++)
		{
			Object	*object = mainObjectManager->GetNthObject(j);

			if (object != NIL)
			{
				if (object->GetObjectType() == Threads::Thread::classID)
				{
					if (((Threads::Thread *) object)->GetStatus() == Threads::THREAD_STARTME) ((Threads::Thread *) object)->Start();
				}
			}
		}
	}

	if (peekLoop > 0)
	{
#ifdef __WIN32__
		do
		{
			bool	 result;

			if (Setup::enableUnicode)	result = PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
			else				result = PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);

			if (result)
			{
				TranslateMessage(&msg);

				if (Setup::enableUnicode)	DispatchMessageW(&msg);
				else				DispatchMessageA(&msg);
			}

			if (Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
			else				PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);

			if (peekLoop == 0) break;

			if (GUI::Window::nOfActiveWindows == 0)
			{
				msg.message = WM_QUIT;
				break;
			}
		}
		while (msg.message != WM_QUIT);
#endif
	}
	else
	{
#ifdef __WIN32__
		while (true)
		{
			bool	 result;

			if (Setup::enableUnicode)	result = GetMessageW(&msg, NIL, 0, 0);
			else				result = GetMessageA(&msg, NIL, 0, 0);

			if (!result) break;

			TranslateMessage(&msg);

			if (Setup::enableUnicode)	DispatchMessageW(&msg);
			else				DispatchMessageA(&msg);

			if (peekLoop > 0) break;

			if (GUI::Window::nOfActiveWindows == 0)
			{
				msg.message = WM_QUIT;
				break;
			}
		}
#endif
	}

#ifdef __WIN32__
	if (msg.message != WM_QUIT)
	{
		return Loop();	// respawn if only peekstate has changed
	}
	else
	{
#endif
		loopActive = false;

		for (int i = 0; i < mainObjectManager->GetNOfObjects(); i++)
		{
			Object	*object = mainObjectManager->GetNthObject(i);

			if (object != NIL)
			{
				if (object->GetObjectType() == Threads::Thread::classID)
				{
					if (!(((Threads::Thread *) object)->GetFlags() & Threads::THREAD_KILLFLAG_WAIT)) ((Threads::Thread *) object)->Stop();
				}
			}
		}

		while (Threads::Thread::GetNOfRunningThreads() > 0) LiSASleep(10);

#ifdef __WIN32__
		return msg.wParam;
	}
#else
	return 0;
#endif
}
