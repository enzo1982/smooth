 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_HYPERLINK_
#define _H_OBJSMOOTH_HYPERLINK_

#define SHyperlink SMOOTHHyperlink

class SMOOTHHyperlink;

#include "object.h"

class SMOOTHAPI SMOOTHHyperlink : public SMOOTHObject
{
	protected:
		SMOOTHString		 linkURL;
		HBITMAP			 linkBitmap;
	public:
					 SMOOTHHyperlink();
					 SMOOTHHyperlink(SMOOTHString, HBITMAP, SMOOTHString, SMOOTHPoint, SMOOTHSize size = SMOOTHSize(0, 0));
					~SMOOTHHyperlink();

		SMOOTHInt		 Hide();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		HBITMAP			 GetBitmap();
		SMOOTHString		 GetURL();

		SMOOTHInt		 SetText(SMOOTHString);
		SMOOTHInt		 SetBitmap(HBITMAP);
		SMOOTHInt		 SetURL(SMOOTHString);
};

SMOOTHVAR SMOOTHInt OBJ_HYPERLINK;

#endif
