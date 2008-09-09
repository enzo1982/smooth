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
		Int	 size = iString.wString.Size();

		wString.Resize(size);

		wcsncpy(wString, iString.wString, size);
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

	for (Int i = 0; i < nOfEntries - 32; i++)
	{
		delete [] allocatedBuffers.GetFirst();

		allocatedBuffers.RemoveNth(0);
	}
}

S::Void S::String::Clean()
{
	mutex.Lock();

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
		if (allocatedBuffers.Length() >= 64) DeleteTemporaryBuffers();

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
		if (allocatedBuffers.Length() >= 64) DeleteTemporaryBuffers();

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

	mutex.Release();

	return Success();
}

char *S::String::ConvertTo(const char *encoding) const
{
	Int	 size = Length() + 1;

	if (size == 1) return NIL;

	Int	 bufferSize = ConvertString((char *) (wchar_t *) wString, size * sizeof(wchar_t), GetInternalFormat(), NIL, 0, encoding);

	if (bufferSize == -1)	bufferSize = ConvertString((char *) (wchar_t *) wString, size * sizeof(wchar_t), GetInternalFormat(), NIL, 0, "ISO-8859-1");

	char	*buffer = NIL;

	if (bufferSize <= 0)
	{
		buffer = new char [Length() + 1];

		ConvertString((char *) (wchar_t *) wString, size * sizeof(wchar_t), GetInternalFormat(), buffer, Length() + 1, encoding);

		for (Int i = -bufferSize; i < Length(); i++) buffer[i] = '?';

		buffer[Length()] = 0;
	}
	else
	{
		buffer = new char [bufferSize + 1];

		ConvertString((char *) (wchar_t *) wString, size * sizeof(wchar_t), GetInternalFormat(), buffer, bufferSize + 1, encoding);
	}

	if (allocatedBuffers.Length() >= 64) DeleteTemporaryBuffers();

	allocatedBuffers.Add(buffer);

	return buffer;
}

wchar_t &S::String::operator [](int n)
{
	if (n + 1 >= wString.Size())
	{
		mutex.Lock();

		Int	 length = Length();

		wString.Resize(n + 2);

		wmemset(wString + length, 0, wString.Size() - length);

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
	if (n + 1 >= wString.Size()) return 0;

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
		Int	 size = newString.Length() + 1;

		mutex.Lock();

		wString.Resize(size);

		wcsncpy(wString, newString.wString, size);

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
	if (wString.Size() == 0) return 0;

	mutex.Lock();

	Int	 size = 0;

	for (Int i = 0; True; i++)
	{
		if (wString[i] == 0) break;

		size++;
	}

	mutex.Release();

	return size;
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

	mutex.Lock();

	wString.Resize(len1 + len2 + 1);

	wcsncpy(wString + len1, str.wString, len2);

	wString[len1 + len2] = 0;

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
	if (str1 == NIL) return *this;

	mutex.Lock();

	for (Int i = 0; i <= Length() - str1.Length(); i++)
	{
		Bool	 foundString = True;

		for (Int j = 0; j < str1.Length(); j++)
		{
			if (wString[i + j] != str1[j])
			{
				foundString = False;
				break;
			}
		}

		if (foundString)
		{
			if (str1.Length() != str2.Length())
			{
				if (str2.Length() > str1.Length()) wString.Resize(Length() + 1 + (str2.Length() - str1.Length()));

				wmemmove(wString + i + str2.Length(), wString + i + str1.Length(), Length() - i - str1.Length() + 1);
			}

			wcsncpy(wString + i, str2.wString, str2.Length());

			i += str2.Length() - 1;
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

const S::Array<S::String> &S::String::Explode(const String &delimiter) const
{
	static Array<String>	 array;

	array.RemoveAll();

	for (Int i = 0; i < Length(); )
	{
		Int	 index = SubString(i, Length() - i).Find(delimiter);
		String	 part;

		if (index == -1) part = SubString(i, Length() - i);
		else		 part = SubString(i, index);

		array.Add(part);

		i += part.Length() + delimiter.Length();
	}

	return array;
}

S::String S::String::Implode(const Array<String> &array, const String &delimiter)
{
	String	 string;

	for (Int i = 0; i < array.Length(); i++)
	{
		if (i > 0) string.Append(delimiter);

		string.Append(array.GetNth(i));
	}

	return string;
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
