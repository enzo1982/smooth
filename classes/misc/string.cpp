 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_STRING_
#define __OBJSMOOTH_STRING_

#include <math.h>
#include <string.h>

#include <smooth/string.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>

int	 SMOOTHString::inputFormat		= SIF_ISO;
int	 SMOOTHString::previousInputFormat	= SIF_ISO;

SMOOTHString::SMOOTHString()
{
	bstring = NIL;
	wstring = NIL;

	Clean();
}

SMOOTHString::SMOOTHString(const int nil)
{
	bstring = NIL;
	wstring = NIL;

	Clean();
}

SMOOTHString::SMOOTHString(const char *newstring)
{
	if (newstring == NIL)
	{
		bstring = NIL;
		wstring = NIL;

		Clean();
	}
	else
	{
		bstring = NIL;
		wstring = NIL;

		ImportMBCS(SMOOTHString::inputFormat, newstring);
	}
}

SMOOTHString::SMOOTHString(const wchar_t *newstring)
{
	if (newstring == NIL)
	{
		bstring = NIL;
		wstring = NIL;

		Clean();
	}
	else
	{
		stringsize = wcslen(newstring) + 1;

		bstring = new char [stringsize];
		wstring = new wchar_t [stringsize];

		wcscpy(wstring, newstring);

		for (int i = 0; i < stringsize - 1; i++)
		{
			bstring[i] = wstring[i];
		}

		bstring[stringsize - 1] = 0;

		checkinconsistency = SMOOTH::False;
	}
}

SMOOTHString::SMOOTHString(const SMOOTHString &newstring)
{
	if (newstring.wstring == NIL)
	{
		bstring = NIL;
		wstring = NIL;

		Clean();
	}
	else
	{
		stringsize = newstring.stringsize;

		bstring = new char [stringsize];
		wstring = new wchar_t [stringsize];

		strncpy(bstring, newstring.bstring, stringsize);
		wcsncpy(wstring, newstring.wstring, stringsize);

		checkinconsistency = newstring.checkinconsistency;

		FixInconsistency();
	}
}

SMOOTHString::~SMOOTHString()
{
	Clean();
}

void SMOOTHString::Clean()
{
	if (bstring != NIL) delete [] bstring;
	if (wstring != NIL) delete [] wstring;

	bstring = NIL;
	wstring = NIL;
	stringsize = 0;
	checkinconsistency = SMOOTH::False;
}

bool SMOOTHString::SetInputFormat(int inpform)
{
	int	 backup;

	switch (inpform)
	{
		default:
		case SIF_DEFAULT:
		case SIF_ISO:
			SMOOTHString::previousInputFormat = SMOOTHString::inputFormat;
			SMOOTHString::inputFormat = SIF_ISO;
			break;
		case SIF_CURRENT:
			SMOOTHString::previousInputFormat = SMOOTHString::inputFormat;
			break;
		case SIF_PREVIOUS:
			backup = SMOOTHString::inputFormat;
			SMOOTHString::inputFormat = SMOOTHString::previousInputFormat;
			SMOOTHString::previousInputFormat = backup;
			break;
		case SIF_UTF8:
			SMOOTHString::previousInputFormat = SMOOTHString::inputFormat;
			SMOOTHString::inputFormat = SIF_UTF8;
			break;
	}

	return SMOOTH::True;
}

void SMOOTHString::FixInconsistency()
{
	if (!checkinconsistency) return;

	checkinconsistency = SMOOTH::False;

	for (int i = 0; i < stringsize; i++)
	{
		if (bstring[i] != wstring[i]) bstring[i] = wstring[i];
	}
}

void SMOOTHString::ImportMBCS(int format, const char *str)
{
	Clean();

	char	*informat = NIL;

	if (format == SIF_ISO)		informat = "ISO-8859-1";
	else if (format == SIF_UTF8)	informat = "UTF-8";

	stringsize = ConvertString(str, strlen(str), informat, NIL, 0, "UTF-16LE") / 2 + 1;

	if (stringsize == 0) return;

	bstring = new char [stringsize];
	wstring = new wchar_t [stringsize];

	ConvertString(str, strlen(str), informat, (char *) wstring, stringsize * 2, "UTF-16LE");

	for (int i = 0; i < stringsize - 1; i++)
	{
		bstring[i] = wstring[i];
	}

	bstring[stringsize - 1] = 0;
	wstring[stringsize - 1] = 0;
}

