 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/application.h>
#include <smooth/i18n.h>
#include <smooth/objectproperties.h>

const S::Int	 S::Application::classID = S::Object::RequestClassID();

S::Application::Application(String name)
{
	self		= this;

	type		= classID;
	containerType	= classID;

	objectProperties->text		= name == NIL ? TXT_SMOOTHAPPLICATION : name;
	objectProperties->size.cx	= LiSAGetDisplaySizeX();
	objectProperties->size.cy	= LiSAGetDisplaySizeY();
}
