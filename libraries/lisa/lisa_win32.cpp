 /* LiSA - Library for System Adaptation
  * Copyright (C) 2002-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public
  * License along with this library; if not, write to the Free
  * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  * MA 02111-1307, USA */

#include <lisa.h>
#include <windows.h>

HCURSOR	 LiSA_CURSOR_ARROW	= NULL;
HCURSOR	 LiSA_CURSOR_TEXTEDIT	= NULL;
HCURSOR	 LiSA_CURSOR_HAND	= NULL;
HCURSOR	 LiSA_CURSOR_HSIZE	= NULL;
HCURSOR	 LiSA_CURSOR_VSIZE	= NULL;

void LiSAInit()
{
	LiSA_CURSOR_ARROW	= (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	LiSA_CURSOR_TEXTEDIT	= (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32513), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	LiSA_CURSOR_HAND	= (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32649), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	LiSA_CURSOR_HSIZE	= (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32644), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	LiSA_CURSOR_VSIZE	= (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32645), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	if (LiSA_CURSOR_HAND == NULL) LiSA_CURSOR_HAND = LiSA_CURSOR_ARROW;
}

void LiSADeinit()
{
}

void LiSASleep(int mSeconds)
{
	if (mSeconds == 0) return;

	Sleep(mSeconds);
}

int LiSASetMouseCursor(LiSAWindow *window, int mouseCursor)
{
	if (mouseCursor != LiSA_MOUSE_ARROW)	SetClassLongA((HWND) window, GCL_HCURSOR, (LONG) NULL);
	else					SetClassLongA((HWND) window, GCL_HCURSOR, (LONG) LiSA_CURSOR_ARROW);

	switch (mouseCursor)
	{
		case LiSA_MOUSE_ARROW:
			SetCursor(LiSA_CURSOR_ARROW);

			return 0;
		case LiSA_MOUSE_TEXTEDIT:
			SetCursor(LiSA_CURSOR_TEXTEDIT);

			return 0;
		case LiSA_MOUSE_HAND:
			SetCursor(LiSA_CURSOR_HAND);

			return 0;
		case LiSA_MOUSE_HSIZE:
			SetCursor(LiSA_CURSOR_HSIZE);

			return 0;
		case LiSA_MOUSE_VSIZE:
			SetCursor(LiSA_CURSOR_VSIZE);

			return 0;
		default:
			return 1;
	}
}

int LiSAOpenURLA(char *url)
{
	if (((int) ShellExecuteA(NULL, "open", url, NULL, NULL, 0)) > 32)	return 0;
	else									return 1;
}

int LiSAOpenURLW(wchar_t *url)
{
	if (((int) ShellExecuteW(NULL, L"open", url, NULL, NULL, 0)) > 32)	return 0;
	else									return 1;
}

int LiSAGetDisplaySizeX()
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int LiSAGetDisplaySizeY()
{
	return GetSystemMetrics(SM_CYSCREEN);
}

LiSAThread *LiSAThreadCreate(unsigned long *threadID, void (*threadProc)(void *), void *threadParam)
{
	return (LiSAThread *) CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) threadProc, threadParam, 0, threadID);
}

int LiSAThreadCancel(LiSAThread *thread)
{
	TerminateThread((HANDLE) thread, 0);

	return 0;
}

void LiSAThreadExit()
{
	ExitThread(0);
}

int LiSAThreadCloseHandle(LiSAThread *thread)
{
	CloseHandle((HANDLE) thread);

	return 0;
}

LiSAThread *LiSAThreadSelf()
{
	return (LiSAThread *) GetCurrentThread();
}

LiSASemaphore *LiSASemaphoreCreate()
{
	return (LiSASemaphore *) CreateSemaphoreA(NULL, 1, 2147483647, NULL);
}

int LiSASemaphoreWait(LiSASemaphore *semaphore)
{
	WaitForSingleObject((HANDLE) semaphore, INFINITE);

	return 0;
}

int LiSASemaphoreRelease(LiSASemaphore *semaphore)
{
	ReleaseSemaphore((HANDLE) semaphore, 1, NULL);

	return 0;
}

int LiSASemaphoreCloseHandle(LiSASemaphore *semaphore)
{
	CloseHandle((HANDLE) semaphore);

	return 0;
}

LiSAMutex *LiSAMutexCreate()
{
	return (LiSAMutex *) CreateMutexA(NULL, false, NULL);
}

int LiSAMutexLock(LiSAMutex *mutex)
{
	WaitForSingleObject((HANDLE) mutex, INFINITE);

	return 0;
}

int LiSAMutexRelease(LiSAMutex *mutex)
{
	ReleaseMutex((HANDLE) mutex);
 
	return 0;
}

int LiSAMutexCloseHandle(LiSAMutex *mutex)
{
	CloseHandle((HANDLE) mutex);

	return 0;
}
