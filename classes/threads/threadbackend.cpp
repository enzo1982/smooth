 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/threadbackend.h>

S::Threads::ThreadBackend::ThreadBackend(Void *iThread)
{
	type = THREAD_NONE;
}

S::Threads::ThreadBackend::~ThreadBackend()
{
}

S::Int S::Threads::ThreadBackend::GetThreadType()
{
	return type;
}

S::Void *S::Threads::ThreadBackend::GetSystemThread()
{
	return NIL;
}

S::Int S::Threads::ThreadBackend::GetThreadID()
{
	return -1;
}

S::Int S::Threads::ThreadBackend::Start(Void (*threadProc)(Void *), Void *threadParam)
{
	return Error;
}

S::Int S::Threads::ThreadBackend::Stop()
{
	return Error;
}

S::Void S::Threads::ThreadBackend::Exit()
{
}

S::Void *S::Threads::ThreadBackend::Self()
{
	return NIL;
}
