 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../definitions.h"

#ifndef _H_OBJSMOOTH_ERROR_
#define _H_OBJSMOOTH_ERROR_

namespace smooth
{
	namespace Error
	{
		class Error;
	};
};

#include "../misc/string.h"
#include "../array.h"
#include "../basic/object.h"

namespace smooth
{
	namespace Error
	{
		class SMOOTHAPI Error : public Object
		{
			private:
				String			 message;
			public:
				static const Int	 classID;

							 Error(String = NIL);
							 Error(const Error &);

				virtual			~Error();

				virtual String		 ToString();

				String			 GetErrorMessage();
				Error			 SetErrorMessage(String);

				virtual operator	 Bool();

				virtual Bool operator	 ==(const Error &);
				virtual Bool operator	 !=(const Error &);
		};
	};
};

#endif
