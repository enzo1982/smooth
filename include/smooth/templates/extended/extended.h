 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_EXTENDED_
#define _H_EXTENDED_

#include "../../callbacks.h"

namespace smooth
{
	template <class t> class Extended
	{
		private:
			t			 var;

			t			&DefaultRead(t &);
			t			&DefaultWrite(t &);
		public:
						 Extended();
						 Extended(const t &);

			t &operator		 =(t &);

			t &operator		 ++();
			t operator		 ++(int);

			t &operator		 --();
			t operator		 --(int);

			Bool operator		 ==(t &);
			Bool operator		 !=(t &);

			Bool operator		 <(t &);
			Bool operator		 >(t &);
			Bool operator		 <=(t &);
			Bool operator		 >=(t &);

			t &operator		 +=(t &);
			t &operator		 -=(t &);
			t &operator		 *=(t &);
			t &operator		 /=(t &);
			t &operator		 %=(t &);

			t &operator		 &=(t &);
			t &operator		 ^=(t &);
			t &operator		 |=(t &);

			t &operator		 <<=(t &);
			t &operator		 >>=(t &);

			operator		 t();

			t &operator		 *();
			t *operator		 ->();
		callbacks:
			Callback1<t &, t &>	 onRead;
			Callback1<t &, t &>	 onWrite;
	};
};

#endif
