 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_ACTIVEAREAPLUGIN_
#define _H_ACTIVEAREAPLUGIN_

class ActiveAreaPlugin;

#include <smooth.h>

using namespace smooth;

class SMOOTH_PLUGIN_API ActiveAreaPlugin : public Object
{
	public:
		Int	 areaColor;
			 ActiveAreaPlugin(Int, Point, Size, ProcParam, Void *);
			~ActiveAreaPlugin();
		Int	 Paint(Int);
		Int	 Process(Int, Int, Int);
};

SMOOTH_PLUGIN_VAR Int OBJ_ACTIVEAREAPLUGIN;

#endif
