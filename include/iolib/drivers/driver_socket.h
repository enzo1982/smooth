 /* IOLib-C++, Universal IO Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
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

#ifndef _H_IOLIB_DRIVER_SOCKET_
#define _H_IOLIB_DRIVER_SOCKET_

class IOLibDriverSOCKET;

#include <iolib/iolibdriver-cxx.h>

#if defined __WIN32__ && !defined __CYGWIN32__
	#include <winsock.h>
#else
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <netdb.h>
#endif

const unsigned long MODE_SOCKET_BLOCKING	= 0;
const unsigned long MODE_SOCKET_NONBLOCKING	= 1;

class

#ifdef __WIN32__
#ifdef IOLIB_DLL
	__declspec (dllexport)
#endif
#endif

IOLibDriverSocket : public IOLibDriver
{
	private:
		SOCKET		 stream;
		bool		 closeStream;

		static int	 initialized;

		unsigned long	 mode;
		int		 timeout;

		void		 CloseSocket();

		static bool	 InitNetworking();
		static bool	 DeinitNetworking();
	public:
				 IOLibDriverSocket(const char *, int);
				 IOLibDriverSocket(SOCKET);
				~IOLibDriverSocket();

		int		 ReadData(unsigned char *, int);
		int		 WriteData(unsigned char *, int);

		bool		 SetMode(int);
		bool		 SetTimeout(int);
};

#endif
