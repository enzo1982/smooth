 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TREEVIEW_
#define _H_OBJSMOOTH_TREEVIEW_

namespace smooth
{
	class TreeView;
};

#include "object.h"
#include "tree.h"

namespace smooth
{
	class SMOOTHAPI TreeView : public Object, public Tree
	{
		private:
			Int		 PaintTree(Tree *, Int, Rect);
		public:
					 TreeView(String, Point, Size);
					~TreeView();

			virtual Int	 Paint(Int);
			Int		 Process(Int, Int, Int);
	};

	SMOOTHVAR Int OBJ_TREEVIEW;
};

#endif
