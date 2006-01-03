 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/system.h>
#include <smooth/version.h>

S::Int S::System::System::nextGUID	= 0;

S::System::System::System()
{
}

S::System::System::System(const System &)
{
}

S::Int S::System::System::RequestGUID()
{
	return nextGUID++;
}

S::String S::System::System::GetVersionString()
{
	return SMOOTH_VERSION;
}
