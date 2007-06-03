 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/net/protocols/file.h>
#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>
#include <smooth/misc/math.h>

#include <time.h>

S::Net::Protocols::Protocol *CreateProtocolFile(const S::String &iURL)
{
	return new S::Net::Protocols::File(iURL);
}

S::Int	 protocolFileTmp = S::Net::Protocols::Protocol::AddProtocol(&CreateProtocolFile, "file://");

S::Net::Protocols::File::File(const String &iURL) : Protocol(iURL)
{
}

S::Net::Protocols::File::~File()
{
}

S::Int S::Net::Protocols::File::DownloadToFile(const String &destination)
{
	Bool	 error = True;

	String	 file;
	Int	 i = 0, j = 0;

	for (i = 0; i < url.Length(); i++)	{ if (url[i] == '/') j = i; }
	for (i = j + 1; i < url.Length(); i++)	{ file[i - j - 1] = url[i]; }

	file = String(destination).Append("\\").Append(file);

	String	 fileName;

	if (url[7] == '/' && url[9] == ':' && url[10] == '/')	j = 8;
	else							j = 7;

	for (i = j; i < url.Length(); i++)
	{
		fileName[i - j] = url[i];
	}

	IO::InStream	*in	= new IO::InStream(IO::STREAM_FILE, fileName, IO::IS_READONLY);

	downloadProgress.Emit(0);
	downloadSpeed.Emit("");

	if (in->GetLastError() == IO::IO_ERROR_OK)
	{
		Int	 bytes = in->Size();

		if (bytes > 0)
		{
			IO::OutStream	*fOut		= new IO::OutStream(IO::STREAM_FILE, file);
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
		}

		error = False;
	}

	downloadProgress.Emit(1000);

	delete in;

	if (!error)	return Success();
	else		return Error();
}
