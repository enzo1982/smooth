 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/directory/dirdlg.h>
#include <smooth/i18n/translator.h>

S::GUI::Dialogs::DirSelectionBase::DirSelectionBase()
{
	caption		= I18n::Translator::defaultTranslator->TranslateString("Select directory");
	directory	= NIL;
}

S::GUI::Dialogs::DirSelectionBase::~DirSelectionBase()
{
}

S::Int S::GUI::Dialogs::DirSelectionBase::SetDirName(const String &nDirectory)
{
	directory = nDirectory;

	return Success();
}

const S::String &S::GUI::Dialogs::DirSelectionBase::GetDirName() const
{
	return directory;
}
