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

#include <smooth/gui/dialogs/file/filedlg_cocoa.h>
#include <smooth/files/file.h>
#include <smooth/misc/number.h>
#include <smooth/foreach.h>

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

	/* Create file chooser dialog.
	 */
	if (mode == SFM_OPEN)
	{
		NSOpenPanel	*openPanel = [NSOpenPanel openPanel];

		[openPanel setFloatingPanel: YES];

		if (flags & SFD_ALLOWMULTISELECT) [openPanel setAllowsMultipleSelection: true];

		SetFilters(openPanel);

		if ([openPanel runModal] == NSOKButton)
		{
			NSArray *URLs = [openPanel URLs];

			for (unsigned int i = 0; i < [URLs count]; i++)
			{
				String	 file;

				file.ImportFrom("UTF-8", [[[URLs objectAtIndex: i] path] UTF8String]);
				files.Add(file);
			}
		}
	}
	else if (mode == SFM_SAVE)
	{
		NSSavePanel	*savePanel = [NSSavePanel savePanel];

		[savePanel setFloatingPanel: YES];

		SetFilters(savePanel);

		if ([savePanel runModal] == NSOKButton)
		{
			String	 file;

			file.ImportFrom("UTF-8", [[[savePanel URL] path] UTF8String]);
			files.Add(file);
		}
	}

	[pool release];

	if (files.Length() == 0) error = Error();

	return error;
}

S::Bool S::GUI::Dialogs::FileSelection::SetFilters(void *iSavePanel)
{
	NSSavePanel *savePanel = (NSSavePanel *) iSavePanel;

	/* Add file filters.
	 */
	NSMutableArray	*fileTypes = [NSMutableArray arrayWithCapacity: filters.Length()];

	for (int i = 0; i < filters.Length(); i++)
	{
		const Array<String>	&patterns = filters.GetNth(i).Explode(";");

		foreach (const String &pattern, patterns)
		{
			if (pattern.FindLast(".") >= 0) pattern = pattern.Tail(pattern.Length() - pattern.FindLast(".") - 1);

			if (pattern.Trim() != NIL) [fileTypes addObject: [NSString stringWithUTF8String: pattern.Trim()]];
		}

		String::ExplodeFinish();
	}

	[savePanel setAllowedFileTypes: fileTypes];

	return True;
}
