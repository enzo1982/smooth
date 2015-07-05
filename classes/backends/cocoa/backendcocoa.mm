 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/cocoa/backendcocoa.h>

#include <smooth/gui/window/window.h>

S::Backends::Backend *CreateBackendCocoa()
{
	return new S::Backends::BackendCocoa();
}

S::Int	 backendCocoaTmp = S::Backends::Backend::AddBackend(&CreateBackendCocoa);

#if defined MAC_OS_X_VERSION_10_6 && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface CocoaApplicationDelegate : NSObject <NSApplicationDelegate> { }
#else
@interface CocoaApplicationDelegate : NSObject { }
#endif
	- (NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication *) sender;
@end

@implementation CocoaApplicationDelegate
	- (NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication *) sender
	{
		/* Ask open windows to close.
		 */
		for (S::Int n = S::GUI::Window::GetNOfWindows() - 1; n >= 0; n--)
		{
			S::GUI::Window	*window	= S::GUI::Window::GetNthWindow(n);

			if (window->IsVisible()) window->Close();
		}

		/* Terminate application if all windows did close.
		 */
		if (S::GUI::Window::nOfActiveWindows == 0) return NSTerminateNow;
		else					   return NSTerminateCancel;
	}
@end

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

	/* Set application delegate.
	 */
	[NSApp setDelegate: [[[CocoaApplicationDelegate alloc] init] autorelease]];

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

S::Bool S::Backends::BackendCocoa::IsOSXVersionAtLeast(Int majorVersion, Int minorVersion, Int microVersion)
{
	SInt32	 osMajorVersion = 0;
	SInt32	 osMinorVersion = 0;
	SInt32	 osMicroVersion = 0;

	Gestalt(gestaltSystemVersionMajor, &osMajorVersion);
	Gestalt(gestaltSystemVersionMinor, &osMinorVersion);
	Gestalt(gestaltSystemVersionBugFix, &osMicroVersion);

	if ( osMajorVersion >  majorVersion ||
	    (osMajorVersion == majorVersion && ( osMinorVersion >  minorVersion ||
						(osMinorVersion == minorVersion && osMicroVersion >= microVersion)))) return True;

	return False;
}
