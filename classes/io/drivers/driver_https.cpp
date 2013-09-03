 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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
	saddr.sin_port	 = htons((short) httpPort);

	memset(&saddr.sin_zero, 0, sizeof(saddr.sin_zero));

	if (connect(stream, (sockaddr *) &saddr, sizeof(struct sockaddr)) == -1) { CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

	/* Send connect request.
	 */
	String	 connect = String("CONNECT ").Append(hostName).Append(":").Append(String::FromInt(port)).Append(" HTTP/1.1\n");

	if (uname != NIL) connect.Append("Proxy-authentication: Basic ").Append(String(String(uname).Append(":").Append(passwd)).EncodeBase64()).Append("\n");

	connect.Append("\n");

	if (send(stream, (char *) connect, connect.Length(), 0) < connect.Length()) { CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

	/* Receive answer.
	 */
	String	 answer;
	char	 c[2] = { 0, 0 };

	while (!(answer.EndsWith("\n\n") || answer.EndsWith("\r\n\r\n")))
	{
		if (recv(stream, c, 1, 0) <= 0) { CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

		answer.Append(c);
	}

	/* Check if connect attempt was successful.
	 */
	if (!answer.StartsWith("HTTP/1.1 200")) { CloseSocket(); lastError = IO_ERROR_UNEXPECTED; return; }

	closeStream = True;
}

S::IO::DriverHTTPS::~DriverHTTPS()
{
	if (closeStream) CloseSocket();
}

S::Int S::IO::DriverHTTPS::ReadData(UnsignedByte *data, Int dataSize)
{
	int	 bytes = recv(stream, (char *) data, dataSize, 0);

	if (bytes <= 0) return -1;
	else		return bytes;
}

S::Int S::IO::DriverHTTPS::WriteData(UnsignedByte *data, Int dataSize)
{
	return send(stream, (char *) data, dataSize, 0);
}

S::Void S::IO::DriverHTTPS::CloseSocket()
{
#if defined __WIN32__
	closesocket(stream);
#else
	close(stream);
#endif
}
