 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/console.h>

#ifdef __WIN32__
#	include <conio.h>
#else
#	include <stdio.h>
#endif

S::System::Console::Console()
{
}

S::System::Console::Console(const Console &)
{
}

S::Int S::System::Console::SetTitle(const String &title)
{
#ifdef __WIN32__
	if (Setup::enableUnicode)	SetConsoleTitleW(title);
	else				SetConsoleTitleA(title);
#endif

	return Success();
}

S::Int S::System::Console::OutputString(const String &string)
{
#ifdef __WIN32__
	_cprintf(string);
#else
	printf(string);
#endif

	return Success();
}

S::Int S::System::Console::OutputLine(const String &string)
{
#ifdef __WIN32__
	_cprintf(string);
	_cprintf("\n");
#else
	printf(string);
	printf("\n");
#endif

	return Success();
}

S::Void S::System::Console::WaitForKey()
{
#ifdef __WIN32__
	_getch();
#else
	getchar();
#endif
}
