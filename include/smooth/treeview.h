 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TREEVIEW_
#define _H_OBJSMOOTH_TREEVIEW_

#define STreeView SMOOTHTreeView

class SMOOTHTreeView;

#include "object.h"
#include "tree.h"

class SMOOTHAPI SMOOTHTreeView : public SMOOTHObject, public SMOOTHTree
{
	private:
		SMOOTHInt		 PaintTree(SMOOTHTree *, SMOOTHInt, SMOOTHRect);
	public:
					 SMOOTHTreeView(SMOOTHString, SMOOTHPoint, SMOOTHSize, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHTreeView();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_TREEVIEW;

#endif
