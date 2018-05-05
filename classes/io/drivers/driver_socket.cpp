 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_socket.h>

#if defined __WIN32__
#	include <windows.h>
#else
#	if defined __sun
#		include <sys/filio.h>
#	endif

#	include <unistd.h>
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#	include <netdb.h>
#	include <netinet/in.h>
#endif

S::IO::DriverSocket::DriverSocket(const String &hostName, Int port) : Driver()
{
	closeStream = False;

	stream	    = -1;
	size	    = -1;

	mode	    = MODE_SOCKET_BLOCKING;
	timeout	    = 0;

	if (hostName.Length() > 255) { lastError = IO_ERROR_BADPARAM; return; }

	/* Open TCP/IP socket.
	 */
	stream = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (stream == (unsigned) (~0)) { lastError = IO_ERROR_UNEXPECTED; return; }

	closeStream = True;

	/* Get server hostname.
	 */
	hostent		*host = gethostbyname(hostName);

	if (host == NIL) { Close(); lastError = IO_ERROR_UNEXPECTED; return; }

	/* Connect to server.
	 */
	sockaddr_in	 saddr;

	saddr.sin_family = AF_INET;
	saddr.sin_addr	 = *((in_addr *) *host->h_addr_list);
	saddr.sin_port	 = htons((short) port);

	memset(&saddr.sin_zero, 0, sizeof(saddr.sin_zero));

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1) { Close(); lastError = IO_ERROR_UNEXPECTED; return; }
}

S::IO::DriverSocket::DriverSocket(unsigned int iStream) : Driver()
{
	size	    = -1;

	stream	    = iStream;
	closeStream = False;
	timeout	    = 0;

	SetMode(MODE_SOCKET_BLOCKING);
}

S::IO::DriverSocket::~DriverSocket()
{
	Close();
}

S::Int S::IO::DriverSocket::ReadData(UnsignedByte *data, Int dataSize)
{
	if (dataSize <= 0) return 0;

	if (mode == MODE_SOCKET_BLOCKING && timeout != 0)
	{
#if defined __WIN32__
		TIMEVAL	 tv = {timeout, 0};
		FD_SET	 sock;

		FD_ZERO(&sock);
		FD_SET(stream, &sock);

		if (select(0, &sock, 0, 0, &tv) != 1) return 0;
#endif
	}

	int	 bytes = recv(stream, (char *) data, dataSize, 0);

	if (bytes <= 0) return -1;
	else		return bytes;
}

S::Int S::IO::DriverSocket::WriteData(UnsignedByte *data, Int dataSize)
{
	if (dataSize <= 0) return 0;

	if (mode == MODE_SOCKET_BLOCKING && timeout != 0)
	{
#if defined __WIN32__
		TIMEVAL	 tv = {timeout, 0};
		FD_SET	 sock;

		FD_ZERO(&sock);
		FD_SET(stream, &sock);

		if (select(0, 0, &sock, 0, &tv) != 1) return 0;
#endif
	}

	return send(stream, (char *) data, dataSize, 0);
}

S::Bool S::IO::DriverSocket::Close()
{
#if defined __WIN32__
	if (!closeStream || closesocket(stream) != 0) return False;
#else
	if (!closeStream || close(stream)	!= 0) return False;
#endif

	closeStream = False;

	return True;
}

S::Bool S::IO::DriverSocket::SetMode(Int nm)
{
	switch (nm)
	{
		default:
			return False;

		case MODE_SOCKET_BLOCKING:
		case MODE_SOCKET_NONBLOCKING:
			mode = nm;

			break;
	}

#if defined __WIN32__
	if (ioctlsocket(stream, FIONBIO, &mode) != 0) return False;
	else					      return True;
#else
	if (ioctl(stream, FIONBIO, &mode)	!= 0) return False;
	else					      return True;
#endif
}

S::Bool S::IO::DriverSocket::SetTimeout(Int nt)
{
	if (mode != MODE_SOCKET_BLOCKING) return False;
	if (timeout < 0)		  return False;

	timeout = nt;

	return True;
}
