 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/gui/widgets/special/droparea.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>

#include <shellapi.h>

const S::Int	 S::GUI::DropArea::classID = S::Object::RequestClassID();

S::GUI::DropArea::DropArea(Point iPos, Size iSize)
{
	type		= classID;
	orientation	= OR_CENTER;

	possibleContainers.AddEntry(Layer::classID);

	onRegister.Connect(&DropArea::OnRegister, this);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = LiSAGetDisplaySizeX();
	if (size.cy == 0) size.cy = LiSAGetDisplaySizeY();

	onDropFile.SetParentObject(this);
}

S::GUI::DropArea::~DropArea()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Void S::GUI::DropArea::OnRegister(Container *container)
{
	container->GetContainerWindow()->onCreate.Connect(&DropArea::Init, this);
}

S::Void S::GUI::DropArea::Init()
{
	Window	*wnd = (Window *) container->GetContainerWindow();

	DragAcceptFiles((HWND) wnd->GetSystemWindow(), True);
}

S::Int S::GUI::DropArea::Hide()
{
	if (!visible)		return Success;

	visible = False;

	if (!registered)	return Success;

	return Success;
}

S::Int S::GUI::DropArea::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Int	 retVal = Success;
	Point	 realPos = GetRealPosition();

	switch (message)
	{
		case WM_DROPFILES:
			{
				HDROP	 hDrop = (HDROP) wParam;
				POINT	 pPos;

				DragQueryPoint(hDrop, &pPos);

				Point	 pos(pPos.x, pPos.y);

				if (pos.x > realPos.x && pos.x < (realPos.x + size.cx) && pos.y > realPos.y && pos.y < (realPos.y + size.cy))
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

							onDropFile.Emit(new String(bufferW));

							delete [] bufferW;
						}
						else
						{
							char	*bufferA = new char [bufferSize];

							DragQueryFileA(hDrop, i, bufferA, bufferSize);

							onDropFile.Emit(new String(bufferA));

							delete [] bufferA;
						}
					}

					DragFinish(hDrop);

					retVal = Break;
				}
			}

			break;
	}

	return retVal;
}
