 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SMOOTHARRAY_ENTRY_
#define _H_SMOOTHARRAY_ENTRY_

#define SArray_Entry SMOOTHArray_Entry

#include "value.h"

template <class t> class SMOOTHArray_Entry : public SMOOTHArray_Value<t>
{
	private:
		SMOOTHArray_Entry<t>		*nextEntry;
		SMOOTHArray_Entry<t>		*prevEntry;
	public:
		bool				 gotNext;
		bool				 gotPrev;
						 SMOOTHArray_Entry();
						~SMOOTHArray_Entry();
		bool				 SetNext(SMOOTHArray_Entry<t> *);
		bool				 SetPrev(SMOOTHArray_Entry<t> *);
		inline SMOOTHArray_Entry<t>	*GetNext();
		inline SMOOTHArray_Entry<t>	*GetPrev();
};

#endif
