 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

namespace smooth
{
	template <class returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_LIST> class SIGNALS_SIGNAL_CLASS_NAME;
};

namespace smooth
{
	template <class returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_LIST> class SIGNALS_SLOT_SIGNAL_CLASS_NAME : public SIGNALS_SLOT_BASE_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>
	{
		private:
			SIGNALS_SIGNAL_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>		*signal;
		public:
															 SIGNALS_SLOT_SIGNAL_CLASS_NAME(SIGNALS_SIGNAL_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *iSignal)	{ signal = iSignal; }
			SIGNALS_SLOT_BASE_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>	*Copy() const																{ return new SIGNALS_SLOT_SIGNAL_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(signal); }

			returnTYPE Emit(SIGNALS_ARGUMENT_PARAMETER_LIST) const
			{
				return signal->Emit(SIGNALS_ARGUMENT_PARAMETERS);
			}

			Bool operator ==(const SIGNALS_SLOT_BASE_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> &cInstance) const
			{
				/* Compare the raw bytes of the signal pointer.
				 *
				 * The previous code would cast cInstance to this class' type and then compare
				 * the members. cInstance can have different template arguments, though, making
				 * it a different type and rendering that cast undefined behavior.
				 */
				UnsignedInt	 offset = (UnsignedByte *) &signal - (UnsignedByte *) this;

				return memcmp((UnsignedByte *) this + offset, (UnsignedByte *) &cInstance + offset, sizeof(*this) - offset) == 0;
			}
	};
};
