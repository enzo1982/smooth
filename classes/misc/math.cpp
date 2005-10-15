 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/math.h>
#include <math.h>

const S::Float S::Math::Pi	= 3.14159265358979323846;
const S::Float S::Math::e	= 2.7182818284590452354;

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

S::Float S::Math::Acos(Float f)
{
	return acos(f);
}

S::Float S::Math::Asin(Float f)
{
	return asin(f);
}

S::Float S::Math::Atan(Float f)
{
	return atan(f);
}

S::Float S::Math::Atan2(Float fy, Float fx)
{
	return atan2(fy, fx);
}

S::Int S::Math::Ceil(Float f)
{
	return (Int) ceil(f);
}

S::Float S::Math::Cos(Float f)
{
	return cos(f);
}

S::Float S::Math::Cosh(Float f)
{
	return cosh(f);
}

S::Float S::Math::Exp(Float f)
{
	return exp(f);
}

S::Int S::Math::Floor(Float f)
{
	return (Int) floor(f);
}

S::Float S::Math::Log(Float f)
{
	return log(f);
}

S::Float S::Math::Log10(Float f)
{
	return log10(f);
}

S::Float S::Math::Log2(Float f)
{
	return log(f) / log(2);
}

S::Float S::Math::Min(Float f1, Float f2)
{
	return (f1 < f2 ? f1 : f2);
}

S::Float S::Math::Max(Float f1, Float f2)
{
	return (f1 > f2 ? f1 : f2);
}

S::Float S::Math::Mod(Float fd, Float fb)
{
	return fmod(fd, fb);
}

S::Float S::Math::Pow(Float fn, Float fe)
{
	return pow(fn, fe);
}

S::Int S::Math::Round(Float f)
{
	return (Int) (f + 0.5);
}

S::Int S::Math::Sign(Float f)
{
	return (f == 0 ? 0 : (f < 0 ? -1 : 1));
}

S::Float S::Math::Sin(Float f)
{
	return sin(f);
}

S::Float S::Math::Sinh(Float f)
{
	return sinh(f);
}

S::Float S::Math::Sqrt(Float f)
{
	return sqrt(f);
}

S::Float S::Math::Tan(Float f)
{
	return tan(f);
}

S::Float S::Math::Tanh(Float f)
{
	return tanh(f);
}

S::Int S::Math::Abs(Int i)
{
	return i * Sign(i);
}

S::Int S::Math::Min(Int i1, Int i2)
{
	return (i1 < i2 ? i1 : i2);
}

S::Int S::Math::Max(Int i1, Int i2)
{
	return (i1 > i2 ? i1 : i2);
}

S::Int S::Math::Mod(Int id, Int ib)
{
	return id % ib;
}

S::Int S::Math::Pow(Int in, Int ie)
{
	Int	 value = 1;

	for (Int i = 0; i < ie; i++) value *= in;

	return value;
}
