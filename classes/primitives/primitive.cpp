 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_PRIMITIVE_
#define __OBJSMOOTH_PRIMITIVE_

#include <smooth/primitive.h>
#include <smooth/stk.h>

SMOOTHPrimitive::SMOOTHPrimitive()
{
}

SMOOTHPrimitive::~SMOOTHPrimitive()
{
}

SMOOTHInt SMOOTHPrimitive::Draw(SMOOTHDrawable *drawAble)
{
	return SMOOTH::Success;
}

#endif
