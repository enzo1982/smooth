 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LAYER_
#define _H_OBJSMOOTH_LAYER_

#define SLayer SMOOTHLayer

class SMOOTHLayer;

#include "object.h"
#include "container.h"

class SMOOTHAPI SMOOTHLayer : public SMOOTHObject, public SMOOTHContainer
{
	protected:
		SMOOTHInt		 layerColor;
	public:
					 SMOOTHLayer(SMOOTHString name = NIL);
					~SMOOTHLayer();

		SMOOTHInt		 Show();
		SMOOTHInt		 Hide();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetColor(SMOOTHInt);
		SMOOTHInt		 SetMetrics(SMOOTHPoint, SMOOTHSize);

		SMOOTHSurface		*GetDrawSurface();

		SMOOTHInt		 RegisterObject(SMOOTHObject *);
		SMOOTHInt		 UnregisterObject(SMOOTHObject *);
};

SMOOTHVAR SMOOTHInt OBJ_LAYER;

#endif
