 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_ACTIVEAREAPLUGIN_
#define _H_ACTIVEAREAPLUGIN_

#define SActiveAreaPlugin SMOOTHActiveAreaPlugin

class SMOOTHActiveAreaPlugin;

#include <smoothx.h>

class SMOOTH_PLUGIN_API SMOOTHActiveAreaPlugin : public SMOOTHObject
{
	public:
		SMOOTHInt	 areaColor;
				 SMOOTHActiveAreaPlugin(SMOOTHInt, SMOOTHPoint, SMOOTHSize, SMOOTHProcParam, SMOOTHVoid *);
				~SMOOTHActiveAreaPlugin();
		SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt	 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);
};

SMOOTH_PLUGIN_VAR SMOOTHInt OBJ_ACTIVEAREAPLUGIN;

#endif
