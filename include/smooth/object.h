 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECT_
#define _H_OBJSMOOTH_OBJECT_

namespace smooth
{
	class Object;
	class Container;
	class ObjectProperties;
};

#include "definitions.h"
#include "string.h"
#include "array.h"
#include "objecttype.h"
#include "point.h"
#include "size.h"
#include "signals.h"
#include "callbacks.h"

namespace smooth
{
	class SMOOTHAPI Object
	{
		private:
			static Int		 nextID;

			Bool			 deleteObject;
			Int			 inUse;
		protected:
			ObjectType		 type;

			Int			 flags;

			Bool			 registered;

			ObjectProperties	*objectProperties;
			Container		*myContainer;

			Void			 GetTextSize();

			Int			 EnterProtectedRegion();
			Int			 LeaveProtectedRegion();
		public:
			static const Int	 classID;

			Array<Int>		 possibleContainers;
			static Int		 objectCount;

			Int			 handle;

						 Object();
			virtual			~Object();

			Bool			 IsRegistered();

			Int			 SetFlags(Int);
			Int			 GetFlags();

			virtual Point		 GetRealPosition();

			virtual Bool		 IsTypeCompatible(Int);

			ObjectProperties	*GetObjectProperties();
			ObjectType		 GetObjectType();

			Int			 SetContainer(Container *);
			Container		*GetContainer();

			Void			 SetRegisteredFlag();
			Void			 UnsetRegisteredFlag();

			static Object		*GetObject(Int, Int);

			static Int		 RequestClassID();
			static Int		 RequestObjectHandle();

			Int			 IsObjectInUse();
			Bool			 IsObjectDeleteable();

			static Int		 DeleteObject(Object *);
	};
};

#endif