wchar_t &SMOOTHString::operator [](int n)
{
	FixInconsistency();

	char	*bbuffer;
	wchar_t	*wbuffer;

	if (n >= stringsize - 1)
	{
		if (stringsize > 0)
		{
			bbuffer = new char [stringsize];
			wbuffer = new wchar_t [stringsize];

			strncpy(bbuffer, bstring, stringsize);
			wcsncpy(wbuffer, wstring, stringsize);

			delete [] bstring;
			delete [] wstring;

			bstring = new char [n + 2];
			wstring = new wchar_t [n + 2];

			for (int i = 0; i < (n + 1); i++)
			{
				bstring[i] = 32;
				wstring[i] = 32;
			}

			bstring[n + 1] = 0;
			wstring[n + 1] = 0;

			for (int j = 0; j < stringsize - 1; j++)
			{
				bstring[j] = bbuffer[j];
				wstring[j] = wbuffer[j];
			}

			stringsize = n + 2;

			delete [] bbuffer;
			delete [] wbuffer;
		}
		else
		{
			Clean();

			bstring = new char [n + 2];
			wstring = new wchar_t [n + 2];

			for (int i = 0; i < (n + 1); i++)
			{
				bstring[i] = 32;
				wstring[i] = 32;
			}

			bstring[n + 1] = 0;
			wstring[n + 1] = 0;

			stringsize = n + 2;
		}

		bstring[n] = 0;
		wstring[n] = 0;
	}

	checkinconsistency = SMOOTH::True;

	return wstring[n];
}

SMOOTHString::operator char *()
{
	FixInconsistency();

	return bstring;
}

SMOOTHString::operator wchar_t *()
{
	FixInconsistency();

	return wstring;
}

SMOOTHString &SMOOTHString::operator =(const int nil)
{
	Clean();

	return *this;
}

SMOOTHString &SMOOTHString::operator =(const char *newstring)
{
	if (newstring == NIL)
	{
		Clean();
	}
	else
	{
		ImportMBCS(SMOOTHString::inputFormat, newstring);
	}

	return *this;
}

SMOOTHString &SMOOTHString::operator =(const wchar_t *newstring)
{
	if (newstring == NIL)
	{
		Clean();
	}
	else
	{
		Clean();

		stringsize = wcslen(newstring) + 1;

		bstring = new char [stringsize];
		wstring = new wchar_t [stringsize];

		wcscpy(wstring, newstring);

		for (int i = 0; i < stringsize - 1; i++)
		{
			bstring[i] = wstring[i];
		}

		bstring[stringsize - 1] = 0;
	}

	return *this;
}

SMOOTHString &SMOOTHString::operator =(const SMOOTHString &newstring)
{
	if (newstring.wstring == NIL)
	{
		Clean();
	}
	else
	{
		SMOOTHString	 bak(newstring);

		Clean();

		stringsize = bak.stringsize;

		bstring = new char [stringsize];
		wstring = new wchar_t [stringsize];

		strncpy(bstring, bak.bstring, stringsize);
		wcsncpy(wstring, bak.wstring, stringsize);

		checkinconsistency = newstring.checkinconsistency;

		FixInconsistency();
	}

	return *this;
}

bool SMOOTHString::operator ==(const int nil)
{
	if (wstring == NIL)	return SMOOTH::True;
	else			return SMOOTH::False;
}

bool SMOOTHString::operator ==(const char *str)
{
	if (bstring == NIL && str == NIL)	return SMOOTH::True;
	if (bstring == NIL || str == NIL)	return SMOOTH::False;

	if (!Compare(str))	return SMOOTH::True;
	else			return SMOOTH::False;
}

bool SMOOTHString::operator ==(const wchar_t *str)
{
	if (wstring == NIL && str == NIL)	return SMOOTH::True;
	if (wstring == NIL || str == NIL)	return SMOOTH::False;

	if (!Compare(str))	return SMOOTH::True;
	else			return SMOOTH::False;
}

bool SMOOTHString::operator ==(const SMOOTHString &str)
{
	if (wstring == NIL && str.wstring == NIL)	return SMOOTH::True;
	if (wstring == NIL || str.wstring == NIL)	return SMOOTH::False;

	if (!Compare(str))	return SMOOTH::True;
	else			return SMOOTH::False;
}

bool SMOOTHString::operator !=(const int nil)
{
	if (wstring == NIL)	return SMOOTH::False;
	else			return SMOOTH::True;
}

