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

#include <smooth/gui/dialogs/filedlg.h>
#include <smooth/gui/window/window.h>
#include <smooth/backends/cocoa/backendcocoa.h>
#include <smooth/foreach.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

@interface CocoaFilePanel : NSObject
{
	@private
		Short			 mode;

		NSInteger		 response;

		NSURL			*url;
		NSArray			*urls;

		Window			*parentWindow;

		BOOL			 multiSelect;

		const Array<String>	*filters;

		String			 defaultPath;
		String			 defaultFile;
}

	/* CocoaFilePanel methods.
	 */
	+ (CocoaFilePanel *)	panelWithMode: (Short) mode;

	- (id)			init;
	- (void)		dealloc;

	- (void)		setParentWindow: (Window *) window;

	- (void)		setMultiSelect: (BOOL) val;

	- (void)		setFilters: (const Array<String> *) arr;

	- (void)		setDefaultPath: (String) path;
	- (void)		setDefaultFile: (String) file;

	- (void)		runModal;
	- (void)		didEndModalSheet: (NSSavePanel *) sheet returnCode: (int) returnCode contextInfo: (void *) contextInfo;

	- (NSInteger)		response;

	- (NSURL *)		URL;
	- (NSArray *)		URLs;
@end

@implementation CocoaFilePanel
	+ (CocoaFilePanel *) panelWithMode: (Short) mode
	{
		CocoaFilePanel *panel = [[[CocoaFilePanel alloc] init] autorelease];

		panel->mode = mode;

		return panel;
	}

	- (id) init
	{
		[super init];

		mode	     = SFM_OPEN;

		response     = 0;

		url	     = nil;
		urls	     = nil;

		parentWindow = NIL;

		multiSelect  = false;

		return self;
	}

	- (void) dealloc
	{
		if (url	 != NIL) [url release];
		if (urls != NIL) [urls release];

		[super dealloc];
	}

	- (void)	setParentWindow: (Window *) window	{ parentWindow = window; }

	- (void)	setMultiSelect: (BOOL) val		{ multiSelect = val; }

	- (void)	setFilters: (const Array<String> *) arr	{ filters = arr; }

	- (void)	setDefaultPath: (String) path		{ defaultPath = path; }
	- (void)	setDefaultFile: (String) file		{ defaultFile = file; }

	- (void) runModal
	{
		/* Create and configure panel.
		 */
		NSSavePanel	*panel = nil;

		if	(mode == SFM_OPEN) panel = [NSOpenPanel openPanel];
		else if (mode == SFM_SAVE) panel = [NSSavePanel savePanel];

		[panel setFloatingPanel: YES];

		if (mode == SFM_OPEN) [(NSOpenPanel *) panel setAllowsMultipleSelection: multiSelect];

		/* Add file filters.
		 */
		NSMutableArray	*fileTypes = [NSMutableArray arrayWithCapacity: filters->Length()];

		foreach (const String &filter, *filters)
		{
			const Array<String>	&patterns = filter.Explode(";");

			foreach (String pattern, patterns)
			{
				if (pattern.FindLast(".") >= 0) pattern = pattern.Tail(pattern.Length() - pattern.FindLast(".") - 1);

				if (pattern.Trim() != NIL) [fileTypes addObject: [NSString stringWithUTF8String: pattern.Trim()]];
			}
		}

		[panel setAllowedFileTypes: fileTypes];

		/* Run the panel.
		 */
		if (Backends::BackendCocoa::IsOSXVersionAtLeast(10, 15, 0))
		{
			[panel beginSheetForDirectory: defaultPath != NIL ? [NSString stringWithUTF8String: defaultPath.ConvertTo("UTF-8")] : nil
						 file: defaultFile != NIL ? [NSString stringWithUTF8String: defaultFile.ConvertTo("UTF-8")] : nil
				       modalForWindow: parentWindow != NIL ? (NSWindow *) parentWindow->GetSystemWindow() : [NSApp mainWindow]
					modalDelegate: self
				       didEndSelector: @selector(didEndModalSheet:returnCode:contextInfo:)
					  contextInfo: nil];

			[panel runModal];
		}
		else
		{
			response = [panel runModalForDirectory: defaultPath != NIL ? [NSString stringWithUTF8String: defaultPath.ConvertTo("UTF-8")] : nil
							  file: defaultFile != NIL ? [NSString stringWithUTF8String: defaultFile.ConvertTo("UTF-8")] : nil];
		}

		/* Get selected URLs.
		 */
		url  = [[panel URL] retain];

		if (mode == SFM_OPEN) urls = [[(NSOpenPanel *) panel URLs] retain];
	}

	- (void) didEndModalSheet: (NSSavePanel *) sheet returnCode: (int) returnCode contextInfo: (void *) contextInfo
	{
		response = returnCode;
	}

	- (NSInteger)	response	{ return response; }

	- (NSURL *)	URL		{ return url; }
	- (NSArray *)	URLs		{ return urls; }
@end

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

	/* Create file chooser dialog.
	 */
	CocoaFilePanel	*panel = [CocoaFilePanel panelWithMode: mode];

	[panel setParentWindow: parentWindow];

	[panel setFilters: &filters];

	if (flags & SFD_ALLOWMULTISELECT) [panel setMultiSelect: true];

	[panel setDefaultPath: defPath];
	[panel setDefaultFile: defFile];

	if ([NSThread isMainThread]) [panel runModal];
	else			     [panel performSelectorOnMainThread: @selector(runModal) withObject: nil waitUntilDone: YES];

	if ([panel response] == NSFileHandlingPanelOKButton)
	{
		if (mode == SFM_OPEN)
		{
			for (NSURL *url in [panel URLs])
			{
				String	 file;

				file.ImportFrom("UTF-8", [[url path] UTF8String]);
				files.Add(file);
			}
		}
		else if (mode == SFM_SAVE)
		{
			String	 file;

			file.ImportFrom("UTF-8", [[[panel URL] path] UTF8String]);
			files.Add(file);
		}
	}

	[pool release];

	if (files.Length() == 0) error = Error();

	return error;
}
