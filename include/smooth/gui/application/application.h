 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_APPLICATION_
#define _H_OBJSMOOTH_APPLICATION_

namespace smooth
{
	namespace GUI
	{
		class Application;
	};
};

#include "../widgets/container.h"

namespace smooth
{
	namespace GUI
	{
		abstract class SMOOTHAPI Application : public Container
		{
			private:
				static String		 startupDirectory;
				static String		 applicationDirectory;
			public:
				static const Int	 classID;

							 Application(String = NIL);

				static String		 GetStartupDirectory();
				static String		 GetApplicationDirectory();
		};
	};
};

#endif
