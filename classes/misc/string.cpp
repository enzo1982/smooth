 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/string.h>
#include <smooth/misc/number.h>
#include <smooth/misc/encoding/base64.h>
#include <smooth/misc/hash/crc32.h>
#include <smooth/templates/buffer.h>

#include <iconv.h>
#include <memory.h>

#if defined __WINE__
#	define wcslen lstrlenW
#	define wcscpy lstrcpyW
#	define wcsncpy lstrcpynW
#endif

namespace smooth
{
	Int	 ConvertString(const char *, Int, const char *, char *, Int, const char *);
};

char			*S::String::inputFormat		= NIL;
char			*S::String::outputFormat	= NIL;
char			*S::String::internalFormat	= NIL;

S::Int			 S::String::nOfStrings		= 0;

S::Array<char *>	 S::String::allocatedBuffers;

S::String::String(const int nil)
{
	mutex.Lock();

	nOfStrings++;

	wString.Resize(0);

	Clean();

	mutex.Release();
}

S::String::String(const char *iString)
{
	mutex.Lock();

	nOfStrings++;

	wString.Resize(0);

	*this = iString;

	mutex.Release();
}

S::String::String(const wchar_t *iString)
{
	mutex.Lock();

	nOfStrings++;

	if (iString == NIL)
	{
		wString.Resize(0);

		Clean();
	}
	else
	{
		Int	 size = wcslen(iString) + 1;

		wString.Resize(size);

		wcsncpy(wString, iString, size);

		stringSize = size;
	}

	mutex.Release();
}

S::String::String(const String &iString)
{
	mutex.Lock();

	nOfStrings++;

	if (iString.wString.Size() == 0)
	{
		wString.Resize(0);

		Clean();
	}
	else
	{
		wString.Resize(iString.stringSize);

		wcsncpy(wString, iString.wString, iString.stringSize);

		stringSize = iString.stringSize;
	}

	mutex.Release();
}

S::String::~String()
{
	mutex.Lock();

	Clean();

	if (--nOfStrings == 0)
	{
		for (Int i = 0; i < allocatedBuffers.Length(); i++) delete [] allocatedBuffers.GetNth(i);

		allocatedBuffers.RemoveAll();
	}

	mutex.Release();
}

S::Void S::String::DeleteTemporaryBuffers()
{
	Int	 nOfEntries = allocatedBuffers.Length();

	for (Int i = 0; i < nOfEntries - 512; i++)
	{
		delete [] allocatedBuffers.GetFirst();

		allocatedBuffers.RemoveNth(0);
	}
}

S::Void S::String::Clean()
{
	mutex.Lock();

	stringSize = 0;

	wString.Resize(0);

	mutex.Release();
}

S::Int S::String::ComputeCRC32() const
{
	if (wString.Size() == 0) return 0;

	Buffer<UnsignedByte>	 buffer((Length() + 1) * sizeof(wchar_t));

	wcscpy((wchar_t *) (UnsignedByte *) buffer, wString);

	return Hash::CRC32(buffer).Compute();
}

S::String S::String::EncodeBase64() const
{
	if (wString.Size() == 0) return NIL;

	Buffer<UnsignedByte>	 buffer(Length() * 4 + 1);

	strcpy((char *) (UnsignedByte *) buffer, ConvertTo("UTF-8"));

	return Encoding::Base64(buffer).Encode();
}

S::String S::String::DecodeBase64() const
{
	if (wString.Size() == 0) return NIL;

	Buffer<UnsignedByte>	 buffer;
	String			 string;

	Encoding::Base64(buffer).Decode(*this);

	string.ImportFrom("UTF-8", (char *) (UnsignedByte *) buffer);

	return string;
}

S::Bool S::String::IsANSI(const String &string)
{
	return !IsUnicode(string);
}

S::Bool S::String::IsUnicode(const String &string)
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
		if (allocatedBuffers.Length() >= 1024) DeleteTemporaryBuffers();

		allocatedBuffers.Add(previousInputFormat);
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
		if (allocatedBuffers.Length() >= 1024) DeleteTemporaryBuffers();

		allocatedBuffers.Add(previousOutputFormat);
	}

	return previousOutputFormat;
}

