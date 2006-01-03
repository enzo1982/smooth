 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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

#include "../widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI TabWidget : public Widget
		{
			protected:
				Array<Int>		 textSize;
				Array<Bool>		 sizeSet;

				Void			 GetLayersSize();
			public:
				static const Int	 classID;

							 TabWidget(const Point &, const Size &);
				virtual			~TabWidget();

				virtual Int		 Paint(Int);
				virtual Int		 Process(Int, Int, Int);

				virtual Int		 RegisterObject(Widget *);
				virtual Int		 UnregisterObject(Widget *);

				Int			 SelectTab(Int);
		};
	};
};

#endif
