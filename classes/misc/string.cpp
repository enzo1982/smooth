 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/string.h>
#include <smooth/misc/math.h>

#include <iconv.h>
#include <memory.h>
#include <iolib-cxx.h>

namespace smooth
{
	Int	 ConvertString(const char *, Int, const char *, char *, Int, const char *);
};

char	*S::String::inputFormat		= NIL;
char	*S::String::outputFormat	= NIL;

S::Int	 S::String::nOfStrings		= 0;

S::Array<char *>	 S::String::allocatedBuffers;

S::Int	 S::String::crc32_table[256];
S::Bool	 S::String::crc32_initialized = False;

S::String::String()
{
	nOfStrings++;

	wString = NIL;

	Clean();
}

S::String::String(const int nil)
{
	nOfStrings++;

	wString = NIL;

	Clean();
}

S::String::String(const char *iString)
{
	nOfStrings++;

	if (iString == NIL)
	{
		wString = NIL;

		Clean();
	}
	else
	{
		wString = NIL;

		if (inputFormat == NIL) SetInputFormat("ISO-8859-1");

		ImportFrom(inputFormat, iString);
	}
}

S::String::String(const wchar_t *iString)
{
	nOfStrings++;

	if (iString == NIL)
	{
		wString = NIL;

		Clean();
	}
	else
	{
		stringSize = wcslen(iString) + 1;

		wString = new wchar_t [stringSize];

		wcscpy(wString, iString);
	}
}

S::String::String(const String &iString)
{
	nOfStrings++;

	if (iString.wString == NIL)
	{
		wString = NIL;

		Clean();
	}
	else
	{
		stringSize = iString.stringSize;

		wString = new wchar_t [stringSize];

		wcsncpy(wString, iString.wString, stringSize);
	}
}

S::String::~String()
{
	Clean();

	if (--nOfStrings == 0)
	{
		for (Int i = 0; i < allocatedBuffers.GetNOfEntries(); i++) delete [] allocatedBuffers.GetNthEntry(i);

		allocatedBuffers.RemoveAll();
	}
}

S::Void S::String::DeleteTemporaryBuffers()
{
	Int	 nOfEntries = allocatedBuffers.GetNOfEntries();

	for (Int i = 0; i < nOfEntries - 512; i++)
	{
		delete [] allocatedBuffers.GetFirstEntry();

		allocatedBuffers.RemoveEntry(allocatedBuffers.GetNthEntryIndex(0));
	}
}

S::Void S::String::Clean()
{
	if (wString != NIL) delete [] wString;

	wString = NIL;
	stringSize = 0;
}

S::Void S::String::CRC32_InitTable()
{
	UnsignedLong	 ulPolynomial = 0x04c11db7;

	for (Int i = 0; i <= 0xFF; i++)
	{
		crc32_table[i] = CRC32_Reflect(i, 8) << 24;

		for (Int j = 0; j < 8; j++) crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);

		crc32_table[i] = CRC32_Reflect(crc32_table[i], 32);
	}

	crc32_initialized = True;
}

S::UnsignedLong S::String::CRC32_Reflect(UnsignedLong ref, char ch)
{
	UnsignedLong	 value(0);

	for (Int i = 1; i < (ch + 1); i++)
	{
		if (ref & 1) value |= 1 << (ch - i);

		ref >>= 1;
	}

	return value;
}

S::Int S::String::ComputeCRC32()
{
	if (!crc32_initialized) CRC32_InitTable();

	UnsignedLong	 ulCRC(0xffffffff);
	Int		 len = Length();
	unsigned char	*buffer = (unsigned char *) (char *) *this;

	while (len--) ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF) ^ *buffer++];

	return ulCRC ^ 0xffffffff;
}

S::Bool S::String::IsANSI(String &string)
{
	return !IsUnicode(string);
}

S::Bool S::String::IsUnicode(String &string)
{
	for (Int i = 0; i < string.Length(); i++)
	{
		if (string[i] > 255) return True;
	}

	return False;
}

char *S::String::SetInputFormat(const char *iFormat)
{
	if (iFormat == NIL) return SetInputFormat("ISO-8859-1");

	char	*previousInputFormat = inputFormat;

	inputFormat = new char [strlen(iFormat) + 1];

	strcpy(inputFormat, iFormat);

	if (previousInputFormat != NIL)
	{
		if (allocatedBuffers.GetNOfEntries() >= 1024) DeleteTemporaryBuffers();

		allocatedBuffers.AddEntry(previousInputFormat);
	}

	return previousInputFormat;
}

