 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_socks4.h>
#include <math.h>
#include <string.h>

#if !defined __WIN32__
#	include <unistd.h>
#	include <arpa/inet.h>
#endif

S::IO::DriverSOCKS4::DriverSOCKS4(const String &proxy, Int socksPort, const String &hostName, Int port) : Driver()
{
	closeStream = false;

	size = -1;

	unsigned char	*socksdata;

	stream = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (stream == (unsigned) (~0))
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	hostent		*host = gethostbyname(proxy);
	hostent		*server_hostent = gethostbyname(hostName);
	sockaddr_in	 saddr;

	if (host == NULL)
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	saddr.sin_family	= AF_INET;
	saddr.sin_addr		= *((in_addr *) *host->h_addr_list);
	saddr.sin_port		= htons((short) socksPort);

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1)
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	if (server_hostent != NULL)
	{
		socksdata = new unsigned char [9];

		socksdata[0] = 4;
		socksdata[1] = 1;
		socksdata[2] = htons((short) port) % 256;
		socksdata[3] = htons((short) port) / 256;
		socksdata[4] = server_hostent->h_addr_list[0][0];
		socksdata[5] = server_hostent->h_addr_list[0][1];
		socksdata[6] = server_hostent->h_addr_list[0][2];
		socksdata[7] = server_hostent->h_addr_list[0][3];
		socksdata[8] = 0;

		send(stream, (char *) socksdata, 9, 0);
	}
	else if (inet_addr(hostName) != INADDR_NONE)
	{
		socksdata = new unsigned char [9];

		socksdata[0] = 4;
		socksdata[1] = 1;
		socksdata[2] = htons((short) port) % 256;
		socksdata[3] = htons((short) port) / 256;
		socksdata[4] = IOGetByte(inet_addr(hostName), 0);
		socksdata[5] = IOGetByte(inet_addr(hostName), 1);
		socksdata[6] = IOGetByte(inet_addr(hostName), 2);
		socksdata[7] = IOGetByte(inet_addr(hostName), 3);
		socksdata[8] = 0;

		send(stream, (char *) socksdata, 9, 0);
	}
	else
	{
		socksdata = new unsigned char [10 + strlen(hostName)];

		socksdata[0] = 4;
		socksdata[1] = 1;
		socksdata[2] = htons((short) port) % 256;
		socksdata[3] = htons((short) port) / 256;
		socksdata[4] = 0;
		socksdata[5] = 0;
		socksdata[6] = 0;
		socksdata[7] = 1;
		socksdata[8] = 0;

		for (Int i = 0; i < (Int) strlen(hostName); i++) socksdata[9 + i] = hostName[i];

		socksdata[9 + strlen(hostName)] = 0;

		send(stream, (char *) socksdata, 10 + strlen(hostName), 0);
	}

	int	 recbytes = 0;

	while (recbytes != 8)
	{
		recbytes += recv(stream, (char *) socksdata + recbytes, 8 - recbytes, 0);
	}

	if (socksdata[1] != 90)	// proxy rejected request
	{
		delete [] socksdata;

		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	delete [] socksdata;

	closeStream = true;
}

S::IO::DriverSOCKS4::~DriverSOCKS4()
{
	if (closeStream) CloseSocket();
}

S::Int S::IO::DriverSOCKS4::ReadData(UnsignedByte *data, Int dataSize)
{
	return recv(stream, (char *) data, dataSize, 0);
}

S::Int S::IO::DriverSOCKS4::WriteData(UnsignedByte *data, Int dataSize)
{
	return send(stream, (char *) data, dataSize, 0);
}

S::Void S::IO::DriverSOCKS4::CloseSocket()
{
#ifdef __WIN32__
	closesocket(stream);
#else
	close(stream);
#endif
}
