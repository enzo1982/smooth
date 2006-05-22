 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../definitions.h"

#ifndef _H_OBJSMOOTH_STRING_
#define _H_OBJSMOOTH_STRING_

namespace smooth
{
	class String;

	namespace Threads
	{
		class MutexBackend;
	};
};

#include "../templates/array.h"

namespace smooth
{
	class SMOOTHAPI String
	{
		private:
			mutable Threads::MutexBackend	*mutex;

			Void				 LockBuffers() const;
			Void				 UnlockBuffers() const;

			wchar_t				*wString;
			mutable Int			 stringSize;

			static char			*inputFormat;
			static char			*outputFormat;

			static Int			 nOfStrings;

			static Array<char *>		 allocatedBuffers;

			static Int			 crc32_table[256];
			static Bool			 crc32_initialized;

			static Void			 CRC32_InitTable();
			static UnsignedLong		 CRC32_Reflect(UnsignedLong, char);

			static Void			 DeleteTemporaryBuffers();
		public:
							 String();
							 String(const int);
							 String(const char *);
							 String(const wchar_t *);
							 String(const String &);

							~String();

			Void				 Clean();

			static Bool			 IsANSI(const String &);
			static Bool			 IsUnicode(const String &);

			static char			*SetInputFormat(const char *);
			static char			*SetOutputFormat(const char *);

			Int				 ImportFrom(const char *, const char *);
			char				*ConvertTo(const char *) const;

			Int				 ComputeCRC32() const;

			String				 EncodeBase64() const;
			String				 DecodeBase64() const;

			String				&Append(const char *);
			String				&Append(const wchar_t *);
			String				&Append(const String &);

			Int				 Find(const char *) const;
			Int				 Find(const wchar_t *) const;
			Int				 Find(const String &) const;

			String				&Replace(const char *, const char *);
			String				&Replace(const wchar_t *, const wchar_t *);
			String				&Replace(const char *, const String &);
			String				&Replace(const wchar_t *, const String &);
			String				&Replace(const String &, const String &);

			String				&Copy(const char *);
			String				&Copy(const wchar_t *);
			String				&Copy(const String &);

			String				&CopyN(const char *, const Int);
			String				&CopyN(const wchar_t *, const Int);
			String				&CopyN(const String &, const Int);

			Int				 Compare(const char *) const;
			Int				 Compare(const wchar_t *) const;
			Int				 Compare(const String &) const;

			Int				 CompareN(const char *, const Int) const;
			Int				 CompareN(const wchar_t *, const Int) const;
			Int				 CompareN(const String &, const Int) const;

			Bool				 StartsWith(const char *) const;
			Bool				 StartsWith(const wchar_t *) const;
			Bool				 StartsWith(const String &) const;

			Bool				 EndsWith(const char *) const;
			Bool				 EndsWith(const wchar_t *) const;
			Bool				 EndsWith(const String &) const;

			String				&Fill(const Int);
			String				&FillN(const Int, const Int);

			Int				 Length() const;

			Int				 ToInt() const;
			Float				 ToFloat() const;

			static String			 FromInt(const Int);
			static String			 FromFloat(const Float);

			String				 ToLower() const;
			String				 ToUpper() const;

			wchar_t &operator		 [](const int);
			wchar_t &operator		 [](const Int);

			wchar_t operator		 [](const int) const;
			wchar_t operator		 [](const Int) const;

			operator			 char *() const;
			operator			 wchar_t *() const;

			String &operator		 =(const int);
			String &operator		 =(const char *);
			String &operator		 =(const wchar_t *);
			String &operator		 =(const String &);

			Bool operator			 ==(const int) const;
			Bool operator			 ==(const char *) const;
			Bool operator			 ==(const wchar_t *) const;
			Bool operator			 ==(const String &) const;

			Bool operator			 !=(const int) const;
			Bool operator			 !=(const char *) const;
			Bool operator			 !=(const wchar_t *) const;
			Bool operator			 !=(const String &) const;
	};
};

#endif