char *S::String::SetOutputFormat(const char *oFormat)
{
	if (oFormat == NIL) return SetOutputFormat("ISO-8859-1");

	char	*previousOutputFormat = outputFormat;

	outputFormat = new char [strlen(oFormat) + 1];

	strcpy(outputFormat, oFormat);

	if (previousOutputFormat != NIL)
	{
		if (allocatedBuffers.GetNOfEntries() >= 1024) DeleteTemporaryBuffers();

		allocatedBuffers.AddEntry(previousOutputFormat);
	}

	return previousOutputFormat;
}

S::Int S::String::ImportFrom(const char *format, const char *str)
{
	Clean();

	Int	 width = 1;

	if	(strncmp(format, "UCS-4", 5)		== 0)	width = 4;
	else if	(strncmp(format, "UCS-2", 5)		== 0)	width = 2;
	else if	(strncmp(format, "UTF-32", 6)		== 0)	width = 4;
	else if	(strncmp(format, "UTF-16", 6)		== 0)	width = 2;
	else if	(strncmp(format, "CSUCS4", 6)		== 0)	width = 4;
	else if	(strncmp(format, "ISO-10646-UCS-4", 15)	== 0)	width = 4;
	else if	(strncmp(format, "ISO-10646-UCS-2", 15)	== 0)	width = 2;
	else if	(strncmp(format, "UNICODELITTLE", 13)	== 0)	width = 2;
	else if	(strncmp(format, "UNICODEBIG", 10)	== 0)	width = 2;
	else if	(strncmp(format, "CSUNICODE", 9)	== 0)	width = 2;
	else if	(strncmp(format, "UNICODE-1-1", 11)	== 0)	width = 2;
	else if	(strncmp(format, "CSUNICODE11", 11)	== 0)	width = 2;

	Int	 len = -1;

	if (width == 1)		while (true) { if (((char  *) str)[++len] == 0) { len *= 1; break; } }
	else if (width == 2)	while (true) { if (((short *) str)[++len] == 0) { len *= 2; break; } }
	else if (width == 4)	while (true) { if (((long  *) str)[++len] == 0) { len *= 4; break; } }

	stringSize = ConvertString(str, len, format, NIL, 0, "UTF-16LE");

	if ((stringSize < 0) && (strcmp(format, "ISO-8859-1") != 0))
	{
		return ImportFrom("ISO-8859-1", str);
	}
	else if (stringSize < 0) return Error;

	stringSize = stringSize / 2 + 1;

	wString = new wchar_t [stringSize];

	ConvertString(str, len, format, (char *) wString, stringSize * 2, "UTF-16LE");

	wString[stringSize - 1] = 0;

	return Success;
}

char *S::String::ConvertTo(const char *encoding)
{
	if (stringSize == 0) return NIL;

	Int	 bufferSize = ConvertString((char *) wString, stringSize * 2, "UTF-16LE", NIL, 0, encoding);

	if (bufferSize == -1)	bufferSize = ConvertString((char *) wString, stringSize * 2, "UTF-16LE", NIL, 0, "ISO-8859-1");

	char	*buffer = NIL;

	if (bufferSize <= 0)
	{
		buffer = new char [Length() + 1];

		ConvertString((char *) wString, stringSize * 2, "UTF-16LE", buffer, Length() + 1, encoding);

		for (Int i = -bufferSize; i < Length(); i++) buffer[i] = '?';

		buffer[Length()] = 0;
	}
	else
	{
		buffer = new char [bufferSize + 1];

		ConvertString((char *) wString, stringSize * 2, "UTF-16LE", buffer, bufferSize + 1, encoding);
	}

	if (allocatedBuffers.GetNOfEntries() >= 1024) DeleteTemporaryBuffers();

	allocatedBuffers.AddEntry(buffer);

	return buffer;
}

