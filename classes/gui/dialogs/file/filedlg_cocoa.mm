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

#include <smooth/gui/dialogs/filedlg.h>
#include <smooth/files/file.h>
#include <smooth/misc/number.h>
#include <smooth/foreach.h>

namespace smooth
{
	static Bool SetFileSelectionFilters(NSSavePanel *savePanel, const Array<String> &filters)
	{
		/* Add file filters.
		 */
		NSMutableArray	*fileTypes = [NSMutableArray arrayWithCapacity: filters.Length()];

		foreach (const String &filter, filters)
		{
			const Array<String>	&patterns = filter.Explode(";");

			foreach (String pattern, patterns)
			{
				if (pattern.FindLast(".") >= 0) pattern = pattern.Tail(pattern.Length() - pattern.FindLast(".") - 1);

				if (pattern.Trim() != NIL) [fileTypes addObject: [NSString stringWithUTF8String: pattern.Trim()]];
			}

			String::ExplodeFinish();
		}

		[savePanel setAllowedFileTypes: fileTypes];

		return True;
	}
};

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

		SetFileSelectionFilters(openPanel, filters);

		if ([openPanel runModalForDirectory: defPath != NIL ? [NSString stringWithUTF8String: defPath.ConvertTo("UTF-8")] : nil
					       file: defFile != NIL ? [NSString stringWithUTF8String: defFile.ConvertTo("UTF-8")] : nil] == NSFileHandlingPanelOKButton)
		{
			for (NSURL *url in [openPanel URLs])
			{
				String	 file;

				file.ImportFrom("UTF-8", [[url path] UTF8String]);
				files.Add(file);
			}
		}
	}
	else if (mode == SFM_SAVE)
	{
		NSSavePanel	*savePanel = [NSSavePanel savePanel];

		[savePanel setFloatingPanel: YES];

		SetFileSelectionFilters(savePanel, filters);

		if ([savePanel runModalForDirectory: defPath != NIL ? [NSString stringWithUTF8String: defPath.ConvertTo("UTF-8")] : nil
					       file: defFile != NIL ? [NSString stringWithUTF8String: defFile.ConvertTo("UTF-8")] : nil] == NSFileHandlingPanelOKButton)
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
