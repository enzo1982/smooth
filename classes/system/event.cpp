 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>

#ifdef __WIN32__
#include <smooth/system/win32/eventwin32.h>
#else
#include <smooth/system/eventbackend.h>
#endif

S::System::EventProcessor::EventProcessor()
{
#ifdef __WIN32__
	backend = new EventWin32();
#else
	backend = new EventBackend();
#endif
}

S::System::EventProcessor::~EventProcessor()
{
	delete backend;
}

S::Int S::System::EventProcessor::ProcessNextEvent(Bool block)
{
	return backend->ProcessNextEvent(block);
}