wchar_t &S::String::operator [](int n)
{
	wchar_t	*wBuffer;

	if (n >= stringSize - 1)
	{
		if (stringSize > 0)
		{
			wBuffer = new wchar_t [stringSize];

			wcsncpy(wBuffer, wString, stringSize);

			delete [] wString;

			wString = new wchar_t [n + 2];

			for (Int i = 0; i < (n + 1); i++)
			{
				wString[i] = 32;
			}

			wString[n + 1] = 0;

			for (Int j = 0; j < stringSize - 1; j++)
			{
				wString[j] = wBuffer[j];
			}

			stringSize = n + 2;

			delete [] wBuffer;
		}
		else
		{
			Clean();

			wString = new wchar_t [n + 2];

			for (Int i = 0; i < (n + 1); i++)
			{
				wString[i] = 32;
			}

			wString[n + 1] = 0;

			stringSize = n + 2;
		}

		wString[n] = 0;
	}

	return wString[n];
}

wchar_t &S::String::operator [](Int n)
{
	return (*this)[(int) n];
}

S::String::operator char *()
{
	if (outputFormat == NIL) SetOutputFormat("ISO-8859-1");

	return ConvertTo(outputFormat);
}

S::String::operator wchar_t *()
{
	return wString;
}

S::String &S::String::operator =(const int nil)
{
	Clean();

	return *this;
}

S::String &S::String::operator =(const char *newString)
{
	if (newString == NIL)
	{
		Clean();
	}
	else
	{
		if (inputFormat == NIL) SetInputFormat("ISO-8859-1");

		ImportFrom(inputFormat, newString);
	}

	return *this;
}

S::String &S::String::operator =(const wchar_t *newString)
{
	if (newString == NIL)
	{
		Clean();
	}
	else
	{
		Clean();

		stringSize = wcslen(newString) + 1;

		wString = new wchar_t [stringSize];

		wcscpy(wString, newString);
	}

	return *this;
}

S::String &S::String::operator =(const String &newString)
{
	if (newString.wString == NIL)
	{
		Clean();
	}
	else
	{
		String	 backup(newString);

		Clean();

		stringSize = backup.stringSize;

		wString = new wchar_t [stringSize];

		wcsncpy(wString, backup.wString, stringSize);
	}

	return *this;
}

S::Bool S::String::operator ==(const int nil)
{
	if (wString == NIL)	return True;
	if (wString[0] == 0)	return True;
	else			return False;
}

