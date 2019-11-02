 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_MATH
#define H_OBJSMOOTH_MATH

namespace smooth
{
	class Math;
};

#include "../definitions.h"

namespace smooth
{
	class SMOOTHAPI Math
	{
		private:
								 Math();
								 Math(const Math &);
		public:
			static const Float			 Pi;
			static const Float			 e;

			static Float				 Abs(Float);
			static Int64				 Round(Float f)		{ return (Int64) (f + (f < 0 ? -0.5 : 0.5)); }
			static Float				 Fract(Float);
			static Int64				 Floor(Float);
			static Int64				 Ceil(Float);
			static Float				 Mod(Float, Float);

			static Float				 Pow(Float, Float);
			static Float				 Sqrt(Float);
			static Float				 Log(Float);
			static Float				 Log2(Float);
			static Float				 Log10(Float);
			static Float				 Exp(Float);

			static Float				 Sin(Float);
			static Float				 Cos(Float);
			static Float				 Tan(Float);

			static Float				 Sinh(Float);
			static Float				 Cosh(Float);
			static Float				 Tanh(Float);

			static Float				 Asin(Float);
			static Float				 Acos(Float);
			static Float				 Atan(Float);
			static Float				 Atan2(Float, Float);

			static Float				 Pow(Int, Int);
			static Float				 Pow(Int64, Int64);

			template <class T> static T		 Sign(T v)		{ return (v == 0 ? 0 : (v < 0 ? -1 : 1)); }
			template <class T> static T		 Abs(T v)		{ return v * Sign(v); }

			template <class T1, class T2> static T1	 Mod(T1 dd, T2 ds)	{ return dd % ds; }

			template <class T1, class T2> static T1	 Min(T1 v1, T2 v2)	{ return (v1 < T1(v2) ? v1 : v2); }
			template <class T1, class T2> static T1	 Max(T1 v1, T2 v2)	{ return (v1 > T1(v2) ? v1 : v2); }

			static Void				 RandomSeed();
			static Int32				 Random();
	};
};

#endif
