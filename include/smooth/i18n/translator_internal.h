 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_I18N_TRANSLATOR_INTERNAL
#define H_OBJSMOOTH_I18N_TRANSLATOR_INTERNAL

#include "translator.h"

namespace smooth
{
	namespace I18n
	{
		class TranslatorInternal : public Translator
		{
			private:
				Int		 GetSupportedLanguages();
			public:
						 TranslatorInternal();
				virtual		~TranslatorInternal();
		};
	};
};

#endif
