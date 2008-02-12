 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
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

					String			 content;

					String			 proxy;
					Int			 proxyPort;
					String			 proxyUser;
					String			 proxyPass;

					Array<Parameter>	 parameters;
					Buffer<UnsignedByte>	 requestBuffer;

					Int			 DecodeHexNumber(const String &);

					Buffer<UnsignedByte>	&ComposeHTTPRequest(const String &, const String &);

					Void			 ComposeGETRequest(const String &, const String &);
					Void			 ComposePOSTRequest(const String &, const String &);

					String			 GetParametersURLEncoded();
				public:
								 HTTP(const String &);
					virtual			~HTTP();

					Int			 SetParameter(const String &, const String &);		// Set header parameter
					Int			 SetParameterFile(const String &, const String &);

					Int			 SetMode(Int);						// Set mode (get or post)

					Int			 SetContent(const String &);				// Set content to be transmitted

					Int			 SetProxy(const String &, Int);				// Set proxy host name and port
					Int			 SetProxyAuth(const String &, const String &);		// Set user name and password for proxy

					Int			 DownloadToFile(const String &);
			};

			const Int	 HTTP_METHOD_GET	= 0;
			const Int	 HTTP_METHOD_POST	= 1;
		};
	};
};

#endif
