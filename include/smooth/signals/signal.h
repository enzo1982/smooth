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

			Array<Method *>		methods0;
			Array<Instance *>	instances0;
			Array<Void (*)()>	functions0;
			Array<Signal *>		sigs0;
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

				for (int l = 0; l < Array<Method *>::GetNOfEntries(&oSignal.methods0); l++)
				{
					methods0.AddEntry(Array<Method *>::GetNthEntry(&oSignal.methods0, l)->Copy());
					instances0.AddEntry(Array<Instance *>::GetNthEntry(&oSignal.instances0, l)->Copy());
				}

				for (int m = 0; m < Array<Void (*)()>::GetNOfEntries(&oSignal.functions0); m++)
				{
					functions0.AddEntry(Array<Void (*)()>::GetNthEntry(&oSignal.functions0, m));
				}

				for (int n= 0; n < Array<Signal *>::GetNOfEntries(&oSignal.sigs0); n++)
				{
					sigs0.AddEntry(Array<Signal *>::GetNthEntry(&oSignal.sigs0, n));
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

				instances.RemoveAll();
				methods.RemoveAll();
				functions.RemoveAll();
				sigs.RemoveAll();

				for (Int j = 0; j < methods0.GetNOfEntries(); j++)
				{
					delete instances0.GetNthEntry(j);
					delete methods0.GetNthEntry(j);
				}

				instances0.RemoveAll();
				methods0.RemoveAll();
				functions0.RemoveAll();
				sigs0.RemoveAll();

				return Success;
			}

			Int GetNOfConnectedSlots()
			{
				return methods.GetNOfEntries() + functions.GetNOfEntries() + sigs.GetNOfEntries() + methods0.GetNOfEntries() + functions0.GetNOfEntries() + sigs0.GetNOfEntries();
			}
	};
};

#endif