bool SMOOTHString::operator !=(const char *str)
{
	if (bstring == NIL && str == NIL)	return SMOOTH::False;
	if (bstring == NIL || str == NIL)	return SMOOTH::True;

	if (Compare(str) != 0)	return SMOOTH::True;
	else			return SMOOTH::False;
}

bool SMOOTHString::operator !=(const wchar_t *str)
{
	if (wstring == NIL && str == NIL)	return SMOOTH::False;
	if (wstring == NIL || str == NIL)	return SMOOTH::True;

	if (Compare(str) != 0)	return SMOOTH::True;
	else			return SMOOTH::False;
}

bool SMOOTHString::operator !=(const SMOOTHString &str)
{
	if (wstring == NIL && str.wstring == NULL)	return SMOOTH::False;
	if (wstring == NIL || str.wstring == NULL)	return SMOOTH::True;

	if (Compare(str) != 0)	return SMOOTH::True;
	else			return SMOOTH::False;
}

int SMOOTHString::Length()
{
	if (stringsize == 0) return 0;

	stringsize = 1;

	for (int i = 0; i >= 0; i++)
	{
		if (wstring[i] == 0) break;

		stringsize++;
	}

	return stringsize - 1;
}

SMOOTHString &SMOOTHString::Append(const char *str)
{
	SMOOTHString	 str2 = str;

	return Append(str2);
}

SMOOTHString &SMOOTHString::Append(const wchar_t *str)
{
	SMOOTHString	 str2 = str;

	return Append(str2);
}

SMOOTHString &SMOOTHString::Append(const SMOOTHString &str)
{
	int	 len1 = Length();
	int	 len2 = SMOOTHString(str).Length();

	wchar_t	*composed = new wchar_t [len1 + len2 + 1];

	for (int i = 0; i < (len1 + len2 + 1); i++)
	{
		composed[i] = 0;
	}

	for (int j = 0; j < len1; j++)
	{
		composed[j] = wstring[j];
	}

	for (int k = len1; k < (len1 + len2); k++)
	{
		composed[k] = str.wstring[k - len1];
	}

	*this = composed;

	delete [] composed;

	return *this;
}

SMOOTHString &SMOOTHString::Copy(const char *str)
{
	SMOOTHString 	 str2 = str;

	return Copy(str2);
}

SMOOTHString &SMOOTHString::Copy(const wchar_t *str)
{
	SMOOTHString 	 str2 = str;

	return Copy(str2);
}

SMOOTHString &SMOOTHString::Copy(const SMOOTHString &str)
{
	*this = str;

	return *this;
}

SMOOTHString &SMOOTHString::CopyN(const char *str, int n)
{
	SMOOTHString 	 str2 = str;

	return CopyN(str2, n);
}

SMOOTHString &SMOOTHString::CopyN(const wchar_t *str, int n)
{
	SMOOTHString 	 str2 = str;

	return CopyN(str2, n);
}

SMOOTHString &SMOOTHString::CopyN(const SMOOTHString &str, int n)
{
	SMOOTHString	 bak(str);

	Clean();

	for (int i = n - 1; i >= 0; i--)
	{
		(*this)[i] = bak[i];
	}

	return *this;
}

int SMOOTHString::Compare(const char *str)
{
	SMOOTHString 	 str2 = str;

	return Compare(str2);
}

int SMOOTHString::Compare(const wchar_t *str)
{
	SMOOTHString 	 str2 = str;

	return Compare(str2);
}

int SMOOTHString::Compare(const SMOOTHString &str)
{
	FixInconsistency();

	int	 len1 = Length();
	int	 len2 = SMOOTHString(str).Length();

	if (len1 != len2)
	{
		return len1 - len2;
	}
	else
	{
		for (int i = 0; i <= len1; i++)
		{
			if (wstring[i] != str.wstring[i]) return 1;
		}
	}

	return 0;
}

SMOOTHString &SMOOTHString::Fill(int value)
{
	for (int i = 0; i < Length(); i++)
	{
		(*this)[i] = value;
	}

	FixInconsistency();

	return *this;
}

SMOOTHString &SMOOTHString::FillN(int value, int count)
{
	Clean();

	for (int i = count - 1; i >= 0; i--)
	{
		(*this)[i] = value;
	}

	FixInconsistency();

	return *this;
}

