 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/cocoa/backendcocoa.h>

S::Backends::Backend *CreateBackendCocoa()
{
	return new S::Backends::BackendCocoa();
}

S::Int	 backendCocoaTmp = S::Backends::Backend::AddBackend(&CreateBackendCocoa);

S::Backends::BackendCocoa::BackendCocoa()
{
	type = BACKEND_COCOA;
	
	pool = nil;
}

S::Backends::BackendCocoa::~BackendCocoa()
{
}

S::Int S::Backends::BackendCocoa::Init()
{
	/* Init the Cocoa app.
	 */
	[NSApplication sharedApplication];

	/* Create a garbage collection pool.
	 */
	pool = [[NSAutoreleasePool alloc] init];

	/* Set default language if not set.
	 */
	NSUserDefaults	*defaults  = [NSUserDefaults standardUserDefaults];
	NSArray		*languages = [defaults objectForKey: @"AppleLanguages"];
	NSString	*language  = [languages objectAtIndex: 0];

	setenv("LANG", [language UTF8String], False);
	
	return Success();
}

S::Int S::Backends::BackendCocoa::Deinit()
{
	/* Release our garbage collection pool.
	 */
	[pool release];

	return Success();
}
