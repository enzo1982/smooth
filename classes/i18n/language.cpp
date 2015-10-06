 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/language.h>

S::I18n::Language::Language()
{
	rightToLeft = False;
	incomplete  = False;
}

S::I18n::Language::~Language()
{
}

S::Errors::Error S::I18n::Language::ParseHeader(XML::Node *language)
{
	if (language == NIL) return Error();

	XML::Node	*info = language->GetNodeByName("info");

	/* Return an error if we didn't find an info node.
	 */
	if (info == NIL) return Error();

	for (Int i = 0; i < info->GetNOfNodes(); i++)
	{
		String	 property = info->GetNthNode(i)->GetAttributeByName("name")->GetContent();

		if (property == "language")	 SetName(info->GetNthNode(i)->GetContent());

		if (property == "righttoleft")	 rightToLeft = (info->GetNthNode(i)->GetContent() == "true" ? True : False);
		if (property == "encoding")	 encoding    =  info->GetNthNode(i)->GetContent();
		if (property == "author")	 author	     =  info->GetNthNode(i)->GetContent();
		if (property == "url")		 url	     =  info->GetNthNode(i)->GetContent();
		if (property == "incomplete")	 incomplete  = (info->GetNthNode(i)->GetContent() == "true" ? True : False);
	}

	return Success();
}

S::Errors::Error S::I18n::Language::Parse(XML::Node *language)
{
	if (language == NIL) return Error();

	XML::Node	*data = language->GetNodeByName("data");

	/* Return an error if we didn't find a data node.
	 */
	if (data == NIL) return Error();

	return Section::Parse(data);
}
