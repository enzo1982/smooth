 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/net/protocols/protocol.h>

S::Array<S::Net::Protocols::Protocol *(*)(const S::String &)>	*S::Net::Protocols::Protocol::protocol_creators = NIL;
S::Array<S::String>						*S::Net::Protocols::Protocol::protocol_magics = NIL;

S::Int S::Net::Protocols::Protocol::AddProtocol(Protocol *(*protocol)(const String &), const String &magic)
{
	if (protocol == NIL) return Error();

	if (protocol_creators == NIL)	protocol_creators = new Array<Protocol *(*)(const String &)>;
	if (protocol_magics == NIL)	protocol_magics = new Array<String>;

	protocol_creators->Add(protocol);
	protocol_magics->Add(magic);

	return Success();
}

S::Net::Protocols::Protocol *S::Net::Protocols::Protocol::CreateForURL(const String &url)
{
	for (Int i = 0; i < protocol_creators->GetNOfEntries(); i++)
	{
		if (url.StartsWith(protocol_magics->GetNth(i))) return protocol_creators->GetNth(i)(url);
	}

	return NIL;
}

S::Net::Protocols::Protocol::Protocol(const String &iURL)
{
	url = iURL;
}

S::Net::Protocols::Protocol::~Protocol()
{
}

S::Int S::Net::Protocols::Protocol::DownloadToBuffer(Buffer<UnsignedByte> &destination)
{
	S::File	 downloadFile = S::System::System::GetTempDirectory().Append("download.temp");
	Bool	 error = DownloadToFile(downloadFile);

	if (error) return Error();

	IO::InStream	*f_in = new IO::InStream(IO::STREAM_FILE, downloadFile, IO::IS_READONLY);

	Int		 bytes = f_in->Size();

	destination.Resize(bytes);

	IO::OutStream	*b_out		= new IO::OutStream(IO::STREAM_BUFFER, destination, bytes);
	UnsignedByte	*buffer		= new UnsignedByte [1024];

	for (Int i = 0; i < bytes; i += 1024)
	{
		b_out->OutputData(f_in->InputData(buffer, Math::Min(1024, bytes - i)), Math::Min(1024, bytes - i));
	}

	delete [] buffer;
	delete b_out;
	delete f_in;

	downloadFile.Delete();

	return Success();
}
