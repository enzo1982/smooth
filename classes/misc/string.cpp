/* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/string.h>
#include <smooth/misc/math.h>
#include <smooth/templates/buffer.h>
#include <smooth/threads/thread.h>

#include <iconv.h>
#include <memory.h>

#ifdef __WIN32__
#include <smooth/threads/backends/win32/mutexwin32.h>
#else
#include <smooth/threads/backends/posix/mutexposix.h>
#endif

namespace smooth
{
	Buffer<char>	 csBuffer;

	Int		 ConvertString(const char *, Int, const char *, char *, Int, const char *);
};

char	*S::String::inputFormat		= NIL;
char	*S::String::outputFormat	= NIL;

S::Int	 S::String::nOfStrings		= 0;

S::Array<char *>	 S::String::allocatedBuffers;

S::Int	 S::String::crc32_table[256];
S::Bool	 S::String::crc32_initialized = False;

S::String::String()
{
	mutex = NIL;

	LockBuffers();

	nOfStrings++;

	wString = NIL;

	Clean();

	UnlockBuffers();
}

S::String::String(const int nil)
{
	mutex = NIL;

	LockBuffers();

	nOfStrings++;

	wString = NIL;

	Clean();

	UnlockBuffers();
}

S::String::String(const char *iString)
{
	mutex = NIL;

	LockBuffers();

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

	UnlockBuffers();
}

S::String::String(const wchar_t *iString)
{
	mutex = NIL;

	LockBuffers();

	nOfStrings++;

	if (iString == NIL)
	{
		wString = NIL;

		Clean();
	}
	else
	{
		Int	 size = wcslen(iString) + 1;

		wString = new wchar_t [size];

		wcsncpy(wString, iString, size);

		stringSize = size;
	}

	UnlockBuffers();
}

S::String::String(const String &iString)
{
	mutex = NIL;

	LockBuffers();

	nOfStrings++;

	if (iString.wString == NIL)
	{
		wString = NIL;

		Clean();
	}
	else
	{
		wString = new wchar_t [iString.stringSize];

		wcsncpy(wString, iString.wString, iString.stringSize);

		stringSize = iString.stringSize;
	}

	UnlockBuffers();
}

S::String::~String()
{
	LockBuffers();

	Clean();

	if (--nOfStrings == 0)
	{
		for (Int i = 0; i < allocatedBuffers.GetNOfEntries(); i++) delete [] allocatedBuffers.GetNth(i);

		allocatedBuffers.RemoveAll();
	}

	UnlockBuffers();

	if (mutex != NIL) delete mutex;
}

S::Void S::String::LockBuffers() const
{
	if (Threads::Thread::GetNOfRunningThreads() > 0)
	{
		if (mutex == NIL)
		{
#ifdef __WIN32__
			mutex = new Threads::MutexWin32(NIL);
#else
			mutex = new Threads::MutexPOSIX(NIL);
#endif
		}

		mutex->Lock();
	}
}

S::Void S::String::UnlockBuffers() const
{
	if (Threads::Thread::GetNOfRunningThreads() > 0) mutex->Release();
}

S::Void S::String::DeleteTemporaryBuffers()
{
	Int	 nOfEntries = allocatedBuffers.GetNOfEntries();

	for (Int i = 0; i < nOfEntries - 512; i++)
	{
		delete [] allocatedBuffers.GetFirst();

		allocatedBuffers.Remove(allocatedBuffers.GetNthIndex(0));
	}
}

S::Void S::String::Clean()
{
	LockBuffers();

	stringSize = 0;

	if (wString != NIL) delete [] wString;

	wString = NIL;

	UnlockBuffers();
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

S::Int S::String::ComputeCRC32() const
{
	if (!crc32_initialized) CRC32_InitTable();

	LockBuffers();

	UnsignedLong	 ulCRC(0xffffffff);
	Int		 len = Length() * sizeof(wchar_t);
	unsigned char	*buffer = (unsigned char *) wString;

	while (len--) ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF) ^ *buffer++];

	UnlockBuffers();

	return ulCRC ^ 0xffffffff;
}

S::String S::String::EncodeBase64() const
{
	Buffer<char>	 buffer(Length() * 4);

	strcpy(buffer, ConvertTo("UTF-8"));

	return buffer.EncodeBase64(strlen(buffer));
}

S::String S::String::DecodeBase64() const
{
	Buffer<char>	 buffer;
	String		 string;

	buffer.DecodeBase64(*this);
	string.ImportFrom("UTF-8", buffer);

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
		if (allocatedBuffers.GetNOfEntries() >= 1024) DeleteTemporaryBuffers();

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
		if (allocatedBuffers.GetNOfEntries() >= 1024) DeleteTemporaryBuffers();

		allocatedBuffers.Add(previousOutputFormat);
	}

	return previousOutputFormat;
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

	Int	 size = ConvertString(str, len, format, NIL, 0, "UTF-16LE");

	if ((size < 0) && (strcmp(format, "ISO-8859-1") != 0))
	{
		return ImportFrom("ISO-8859-1", str);
	}
	else if (size < 0) return Error();

	size = size / 2 + 1;

	LockBuffers();

	wString = new wchar_t [size];

	ConvertString(str, len, format, (char *) wString, size * 2, "UTF-16LE");

	wString[size - 1] = 0;

	stringSize = size;

	UnlockBuffers();

	return Success();
}

char *S::String::ConvertTo(const char *encoding) const
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

	allocatedBuffers.Add(buffer);

	return buffer;
}

wchar_t &S::String::operator [](int n)
{
	wchar_t	*wBuffer;

	if (n >= stringSize - 1)
	{
		LockBuffers();

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

		UnlockBuffers();
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

		LockBuffers();

		Clean();

		wString = new wchar_t [size];

		wcsncpy(wString, newString, size);

		stringSize = size;

		UnlockBuffers();
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

		LockBuffers();

		Clean();

		wString = new wchar_t [backup.stringSize];

		wcsncpy(wString, backup.wString, backup.stringSize);

		stringSize = backup.stringSize;

		UnlockBuffers();
	}

	return *this;
}

S::Bool S::String::operator ==(const int nil) const
{
	if (wString == NIL)	return True;
	if (wString[0] == 0)	return True;
	else			return False;
}

S::Bool S::String::operator ==(const char *str) const
{
	if (wString == NIL && str == NIL)	return True;
	if (wString == NIL && str != NIL)	if (str[0] == 0) return True;
	if (wString != NIL && str == NIL)	if (wString[0] == 0) return True;
	if (wString == NIL || str == NIL)	return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator ==(const wchar_t *str) const
{
	if (wString == NIL && str == NIL)	return True;
	if (wString == NIL && str != NIL)	if (str[0] == 0) return True;
	if (wString != NIL && str == NIL)	if (wString[0] == 0) return True;
	if (wString == NIL || str == NIL)	return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator ==(const String &str) const
{
	if (wString == NIL && str.wString == NIL)	return True;
	if (wString == NIL && str.wString != NIL)	if (str.wString[0] == 0) return True;
	if (wString != NIL && str.wString == NIL)	if (wString[0] == 0) return True;
	if (wString == NIL || str.wString == NIL)	return False;

	if (!Compare(str))	return True;
	else			return False;
}

S::Bool S::String::operator !=(const int nil) const
{
	if (wString == NIL)	return False;
	if (wString[0] == 0)	return False;
	else			return True;
}

S::Bool S::String::operator !=(const char *str) const
{
	if (wString == NIL && str == NIL)	return False;
	if (wString == NIL && str != NIL)	if (str[0] == 0) return False;
	if (wString != NIL && str == NIL)	if (wString[0] == 0) return False;
	if (wString == NIL || str == NIL)	return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Bool S::String::operator !=(const wchar_t *str) const
{
	if (wString == NIL && str == NIL)	return False;
	if (wString == NIL && str != NIL)	if (str[0] == 0) return False;
	if (wString != NIL && str == NIL)	if (wString[0] == 0) return False;
	if (wString == NIL || str == NIL)	return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Bool S::String::operator !=(const String &str) const
{
	if (wString == NIL && str.wString == NULL)	return False;
	if (wString == NIL && str.wString != NIL)	if (str.wString[0] == 0) return False;
	if (wString != NIL && str.wString == NIL)	if (wString[0] == 0) return False;
	if (wString == NIL || str.wString == NULL)	return True;

	if (Compare(str) != 0)	return True;
	else			return False;
}

S::Int S::String::Length() const
{
	if (stringSize == 0) return 0;

	LockBuffers();

	stringSize = 0;

	for (Int i = 0; i >= 0; i++)
	{
		stringSize++;

		if (wString[i] == 0) break;
	}

	UnlockBuffers();

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

	wchar_t	*composed = new wchar_t [len1 + len2 + 1];

	LockBuffers();

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

	UnlockBuffers();

	delete [] composed;

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

	LockBuffers();

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

		if (foundString) { UnlockBuffers(); return i; }
	}

	UnlockBuffers();

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

	LockBuffers();

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

	UnlockBuffers();

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

	LockBuffers();

	Clean();

	for (Int i = n - 1; i >= 0; i--)
	{
		(*this)[i] = backup[i];
	}

	UnlockBuffers();

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
		LockBuffers();

		for (Int i = 0; i < len1; i++)
		{
			if (wString[i] != str.wString[i]) { UnlockBuffers(); return 1; }
		}

		UnlockBuffers();
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

	LockBuffers();

	for (Int i = 0; i < n; i++)
	{
		if (wString[i] != str.wString[i]) { UnlockBuffers(); return 1; }
	}

	UnlockBuffers();

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
		LockBuffers();

		for (Int i = 0; i < len2; i++)
		{
			if (wString[i] != str.wString[i]) { UnlockBuffers(); return False; }
		}

		UnlockBuffers();

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
		LockBuffers();

		for (Int i = 0; i < len2; i++)
		{
			if (wString[len1 - len2 + i] != str.wString[i]) { UnlockBuffers(); return False; }
		}

		UnlockBuffers();

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

S::String &S::String::Fill(const Int value)
{
	LockBuffers();

	for (Int i = 0; i < Length(); i++)
	{
		(*this)[i] = value;
	}

	UnlockBuffers();

	return *this;
}

S::String &S::String::FillN(const Int value, const Int count)
{
	LockBuffers();

	Clean();

	for (Int i = count - 1; i >= 0; i--)
	{
		(*this)[i] = value;
	}

	UnlockBuffers();

	return *this;
}

S::Int64 S::String::ToInt() const
{
	if (Length() == 0) return 0;

	Bool	 neg = False;
	Int	 first = 0;
	Int64	 n = 0;
	Int	 size = Length();

	LockBuffers();

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
		n += (Int64) Math::Pow(10l, size - (j - first) - 1) * (wString[j] - 48);
	}

	UnlockBuffers();

	if (!neg)	return n;
	else		return 0 - n;
}

S::Float S::String::ToFloat() const
{
	if (Length() == 0) return 0;

	Bool	 neg = False;
	Int	 first = 0;
	Float	 n = 0;
	Int	 size = Length();
	Int	 afpsize = 0;
	Int	 firstafp = 0;

	LockBuffers();

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
		n += (Int) Math::Pow(10l, size - (j - first) - 1) * (wString[j] - 48);
	}

	for (Int k = firstafp; k < (firstafp + afpsize); k++)
	{
		n += (Float) Math::Pow(10l, 0 - (k - firstafp) - 1) * (wString[k] - 48);
	}

	UnlockBuffers();

	if (!neg)	return n;
	else		return 0 - n;
}

S::String S::String::FromInt(const Int64 value)
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
			newString[i + 1] = (wchar_t) Math::Floor(((-value) % Math::Pow((Int64) 10, (Int64) sz - i - 1)) / Math::Pow((Int64) 10, (Int64) sz - (i + 1) - 1)) + 48;
		}
	}
	else
	{
		for (Int i = 0; i < sz; i++)
		{
			newString[i] = (wchar_t) Math::Floor((value % Math::Pow((Int64) 10, (Int64) sz - i)) / Math::Pow((Int64) 10, (Int64) sz - (i + 1))) + 48;
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
			newString[i + 1] = (wchar_t) Math::Floor(((Int) (-value) % Math::Pow(10l, sz - i - 1)) / Math::Pow(10l, sz - (i + 1) - 1)) + 48;
		}

		afps = FromInt((Int) (-(value - (Int) value) * Math::Pow(10l, 8)));
	}
	else
	{
		for (Int i = 0; i < sz; i++)
		{
			newString[i] = (wchar_t) Math::Floor(((Int) value % Math::Pow(10l, sz - i)) / Math::Pow(10l, sz - (i + 1))) + 48;
		}

		afps = FromInt((Int) ((value - (Int) value) * Math::Pow(10l, 8)));
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

S::String S::String::ToLower() const
{
	String	 retVal = *this;

	LockBuffers();

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

	UnlockBuffers();

	return retVal;
}

S::String S::String::ToUpper() const
{
	String	 retVal = *this;

	LockBuffers();

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

	UnlockBuffers();

	return retVal;
}

S::Int S::ConvertString(const char *inBuffer, Int inBytes, const char *inEncoding, char *outBuffer, Int outBytes, const char *outEncoding)
{
	Int	 size = 0;

	if (outBuffer == NIL)
	{
		csBuffer.Resize(inBytes * 8);
		csBuffer.Zero();

		outBytes	= csBuffer.Size();
		outBuffer	= csBuffer;
	}

	if (Setup::useIconv)
	{
		iconv_t		 cd	= iconv_open(outEncoding, inEncoding);

		if ((Int) cd == -1) return -1;

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
	else if ((strcmp(inEncoding, "UTF-16LE") == 0 && strcmp(outEncoding, "UTF-16BE") == 0) || (strcmp(inEncoding, "UTF-16BE") == 0 && strcmp(outEncoding, "UTF-16LE") == 0))
	{
		size = inBytes - 2;

		if (size < outBytes && size > 0)
		{
			for (Int i = 0; i < size / 2; i++) ((wchar_t *) outBuffer)[i] = ((((wchar_t *) inBuffer)[i] & 255) << 8) | (((wchar_t *) inBuffer)[i] >> 8);
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
		else if (inEncoding[0] == 'C' && inEncoding[1] == 'P')	codePage = String(inEncoding + 2).ToInt();

		size = MultiByteToWideChar(codePage, 0, inBuffer, -1, NIL, 0) * 2;

		// Codepage not installed? Let's try CP_ACP!
		if (size == 0 && GetLastError() == ERROR_INVALID_PARAMETER) size = MultiByteToWideChar(codePage = CP_ACP, 0, inBuffer, -1, NIL, 0) * 2;

		if (size < outBytes && size > 0) MultiByteToWideChar(codePage, 0, inBuffer, -1, (wchar_t *) outBuffer, size / 2);
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
		else if (outEncoding[0] == 'C' && outEncoding[1] == 'P')codePage = String(outEncoding + 2).ToInt();

		size = WideCharToMultiByte(codePage, 0, (wchar_t *) inBuffer, -1, NIL, 0, NIL, NIL);

		// Codepage not installed? Let's try CP_ACP!
		if (size == 0 && GetLastError() == ERROR_INVALID_PARAMETER) size = WideCharToMultiByte(codePage = CP_ACP, 0, (wchar_t *) inBuffer, -1, NIL, 0, NIL, NIL);

		if (size < outBytes && size > 0) WideCharToMultiByte(codePage, 0, (wchar_t *) inBuffer, -1, outBuffer, size, NIL, NIL);
		else if (size >= outBytes)	 size = 0;
	}
#endif

	return size;
}
