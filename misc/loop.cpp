 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/dllmain.h>
#include <smooth/resources.h>
#include <smooth/window.h>
#include <smooth/metrics.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>
#include <smooth/thread.h>
#include <smooth/threadmanager.h>
#include <smooth/background.h>
#include <smooth/objectmanager.h>
#include <smooth/color.h>

#ifdef __WIN32__
#include <winsock.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

#ifdef __WIN32__
__declspec (dllexport) HINSTANCE	 S::hInstance		= NIL;
__declspec (dllexport) HINSTANCE	 S::hPrevInstance	= NIL;
__declspec (dllexport) S::String	 S::szCmdLine		= NIL;
__declspec (dllexport) int		 S::iCmdShow		= 0;
#endif

#ifdef __WIN32__
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
	WORD		 wVersionRequested = MAKEWORD(1,1);
	WSADATA		 wsaData;

	WSAStartup(wVersionRequested, &wsaData);

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
#endif

	mainObjectManager	= new ObjectManager();
	mainThreadManager	= new ThreadManager();

	GetColors();
	SetMetrics();
	SetMeasurement(SMT_UNITS);
	GetDefaultLanguage();
	SMOOTH::SetLanguage(DefaultLanguage);

	String::SetInputFormat("ISO-8859-1");
	String::SetOutputFormat(String("CP").Append(String::FromInt(codePage)));

	backgroundApplication = new BackgroundApplication();
}

S::Void S::Free()
{
	if (--initCount) return;

	delete backgroundApplication;

	delete mainThreadManager;
	delete mainObjectManager;

#ifdef __WIN32__
	WSACleanup();
#endif

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

	GUI::Window	*wnd;
	Thread		*thread;

	if (!loopActive)
	{
#ifdef __WIN32__
		if (GUI::Window::nOfActiveWindows == 0) PostQuitMessage(0);
#endif

		initializing = false;
		loopActive = true;

		for (int i = 0; i < Object::objectCount; i++)
		{
			wnd = (GUI::Window *) mainObjectManager->RequestObject(i);

			if (wnd != (GUI::Window *) NIL)
			{
				if (wnd->type == GUI::Window::classID)
				{
					if (!wnd->created) wnd->Create();
					if (!wnd->initshow) wnd->Show();
				}
			}
		}

		// start waiting threads here

		for (int j = 0; j < Object::objectCount; j++)
		{
			thread = mainThreadManager->RequestThread(j);

			if (thread != (Thread *) NIL)
			{
				if (thread->GetStatus() == THREAD_STARTME) thread->Start();
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
				if (GUI::Window::nOfActiveWindows == 0)
				{
					msg.message = WM_QUIT;
					break;
				}

				TranslateMessage(&msg);

				if (Setup::enableUnicode)	DispatchMessageW(&msg);
				else				DispatchMessageA(&msg);
			}

			if (Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
			else				PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);

			if (peekLoop == 0) break;
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

			if (GUI::Window::nOfActiveWindows == 0)
			{
				msg.message = WM_QUIT;
				break;
			}

			TranslateMessage(&msg);

			if (Setup::enableUnicode)	DispatchMessageW(&msg);
			else				DispatchMessageA(&msg);

			if (peekLoop > 0) break;
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

		for (int i = 0; i < Object::objectCount; i++)
		{
			thread = mainThreadManager->RequestThread(i);

			if (thread != (Thread *) NIL)
			{
				if (!(thread->GetFlags() & THREAD_KILLFLAG_WAIT)) thread->Stop();
			}
		}

		while (Thread::counter > 0) LiSASleep(10);

#ifdef __WIN32__
		return msg.wParam;
	}
#else
	return 0;
#endif
}
