 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECT_
#define _H_OBJSMOOTH_OBJECT_

#define SObject SMOOTHObject

class SMOOTHObject;
class SMOOTHContainer;
class SMOOTHObjectProperties;

#include "definitions.h"
#include "string.h"
#include "array.h"
#include "objecttype.h"

const SMOOTHInt SP_PAINT	= 0;
const SMOOTHInt SP_MOUSEIN	= 1;
const SMOOTHInt SP_MOUSEOUT	= 2;
const SMOOTHInt SP_MOUSEDOWN	= 3;
const SMOOTHInt SP_MOUSEUP	= 4;

class SMOOTHAPI SMOOTHObject
{
	private:
		SMOOTHBool		 deleteObject;
		SMOOTHInt		 inUse;
	protected:
		SMOOTHObjectType	 type;

		SMOOTHBool		 registered;
		SMOOTHBool		 visible;
		SMOOTHBool		 active;

		SMOOTHObjectProperties	*objectProperties;
		SMOOTHContainer		*myContainer;

		SMOOTHVoid		 GetTextSize();

		SMOOTHInt		 EnterProtectedRegion();
		SMOOTHInt		 LeaveProtectedRegion();
	public:
		SMOOTHArray<SMOOTHInt>	 possibleContainers;
		static SMOOTHInt	 objectCount;

		SMOOTHInt		 subtype;
		SMOOTHInt		 handle;

					 SMOOTHObject();
		virtual			~SMOOTHObject();

		virtual SMOOTHInt	 Show();
		virtual SMOOTHInt	 Hide();

		virtual SMOOTHInt	 Activate();
		virtual SMOOTHInt	 Deactivate();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		virtual SMOOTHInt	 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		virtual SMOOTHInt	 SetText(SMOOTHString);
		virtual SMOOTHString	 GetText();

		virtual SMOOTHInt	 SetTooltip(SMOOTHString);
		virtual SMOOTHString	 GetTooltip();

		virtual SMOOTHInt	 SetProc(SMOOTHProcParam, SMOOTHVoid *);
		virtual SMOOTHInt	 SetFont(SMOOTHString, SMOOTHInt, SMOOTHInt, SMOOTHInt);
		virtual SMOOTHInt	 SetOrientation(SMOOTHInt);
		virtual SMOOTHInt	 SetPosition(SMOOTHPoint);
		virtual SMOOTHInt	 SetMetrics(SMOOTHPoint, SMOOTHSize);

		SMOOTHBool		 IsRegistered();
		SMOOTHBool		 IsVisible();
		SMOOTHBool		 IsActive();

		virtual SMOOTHPoint	 GetRealPosition();

		virtual SMOOTHBool	 IsTypeCompatible(SMOOTHInt);

		SMOOTHObjectProperties	*GetObjectProperties();
		SMOOTHObjectType	 GetObjectType();

		SMOOTHInt		 SetContainer(SMOOTHContainer *);
		SMOOTHContainer		*GetContainer();

		SMOOTHVoid		 SetRegisteredFlag();
		SMOOTHVoid		 UnsetRegisteredFlag();

		SMOOTHInt		 IsObjectInUse();
		SMOOTHBool		 IsObjectDeleteable();

		SMOOTHInt		 DeleteObject();
};

SMOOTHVAR SMOOTHInt OBJ_OBJECT;

#endif
