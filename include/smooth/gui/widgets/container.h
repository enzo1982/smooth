 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CONTAINER_
#define _H_OBJSMOOTH_CONTAINER_

namespace smooth
{
	namespace GUI
	{
		class Container;
		class Widget;
		class Surface;
		class Window;
	};
};

#include "containertype.h"
#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		abstract class SMOOTHAPI Container : public Widget
		{
			protected:
				ContainerType		 containerType;

				Array<Widget *>		 assocObjects;

				Surface			*nullSurface;
				Surface			*drawSurface;
			public:
				static const Int	 classID;

							 Container();
				virtual			~Container();

				virtual Int		 RegisterObject(Widget *);
				virtual Int		 UnregisterObject(Widget *);

				Int			 GetNOfObjects();
				Widget			*GetNthObject(Int);
				Widget			*RequestObject(Int);

				virtual Bool		 IsContainerCompatible(Int);
				ContainerType		 GetContainerType();

				Window			*GetContainerWindow();

				virtual Surface		*GetDrawSurface();
		};
	};
};

#endif
