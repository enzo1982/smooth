 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../../threads/thread.h"
#include "../array.h"

namespace smooth
{
	SMOOTHVAR Array<Void *> threadSlots;

	template <NONBLOCKING_ARGUMENT_LIST NONBLOCKING_CONDITIONAL_COMMA class dummyTYPE = Void> class NONBLOCKING_NONBLOCKING_CLASS_NAME
	{
		protected:
			NONBLOCKING_SLOT_BASE_CLASS_NAME<Void NONBLOCKING_CONDITIONAL_COMMA NONBLOCKING_ARGUMENT_TYPES>	*slotN;
			Threads::Thread											*thread;

			NONBLOCKING_ARGUMENT_DECLARATIONS
		public:
			template <class classTYPE, class oClassTYPE, class slotReturnTYPE> NONBLOCKING_NONBLOCKING_CLASS_NAME(slotReturnTYPE (classTYPE::*proc)(NONBLOCKING_ARGUMENT_TYPES), oClassTYPE *inst)
			{
				slotN	= new NONBLOCKING_SLOT_CLASS_CLASS_NAME<oClassTYPE, slotReturnTYPE NONBLOCKING_CONDITIONAL_COMMA NONBLOCKING_ARGUMENT_TYPES>(proc, inst);
				thread	= NIL;
			}

			template <class slotReturnTYPE> NONBLOCKING_NONBLOCKING_CLASS_NAME(slotReturnTYPE (*proc)(NONBLOCKING_ARGUMENT_TYPES))
			{
				slotN	= new NONBLOCKING_SLOT_GLOBAL_CLASS_NAME<slotReturnTYPE NONBLOCKING_CONDITIONAL_COMMA NONBLOCKING_ARGUMENT_TYPES>(proc);
				thread	= NIL;
			}

			template <class slotReturnTYPE> NONBLOCKING_NONBLOCKING_CLASS_NAME(NONBLOCKING_NONBLOCKING_CLASS_NAME<slotReturnTYPE NONBLOCKING_CONDITIONAL_COMMA NONBLOCKING_ARGUMENT_TYPES> *sig)
			{
				slotN	= new NONBLOCKING_SLOT_SIGNAL_CLASS_NAME<slotReturnTYPE NONBLOCKING_CONDITIONAL_COMMA NONBLOCKING_ARGUMENT_TYPES>(sig);
				thread	= NIL;
			}

			virtual ~NONBLOCKING_NONBLOCKING_CLASS_NAME()
			{
				if (thread == NIL) delete slotN;
			}


			Void Call(NONBLOCKING_ARGUMENT_PARAMETER_LIST)
			{
				NONBLOCKING_ARGUMENT_ASSIGNMENTS

				thread = new Threads::Thread();

				threadSlots.Add(slotN, thread->GetHandle());

				thread->SetFlags(Threads::THREAD_WAITFLAG_START);

				thread->threadMain.Connect(&NONBLOCKING_NONBLOCKING_CLASS_NAME::Thread, this);

				thread->Start();
			}

			Int Thread(Threads::Thread *thread)
			{
				NONBLOCKING_SLOT_BASE_CLASS_NAME<Void NONBLOCKING_CONDITIONAL_COMMA NONBLOCKING_ARGUMENT_TYPES>	*slot = (NONBLOCKING_SLOT_BASE_CLASS_NAME<Void NONBLOCKING_CONDITIONAL_COMMA NONBLOCKING_ARGUMENT_TYPES> *) threadSlots.Get(thread->GetHandle());

				threadSlots.Remove(thread->GetHandle());

				slot->Emit(NONBLOCKING_ARGUMENT_PARAMETERS);

				delete slot;
				delete thread;

				return Success();
			}
	};
};
