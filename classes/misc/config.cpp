 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/config.h>
#include <smooth/files/directory.h>
#include <smooth/gui/application/application.h>

static const S::String	 ConfigurationDefaultID	= "default";

static const S::String	 NodeConfigurationID	= "configuration";
static const S::String	 NodeSectionID		= "section";
static const S::String	 NodeValueID		= "value";

static const S::String	 AttributeInheritingID	= "inheriting";
static const S::String	 AttributeNameID	= "name";

S::Configuration::Configuration()
{
	ownRoot = NIL;

	configFile = NIL;

	activeConfig = ConfigurationDefaultID;
}

S::Configuration::Configuration(const String &file, Bool create)
{
	ownRoot = NIL;

	configFile = NIL;

	activeConfig = ConfigurationDefaultID;

	Open(file, create);
}

S::Configuration::Configuration(const Configuration &oConfig)
{
	ownRoot = NIL;

	configFile = new XML::Document(*oConfig.configFile);

	activeConfig = oConfig.activeConfig;
}

S::Configuration::~Configuration()
{
	if (configFile != NIL) Close();
}

S::Int S::Configuration::Open(const String &file, Bool create)
{
	if (configFile != NIL) Close();

	configFile = new XML::Document();

	if (configFile->LoadFile(file) == Success())
	{
		fileName = file;
	}
	else if (configFile->LoadFile(GUI::Application::GetStartupDirectory().Append(file)) == Success())
	{
		fileName = GUI::Application::GetStartupDirectory().Append(file);
	}
	else if (configFile->LoadFile(GUI::Application::GetApplicationDirectory().Append(file)) == Success())
	{
		fileName = GUI::Application::GetApplicationDirectory().Append(file);
	}
	else
	{
		if (!create)
		{
			Close();

			return Error();
		}
		else
		{
			ownRoot = new XML::Node("ConfigFile");

			configFile->SetRootNode(ownRoot);

			ownRoot->AddNode(NodeConfigurationID)->SetAttribute(AttributeNameID, ConfigurationDefaultID);

			if (file[1] == ':' ||
			    file[0] == '/' ||
			    file[0] == '\\') fileName = file;
			else		     fileName = GUI::Application::GetApplicationDirectory().Append(file);
		}
	}

	/* Check XML file type.
	 */
	XML::Node	*root = configFile->GetRootNode();

	if (root == NIL || root->GetNodeByName(NodeConfigurationID) == NIL)
	{
		Close();

		return Error();
	}

	return Success();
}

S::Int S::Configuration::Save()
{
	if (configFile == NIL) return Error();

	XML::Node	*root = configFile->GetRootNode();

	if (fileName != NIL && root != NIL)
	{
		/* Create base directory first.
		 */
		Directory(File(fileName).GetFilePath()).Create();

		/* Save config file.
		 */
		XML::Node	*configurationNode = root->GetNodeByName(NodeConfigurationID);

		if (configurationNode != NIL && configurationNode->GetNOfNodes() >= 1) configFile->SaveFile(fileName);
	}

	return Success();
}

S::Int S::Configuration::Close()
{
	if (configFile != NIL)
	{
		delete configFile;

		configFile = NIL;

		fileName = NIL;
	}

	if (ownRoot != NIL)
	{
		delete ownRoot;

		ownRoot = NIL;
	}

	return Success();
}

S::Int S::Configuration::AddConfiguration(const String &nConfig)
{
	if (configFile == NIL) return Error();

	XML::Node	*configuration = FindConfigurationNode(nConfig);

	if (configuration == NIL)
	{
		configFile->GetRootNode()->AddNode(NodeConfigurationID)->SetAttribute(AttributeNameID, nConfig);

		return Success();
	}

	return Error();
}

S::Int S::Configuration::RemoveConfiguration(const String &rConfig)
{
	if (configFile == NIL) return Error();

	XML::Node	*configuration = FindConfigurationNode(rConfig);

	if (configuration != NIL)
	{
		if (activeConfig == rConfig) SetActiveConfiguration(ConfigurationDefaultID);

		configFile->GetRootNode()->RemoveNode(configuration);

		return Success();
	}

	return Error();
}

S::Int S::Configuration::GetNOfConfigurations() const
{
	XML::Node	*root = configFile->GetRootNode();

	if (root == NIL) return 0;

	Int	 count = 0;

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if (node->GetName() != NodeConfigurationID) continue;

		count++;
	}

	return count;
}

S::String S::Configuration::GetNthConfigurationName(Int n) const
{
	XML::Node	*root = configFile->GetRootNode();

	if (root == NIL) return NIL;

	Int	 count = 0;

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if (node->GetName() != NodeConfigurationID) continue;

		if (count++ == n)
		{
			XML::Attribute	*attributeName = node->GetAttributeByName(AttributeNameID);

			if (attributeName != NIL) return attributeName->GetContent();
			else			  return NIL;
		}
	}

	return NIL;
}

