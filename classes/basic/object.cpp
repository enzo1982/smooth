 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_OBJECT_
#define __OBJSMOOTH_OBJECT_

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

SMOOTHInt	 OBJ_OBJECT = SMOOTH::RequestObjectID();
SMOOTHInt	 SMOOTHObject::objectCount = 0;

SMOOTHObject::SMOOTHObject()
{
	objectProperties		= new SMOOTHObjectProperties();

	handle				= SMOOTH::RequestObjectHandle();

	type.object			= this;
	type				= OBJ_OBJECT;
	subtype				= 0;

	objectProperties->proc		= NIL;
	objectProperties->procParam	= NIL;

	registered			= SMOOTH::False;
	visible				= SMOOTH::False;
	active				= SMOOTH::True;

	deleteObject			= SMOOTH::False;
	inUse				= 0;

	myContainer			= NIL;

	mainObjectManager->RegisterObject(this);
}

SMOOTHObject::~SMOOTHObject()
{
	mainObjectManager->UnregisterObject(this);

	delete objectProperties;
}

SMOOTHInt SMOOTHObject::EnterProtectedRegion()
{
	return ++inUse;
}

SMOOTHInt SMOOTHObject::LeaveProtectedRegion()
{
	return --inUse;
}

SMOOTHInt SMOOTHObject::IsObjectInUse()
{
	return inUse;
}

SMOOTHBool SMOOTHObject::IsObjectDeleteable()
{
	return deleteObject;
}

SMOOTHInt SMOOTHObject::DeleteObject()
{
	deleteObject = SMOOTH::True;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::Show()
{
	if (visible)		return SMOOTH::Success;

	visible = SMOOTH::True;

	if (!registered)	return SMOOTH::Success;

	Paint(SP_PAINT);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::Hide()
{
	if (!visible)		return SMOOTH::Success;

	visible = SMOOTH::False;

	if (!registered)	return SMOOTH::Success;

	SMOOTHRect	 rect;
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHSurface	*surface = myContainer->GetDrawSurface();

	rect.left	= realPos.x;
	rect.top	= realPos.y;
	rect.right	= realPos.x + objectProperties->size.cx + 1;
	rect.bottom	= realPos.y + objectProperties->size.cy + 1;

	surface->Box(rect, SMOOTH::Setup::BackgroundColor, FILLED);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::Activate()
{
	if (active) return SMOOTH::Success;

	SMOOTHBool	 prevVisible = visible;

	if (registered && visible) Hide();

	active = SMOOTH::True;

	if (registered && prevVisible) Show();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::Deactivate()
{
	if (!active) return SMOOTH::Success;

	SMOOTHBool	 prevVisible = visible;

	if (registered && visible) Hide();

	active = SMOOTH::False;

	if (registered && prevVisible) Show();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::SetText(SMOOTHString newText)
{
	SMOOTHBool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->text = newText;

	GetTextSize();

	objectProperties->checked = SMOOTH::False;
	objectProperties->clicked = SMOOTH::False;

	if (registered && prevVisible) Show();

	Process(SM_MOUSEMOVE, 0, 0);

	return SMOOTH::Success;
}

SMOOTHString SMOOTHObject::GetText()
{
	return objectProperties->text;
}

SMOOTHInt SMOOTHObject::SetTooltip(SMOOTHString newTooltip)
{
	objectProperties->tooltip = newTooltip;

	GetTextSize();

	return SMOOTH::Success;
}

SMOOTHString SMOOTHObject::GetTooltip()
{
	return objectProperties->tooltip;
}

SMOOTHInt SMOOTHObject::SetProc(SMOOTHProcParam, SMOOTHVoid *newProcParam)
{
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= newProcParam;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::SetFont(SMOOTHString newFont, SMOOTHInt newFontSize, SMOOTHInt newFontColor, SMOOTHInt newFontWeight)
{
	SMOOTHBool	 prevVisible = visible;

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

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::SetOrientation(SMOOTHInt newOrientation)
{
	SMOOTHBool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->orientation = newOrientation;

	if (registered && prevVisible) Show();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::SetPosition(SMOOTHPoint newPos)
{
	SMOOTHBool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->pos = newPos;

	if (registered && prevVisible) Show();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObject::SetMetrics(SMOOTHPoint newPos, SMOOTHSize newSize)
{
	SMOOTHBool	 prevVisible = visible;

	if (registered && visible) Hide();

	objectProperties->pos	= newPos;
	objectProperties->size	= newSize;

	if (registered && prevVisible) Show();

	return SMOOTH::Success;
}

SMOOTHBool SMOOTHObject::IsRegistered()
{
	return registered;
}

SMOOTHBool SMOOTHObject::IsVisible()
{
	return visible;
}

SMOOTHBool SMOOTHObject::IsActive()
{
	return active;
}

SMOOTHPoint SMOOTHObject::GetRealPosition()
{
	if (!registered) return objectProperties->pos;

	SMOOTHLayer	*layer = NIL;
	SMOOTHPoint	 realPos = objectProperties->pos;

	if (myContainer->GetContainerObject()->GetObjectType() == OBJ_LAYER)	layer = (SMOOTHLayer *) myContainer->GetContainerObject();
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

SMOOTHBool SMOOTHObject::IsTypeCompatible(SMOOTHInt objType)
{
	if (objType == OBJ_OBJECT)	return SMOOTH::True;
	else				return SMOOTH::False;
}

SMOOTHObjectProperties *SMOOTHObject::GetObjectProperties()
{
	return objectProperties;
}

SMOOTHObjectType SMOOTHObject::GetObjectType()
{
	return type;
}

SMOOTHInt SMOOTHObject::SetContainer(SMOOTHContainer *newContainer)
{
	myContainer = newContainer;

	return SMOOTH::Success;
}

SMOOTHContainer *SMOOTHObject::GetContainer()
{
	return myContainer;
}

SMOOTHVoid SMOOTHObject::SetRegisteredFlag()
{
	registered = SMOOTH::True;
}

SMOOTHVoid SMOOTHObject::UnsetRegisteredFlag()
{
	registered = SMOOTH::False;
}

SMOOTHVoid SMOOTHObject::GetTextSize()
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

#endif
