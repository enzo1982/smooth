 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CLIENT_
#define _H_OBJSMOOTH_CLIENT_

#define SClient SMOOTHClient

class SMOOTHClient;

#include "object.h"

class SMOOTHAPI SMOOTHClient : public SMOOTHObject
{
	protected:
		SMOOTHRect		 updateRect;
	public:
					 SMOOTHClient(SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHClient();

		virtual SMOOTHInt	 Paint(SMOOTHInt);

		SMOOTHRect		 GetUpdateRect();

		virtual SMOOTHSize	 GetSize();

		virtual SMOOTHInt	 BlitFromBitmap(HBITMAP, SMOOTHRect, SMOOTHRect);
		virtual SMOOTHInt	 BlitToBitmap(SMOOTHRect, HBITMAP, SMOOTHRect);
};

SMOOTHVAR SMOOTHInt OBJ_CLIENT;

#endif
