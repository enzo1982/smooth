 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECT_
#define _H_OBJSMOOTH_OBJECT_

namespace smooth
{
	class Object;
};

#include "../definitions.h"
#include "../misc/string.h"
#include "../array.h"
#include "objecttype.h"

namespace smooth
{
	abstract class SMOOTHAPI Object
	{
		friend class Signal;
		private:
			static Int		 nextClassID;
			static Int		 nextObjectHandle;

			static Array<Object *>	 objects;

			Int			 handle;
			String			 name;

			Bool			 deleteObject;
			Int			 isObjectInUse;
		protected:
			ObjectType		 type;

			Int			 flags;

			Int			 EnterProtectedRegion();
			Int			 LeaveProtectedRegion();
		public:
			static const Int	 classID;

			static Int		 RequestClassID();
			static Int		 RequestObjectHandle();

			static Int		 GetNOfObjects();
			static Object		*GetNthObject(Int);

			static Object		*GetObject(Int, Int = Object::classID);
			static Object		*GetObject(String);

			static Int		 DeleteObject(Object *);

						 Object();
			virtual			~Object();

			Int			 GetHandle();

			Int			 SetName(String);
			String			 GetName();

			Int			 SetFlags(Int);
			Int			 GetFlags();

			ObjectType		 GetObjectType();
			virtual Bool		 IsTypeCompatible(Int);

			Bool			 IsObjectInUse();
			Bool			 IsObjectDeletable();
	};
};

#endif
