 /* IOLib-C++, Universal IO Library
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public
  * License along with this library; if not, write to the Free
  * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  * MA 02111-1307, USA */

#ifndef _H_IOLIB_INSTREAM_
#define _H_IOLIB_INSTREAM_

class InStream;

#include "iolib-cxx.h"

const int	 IS_NORMAL	= 2;
const int	 IS_READONLY	= 3;

#define InputNumberIntel InputNumber

class

#ifdef __WIN32__
#ifdef IOLIB_DLL
	__declspec (dllexport)
#endif
#endif

InStream : public IOLibStream
{
	friend class OutStream;
	private:
		OutStream		*outStream;

		bool			 InitPBD		();
		bool			 CompletePBD		();
		bool			 ReadData		();
	public:
		long			 InputNumber		(int);
		long			 InputNumberRaw		(int);
		long			 InputNumberPDP		(int);
		long			 InputNumberPBD		(int);
		char			*InputString		(int);
		char			*InputLine		();
		void			*InputData		(void *, int);
		bool			 SetPackageSize		(int);
		bool			 SetFilter		(IOLibFilter *);
		bool			 RemoveFilter		();
		bool			 Close			();
		bool			 Seek			(long);
		bool			 RelSeek		(long);
					 InStream		(int);
					 InStream		(int, IOLibDriver *);
					 InStream		(int, const char *, int = IS_NORMAL);
					 InStream		(int, int);
					 InStream		(int, FILE *);
					 InStream		(int, void *, long);
					 InStream		(int, OutStream *);
					~InStream		();
};

#endif
