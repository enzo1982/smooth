 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/filedlg.h>

S::GUI::Dialogs::FileSelection::FileSelection()
{
	flags = 0;
	mode  = 0;
}

S::GUI::Dialogs::FileSelection::~FileSelection()
{
}

S::Int S::GUI::Dialogs::FileSelection::AddFilter(const String &name, const String &filter)
{
	filterNames.Add(name);
	filters.Add(filter);

	return Success();
}

S::Int S::GUI::Dialogs::FileSelection::SetFlags(Short newFlags)
{
	flags |= newFlags;

	return Success();
}

S::Int S::GUI::Dialogs::FileSelection::SetMode(Short newMode)
{
	mode = newMode;

	return Success();
}

S::Int S::GUI::Dialogs::FileSelection::SetInitialPath(const String &newDefPath)
{
	defPath = newDefPath;

	return Success();
}

S::Int S::GUI::Dialogs::FileSelection::SetDefaultExtension(const String &newDefExt)
{
	defExt = newDefExt;

	return Success();
}

S::Int S::GUI::Dialogs::FileSelection::GetNumberOfFiles() const
{
	return files.Length();
}

S::Int S::GUI::Dialogs::FileSelection::SetFileName(const String &newDefFile)
{
	defFile = newDefFile;

	return Success();
}

const S::String &S::GUI::Dialogs::FileSelection::GetFileName() const
{
	static const String	 nil;

	if (files.Length() > 0)	return files.GetFirst();
	else			return nil;
}

const S::String &S::GUI::Dialogs::FileSelection::GetNthFileName(Int n) const
{
	static const String	 nil;

	if (files.Length() > n)	return files.GetNth(n);
	else			return nil;
}
