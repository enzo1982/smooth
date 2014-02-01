 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/encoding/urlencode.h>
#include <smooth/misc/number.h>

S::String S::Encoding::URLEncode::Encode(const String &string, Bool encodeReserved)
{
	/* Convert input string to UTF-8.
	 */
	String	 utf8;

	utf8.ImportFrom("ISO-8859-1", string.ConvertTo("UTF-8"));

	/* Encode UTF-8 string using URL encode.
	 */
	String	 encoded;

	for (Int i = 0, e = 0; i < utf8.Length(); i++)
	{
		if ((utf8[i] >= 'A' && utf8[i] <= 'Z') || (utf8[i] >= 'a' && utf8[i] <= 'z') || (utf8[i] >= '0' && utf8[i] <= '9') ||
		    (utf8[i] == '-' || utf8[i] == '_' || utf8[i] == '.' || utf8[i] == '~') ||
		    (!encodeReserved && (utf8[i] == '!' || utf8[i] == '*' || utf8[i] == '\'' || utf8[i] == '(' || utf8[i] == ')' || utf8[i] == ';' ||
					 utf8[i] == ':' || utf8[i] == '@' || utf8[i] == '&'  || utf8[i] == '=' || utf8[i] == '+' || utf8[i] == '$' ||
					 utf8[i] == ',' || utf8[i] == '/' || utf8[i] == '?'  || utf8[i] == '#' || utf8[i] == '[' || utf8[i] == ']')))
		{
			encoded[e++] = utf8[i];
		}
		else
		{
			String	 hex = Number((Int64) utf8[i]).ToHexString(2).ToUpper();

			encoded[e++] = '%';
			encoded[e++] = hex[0];
			encoded[e++] = hex[1];
		}
	}

	return encoded;
}

S::String S::Encoding::URLEncode::Decode(const String &encoded)
{
	/* Decode URL encoded string to UTF-8.
	 */
	String	 utf8;

	for (Int i = 0, d = 0; i < encoded.Length(); i++)
	{
		if (encoded[i] == '%')
		{
			utf8[d++] = (Int64) Number::FromHexString(encoded.SubString(i + 1, 2));

			i += 2;
		}
		else
		{
			utf8[d++] = encoded[i];
		}
	}

	/* Import string from UTF-8.
	 */
	String	 string;

	string.ImportFrom("UTF-8", utf8.ConvertTo("ISO-8859-1"));

	return string;
}
