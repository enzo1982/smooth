 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window.h>
#include <smooth/layer.h>
#include <smooth/droparea.h>
#include <smooth/definitions.h>
#include <smooth/objectproperties.h>
#include <smooth/stk.h>
#include <smooth/loop.h>
#include <smooth/math.h>

#include <shellapi.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_DROPAREA = S::Object::RequestObjectID();

S::GUI::DropArea::DropArea(Point pos, Size size)
{
	type				= OBJ_DROPAREA;
	objectProperties->orientation	= OR_CENTER;

	possibleContainers.AddEntry(OBJ_LAYER);

	onRegister.Connect(&DropArea::OnRegister, this);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = LiSAGetDisplaySizeX();
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = LiSAGetDisplaySizeY();
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
}

S::GUI::DropArea::~DropArea()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Void S::GUI::DropArea::OnRegister(Container *container)
{
	container->GetContainerWindow()->onCreate.Connect(&DropArea::Init, this);
}

S::Void S::GUI::DropArea::Init()
{
	Window	*wnd = myContainer->GetContainerWindow();

	DragAcceptFiles(wnd->hwnd, True);
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

	Window	*wnd = myContainer->GetContainerWindow();

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

				if (pos.x > realPos.x && pos.x < (realPos.x + objectProperties->size.cx) && pos.y > realPos.y && pos.y < (realPos.y + objectProperties->size.cy))
				{
					Int	 nOfFiles;

					if (Setup::enableUnicode)	nOfFiles = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
					else				nOfFiles = DragQueryFileA(hDrop, 0xFFFFFFFF, NULL, 0);

					for (Int i = 0; i < nOfFiles; i++)
					{
						Int	 bufferSize = 32768;
						char	*bufferA = new char [bufferSize];
						wchar_t	*bufferW = new wchar_t [bufferSize];

						if (Setup::enableUnicode)	DragQueryFileW(hDrop, i, bufferW, bufferSize);
						else				DragQueryFileA(hDrop, i, bufferA, bufferSize);

						if (Setup::enableUnicode)	onDropFile.Emit(new String(bufferW));
						else				onDropFile.Emit(new String(bufferA));

						delete [] bufferA;
						delete [] bufferW;
					}

					DragFinish(hDrop);

					retVal = Break;
				}
			}

			break;
	}

	return retVal;
}
