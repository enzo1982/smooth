 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
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

#include <smooth.h>
#include "designer.h"
#include "status.h"

using namespace smooth;

class Designer_EditComponent : public Application
{
	private:
		Window		*wnd;
		Titlebar	*title;

		Array<Object *>	 objects;

		Designer	*designer;
		Designer_Status	 status;

		Void		 EventProc(Int, Int, Int);
		Bool		 ExitProc();
	public:
				 Designer_EditComponent(Designer *, String);
				~Designer_EditComponent();
		Void		 ShowDialog();
		String		 GetName();
		Window		*GetWindow();
		Object		*AddObject(Int);
		Object		*GetFirstObject(Int);
};

const Int	 SMOOTH_BUTTON		=  1;
const Int	 SMOOTH_MENUBAR		=  2;
const Int	 SMOOTH_LAYER		=  3;

#endif
