 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_APPLICATION_
#define _H_OBJSMOOTH_APPLICATION_

namespace smooth
{
	class Application;
};

#include "object.h"
#include "container.h"

namespace smooth
{
	class SMOOTHAPI Application : public Object, public Container
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

#endif
