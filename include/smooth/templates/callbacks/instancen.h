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

#include "../../callbacks/instance.h"

namespace smooth
{
	template <class ct, class rt CALLBACKS_CONDITIONAL_COMMA CALLBACKS_ARGUMENT_LIST> class CALLBACKS_INSTANCE_CLASS_NAME : public CInstance<rt>
	{
		public:
			ct		*instance;
					 CALLBACKS_INSTANCE_CLASS_NAME(ct *iInstance)	{ instance = iInstance; }
			CInstance<rt>	*Copy()						{ return new CALLBACKS_INSTANCE_CLASS_NAME<ct, rt CALLBACKS_CONDITIONAL_COMMA CALLBACKS_ARGUMENT_TYPES>(instance); }

			rt Call(CMethod *method, ...)
			{
				va_list	 ap;

				va_start(ap, method);

				rt rVal = (instance->*(((CMethodT<rt (ct::*)(CALLBACKS_ARGUMENT_TYPES)> *) method)->method))(CALLBACKS_VA_ARGUMENTS);

				va_end(ap);


				return rVal;
			}

			Bool operator ==(CInstance<rt> *cInstance)
			{
				if (instance == ((CALLBACKS_INSTANCE_CLASS_NAME<ct, rt CALLBACKS_CONDITIONAL_COMMA CALLBACKS_ARGUMENT_TYPES> *) cInstance)->instance)	return True;
				else																	return False;
			}
	};
};
