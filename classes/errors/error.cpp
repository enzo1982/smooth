 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/errors/error.h>

S::Errors::Error::Error()
{
	code = -1;
}

S::Errors::Error::Error(const Error &oError)
{
	code = oError.code;
}

S::Errors::Error::~Error()
{
}

S::String S::Errors::Error::ToString()
{
	return "Unknown error";
}

S::Errors::Error::operator S::Bool() const
{
	return False;
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
