 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/args.h>

S::ArgumentsParser::ArgumentsParser(const String &commandLine)
{
	String	 param;
	Bool	 quoted = False;

	for (Int i = 0, j = 0; i < commandLine.Length(); i++)
	{
		if ((commandLine[i] == ' ' && !quoted && commandLine[i - 1] != '\\') || i == commandLine.Length() - 1)
		{
#ifdef __APPLE__
			if (!param.StartsWith("-psn_"))
#endif

			if (param.Length() > 0) args.Add(param);

			param	= NIL;
			j	= 0;
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

const S::Array<S::String> &S::ArgumentsParser::GetArguments() const
{
	return args;
}
