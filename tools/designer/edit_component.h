 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DESIGNER_EDITCOMPONENT_
#define _H_OBJSMOOTH_DESIGNER_EDITCOMPONENT_

class Designer_EditComponent;

#include <smoothx.h>
#include "designer.h"
#include "status.h"

class Designer_EditComponent : public SMOOTHApplication
{
	private:
		SMOOTHWindow			*wnd;
		SMOOTHTitlebar			*title;

		SMOOTHArray<SMOOTHObject *>	 objects;

		Designer			*designer;
		Designer_Status			 status;

		SMOOTHVoid			 MessageProc(SMOOTHInt, SMOOTHInt, SMOOTHInt);
		SMOOTHBool			 KillProc();
	public:
						 Designer_EditComponent(Designer *, SMOOTHString);
						~Designer_EditComponent();
		SMOOTHVoid			 ShowDialog();
		SMOOTHString			 GetName();
		SMOOTHWindow			*GetWindow();
		SMOOTHObject			*AddObject(SMOOTHInt);
		SMOOTHObject			*GetFirstObject(SMOOTHInt);
};

const SMOOTHInt	 SMOOTH_BUTTON		=  1;
const SMOOTHInt	 SMOOTH_MENUBAR		=  2;
const SMOOTHInt	 SMOOTH_LAYER		=  3;

#endif
