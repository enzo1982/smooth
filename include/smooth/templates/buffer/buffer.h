 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BUFFER_
#define _H_OBJSMOOTH_BUFFER_

namespace smooth
{
	class Memory;
};

#include "../../definitions.h"

namespace smooth
{
	template <class t> class Buffer
	{
		private:
			Memory		*memory_manager;
			t		*memory;

			Int		 size;
			Int		 allocated;
		public:
					 Buffer();
					 Buffer(Int);
					 Buffer(const Buffer<t> &);

					~Buffer();

			Int		 Size() const		{ return size; }
			Bool		 Resize(Int);

			Bool		 Zero();
			Bool		 Free();

			t &operator	 [](const int n)	{ return memory[n]; }
			t &operator	 [](const Int n)	{ return (*this)[(int) n]; }

			t operator	 [](const int n) const	{ return memory[n]; }
			t operator	 [](const Int n) const	{ return (*this)[(int) n]; }

			operator	 t *() const		{ return memory; }
	};
};

#endif
