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

HBITMAP	 S::DEFAULTICON		= NIL;

bool	 S::loopActive		= false;
int	 S::peekLoop		= 0;
bool	 S::initializing	= true;

S::Int	 initCount = 0;

S::Void S::Init()
{
	if (initCount++) return;

	LiSAInit();

#ifdef __WIN32__
	WORD		 wVersionRequested = MAKEWORD(1,1);
	WSADATA		 wsaData;

	WSAStartup(wVersionRequested, &wsaData);

	if (hDllInstance == NIL) hDllInstance = hInstance;

	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)	SMOOTH::Setup::enableUnicode = True;
	else							SMOOTH::Setup::enableUnicode = False;

	DEFAULTICON = LoadBitmapA(hDllInstance, MAKEINTRESOURCEA(IDB_ICON));
	SMOOTHICON = LoadIconA(hDllInstance, MAKEINTRESOURCEA(IDI_ICON));
#endif

	mainObjectManager	= new ObjectManager();
	mainThreadManager	= new ThreadManager();

	GetColors();
	SetMetrics();
	SetMeasurement(SMT_UNITS);
	GetDefaultLanguage();
	SMOOTH::SetLanguage(DefaultLanguage);

	String::SetInputFormat("UTF-8");

	backgroundApplication = new BackgroundApplication();
}

S::Void S::Free()
{
	if (--initCount) return;

#ifdef __WIN32__
	::DeleteObject(DEFAULTICON);
	DestroyIcon(SMOOTHICON);
#endif

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
	SMOOTH::Setup::BackgroundColor			= GetSysColor(COLOR_3DFACE);
	SMOOTH::Setup::ClientColor			= GetSysColor(COLOR_WINDOW);
	SMOOTH::Setup::ClientTextColor			= GetSysColor(COLOR_WINDOWTEXT);
	SMOOTH::Setup::TextColor			= GetSysColor(COLOR_BTNTEXT);
	SMOOTH::Setup::GrayTextColor			= GetSysColor(COLOR_GRAYTEXT);
	SMOOTH::Setup::GradientStartColor		= GetSysColor(COLOR_ACTIVECAPTION);
	SMOOTH::Setup::GradientEndColor			= GetSysColor(27);
	SMOOTH::Setup::GradientTextColor		= GetSysColor(COLOR_CAPTIONTEXT);
	SMOOTH::Setup::InactiveGradientStartColor	= GetSysColor(COLOR_INACTIVECAPTION);
	SMOOTH::Setup::InactiveGradientEndColor		= GetSysColor(28);
	SMOOTH::Setup::InactiveGradientTextColor	= GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	SMOOTH::Setup::DividerLightColor		= GetSysColor(COLOR_3DHIGHLIGHT);
	SMOOTH::Setup::DividerDarkColor			= GetSysColor(COLOR_3DSHADOW);
	SMOOTH::Setup::LightGrayColor			= GetSysColor(COLOR_3DHIGHLIGHT);
	SMOOTH::Setup::TooltipColor			= GetSysColor(COLOR_INFOBK);
	SMOOTH::Setup::TooltipTextColor			= GetSysColor(COLOR_INFOTEXT);
#endif
}

S::Int S::Loop()
{
#ifdef __WIN32__
	MSG		 msg;
#endif

	Window	*wnd;
	Thread	*thread;

	if (!loopActive)
	{
#ifdef __WIN32__
		if (Window::nOfActiveWindows == 0) PostQuitMessage(0);
#endif

		initializing = false;
		loopActive = true;

		for (int i = 0; i < Object::objectCount; i++)
		{
			wnd = (Window *) mainObjectManager->RequestObject(i);

			if (wnd != (Window *) NIL)
			{
				if (wnd->type == OBJ_WINDOW)
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

			if (SMOOTH::Setup::enableUnicode)	result = PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
			else					result = PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);

			if (result)
			{
				if (Window::nOfActiveWindows == 1)
				{
					msg.message = WM_QUIT;
					break;
				}

				TranslateMessage(&msg);

				if (SMOOTH::Setup::enableUnicode)	DispatchMessageW(&msg);
				else					DispatchMessageA(&msg);
			}

			if (SMOOTH::Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
			else					PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);

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

			if (SMOOTH::Setup::enableUnicode)	result = GetMessageW(&msg, NIL, 0, 0);
			else					result = GetMessageA(&msg, NIL, 0, 0);

			if (!result) break;

			if (Window::nOfActiveWindows == 1)
			{
				msg.message = WM_QUIT;
				break;
			}

			TranslateMessage(&msg);

			if (SMOOTH::Setup::enableUnicode)	DispatchMessageW(&msg);
			else					DispatchMessageA(&msg);

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
				if (thread->GetKillFlag() == THREAD_KILLFLAG_KILL) thread->Stop();
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
