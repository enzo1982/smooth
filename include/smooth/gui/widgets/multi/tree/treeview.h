 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TREEVIEW_
#define _H_OBJSMOOTH_TREEVIEW_

namespace smooth
{
	namespace GUI
	{
		class TreeView;
	};
};

#include "../../widget.h"
#include "tree.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI TreeView : public Widget, public Tree
		{
			private:
				Int			 PaintTree(Tree *, Int, Rect);
			public:
				static const Int	 classID;

							 TreeView(String, Point, Size);
							~TreeView();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);
		};
	};
};

#endif
