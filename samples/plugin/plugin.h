 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_ACTIVEAREAPLUGIN_
#define _H_ACTIVEAREAPLUGIN_

class ActiveAreaPlugin;

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class SMOOTH_PLUGIN_API ActiveAreaPlugin : public Widget
{
	private:
		Int			 areaColor;
	public:
		static const Int	 classID;

					 ActiveAreaPlugin(Int, Point, Size);
					~ActiveAreaPlugin();

		Int			 Paint(Int);
		Int			 Process(Int, Int, Int);
};

#endif
