 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TABWIDGET_
#define _H_OBJSMOOTH_TABWIDGET_

namespace smooth
{
	namespace GUI
	{
		class TabWidget;
	};
};

#include "widget.h"
#include "container.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI TabWidget : public Widget, public Container
		{
			protected:
				Array<Int>	 textSize;
				Array<Bool>	 sizeSet;

				Void		 GetSize();
			public:
						 TabWidget(Point, Size);
						~TabWidget();

				virtual Int	 Paint(Int);
				Int		 Process(Int, Int, Int);

				Int		 SelectTab(Int);

				Int		 RegisterObject(Object *);
				Int		 UnregisterObject(Object *);
		};
	};

	SMOOTHVAR Int OBJ_TABREGISTER;
};

#endif
