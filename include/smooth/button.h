 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BUTTON_
#define _H_OBJSMOOTH_BUTTON_

#define SButton SMOOTHButton

class SMOOTHButton;
class SMOOTHTooltip;
class SMOOTHTimer;

#include "object.h"

class SMOOTHAPI SMOOTHButton : public SMOOTHObject
{
	private:
		SMOOTHTooltip		*tooltip;
		SMOOTHTimer		*tipTimer;

		SMOOTHVoid		 ActivateTooltip();
	protected:
		HBITMAP			 bitmap;
		SMOOTHSize		 bmpSize;
	public:
					 SMOOTHButton(SMOOTHString, HBITMAP, SMOOTHPoint, SMOOTHSize, SMOOTHProcParam, void *);
					~SMOOTHButton();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_BUTTON;

#endif
