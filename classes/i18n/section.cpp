 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/section.h>
#include <smooth/foreach.h>

S::I18n::Section::Section()
{
	contextSection = NIL;
}

S::I18n::Section::~Section()
{
	foreach (Section *section, sections) delete section;
}

const S::String &S::I18n::Section::GetString(const String &string)
{
	if (contextSection != NIL)
	{
		const String	&result = contextSection->GetString(string);

		if (result != NIL) return result;
	}

	return strings.GetReference(string.ComputeCRC32());
}

S::Bool S::I18n::Section::SetContext(const String &nContext)
{
	context		= nContext;
	contextSection	= NIL;

	if (context != NIL)
	{
		Int	 index = context.Find("::");

		String	 contextFirst = context;
		String	 contextRest;

		if (index >= 0)
		{
			contextFirst = context.Head(index);
			contextRest = context.Tail(context.Length() - index - 2);
		}

		foreach (Section *section, sections)
		{
			if (section->GetName() == contextFirst)
			{
				section->SetContext(contextRest);

				contextSection = section;

				return True;
			}
		}

		return False;
	}

	return True;
}

S::Errors::Error S::I18n::Section::Parse(XML::Node *section)
{
	if (section == NIL) return Error();

	static String	 sectionNode	 = "section";
	static String	 nameAttribute	 = "name";
	static String	 entryNode	 = "entry";
	static String	 stringAttribute = "string";

	if (section->GetAttributeByName(nameAttribute) != NIL) SetName(section->GetAttributeByName(nameAttribute)->GetContent());

	for (Int i = 0; i < section->GetNOfNodes(); i++)
	{
		XML::Node	*entry = section->GetNthNode(i);

		if (entry->GetName() == entryNode && entry->GetAttributeByName(stringAttribute) != NIL)
		{
			strings.Add(entry->GetContent(), entry->GetAttributeByName(stringAttribute)->GetContent().ComputeCRC32());
		}
		else if (entry->GetName() == sectionNode && entry->GetAttributeByName(nameAttribute) != NIL)
		{
			Section	*section = new Section();

			section->Parse(entry);

			sections.Add(section, section->GetName().ComputeCRC32());
		}
	}

	return Success();
}
