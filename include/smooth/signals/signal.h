 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SIGNAL_
#define _H_SIGNAL_

namespace smooth
{
	class Signal;
};

#include "method.h"
#include "instance.h"

namespace smooth
{
	class Signal
	{
		protected:
			Array<Method *>		methods;
			Array<Instance *>	instances;

			Int	 nOfMethods;
		public:
			Signal()
			{
				nOfMethods = 0;
			}

			Signal(const Signal &oSignal)
			{
				nOfMethods = oSignal.nOfMethods;

				for (int i = 0; i < nOfMethods; i++)
				{
					methods.AddEntry(Array<Method *>::GetNthEntry(&oSignal.methods, i)->Copy());
					instances.AddEntry(Array<Instance *>::GetNthEntry(&oSignal.instances, i)->Copy());
				}
			}

			virtual ~Signal()
			{
				DisconnectAll();
			}

			Int DisconnectAll()
			{
				for (Int i = 0; i < nOfMethods; i++)
				{
					delete instances.GetNthEntry(i);
					delete methods.GetNthEntry(i);
				}

				instances.DeleteAll();
				methods.DeleteAll();

				nOfMethods = 0;

				return Success;
			}

			Int GetNOfConnectedSlots()
			{
				return nOfMethods;
			}
	};
};

#endif
