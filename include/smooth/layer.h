 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LAYER_
#define _H_OBJSMOOTH_LAYER_

namespace smooth
{
	namespace GUI
	{
		class Layer;
	};
};

#include "widget.h"
#include "container.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Layer : public Widget, public Container
		{
			protected:
				Int			 layerColor;
			public:
				static const Int	 classID;

							 Layer(String name = NIL);
							~Layer();

				Int			 Show();
				Int			 Hide();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Int			 SetColor(Int);
				Int			 SetMetrics(Point, Size);

				Surface			*GetDrawSurface();

				Int			 RegisterObject(Object *);
				Int			 UnregisterObject(Object *);
		};
	};
};

#endif
