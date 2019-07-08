 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/args.h>

S::ArgumentsParser::ArgumentsParser(int argc, char **argv)
{
	command = argv[0];

	for (Int i = 1; i < argc; i++)
	{
#ifdef __APPLE__
		if (strncmp(argv[i], "-psn_", 5) == 0) continue;
#endif

		args.Add(argv[i]);
	}
}

S::ArgumentsParser::ArgumentsParser(const String &commandLine)
{
	String	 param;
	Bool	 quoted	= False;
	Bool	 first	= True;

	for (Int i = 0, j = 0; i <= commandLine.Length(); i++)
	{
		if ((commandLine[i] == ' ' && !quoted && commandLine[i - 1] != '\\') || i == commandLine.Length())
		{
			if (!param.Length()) continue;

			if (first) command = param;
			else	   args.Add(param);

			first = False;
			param = NIL;
			j     = 0;
		}
		else if (commandLine[i] == '\"')
		{
			quoted = !quoted;
		}
		else if (!(commandLine[i] == '\\' && commandLine[i + 1] == ' '))
		{
			param[j++] = commandLine[i];
		}
	}
}

S::ArgumentsParser::~ArgumentsParser()
{
}