const char *S::String::GetInternalFormat()
{
	if (internalFormat == NIL)
	{
		if	(sizeof(wchar_t) == 1)	internalFormat = (char *) "UTF-8";
		else if (sizeof(wchar_t) == 2)	internalFormat = (char *) "UTF-16LE";
		else if (sizeof(wchar_t) == 4)	internalFormat = (char *) "UTF-32LE";
	}

	return internalFormat;
}

S::Int S::String::ImportFrom(const char *format, const char *str)
{
	Clean();

	if (str == NIL) return Success();

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

	Int	 size = ConvertString(str, len, format, NIL, 0, GetInternalFormat());

	if ((size < 0) && (strcmp(format, "ISO-8859-1") != 0))
	{
		return ImportFrom("ISO-8859-1", str);
	}
	else if (size < 0) return Error();

	size = size / sizeof(wchar_t) + 1;

	mutex.Lock();

	wString.Resize(size);

	ConvertString(str, len, format, (char *) (wchar_t *) wString, size * sizeof(wchar_t), GetInternalFormat());

	wString[size - 1] = 0;

	stringSize = size;

	mutex.Release();

	return Success();
}

char *S::String::ConvertTo(const char *encoding) const
{
	if (stringSize == 0) return NIL;

	Int	 bufferSize = ConvertString((char *) (wchar_t *) wString, stringSize * sizeof(wchar_t), GetInternalFormat(), NIL, 0, encoding);

	if (bufferSize == -1)	bufferSize = ConvertString((char *) (wchar_t *) wString, stringSize * sizeof(wchar_t), GetInternalFormat(), NIL, 0, "ISO-8859-1");

	char	*buffer = NIL;

	if (bufferSize <= 0)
	{
		buffer = new char [Length() + 1];

		ConvertString((char *) (wchar_t *) wString, stringSize * sizeof(wchar_t), GetInternalFormat(), buffer, Length() + 1, encoding);

		for (Int i = -bufferSize; i < Length(); i++) buffer[i] = '?';

		buffer[Length()] = 0;
	}
	else
	{
		buffer = new char [bufferSize + 1];

		ConvertString((char *) (wchar_t *) wString, stringSize * sizeof(wchar_t), GetInternalFormat(), buffer, bufferSize + 1, encoding);
	}

	if (allocatedBuffers.Length() >= 1024) DeleteTemporaryBuffers();

	allocatedBuffers.Add(buffer);

	return buffer;
}

wchar_t &S::String::operator [](int n)
{
	if (n >= stringSize - 1)
	{
		mutex.Lock();

		if (stringSize > 0)
		{
			wBuffer.Resize(stringSize);

			wcsncpy(wBuffer, wString, stringSize);

			wString.Resize(n + 2);

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
		}
		else
		{
			Clean();

			wString.Resize(n + 2);

			for (Int i = 0; i < (n + 1); i++)
			{
				wString[i] = 32;
			}

			wString[n + 1] = 0;

			stringSize = n + 2;
		}

		wString[n] = 0;

		mutex.Release();
	}

	return wString[n];
}

wchar_t &S::String::operator [](Int n)
{
	return (*this)[(int) n];
}

wchar_t S::String::operator [](int n) const
{
	if (n >= stringSize - 1) return 0;

	return wString[n];
}

wchar_t S::String::operator [](Int n) const
{
	return (*this)[(int) n];
}

S::String::operator char *() const
{
	if (outputFormat == NIL) SetOutputFormat("ISO-8859-1");

	return ConvertTo(outputFormat);
}

S::String::operator wchar_t *() const
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
		Int	 size = wcslen(newString) + 1;

		mutex.Lock();

		Clean();

		wString.Resize(size);

		wcsncpy(wString, newString, size);

		stringSize = size;

		mutex.Release();
	}

	return *this;
}

