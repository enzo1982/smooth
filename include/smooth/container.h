 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CONTAINER_
#define _H_OBJSMOOTH_CONTAINER_

namespace smooth
{
	class Container;
	class Object;

	namespace GUI
	{
		class Surface;
		class Window;
	};
};

#include "array.h"
#include "containertype.h"

namespace smooth
{
	class SMOOTHAPI Container
	{
		protected:
			ContainerType		 containerType;
			Object			*self;

			Int			 nOfObjects;
			Array<Object *>		 assocObjects;

			GUI::Surface		*nullSurface;
			GUI::Surface		*drawSurface;
		public:
			static const Int	 classID;

						 Container();
			virtual			~Container();

			virtual Int		 RegisterObject(Object *);
			virtual Int		 UnregisterObject(Object *);

			Int			 GetNOfObjects();
			Object			*GetNthObject(Int);
			Object			*RequestObject(Int);

			virtual Bool		 IsContainerCompatible(Int);
			ContainerType		 GetContainerType();

			Object			*GetContainerObject();
			GUI::Window		*GetContainerWindow();

			virtual GUI::Surface	*GetDrawSurface();
	};
};

#endif
