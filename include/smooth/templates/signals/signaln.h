 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../../signals/signal.h"

#include "methodt.h"
#include "instancen.h"

namespace smooth
{
	template <class rt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_LIST> class SIGNALS_SIGNAL_CLASS_NAME : public Signal
	{
		public:
			template <class ct, class rrt> Int Connect(rrt (ct::*proc)(SIGNALS_ARGUMENT_TYPES), ct *inst)
			{
				instances.AddEntry(new SIGNALS_INSTANCE_CLASS_NAME<ct, rrt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(inst));
				methods.AddEntry(new MethodT<rrt (ct::*)(SIGNALS_ARGUMENT_TYPES)>(proc));

				return Success;
			}

			template <class rrt> Int Connect(rrt (*proc)(SIGNALS_ARGUMENT_TYPES))
			{
				functions.AddEntry((Void (*)()) proc);

				return Success;
			}

			Int Connect(SIGNALS_SIGNAL_CLASS_NAME *sig)
			{
				if (sig == this) return Error;

				sigs.AddEntry(sig);

				return Success;
			}

#ifndef SIGNALS_SIGNAL_ZERO
			template <class ct, class rrt> Int Connect(rrt (ct::*proc)(), ct *inst)
			{
				instances0.AddEntry(new Instance0<ct, rrt>(inst));
				methods0.AddEntry(new MethodT<rrt (ct::*)()>(proc));

				return Success;
			}

			template <class rrt> Int Connect(rrt (*proc)())
			{
				functions0.AddEntry((Void (*)()) proc);

				return Success;
			}

			Int Connect(Signal0<Void> *sig)
			{
				if (sig == this) return Error;

				sigs0.AddEntry(sig);

				return Success;
			}
#endif

			template <class ct, class rrt> int Disconnect(rrt (ct::*proc)(SIGNALS_ARGUMENT_TYPES), ct *inst)
			{
				SIGNALS_INSTANCE_CLASS_NAME<ct, rrt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>	*instance = new SIGNALS_INSTANCE_CLASS_NAME<ct, rrt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(inst);
				MethodT<rrt (ct::*)(SIGNALS_ARGUMENT_TYPES)>						*method = new MethodT<rrt (ct::*)(SIGNALS_ARGUMENT_TYPES)>(proc);

				for (Int i = 0; i < methods.GetNOfEntries(); i++)
				{
					if (*instances.GetNthEntry(i) == instance && *methods.GetNthEntry(i) == method)
					{
						delete instances.GetNthEntry(i);
						delete methods.GetNthEntry(i);

						instances.RemoveEntry(instances.GetNthEntryIndex(i));
						methods.RemoveEntry(methods.GetNthEntryIndex(i));

						break;
					}
				}

				delete instance;
				delete method;

				return Success;
			}

			template <class rrt> Int Disconnect(rrt (*proc)(SIGNALS_ARGUMENT_TYPES))
			{
				for (Int i = 0; i < functions.GetNOfEntries(); i++)
				{
					if (functions.GetNthEntry(i) == (Void (*)()) proc)
					{
						functions.RemoveEntry(functions.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}

			Int Disconnect(SIGNALS_SIGNAL_CLASS_NAME *sig)
			{
				for (Int i = 0; i < sigs.GetNOfEntries(); i++)
				{
					if (sigs.GetNthEntry(i) == sig)
					{
						sigs.RemoveEntry(sigs.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}

#ifndef SIGNALS_SIGNAL_ZERO
			template <class ct, class rrt> int Disconnect(rrt (ct::*proc)(), ct *inst)
			{
				Instance0<ct, rrt>	*instance = new Instance0<ct, rrt>(inst);
				MethodT<rrt (ct::*)()>	*method = new MethodT<rrt (ct::*)()>(proc);

				for (Int i = 0; i < methods0.GetNOfEntries(); i++)
				{
					if (*instances0.GetNthEntry(i) == instance && *methods0.GetNthEntry(i) == method)
					{
						delete instances0.GetNthEntry(i);
						delete methods0.GetNthEntry(i);

						instances0.RemoveEntry(instances0.GetNthEntryIndex(i));
						methods0.RemoveEntry(methods0.GetNthEntryIndex(i));

						break;
					}
				}

				delete instance;
				delete method;

				return Success;
			}

			template <class rrt> Int Disconnect(rrt (*proc)())
			{
				for (Int i = 0; i < functions0.GetNOfEntries(); i++)
				{
					if (functions0.GetNthEntry(i) == (Void (*)()) proc)
					{
						functions0.RemoveEntry(functions0.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}

			Int Disconnect(Signal0<Void> *sig)
			{
				for (Int i = 0; i < sigs0.GetNOfEntries(); i++)
				{
					if (sigs0.GetNthEntry(i) == sig)
					{
						sigs0.RemoveEntry(sigs0.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}
#endif

			Void Emit(SIGNALS_ARGUMENT_PARAMETER_LIST)
			{
				for (Int i = 0; i < methods.GetNOfEntries(); i++)
				{
					instances.GetNthEntry(i)->Call(methods.GetNthEntry(i) SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_INV_PARAMETERS);
				}

				for (Int j = 0; j < functions.GetNOfEntries(); j++)
				{
					((Void (*)(SIGNALS_ARGUMENT_TYPES)) functions.GetNthEntry(j))(SIGNALS_ARGUMENT_PARAMETERS);
				}

				for (Int k = 0; k < sigs.GetNOfEntries(); k++)
				{
					((SIGNALS_SIGNAL_CLASS_NAME *) sigs.GetNthEntry(k))->Emit(SIGNALS_ARGUMENT_PARAMETERS);
				}

				for (Int l = 0; l < methods0.GetNOfEntries(); l++)
				{
					instances0.GetNthEntry(l)->Call(methods0.GetNthEntry(l));
				}

				for (Int m = 0; m < functions0.GetNOfEntries(); m++)
				{
					((Void (*)()) functions0.GetNthEntry(m))();
				}

				for (Int n = 0; n < sigs0.GetNOfEntries(); n++)
				{
					((Signal0<Void> *) sigs0.GetNthEntry(n))->Emit();
				}
			}
	};
};
