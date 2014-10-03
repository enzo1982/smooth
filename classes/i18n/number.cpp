 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/number.h>

#ifdef __WIN32__
#	include <windows.h>
#endif

S::String S::I18n::Number::GetLocalizedNumberString(Int64 number)
{
	/* Localize a number according to the current locale settings
	 */
	Int	 negFormat = 1;
	String	 sThousand = ".";
	String	 sGrouping = "3;0";

#ifdef __WIN32__
	wchar_t	 buffer[10];

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, buffer, 2);
	negFormat = String(buffer).ToInt();

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer, 4);
	sThousand = buffer;

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SGROUPING, buffer, 10);
	sGrouping = buffer;
#endif

	String	 nString = String::FromInt(number >= 0 ? number : -number);
	String	 retVal;

	/* Separate number blocks.
	 */
	const Array<String>	&groupSize = sGrouping.Explode(";");
	Int			 group	   = 0;
	Int			 count	   = 0;

	for (Int i = nString.Length() - 1; i >= 0; i--)
	{
		if (groupSize.Length() > group && groupSize.GetNth(group).ToInt() == count++)
		{
			retVal = String(sThousand).Append(retVal);

			count = 1;
			group++;

			if (groupSize.Length() > group && groupSize.GetNth(group).ToInt() == 0) group--;
		}

		retVal = String().FillN(nString[i], 1).Append(retVal);
	}

	String::ExplodeFinish();

	/* Format negative numbers according to the locale.
	 */
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
