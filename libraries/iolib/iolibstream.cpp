 /* IOLib-C++, Universal IO Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
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

#include <iolib-cxx.h>
#include <string.h>

#if !defined __WIN32__ || defined __CYGWIN32__
	#include <unistd.h>
#endif

#if !defined __WIN32__
	#include <sys/ioctl.h>
#endif

IOLibStream::IOLibStream()
{
	streamType	= STREAM_NONE;
	size		= 0;
	currentFilePos	= 0;
	currentBufferPos= 0;
	pbdActive	= false;
	keepPbd		= false;
	pbdLength	= 0;
	data		= NULL;
	closefile	= true;
	crosslinked	= false;
	driver		= NULL;
	filter		= NULL;
	allowpackset	= true;
	packageSize	= DEFAULT_PACKAGE_SIZE;
	stdpacksize	= packageSize;
	origpacksize	= packageSize;
	origsize	= 0;
	origfilepos	= 0;
	lastError	= IOLIB_ERROR_OK;
}

IOLibStream::~IOLibStream()
{
}

int IOLibStream::GetStreamType()
{
	return streamType;
}

long IOLibStream::Size()
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return -1; }

	return size;
}

long IOLibStream::GetPos()
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return -1; }

	return currentFilePos;
}

int IOLibStream::GetLastError()
{
	return lastError;
}
