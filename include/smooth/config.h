 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CONFIGURATION_
#define _H_OBJSMOOTH_CONFIGURATION_

namespace smooth
{
	class Configuration;
};

#include "string.h"
#include "xml/document.h"

namespace smooth
{
	class SMOOTHAPI Configuration
	{
		private:
			String		 fileName;

			XML::Document	*configFile;
			XML::Node	*ownRoot;

			XML::Node	*FindSectionNode(String);
			XML::Node	*FindValueNode(String, String);
		public:
					 Configuration();
					 Configuration(String, Bool = True);
					~Configuration();

			Int		 Open(String = "config.xml", Bool = True);
			Int		 Close();

			Int		 GetIntValue(String, String, Int = 0);
			Int		 SetIntValue(String, String, Int);

			String		 GetStringValue(String, String, String = NIL);
			Int		 SetStringValue(String, String, String);
	};
};

#endif