S::String &S::String::operator =(const String &newString)
{
	if (newString.wString.Size() == 0)
	{
		Clean();
	}
	else
	{
		String	 backup(newString);

		mutex.Lock();

		Clean();

		wString.Resize(backup.stringSize);

		wcsncpy(wString, backup.wString, backup.stringSize);

		stringSize = backup.stringSize;

		mutex.Release();
	}

	return *this;
}

S::Bool S::String::operator ==(const int nil) const
{
	if (wString.Size() == 0) return True;
	if (wString[0] == 0)	 return True;
	else			 return False;
}

S::Bool S::String::operator ==(const char *str) const
{
	if (wString.Size() == 0 && str == NIL) return True;
	if (wString.Size() == 0 && str != NIL) if (str[0] == 0) return True;
	if (wString.Size() != 0 && str == NIL) if (wString[0] == 0) return True;
	if (wString.Size() == 0 || str == NIL) return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator ==(const wchar_t *str) const
{
	if (wString.Size() == 0 && str == NIL) return True;
	if (wString.Size() == 0 && str != NIL) if (str[0] == 0) return True;
	if (wString.Size() != 0 && str == NIL) if (wString[0] == 0) return True;
	if (wString.Size() == 0 || str == NIL) return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator ==(const String &str) const
{
	if (wString.Size() == 0 && str.wString.Size() == 0) return True;
	if (wString.Size() == 0 && str.wString.Size() != 0) if (str.wString[0] == 0) return True;
	if (wString.Size() != 0 && str.wString.Size() == 0) if (wString[0] == 0) return True;
	if (wString.Size() == 0 || str.wString.Size() == 0) return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator !=(const int nil) const
{
	if (wString.Size() == 0) return False;
	if (wString[0] == 0)	 return False;
	else			 return True;
}

S::Bool S::String::operator !=(const char *str) const
{
	if (wString.Size() == 0 && str == NIL)	return False;
	if (wString.Size() == 0 && str != NIL)	if (str[0] == 0) return False;
	if (wString.Size() != 0 && str == NIL)	if (wString[0] == 0) return False;
	if (wString.Size() == 0 || str == NIL)	return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Bool S::String::operator !=(const wchar_t *str) const
{
	if (wString.Size() == 0 && str == NIL) return False;
	if (wString.Size() == 0 && str != NIL) if (str[0] == 0) return False;
	if (wString.Size() != 0 && str == NIL) if (wString[0] == 0) return False;
	if (wString.Size() == 0 || str == NIL) return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Bool S::String::operator !=(const String &str) const
{
	if (wString.Size() == 0 && str.wString.Size() == 0) return False;
	if (wString.Size() == 0 && str.wString.Size() != 0) if (str.wString[0] == 0) return False;
	if (wString.Size() != 0 && str.wString.Size() == 0) if (wString[0] == 0) return False;
	if (wString.Size() == 0 || str.wString.Size() == 0) return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Int S::String::Length() const
{
	if (stringSize == 0) return 0;

	mutex.Lock();

	stringSize = 0;

	for (Int i = 0; True; i++)
	{
		stringSize++;

		if (wString[i] == 0) break;
	}

	mutex.Release();

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
	Int	 len2 = str.Length();

	wBuffer.Resize(len1 + len2 + 1);

	mutex.Lock();

	wcsncpy(wBuffer, wString, len1);
	wcsncpy(wBuffer + len1, str.wString, len2);

	wBuffer[len1 + len2] = 0;

	*this = wBuffer;

	mutex.Release();

	return *this;
}

S::Int S::String::Find(const char *str) const
{
	String 	 str2 = str;

	return Find(str2);
}

S::Int S::String::Find(const wchar_t *str) const
{
	String 	 str2 = str;

	return Find(str2);
}

S::Int S::String::Find(const String &str) const
{
	String	 bStr(str);

	mutex.Lock();

	for (Int i = 0; i <= Length() - bStr.Length(); i++)
	{
		Bool	 foundString = True;

		for (Int j = 0; j < bStr.Length(); j++)
		{
			if (wString[i + j] != bStr[j])
			{
				foundString = False;
				break;
			}
		}

		if (foundString) { mutex.Release(); return i; }
	}

	mutex.Release();

	return -1;
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

	mutex.Lock();

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

				wBuffer.Resize(length + 1);

				memcpy((void *) wBuffer, (void *) wString, (length + 1) * sizeof(wchar_t));

				wString.Resize(length + 1 + (bStr2.Length() - bStr1.Length()));

				memcpy((void *) wString, (void *) wBuffer, i * sizeof(wchar_t));

				for (Int k = 0; k < bStr2.Length(); k++) wString[i + k] = bStr2[k];

				for (Int l = 0; l < length - i - bStr1.Length() + 1; l++) wString[i + bStr2.Length() + l] = wBuffer[i + bStr1.Length() + l];

				i += bStr2.Length() - 1;
			}
		}
	}

	mutex.Release();

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

	mutex.Lock();

	Clean();

	for (Int i = n - 1; i >= 0; i--)
	{
		(*this)[i] = backup[i];
	}

	mutex.Release();

	return *this;
}

S::Int S::String::Compare(const char *str) const
{
	String 	 str2 = str;

	return Compare(str2);
}

S::Int S::String::Compare(const wchar_t *str) const
{
	String 	 str2 = str;

	return Compare(str2);
}

S::Int S::String::Compare(const String &str) const
{
	Int	 len1 = Length();
	Int	 len2 = str.Length();

	if (len1 != len2)
	{
		return len1 - len2;
	}
	else
	{
		mutex.Lock();

		for (Int i = 0; i < len1; i++)
		{
			if (wString[i] != str.wString[i]) { mutex.Release(); return 1; }
		}

		mutex.Release();
	}

	return 0;
}

S::Int S::String::CompareN(const char *str, Int n) const
{
	String 	 str2 = str;

	return CompareN(str2, n);
}

S::Int S::String::CompareN(const wchar_t *str, Int n) const
{
	String 	 str2 = str;

	return CompareN(str2, n);
}

S::Int S::String::CompareN(const String &str, Int n) const
{
	if (Length() < n) return 1;

	mutex.Lock();

	for (Int i = 0; i < n; i++)
	{
		if (wString[i] != str.wString[i]) { mutex.Release(); return 1; }
	}

	mutex.Release();

	return 0;
}

S::Bool S::String::StartsWith(const char *str) const
{
	String 	 str2 = str;

	return StartsWith(str2);
}

S::Bool S::String::StartsWith(const wchar_t *str) const
{
	String 	 str2 = str;

	return StartsWith(str2);
}

S::Bool S::String::StartsWith(const String &str) const
{
	Int	 len1 = Length();
	Int	 len2 = str.Length();

	if (len1 >= len2)
	{
		mutex.Lock();

		for (Int i = 0; i < len2; i++)
		{
			if (wString[i] != str.wString[i]) { mutex.Release(); return False; }
		}

		mutex.Release();

		return True;
	}

	return False;
}

S::Bool S::String::EndsWith(const char *str) const
{
	String 	 str2 = str;

	return EndsWith(str2);
}

S::Bool S::String::EndsWith(const wchar_t *str) const
{
	String 	 str2 = str;

	return EndsWith(str2);
}

S::Bool S::String::EndsWith(const String &str) const
{
	Int	 len1 = Length();
	Int	 len2 = str.Length();

	if (len1 >= len2)
	{
		mutex.Lock();

		for (Int i = 0; i < len2; i++)
		{
			if (wString[len1 - len2 + i] != str.wString[i]) { mutex.Release(); return False; }
		}

		mutex.Release();

		return True;
	}

	return False;
}

S::String S::String::SubString(Int start, Int number) const
{
	String	 subString;

	for (Int i = 0; i < number; i++) subString[i] = (*this)[start + i];

	return subString;
}

S::String S::String::Head(Int n) const
{
	return SubString(0, n);
}

S::String S::String::Tail(Int n) const
{
	return SubString(Length() - n, n);
}

S::String S::String::Trim() const
{
	Int	 len = Length();
	Int	 triml = 0;
	Int	 trimr = 0;

	for (Int i = 0; i < len; i++)
	{
		if ((*this)[i] == ' '  ||
		    (*this)[i] == '\t' ||
		    (*this)[i] == '\n' ||
		    (*this)[i] == '\r' ||
		    (*this)[i] == '\0' ||
		    (*this)[i] == '\x0B') triml++;
		else			  break;
	}

	for (Int i = len - 1; i >= 0; i--)
	{
		if ((*this)[i] == ' '  ||
		    (*this)[i] == '\t' ||
		    (*this)[i] == '\n' ||
		    (*this)[i] == '\r' ||
		    (*this)[i] == '\0' ||
		    (*this)[i] == '\x0B') trimr++;
		else			  break;
	}

	return SubString(triml, len - triml - trimr);
}

S::String &S::String::Fill(const Int value)
{
	mutex.Lock();

	for (Int i = 0; i < Length(); i++)
	{
		(*this)[i] = value;
	}

	mutex.Release();

	return *this;
}

S::String &S::String::FillN(const Int value, const Int count)
{
	mutex.Lock();

	Clean();

	for (Int i = count - 1; i >= 0; i--)
	{
		(*this)[i] = value;
	}

	mutex.Release();

	return *this;
}

S::Int64 S::String::ToInt() const
{
	return Number::FromIntString(*this);
}

S::Float S::String::ToFloat() const
{
	return Number::FromFloatString(*this);
}

S::String S::String::FromInt(const Int64 value)
{
	return Number(value).ToIntString();
}

S::String S::String::FromFloat(Float value)
{
	return Number(value).ToFloatString();
}

S::String S::String::ToLower() const
{
	String	 retVal = *this;

	mutex.Lock();

	for (Int i = 0; i < Length(); i++)
	{
		if	((*this)[i] >= 0x0041 && (*this)[i] <= 0x005A)				retVal[i] = (*this)[i] + 0x0020; // Latin alphabet
		else if	((*this)[i] >= 0x00C0 && (*this)[i] <= 0x00D6)				retVal[i] = (*this)[i] + 0x0020; // Latin special letters
		else if	((*this)[i] >= 0x00D8 && (*this)[i] <= 0x00DE)				retVal[i] = (*this)[i] + 0x0020; // Latin special letters
		else if	((*this)[i] >= 0x0100 && (*this)[i] <= 0x0136 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x0139 && (*this)[i] <= 0x0147 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x014A && (*this)[i] <= 0x0176 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x0179 && (*this)[i] <= 0x017D &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x0182 && (*this)[i] <= 0x0184 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01A0 && (*this)[i] <= 0x01A4 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01B3 && (*this)[i] <= 0x01B5 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01CD && (*this)[i] <= 0x01DB &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01DE && (*this)[i] <= 0x01EE && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01F8 && (*this)[i] <= 0x0232 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x1E00 && (*this)[i] <= 0x1E94 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x1EA0 && (*this)[i] <= 0x1EF8 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Latin special letters
		else if	((*this)[i] == 0x0178)							retVal[i] =		 0x00FF; // Latin special letter
		else if	((*this)[i] == 0x0187)							retVal[i] =		 0x0188; // Latin special letter
		else if	((*this)[i] == 0x018B)							retVal[i] =		 0x018C; // Latin special letter
		else if	((*this)[i] == 0x018F)							retVal[i] =		 0x0259; // Latin special letter
		else if	((*this)[i] == 0x0191)							retVal[i] =		 0x0192; // Latin special letter
		else if	((*this)[i] == 0x0198)							retVal[i] =		 0x0199; // Latin special letter
		else if	((*this)[i] == 0x01AC)							retVal[i] =		 0x01AD; // Latin special letter
		else if	((*this)[i] == 0x01AF)							retVal[i] =		 0x01B0; // Latin special letter
		else if	((*this)[i] == 0x01B8)							retVal[i] =		 0x01B9; // Latin special letter
		else if	((*this)[i] == 0x01F4)							retVal[i] =		 0x01F5; // Latin special letter
		else if	((*this)[i] >= 0x0391 && (*this)[i] <= 0x03AB)				retVal[i] = (*this)[i] + 0x0020; // Greek alphabet
		else if	((*this)[i] >= 0x0388 && (*this)[i] <= 0x038A)				retVal[i] = (*this)[i] + 0x0025; // Greek special letters
		else if	((*this)[i] >= 0x038E && (*this)[i] <= 0x038F)				retVal[i] = (*this)[i] + 0x003F; // Greek special letters
		else if	((*this)[i] >= 0x03DA && (*this)[i] <= 0x03E0 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Greek special letters
		else if	((*this)[i] == 0x0386)							retVal[i] =		 0x03AC; // Greek special letter
		else if	((*this)[i] == 0x038C)							retVal[i] =		 0x03CC; // Greek special letter
		else if	((*this)[i] >= 0x0410 && (*this)[i] <= 0x042F)				retVal[i] = (*this)[i] + 0x0020; // Cyrillic alphabet
		else if	((*this)[i] >= 0x0400 && (*this)[i] <= 0x040F)				retVal[i] = (*this)[i] + 0x0050; // Cyrillic special letters
		else if	((*this)[i] >= 0x0460 && (*this)[i] <= 0x0480 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Cyrillic special letters
		else if	((*this)[i] >= 0x048C && (*this)[i] <= 0x04BE && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Cyrillic special letters
		else if	((*this)[i] >= 0x04C1 && (*this)[i] <= 0x04CB &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Cyrillic special letters
		else if	((*this)[i] >= 0x04D0 && (*this)[i] <= 0x04F8 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x0001; // Cyrillic special letters
	}

	mutex.Release();

	return retVal;
}

S::String S::String::ToUpper() const
{
	String	 retVal = *this;

	mutex.Lock();

	for (Int i = 0; i < Length(); i++)
	{
		if	((*this)[i] >= 0x0061 && (*this)[i] <= 0x007A)				retVal[i] = (*this)[i] - 0x0020; // Latin alphabet
		else if	((*this)[i] >= 0x00E0 && (*this)[i] <= 0x00F6)				retVal[i] = (*this)[i] - 0x0020; // Latin special letters
		else if	((*this)[i] >= 0x00F8 && (*this)[i] <= 0x00FE)				retVal[i] = (*this)[i] - 0x0020; // Latin special letters
		else if	((*this)[i] >= 0x0101 && (*this)[i] <= 0x0137 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x013A && (*this)[i] <= 0x0148 && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x014B && (*this)[i] <= 0x0177 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x017A && (*this)[i] <= 0x017E && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x0183 && (*this)[i] <= 0x0185 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01A1 && (*this)[i] <= 0x01A5 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01B4 && (*this)[i] <= 0x01B6 && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01CE && (*this)[i] <= 0x01DC && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01DF && (*this)[i] <= 0x01EF &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x01F9 && (*this)[i] <= 0x0233 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x1E01 && (*this)[i] <= 0x1E95 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] >= 0x1EA1 && (*this)[i] <= 0x1EF9 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Latin special letters
		else if	((*this)[i] == 0x00FF)							retVal[i] =		 0x0178; // Latin special letter
		else if	((*this)[i] == 0x0188)							retVal[i] =		 0x0187; // Latin special letter
		else if	((*this)[i] == 0x018C)							retVal[i] =		 0x018B; // Latin special letter
		else if	((*this)[i] == 0x0259)							retVal[i] =		 0x018F; // Latin special letter
		else if	((*this)[i] == 0x0192)							retVal[i] =		 0x0191; // Latin special letter
		else if	((*this)[i] == 0x0199)							retVal[i] =		 0x0198; // Latin special letter
		else if	((*this)[i] == 0x01AD)							retVal[i] =		 0x01AC; // Latin special letter
		else if	((*this)[i] == 0x01B0)							retVal[i] =		 0x01AF; // Latin special letter
		else if	((*this)[i] == 0x01B9)							retVal[i] =		 0x01B8; // Latin special letter
		else if	((*this)[i] == 0x01F5)							retVal[i] =		 0x01F4; // Latin special letter
		else if	((*this)[i] >= 0x03B1 && (*this)[i] <= 0x03CB)				retVal[i] = (*this)[i] - 0x0020; // Greek alphabet
		else if	((*this)[i] >= 0x03AD && (*this)[i] <= 0x03AF)				retVal[i] = (*this)[i] - 0x0025; // Greek special letters
		else if	((*this)[i] >= 0x03CD && (*this)[i] <= 0x03CE)				retVal[i] = (*this)[i] - 0x003F; // Greek special letters
		else if	((*this)[i] >= 0x03DB && (*this)[i] <= 0x03E1 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Greek special letters
		else if	((*this)[i] == 0x03AC)							retVal[i] =		 0x0386; // Greek special letter
		else if	((*this)[i] == 0x03CC)							retVal[i] =		 0x038C; // Greek special letter
		else if	((*this)[i] >= 0x0430 && (*this)[i] <= 0x044F)				retVal[i] = (*this)[i] - 0x0020; // Cyrillic alphabet
		else if	((*this)[i] >= 0x0450 && (*this)[i] <= 0x045F)				retVal[i] = (*this)[i] - 0x0050; // Cyrillic special letters
		else if	((*this)[i] >= 0x0461 && (*this)[i] <= 0x0481 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Cyrillic special letters
		else if	((*this)[i] >= 0x048D && (*this)[i] <= 0x04BF &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Cyrillic special letters
		else if	((*this)[i] >= 0x04C2 && (*this)[i] <= 0x04CC && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Cyrillic special letters
		else if	((*this)[i] >= 0x04D1 && (*this)[i] <= 0x04F9 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x0001; // Cyrillic special letters
	}

	mutex.Release();

	return retVal;
}

S::Int S::ConvertString(const char *inBuffer, Int inBytes, const char *inEncoding, char *outBuffer, Int outBytes, const char *outEncoding)
{
	static Buffer<char>	 csBuffer;
	Int			 size = 0;

	if (outBuffer == NIL)
	{
		csBuffer.Resize(inBytes * 8);
		csBuffer.Zero();

		outBytes	= csBuffer.Size();
		outBuffer	= csBuffer;
	}

#ifdef __WIN32__
	if (Setup::useIconv)
#else
	if (1)
#endif
	{
		iconv_t		 cd	= iconv_open(outEncoding, inEncoding);

		if (cd == (iconv_t) -1) return -1;

		size_t		 inBytesLeft	= inBytes;
		size_t		 outBytesLeft	= outBytes;
		const char     **inPointer	= &inBuffer;
		char	       **outPointer	= &outBuffer;

		while (inBytesLeft)
		{
			if (iconv(cd, inPointer, &inBytesLeft, outPointer, &outBytesLeft) == (size_t) -1) break;
		}

		iconv_close(cd);

		size = !outBytesLeft ? 0 : outBytes - outBytesLeft;

		if (inBytesLeft) size *= -1;
	}
	else if (strcmp(inEncoding, outEncoding) == 0)
	{
		size = inBytes;

		if (size < outBytes && size > 0)
		{
			wcsncpy((wchar_t *) outBuffer, (wchar_t *) inBuffer, size / sizeof(wchar_t));
		}

		if (size >= outBytes) size = 0;
	}
	else if ((strcmp(inEncoding, "UTF-16LE") == 0 && strcmp(outEncoding, "UTF-16BE") == 0) || (strcmp(inEncoding, "UTF-16BE") == 0 && strcmp(outEncoding, "UTF-16LE") == 0))
	{
		size = inBytes;

		if (size < outBytes && size > 0)
		{
			for (UnsignedInt i = 0; i < size / sizeof(wchar_t); i++) ((wchar_t *) outBuffer)[i] = ((((wchar_t *) inBuffer)[i] & 255) << 8) | (((wchar_t *) inBuffer)[i] >> 8);
		}

		if (size >= outBytes) size = 0;
	}
#ifdef __WIN32__
	else if (strcmp(outEncoding, "UTF-16LE") == 0)
	{
		Int	 codePage = CP_ACP;

		if (strcmp(inEncoding, "UTF-8") == 0)			codePage = CP_UTF8;
		else if (strcmp(inEncoding, "UTF-7") == 0)		codePage = CP_UTF7;
		else if (strcmp(inEncoding, "ISO-8859-1") == 0)		codePage = 28591;
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
		else if (strcmp(inEncoding, "KOI8-U") == 0)		codePage = 21866;
		else if (strcmp(inEncoding, "SHIFT-JIS") == 0)		codePage = 932;
		else if (strcmp(inEncoding, "GBK") == 0)		codePage = 936;
		else if (strcmp(inEncoding, "BIG-5") == 0)		codePage = 950;
		else if (strcmp(inEncoding, "ISO-2022-JP") == 0)	codePage = 50220;
		else if (strcmp(inEncoding, "ISO-2022-KR") == 0)	codePage = 50225;
		else if (strcmp(inEncoding, "ISO-2022-CN") == 0)	codePage = 50227;

		if (inEncoding[0] == 'C' && inEncoding[1] == 'P')
		{
			String	 cpString = String().ImportFrom("ISO-8859-1", inEncoding + 2);

			codePage = cpString.ToInt();
		}

		size = MultiByteToWideChar(codePage, 0, inBuffer, -1, NIL, 0) * sizeof(wchar_t);

		// Codepage not installed? Let's try CP_ACP!
		if (size == 0 && GetLastError() == ERROR_INVALID_PARAMETER) size = MultiByteToWideChar(codePage = CP_ACP, 0, inBuffer, -1, NIL, 0) * sizeof(wchar_t);

		if (size < outBytes && size > 0) MultiByteToWideChar(codePage, 0, inBuffer, -1, (wchar_t *) outBuffer, size / sizeof(wchar_t));
		else if (size >= outBytes)	 size = 0;
	}
	else if (strcmp(inEncoding, "UTF-16LE") == 0)
	{
		Int	 codePage = CP_ACP;

		if (strcmp(outEncoding, "UTF-8") == 0)			codePage = CP_UTF8;
		else if (strcmp(outEncoding, "UTF-7") == 0)		codePage = CP_UTF7;
		else if (strcmp(outEncoding, "ISO-8859-1") == 0)	codePage = 28591;
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
		else if (strcmp(outEncoding, "KOI8-U") == 0)		codePage = 21866;
		else if (strcmp(outEncoding, "SHIFT-JIS") == 0)		codePage = 932;
		else if (strcmp(outEncoding, "GBK") == 0)		codePage = 936;
		else if (strcmp(outEncoding, "BIG-5") == 0)		codePage = 950;
		else if (strcmp(outEncoding, "ISO-2022-JP") == 0)	codePage = 50220;
		else if (strcmp(outEncoding, "ISO-2022-KR") == 0)	codePage = 50225;
		else if (strcmp(outEncoding, "ISO-2022-CN") == 0)	codePage = 50227;

		if (outEncoding[0] == 'C' && outEncoding[1] == 'P')
		{
			String	 cpString = String().ImportFrom("ISO-8859-1", outEncoding + 2);

			codePage = cpString.ToInt();
		}

		size = WideCharToMultiByte(codePage, 0, (wchar_t *) inBuffer, -1, NIL, 0, NIL, NIL);

		// Codepage not installed? Let's try CP_ACP!
		if (size == 0 && GetLastError() == ERROR_INVALID_PARAMETER) size = WideCharToMultiByte(codePage = CP_ACP, 0, (wchar_t *) inBuffer, -1, NIL, 0, NIL, NIL);

		if (size < outBytes && size > 0) WideCharToMultiByte(codePage, 0, (wchar_t *) inBuffer, -1, outBuffer, size, NIL, NIL);
		else if (size >= outBytes)	 size = 0;
	}
#endif

	return size;
}
