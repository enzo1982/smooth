 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <stdarg.h>

#include "../../signals/instance.h"

namespace smooth
{
	template <class ct, class rt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_LIST> class SIGNALS_INSTANCE_CLASS_NAME : public Instance
	{
		public:
			ct		*instance;
					 SIGNALS_INSTANCE_CLASS_NAME(ct *iInstance)	{ instance = iInstance; }
			Instance	*Copy()						{ return new SIGNALS_INSTANCE_CLASS_NAME<ct, rt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(instance); }

			Void Call(Method *method, ...)
			{
				va_list	 ap;

				va_start(ap, method);

				(instance->*(((MethodT<rt (ct::*)(SIGNALS_ARGUMENT_TYPES)> *) method)->method))(SIGNALS_VA_ARGUMENTS);

				va_end(ap);

			}

			Bool operator ==(Instance *cInstance)
			{
				if (instance == ((SIGNALS_INSTANCE_CLASS_NAME<ct, rt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) cInstance)->instance)	return True;
				else																return False;
			}
	};
};
