 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CALLBACK_
#define _H_CALLBACK_

#include "method.h"
#include "instance.h"

namespace smooth
{
	template <class rt> class Callback
	{
		protected:
			CMethod		*method;
			CInstance<rt>	*instance;
			rt 		 (*function)();
		public:
			Callback()
			{
				method = NIL;
				instance = NIL;
				function = NIL;
			}

			Callback(const Callback &oCallback)
			{
				if (oCallback.method != NIL)
				{
					method = oCallback.method->Copy();
					instance = oCallback.instance->Copy();
				}

				function = oCallback.function;
			}

			virtual ~Callback()
			{
				DisconnectAll();
			}

			Int DisconnectAll()
			{
				if (method != NIL)
				{
					delete instance;
					delete method;

					instance = NIL;
					method = NIL;
				};

				function = NIL;

				return Success;
			}
	};
};

#endif
