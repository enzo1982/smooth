 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_APPLICATION_
#define __OBJSMOOTH_APPLICATION_

#include <smooth/application.h>
#include <smooth/window.h>
#include <smooth/i18n.h>
#include <smooth/objectproperties.h>
#include <smooth/stk.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_APPLICATION = SMOOTH::RequestObjectID();

SMOOTHApplication::SMOOTHApplication()
{
	self		= this;

	type		= OBJ_APPLICATION;
	containerType	= OBJ_APPLICATION;

	objectProperties->text		= TXT_SMOOTHAPPLICATION;
	objectProperties->size.cx	= LiSAGetDisplaySizeX();
	objectProperties->size.cy	= LiSAGetDisplaySizeY();
}

SMOOTHApplication::SMOOTHApplication(SMOOTHString name)
{
	self		= this;

	type		= OBJ_APPLICATION;
	containerType	= OBJ_APPLICATION;

	objectProperties->text		= name;
	objectProperties->size.cx	= LiSAGetDisplaySizeX();
	objectProperties->size.cy	= LiSAGetDisplaySizeY();
}

#endif
