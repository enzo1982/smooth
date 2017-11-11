 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_THREADLOCAL
#define H_OBJSMOOTH_THREADLOCAL

#include "../../definitions.h"

#if !defined __WIN32__ && !defined __APPLE__ && !defined __OpenBSD__ && !defined __HAIKU__
#	define multithread(t) __thread t
#else
#	if defined __WIN32__
#		ifndef WINAPI
#			define WINAPI __stdcall
#		endif

#		ifndef WINBASEAPI
#			define WINBASEAPI __declspec (dllimport)
#		endif

		typedef int	      WINBOOL;
		typedef unsigned long DWORD;
		typedef void *	      LPVOID;

		extern "C"
		{
			WINBASEAPI DWORD   WINAPI TlsAlloc();
			WINBASEAPI LPVOID  WINAPI TlsGetValue(DWORD dwTlsIndex);
			WINBASEAPI WINBOOL WINAPI TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);
			WINBASEAPI WINBOOL WINAPI TlsFree(DWORD dwTlsIndex);
		}
#	else
#		include <pthread.h>
#	endif

	namespace smooth
	{
		template <class t> class ThreadLocal
		{
#	if defined __WIN32__
			private:
				DWORD				 key;
			public:
								 ThreadLocal()			{ key = TlsAlloc(); }
								 ThreadLocal(const t &value)	{ key = TlsAlloc(); *this = value; }

								~ThreadLocal()			{ TlsFree(key); }

				inline operator			 t() const			{ return (t) TlsGetValue(key); }

				ThreadLocal<t> &operator	 =(const t &value)		{ TlsSetValue(key, (void *) value); return *this; }
#	else
			private:
				pthread_key_t			 key;
			public:
								 ThreadLocal()			{ pthread_key_create(&key, NIL); }
								 ThreadLocal(const t &value)	{ pthread_key_create(&key, NIL); *this = value; }

								~ThreadLocal()			{ pthread_key_delete(key); }

				inline operator			 t() const			{ return (t) pthread_getspecific(key); }

				ThreadLocal<t> &operator	 =(const t &value)		{ pthread_setspecific(key, (const void *) value); return *this; }
#	endif
				ThreadLocal<t>  operator	 +(const t &value) const	{ return t(*this) + value; }
				ThreadLocal<t>  operator	 -(const t &value) const	{ return t(*this) - value; }
				ThreadLocal<t>  operator	 *(const t &value) const	{ return t(*this) * value; }
				ThreadLocal<t>  operator	 /(const t &value) const	{ return t(*this) / value; }

				ThreadLocal<t>  operator	 <<(const t &value) const	{ return t(*this) << value; }
				ThreadLocal<t>  operator	 >>(const t &value) const	{ return t(*this) >> value; }

				ThreadLocal<t> &operator	 +=(const t &value)		{ return *this = t(*this) + value; }
				ThreadLocal<t> &operator	 -=(const t &value)		{ return *this = t(*this) - value; }
				ThreadLocal<t> &operator	 *=(const t &value)		{ return *this = t(*this) * value; }
				ThreadLocal<t> &operator	 /=(const t &value)		{ return *this = t(*this) / value; }

				ThreadLocal<t> &operator	 <<=(int value)			{ return *this = t(*this) << value; }
				ThreadLocal<t> &operator	 >>=(int value)			{ return *this = t(*this) >> value; }

				ThreadLocal<t> &operator	 ++()				{ return *this = t(*this) + 1; }
				ThreadLocal<t> &operator	 --()				{ return *this = t(*this) - 1; }

				t		operator	 ++(int)			{ t r = *this; ++(*this); return r; }
				t		operator	 --(int)			{ t r = *this; --(*this); return r; }

				inline t operator		 ->()				{ return t(*this); }
				inline const t operator		 ->() const			{ return t(*this); }

				inline Bool operator		 ==(const t &value) const	{ return t(*this) == value; }
				inline Bool operator		 !=(const t &value) const	{ return t(*this) != value; }
		};
	};

#	define multithread(t) ThreadLocal<t>
#endif

#endif
