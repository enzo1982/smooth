 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_XMLDOCUMENT_
#define __OBJSMOOTH_XMLDOCUMENT_

#include <libxml/parser.h>

#include <smooth/object.h>
#include <smooth/xml/document.h>
#include <smooth/stk.h>

SMOOTHXMLDocument::SMOOTHXMLDocument()
{
	rootNode	= NIL;
}

SMOOTHXMLDocument::~SMOOTHXMLDocument()
{
	if (rootNode != NIL) delete rootNode;
}

SMOOTHXMLNode *SMOOTHXMLDocument::GetRootNode()
{
	return rootNode;
}

SMOOTHInt SMOOTHXMLDocument::SetRootNode(SMOOTHXMLNode *newRootNode)
{
	rootNode = newRootNode;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLDocument::LoadFile(SMOOTHString fileName)
{
	xmlKeepBlanksDefault(SMOOTH::False);

	xmlDocPtr	 doc = xmlParseFile(fileName);
	xmlNodePtr	 xmlRoot = xmlDocGetRootElement(doc);
	SMOOTHXMLNode	*root = new SMOOTHXMLNode();

	SMOOTHString::SetInputFormat(SIF_UTF8);

	root->SetName((char *) xmlRoot->name);
	root->SetContent((char *) xmlRoot->content);

	SetRootNode(root);

	LoadNode(xmlRoot, rootNode);

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);

	xmlFreeDoc(doc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLDocument::LoadNode(xmlNodePtr node, SMOOTHXMLNode *smoothNode)
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

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLDocument::SaveFile(SMOOTHString fileName)
{
	xmlDocPtr	 doc = xmlNewDoc((xmlChar *) "1.0");

	if (rootNode != NIL)
	{
		doc->children = xmlNewDocNode(doc, NIL, (xmlChar *) (char *) rootNode->GetName(), (xmlChar *) (char *) rootNode->GetContent());

		SaveNode(rootNode, doc->children);
	}

	xmlSaveFormatFile(fileName, doc, SMOOTH::True);

	xmlFreeDoc(doc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLDocument::SaveNode(SMOOTHXMLNode *node, xmlNodePtr parent)
{
	for (SMOOTHInt i = 0; i < node->GetNOfAttributes(); i++)
	{
		SMOOTHXMLAttribute *attribute = node->GetNthAttribute(i);

		xmlSetProp(parent, (xmlChar *) (char *) attribute->GetName(), (xmlChar *) (char *) attribute->GetContent());
	}

	for (SMOOTHInt j = 0; j < node->GetNOfNodes(); j++)
	{
		SMOOTHXMLNode	*subNode = node->GetNthNode(j);
		xmlNodePtr	 xmlNode = xmlNewChild(parent, NIL, (xmlChar *) (char *) subNode->GetName(), (xmlChar *) (char *) subNode->GetContent());

		SaveNode(subNode, xmlNode);
	}

	return SMOOTH::Success;
}

#endif
