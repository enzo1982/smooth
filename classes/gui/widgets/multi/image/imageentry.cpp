 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/image/imageentry.h>
#include <smooth/gui/widgets/basic/image.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::ImageEntry::classID = S::Object::RequestClassID();

S::GUI::ImageEntry::ImageEntry(const Bitmap &iBitmap, const Size &iSize) : ListEntry(NIL)
{
	type	= classID;

	SetSize(iSize);

	image	= new Image(iBitmap, Point(2, 2), iSize - Size(4, 4));

	Add(image);

	onChangeSize.Connect(&ImageEntry::OnChangeSize, this);
}

S::GUI::ImageEntry::~ImageEntry()
{
	DeleteObject(image);
}

S::Int S::GUI::ImageEntry::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface	= container->GetDrawSurface();
	Point	 realPos	= GetRealPosition();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->StartPaint(GetVisibleArea());

			surface->Box(Rect(realPos, GetSize()), Setup::ClientColor, Rect::Filled);
			image->Paint(SP_PAINT);

			surface->EndPaint();

			break;
		case SP_MOUSEIN:
			surface->StartPaint(GetVisibleArea());

			surface->Box(Rect(realPos,				Size(GetWidth(), 2)), Setup::GradientStartColor, Rect::Filled);
			surface->Box(Rect(realPos,				Size(2, GetHeight())), Setup::GradientStartColor, Rect::Filled);
			surface->Box(Rect(realPos + Point(GetWidth() - 2, 0),	Size(2, GetHeight())), Setup::GradientStartColor, Rect::Filled);
			surface->Box(Rect(realPos + Point(0, GetHeight() - 2),	Size(GetWidth(), 2)), Setup::GradientStartColor, Rect::Filled);

			surface->EndPaint();

			break;
		case SP_MOUSEOUT:
			surface->StartPaint(GetVisibleArea());

			surface->Box(Rect(realPos,				Size(GetWidth(), 2)), Setup::ClientColor, Rect::Filled);
			surface->Box(Rect(realPos,				Size(2, GetHeight())), Setup::ClientColor, Rect::Filled);
			surface->Box(Rect(realPos + Point(GetWidth() - 2, 0),	Size(2, GetHeight())), Setup::ClientColor, Rect::Filled);
			surface->Box(Rect(realPos + Point(0, GetHeight() - 2),	Size(GetWidth(), 2)), Setup::ClientColor, Rect::Filled);

			surface->EndPaint();

			break;
	}

	return Success();
}

S::Void S::GUI::ImageEntry::OnChangeSize(const Size &newSize)
{
	image->SetSize(newSize - Size(4, 4));
}

S::Bool S::GUI::ImageEntry::IsTypeCompatible(Int compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == ListEntry::classID)	return True;
	else													return False;
}
