 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/config.h>
#include <smooth/application.h>

S::Configuration::Configuration()
{
	ownRoot = NIL;

	configFile = NIL;
}

S::Configuration::~Configuration()
{
	if (configFile != NIL) Close();
}

S::Int S::Configuration::Open(String file, Bool create)
{
	configFile = new XML::Document();

	if (configFile->LoadFile(file) == Success)
	{
		fileName = file;
	}
	else if (configFile->LoadFile(Application::GetStartupDirectory().Append(file)) == Success)
	{
		fileName = Application::GetStartupDirectory().Append(file);
	}
	else if (configFile->LoadFile(Application::GetApplicationDirectory().Append(file)) == Success)
	{
		fileName = Application::GetApplicationDirectory().Append(file);
	}
	else
	{
		if (!create)
		{
			Close();

			return Error;
		}
		else
		{
			ownRoot = new XML::Node();

			ownRoot->SetName("ConfigFile");

			configFile->SetRootNode(ownRoot);

			ownRoot->AddNode("data");

			fileName = Application::GetApplicationDirectory().Append(file);
		}
	}

	if (configFile->GetRootNode() == NIL)
	{
		Close();

		return Error;
	}
	else if (configFile->GetRootNode()->GetNodeByName("data") == NIL)
	{
		Close();

		return Error;
	}

	return Success;
}

S::Int S::Configuration::Close()
{
	if (configFile != NIL)
	{
		if (fileName != NIL && configFile->GetRootNode() != NIL)
		{
			if (configFile->GetRootNode()->GetNodeByName("data") != NIL)
			{
				if (configFile->GetRootNode()->GetNodeByName("data")->GetNOfNodes() >= 1) configFile->SaveFile(fileName);
			}
		}

		delete configFile;

		configFile = NIL;

		fileName = NIL;
	}

	if (ownRoot != NIL)
	{
		delete ownRoot;

		ownRoot = NIL;
	}

	return Success;
}

S::Int S::Configuration::GetIntValue(String section, String name, Int defValue)
{
	if (configFile == NIL) return defValue;

	XML::Node	*value = FindValueNode(section, name);

	if (value != NIL)	return value->GetContent().ToInt();
	else			return defValue;
}

S::Int S::Configuration::SetIntValue(String section, String name, Int newValue)
{
	if (configFile == NIL) return Error;

	XML::Node	*value = FindValueNode(section, name);

	if (value != NIL)
	{
		value->SetContent(String::FromInt(newValue));
	}
	else
	{
		if (FindSectionNode(section) == NIL)
		{
			value = configFile->GetRootNode()->GetNodeByName("data")->AddNode("section");

			value->SetAttribute("name", section);
		}

		value = FindSectionNode(section)->AddNode("value", String::FromInt(newValue));

		value->SetAttribute("name", name);
	}

	return Success;
}

S::String S::Configuration::GetStringValue(String section, String name, String defValue)
{
	if (configFile == NIL) return defValue;

	XML::Node	*value = FindValueNode(section, name);

	if (value != NIL)	return value->GetContent();
	else			return defValue;
}

S::Int S::Configuration::SetStringValue(String section, String name, String newValue)
{
	if (configFile == NIL) return Error;

	XML::Node	*value = FindValueNode(section, name);

	if (value != NIL)
	{
		value->SetContent(newValue);
	}
	else
	{
		if (FindSectionNode(section) == NIL)
		{
			value = configFile->GetRootNode()->GetNodeByName("data")->AddNode("section");

			value->SetAttribute("name", section);
		}

		value = FindSectionNode(section)->AddNode("value", newValue);

		value->SetAttribute("name", name);
	}

	return Success;
}

S::XML::Node *S::Configuration::FindSectionNode(String section)
{
	XML::Node	*data = configFile->GetRootNode()->GetNodeByName("data");

	for (Int i = 0; i < data->GetNOfNodes(); i++)
	{
		XML::Node	*node = data->GetNthNode(i);
		XML::Attribute	*attributeName = node->GetAttributeByName("name");

		if (attributeName != NIL)
		{
			if (attributeName->GetContent() == section) return node;
		}
	}

	return NIL;
}

S::XML::Node *S::Configuration::FindValueNode(String sect, String name)
{
	XML::Node	*section = FindSectionNode(sect);

	if (section == NIL) return NIL;

	for (Int i = 0; i < section->GetNOfNodes(); i++)
	{
		XML::Node	*node = section->GetNthNode(i);
		XML::Attribute	*attributeName = node->GetAttributeByName("name");

		if (attributeName != NIL)
		{
			if (attributeName->GetContent() == name) return node;
		}
	}

	return NIL;
}
