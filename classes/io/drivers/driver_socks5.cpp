 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_socks5.h>
#include <smooth/misc/math.h>

#if defined __WIN32__
#	include <windows.h>
#else
#	include <unistd.h>
#	include <sys/socket.h>
#	include <netdb.h>
#	include <netinet/in.h>
#endif

S::IO::DriverSOCKS5::DriverSOCKS5(const String &proxy, Int socksPort, const String &hostName, Int port, const String &uname, const String &passwd) : Driver()
{
	closeStream = False;

	stream	    = -1;
	size	    = -1;

	if (hostName.Length() > 255) { lastError = IO_ERROR_BADPARAM; return; }

	/* Open TCP/IP socket.
	 */
	stream = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (stream == (unsigned) (~0)) { lastError = IO_ERROR_UNEXPECTED; return; }

	/* Get proxy hostname.
	 */
	hostent		*host = gethostbyname(proxy);

	if (host == NIL) { CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

	/* Connect to proxy.
	 */
	sockaddr_in	 saddr;

	saddr.sin_family = AF_INET;
	saddr.sin_addr	 = *((in_addr *) *host->h_addr_list);
	saddr.sin_port	 = htons((short) socksPort);

	memset(&saddr.sin_zero, 0, sizeof(saddr.sin_zero));

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1) { CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

	/* Send connect request.
	 */
	if (uname == NIL)
	{
		unsigned char	*socksdata = new unsigned char [3];

		socksdata[0] = 5;	// SOCKS version 5
		socksdata[1] = 1;	// One authentication method
		socksdata[2] = 0x00;	// Method 1: No authentication

		if (send(stream, (char *) socksdata, 3, 0) < 3) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

		/* Read and evaluate response.
		 */
		int	 recbytes = 0;

		while (recbytes != 2)
		{
			int	 bytes = recv(stream, (char *) socksdata + recbytes, 2 - recbytes, 0);

			if (bytes <= 0) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

			recbytes += bytes;
		}

		/* Check if proxy requires authentication.
		 */
		if (socksdata[1] == 0xFF) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

		delete [] socksdata;
	}
	else
	{
		unsigned char	*socksdata = new unsigned char [4];

		socksdata[0] = 5;	// SOCKS version 5
		socksdata[1] = 2;	// Two authentication methods
		socksdata[2] = 0x00;	// Method 0x00: No authentication
		socksdata[3] = 0x02;	// Method 0x02: Username / password

		if (send(stream, (char *) socksdata, 4, 0) < 4) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

		/* Read and evaluate response.
		 */
		int	 recbytes = 0;

		while (recbytes != 2)
		{
			int	 bytes = recv(stream, (char *) socksdata + recbytes, 2 - recbytes, 0);

			if (bytes <= 0) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

			recbytes += bytes;
		}

		/* Check if proxy accepted authentication method.
		 */
		if (socksdata[1] == 0xFF) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

		if (socksdata[1] == 0x02)
		{
			/* Method 0x02: Username / password
			 */
			delete [] socksdata;

			socksdata = new unsigned char [3 + strlen(uname) + strlen(passwd)];

			socksdata[0] = 1;
			socksdata[1] = strlen(uname);

			for (Int i = 0; i < (Int) strlen(uname); i++) socksdata[2 + i] = uname[i];

			socksdata[2 + strlen(uname)] = strlen(passwd);

			for (Int i = 0; i < (Int) strlen(passwd); i++) socksdata[3 + strlen(uname) + i] = passwd[i];

			if (send(stream, (char *) socksdata, 3 + strlen(uname) + strlen(passwd), 0) < signed(3 + strlen(uname) + strlen(passwd))) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

			/* Read and evaluate response.
			 */
			int	 recbytes = 0;

			while (recbytes != 2)
			{
				int	 bytes = recv(stream, (char *) socksdata + recbytes, 2 - recbytes, 0);

				if (bytes <= 0) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

				recbytes += bytes;
			}

			/* Check if proxy accepted username/password.
			 */
			if (socksdata[1] != 0x00) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }
		}

		delete [] socksdata;
	}

	/* Send connect request.
	 */
	unsigned char	*socksdata = new unsigned char [5 + 255 + 2];

	socksdata[0] = 5;	// SOCKS version 5
	socksdata[1] = 0x01;	// Command: Connect
	socksdata[2] = 0x00;	// Reserved field
	socksdata[3] = 0x03;	// Connect type: Domain name
	socksdata[4] = strlen(hostName);

	for (Int i = 0; i < (Int) strlen(hostName); i++) socksdata[5 + i] = hostName[i];

	socksdata[5 + strlen(hostName)] = htons((short) port) % 256;
	socksdata[6 + strlen(hostName)] = htons((short) port) / 256;

	if (send(stream, (char *) socksdata, 7 + strlen(hostName), 0) < signed(7 + strlen(hostName))) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

	/* Receive and evaluate first 4 bytes of answer.
	 */
	int	 recbytes = 0;

	while (recbytes != 4)
	{
		int	 bytes = recv(stream, (char *) socksdata + recbytes, 4 - recbytes, 0);

		if (bytes <= 0) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

		recbytes += bytes;
	}

	/* Check if connect attempt was successful.
	 */
	if (socksdata[1] != 0x00) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

	/* Receive rest of answer.
	 */
	if (socksdata[3] == 0x01)
	{
		/* Read IP address and port.
		 */
		int	 recbytes = 0;

		while (recbytes != 6)
		{
			int	 bytes = recv(stream, (char *) socksdata + 4 + recbytes, 6 - recbytes, 0);

			if (bytes <= 0) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

			recbytes += bytes;
		}
	}
	else if (socksdata[3] == 0x03)
	{
		/* Read length of hostname.
		 */
		if (recv(stream, (char *) socksdata + 4, 1, 0) <= 0) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

		/* Read hostname and port.
		 */
		int	 neededbytes = Math::Min(255, socksdata[4]) + 2;
		int	 recbytes    = 0;

		while (recbytes != neededbytes)
		{
			int	 bytes = recv(stream, (char *) socksdata + 5 + recbytes, neededbytes - recbytes, 0);

			if (bytes <= 0) { delete [] socksdata; CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

			recbytes += bytes;
		}
	}

	delete [] socksdata;

	closeStream = True;
}

S::IO::DriverSOCKS5::~DriverSOCKS5()
{
	if (closeStream) CloseSocket();
}

S::Int S::IO::DriverSOCKS5::ReadData(UnsignedByte *data, Int dataSize)
{
	if (dataSize <= 0) return 0;

	int	 bytes = recv(stream, (char *) data, dataSize, 0);

	if (bytes <= 0) return -1;
	else		return bytes;
}

S::Int S::IO::DriverSOCKS5::WriteData(UnsignedByte *data, Int dataSize)
{
	if (dataSize <= 0) return 0;

	return send(stream, (char *) data, dataSize, 0);
}

S::Void S::IO::DriverSOCKS5::CloseSocket()
{
#if defined __WIN32__
	closesocket(stream);
#else
	close(stream);
#endif
}
