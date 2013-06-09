 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#import <Cocoa/Cocoa.h>

#include <smooth/gui/dialogs/directory/dirdlg_cocoa.h>
#include <smooth/files/directory.h>

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

	/* Create file chooser dialog
	 */
	NSOpenPanel	*openPanel = [NSOpenPanel openPanel];

	[openPanel setFloatingPanel: YES];

	[openPanel setCanChooseDirectories: true];
	[openPanel setCanChooseFiles: false];

	if ([openPanel runModal] == NSOKButton) directory.ImportFrom("UTF-8", [[[openPanel URL] path] UTF8String]);

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
