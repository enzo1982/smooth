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

	registered		= False;

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

S::Bool S::Object::IsRegistered()
{
	return registered;
}

S::Point S::Object::GetRealPosition()
{
	if (!registered) return objectProperties->pos;

	Point		 realPos = objectProperties->pos;

	if (myContainer->GetContainerObject()->GetObjectType() == OBJ_LAYER)
	{
		GUI::Layer	*layer = (GUI::Layer *) myContainer->GetContainerObject();
		Point		 layerPos = layer->GetRealPosition();

		realPos.x = layerPos.x + objectProperties->pos.x;
		realPos.y = layerPos.y + objectProperties->pos.y;

		if (objectProperties->orientation == OR_UPPERRIGHT)
		{
			realPos.x = layerPos.x + layer->GetObjectProperties()->size.cx - objectProperties->pos.x;
		}
		else if (objectProperties->orientation == OR_LOWERLEFT)
		{
			realPos.y = layerPos.y + layer->GetObjectProperties()->size.cy - objectProperties->pos.y;
		}
		else if (objectProperties->orientation == OR_LOWERRIGHT)
		{
			realPos.x = layerPos.x + layer->GetObjectProperties()->size.cx - objectProperties->pos.x;
			realPos.y = layerPos.y + layer->GetObjectProperties()->size.cy - objectProperties->pos.y;
		}
	}
	else
	{
		Object	*container = myContainer->GetContainerObject();

		if (objectProperties->orientation == OR_UPPERRIGHT)
		{
			realPos.x = container->GetObjectProperties()->size.cx - objectProperties->pos.x;
		}
		else if (objectProperties->orientation == OR_LOWERLEFT)
		{
			realPos.y = container->GetObjectProperties()->size.cy - objectProperties->pos.y;
		}
		else if (objectProperties->orientation == OR_LOWERRIGHT)
		{
			realPos.x = container->GetObjectProperties()->size.cx - objectProperties->pos.x;
			realPos.y = container->GetObjectProperties()->size.cy - objectProperties->pos.y;
		}
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
	objectProperties->textSize.cx = GetTextSizeX(objectProperties->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);
	objectProperties->textSize.cy = GetTextSizeY(objectProperties->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

#ifdef __WIN32__
	HDC	 dc = GetWindowDC(0);

	objectProperties->tooltipSize.cx = GetTextSizeX(objectProperties->tooltip, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_NORMAL);
	objectProperties->tooltipSize.cy = GetTextSizeY(objectProperties->tooltip, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_NORMAL);

	ReleaseDC(0, dc);
	DeleteDC(dc);
#endif
}

S::Object *S::Object::GetObject(Int objectHandle, Int objectType)
{
	Object	*object = mainObjectManager->RequestObject(objectHandle);

	if (object == NIL) return NIL;

	if (object->GetObjectType() == objectType)	return object;
	else						return NIL;
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
