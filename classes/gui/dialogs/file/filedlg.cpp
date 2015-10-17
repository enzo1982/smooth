 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/file/filedlg.h>

S::GUI::Dialogs::FileSelectionBase::FileSelectionBase()
{
	flags = 0;
	mode  = 0;
}

S::GUI::Dialogs::FileSelectionBase::~FileSelectionBase()
{
}

S::Int S::GUI::Dialogs::FileSelectionBase::AddFilter(const String &name, const String &filter)
{
	filterNames.Add(name);
	filters.Add(filter);

	return Success();
}

S::Int S::GUI::Dialogs::FileSelectionBase::SetFlags(Short newFlags)
{
	flags |= newFlags;

	return Success();
}

S::Int S::GUI::Dialogs::FileSelectionBase::SetMode(Short newMode)
{
	mode = newMode;

	return Success();
}

S::Int S::GUI::Dialogs::FileSelectionBase::SetInitialPath(const String &newDefPath)
{
	defPath = newDefPath;

	return Success();
}

S::Int S::GUI::Dialogs::FileSelectionBase::SetDefaultExtension(const String &newDefExt)
{
	defExt = newDefExt;

	return Success();
}

S::Int S::GUI::Dialogs::FileSelectionBase::GetNumberOfFiles() const
{
	return files.Length();
}

S::Int S::GUI::Dialogs::FileSelectionBase::SetFileName(const String &newDefFile)
{
	defFile = newDefFile;

	return Success();
}

const S::String &S::GUI::Dialogs::FileSelectionBase::GetFileName() const
{
	static const String	 nil;

	if (files.Length() > 0)	return files.GetFirst();
	else			return nil;
}

const S::String &S::GUI::Dialogs::FileSelectionBase::GetNthFileName(Int n) const
{
	static const String	 nil;

	if (files.Length() > n)	return files.GetNth(n);
	else			return nil;
}
