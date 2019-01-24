 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/system/system.h>
#include <smooth/foreach.h>
#include <smooth/version.h>

#include <curl/curl.h>

namespace smooth
{
	namespace Net
	{
		namespace Protocols
		{
			size_t	 httpHeader(char *, size_t, size_t, void *);
			size_t	 httpWrite(char *, size_t, size_t, void *);
			int	 httpProgress(void *, double, double, double, double);
		};
	};
};

S::Net::Protocols::Protocol *CreateProtocolHTTP(const S::String &iURL)
{
	return new S::Net::Protocols::HTTP(iURL);
}

S::Int	 protocolHTTPTmp  = S::Net::Protocols::Protocol::AddProtocol(&CreateProtocolHTTP, L"http://");
S::Int	 protocolHTTPSTmp = S::Net::Protocols::Protocol::AddProtocol(&CreateProtocolHTTP, L"https://");

S::Net::Protocols::HTTP::HTTP(const String &iURL) : Protocol(iURL)
{
	mode	  = HTTP_METHOD_GET;

	proxyMode = HTTP_PROXY_NONE;
	proxyPort = 0;

	out	  = NIL;

	ulStart	  = 0;
	dlStart	  = 0;
}

S::Net::Protocols::HTTP::~HTTP()
{
}

S::Int S::Net::Protocols::HTTP::SetHeaderField(const String &key, const String &value)
{
	Parameter	 field;

	field.key	= key;
	field.value	= value;

	requestFields.Add(field);

	return Success();
}

S::String S::Net::Protocols::HTTP::GetHeaderField(const String &key) const
{
	foreach (const Parameter &field, requestFields)
	{
		if (field.key == key) return field.value;
	}

	return NIL;
}

S::Int S::Net::Protocols::HTTP::SetParameter(const String &key, const String &value)
{
	Parameter	 parameter;

	parameter.key	= key;
	parameter.value	= value;

	requestParameters.Add(parameter);

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetParameterFile(const String &key, const String &fileName)
{
	if (!S::File(fileName).Exists()) return Error();

	Parameter	 parameter;

	parameter.key	= key;
	parameter.value	= fileName;

	parameter.isFile= True;

	requestParameters.Add(parameter);

	return Success();
}

S::Int S::Net::Protocols::HTTP::SetMode(Short nMode)
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

S::Int S::Net::Protocols::HTTP::SetProxyMode(Short nProxyMode)
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

S::String S::Net::Protocols::HTTP::GetResponseHeaderField(const String &key) const
{
	foreach (const Parameter &field, responseFields)
	{
		if (field.key == key) return field.value;
	}

	return NIL;
}

S::Int S::Net::Protocols::HTTP::DownloadToFile(const String &fileName)
{
	/* Create a cURL context.
	 */
	Bool	 error = False;
	CURL	*curl  = curl_easy_init();

	if (!curl) return Error();

	/* Set HTTP method.
	 */
	for (Int i = 0; i < requestParameters.Length(); i++)
	{
		if (requestParameters.GetNth(i).isFile) { mode = HTTP_METHOD_POST; break; }
	}

	if (content != NIL) mode = HTTP_METHOD_POST;

	if	(mode == HTTP_METHOD_POST) curl_easy_setopt(curl, CURLOPT_POST, 1L);
	else if (mode == HTTP_METHOD_GET)  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

	/* Set parameters.
	 */
	String		 url	 = this->url;
	curl_slist	*headers = NULL;

	if (requestParameters.Length() > 0)
	{
		if	(mode == HTTP_METHOD_GET)  url.Append("?").Append(GetParametersURLEncoded());
		else if (mode == HTTP_METHOD_POST) curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *) GetParametersURLEncoded());
	}

	/* Set headers.
	 */
	foreach (const Parameter &field, requestFields) headers = curl_slist_append(headers, String(field.key).Append(": ").Append(field.value));

	/* Check to see if we are to transfer any files.
	 */
	Bool	 haveFiles = False;

	for (Int i = 0; i < requestParameters.Length(); i++)
	{
		if (requestParameters.GetNth(i).isFile) { haveFiles = True; break; }
	}

	/* Set content.
	 */
	Buffer<UnsignedByte>	 postBuffer;

	if (content != NIL)
	{
		headers = curl_slist_append(headers, "Content-Type: text/plain; charset=UTF-8");

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *) content.ConvertTo("UTF-8"));
	}
	else if (haveFiles)
	{
		String	 separator = "THIS_STRING_SEPARATES";

		headers = curl_slist_append(headers, String("Content-Type: multipart/form-data; boundary=").Append(separator));

		/* Write contents to temporary output file.
		 */
		Math::RandomSeed();

		S::File		 tempFile = S::System::System::GetTempDirectory().Append("httprequest-").Append(Number((Int64) Math::Random() & 0x7FFFFFFF).ToHexString()).Append(".temp");
		IO::OutStream	 out(IO::STREAM_FILE, tempFile, IO::OS_REPLACE);

		out.OutputString(String("--").Append(separator).Append("\r\n"));
		out.OutputString("Content-Disposition: form-data; name=\"MAX_FILE_SIZE\"\r\n\r\n");
		out.OutputString("1000000\r\n");

		foreach (const Parameter &parameter, requestParameters)
		{
			out.OutputString(String("--").Append(separator).Append("\r\n"));

			if (!parameter.isFile)
			{
				out.OutputString(String("Content-Disposition: form-data; name=\"").Append(parameter.key).Append("\"\r\n\r\n"));
				out.OutputString(String(parameter.value).Append("\r\n"));
			}
			else
			{
				out.OutputString(String("Content-Disposition: form-data; name=\"").Append(parameter.key).Append("\"; filename=\"").Append(parameter.value).Append("\"\r\n\r\n"));

				IO::InStream	 in(IO::STREAM_FILE, parameter.value, IO::IS_READ);

				for (Int i = 0; i < in.Size(); i++) out.OutputNumber(in.InputNumber(1), 1);

				out.OutputString("\r\n");
			}
		}

		out.OutputString(String("--").Append(separator).Append("--"));

		out.Close();

		/* Read contents into buffer.
		 */
		IO::InStream	 in(IO::STREAM_FILE, tempFile, IO::IS_READ);

		postBuffer.Resize(in.Size());

		for (Int i = 0; i < in.Size(); i++) postBuffer[i] = in.InputNumber(1);

		in.Close();

		tempFile.Delete();

		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postBuffer.Size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *) (UnsignedByte *) postBuffer);
	}

	/* Set URL and basic parameters.
	 */
	curl_easy_setopt(curl, CURLOPT_URL, (char *) url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

	/* Set proxy information.
	 */
	if (proxyMode != HTTP_PROXY_NONE)
	{
		curl_easy_setopt(curl, CURLOPT_PROXY, (char *) proxy);
		curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyPort);

		char	*curlProxyUser = curl_easy_escape(curl, proxyUser, 0);
		char	*curlProxyPass = curl_easy_escape(curl, proxyPass, 0);

		curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, (char *) String(curlProxyUser).Append(":").Append(curlProxyPass));

		curl_free(curlProxyUser);
		curl_free(curlProxyPass);

		if	(proxyMode == HTTP_PROXY_HTTP)	 curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);

