 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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
			rt		 value;
			Bool		 gotValue;
		public:
			Callback()
			{
				method = NIL;
				instance = NIL;
				function = NIL;
				value = 0;
				gotValue = False;
			}

			Callback(const Callback &oCallback)
			{
				method = NIL;
				instance = NIL;

				DisconnectAll();

				if (oCallback.method != NIL)
				{
					method = oCallback.method->Copy();
					instance = oCallback.instance->Copy();
				}

				function = oCallback.function;

				if (oCallback.gotValue)
				{
					value = oCallback.value;
					gotValue = oCallback.gotValue;
				}
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
				value = 0;
				gotValue = False;

				return Success;
			}
	};
};

#endif
