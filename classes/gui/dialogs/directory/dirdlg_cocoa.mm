 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#import <Cocoa/Cocoa.h>

#include <smooth/gui/dialogs/dirdlg.h>
#include <smooth/files/directory.h>

@interface CocoaFolderPanel : NSObject
{
	@private
		NSInteger	 response;

		NSURL		*url;
}

	/* CocoaFolderPanel methods.
	 */
	+ (CocoaFolderPanel *)	panel;

	- (id)			init;
	- (void)		dealloc;

	- (void)		runModal;
	
	- (NSInteger)		response;

	- (NSURL *)		URL;
@end

@implementation CocoaFolderPanel
	+ (CocoaFolderPanel *) panel
	{
		CocoaFolderPanel *panel = [[[CocoaFolderPanel alloc] init] autorelease];

		return panel;
	}

	- (id) init
	{
		[super init];

		response    = 0;

		url	    = nil;

		return self;
	}

	- (void) dealloc
	{
		if (url	 != NIL) [url release];

		[super dealloc];
	}

	- (void) runModal
	{
		/* Create and configure panel.
		 */
		NSOpenPanel	*panel = [NSOpenPanel openPanel];

		[panel setFloatingPanel: YES];

		[panel setCanChooseDirectories: true];
		[panel setCanChooseFiles: false];

		/* Run the panel.
		 */
		response = [panel runModal];

		/* Get selected URL.
		 */
		url  = [[panel URL] retain];
	}

	- (NSInteger)	response	{ return response; }

	- (NSURL *)	URL		{ return url; }
@end

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

	/* Create file chooser dialog
	 */
	CocoaFolderPanel	*panel = [CocoaFolderPanel panel];

	if ([NSThread isMainThread]) [panel runModal];
	else			     [panel performSelectorOnMainThread: @selector(runModal) withObject: nil waitUntilDone: YES];

	if ([panel response] == NSOKButton) directory.ImportFrom("UTF-8", [[[panel URL] path] UTF8String]);

	[pool release];

	/* Check if we actually have a directory
	 */
	if (directory != NIL)
	{
		if (!directory.EndsWith(Directory::GetDirectoryDelimiter())) directory.Append(Directory::GetDirectoryDelimiter());
	}
	else
	{
		error = Error();
	}

	return error;
}
