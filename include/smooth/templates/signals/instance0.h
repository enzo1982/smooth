 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../../signals/instance.h"

namespace smooth
{
	template <class ct> class Instance0 : public Instance
	{
		private:
			ct	*instance;
		public:
			Instance0(ct *iInstance)
			{
				instance = iInstance;
			}

			Void Call(Method *method)
			{
				(((ct *) instance)->*(((Method0<Void (ct::*)()> *) method)->GetMethod()))();
			}

			Instance *Copy()
			{
				return new Instance0<ct>(instance);
			}
	};
};
