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

#include "method.h"
#include "instance.h"

namespace smooth
{
	class Signal
	{
		protected:
			Array<Method *>		methods;
			Array<Instance *>	instances;
			Array<Void (*)()>	functions;
			Array<Signal *>		sigs;

			Int	 nOfMethods;
			Int	 nOfFunctions;
			Int	 nOfSignals;
		public:
			Signal()
			{
				nOfMethods = 0;
				nOfFunctions = 0;
				nOfSignals = 0;
			}

			Signal(const Signal &oSignal)
			{
				nOfMethods = oSignal.nOfMethods;
				nOfFunctions = oSignal.nOfFunctions;
				nOfSignals = oSignal.nOfSignals;

				for (int i = 0; i < nOfMethods; i++)
				{
					methods.AddEntry(Array<Method *>::GetNthEntry(&oSignal.methods, i)->Copy());
					instances.AddEntry(Array<Instance *>::GetNthEntry(&oSignal.instances, i)->Copy());
				}

				for (int j = 0; j < nOfFunctions; j++)
				{
					functions.AddEntry(Array<Void (*)()>::GetNthEntry(&oSignal.functions, j));
				}

				for (int k= 0; k < nOfSignals; k++)
				{
					sigs.AddEntry(Array<Signal *>::GetNthEntry(&oSignal.sigs, k));
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
				functions.DeleteAll();
				sigs.DeleteAll();

				nOfMethods = 0;
				nOfFunctions = 0;
				nOfSignals = 0;

				return Success;
			}

			Int GetNOfConnectedSlots()
			{
				return nOfMethods + nOfFunctions + nOfSignals;
			}
	};
};

#endif
