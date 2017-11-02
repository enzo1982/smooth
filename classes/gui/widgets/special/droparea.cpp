 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/special/droparea.h>
#include <smooth/gui/window/window.h>
#include <smooth/foreach.h>

const S::Short	 S::GUI::DropArea::classID = S::Object::RequestClassID();

S::GUI::DropArea::DropArea(const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type	    = classID;
	dropTarget  = True;
	orientation = OR_FREE;

	if (GetSize() == Size(0, 0)) SetSize(Size(4096, 4096));

	onDropFile.SetParentObject(this);
	onDropFiles.SetParentObject(this);
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
	if (!IsRegistered())		 return Error();
	if (!IsActive() || !IsVisible()) return Success();

	switch (message)
	{
		case SM_DROPFILES:
			{
				Point	 mousePos = Point(wParam, lParam);

				Point	 realPos  = GetRealPosition();
				Size	 realSize = GetRealSize();

				if (mousePos.x > realPos.x && mousePos.x < (realPos.x + realSize.cx) && mousePos.y > realPos.y && mousePos.y < (realPos.y + realSize.cy))
				{
					const Array<String>	&fileNames = GetContainerWindow()->GetDroppedFiles();

					EnterProtectedRegion();

					foreach (const String &fileName, fileNames) onDropFile.Emit(fileName);

					onDropFiles.Emit(fileNames);

					LeaveProtectedRegion();

					return MessageProcessed;
				}
			}

			break;
	}

	return Widget::Process(message, wParam, lParam);
}
