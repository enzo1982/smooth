 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MATH
#define _H_OBJSMOOTH_MATH_

namespace smooth
{
	class Math;
};

#include "definitions.h"

namespace smooth
{
	class SMOOTHAPI Math
	{
		private:
						 Math();
						 Math(const Math &);
		public:
			static const Float	 Pi;
			static const Float	 e;

			static Float		 Abs(Float);
			static Float		 Acos(Float);
			static Float		 Asin(Float);
			static Float		 Atan(Float);
			static Float		 Atan2(Float, Float);
			static Int		 Ceil(Float);
			static Float		 Cos(Float);
			static Float		 Cosh(Float);
			static Float		 Exp(Float);
			static Int		 Floor(Float);
			static Float		 Log(Float);
			static Float		 Log10(Float);
			static Float		 Log2(Float);
			static Float		 Min(Float, Float);
			static Float		 Max(Float, Float);
			static Float		 Mod(Float, Float);
			static Float		 Pow(Float, Float);
			static Int		 Round(Float);
			static Int		 Sign(Float);
			static Float		 Sin(Float);
			static Float		 Sinh(Float);
			static Float		 Sqrt(Float);
			static Float		 Tan(Float);
			static Float		 Tanh(Float);
	};
};

#endif
