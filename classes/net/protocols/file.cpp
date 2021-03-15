 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/system/system.h>
#include <smooth/templates/buffer.h>

using namespace smooth::IO;

S::Net::Protocols::Protocol *CreateProtocolFile(const S::String &iURL)
{
	return new S::Net::Protocols::File(iURL);
}

S::Int	 protocolFileTmp = S::Net::Protocols::Protocol::AddProtocol(&CreateProtocolFile, L"file://");

S::Net::Protocols::File::File(const String &iURL) : Protocol(iURL)
{
}

S::Net::Protocols::File::~File()
{
}

S::Int S::Net::Protocols::File::DownloadToFile(const String &destination)
{
	Bool	 error	  = True;
	String	 fileName = url.Tail(url.Length() - 7);

	if (url[7] == '/' && url[9] == ':' && url[10] == '/') fileName = url.Tail(url.Length() - 8);

	InStream	 in(STREAM_FILE, fileName, IS_READ);

	downloadProgress.Emit(0);
	downloadSpeed.Emit(NIL);

	if (in.GetLastError() == IO::IO_ERROR_OK)
	{
		Int	 bytes = in.Size();

		if (bytes > 0)
		{
			OutStream		 out(STREAM_FILE, destination, OS_REPLACE);
			Buffer<UnsignedByte>	 buffer(32768);
			Int			 startTicks = S::System::System::Clock();
			Int			 percent    = 0;

			for (Int i = 0; i < bytes; i += buffer.Size())
			{
				Int	 amount = Math::Min(buffer.Size(), bytes - i);

				in.InputData(buffer, amount);
				out.OutputData(buffer, amount);

				if (Math::Round(1000.0 * i / bytes) != percent)
				{
					percent = Math::Round(1000.0 * i / bytes);

					downloadProgress.Emit(percent);
					downloadSpeed.Emit(String::FromInt(Math::Round((i / 1024) / (Float(S::System::System::Clock() - startTicks) / 1000))).Append(" kB/s"));
				}
			}
		}

		error = False;
	}

	downloadProgress.Emit(1000);

	if (!error) return Success();
	else	    return Error();
}
