 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/net/protocols/protocol.h>
#include <smooth/files/file.h>
#include <smooth/system/system.h>
#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>
#include <smooth/misc/math.h>
#include <smooth/misc/number.h>
#include <smooth/init.h>

using namespace smooth::IO;

S::Array<S::Net::Protocols::Protocol *(*)(const S::String &), S::Void *>	*S::Net::Protocols::Protocol::protocol_creators = NIL;
S::Array<S::String>								*S::Net::Protocols::Protocol::protocol_magics = NIL;

S::Int	 addProtocolFreeTmp = S::AddFreeFunction(&S::Net::Protocols::Protocol::RemoveAllProtocols);

S::Int S::Net::Protocols::Protocol::AddProtocol(Protocol *(*protocol)(const String &), const String &magic)
{
	if (protocol == NIL) return Error();

	if (protocol_creators == NIL)	protocol_creators = new Array<Protocol *(*)(const String &), Void *>;
	if (protocol_magics == NIL)	protocol_magics = new Array<String>;

	protocol_creators->Add(protocol);
	protocol_magics->Add(magic);

	return Success();
}

S::Int S::Net::Protocols::Protocol::RemoveAllProtocols()
{
	protocol_creators->RemoveAll();
	protocol_magics->RemoveAll();

	delete protocol_creators;
	delete protocol_magics;

	return Success();
}

S::Net::Protocols::Protocol *S::Net::Protocols::Protocol::CreateForURL(const String &url)
{
	for (Int i = 0; i < protocol_creators->Length(); i++)
	{
		if (url.StartsWith(protocol_magics->GetNth(i))) return protocol_creators->GetNth(i)(url);
	}

	return NIL;
}

S::Net::Protocols::Protocol::Protocol(const String &iURL)
{
	url = iURL;

	doCancelDownload.Connect(False);
}

S::Net::Protocols::Protocol::~Protocol()
{
}

S::Int S::Net::Protocols::Protocol::DownloadToBuffer(Buffer<UnsignedByte> &destination)
{
	Math::RandomSeed();

	S::File	 downloadFile = S::System::System::GetTempDirectory().Append("download-").Append(Number((Int64) Math::Random() & 0x7FFFFFFF).ToHexString()).Append(".temp");
	Bool	 error = DownloadToFile(downloadFile);

	if (error) return Error();

	InStream	 in(STREAM_FILE, downloadFile, IS_READ);
	Int		 size = in.Size();

	destination.Resize(size);

	for (Int p = 0; p < size; p += 1024) in.InputData(destination + p, Math::Min(1024, size - p));

	in.Close();

	downloadFile.Delete();

	return Success();
}
