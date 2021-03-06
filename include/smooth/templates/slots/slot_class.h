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
	template <class classTYPE, class returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_LIST> class SIGNALS_SLOT_CLASS_CLASS_NAME : public SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>
	{
		private:
			classTYPE										*instance;
			returnTYPE										 (classTYPE::*method)(SIGNALS_ARGUMENT_TYPES);
		public:
														 SIGNALS_SLOT_CLASS_CLASS_NAME(returnTYPE (classTYPE::*iMethod)(SIGNALS_ARGUMENT_TYPES), classTYPE *iInstance)	{ method = iMethod; instance = iInstance; }
			SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>	*Copy() const													{ return new SIGNALS_SLOT_CLASS_CLASS_NAME<classTYPE, returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(method, instance); }

			Void Emit(SIGNALS_ARGUMENT_PARAMETER_LIST) const
			{
				(instance->*(method))(SIGNALS_ARGUMENT_PARAMETERS);
			}

			Bool operator ==(const SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> &cInstance) const
			{
				/* Compare the raw bytes of the instance and method pointers.
				 *
				 * The previous code would cast cInstance to this class' type and then compare
				 * the members. cInstance can have different template arguments, though, making
				 * it a different type and rendering that cast undefined behavior.
				 */
				UnsignedInt	 offset = (UnsignedByte *) &instance - (UnsignedByte *) this;

				return memcmp((UnsignedByte *) this + offset, (UnsignedByte *) &cInstance + offset, sizeof(*this) - offset) == 0;
			}
	};
};
