 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/number.h>

S::String S::I18n::Number::GetLocalizedNumberString(Int64 number)
{
	// Localize a number according to the current locale settings

	Int	 negFormat;
	String	 sThousand;

	if (Setup::enableUnicode)
	{
		static wchar_t	 buffer[4];

		GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer, 4);
		sThousand = buffer;

		GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, buffer, 2);
		negFormat = String(buffer).ToInt();
	}
	else
	{
		static char	 buffer[4];

		GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer, 4);
		sThousand = buffer;

		GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, buffer, 2);
		negFormat = String(buffer).ToInt();
	}

	String	 nString = String::FromInt(number >= 0 ? number : -number);
	String	 retVal;

	// Separate three number blocks

	for (Int i = 0; i < nString.Length(); i++)
	{
		if ((nString.Length() - i) % 3 == 0 && i > 0) retVal.Append(sThousand);

		retVal[retVal.Length()] = nString[i];
	}

	// Format negative numbers according to the locale

	if (number < 0)
	{
		switch (negFormat)
		{
			case 0:		// (1.1)
				retVal = String("(").Append(retVal).Append(")");
				break;
			case 1:		// -1.1
				retVal = String("-").Append(retVal);
				break;
			case 2:		// - 1.1
				retVal = String("- ").Append(retVal);
				break;
			case 3:		// 1.1-
				retVal.Append("-");
				break;
			case 4:		// 1.1 -
				retVal.Append(" -");
				break;
		}
	}

	return retVal;
}
