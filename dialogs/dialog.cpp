 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DIALOG_
#define __OBJSMOOTH_DIALOG_

#include <smooth/dialog.h>
#include <smooth/stk.h>

SMOOTHDialog::SMOOTHDialog()
{
	parentWindow = NIL;
}

SMOOTHDialog::~SMOOTHDialog()
{
}

SMOOTHInt SMOOTHDialog::SetCaption(SMOOTHString newCaption)
{
	caption = newCaption;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHDialog::SetParentWindow(SMOOTHWindow *newParent)
{
	parentWindow = newParent;

	return SMOOTH::Success;
}

#endif
