 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/document.h>

S::XML::Document::Document()
{
	rootNode	= NIL;
	ownRoot		= False;
	encoding	= "UTF-8";
}

S::XML::Document::~Document()
{
	if (rootNode != NIL && ownRoot) delete rootNode;
}

S::XML::Node *S::XML::Document::GetRootNode()
{
	return rootNode;
}

S::Int S::XML::Document::SetRootNode(Node *newRootNode)
{
	rootNode = newRootNode;
	ownRoot = False;

	return Success;
}

S::Int S::XML::Document::SetEncoding(String newEncoding)
{
	encoding = newEncoding;

	return Success;
}

S::Int S::XML::Document::LoadFile(String fileName)
{
	xmlKeepBlanksDefault(False);

	xmlDocPtr doc = xmlParseFile(fileName);

	if (doc != NIL)
	{
		xmlNodePtr	 xmlRoot = xmlDocGetRootElement(doc);
		Node		*root = new Node();
		String		 inputFormat = String::SetInputFormat("UTF-8");

		root->SetName((char *) xmlRoot->name);
		root->SetContent((char *) xmlRoot->content);

		rootNode = root;
		ownRoot = True;

		LoadNode(xmlRoot, rootNode);

		String::SetInputFormat(inputFormat);

		xmlFreeDoc(doc);

		return Success;
	}
	else
	{
		return Failure;
	}			
}

S::Int S::XML::Document::ParseMemory(Void *buffer, Int size)
{
	xmlKeepBlanksDefault(False);

	xmlDocPtr doc = xmlParseMemory((char *) buffer, size);

	if (doc != NIL)
	{
		xmlNodePtr	 xmlRoot = xmlDocGetRootElement(doc);
		Node		*root = new Node();
		String		 inputFormat = String::SetInputFormat("UTF-8");

		root->SetName((char *) xmlRoot->name);
		root->SetContent((char *) xmlRoot->content);

		rootNode = root;
		ownRoot = True;

		LoadNode(xmlRoot, rootNode);

		String::SetInputFormat(inputFormat);

		xmlFreeDoc(doc);

		return Success;
	}
	else
	{
		return Failure;
	}			
}

S::Int S::XML::Document::LoadNode(xmlNodePtr node, Node *smoothNode)
{
	if (node->properties != NIL)
	{
		xmlAttr		*xmlAttribute = node->properties;

		do
		{
			smoothNode->SetAttribute((char *) xmlAttribute->name, (char *) xmlAttribute->children->content);

			xmlAttribute = xmlAttribute->next;
		}
		while (xmlAttribute != NIL);
	}

	if (node->children != NIL)
	{
		xmlNodePtr	 xmlNode = node->children;

		do
		{
			if (xmlNode->type == XML_TEXT_NODE)	smoothNode->SetContent((char *) xmlNode->content);
			else					LoadNode(xmlNode, smoothNode->AddNode((char *) xmlNode->name, (char *) xmlNode->content));

			xmlNode = xmlNode->next;
		}
		while (xmlNode != NIL);
	}

	return Success;
}

S::Int S::XML::Document::SaveFile(String fileName)
{
	xmlDocPtr	 doc = xmlNewDoc((xmlChar *) "1.0");

	if (rootNode != NIL)
	{
		doc->children = xmlNewDocNode(doc, NIL, (xmlChar *) rootNode->GetName().ConvertTo("UTF-8"), (xmlChar *) rootNode->GetContent().ConvertTo("UTF-8"));

		SaveNode(rootNode, doc->children);
	}

	xmlSaveFormatFileEnc(fileName, doc, encoding, True);

	xmlFreeDoc(doc);

	return Success;
}

S::Int S::XML::Document::SaveNode(Node *node, xmlNodePtr parent)
{
	for (Int i = 0; i < node->GetNOfAttributes(); i++)
	{
		Attribute	*attribute = node->GetNthAttribute(i);

		xmlSetProp(parent, (xmlChar *) attribute->GetName().ConvertTo("UTF-8"), (xmlChar *) attribute->GetContent().ConvertTo("UTF-8"));
	}

	for (Int j = 0; j < node->GetNOfNodes(); j++)
	{
		Node		*subNode = node->GetNthNode(j);
		xmlNodePtr	 xmlNode = xmlNewChild(parent, NIL, (xmlChar *) subNode->GetName().ConvertTo("UTF-8"), (xmlChar *) subNode->GetContent().ConvertTo("UTF-8"));

		SaveNode(subNode, xmlNode);
	}

	return Success;
}
