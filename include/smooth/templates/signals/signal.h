 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "signal_base.h"

namespace smooth
{
	template <class returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_LIST> class SIGNALS_SIGNAL_CLASS_NAME : public Signal
	{
		protected:
			Array<Void *>	 slotsN;
			Array<Void *>	 slots0;
		public:
			SIGNALS_SIGNAL_CLASS_NAME()
			{
			}

			SIGNALS_SIGNAL_CLASS_NAME(const SIGNALS_SIGNAL_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> &oSignal)
			{
				for (int i = 0; i < oSignal.slotsN.GetNOfEntries(); i++)
				{
					slotsN.AddEntry(((SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) oSignal.slotsN.GetNthEntry(i))->Copy());
				}

				for (int j = 0; j < oSignal.slots0.GetNOfEntries(); j++)
				{
					slots0.AddEntry(((SlotBase0<Void> *) oSignal.slots0.GetNthEntry(j))->Copy());
				}

				parent = oSignal.parent;
			}

			virtual ~SIGNALS_SIGNAL_CLASS_NAME()
			{
				DisconnectAll();
			}

			template <class classTYPE, class oClassTYPE, class slotReturnTYPE> Int Connect(slotReturnTYPE (classTYPE::*proc)(SIGNALS_ARGUMENT_TYPES), oClassTYPE *inst)
			{
				slotsN.AddEntry(new SIGNALS_SLOT_CLASS_CLASS_NAME<oClassTYPE, slotReturnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(proc, inst));

				return Success();
			}

			template <class slotReturnTYPE> Int Connect(slotReturnTYPE (*proc)(SIGNALS_ARGUMENT_TYPES))
			{
				slotsN.AddEntry(new SIGNALS_SLOT_GLOBAL_CLASS_NAME<slotReturnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(proc));

				return Success();
			}

			template <class slotReturnTYPE> Int Connect(SIGNALS_SIGNAL_CLASS_NAME<slotReturnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *sig)
			{
				if ((Signal *) sig == (Signal *) this) return Error();

				slotsN.AddEntry(new SIGNALS_SLOT_SIGNAL_CLASS_NAME<slotReturnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(sig));
		
				return Success();
			}

#ifndef SIGNALS_SIGNAL_ZERO
			template <class classTYPE, class oClassTYPE, class slotReturnTYPE> Int Connect(slotReturnTYPE (classTYPE::*proc)(), oClassTYPE *inst)
			{
				slots0.AddEntry(new SlotClass0<oClassTYPE, slotReturnTYPE>(proc, inst));

				return Success();
			}

			template <class slotReturnTYPE> Int Connect(slotReturnTYPE (*proc)())
			{
				slots0.AddEntry(new SlotGlobal0<slotReturnTYPE>(proc));

				return Success();
			}

			template <class slotReturnTYPE> Int Connect(Signal0<slotReturnTYPE> *sig)
			{
				if ((Signal *) sig == (Signal *) this) return Error();

				slots0.AddEntry(new SlotSignal0<slotReturnTYPE>(sig));

				return Success();
			}
#endif

			template <class classTYPE, class oClassTYPE, class slotReturnTYPE> Int Disconnect(slotReturnTYPE (classTYPE::*proc)(SIGNALS_ARGUMENT_TYPES), oClassTYPE *inst)
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)
				{
					if ((*((SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i))) == SIGNALS_SLOT_CLASS_CLASS_NAME<oClassTYPE, slotReturnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(proc, inst))
					{
						delete (SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i);

						slotsN.RemoveEntry(slotsN.GetNthEntryIndex(i));

						break;
					}
				}

				return Success();
			}

			template <class slotReturnTYPE> Int Disconnect(slotReturnTYPE (*proc)(SIGNALS_ARGUMENT_TYPES))
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)
				{
					if ((*((SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i))) == SIGNALS_SLOT_GLOBAL_CLASS_NAME<slotReturnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(proc))
					{
						delete (SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i);

						slotsN.RemoveEntry(slotsN.GetNthEntryIndex(i));

						break;
					}
				}

				return Success();
			}

			template <class slotReturnTYPE> Int Disconnect(SIGNALS_SIGNAL_CLASS_NAME<slotReturnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *sig)
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)
				{
					if ((*((SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i))) == SIGNALS_SLOT_SIGNAL_CLASS_NAME<slotReturnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(sig))
					{
						delete (SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i);

						slotsN.RemoveEntry(slotsN.GetNthEntryIndex(i));

						break;
					}
				}

				return Success();
			}

#ifndef SIGNALS_SIGNAL_ZERO
			template <class classTYPE, class oClassTYPE, class slotReturnTYPE> Int Disconnect(slotReturnTYPE (classTYPE::*proc)(), oClassTYPE *inst)
			{
				for (Int i = 0; i < slots0.GetNOfEntries(); i++)
				{
					if ((*((SlotBase0<Void> *) slots0.GetNthEntry(i))) == SlotClass0<oClassTYPE, slotReturnTYPE>(proc, inst))
					{
						delete (SlotBase0<Void> *) slots0.GetNthEntry(i);

						slots0.RemoveEntry(slots0.GetNthEntryIndex(i));

						break;
					}
				}

				return Success();
			}

			template <class slotReturnTYPE> Int Disconnect(slotReturnTYPE (*proc)())
			{
				for (Int i = 0; i < slots0.GetNOfEntries(); i++)
				{
					if ((*((SlotBase0<Void> *) slots0.GetNthEntry(i))) == SlotGlobal0<slotReturnTYPE>(proc))
					{
						delete (SlotBase0<Void> *) slots0.GetNthEntry(i);

						slots0.RemoveEntry(slots0.GetNthEntryIndex(i));

						break;
					}
				}

				return Success();
			}

			template <class slotReturnTYPE> Int Disconnect(Signal0<slotReturnTYPE> *sig)
			{
				for (Int i = 0; i < slots0.GetNOfEntries(); i++)
				{
					if ((*((SlotBase0<Void> *) slots0.GetNthEntry(i))) == SlotSignal0<slotReturnTYPE>(sig))
					{
						delete (SlotBase0<Void> *) slots0.GetNthEntry(i);

						slots0.RemoveEntry(slots0.GetNthEntryIndex(i));

						break;
					}
				}

				return Success();
			}
#endif

			Int DisconnectAll()
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)	delete (SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i);
				for (Int j = 0; j < slots0.GetNOfEntries(); j++)	delete (SlotBase0<Void> *) slots0.GetNthEntry(j);

				slotsN.RemoveAll();
				slots0.RemoveAll();

				return Success();
			}

			Void Emit(SIGNALS_ARGUMENT_PARAMETER_LIST)
			{
				ProtectParent();

				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)	((SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i))->Emit(SIGNALS_ARGUMENT_PARAMETERS);
				for (Int j = 0; j < slots0.GetNOfEntries(); j++)	((SlotBase0<Void> *) slots0.GetNthEntry(j))->Emit();

				UnprotectParent();
			}

			Void EmitUnprotected(SIGNALS_ARGUMENT_PARAMETER_LIST)
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)	((SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *) slotsN.GetNthEntry(i))->Emit(SIGNALS_ARGUMENT_PARAMETERS);
				for (Int j = 0; j < slots0.GetNOfEntries(); j++)	((SlotBase0<Void> *) slots0.GetNthEntry(j))->Emit();
			}

			Int GetNOfConnectedSlots()
			{
				return slotsN.GetNOfEntries() + slots0.GetNOfEntries();
			}
	};
};
