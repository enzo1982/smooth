 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>
#include <smooth/system/backends/eventbackend.h>

S::System::EventProcessor::EventProcessor()
{
	backend = EventBackend::CreateBackendInstance();
}

S::System::EventProcessor::~EventProcessor()
{
	delete backend;
}

S::Int S::System::EventProcessor::ProcessNextEvent(Bool block)
{
	return backend->ProcessNextEvent(block);
}
