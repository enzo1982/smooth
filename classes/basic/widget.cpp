 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/widget.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_WIDGET = S::Object::RequestObjectID();

S::GUI::Widget::Widget()
{
}

S::GUI::Widget::~Widget()
{
}
