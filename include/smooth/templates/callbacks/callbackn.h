 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../../callbacks/callback.h"

#include "methodt.h"
#include "instancen.h"

namespace smooth
{
	template <class rt CALLBACKS_CONDITIONAL_COMMA CALLBACKS_ARGUMENT_LIST> class CALLBACKS_CALLBACK_CLASS_NAME : public Callback<rt>
	{
		public:
			template <class ct> Int Connect(rt (ct::*proc)(CALLBACKS_ARGUMENT_TYPES), ct *inst)
			{
				DisconnectAll();

				instance = new CALLBACKS_INSTANCE_CLASS_NAME<ct, rt CALLBACKS_CONDITIONAL_COMMA CALLBACKS_ARGUMENT_TYPES>(inst);
				method = new CMethodT<rt (ct::*)(CALLBACKS_ARGUMENT_TYPES)>(proc);

				return Success;
			}

			Int Connect(rt (*proc)(CALLBACKS_ARGUMENT_TYPES))
			{
				DisconnectAll();

				function = (rt (*)()) proc;

				return Success;
			}

			template <class ct> int Disconnect(rt (ct::*proc)(CALLBACKS_ARGUMENT_TYPES), ct *inst)
			{
				CALLBACKS_INSTANCE_CLASS_NAME<ct, rt CALLBACKS_CONDITIONAL_COMMA CALLBACKS_ARGUMENT_TYPES>	*minstance = new CALLBACKS_INSTANCE_CLASS_NAME<ct, rt CALLBACKS_CONDITIONAL_COMMA CALLBACKS_ARGUMENT_TYPES>(inst);
				CMethodT<rt (ct::*)(CALLBACKS_ARGUMENT_TYPES)>							*mmethod = new CMethodT<rt (ct::*)(CALLBACKS_ARGUMENT_TYPES)>(proc);

				if (*instance == minstance && *method == mmethod)
				{
					delete instance;
					delete method;

					instance = NIL;
					method = NIL;
				}

				delete minstance;
				delete mmethod;

				return Success;
			}

			Int Disconnect(rt (*proc)(CALLBACKS_ARGUMENT_TYPES))
			{
				if (function == (rt (*)()) proc)
				{
					function = NIL;
				}

				return Success;
			}

			rt Call(CALLBACKS_ARGUMENT_PARAMETER_LIST)
			{
				rt rVal = (rt) NIL;

				if (method != NIL)		rVal = instance->Call(method CALLBACKS_CONDITIONAL_COMMA CALLBACKS_ARGUMENT_INV_PARAMETERS);
				else if (function != NIL)	rVal = ((rt (*)(CALLBACKS_ARGUMENT_TYPES)) function)(CALLBACKS_ARGUMENT_PARAMETERS);

				return rVal;
			}
	};
};
