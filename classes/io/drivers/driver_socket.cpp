 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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
#	include <unistd.h>
#	include <sys/ioctl.h>
#endif

S::IO::DriverSocket::DriverSocket(const String &hostName, Int port) : Driver()
{
	closeStream = false;

	size = -1;

	mode = MODE_SOCKET_BLOCKING;
	timeout = 0;

	stream = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (stream == (unsigned) (~0))
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	hostent		*host = gethostbyname(hostName);
	sockaddr_in	 saddr;

	if (host == NULL)
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	saddr.sin_family	= AF_INET;
	saddr.sin_addr		= *((in_addr *) *host->h_addr_list);
	saddr.sin_port		= htons((short) port);

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1)
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	closeStream = true;
}

S::IO::DriverSocket::DriverSocket(unsigned int iStream) : Driver()
{
	size = -1;

	stream		= iStream;
	closeStream	= false;
	timeout		= 0;

	SetMode(MODE_SOCKET_BLOCKING);
}

S::IO::DriverSocket::~DriverSocket()
{
	if (closeStream) CloseSocket();
}

S::Int S::IO::DriverSocket::ReadData(UnsignedByte *data, Int dataSize)
{
	if (mode == MODE_SOCKET_BLOCKING && timeout != 0)
	{
#ifdef __WIN32__
		TIMEVAL	 tv = {timeout, 0};
		FD_SET	 sock;

		FD_ZERO(&sock);
		FD_SET(stream, &sock);

		if (select(0, &sock, 0, 0, &tv) != 1) return 0;
#endif
	}

	int	 bytes = recv(stream, (char *) data, dataSize, 0);

	if (bytes == 0) return -1; // connection closed
	else		return bytes;
}

S::Int S::IO::DriverSocket::WriteData(UnsignedByte *data, Int dataSize)
{
	if (mode == MODE_SOCKET_BLOCKING && timeout != 0)
	{
#ifdef __WIN32__
		TIMEVAL	 tv = {timeout, 0};
		FD_SET	 sock;

		FD_ZERO(&sock);
		FD_SET(stream, &sock);

		if (select(0, 0, &sock, 0, &tv) != 1) return 0;
#endif
	}

	return send(stream, (char *) data, dataSize, 0);
}

S::Void S::IO::DriverSocket::CloseSocket()
{
#if defined __WIN32__ && !defined __WINE__
	closesocket(stream);
#else
	close(stream);
#endif
}

S::Bool S::IO::DriverSocket::SetMode(Int nm)
{
	switch (nm)
	{
		default:
			return false;
		case MODE_SOCKET_BLOCKING:
		case MODE_SOCKET_NONBLOCKING:
			mode = nm;
			break;
	}

#if defined __WIN32__ && !defined __WINE__
	if (ioctlsocket(stream, FIONBIO, &mode) != 0)	return false;
	else						return true;
#else
	if (ioctl(stream, FIONBIO, &mode) != 0)	return false;
	else					return true;
#endif
}

S::Bool S::IO::DriverSocket::SetTimeout(Int nt)
{
	if (mode != MODE_SOCKET_BLOCKING)	return false;
	if (timeout < 0)			return false;

	timeout = nt;

	return true;
}
