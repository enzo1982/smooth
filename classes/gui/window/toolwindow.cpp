 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/toolwindow.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/window/backends/windowbackend.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/color.h>

const S::Int	 S::GUI::ToolWindow::classID = S::Object::RequestClassID();

S::GUI::ToolWindow::ToolWindow(const Point &iPos, const Size &iSize) : Window("smooth ToolWindow", iPos, iSize)
{
	type		= classID;
	orientation	= OR_FREE;
	owner		= NIL;
	visible		= True;

	innerOffset = Rect(Point(0, 0), Size(0, 0));

	SetFlags(WF_TOPMOST | WF_NOTASKBUTTON | WF_THINBORDER);
}

S::GUI::ToolWindow::~ToolWindow()
{
}

S::Int S::GUI::ToolWindow::SetOwner(Widget *newOwner)
{
	owner = newOwner;

	return Success();
}

S::Int S::GUI::ToolWindow::FreeOwner()
{
	owner = NIL;

	return Success();
}

S::Int S::GUI::ToolWindow::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!created)		return Success();
	if (!IsVisible())	return Success();

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

	if (message == SP_UPDATE)
	{
		surface->PaintRect(updateRect);
	}
	else
	{
		Int	 color = Color(192, 192, 192);

#ifdef __WIN32__
		color = GetSysColor(COLOR_BTNFACE);
#endif

		surface->Box(updateRect, color, FILLED);

		onPaint.Emit();

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			GetNthObject(i)->Paint(SP_PAINT);
		}
	}

	LeaveProtectedRegion();

	return Success();
}

S::Int S::GUI::ToolWindow::Process(Int message, Int wParam, Int lParam)
{
	if (!created) return Success();

	EnterProtectedRegion();

	if (!(message == SM_MOUSEMOVE && wParam == 1)) onEvent.Emit(message, wParam, lParam);

	Int	 rVal = -1;

#ifdef __WIN32__
	switch (message)
	{
		case WM_CLOSE:
			if (doQuit.Call()) backend->Close();

			rVal = 0;

			break;
		case WM_DESTROY:
			destroyed = True;

			if (nOfActiveWindows == 0 && loopActive)
			{
				if (Setup::enableUnicode)	::SendMessageW((HWND) backend->GetSystemWindow(), WM_QUIT, 0, 0);
				else				::SendMessageA((HWND) backend->GetSystemWindow(), WM_QUIT, 0, 0);
			}
			else
			{
				nOfActiveWindows--;
			}

			rVal = 0;

			break;
		case WM_QUIT:
			destroyed = True;

			nOfActiveWindows--;

			LeaveProtectedRegion();

			PostQuitMessage(0);

			return 0;
		case WM_PAINT:
			{
				RECT	 uRect = { 0, 0, 0, 0 };

				updateRect = uRect;

				if (::GetUpdateRect((HWND) backend->GetSystemWindow(), &uRect, 0))
				{
					updateRect = uRect;

					updateRect.right	+= 5;
					updateRect.bottom	+= 5;

					PAINTSTRUCT	 ps;

					BeginPaint((HWND) backend->GetSystemWindow(), &ps);

					if (Math::Abs((updateRect.right - updateRect.left) - GetWidth()) < 20 && Math::Abs((updateRect.bottom - updateRect.top) - GetHeight()) < 20)	Paint(SP_PAINT);
					else																		Paint(SP_UPDATE);

					EndPaint((HWND) backend->GetSystemWindow(), &ps);
				}
			}

			rVal = 0;

			break;
		case WM_WINDOWPOSCHANGED:
			{
				WINDOWPOS	*wndPos = (LPWINDOWPOS) lParam;

				for (Int i = 0; i < GetNOfObjects(); i++)
				{
					Widget	*object = GetNthObject(i);

					if (object->GetOrientation() == OR_CENTER)
					{
						object->SetSize(Size(wndPos->cx, wndPos->cy));
					}
				}

				SetMetrics(Point(wndPos->x, wndPos->y), Size(wndPos->cx, wndPos->cy));

				GetDrawSurface()->SetSize(GetSize());
			}

			updateRect = Rect(Point(0, 0), GetSize());

			break;
	}
#endif

	if (rVal == -1)
	{
		for (Int i = GetNOfObjects() - 1; i >= 0; i--)
		{
			Widget	*object = GetNthObject(i);

			if (object == NIL) continue;

			if (object->Process(message, wParam, lParam) == Break)
			{
				rVal = 0;

				break;
			}
		}
	}

	LeaveProtectedRegion();

	if (owner != NIL) owner->Process(message, wParam, lParam);
	
	return rVal == -1 ? Success() : Break;
}

S::Bool S::GUI::ToolWindow::IsTypeCompatible(Int compType)
{
	if (compType == Object::classID || compType == Widget::classID || compType == Window::classID)	return True;
	else												return False;
}
