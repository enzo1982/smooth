 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/object.h>
#include <smooth/objectproperties.h>
#include <smooth/objectmanager.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/window.h>
#include <smooth/layer.h>
#include <smooth/toolkit.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_OBJECT = S::Object::RequestObjectID();
S::Int	 S::Object::objectCount = 0;
S::Int	 S::Object::nextID = 0;

S::Object::Object()
{
	objectProperties	= new ObjectProperties();

	handle			= RequestObjectHandle();

	type.object		= this;
	type			= OBJ_OBJECT;
	subtype			= 0;

	registered		= False;
	visible			= False;
	active			= True;

	deleteObject		= False;
	inUse			= 0;

	myContainer		= NIL;

	mainObjectManager->RegisterObject(this);
}

S::Object::~Object()
{
	mainObjectManager->UnregisterObject(this);

	delete objectProperties;
}

S::Int S::Object::EnterProtectedRegion()
{
	return ++inUse;
}

S::Int S::Object::LeaveProtectedRegion()
{
	return --inUse;
}

S::Int S::Object::IsObjectInUse()
{
	return inUse;
}

S::Bool S::Object::IsObjectDeleteable()
{
	return deleteObject;
}

S::Int S::Object::Show()
{
	if (visible)		return Success;

	visible = True;

	if (!registered)	return Success;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::Object::Hide()
{
	if (!visible)		return Success;

	visible = False;

	if (!registered)	return Success;

	Rect		 rect;
	Point		 realPos = GetRealPosition();
	GUI::Surface	*surface = myContainer->GetDrawSurface();

	rect.left	= realPos.x;
	rect.top	= realPos.y;
	rect.right	= realPos.x + objectProperties->size.cx + 1;
	rect.bottom	= realPos.y + objectProperties->size.cy + 1;

	surface->Box(rect, SMOOTH::Setup::BackgroundColor, FILLED);

	return Success;
}

S::Int S::Object::Activate()
{
	if (active) return Success;

	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	active = True;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::Object::Deactivate()
{
	if (!active) return Success;

	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	active = False;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::Object::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	return Success;
}

S::Int S::Object::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	return Success;
}

S::Int S::Object::SetText(String newText)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->text = newText;

	GetTextSize();

	objectProperties->checked = False;
	objectProperties->clicked = False;

	if (registered && prevVisible) Show();

	Process(SM_MOUSEMOVE, 0, 0);

	return Success;
}

S::String S::Object::GetText()
{
	return objectProperties->text;
}

S::Int S::Object::SetTooltip(String newTooltip)
{
	objectProperties->tooltip = newTooltip;

	GetTextSize();

	return Success;
}

S::String S::Object::GetTooltip()
{
	return objectProperties->tooltip;
}

S::Int S::Object::SetFont(String newFont, Int newFontSize, Int newFontColor, Int newFontWeight)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->font = newFont;
	objectProperties->fontColor = newFontColor;

	HDC		 dc = GetContext(0);

#ifdef __WIN32__
	objectProperties->fontSize = -MulDiv(newFontSize, GetDeviceCaps(dc, LOGPIXELSY), 72);
#endif

	FreeContext(0, dc);

	objectProperties->fontWeight = newFontWeight;

	GetTextSize();

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::Object::SetOrientation(Int newOrientation)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->orientation = newOrientation;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::Object::SetPosition(Point newPos)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->pos = newPos;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::Object::SetMetrics(Point newPos, Size newSize)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->pos	= newPos;
	objectProperties->size	= newSize;

	if (registered && prevVisible) Show();

	return Success;
}

S::Bool S::Object::IsRegistered()
{
	return registered;
}

S::Bool S::Object::IsVisible()
{
	return visible;
}

S::Bool S::Object::IsActive()
{
	return active;
}

S::Point S::Object::GetRealPosition()
{
	if (!registered) return objectProperties->pos;

	GUI::Layer	*layer = NIL;
	Point		 realPos = objectProperties->pos;

	if (myContainer->GetContainerObject()->GetObjectType() == OBJ_LAYER)	layer = (GUI::Layer *) myContainer->GetContainerObject();
	else									return realPos;

	realPos.x = layer->GetObjectProperties()->pos.x + objectProperties->pos.x;
	realPos.y = layer->GetObjectProperties()->pos.y + objectProperties->pos.y;

	if (objectProperties->orientation == OR_UPPERRIGHT)
	{
		realPos.x = layer->GetObjectProperties()->pos.x + layer->GetObjectProperties()->size.cx - objectProperties->pos.x;
	}
	else if (objectProperties->orientation == OR_LOWERLEFT)
	{
		realPos.y = layer->GetObjectProperties()->pos.y + layer->GetObjectProperties()->size.cy - objectProperties->pos.y;
	}
	else if (objectProperties->orientation == OR_LOWERRIGHT)
	{
		realPos.x = layer->GetObjectProperties()->pos.x + layer->GetObjectProperties()->size.cx - objectProperties->pos.x;
		realPos.y = layer->GetObjectProperties()->pos.y + layer->GetObjectProperties()->size.cy - objectProperties->pos.y;
	}

	return realPos;
}

S::Bool S::Object::IsTypeCompatible(Int objType)
{
	if (objType == OBJ_OBJECT)	return True;
	else				return False;
}

S::ObjectProperties *S::Object::GetObjectProperties()
{
	return objectProperties;
}

S::ObjectType S::Object::GetObjectType()
{
	return type;
}

S::Int S::Object::SetContainer(Container *newContainer)
{
	myContainer = newContainer;

	return Success;
}

S::Container *S::Object::GetContainer()
{
	return myContainer;
}

S::Void S::Object::SetRegisteredFlag()
{
	registered = True;
}

S::Void S::Object::UnsetRegisteredFlag()
{
	registered = False;
}

S::Void S::Object::GetTextSize()
{
	HDC	 dc = GetContext(0);

	objectProperties->textSize.cx = GetTextSizeX(dc, objectProperties->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
	objectProperties->textSize.cy = GetTextSizeY(dc, objectProperties->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

#ifdef __WIN32__
	objectProperties->tooltipSize.cx = GetTextSizeX(dc, objectProperties->tooltip, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_NORMAL);
	objectProperties->tooltipSize.cy = GetTextSizeY(dc, objectProperties->tooltip, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_NORMAL);
#endif

	FreeContext(0, dc);
}

S::Int S::Object::DeleteObject(Object *object)
{
	if (object != NIL)
	{
		if (!object->IsObjectInUse())
		{
			delete object;

			return Success;
		}

		object->deleteObject = True;

		return Success;
	}

	return Error;
}

S::Int S::Object::RequestObjectID()
{
	return nextID++;
}

S::Int S::Object::RequestObjectHandle()
{
	return 	objectCount++;
}