int SMOOTHString::ToInt()
{
	bool	 neg = SMOOTH::False;
	int	 first = 0;
	int	 n = 0;
	int	 size = Length();

	for (int i = size - 1; i >= 0; i--)
	{
		if ((wstring[i] < 48 || wstring[i] > 57) && wstring[i] != 45) size = i;
	}

	if (wstring[0] == '-')
	{
		neg = SMOOTH::True;
		first = 1;
		size--;
	}

	for (int j = first; j < (first + size); j++)
	{
		n += (int) pow(10, size-(j-first)-1) * (wstring[j] - 48);
	}

	if (!neg)	return n;
	else		return 0 - n;
}

double SMOOTHString::ToDouble()
{
	bool		 neg = SMOOTH::False;
	int		 first = 0;
	long double	 n = 0;
	int		 size = Length();
	int		 afpsize = 0;
	int		 firstafp = 0;

	for (int i = size - 1; i >= 0; i--)
	{
		if ((wstring[i] < 48 || wstring[i] > 57) && wstring[i] != 45) size = i;
	}

	if (wstring[size] == '.')
	{
		firstafp = size + 1;

		for (int i = size + 1; i < Length(); i++)
		{
			if (wstring[i] >= 48 && wstring[i] <= 57)	afpsize++;
			else						break;
		}
	}

	if (wstring[0] == '-')
	{
		neg = SMOOTH::True;
		first = 1;
		size--;
	}

	for (int j = first; j < (first + size); j++)
	{
		n += (int) pow(10, size-(j-first)-1) * (wstring[j] - 48);
	}

	for (int k = firstafp; k < (firstafp + afpsize); k++)
	{
		n += (double) pow(10, 0-(k-firstafp)-1) * (wstring[k] - 48);
	}

	if (!neg)	return n;
	else		return 0 - n;
}

SMOOTHString SMOOTHString::IntToString(int value)
{
	SMOOTHString	 newstring;
	int		 sz;

	if (value == 0)		sz = 1;
	else if (value < 0)	sz = (int) log10(-value) + 2;
	else			sz = (int) log10(value) + 1;

	if (value < 0)
	{
		newstring[0] = 45;

		for (int i = 0; i < sz-1; i++)
		{
			newstring[i+1] = (wchar_t) floor(((-value) % (int) pow(10, sz - i - 1)) / pow(10, sz - (i + 1) - 1)) + 48;
		}
	}
	else
	{
		for (int i = 0; i < sz; i++)
		{
			newstring[i] = (wchar_t) floor((value % (int) pow(10, sz - i)) / pow(10, sz - (i + 1))) + 48;
		}
	}

	newstring.FixInconsistency();

	return newstring;
}

SMOOTHString SMOOTHString::DoubleToString(double value)
{
	SMOOTHString	 newstring;
	int		 sz;
	int		 afpslen;
	int		 lastnn = 0;
	SMOOTHString	 afps;

	if ((int) value == 0)	sz = 1;
	else if (value < 0)	sz = (int) log10(-value) + 1;
	else			sz = (int) log10(value) + 1;

	if (value < 0)
	{
		sz++;

		newstring[0] = 45;

		for (int i = 0; i < sz-1; i++)
		{
			newstring[i+1] = (wchar_t) floor(((int) (-value) % (int) pow(10, sz - i - 1)) / pow(10, sz - (i + 1) - 1)) + 48;
		}

		afps = SMOOTHString::IntToString((int) (-(value - (int) value) * (int) pow(10, 9)));
	}
	else
	{
		for (int i = 0; i < sz; i++)
		{
			newstring[i] = (wchar_t) floor(((int) value % (int) pow(10, sz - i)) / pow(10, sz - (i + 1))) + 48;
		}

		afps = SMOOTHString::IntToString((int) ((value - (int) value) * (int) pow(10, 9)));
	}

	afpslen = afps.Length();

	for (int i = 0; i < afpslen; i++)
	{
		if (afps[i] != 48) lastnn = i + 1;
	}

	afps.CopyN(afps, lastnn);

	lastnn = 0;

	for (int j = 0; j < afpslen; j++)
	{
		if (afps[j] != 57) lastnn = j + 1;
	}

	if (lastnn == 0)	afps.CopyN(afps, 1);
	else			afps.CopyN(afps, lastnn);

	if (lastnn != 0) afps[lastnn - 1] = afps[lastnn - 1] + 1;

	if (value - (int) value != 0)
	{
		newstring.Append(".");

		for (int i = 0; i < (9 - afpslen); i++)
		{
			newstring.Append("0");
		}

		newstring.Append(afps);
	}

	return newstring;
}

#endif
