 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/charsetdetector_internal.h>

#include <string.h>

#if defined __WIN32__ || defined __APPLE__ || defined __linux__
S::I18n::CharsetDetectorInternal::CharsetDetectorInternal() : nsUniversalDetector(NS_FILTER_ALL)
#else
S::I18n::CharsetDetectorInternal::CharsetDetectorInternal()
#endif
{
}

S::I18n::CharsetDetectorInternal::~CharsetDetectorInternal()
{
}

S::String S::I18n::CharsetDetectorInternal::DetectCharset(const char *data)
{
#if defined __WIN32__ || defined __APPLE__ || defined __linux__
	/* Pass data to nsUniversalDetector and return the result.
	 */
	HandleData(data, strlen(data));
	DataEnd();
#endif

	return detectedCharset;
}

void S::I18n::CharsetDetectorInternal::Report(const char *charset)
{
	detectedCharset = charset;
}
