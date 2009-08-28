 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>

using namespace smooth;
using namespace smooth::IO;

Void generateConversion(Int field, OutStream &out)
{
	InStream	 in(STREAM_FILE, "UnicodeData.txt", IS_READONLY);

	Int	 seriesCount	=  0;
	Int	 seriesLength	=  0;
	Int	 seriesType	= -1;
	Int64	 seriesStart	=  0;
	Int64	 difference	=  0;

	while (True)
	{
		const Array<String>	&line = in.InputLine().Explode(";");

		/* Exit if no more data is found.
		 */
		if (line.Length() == 0) break;

		Int	 hexVal = (Int64) Number::FromHexString(line.GetFirst());
		Int	 hexLower = (Int64) Number::FromHexString(line.GetNth(field));

		/* Continue if we are in the middle of a special letters series.
		 */
		if (Math::Abs(difference) == 1 && hexVal < (seriesStart + (seriesLength * 2)) && line.GetNth(field) == NIL) continue;

		if (seriesLength > 0 && (line.GetNth(field) == NIL || hexLower - hexVal != difference))
		{
			if (seriesCount != 0) out.OutputString("\r\n\t\telse ");
			else		      out.OutputString("\r\n\t\t");

			if (seriesLength == 1)
			{
				out.OutputString(String("if\t((*this)[i] == 0x").Append(Number(seriesStart).ToHexString()).Append(")"));
				out.OutputString("\t\t\t\t\t\t\t");
				out.OutputString(String("retVal[i] =\t\t 0x").Append(Number(seriesStart + difference).ToHexString()).Append(";\t// Single character"));
			}
			else if (seriesType == 0)
			{
				out.OutputString(String("if\t((*this)[i] >= 0x").Append(Number(seriesStart).ToHexString()).Append(" && (*this)[i] <= 0x").Append(Number(seriesStart + seriesLength - 1).ToHexString()).Append(")"));
				out.OutputString("\t\t\t\t");
				out.OutputString(String("retVal[i] = (*this)[i] ").Append(difference >= 0 ? "+" : "-").Append(" 0x"));
				out.OutputString(String(Number(Math::Abs(difference)).ToHexString()).Append(";\t// Alphabet"));
			}
			else if (seriesType == 1)
			{
				out.OutputString(String("if\t((*this)[i] >= 0x").Append(Number(seriesStart).ToHexString()).Append(" && (*this)[i] <= 0x").Append(Number(seriesStart + 2 * (seriesLength - 1)).ToHexString()).Append(" && ").Append(seriesStart & 1 ? " " : "!").Append("((*this)[i] & 1)").Append(")"));
				out.OutputString("\t");
				out.OutputString(String("retVal[i] = (*this)[i] ").Append(difference >= 0 ? "+" : "-").Append(" 0x"));
				out.OutputString(String(Number(Math::Abs(difference)).ToHexString()).Append(";\t// Special letters"));
			}

			seriesCount++;
			seriesLength	=  0;
			seriesType	= -1;
			seriesStart	=  0;
			difference	=  0;
		}

		/* smooth strings support only 16 bit values at the moment.
		 */
		if (hexVal >= 65536) continue;

		if (line.GetNth(field) != NIL)
		{
			if (seriesLength == 0)
			{
				difference = hexLower - hexVal;
				seriesStart = hexVal;

				if (Math::Abs(difference) == 1) seriesType = 1;
				else				seriesType = 0;
			}

			seriesLength++;
		}
	}

	in.Close();
}

Int smooth::Main()
{
	OutStream	 out(STREAM_FILE, "string_case.cpp", OS_OVERWRITE);

	out.OutputString(" /* The smooth Class Library						\
		\r\n  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>		\
		\r\n  *										\
		\r\n  * This library is free software; you can redistribute it and/or		\
		\r\n  * modify it under the terms of \"The Artistic License, Version 2.0\".	\
		\r\n  *										\
		\r\n  * THIS PACKAGE IS PROVIDED \"AS IS\" AND WITHOUT ANY EXPRESS OR		\
		\r\n  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED		\
		\r\n  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */	\
		\r\n										\
		\r\n#include <smooth/misc/string.h>						\
		\r\n										\
		\r\nS::String S::String::ToUpper() const					\
		\r\n{										\
		\r\n	String	 retVal = *this;						\
		\r\n										\
		\r\n	mutex.Lock();								\
		\r\n										\
		\r\n	for (Int i = 0; i < Length(); i++)					\
		\r\n	{");

	generateConversion(12, out);	// To upper case

	out.OutputString("					\
								\
		\r\n	}					\
		\r\n						\
		\r\n	mutex.Release();			\
		\r\n						\
		\r\n	return retVal;				\
		\r\n}						\
		\r\n");

	out.OutputString("					\
								\
		\r\nS::String S::String::ToLower() const	\
		\r\n{						\
		\r\n	String	 retVal = *this;		\
		\r\n						\
		\r\n	mutex.Lock();				\
		\r\n						\
		\r\n	for (Int i = 0; i < Length(); i++)	\
		\r\n	{");

	generateConversion(13, out);	// To lower case

	out.OutputString("					\
								\
		\r\n	}					\
		\r\n						\
		\r\n	mutex.Release();			\
		\r\n						\
		\r\n	return retVal;				\
		\r\n}						\
		\r\n");

	out.OutputString("					\
								\
		\r\nS::String S::String::ToTitle() const	\
		\r\n{						\
		\r\n	String	 retVal = *this;		\
		\r\n						\
		\r\n	mutex.Lock();				\
		\r\n						\
		\r\n	for (Int i = 0; i < Length(); i++)	\
		\r\n	{");

	generateConversion(14, out);	// To title case

	out.OutputString("					\
								\
		\r\n	}					\
		\r\n						\
		\r\n	mutex.Release();			\
		\r\n						\
		\r\n	return retVal;				\
		\r\n}						\
		\r\n");

	out.Close();

	return 0;
}
