 /* IOLib-C++, Universal IO Library
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public
  * License along with this library; if not, write to the Free
  * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  * MA 02111-1307, USA */

#ifndef __IOLIB_FILTER_
#define __IOLIB_FILTER_

#include <iolib-cxx.h>

IOLibFilter::IOLibFilter()
{
	packageSize = 0;
}

IOLibFilter::~IOLibFilter()
{
}

bool IOLibFilter::Activate()
{
	return true;
}

bool IOLibFilter::Deactivate()
{
	return true;
}

bool IOLibFilter::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool IOLibFilter::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

#endif
