 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/application.h>
#include <smooth/i18n.h>
#include <smooth/objectproperties.h>

const S::Int	 S::Application::classID = S::Object::RequestClassID();

S::String	 S::Application::startupDirectory;
S::String	 S::Application::applicationDirectory;

S::Application::Application(String name)
{
	self		= this;

	type		= classID;
	containerType	= classID;

	objectProperties->text		= name == NIL ? String("smooth Application") : name;
	objectProperties->size.cx	= LiSAGetDisplaySizeX();
	objectProperties->size.cy	= LiSAGetDisplaySizeY();
}

S::String S::Application::GetStartupDirectory()
{
	if (startupDirectory != NIL) return startupDirectory;

	if (Setup::enableUnicode)
	{
		wchar_t	*buffer = new wchar_t [MAX_PATH];

		GetCurrentDirectoryW(MAX_PATH, buffer);

		startupDirectory = buffer;

		delete [] buffer;
	}
	else
	{
		char	*buffer = new char [MAX_PATH];

		GetCurrentDirectoryA(MAX_PATH, buffer);

		startupDirectory = buffer;

		delete [] buffer;
	}

	Int	 len = startupDirectory.Length() - 1;

	if (startupDirectory[len] != '\\') startupDirectory[++len] = '\\';

	return startupDirectory;
}

S::String S::Application::GetApplicationDirectory()
{
	if (applicationDirectory != NIL) return applicationDirectory;

	String	 path;
	Int	 lastBs = 0;

	if (Setup::enableUnicode)
	{
		wchar_t	*buffer = new wchar_t [MAX_PATH];

		GetModuleFileNameW(NIL, buffer, MAX_PATH);

		path = buffer;

		delete [] buffer;
	}
	else
	{
		char	*buffer = new char [MAX_PATH];

		GetModuleFileNameA(NIL, buffer, MAX_PATH);

		path = buffer;

		delete [] buffer;
	}

	for (Int i = 0; i < path.Length(); i++) if (path[i] == '\\') lastBs = i;

	path[lastBs + 1] = 0;

	applicationDirectory = path;

	return applicationDirectory;
}