#if defined CURLPROXY_HTTPS
		else if (proxyMode == HTTP_PROXY_HTTPS)	 curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS);
#endif

#if defined CURLPROXY_SOCKS4A
		else if (proxyMode == HTTP_PROXY_SOCKS4) curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4A);
#else
		else if (proxyMode == HTTP_PROXY_SOCKS4) curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
#endif

#if defined CURLPROXY_SOCKS5_HOSTNAME
		else if (proxyMode == HTTP_PROXY_SOCKS5) curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);
#else
		else if (proxyMode == HTTP_PROXY_SOCKS5) curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
#endif
	}

	/* Set callback functions.
	 */
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &httpHeader);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &httpWrite);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, &httpProgress);
	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);

	/* Setup files and issue request.
	 */
	ulStart	= 0;
	dlStart	= 0;

	file	= fileName;
	out	= new IO::OutStream(IO::STREAM_FILE, fileName, IO::OS_REPLACE);

	if (curl_easy_perform(curl) != CURLE_OK) error = True;

	delete out;

	/* Clean up.
	 */
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);

	if (!error) return Success();
	else	    return Error();
}

S::String S::Net::Protocols::HTTP::GetParametersURLEncoded() const
{
	String	 str;

	for (Int i = 0; i < requestParameters.Length(); i++)
	{
		const Parameter	&parameter = requestParameters.GetNth(i);

		if (i > 0) str.Append("&");

		str.Append(parameter.key).Append("=").Append(parameter.value);
	}

	return str;
}

size_t S::Net::Protocols::httpHeader(char *buffer, size_t size, size_t n, void *data)
{
	HTTP	*http = (HTTP *) data;

	/* Cancel request if requested.
	 */
	if (http->doCancelDownload.Call()) return 0;

	/* Get data as string.
	 */
	String	 header;

	for (UnsignedInt i = 0; i < size * n; i++) header[i] = buffer[i];

	/* Handle headers.
	 */
	if (header.StartsWith("HTTP/1."))
	{
		/* Reset output stream when we get a new HTTP response.
		 */
		delete http->out;

		http->out = new IO::OutStream(IO::STREAM_FILE, http->file, IO::OS_REPLACE);

		http->responseFields.RemoveAll();
	}
	else if (header.Contains(": "))
	{
		Parameter	 field;
		Int		 colon	= header.Find(": ");

		field.key	= header.Head(colon);
		field.value	= header.Tail(header.Length() - colon - 2);

		http->responseFields.Add(field);
	}

	return size * n;
}

size_t S::Net::Protocols::httpWrite(char *buffer, size_t size, size_t n, void *data)
{
	HTTP	*http = (HTTP *) data;

	/* Cancel download if requested.
	 */
	if (http->doCancelDownload.Call()) return 0;

	/* Write data to output file.
	 */
	http->out->OutputData(buffer, size * n);

	return size * n;
}

int S::Net::Protocols::httpProgress(void *data, double dlTotal, double dlNow, double ulTotal, double ulNow)
{
	HTTP	*http = (HTTP *) data;

	if (ulNow > 0 && http->ulStart == 0) http->ulStart = System::System::Clock();
	if (dlNow > 0 && http->dlStart == 0) http->dlStart = System::System::Clock();

	if (http->dlStart == 0)
	{
		if (ulTotal > 0) http->uploadProgress.Emit(Math::Round(1000.0 * ulNow / ulTotal));
		if (ulNow   > 0) http->uploadSpeed.Emit(String::FromInt(Math::Round((ulNow / 1024) / (Float(System::System::Clock() - http->ulStart) / 1000))).Append(" kB/s"));
	}
	else
	{
		if (dlTotal > 0) http->downloadProgress.Emit(Math::Round(1000.0 * dlNow / dlTotal));
		if (dlNow   > 0) http->downloadSpeed.Emit(String::FromInt(Math::Round((dlNow / 1024) / (Float(System::System::Clock() - http->dlStart) / 1000))).Append(" kB/s"));
	}

	return 0;
}
