 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SMOOTHARRAY_INCLUDE_
#define _H_SMOOTHARRAY_INCLUDE_

#ifdef _MSC_VER
#define ARRAY_NULLVALUE		(0)
#define ARRAY_NULLPOINTER	(0)
#else
#define ARRAY_NULLVALUE		((t)				0)
#define ARRAY_NULLPOINTER	((smooth::Array_Entry<t> *)	0)
#endif

#include "templates/array/entry.h"
#include "templates/array/array.h"

#include "templates/array/entry.cpp"
#include "templates/array/array.cpp"

#endif