S::Int S::Configuration::SetActiveConfiguration(const String &nConfig)
{
	if (configFile == NIL) return Error();

	XML::Node	*configuration = FindConfigurationNode(nConfig);

	if (configuration == NIL) AddConfiguration(nConfig);

	activeConfig = nConfig;

	return Success();
}

S::Int S::Configuration::SetParentConfiguration(const String &nParent)
{
	if (configFile == NIL) return Error();

	XML::Node	*configuration = FindConfigurationNode(activeConfig);

	if (configuration == NIL) return Error();

	configuration->SetAttribute(AttributeInheritingID, nParent);

	return Success();
}

S::String S::Configuration::GetConfigurationName() const
{
	if (configFile == NIL) return NIL;

	XML::Node	*configuration = FindConfigurationNode(activeConfig);

	if (configuration == NIL) return NIL;

	return activeConfig;
}

S::Int S::Configuration::SetConfigurationName(const String &nName)
{
	if (configFile == NIL) return Error();

	XML::Node	*configuration = FindConfigurationNode(activeConfig);

	if (configuration == NIL) return Error();

	activeConfig = nName;

	configuration->SetAttribute(AttributeNameID, nName);

	return Success();
}

S::Int S::Configuration::GetIntValue(const String &section, const String &name, Int defValue)
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
		XML::Attribute	*attributeInheriting = configuration->GetAttributeByName(AttributeInheritingID);

		if (attributeInheriting != NIL)
		{
			String	 oConfig = activeConfig;

			activeConfig = attributeInheriting->GetContent();

			Int	 retVal = GetIntValue(section, name, defValue);

			activeConfig = oConfig;

			return retVal;
		}

		return defValue;
	}
}

S::Int S::Configuration::SetIntValue(const String &section, const String &name, Int newValue)
{
	if (configFile == NIL) return Error();

	XML::Node	*valueNode = FindValueNode(section, name);

	if (valueNode != NIL)
	{
		valueNode->SetContent(String::FromInt(newValue));
	}
	else
	{
		XML::Node	*sectionNode = FindSectionNode(section);

		if (sectionNode == NIL)
		{
			sectionNode = FindConfigurationNode(activeConfig)->AddNode(NodeSectionID);
			sectionNode->SetAttribute(AttributeNameID, section);
		}

		valueNode = sectionNode->AddNode(NodeValueID, String::FromInt(newValue));
		valueNode->SetAttribute(AttributeNameID, name);
	}

	return Success();
}

S::String S::Configuration::GetStringValue(const String &section, const String &name, const String &defValue)
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
		XML::Attribute	*attributeInheriting = configuration->GetAttributeByName(AttributeInheritingID);

		if (attributeInheriting != NIL)
		{
			String	 oConfig = activeConfig;

			activeConfig = attributeInheriting->GetContent();

			String	 retVal = GetStringValue(section, name, defValue);

			activeConfig = oConfig;

			return retVal;
		}

		return defValue;
	}
}

S::Int S::Configuration::SetStringValue(const String &section, const String &name, const String &newValue)
{
	if (configFile == NIL) return Error();

	XML::Node	*valueNode = FindValueNode(section, name);

	if (valueNode != NIL)
	{
		valueNode->SetContent(newValue);
	}
	else
	{
		XML::Node	*sectionNode = FindSectionNode(section);

		if (sectionNode == NIL)
		{
			sectionNode = FindConfigurationNode(activeConfig)->AddNode(NodeSectionID);
			sectionNode->SetAttribute(AttributeNameID, section);
		}

		valueNode = sectionNode->AddNode(NodeValueID, newValue);
		valueNode->SetAttribute(AttributeNameID, name);
	}

	return Success();
}

S::XML::Node *S::Configuration::FindConfigurationNode(const String &configuration) const
{
	XML::Node	*root = configFile->GetRootNode();

	if (root == NIL) return NIL;

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if (node->GetName() != NodeConfigurationID) continue;

		XML::Attribute	*attributeName = node->GetAttributeByName(AttributeNameID);

		if (attributeName != NIL && attributeName->GetContent() == configuration) return node;
	}

	return NIL;
}

S::XML::Node *S::Configuration::FindSectionNode(const String &section) const
{
	XML::Node	*configuration = FindConfigurationNode(activeConfig);

	if (configuration == NIL) return NIL;

	for (Int i = 0; i < configuration->GetNOfNodes(); i++)
	{
		XML::Node	*node = configuration->GetNthNode(i);
		XML::Attribute	*attributeName = node->GetAttributeByName(AttributeNameID);

		if (attributeName != NIL && attributeName->GetContent() == section) return node;
	}

	return NIL;
}

S::XML::Node *S::Configuration::FindValueNode(const String &sect, const String &name) const
{
	XML::Node	*section = FindSectionNode(sect);

	if (section == NIL) return NIL;

	for (Int i = 0; i < section->GetNOfNodes(); i++)
	{
		XML::Node	*node = section->GetNthNode(i);
		XML::Attribute	*attributeName = node->GetAttributeByName(AttributeNameID);

		if (attributeName != NIL && attributeName->GetContent() == name) return node;
	}

	return NIL;
}
