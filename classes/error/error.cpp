 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/error/error.h>
#include <smooth/error/success.h>

const S::Int	 S::Error::Error::classID = S::Object::RequestClassID();

S::Error::Error::Error(String iMessage)
{
	type = classID;

	if (iMessage != NIL)	message = iMessage;
	else			message = "No error message provided!";
}

S::Error::Error::Error(const Error &oError)
{
	message = oError.message;
}

S::Error::Error::~Error()
{
}

S::String S::Error::Error::ToString()
{
	return GetErrorMessage();
}

S::String S::Error::Error::GetErrorMessage()
{
	return message;
}

S::Error::Error S::Error::Error::SetErrorMessage(String nMessage)
{
	if (nMessage != NIL)	message = nMessage;
	else			message = "No error message provided!";

	return Success();
}

S::Error::Error::operator S::Bool()
{
	return True;
}

S::Bool S::Error::Error::operator ==(const Error &error)
{
	if (message == error.message)	return True;
	else				return False;
}

S::Bool S::Error::Error::operator !=(const Error &error)
{
	return !(*this == error);
}
