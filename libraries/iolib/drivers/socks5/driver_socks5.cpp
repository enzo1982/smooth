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

#include "driver_socks5.h"
#include <math.h>

int IOLibDriverSOCKS5::initialized = 0;

IOLibDriverSOCKS5::IOLibDriverSOCKS5(const char *proxy, int socksPort, const char *hostName, int port, const char *uname, const char *passwd) : IOLibDriver()
{
	InitNetworking();

	closeStream = false;

	size = -1;

	unsigned char	*socksdata;

	stream = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (stream == (SOCKET)(~0))
	{
		CloseSocket();

		{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
	}

	hostent		*host = gethostbyname(proxy);
	sockaddr_in	 saddr;

	if (host == NULL)
	{
		CloseSocket();

		{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
	}

	saddr.sin_family	= AF_INET;
	saddr.sin_addr		= *((in_addr *) *host->h_addr_list);
	saddr.sin_port		= htons((short) socksPort);

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1)
	{
		CloseSocket();

		{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
	}

	int	 recbytes = 0;
	int	 neededbytes;

	if (uname == NULL)
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

			{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
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

			{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
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

				{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
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

		{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
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
		recbytes	= 0;
		neededbytes	= socksdata[4] + 2;

		while (recbytes != neededbytes)
		{
			recbytes += recv(stream, (char *) socksdata + 5 + recbytes, neededbytes - recbytes, 0);
		}
	}

	delete [] socksdata;

	closeStream = true;
}

IOLibDriverSOCKS5::~IOLibDriverSOCKS5()
{
	if (closeStream) CloseSocket();

	DeinitNetworking();
}

int IOLibDriverSOCKS5::ReadData(unsigned char *data, int dataSize)
{
	return recv(stream, (char *) data, dataSize, 0);
}

int IOLibDriverSOCKS5::WriteData(unsigned char *data, int dataSize)
{
	return send(stream, (char *) data, dataSize, 0);
}

void IOLibDriverSOCKS5::CloseSocket()
{
#ifdef __WIN32__
	closesocket(stream);
#else
	close(stream);
#endif
}

unsigned long IOLibDriverSOCKS5::GetIPAddress(const char *host)
{
	unsigned long	 ipaddr = 0;
	int		 part1chars = 0;
	int		 part2chars = 0;
	int		 part3chars = 0;
	int		 part4chars = 0;
	int		 part1 = 0;
	int		 part2 = 0;
	int		 part3 = 0;
	int		 part4 = 0;

	if (strlen(host) >= 7)
	{
		for (int i = 0; i < 3; i++)
		{
			if (host[i] >= 48 && host[i] <= 57) part1chars++;
			else break;
		}
	}

	if (strlen(host) >= (unsigned int) part1chars + 6)
	{
		if (host[part1chars] == '.')
		{
			for (int i = part1chars + 1; i < part1chars + 4; i++)
			{
				if (host[i] >= 48 && host[i] <= 57) part2chars++;
				else break;
			}
		}
	}

	if (strlen(host) >= (unsigned int) part2chars + (unsigned int) part2chars + 5)
	{
		if (host[part1chars + part2chars + 1] == '.')
		{
			for (int i = part1chars + part2chars + 2; i < part1chars + part2chars + 5; i++)
			{
				if (host[i] >= 48 && host[i] <= 57) part3chars++;
				else break;
			}
		}
	}

	if (strlen(host) >= (unsigned int) part1chars + (unsigned int) part2chars + (unsigned int) part3chars + 4)
	{
		if (host[part1chars + part2chars + part3chars + 2] == '.')
		{
			for (int i = part1chars + part2chars + part3chars + 3; i < part1chars + part2chars + part3chars + 6; i++)
			{
				if (host[i] == 0) break;

				if (host[i] >= 48 && host[i] <= 57) part4chars++;
				else break;
			}
		}
	}

	if (part1chars > 0 && part2chars > 0 && part3chars > 0 && part4chars > 0)
	{
		for (int i = 0; i < part1chars; i++)	part1 += (int) pow(10, (part1chars - 1) - i) * (host[i] - 48);
		for (int j = 0; j < part2chars; j++)	part2 += (int) pow(10, (part2chars - 1) - j) * (host[part1chars + j + 1] - 48);
		for (int k = 0; k < part3chars; k++)	part3 += (int) pow(10, (part3chars - 1) - k) * (host[part1chars + part2chars + k + 2] - 48);
		for (int l = 0; l < part4chars; l++)	part4 += (int) pow(10, (part4chars - 1) - l) * (host[part1chars + part2chars + part3chars + l + 3] - 48);
	}

	if (part1 >= 0 && part1 <= 255 && part2 >= 0 && part2 <= 255 && part3 >= 0 && part3 <= 255 && part4 >= 0 && part4 <= 255)
	{
		ipaddr = ntohl(part1 + 256 * part2 + 65536 * part3 + 16777216 * part4);
	}

	return ipaddr;
}

bool IOLibDriverSOCKS5::InitNetworking()
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

bool IOLibDriverSOCKS5::DeinitNetworking()
{
#if defined __WIN32__ && !defined __CYGWIN32__
	initialized--;

	if (initialized == 0) WSACleanup();

	return true;
#else
	return true;
#endif
}
