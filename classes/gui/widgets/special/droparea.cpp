 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/special/droparea.h>
#include <smooth/gui/window/window.h>

#include <shellapi.h>

const S::Int	 S::GUI::DropArea::classID = S::Object::RequestClassID();

S::GUI::DropArea::DropArea(const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type		= classID;
	orientation	= OR_FREE;
	initialized	= False;

	if (GetSize() == Size(0, 0)) SetSize(Size(4096, 4096));

	onDropFile.SetParentObject(this);
}

S::GUI::DropArea::~DropArea()
{
}

S::Int S::GUI::DropArea::Hide()
{
	visible = False;

	return Success();
}

S::Int S::GUI::DropArea::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())			return Error();
	if (!IsActive() || !IsVisible())	return Success();

	if (!initialized)
	{
		// Enable Drag & Drop only on the Windows NT family for now, due to problems on Windows 9x
		{
			OSVERSIONINFOA	 vInfo;

			vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

			GetVersionExA(&vInfo);

			if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) DragAcceptFiles((HWND) container->GetContainerWindow()->GetSystemWindow(), True);
		}

		initialized = True;
	}

	Int	 retVal = Success();
	Point	 realPos = GetRealPosition();

	EnterProtectedRegion();

	switch (message)
	{
		case WM_DROPFILES:
			{
				HDROP	 hDrop = (HDROP) wParam;
				POINT	 pPos;

				DragQueryPoint(hDrop, &pPos);

				Point	 pos(pPos.x, pPos.y);

				if (pos.x > realPos.x && pos.x < (realPos.x + GetWidth()) && pos.y > realPos.y && pos.y < (realPos.y + GetHeight()))
				{
					Int	 nOfFiles;

					if (Setup::enableUnicode)	nOfFiles = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
					else				nOfFiles = DragQueryFileA(hDrop, 0xFFFFFFFF, NULL, 0);

					for (Int i = 0; i < nOfFiles; i++)
					{
						Int	 bufferSize = 32768;

						if (Setup::enableUnicode)
						{
							wchar_t	*bufferW = new wchar_t [bufferSize];

							DragQueryFileW(hDrop, i, bufferW, bufferSize);

							onDropFile.Emit(String(bufferW));

							delete [] bufferW;
						}
						else
						{
							char	*bufferA = new char [bufferSize];

							DragQueryFileA(hDrop, i, bufferA, bufferSize);

							onDropFile.Emit(String(bufferA));

							delete [] bufferA;
						}
					}

					DragFinish(hDrop);

					retVal = Break;
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return retVal;
}
