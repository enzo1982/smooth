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
	class ObjectProperties;

	namespace GUI
	{
		class Container;
	};
};

#include "definitions.h"
#include "misc/string.h"
#include "array.h"
#include "objecttype.h"

namespace smooth
{
	abstract class SMOOTHAPI Object
	{
		friend class Signal;
		private:
			static Int		 nextID;

			static Array<Object *>	 objects;

			Bool			 deleteObject;
			Int			 inUse;
		protected:
			Int			 flags;
			ObjectType		 type;

			Bool			 registered;

			ObjectProperties	*objectProperties;
			GUI::Container		*myContainer;

			Int			 EnterProtectedRegion();
			Int			 LeaveProtectedRegion();
		public:
			static Int		 GetNOfObjects();
			static Object		*GetNthObject(Int);
			static Object		*RequestObject(Int);

			static const Int	 classID;

			Array<Int>		 possibleContainers;
			static Int		 objectCount;

			Int			 handle;

						 Object();
			virtual			~Object();

			Bool			 IsRegistered();

			Int			 SetFlags(Int);
			Int			 GetFlags();

			virtual Bool		 IsTypeCompatible(Int);

			ObjectProperties	*GetObjectProperties();
			ObjectType		 GetObjectType();

			Int			 SetContainer(GUI::Container *);
			GUI::Container		*GetContainer();

			Void			 SetRegisteredFlag();
			Void			 UnsetRegisteredFlag();

			Int			 IsObjectInUse();
			Bool			 IsObjectDeleteable();

			static Int		 RequestClassID();
			static Int		 RequestObjectHandle();

			static Object		*GetObject(Int, Int);

			static Int		 DeleteObject(Object *);
	};
};

#endif
