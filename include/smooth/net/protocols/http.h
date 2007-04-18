 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_PROTOCOL_HTTP_
#define _H_OBJSMOOTH_PROTOCOL_HTTP_

#include "protocol.h"

namespace smooth
{
	namespace Net
	{
		namespace Protocols
		{
			class SMOOTHAPI Parameter
			{
				public:
					String	 key;
					String	 value;

					Bool	 isFile;

						 Parameter(Int null = 0) { isFile = False; }
			};

			class SMOOTHAPI HTTP : public Protocol
			{
				private:
					Int			 mode;

					Array<Parameter>	 parameters;
					Buffer<UnsignedByte>	 requestBuffer;

					Int			 DecodeHexNumber(const String &);

					Buffer<UnsignedByte>	&ComposeHTTPRequest(const String &, const String &);
				public:
								 HTTP(const String &);
					virtual			~HTTP();

					Int			 SetParameter(const String &, const String &);
					Int			 SetParameterFile(const String &, const String &);

					Int			 SetMode(Int);

					Int			 DownloadToFile(const String &);
			};

			const Int	 HTTP_METHOD_GET	= 0;
			const Int	 HTTP_METHOD_POST	= 1;
		};
	};
};

#endif
