 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DIALOG_
#define _H_OBJSMOOTH_DIALOG_

#define SDialog SMOOTHDialog

class SMOOTHDialog;
class SMOOTHWindow;

#include "definitions.h"
#include "application.h"

class SMOOTHAPI SMOOTHDialog : public SMOOTHApplication
{
	protected:
		SMOOTHString		 caption;
		SMOOTHWindow		*parentWindow;
	public:
					 SMOOTHDialog();
					~SMOOTHDialog();

		virtual SMOOTHInt	 ShowDialog() = 0;

		SMOOTHInt		 SetCaption(SMOOTHString);
		SMOOTHInt		 SetParentWindow(SMOOTHWindow *);
};

#endif
