 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_LOOP_
#define __OBJSMOOTH_LOOP_

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
__declspec (dllexport) HINSTANCE	 hInstance = NIL;
__declspec (dllexport) HINSTANCE	 hPrevInstance = NIL;
__declspec (dllexport) SMOOTHString	 szCmdLine = NIL;
__declspec (dllexport) int		 iCmdShow = 0;
#endif

#ifdef __WIN32__
__declspec (dllexport) HICON	 SMOOTHICON = NIL;
#endif

HBITMAP	 DEFAULTICON = NIL;

bool	 loopActive	= false;
int	 peekLoop	= 0;
bool	 initializing	= true;

SMOOTHVoid SMOOTH::Init()
{
	LiSAInit();

#ifdef __WIN32__
	if (hDllInstance == NIL) hDllInstance = hInstance;

	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)	SMOOTH::Setup::enableUnicode = SMOOTH::True;
	else							SMOOTH::Setup::enableUnicode = SMOOTH::False;

	DEFAULTICON = LoadBitmapA(hDllInstance, MAKEINTRESOURCEA(IDB_ICON));
	SMOOTHICON = LoadIconA(hDllInstance, MAKEINTRESOURCEA(IDI_ICON));
#endif

	mainObjectManager	= new SMOOTHObjectManager();
	mainThreadManager	= new SMOOTHThreadManager();

	SMOOTHGetColors();
	SMOOTHSetMetrics();
	SMOOTHSetMeasurement(SMT_UNITS);
	SMOOTHGetDefaultLanguage();
	SMOOTH::SetLanguage(SMOOTHDefaultLanguage);

	SMOOTHString::SetInputFormat(SIF_ISO);

	backgroundApplication = new SMOOTHBackgroundApplication();
}

SMOOTHVoid SMOOTH::Free()
{
#ifdef __WIN32__
	::DeleteObject(DEFAULTICON);
	DestroyIcon(SMOOTHICON);
#endif

	delete backgroundApplication;

	delete mainThreadManager;
	delete mainObjectManager;

	LiSADeinit();
}

void SMOOTHGetColors()
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

SMOOTHInt SMOOTH::Loop()
{
#ifdef __WIN32__
	MSG		 msg;
#endif

	SMOOTHWindow	*wnd;
	SMOOTHThread	*thread;

	if (!loopActive)
	{
#ifdef __WIN32__
		if (SMOOTHWindow::nOfActiveWindows == 0) PostQuitMessage(0);
#endif

		initializing = false;
		loopActive = true;

		SMOOTHInitNetworking();

		for (int i = 0; i < SMOOTHObject::objectCount; i++)
		{
			wnd = (SMOOTHWindow *) mainObjectManager->RequestObject(i);

			if (wnd != (SMOOTHWindow *) NIL)
			{
				if (wnd->type == OBJ_WINDOW)
				{
					if (!wnd->created) wnd->Create();
					if (!wnd->initshow) wnd->Show();
				}
			}
		}

		// start waiting threads here

		for (int j = 0; j < SMOOTHObject::objectCount; j++)
		{
			thread = mainThreadManager->RequestThread(j);

			if (thread != (SMOOTHThread *) NIL)
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
				if (SMOOTHWindow::nOfActiveWindows == 1)
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

			if (SMOOTHWindow::nOfActiveWindows == 1)
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
		return SMOOTH::Loop();	// respawn if only peekstate has changed
	}
	else
	{
#endif
		loopActive = false;

		for (int i = 0; i < SMOOTHObject::objectCount; i++)
		{
			thread = mainThreadManager->RequestThread(i);

			if (thread != (SMOOTHThread *) NIL)
			{
				if (thread->GetKillFlag() == THREAD_KILLFLAG_KILL) thread->Stop();
			}
		}

		while (SMOOTHThread::counter > 0) LiSASleep(10);

		SMOOTHFinishNetworking();

#ifdef __WIN32__
		return msg.wParam;
	}
#else
	return 0;
#endif
}

void SMOOTHInitNetworking()
{
#ifdef __WIN32__
	WORD		 wVersionRequested = MAKEWORD(1,1);
	WSADATA		 wsaData;

	WSAStartup(wVersionRequested, &wsaData);
#endif
}

void SMOOTHFinishNetworking()
{
#ifdef __WIN32__
	WSACleanup();
#endif
}

#endif
