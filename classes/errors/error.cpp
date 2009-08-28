 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/errors/error.h>

S::String S::Errors::Error::ToString() const
{
	return "Unknown error";
}

S::Bool S::Errors::Error::operator ==(const Error &error) const
{
	if (code == error.code)	return True;
	else			return False;
}

S::Bool S::Errors::Error::operator !=(const Error &error) const
{
	return !(*this == error);
}
