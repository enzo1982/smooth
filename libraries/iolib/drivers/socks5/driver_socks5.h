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

#ifndef _H_IOLIB_DRIVER_SOCKS5_
#define _H_IOLIB_DRIVER_SOCKS5_

class IOLibDriverSOCKS5;

#include <iolib/iolibdriver-cxx.h>

#if defined __WIN32__ && !defined __CYGWIN32__
	#include <winsock2.h>
#else
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <netdb.h>
#endif

class

#ifdef __WIN32__
#ifdef IOLIB_DLL
	__declspec (dllexport)
#endif
#endif

IOLibDriverSOCKS5 : public IOLibDriver
{
	private:
		SOCKET		 stream;
		bool		 closeStream;

		static int	 initialized;

		void		 CloseSocket();
		unsigned long	 GetIPAddress(const char *);
	public:
				 IOLibDriverSOCKS5(const char *, int, const char *, int, const char *uname = NULL, const char *passwd = NULL);
				~IOLibDriverSOCKS5();

		int		 ReadData(unsigned char *, int);
		int		 WriteData(unsigned char *, int);

		static bool	 InitNetworking();
		static bool	 DeinitNetworking();
};

#endif
