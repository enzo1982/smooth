 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/application/application.h>
#include <smooth/i18n/i18n.h>

const S::Int	 S::GUI::Application::classID = S::Object::RequestClassID();

S::String	 S::GUI::Application::startupDirectory;
S::String	 S::GUI::Application::applicationDirectory;

S::GUI::Application::Application(const String &name) : Widget(Point(0, 0), Size(LiSAGetDisplaySizeX(), LiSAGetDisplaySizeY()))
{
	type	= classID;

	Show();

	text	= name == NIL ? String("smooth Application") : name;
}

S::String S::GUI::Application::GetStartupDirectory()
{
	if (startupDirectory != NIL) return startupDirectory;

#ifdef __WIN32__
	Int	 length = 0;

	if (Setup::enableUnicode)
	{
		wchar_t	*buffer = new wchar_t [MAX_PATH];

		length = GetCurrentDirectoryW(MAX_PATH, buffer);

		startupDirectory = buffer;

		delete [] buffer;
	}

	if (!Setup::enableUnicode || length == 0)
	{
		char	*buffer = new char [MAX_PATH];

		GetCurrentDirectoryA(MAX_PATH, buffer);

		startupDirectory = buffer;

		delete [] buffer;
	}

	Int	 len = startupDirectory.Length() - 1;

	if (startupDirectory[len] != '\\') startupDirectory[++len] = '\\';
#endif

	return startupDirectory;
}

S::String S::GUI::Application::GetApplicationDirectory()
{
	if (applicationDirectory != NIL) return applicationDirectory;

#ifdef __WIN32__
	String	 path;
	Int	 lastBs = 0;
	Int	 length = 0;

	if (Setup::enableUnicode)
	{
		wchar_t	*buffer = new wchar_t [MAX_PATH];

		length = GetModuleFileNameW(NIL, buffer, MAX_PATH);

		path = buffer;

		delete [] buffer;
	}

	if (!Setup::enableUnicode || length == 0)
	{
		char	*buffer = new char [MAX_PATH];

		GetModuleFileNameA(NIL, buffer, MAX_PATH);

		path = buffer;

		delete [] buffer;
	}

	for (Int i = 0; i < path.Length(); i++) if (path[i] == '\\') lastBs = i;

	path[lastBs + 1] = 0;

	applicationDirectory = path;
#endif

	return applicationDirectory;
}
