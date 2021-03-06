 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_OBJECTTYPE
#define H_OBJSMOOTH_OBJECTTYPE

namespace smooth
{
	class Object;
	class ObjectType;
};

#include "../definitions.h"

namespace smooth
{
	class SMOOTHAPI ObjectType
	{
		private:
			Short			 type;
			Object			*object;
		public:
						 ObjectType(Object *);
						 ObjectType(const ObjectType &);

			operator		 Short() const		 { return type; }
			Short operator		 =(Short newType)	 { type = newType; return type; }

			Bool operator		 ==(Short) const;
			inline Bool operator	 !=(Short objType) const { return !(*this == objType); }
	};
};

#endif
