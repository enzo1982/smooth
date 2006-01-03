 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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
#define ARRAY_NULLVALUE		((s)				0)
#define ARRAY_NULLPOINTER	((smooth::Array_Entry<s> *)	0)
#endif

#include "array/entry.h"
#include "array/backend.h"
#include "array/array.h"

#include "array/backend.cpp"

#endif
