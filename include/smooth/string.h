 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_STRING_
#define _H_OBJSMOOTH_STRING_

#define SString SMOOTHString

class SMOOTHString;

#include "system.h"

const int	 SIF_DEFAULT	= 0;
const int	 SIF_CURRENT	= 1;
const int	 SIF_PREVIOUS	= 2;
const int	 SIF_ISO	= 3;
const int	 SIF_UTF8	= 4;

class SMOOTHAPI SMOOTHString
{
	private:
		char			*bstring;
		wchar_t			*wstring;
		int			 stringsize;
		bool			 checkinconsistency;

		static int		 inputFormat;
		static int		 previousInputFormat;

		void			 FixInconsistency();
		void			 ImportMBCS(int, const char *);
	public:
					 SMOOTHString();
					 SMOOTHString(const int);
					 SMOOTHString(const char *);
					 SMOOTHString(const wchar_t *);
					 SMOOTHString(const SMOOTHString &);

					~SMOOTHString();

		void			 Clean();

		static bool		 SetInputFormat(int);

		SMOOTHString		&Append(const char *);
		SMOOTHString		&Append(const wchar_t *);
		SMOOTHString		&Append(const SMOOTHString &);

		SMOOTHString		&Copy(const char *);
		SMOOTHString		&Copy(const wchar_t *);
		SMOOTHString		&Copy(const SMOOTHString &);

		SMOOTHString		&CopyN(const char *, int);
		SMOOTHString		&CopyN(const wchar_t *, int);
		SMOOTHString		&CopyN(const SMOOTHString &, int);

		int			 Compare(const char *);
		int			 Compare(const wchar_t *);
		int			 Compare(const SMOOTHString &);

		SMOOTHString		&Fill(int);
		SMOOTHString		&FillN(int, int);

		int			 Length();

		int			 ToInt();
		double			 ToDouble();

		wchar_t &operator	 [](int);

		operator		 char *();
		operator		 wchar_t *();

		SMOOTHString &operator	 =(const int);
		SMOOTHString &operator	 =(const char *);
		SMOOTHString &operator	 =(const wchar_t *);
		SMOOTHString &operator	 =(const SMOOTHString &);

		bool operator		 ==(const int);
		bool operator		 ==(const char *);
		bool operator		 ==(const wchar_t *);
		bool operator		 ==(const SMOOTHString &);

		bool operator		 !=(const int);
		bool operator		 !=(const char *);
		bool operator		 !=(const wchar_t *);
		bool operator		 !=(const SMOOTHString &);

		static SMOOTHString	 IntToString(int);
		static SMOOTHString	 DoubleToString(double);
};

#endif
