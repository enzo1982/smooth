 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/config.h>
#include <smooth/gui/application/application.h>

S::Configuration::Configuration()
{
	ownRoot = NIL;

	configFile = NIL;

	activeConfig = "default";
}

S::Configuration::Configuration(String file, Bool create)
{
	ownRoot = NIL;

	configFile = NIL;

	activeConfig = "default";

	Open(file, create);
}

S::Configuration::~Configuration()
{
	if (configFile != NIL) Close();
}

S::Int S::Configuration::Open(String file, Bool create)
{
	if (configFile != NIL) Close();

	configFile = new XML::Document();

	if (configFile->LoadFile(file) == Success)
	{
		fileName = file;
	}
	else if (configFile->LoadFile(GUI::Application::GetStartupDirectory().Append(file)) == Success)
	{
		fileName = GUI::Application::GetStartupDirectory().Append(file);
	}
	else if (configFile->LoadFile(GUI::Application::GetApplicationDirectory().Append(file)) == Success)
	{
		fileName = GUI::Application::GetApplicationDirectory().Append(file);
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

			ownRoot->AddNode("configuration")->SetAttribute("name", "default");

			fileName = GUI::Application::GetApplicationDirectory().Append(file);
		}
	}

	if (configFile->GetRootNode() == NIL)
	{
		Close();

		return Error;
	}
	else if (configFile->GetRootNode()->GetNodeByName("configuration") == NIL)
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
			if (configFile->GetRootNode()->GetNodeByName("configuration") != NIL)
			{
				if (configFile->GetRootNode()->GetNodeByName("configuration")->GetNOfNodes() >= 1) configFile->SaveFile(fileName);
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

S::Int S::Configuration::SetActiveConfiguration(String nConfig)
{
	if (configFile == NIL) return Error;

	XML::Node	*configuration = FindConfigurationNode(nConfig);

	if (configuration == NIL)
	{
		configFile->GetRootNode()->AddNode("configuration")->SetAttribute("name", nConfig);
	}

	activeConfig = nConfig;

	return Success;
}

S::Int S::Configuration::SetConfigurationName(String nName)
{
	if (configFile == NIL) return Error;

	XML::Node	*configuration = FindConfigurationNode(activeConfig);

	if (configuration == NIL) return Error;

	activeConfig = nName;

	configuration->SetAttribute("name", nName);

	return Success;
}

S::Int S::Configuration::SetParentConfiguration(String nParent)
{
	if (configFile == NIL) return Error;

	XML::Node	*configuration = FindConfigurationNode(activeConfig);

	if (configuration == NIL) return Error;

	configuration->SetAttribute("inheriting", nParent);

	return Success;
}

S::Int S::Configuration::GetIntValue(String section, String name, Int defValue)
{
	if (configFile == NIL) return defValue;

	XML::Node	*value = FindValueNode(section, name);

	if (value != NIL)
	{
		return value->GetContent().ToInt();
	}
	else
	{
		XML::Node	*configuration = FindConfigurationNode(activeConfig);

		if (configuration->GetAttributeByName("inheriting") != NIL)
		{
			String	 oConfig = activeConfig;

			activeConfig = configuration->GetAttributeByName("inheriting")->GetContent();

			Int	 retVal = GetIntValue(section, name, defValue);

			activeConfig = oConfig;

			return retVal;
		}

		return defValue;
	}
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
			value = FindConfigurationNode(activeConfig)->AddNode("section");

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

	if (value != NIL)
	{
		return value->GetContent();
	}
	else
	{
		XML::Node	*configuration = FindConfigurationNode(activeConfig);

		if (configuration->GetAttributeByName("inheriting") != NIL)
		{
			String	 oConfig = activeConfig;

			activeConfig = configuration->GetAttributeByName("inheriting")->GetContent();

			String	 retVal = GetStringValue(section, name, defValue);

			activeConfig = oConfig;

			return retVal;
		}

		return defValue;
	}
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
			value = FindConfigurationNode(activeConfig)->AddNode("section");

			value->SetAttribute("name", section);
		}

		value = FindSectionNode(section)->AddNode("value", newValue);

		value->SetAttribute("name", name);
	}

	return Success;
}

S::XML::Node *S::Configuration::FindConfigurationNode(String configuration)
{
	XML::Node	*root = configFile->GetRootNode();

	if (root == NIL) return NIL;

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if (node->GetName() != "configuration") continue;

		XML::Attribute	*attributeName = node->GetAttributeByName("name");

		if (attributeName != NIL)
		{
			if (attributeName->GetContent() == configuration) return node;
		}
	}

	return NIL;
}

S::XML::Node *S::Configuration::FindSectionNode(String section)
{
	XML::Node	*configuration = FindConfigurationNode(activeConfig);

	if (configuration == NIL) return NIL;

	for (Int i = 0; i < configuration->GetNOfNodes(); i++)
	{
		XML::Node	*node = configuration->GetNthNode(i);
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
