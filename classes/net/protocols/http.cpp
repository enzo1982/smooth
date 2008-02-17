 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/net/protocols/http.h>
#include <smooth/files/file.h>
#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>
#include <smooth/io/drivers/driver_socket.h>
#include <smooth/io/drivers/driver_https.h>
#include <smooth/io/drivers/driver_socks4.h>
#include <smooth/io/drivers/driver_socks5.h>
#include <smooth/misc/math.h>
#include <smooth/misc/number.h>

#include <time.h>

S::Net::Protocols::Protocol *CreateProtocolHTTP(const S::String &iURL)
{
	return new S::Net::Protocols::HTTP(iURL);
}

S::Int	 protocolHTTPTmp = S::Net::Protocols::Protocol::AddProtocol(&CreateProtocolHTTP, "http://");

S::Net::Protocols::HTTP::HTTP(const String &iURL) : Protocol(iURL)
{
	mode	  = HTTP_METHOD_GET;
	proxyMode = HTTP_PROXY_NONE;
}

S::Net::Protocols::HTTP::~HTTP()
{
}

S::Int S::Net::Protocols::HTTP::SetHeaderField(const String &key, const String &value)
{
	Parameter	 field;

	field.key	= key;
	field.value	= value;

	fields.Add(field);

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetParameter(const String &key, const String &value)
{
	Parameter	 parameter;

	parameter.key	= key;
	parameter.value	= value;

	parameters.Add(parameter);

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetParameterFile(const String &key, const String &fileName)
{
	if (!S::File(fileName).Exists()) return Error();

	Parameter	 parameter;

	parameter.key	= key;
	parameter.value	= fileName;

	parameter.isFile= True;

	parameters.Add(parameter);

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetMode(Int nMode)
{
	mode = nMode;

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetContent(const String &nContent)
{
	content = nContent;

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetProxy(const String &nProxy, Int nProxyPort)
{
	proxy	  = nProxy;
	proxyPort = nProxyPort;

	if (proxyMode == HTTP_PROXY_NONE) proxyMode = HTTP_PROXY_HTTP;

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetProxyMode(Int nProxyMode)
{
	proxyMode = nProxyMode;

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetProxyAuth(const String &nProxyUser, const String &nProxyPass)
{
	proxyUser = nProxyUser;
	proxyPass = nProxyPass;

	return Success();
}

S::Int S::Net::Protocols::HTTP::DownloadToFile(const String &fileName)
{
	if (DecodeURL() == Error()) return Error();

	Bool		 error	= False;
	IO::Driver	*socket	= NIL;

	/* Create a connection to the server or proxy
	 */
	if	(proxyMode == HTTP_PROXY_NONE)	 socket = new IO::DriverSocket(server, port);
	else if (proxyMode == HTTP_PROXY_HTTP)	 socket = new IO::DriverSocket(proxy, proxyPort);
	else if (proxyMode == HTTP_PROXY_HTTPS)	 socket = new IO::DriverHTTPS(proxy, proxyPort, server, port, proxyUser, proxyPass);
	else if (proxyMode == HTTP_PROXY_SOCKS4) socket = new IO::DriverSOCKS4(proxy, proxyPort, server, port);
	else if (proxyMode == HTTP_PROXY_SOCKS5) socket = new IO::DriverSOCKS5(proxy, proxyPort, server, port, proxyUser, proxyPass);

	IO::InStream	*in	= new IO::InStream(IO::STREAM_DRIVER, socket);
	IO::OutStream	*out	= new IO::OutStream(IO::STREAM_STREAM, in);

	downloadProgress.Emit(0);
	downloadSpeed.Emit("");

	if (in->GetLastError() == IO::IO_ERROR_OK)
	{
		Buffer<UnsignedByte>	&buffer = ComposeHTTPRequest();

		uploadProgress.Emit(0);
		uploadSpeed.Emit("");

		Int	 startTicks	= clock();
		Int	 percent	= 0;

		for (Int i = 0; i < buffer.Size(); i += 1024)
		{
			if (!out->OutputData(((UnsignedByte *) buffer) + i, Math::Min(1024, buffer.Size() - i)))
			{
				error = True;

				break;
			}

			if (Math::Round(1000.0 * i / buffer.Size()) != percent)
			{
				percent = Math::Round(1000.0 * i / buffer.Size());

				uploadProgress.Emit(percent);
				uploadSpeed.Emit(String::FromInt(Math::Round((i / 1024) / (Float(clock() - startTicks) / 1000))).Append(" kB/s"));
			}
		}

		uploadProgress.Emit(1000);

		if (!error)
		{
			S::File(fileName).Delete();

			Int	 bytes = 0;
			String	 encoding;
			String	 str;

			do
			{
				str = in->InputLine();

				if (str.StartsWith("Transfer-Encoding: "))	encoding = str.Tail(str.Length() - 19);
				else if (str.StartsWith("Content-Length: "))	bytes = str.Tail(str.Length() - 16).ToInt();
			}
			while (str != NIL);

			while (true)
			{
				if (encoding == "chunked")
				{
					str = in->InputLine();
					bytes = (Int64) Number::FromHexString(str);

					if (bytes == 0) break;
				}

				IO::OutStream	*fOut		= new IO::OutStream(IO::STREAM_FILE, fileName, IO::OS_APPEND);
				UnsignedByte	*buffer		= new UnsignedByte [1024];
				Int		 startTicks	= clock();
				Int		 percent	= 0;

				for (Int i = 0; i < bytes; i += 1024)
				{
					fOut->OutputData(in->InputData((Void *) buffer, Math::Min(1024, bytes - i)), Math::Min(1024, bytes - i));

					if (Math::Round(1000.0 * i / bytes) != percent)
					{
						percent = Math::Round(1000.0 * i / bytes);

						downloadProgress.Emit(percent);
						downloadSpeed.Emit(String::FromInt(Math::Round((i / 1024) / (Float(clock() - startTicks) / 1000))).Append(" kB/s"));
					}
				}

				delete [] buffer;
				delete fOut;

				if (encoding == "chunked") in->InputLine();
				else			   break;
			}
		}
	}
	else
	{
		error = True;
	}

	downloadProgress.Emit(1000);

	delete in;
	delete out;
	delete socket;

	if (!error)	return Success();
	else		return Error();
}

S::Errors::Error S::Net::Protocols::HTTP::DecodeURL()
{
	if (!url.StartsWith("http://")) return Error();

	server	= "";
	path	= "";
	port	= 80;

	Int	 i, j;

	for (i = 7; i < url.Length(); i++)
	{
		if (url[i] == '/' || url[i] == ':') break;

		server[i - 7] = url[i];
	}

	if (url[i] == ':')
	{
		String	 portString;

		for (i = i + 1; i < url.Length(); i++)
		{
			if (url[i] == '/') break;

			portString[i - server.Length() - 8] = url[i];
		}

		port = portString.ToInt();
	}

	for (j = i; j < url.Length(); j++)
	{
		path[j - i] = url[j];
	}

	return Success();
}

S::Buffer<S::UnsignedByte> &S::Net::Protocols::HTTP::ComposeHTTPRequest()
{
	for (Int i = 0; i < parameters.Length(); i++)
	{
		if (parameters.GetNth(i).isFile) { mode = HTTP_METHOD_POST; break; }
	}

	if (content != NIL) mode = HTTP_METHOD_POST;

	if	(mode == HTTP_METHOD_GET)  ComposeGETRequest();
	else if (mode == HTTP_METHOD_POST) ComposePOSTRequest();

	return requestBuffer;
}

S::Void S::Net::Protocols::HTTP::ComposeGETRequest()
{
	String	 header = ComposeHeader();

	requestBuffer.Resize(header.Length());

	for (Int i = 0; i < header.Length(); i++) requestBuffer[i] = header[i];
}

S::Void S::Net::Protocols::HTTP::ComposePOSTRequest()
{
	Bool	 haveFiles = False;

	for (Int i = 0; i < parameters.Length(); i++)
	{
		if (parameters.GetNth(i).isFile) { haveFiles = True; break; }
	}

	String		 contentType;
	IO::OutStream	*out = new IO::OutStream(IO::STREAM_FILE, "httprequest.temp", IO::OS_OVERWRITE);

	if (content != NIL)
	{
		contentType = "text/plain; charset=UTF-8";

		String	 outputFormat = String::SetOutputFormat("UTF-8");

		out->OutputString(content);

		String::SetOutputFormat(outputFormat);
	}
	else if (!haveFiles)
	{
		contentType = "application/x-www-form-urlencoded";

		out->OutputString(GetParametersURLEncoded());
	}
	else
	{
		String	 separator = "THIS_STRING_SEPARATES";

		contentType = String("multipart/form-data; boundary=").Append(separator);

		out->OutputString(String("--").Append(separator).Append("\n"));
		out->OutputString("Content-Disposition: form-data; name=\"MAX_FILE_SIZE\"\n\n");
		out->OutputString("1000000\n");

		for (Int i = 0; i < parameters.Length(); i++)
		{
			out->OutputString(String("--").Append(separator).Append("\n"));

			Parameter	 parameter = parameters.GetNth(i);

			if (!parameter.isFile)
			{
				out->OutputString(String("Content-Disposition: form-data; name=\"").Append(parameter.key).Append("\"\n\n"));
				out->OutputString(String(parameter.value).Append("\n"));
			}
			else
			{
				out->OutputString(String("Content-Disposition: form-data; name=\"").Append(parameter.key).Append("\"; filename=\"").Append(parameter.value).Append("\"\n\n"));

				IO::InStream	*in = new IO::InStream(IO::STREAM_FILE, parameter.value, IO::IS_READONLY);

				for (Int j = 0; j < in->Size(); j++) out->OutputNumber(in->InputNumber(1), 1);

				delete in;

				out->OutputString("\n");
			}
		}

		out->OutputString(String("--").Append(separator).Append("--"));
	}

	delete out;

	IO::InStream	*in = new IO::InStream(IO::STREAM_FILE, "httprequest.temp", IO::IS_READONLY);

	SetHeaderField("Content-Length", String::FromInt(in->Size()));
	SetHeaderField("Content-Type", contentType);

	String	 header = ComposeHeader();

	requestBuffer.Resize(header.Length() + in->Size());

	for (Int i = 0; i < header.Length(); i++) requestBuffer[i] = header[i];
	for (Int i = 0; i < in->Size(); i++) requestBuffer[header.Length() + i] = in->InputNumber(1);

	delete in;

	S::File("httprequest.temp").Delete();
}

S::String S::Net::Protocols::HTTP::ComposeHeader()
{
	String	 str;

	if	(mode == HTTP_METHOD_GET)  str.Append("GET ");
	else if (mode == HTTP_METHOD_POST) str.Append("POST ");

	/* Prepend http:// prefix and server domain to path if an HTTP proxy is used
	 */
	if (proxyMode == HTTP_PROXY_HTTP)
	{
		str.Append("http://").Append(server);

		if (port != 80) str.Append(":").Append(String::FromInt(port));
	}

	str.Append(path);

	if (mode == HTTP_METHOD_GET && parameters.Length() > 0)
	{
		str.Append("?");
		str.Append(GetParametersURLEncoded());
	}

	str.Append(" HTTP/1.1\n");

	if (port == 80)	SetHeaderField("Host", server);
	else		SetHeaderField("Host", String(server).Append(":").Append(String::FromInt(port)));

	/* Add proxy authorization if requested
	 */
	if (proxyMode == HTTP_PROXY_HTTP && proxyUser != NIL)
	{
		SetHeaderField("Proxy-Authorization", String("Basic ").Append(String(String(proxyUser).Append(":").Append(proxyPass)).EncodeBase64()));
	}

	for (Int i = 0; i < fields.Length(); i++)
	{
		Parameter	 field = fields.GetNth(i);

		str.Append(field.key).Append(": ").Append(field.value).Append("\n");
	}

	str.Append("\n");

	return str;
}

S::String S::Net::Protocols::HTTP::GetParametersURLEncoded()
{
	String	 str;

	for (Int i = 0; i < parameters.Length(); i++)
	{
		Parameter	 parameter = parameters.GetNth(i);

		if (i > 0) str.Append("&");

		str.Append(parameter.key).Append("=").Append(parameter.value);
	}

	return str;
}
