 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#import <Cocoa/Cocoa.h>

#include <smooth/gui/dialogs/dirdlg.h>
#include <smooth/gui/window/window.h>
#include <smooth/backends/cocoa/backendcocoa.h>
#include <smooth/files/directory.h>

using namespace smooth;
using namespace smooth::GUI;

@interface CocoaFolderPanel : NSObject
{
	@private
		NSInteger	 response;

		NSURL		*url;

		Window		*parentWindow;
}

	/* CocoaFolderPanel methods.
	 */
	+ (CocoaFolderPanel *)	panel;

	- (id)			init;
	- (void)		dealloc;

	- (void)		setParentWindow: (Window *) window;

	- (void)		runModal;
	- (void)		didEndModalSheet: (NSSavePanel *) sheet returnCode: (int) returnCode contextInfo: (void *) contextInfo;

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

		response     = 0;

		url	     = nil;

		parentWindow = NIL;

		return self;
	}

	- (void) dealloc
	{
		if (url	 != NIL) [url release];

		[super dealloc];
	}

	- (void) setParentWindow: (Window *) window { parentWindow = window; }

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
		if (Backends::BackendCocoa::IsOSXVersionAtLeast(10, 15, 0))
		{
			[panel beginSheetForDirectory: nil
						 file: nil
				       modalForWindow: parentWindow != NIL ? (NSWindow *) parentWindow->GetSystemWindow() : [NSApp mainWindow]
					modalDelegate: self
				       didEndSelector: @selector(didEndModalSheet:returnCode:contextInfo:)
					  contextInfo: nil];

			[panel runModal];
		}
		else
		{
			response = [panel runModal];
		}

		/* Get selected URL.
		 */
		url  = [[panel URL] retain];
	}

	- (void) didEndModalSheet: (NSSavePanel *) sheet returnCode: (int) returnCode contextInfo: (void *) contextInfo
	{
		response = returnCode;
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

	[panel setParentWindow: parentWindow];

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
