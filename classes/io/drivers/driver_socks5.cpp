 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_socks5.h>

#if defined __WIN32__
#	include <windows.h>
#else
#	include <unistd.h>
#endif

S::IO::DriverSOCKS5::DriverSOCKS5(const String &proxy, Int socksPort, const String &hostName, Int port, const String &uname, const String &passwd) : Driver()
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
	sockaddr_in	 saddr;

	if (host == NULL)
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	saddr.sin_family = AF_INET;
	saddr.sin_addr	 = *((in_addr *) *host->h_addr_list);
	saddr.sin_port	 = htons((short) socksPort);

	memset(&saddr.sin_zero, 0, sizeof(saddr.sin_zero));

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1)
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	int	 recbytes = 0;

	if (uname == NIL)
	{
		socksdata = new unsigned char [3];

		socksdata[0] = 5;
		socksdata[1] = 1;
		socksdata[2] = 0;

		send(stream, (char *) socksdata, 3, 0);

		while (recbytes != 2)
		{
			recbytes += recv(stream, (char *) socksdata + recbytes, 2 - recbytes, 0);
		}

		if (socksdata[1] == 255)	// proxy requires authentication
		{
			delete [] socksdata;

			CloseSocket();

			{ lastError = IO_ERROR_UNEXPECTED; return; }
		}

		delete [] socksdata;
	}
	else
	{
		socksdata = new unsigned char [4];

		socksdata[0] = 5;
		socksdata[1] = 2;
		socksdata[2] = 0;
		socksdata[3] = 2;

		send(stream, (char *) socksdata, 4, 0);

		while (recbytes != 2)
		{
			recbytes += recv(stream, (char *) socksdata + recbytes, 2 - recbytes, 0);
		}

		if (socksdata[1] == 255)	// proxy needs authentication, but doesn't support username/password
		{
			delete [] socksdata;

			CloseSocket();

			{ lastError = IO_ERROR_UNEXPECTED; return; }
		}

		if (socksdata[1] == 2)
		{
			delete [] socksdata;

			socksdata = new unsigned char [3 + strlen(uname) + strlen(passwd)];

			socksdata[0] = 1;
			socksdata[1] = strlen(uname);

			for (int i = 0; i < (int) strlen(uname); i++) socksdata[2 + i] = uname[i];

			socksdata[2 + strlen(uname)] = strlen(passwd);

			for (int j = 0; j < (int) strlen(passwd); j++) socksdata[3 + strlen(uname) + j] = passwd[j];

			send(stream, (char *) socksdata, 3 + strlen(uname) + strlen(passwd), 0);

			recbytes = 0;

			while (recbytes != 2)
			{
				recbytes += recv(stream, (char *) socksdata + recbytes, 2 - recbytes, 0);
			}

			if (socksdata[1] != 0)	// proxy rejected username/password
			{
				delete [] socksdata;

				CloseSocket();

				{ lastError = IO_ERROR_UNEXPECTED; return; }
			}
		}

		delete [] socksdata;
	}

	socksdata = new unsigned char [7 + strlen(hostName)];

	socksdata[0] = 5;
	socksdata[1] = 1;
	socksdata[2] = 0;
	socksdata[3] = 3;
	socksdata[4] = strlen(hostName);

	for (int i = 0; i < (int) strlen(hostName); i++) socksdata[5 + i] = hostName[i];

	socksdata[5 + strlen(hostName)] = htons((short) port) % 256;
	socksdata[6 + strlen(hostName)] = htons((short) port) / 256;

	send(stream, (char *) socksdata, 7 + strlen(hostName), 0);

	recbytes = 0;

	while (recbytes != 5)
	{
		recbytes += recv(stream, (char *) socksdata + recbytes, 5 - recbytes, 0);
	}

	if (socksdata[1] != 0)	// an error occurred
	{
		delete [] socksdata;

		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	if (socksdata[3] == 1)
	{
		recbytes = 0;

		while (recbytes != 5)
		{
			recbytes += recv(stream, (char *) socksdata + 5 + recbytes, 5 - recbytes, 0);
		}
	}
	else if (socksdata[3] == 3)
	{
		int	 neededbytes = socksdata[4] + 2;

		recbytes = 0;

		while (recbytes != neededbytes)
		{
			recbytes += recv(stream, (char *) socksdata + 5 + recbytes, neededbytes - recbytes, 0);
		}
	}

	delete [] socksdata;

	closeStream = true;
}

S::IO::DriverSOCKS5::~DriverSOCKS5()
{
	if (closeStream) CloseSocket();
}

S::Int S::IO::DriverSOCKS5::ReadData(UnsignedByte *data, Int dataSize)
{
	int	 bytes = recv(stream, (char *) data, dataSize, 0);

	if (bytes == 0) return -1; // connection closed
	else		return bytes;
}

S::Int S::IO::DriverSOCKS5::WriteData(UnsignedByte *data, Int dataSize)
{
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
