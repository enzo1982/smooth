 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_I18N_
#define _H_OBJSMOOTH_I18N_

#include "string.h"
#include "xml/document.h"

namespace smooth
{
	SMOOTHVAR Int	 I18N_DEFAULTFONTSIZE;
	SMOOTHVAR Int	 I18N_SMALLFONTSIZE;
	SMOOTHVAR String I18N_DEFAULTFONT;

	namespace I18n
	{
		class SMOOTHAPI Language
		{
			public:
				Array<String>	 strings;

				String		 name;
				String		 encoding;
				String		 magic;
				String		 author;
				String		 url;
				Bool		 rightToLeft;

						 Language();
						~Language();
		};

		class SMOOTHAPI Translator
		{
			private:
				Int			 internal;

				Array<Language *>	 languages;
				Language		*activeLanguage;

				Int			 GetSupportedLanguages();
				Int			 LoadDoc(XML::Document *, Language *);
			public:
							 Translator(Bool = False);
							~Translator();

				Int			 SetInternalLanguageInfo(String, String, String, Bool);

				Int			 GetNOfLanguages();

				String			 GetNthLanguageName(Int);
				String			 GetNthLanguageID(Int);
				String			 GetNthLanguageAuthor(Int);
				String			 GetNthLanguageEncoding(Int);
				String			 GetNthLanguageURL(Int);

				Int			 ActivateLanguage(String);
				String			 TranslateString(String);
		};
	};
};

#endif
