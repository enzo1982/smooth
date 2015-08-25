 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/access.h>
#include <smooth/threads/mutex.h>
#include <smooth/init.h>

S::Threads::Mutex	*S::Threads::Access::mutex = NIL;

S::Int	 addAccessInitTmp = S::AddInitFunction(&S::Threads::Access::Initialize);
S::Int	 addAccessFreeTmp = S::AddFreeFunction(&S::Threads::Access::Free);

S::Int S::Threads::Access::Initialize()
{
	mutex = new Mutex();

	return Success();
}

S::Int S::Threads::Access::Free()
{
	delete mutex;

	mutex = NIL;

	return Success();
}

S::Bool S::Threads::Access::Value(volatile Bool &value)
{
	if (mutex != NIL) mutex->Lock();

	Bool	 result = value;

	if (mutex != NIL) mutex->Release();

	return result;
}

S::Bool S::Threads::Access::Set(volatile Bool &value, Bool n)
{
	mutex->Lock();

	Bool	 result = value = n;

	mutex->Release();

	return result;
}

S::Short S::Threads::Access::Value(volatile Short &value)
{
	if (mutex != NIL) mutex->Lock();

	Short	 result = value;

	if (mutex != NIL) mutex->Release();

	return result;
}

S::Short S::Threads::Access::Increment(volatile Short &value)
{
	mutex->Lock();

	Short	 result = ++value;

	mutex->Release();

	return result;
}

S::Short S::Threads::Access::Decrement(volatile Short &value)
{
	mutex->Lock();

	Short	 result = --value;

	mutex->Release();

	return result;
}

S::Short S::Threads::Access::Set(volatile Short &value, Short n)
{
	mutex->Lock();

	Short	 result = value = n;

	mutex->Release();

	return result;
}

S::Int S::Threads::Access::Value(volatile Int &value)
{
	mutex->Lock();

	Int	 result = value;

	mutex->Release();

	return result;
}

S::Int S::Threads::Access::Increment(volatile Int &value)
{
	mutex->Lock();

	Int	 result = ++value;

	mutex->Release();

	return result;
}

S::Int S::Threads::Access::Decrement(volatile Int &value)
{
	mutex->Lock();

	Int	 result = --value;

	mutex->Release();

	return result;
}

S::Int S::Threads::Access::Set(volatile Int &value, Int n)
{
	mutex->Lock();

	Int	 result = value = n;

	mutex->Release();

	return result;
}
