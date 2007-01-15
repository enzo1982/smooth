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

			Int		 Size() const;
			Int		 Resize(Int);

			Int		 Zero();
			Int		 Free();

			t &operator	 [](const int);
			t &operator	 [](const Int);

			t operator	 [](const int) const;
			t operator	 [](const Int) const;

			t *operator	 +(const int);

			operator	 t *();
			operator	 const t *() const;

			String		 EncodeBase64(Int) const;
			Int		 DecodeBase64(const String &);
	};
};

#endif
