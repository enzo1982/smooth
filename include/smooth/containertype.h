 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CONTAINERTYPE_
#define _H_OBJSMOOTH_CONTAINERTYPE_

namespace smooth
{
	class Container;
	class ContainerType;
};

#include "definitions.h"

namespace smooth
{
	class SMOOTHAPI ContainerType
	{
		friend class SMOOTHAPI Container;
		private:
			Int		 type;
			Container	*container;
		public:
					 ContainerType();
					 ContainerType(const ContainerType &);

			operator	 Int();
			Int operator	 =(Int);

			Bool operator	 ==(Int);
			Bool operator	 !=(Int);
			Bool operator	 ==(Array<Int> *);
			Bool operator	 !=(Array<Int> *);
	};
};

#endif
