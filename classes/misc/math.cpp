 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/math.h>
#include <smooth/threads/thread.h>
#include <smooth/threads/mutex.h>

#include <math.h>
#include <time.h>
#include <stdlib.h>

namespace smooth
{
	static Threads::Mutex	 randMutex;
}

const S::Float S::Math::Pi = 3.1415926535897932385;
const S::Float S::Math::e  = 2.7182818284590452354;

S::Math::Math()
{
}

S::Math::Math(const Math &)
{
}

S::Float S::Math::Abs(Float f)
{
	return fabs(f);
}

S::Float S::Math::Fract(Float f)
{
	return f - Floor(f);
}

S::Int64 S::Math::Floor(Float f)
{
	return (Int64) floor(f);
}

S::Int64 S::Math::Ceil(Float f)
{
	return (Int64) ceil(f);
}

S::Float S::Math::Mod(Float fd, Float fb)
{
	return fmod(fd, fb);
}

S::Float S::Math::Pow(Float fn, Float fe)
{
	return pow(fn, fe);
}

S::Float S::Math::Sqrt(Float f)
{
	return sqrt(f);
}

S::Float S::Math::Log(Float f)
{
	return log(f);
}

S::Float S::Math::Log2(Float f)
{
	return log(f) / log(2.0);
}

S::Float S::Math::Log10(Float f)
{
	return log10(f);
}

S::Float S::Math::Exp(Float f)
{
	return exp(f);
}

S::Float S::Math::Sin(Float f)
{
	return sin(f);
}

S::Float S::Math::Cos(Float f)
{
	return cos(f);
}

S::Float S::Math::Tan(Float f)
{
	return tan(f);
}

S::Float S::Math::Sinh(Float f)
{
	return sinh(f);
}

S::Float S::Math::Cosh(Float f)
{
	return cosh(f);
}

S::Float S::Math::Tanh(Float f)
{
	return tanh(f);
}
S::Float S::Math::Asin(Float f)
{
	return asin(f);
}

S::Float S::Math::Acos(Float f)
{
	return acos(f);
}

S::Float S::Math::Atan(Float f)
{
	return atan(f);
}

S::Float S::Math::Atan2(Float fy, Float fx)
{
	return atan2(fy, fx);
}

S::Float S::Math::Pow(Int in, Int ie)
{
	Float	 value = 1;

	if (ie >= 0) for (Int i = 0; i <  ie; i++) value *= in;
	else	     for (Int i = 0; i < -ie; i++) value /= in;

	return value;
}

S::Float S::Math::Pow(Int64 in, Int64 ie)
{
	Float	 value = 1;

	if (ie >= 0) for (Int i = 0; i <  ie; i++) value *= in;
	else	     for (Int i = 0; i < -ie; i++) value /= in;

	return value;
}

S::Void S::Math::RandomSeed()
{
	Threads::Lock	 lock(randMutex);

	/* Seed the random number generator using a combination of the
	 * current time, ticks since program start and the current thread ID.
	 */
	srand(time(NULL) ^ clock() ^ Threads::Thread::GetCurrentThreadID());
}

S::Int32 S::Math::Random()
{
	Threads::Lock	 lock(randMutex);

	/* rand() may generate only 15 bit numbers (e.g. on Windows),
	 * so combine multiple results to generate a 32 bit value.
	 */
	Int32	 value = 0;

	for (Int i = 0; i < 3; i++) value = (value << 12) | (rand() & 0xFFF);

	return value;
}
