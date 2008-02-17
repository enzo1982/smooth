 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/tree/treeview.h>

const S::Int	 S::GUI::TreeView::classID = S::Object::RequestClassID();

S::GUI::TreeView::TreeView(const Point &iPos, const Size &iSize) : ListBox(iPos, iSize)
{
	type	= classID;
}

S::GUI::TreeView::~TreeView()
{
}

S::Bool S::GUI::TreeView::IsTypeCompatible(Int compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == ListBox::classID)	return True;
	else												return False;
}
