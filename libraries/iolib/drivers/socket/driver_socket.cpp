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

#include "driver_socket.h"

int IOLibDriverSocket::initialized = 0;

IOLibDriverSocket::IOLibDriverSocket(const char *hostName, int port) : IOLibDriver()
{
	InitNetworking();

	closeStream = false;

	size = -1;

	mode = MODE_SOCKET_BLOCKING;
	timeout = 0;

	stream = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (stream == (SOCKET)(~0))
	{
		CloseSocket();

		{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
	}

	hostent		*host = gethostbyname(hostName);
	sockaddr_in	 saddr;

	if (host == NULL)
	{
		CloseSocket();

		{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
	}

	saddr.sin_family	= AF_INET;
	saddr.sin_addr		= *((in_addr *) *host->h_addr_list);
	saddr.sin_port		= htons((short) port);

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1)
	{
		CloseSocket();

		{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
	}

	closeStream = true;
}

IOLibDriverSocket::IOLibDriverSocket(SOCKET iStream) : IOLibDriver()
{
	size = -1;

	stream		= iStream;
	closeStream	= false;
	timeout		= 0;

	SetMode(MODE_SOCKET_BLOCKING);
}

IOLibDriverSocket::~IOLibDriverSocket()
{
	if (closeStream) CloseSocket();

	DeinitNetworking();
}

int IOLibDriverSocket::ReadData(unsigned char *data, int dataSize)
{
	if (mode == MODE_SOCKET_BLOCKING && timeout != 0)
	{
		TIMEVAL	 tv = {timeout, 0};
		FD_SET	 sock;

		FD_ZERO(&sock);
		FD_SET(stream, &sock);

		if (select(0, &sock, 0, 0, &tv) != 1) return 0;
	}

	return recv(stream, (char *) data, dataSize, 0);
}

int IOLibDriverSocket::WriteData(unsigned char *data, int dataSize)
{
	if (mode == MODE_SOCKET_BLOCKING && timeout != 0)
	{
		TIMEVAL	 tv = {timeout, 0};
		FD_SET	 sock;

		FD_ZERO(&sock);
		FD_SET(stream, &sock);

		if (select(0, 0, &sock, 0, &tv) != 1) return 0;
	}

	return send(stream, (char *) data, dataSize, 0);
}

void IOLibDriverSocket::CloseSocket()
{
#ifdef __WIN32__
	closesocket(stream);
#else
	close(stream);
#endif
}

bool IOLibDriverSocket::SetMode(int nm)
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

#ifdef __WIN32__
	if (ioctlsocket(stream, FIONBIO, &mode) != 0)	return false;
	else						return true;
#else
	if (ioctl(stream, FIONBIO, &mode) != 0)	return false;
	else					return true;
#endif
}

bool IOLibDriverSocket::SetTimeout(int nt)
{
	if (mode != MODE_SOCKET_BLOCKING)	return false;
	if (timeout < 0)			return false;

	timeout = nt;

	return true;
}

bool IOLibDriverSocket::InitNetworking()
{
#if defined __WIN32__ && !defined __CYGWIN32__
	WORD	 wVersionRequested = MAKEWORD(1,1);
	WSADATA	 wsaData;

	if (initialized == 0)
	{
		WSAStartup(wVersionRequested, &wsaData);

		if (wsaData.wVersion != wVersionRequested) return false;
	}

	initialized++;

	return true;
#else
	return true;
#endif
}

bool IOLibDriverSocket::DeinitNetworking()
{
#if defined __WIN32__ && !defined __CYGWIN32__
	initialized--;

	if (initialized == 0) WSACleanup();

	return true;
#else
	return true;
#endif
}
