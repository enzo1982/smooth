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
		public:
			Signal()
			{
			}

			Signal(const Signal &oSignal)
			{
				for (int i = 0; i < Array<Method *>::GetNOfEntries(&oSignal.methods); i++)
				{
					methods.AddEntry(Array<Method *>::GetNthEntry(&oSignal.methods, i)->Copy());
					instances.AddEntry(Array<Instance *>::GetNthEntry(&oSignal.instances, i)->Copy());
				}

				for (int j = 0; j < Array<Void (*)()>::GetNOfEntries(&oSignal.functions); j++)
				{
					functions.AddEntry(Array<Void (*)()>::GetNthEntry(&oSignal.functions, j));
				}

				for (int k= 0; k < Array<Signal *>::GetNOfEntries(&oSignal.sigs); k++)
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
				for (Int i = 0; i < methods.GetNOfEntries(); i++)
				{
					delete instances.GetNthEntry(i);
					delete methods.GetNthEntry(i);
				}

				instances.DeleteAll();
				methods.DeleteAll();
				functions.DeleteAll();
				sigs.DeleteAll();

				return Success;
			}

			Int GetNOfConnectedSlots()
			{
				return methods.GetNOfEntries() + functions.GetNOfEntries() + sigs.GetNOfEntries();
			}
	};
};

#endif
