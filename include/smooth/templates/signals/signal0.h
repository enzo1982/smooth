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

#include "method0.h"
#include "instance0.h"

namespace smooth
{
	template <class r = Void> class Signal0 : public Signal
	{
		public:
			template <class ct> Int Connect(Void (ct::*proc)(), ct *inst)
			{
				instances.AddEntry(new Instance0<ct>(inst));
				methods.AddEntry(new Method0<Void (ct::*)()>(proc));

				nOfMethods++;

				return Success;
			}

			template <class ct> int Disconnect(Void (ct::*proc)(), ct *inst)
			{
				Instance0<ct>		*instance = new Instance0<ct>(inst);
				Method0<Void (ct::*)()>	*method = new Method0<Void (ct::*)()>(proc);

				for (Int i = 0; i < nOfMethods; i++)
				{
					if (*instances.GetNthEntry(i) == *instance && *methods.GetNthEntry(i) == *method)
					{
						delete instances.GetNthEntry(i);
						delete methods.GetNthEntry(i);

						instances.DeleteEntry(instances.GetNthEntryIndex(i));
						methods.DeleteEntry(methods.GetNthEntryIndex(i));

						nOfMethods--;

						break;
					}
				}

				delete instance;
				delete method;

				return Success;
			}

			Void Emit()
			{
				for (Int i = 0; i < nOfMethods; i++)
				{
					instances.GetNthEntry(i)->Call(methods.GetNthEntry(i));
				}
			}
	};
};
