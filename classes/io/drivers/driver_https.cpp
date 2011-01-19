 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_https.h>
#include <math.h>

#if defined __WIN32__
#	include <windows.h>
#else
#	include <unistd.h>
#endif

S::IO::DriverHTTPS::DriverHTTPS(const String &proxy, Int httpPort, const String &hostName, Int port, const String &uname, const String &passwd) : Driver()
{
	closeStream = false;

	size = -1;

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

	saddr.sin_family	= AF_INET;
	saddr.sin_addr		= *((in_addr *) *host->h_addr_list);
	saddr.sin_port		= htons((short) httpPort);

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1)
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	String	 connect = String("CONNECT ").Append(hostName).Append(":").Append(String::FromInt(port)).Append(" HTTP/1.1\n");

	if (uname != "") connect.Append("Proxy-authentication: Basic ").Append(String(String(uname).Append(":").Append(passwd)).EncodeBase64()).Append("\n");

	connect.Append("\n");

	send(stream, (char *) connect, connect.Length(), 0);

	String	 answer = "";
	char	 c[2] = { 0, 0 };

	while (!(answer.EndsWith("\n\n") || answer.EndsWith("\r\n\r\n"))) { recv(stream, c, 1, 0); answer.Append(c); }

	if (!answer.StartsWith("HTTP/1.1 200")) // connect attempt unsuccessful
	{
		CloseSocket();

		{ lastError = IO_ERROR_UNEXPECTED; return; }
	}

	closeStream = true;
}

S::IO::DriverHTTPS::~DriverHTTPS()
{
	if (closeStream) CloseSocket();
}

S::Int S::IO::DriverHTTPS::ReadData(UnsignedByte *data, Int dataSize)
{
	return recv(stream, (char *) data, dataSize, 0);
}

S::Int S::IO::DriverHTTPS::WriteData(UnsignedByte *data, Int dataSize)
{
	return send(stream, (char *) data, dataSize, 0);
}

S::Void S::IO::DriverHTTPS::CloseSocket()
{
#if defined __WIN32__ && !defined __WINE__
	closesocket(stream);
#else
	close(stream);
#endif
}
