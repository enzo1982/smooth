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

#include "driver_socks4.h"
#include <math.h>

int IOLibDriverSOCKS4::initialized = 0;

IOLibDriverSOCKS4::IOLibDriverSOCKS4(const char *proxy, int socksPort, const char *hostName, int port) : IOLibDriver()
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
	hostent		*server_hostent = gethostbyname(hostName);
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
	else if (GetIPAddress(hostName) != 0)
	{
		socksdata = new unsigned char [9];

		socksdata[0] = 4;
		socksdata[1] = 1;
		socksdata[2] = htons((short) port) % 256;
		socksdata[3] = htons((short) port) / 256;
		socksdata[4] = GetByte(htonl(GetIPAddress(hostName)), 0);
		socksdata[5] = GetByte(htonl(GetIPAddress(hostName)), 1);
		socksdata[6] = GetByte(htonl(GetIPAddress(hostName)), 2);
		socksdata[7] = GetByte(htonl(GetIPAddress(hostName)), 3);
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

		for (int i = 0; i < (int) strlen(hostName); i++) socksdata[9 + i] = hostName[i];

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

		{ lastError = IOLIB_ERROR_UNEXPECTED; return; }
	}

	delete [] socksdata;

	closeStream = true;
}

IOLibDriverSOCKS4::~IOLibDriverSOCKS4()
{
	if (closeStream) CloseSocket();

	DeinitNetworking();
}

int IOLibDriverSOCKS4::ReadData(unsigned char *data, int dataSize)
{
	return recv(stream, (char *) data, dataSize, 0);
}

int IOLibDriverSOCKS4::WriteData(unsigned char *data, int dataSize)
{
	return send(stream, (char *) data, dataSize, 0);
}

void IOLibDriverSOCKS4::CloseSocket()
{
#ifdef __WIN32__
	closesocket(stream);
#else
	close(stream);
#endif
}

unsigned long IOLibDriverSOCKS4::GetIPAddress(const char *host)
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

bool IOLibDriverSOCKS4::InitNetworking()
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

bool IOLibDriverSOCKS4::DeinitNetworking()
{
#if defined __WIN32__ && !defined __CYGWIN32__
	initialized--;

	if (initialized == 0) WSACleanup();

	return true;
#else
	return true;
#endif
}
