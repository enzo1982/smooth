 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TOOLWINDOW_
#define __OBJSMOOTH_TOOLWINDOW_

#include <smooth/toolwindow.h>
#include <smooth/window.h>
#include <smooth/toolkit.h>
#include <smooth/object.h>
#include <smooth/divisionbar.h>
#include <smooth/definitions.h>
#include <smooth/metrics.h>
#include <smooth/loop.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>

#include <math.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_TOOLWINDOW = SMOOTH::RequestObjectID();

SMOOTHToolWindow::SMOOTHToolWindow() : SMOOTHWindow(TXT_SMOOTHTOOLWINDOW)
{
	type				= OBJ_TOOLWINDOW;
	containerType			= OBJ_WINDOW;

	objectProperties->pos.x		= 0;
	objectProperties->pos.y		= 0;
	objectProperties->size.cx	= 100;
	objectProperties->size.cy	= 100;
	objectProperties->orientation	= OR_FREE;

	possibleContainers.DeleteAll();
	possibleContainers.AddEntry(OBJ_WINDOW);

	owner				= NIL;

#ifdef __WIN32__
	style				= WS_BORDER|WS_POPUP;

	SetExStyle(WS_EX_TOOLWINDOW);
#endif
}

SMOOTHToolWindow::~SMOOTHToolWindow()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHToolWindow::SetOwner(SMOOTHObject *newOwner)
{
	owner = newOwner;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHToolWindow::FreeOwner()
{
	owner = NIL;

	return SMOOTH::Success;
}

HWND SMOOTHToolWindow::Create()
{
	if (registered && !created)
	{
		hwnd = CreateSimpleWindow(SMOOTHRect(objectProperties->pos, objectProperties->size), objectProperties->text, className, sysicon, style, exstyle);

		if (hwnd != NIL)
		{
			created = SMOOTH::True;
			visible = SMOOTH::False;

			return hwnd;
		}
		else
		{
			return NIL;
		}
	}

	return NIL;
}

SMOOTHInt SMOOTHToolWindow::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHObject	*object;

	if (created)
	{
		for (SMOOTHInt i = 0; i < nOfObjects; i++)
		{
			object = assocObjects.GetNthEntry(i);

			object->Paint(SP_PAINT);
		}
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHToolWindow::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	EnterProtectedRegion();

	if (!(message == SM_MOUSEMOVE && wParam == 1)) SMOOTHMessageProcCall(messageProc, messageProcParam, message, wParam, lParam);

	switch (message)
	{
		case SM_MOUSEMOVE:
			if (!cursorset) LiSASetMouseCursor(LiSA_MOUSE_ARROW);

			break;
#ifdef __WIN32__
		case WM_CLOSE:
			if (SMOOTHKillProcCall(killProc, killProcParam))
			{
				DestroyWindow(hwnd);

				hwnd = NIL;
			}

			LeaveProtectedRegion();

			return 0;
		case WM_DESTROY:
			destroyed = SMOOTH::True;

			if (nOfActiveWindows == 1 && loopActive)
			{
				if (SMOOTH::Setup::enableUnicode)	SendMessageW(hwnd, WM_QUIT, 0, 0);
				else					SendMessageA(hwnd, WM_QUIT, 0, 0);
			}
			else
			{
				nOfActiveWindows--;
			}

			LeaveProtectedRegion();

			return 0;
		case WM_QUIT:
			destroyed = SMOOTH::True;

			nOfActiveWindows--;

			LeaveProtectedRegion();

			PostQuitMessage(0);

			return 0;
		case WM_PAINT:
			if (::GetUpdateRect(hwnd, NULL, 0) == 0)
			{
				updateRect.left		= 0;
				updateRect.top		= 0;
				updateRect.right	= objectProperties->size.cx;
				updateRect.bottom	= objectProperties->size.cy;

				PAINTSTRUCT	 ps;

				BeginPaint(hwnd, &ps);

				Paint(SP_PAINT);

				EndPaint(hwnd, &ps);
			}
			else
			{
				RECT	 uRect = updateRect;

				if (::GetUpdateRect(hwnd, &uRect, 0))
				{
					updateRect = uRect;

					updateRect.right	+= 5;
					updateRect.bottom	+= 5;

					PAINTSTRUCT	 ps;

					BeginPaint(hwnd, &ps);

					Paint(SP_PAINT);

					EndPaint(hwnd, &ps);
				}
				else
				{
					updateRect = uRect;
				}
			}

			LeaveProtectedRegion();

			return 0;
		case WM_WINDOWPOSCHANGED:
			{
				WINDOWPOS	*wndPos = (LPWINDOWPOS) lParam;
				SMOOTHObject	*object = NIL;

				for (SMOOTHInt i = 0; i < nOfObjects; i++)
				{
					object = assocObjects.GetNthEntry(i);

					if (object->GetObjectProperties()->orientation == OR_CENTER)
					{
						object->GetObjectProperties()->size.cx = wndPos->cx;
						object->GetObjectProperties()->size.cy = wndPos->cy;
					}
				}

				objectProperties->pos.x		= wndPos->x;
				objectProperties->pos.y		= wndPos->y;
				objectProperties->size.cx	= wndPos->cx;
				objectProperties->size.cy	= wndPos->cy;
			}

			updateRect.left		= 0;
			updateRect.top		= 0;
			updateRect.right	= updateRect.left + objectProperties->size.cx;
			updateRect.bottom	= updateRect.top + objectProperties->size.cy;

			break;
#endif
	}

	SMOOTHObject	*object = NIL;

	for (SMOOTHInt j = nOfObjects - 1; j >= 0; j--)
	{
		object = assocObjects.GetNthEntry(j);

		if (object->Process(message, wParam, lParam) == SMOOTH::Break)
		{
			LeaveProtectedRegion();

			return SMOOTH::Break;
		}
	}

	if (owner != NIL) owner->Process(message, wParam, lParam);
	
	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHToolWindow::RegisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);
			nOfObjects++;

			object->SetContainer(this);
			object->SetRegisteredFlag();

			switch (object->GetObjectProperties()->orientation)
			{
				case OR_CENTER:
					object->GetObjectProperties()->pos.x	= 0;
					object->GetObjectProperties()->pos.y	= 0;

					object->GetObjectProperties()->size.cy	= objectProperties->size.cy;
					object->GetObjectProperties()->size.cx	= objectProperties->size.cx;

					break;
			}

			object->Show();

			return SMOOTH::Success;
		}
	}

	return SMOOTH::Error;
}

SMOOTHBool SMOOTHToolWindow::IsTypeCompatible(SMOOTHInt compType)
{
	if (compType == OBJ_OBJECT || compType == OBJ_WINDOW)	return SMOOTH::True;
	else							return SMOOTH::False;
}

#endif