S::Bool S::String::operator ==(const char *str)
{
	if (wString == NIL && str == NIL)	return True;
	if (wString == NIL && str != NIL)	if (str[0] == 0) return True;
	if (wString != NIL && str == NIL)	if (wString[0] == 0) return True;
	if (wString == NIL || str == NIL)	return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator ==(const wchar_t *str)
{
	if (wString == NIL && str == NIL)	return True;
	if (wString == NIL && str != NIL)	if (str[0] == 0) return True;
	if (wString != NIL && str == NIL)	if (wString[0] == 0) return True;
	if (wString == NIL || str == NIL)	return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator ==(const String &str)
{
	if (wString == NIL && str.wString == NIL)	return True;
	if (wString == NIL && str.wString != NIL)	if (str.wString[0] == 0) return True;
	if (wString != NIL && str.wString == NIL)	if (wString[0] == 0) return True;
	if (wString == NIL || str.wString == NIL)	return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator !=(const int nil)
{
	if (wString == NIL)	return False;
	if (wString[0] == 0)	return False;
	else			return True;
}

S::Bool S::String::operator !=(const char *str)
{
	if (wString == NIL && str == NIL)	return False;
	if (wString == NIL && str != NIL)	if (str[0] == 0) return False;
	if (wString != NIL && str == NIL)	if (wString[0] == 0) return False;
	if (wString == NIL || str == NIL)	return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Bool S::String::operator !=(const wchar_t *str)
{
	if (wString == NIL && str == NIL)	return False;
	if (wString == NIL && str != NIL)	if (str[0] == 0) return False;
	if (wString != NIL && str == NIL)	if (wString[0] == 0) return False;
	if (wString == NIL || str == NIL)	return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Bool S::String::operator !=(const String &str)
{
	if (wString == NIL && str.wString == NULL)	return False;
	if (wString == NIL && str.wString != NIL)	if (str.wString[0] == 0) return False;
	if (wString != NIL && str.wString == NIL)	if (wString[0] == 0) return False;
	if (wString == NIL || str.wString == NULL)	return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Int S::String::Length()
{
	if (stringSize == 0) return 0;

	stringSize = 1;

	for (Int i = 0; i >= 0; i++)
	{
		if (wString[i] == 0) break;

		stringSize++;
	}

	return stringSize - 1;
}

S::String &S::String::Append(const char *str)
{
	String	 str2 = str;

	return Append(str2);
}

S::String &S::String::Append(const wchar_t *str)
{
	String	 str2 = str;

	return Append(str2);
}

S::String &S::String::Append(const String &str)
{
	Int	 len1 = Length();
	Int	 len2 = String(str).Length();

	wchar_t	*composed = new wchar_t [len1 + len2 + 1];

	for (Int i = 0; i < (len1 + len2 + 1); i++)
	{
		composed[i] = 0;
	}

	for (Int j = 0; j < len1; j++)
	{
		composed[j] = wString[j];
	}

	for (Int k = len1; k < (len1 + len2); k++)
	{
		composed[k] = str.wString[k - len1];
	}

	*this = composed;

	delete [] composed;

	return *this;
}

S::String &S::String::Replace(const char *str1, const char *str2)
{
	String	 str3 = str1;
	String	 str4 = str2;

	return Replace(str3, str4);
}

S::String &S::String::Replace(const wchar_t *str1, const wchar_t *str2)
{
	String	 str3 = str1;
	String	 str4 = str2;

	return Replace(str3, str4);
}

S::String &S::String::Replace(const char *str1, const String &str2)
{
	String	 str3 = str1;

	return Replace(str3, str2);
}

S::String &S::String::Replace(const wchar_t *str1, const String &str2)
{
	String	 str3 = str1;

	return Replace(str3, str2);
}

S::String &S::String::Replace(const String &str1, const String &str2)
{
	String	 bStr1(str1);
	String	 bStr2(str2);

	if (bStr1 == NIL) return *this;

	for (Int i = 0; i <= Length() - bStr1.Length(); i++)
	{
		Bool	 foundString = True;

		for (Int j = 0; j < bStr1.Length(); j++)
		{
			if (wString[i + j] != bStr1[j])
			{
				foundString = False;
				break;
			}
		}

		if (foundString)
		{
			if (bStr1.Length() == bStr2.Length())
			{
				for (Int k = 0; k < bStr2.Length(); k++) wString[i + k] = bStr2[k];

				i += bStr2.Length() - 1;
			}
			else if (bStr1.Length() > bStr2.Length())
			{
				for (Int k = 0; k < bStr2.Length(); k++) wString[i + k] = bStr2[k];

				for (Int l = bStr2.Length(); l < Length() - i - (bStr1.Length() - bStr2.Length()); l++) wString[i + l] = wString[i + l + (bStr1.Length() - bStr2.Length())];

				wString[Length() - (bStr1.Length() - bStr2.Length())] = 0;

				i += bStr2.Length() - 1;
			}
			else
			{
				Int	 length = Length();
				wchar_t	*backup = new wchar_t [length + 1];

				memcpy((void *) backup, (void *) wString, (length + 1) * 2);

				delete [] wString;

				wString = new wchar_t [length + 1 + (bStr2.Length() - bStr1.Length())];

				memcpy((void *) wString, (void *) backup, i * 2);

				for (Int k = 0; k < bStr2.Length(); k++) wString[i + k] = bStr2[k];

				for (Int l = 0; l < length - i - bStr1.Length() + 1; l++) wString[i + bStr2.Length() + l] = backup[i + bStr1.Length() + l];

				delete [] backup;

				i += bStr2.Length() - 1;
			}
		}
	}

	return *this;
}

S::String &S::String::Copy(const char *str)
{
	String 	 str2 = str;

	return Copy(str2);
}

S::String &S::String::Copy(const wchar_t *str)
{
	String 	 str2 = str;

	return Copy(str2);
}

S::String &S::String::Copy(const String &str)
{
	*this = str;

	return *this;
}

S::String &S::String::CopyN(const char *str, const Int n)
{
	String 	 str2 = str;

	return CopyN(str2, n);
}

S::String &S::String::CopyN(const wchar_t *str, const Int n)
{
	String 	 str2 = str;

	return CopyN(str2, n);
}

S::String &S::String::CopyN(const String &str, const Int n)
{
	String	 backup(str);

	Clean();

	for (Int i = n - 1; i >= 0; i--)
	{
		(*this)[i] = backup[i];
	}

	return *this;
}

S::Int S::String::Compare(const char *str)
{
	String 	 str2 = str;

	return Compare(str2);
}

S::Int S::String::Compare(const wchar_t *str)
{
	String 	 str2 = str;

	return Compare(str2);
}

S::Int S::String::Compare(const String &str)
{
	Int	 len1 = Length();
	Int	 len2 = String(str).Length();

	if (len1 != len2)
	{
		return len1 - len2;
	}
	else
	{
		for (Int i = 0; i <= len1; i++)
		{
			if (wString[i] != str.wString[i]) return 1;
		}
	}

	return 0;
}

S::Int S::String::CompareN(const char *str, Int n)
{
	String 	 str2 = str;

	return CompareN(str2, n);
}

S::Int S::String::CompareN(const wchar_t *str, Int n)
{
	String 	 str2 = str;

	return CompareN(str2, n);
}

S::Int S::String::CompareN(const String &str, Int n)
{
	if (Length() < n) return 1;

	for (int i = 0; i < n; i++)
	{
		if (wString[i] != str.wString[i]) return 1;
	}

	return 0;
}

S::String &S::String::Fill(const Int value)
{
	for (Int i = 0; i < Length(); i++)
	{
		(*this)[i] = value;
	}

	return *this;
}

S::String &S::String::FillN(const Int value, const Int count)
{
	Clean();

	for (Int i = count - 1; i >= 0; i--)
	{
		(*this)[i] = value;
	}

	return *this;
}

S::Int S::String::ToInt()
{
	if (Length() == 0) return 0;

	Bool	 neg = False;
	Int	 first = 0;
	Int	 n = 0;
	Int	 size = Length();

	for (Int i = size - 1; i >= 0; i--)
	{
		if ((wString[i] < 48 || wString[i] > 57) && wString[i] != 45) size = i;
	}

	if (wString[0] == '-')
	{
		neg = True;
		first = 1;
		size--;
	}

	for (Int j = first; j < (first + size); j++)
	{
		n += (Int) Math::Pow(10, size - (j - first) - 1) * (wString[j] - 48);
	}

	if (!neg)	return n;
	else		return 0 - n;
}

S::Float S::String::ToFloat()
{
	if (Length() == 0) return 0;

	Bool	 neg = False;
	Int	 first = 0;
	Float	 n = 0;
	Int	 size = Length();
	Int	 afpsize = 0;
	Int	 firstafp = 0;

	for (Int i = size - 1; i >= 0; i--)
	{
		if ((wString[i] < 48 || wString[i] > 57) && wString[i] != 45) size = i;
	}

	if (wString[size] == '.')
	{
		firstafp = size + 1;

		for (Int i = size + 1; i < Length(); i++)
		{
			if (wString[i] >= 48 && wString[i] <= 57)	afpsize++;
			else						break;
		}
	}

	if (wString[0] == '-')
	{
		neg = True;
		first = 1;
		size--;
	}

	for (Int j = first; j < (first + size); j++)
	{
		n += (Int) Math::Pow(10, size - (j - first) - 1) * (wString[j] - 48);
	}

	for (Int k = firstafp; k < (firstafp + afpsize); k++)
	{
		n += (Float) Math::Pow(10, 0 - (k - firstafp) - 1) * (wString[k] - 48);
	}

	if (!neg)	return n;
	else		return 0 - n;
}

S::String S::String::FromInt(const Int value)
{
	String	 newString;
	Int	 sz;

	if (value == 0)		sz = 1;
	else if (value < 0)	sz = (Int) Math::Log10(-value) + 2;
	else			sz = (Int) Math::Log10(value) + 1;

	if (value < 0)
	{
		newString[0] = 45;

		for (Int i = 0; i < sz-1; i++)
		{
			newString[i + 1] = (wchar_t) Math::Floor(((-value) % (Int) Math::Pow(10, sz - i - 1)) / Math::Pow(10, sz - (i + 1) - 1)) + 48;
		}
	}
	else
	{
		for (Int i = 0; i < sz; i++)
		{
			newString[i] = (wchar_t) Math::Floor((value % (Int) Math::Pow(10, sz - i)) / Math::Pow(10, sz - (i + 1))) + 48;
		}
	}

	return newString;
}

S::String S::String::FromFloat(Float value)
{
	String	 newString;
	Int	 sz;
	Int	 afpslen;
	Int	 lastnn = 0;
	String	 afps;

	if ((Int) value == 0)	sz = 1;
	else if (value < 0)	sz = (Int) Math::Log10(-value) + 1;
	else			sz = (Int) Math::Log10(value) + 1;

	if (value < 0)
	{
		sz++;

		newString[0] = 45;

		for (Int i = 0; i < sz-1; i++)
		{
			newString[i + 1] = (wchar_t) Math::Floor(((Int) (-value) % (Int) Math::Pow(10, sz - i - 1)) / Math::Pow(10, sz - (i + 1) - 1)) + 48;
		}

		afps = FromInt((Int) (-(value - (Int) value) * (Int) Math::Pow(10, 8)));
	}
	else
	{
		for (Int i = 0; i < sz; i++)
		{
			newString[i] = (wchar_t) Math::Floor(((Int) value % (Int) Math::Pow(10, sz - i)) / Math::Pow(10, sz - (i + 1))) + 48;
		}

		afps = FromInt((Int) ((value - (Int) value) * (Int) Math::Pow(10, 8)));
	}

	afpslen = afps.Length();

	for (Int i = 0; i < afpslen; i++)
	{
		if (afps[i] != 48) lastnn = i + 1;
	}

	afps.CopyN(afps, lastnn);

	lastnn = 0;

	for (Int j = 0; j < afpslen; j++)
	{
		if (afps[j] != 57) lastnn = j + 1;
	}

	if (lastnn == 0)	afps.CopyN(afps, 1);
	else			afps.CopyN(afps, lastnn);

	if (lastnn != 0) afps[lastnn - 1] = afps[lastnn - 1] + 1;

	if (value - (Int) value != 0)
	{
		newString.Append(".");

		for (Int i = 0; i < (8 - afpslen); i++)
		{
			newString.Append("0");
		}

		newString.Append(afps);
	}

	return newString;
}

S::Int S::ConvertString(const char *inBuffer, Int inBytes, const char *inEncoding, char *outBuffer, Int outBytes, const char *outEncoding)
{
	Bool	 delBuffer = False;
	Int	 size = 0;

	if (outBuffer == NIL)
	{
		delBuffer	= True;
		outBytes	= inBytes * 8;
		outBuffer	= new char [outBytes];
	}

	for (Int i = 0; i < outBytes; i++) outBuffer[i] = 0;

	if (Setup::useIconv)
	{
		iconv_t		 cd	= iconv_open(outEncoding, inEncoding);

		if ((int) cd == -1) return -1;

		InStream	*in	= new InStream(STREAM_BUFFER, (void *) inBuffer, inBytes);
		OutStream	*out	= new OutStream(STREAM_BUFFER, (void *) outBuffer, outBytes);

		iconv(cd, NULL, NULL, NULL, NULL);

		char		 inBuf[4096 + 4096];
		size_t		 inBufRest = 0;
		char		 outBuf[4096];
		Bool		 convError = False;

		for (;;)
		{
			size_t	 inBufSize = min(in->Size() - in->GetPos(), 4096);

			in->InputData((void *) (inBuf + 4096), inBufSize);

			if (inBufSize == 0)
			{
				if (inBufRest == 0)
				{
					break;
				}
				else
				{
					iconv_close(cd);

					delete in;
					delete out;

					return 0;
				}
			}
			else
			{
				const char	*inPtr	= inBuf + 4096 - inBufRest;
				size_t		 inSize	= inBufRest + inBufSize;

				inBufRest = 0;

				while (inSize > 0)
				{
					char	*outPtr		= outBuf;
					size_t	 outSize	= sizeof(outBuf);

					if ((signed) iconv(cd, (const char **) &inPtr, &inSize, &outPtr, &outSize) < 0)
					{
						out->OutputData((void *) outBuf, outPtr - outBuf);
						size += (outPtr - outBuf);

						convError = True;

						break;
					}

					if (outPtr != outBuf)
					{
						out->OutputData((void *) outBuf, outPtr - outBuf);
						size += (outPtr - outBuf);
					}
				}

				if (convError) break;
			}
		}

		char	*outPtr		= outBuf;
		size_t	 outSize	= sizeof(outBuf);

		iconv(cd, NULL, NULL, &outPtr, &outSize);

		if (outPtr != outBuf)
		{
			out->OutputData((void *) outBuf, outPtr - outBuf);
			size += (outPtr - outBuf);
		}

		iconv_close(cd);

		if (size >= outBytes) size = 0;
		if (convError) size = -size;

		delete in;
		delete out;
	}
#ifdef __WIN32__
	else if (strcmp(outEncoding, "UTF-16LE") == 0)
	{
		Int	 codePage = CP_ACP;

		if (strcmp(inEncoding, "ISO-8859-1") == 0)		codePage = 28591;
		else if (strcmp(inEncoding, "ISO-8859-2") == 0)		codePage = 28592;
		else if (strcmp(inEncoding, "ISO-8859-3") == 0)		codePage = 28593;
		else if (strcmp(inEncoding, "ISO-8859-4") == 0)		codePage = 28594;
		else if (strcmp(inEncoding, "ISO-8859-5") == 0)		codePage = 28595;
		else if (strcmp(inEncoding, "ISO-8859-6") == 0)		codePage = 28596;
		else if (strcmp(inEncoding, "ISO-8859-7") == 0)		codePage = 28597;
		else if (strcmp(inEncoding, "ISO-8859-8") == 0)		codePage = 28598;
		else if (strcmp(inEncoding, "ISO-8859-9") == 0)		codePage = 28599;
		else if (strcmp(inEncoding, "ISO-8859-15") == 0)	codePage = 28605;
		else if (strcmp(inEncoding, "KOI8-R") == 0)		codePage = 20866;
		else if (strcmp(inEncoding, "KOI8-RU") == 0)		codePage = 21866;
		else if (strcmp(inEncoding, "GBK") == 0)		codePage = 936;
		else if (strcmp(inEncoding, "UTF-7") == 0)		codePage = CP_UTF7;
		else if (strcmp(inEncoding, "UTF-8") == 0)		codePage = CP_UTF8;

		size = MultiByteToWideChar(codePage, 0, inBuffer, -1, NIL, 0) * 2;

		if (size < outBytes && size > 0)
		{
			MultiByteToWideChar(codePage, 0, inBuffer, -1, (wchar_t *) outBuffer, size / 2);
		}

		if (size >= outBytes) size = 0;
	}
	else if (strcmp(inEncoding, "UTF-16LE") == 0)
	{
		Int	 codePage = CP_ACP;

		if (strcmp(outEncoding, "ISO-8859-1") == 0)		codePage = 28591;
		else if (strcmp(outEncoding, "ISO-8859-2") == 0)	codePage = 28592;
		else if (strcmp(outEncoding, "ISO-8859-3") == 0)	codePage = 28593;
		else if (strcmp(outEncoding, "ISO-8859-4") == 0)	codePage = 28594;
		else if (strcmp(outEncoding, "ISO-8859-5") == 0)	codePage = 28595;
		else if (strcmp(outEncoding, "ISO-8859-6") == 0)	codePage = 28596;
		else if (strcmp(outEncoding, "ISO-8859-7") == 0)	codePage = 28597;
		else if (strcmp(outEncoding, "ISO-8859-8") == 0)	codePage = 28598;
		else if (strcmp(outEncoding, "ISO-8859-9") == 0)	codePage = 28599;
		else if (strcmp(outEncoding, "ISO-8859-15") == 0)	codePage = 28605;
		else if (strcmp(outEncoding, "KOI8-R") == 0)		codePage = 20866;
		else if (strcmp(outEncoding, "KOI8-RU") == 0)		codePage = 21866;
		else if (strcmp(outEncoding, "GBK") == 0)		codePage = 936;
		else if (strcmp(outEncoding, "UTF-7") == 0)		codePage = CP_UTF7;
		else if (strcmp(outEncoding, "UTF-8") == 0)		codePage = CP_UTF8;

		size = WideCharToMultiByte(codePage, 0, (wchar_t *) inBuffer, -1, NIL, 0, NIL, NIL);

		if (size < outBytes && size > 0)
		{
			WideCharToMultiByte(codePage, 0, (wchar_t *) inBuffer, -1, outBuffer, size, NIL, NIL);
		}

		if (size >= outBytes) size = 0;
	}
#endif

	if (delBuffer) delete [] outBuffer;

	return size;
}
