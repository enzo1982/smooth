 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_XMLNODE_
#define __OBJSMOOTH_XMLNODE_

#include <libxml/parser.h>

#include <smooth/object.h>
#include <smooth/xml/node.h>
#include <smooth/stk.h>

SMOOTHXMLNode::SMOOTHXMLNode()
{
	nextNode	= NIL;
	prevNode	= NIL;

	parentDoc	= NIL;
	parentNode	= NIL;

	nodeID		= -1;
	name		= NIL;
	content		= NIL;
}

SMOOTHXMLNode::~SMOOTHXMLNode()
{
	SMOOTHInt	 nOfAttributes	= GetNOfAttributes();
	SMOOTHInt	 nOfNodes	= GetNOfNodes();

	for (SMOOTHInt i = 0; i < nOfAttributes; i++)	RemoveAttribute(GetNthAttribute(0));
	for (SMOOTHInt j = 0; j < nOfNodes; j++)	RemoveNode(GetNthNode(0));
}

SMOOTHXMLDocument *SMOOTHXMLNode::GetParentDocument()
{
	return parentDoc;
}

SMOOTHInt SMOOTHXMLNode::SetParentDocument(SMOOTHXMLDocument *newParentDoc)
{
	parentDoc = newParentDoc;

	return SMOOTH::Success;
}

SMOOTHXMLNode *SMOOTHXMLNode::GetParentNode()
{
	return parentNode;
}

SMOOTHInt SMOOTHXMLNode::SetParentNode(SMOOTHXMLNode *newParentNode)
{
	parentNode = newParentNode;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLNode::GetNodeID()
{
	return nodeID;
}

SMOOTHInt SMOOTHXMLNode::SetNodeID(SMOOTHInt newID)
{
	nodeID = newID;

	return SMOOTH::Success;
}

SMOOTHXMLNode *SMOOTHXMLNode::GetNextNode()
{
	return nextNode;
}

SMOOTHXMLNode *SMOOTHXMLNode::GetPrevNode()
{
	return prevNode;
}

SMOOTHInt SMOOTHXMLNode::SetNextNode(SMOOTHXMLNode *newNode)
{
	nextNode = newNode;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLNode::SetPrevNode(SMOOTHXMLNode *newNode)
{
	prevNode = newNode;

	return SMOOTH::Success;
}

SMOOTHString SMOOTHXMLNode::GetName()
{
	return name;
}

SMOOTHInt SMOOTHXMLNode::SetName(SMOOTHString newName)
{
	name = newName;

	return SMOOTH::Success;
}

SMOOTHString SMOOTHXMLNode::GetContent()
{
	return content;
}

SMOOTHInt SMOOTHXMLNode::SetContent(SMOOTHString newContent)
{
	content = newContent;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLNode::GetNOfAttributes()
{
	return attributes.GetNOfEntries();
}

SMOOTHXMLAttribute *SMOOTHXMLNode::GetNthAttribute(SMOOTHInt attributeNumber)
{
	if (attributeNumber >= GetNOfAttributes()) return NIL;

	return attributes.GetNthEntry(attributeNumber);
}

SMOOTHXMLAttribute *SMOOTHXMLNode::GetAttributeByName(SMOOTHString attributeName)
{
	if (GetNOfAttributes() == 0) return NIL;

	SMOOTHXMLAttribute	*attribute	= GetNthAttribute(0);
	SMOOTHInt		 nOfAttributes	= GetNOfAttributes();

	for (SMOOTHInt i = 1; i < nOfAttributes; i++)
	{
		if (attribute->GetName() == attributeName) return attribute;

		attribute = attribute->GetNextAttribute();
	}

	if (attribute->GetName() == attributeName) return attribute;

	return NIL;
}

SMOOTHXMLAttribute *SMOOTHXMLNode::SetAttribute(SMOOTHString attributeName, SMOOTHString attributeContent)
{
	SMOOTHXMLAttribute	*attribute = GetAttributeByName(attributeName);

	if (attribute == NIL)
	{
		attribute = new SMOOTHXMLAttribute();

		attribute->SetParentNode(this);

		attribute->SetName(attributeName);
		attribute->SetContent(attributeContent);

		if (GetNOfAttributes() > 0)
		{
			GetNthAttribute(GetNOfAttributes() - 1)->SetNextAttribute(attribute);

			attribute->SetPrevAttribute(GetNthAttribute(GetNOfAttributes() - 1));
		}

		attribute->SetAttributeID(attributes.AddEntry(attribute));
	}
	else
	{
		attribute->SetContent(attributeContent);
	}

	return attribute;
}

SMOOTHInt SMOOTHXMLNode::RemoveAttribute(SMOOTHXMLAttribute *attribute)
{
	if (attribute == NIL)
	{
		return SMOOTH::Error;
	}
	else
	{
		attributes.DeleteEntry(attribute->GetAttributeID());

		if (attribute->GetNextAttribute() != NIL)
		{
			attribute->GetNextAttribute()->SetPrevAttribute(NIL);

			if (attribute->GetPrevAttribute() != NIL) attribute->GetNextAttribute()->SetPrevAttribute(attribute->GetPrevAttribute());
		}

		if (attribute->GetPrevAttribute() != NIL)
		{
			attribute->GetPrevAttribute()->SetNextAttribute(NIL);

			if (attribute->GetNextAttribute() != NIL) attribute->GetPrevAttribute()->SetNextAttribute(attribute->GetNextAttribute());
		}

		delete attribute;
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLNode::RemoveAttributeByName(SMOOTHString attributeName)
{
	return RemoveAttribute(GetAttributeByName(attributeName));
}

SMOOTHInt SMOOTHXMLNode::GetNOfNodes()
{
	return subnodes.GetNOfEntries();
}

SMOOTHXMLNode *SMOOTHXMLNode::GetNthNode(SMOOTHInt nodeNumber)
{
	if (nodeNumber >= GetNOfNodes()) return NIL;

	return subnodes.GetNthEntry(nodeNumber);
}

SMOOTHXMLNode *SMOOTHXMLNode::GetNodeByName(SMOOTHString nodeName)
{
	if (GetNOfNodes() == 0) return NIL;

	SMOOTHXMLNode	*node		= GetNthNode(0);
	int		 nOfNodes	= GetNOfNodes();

	for (SMOOTHInt i = 1; i < nOfNodes; i++)
	{
		if (node->GetName() == nodeName) return node;

		node = node->GetNextNode();
	}

	if (node->GetName() == nodeName) return node;

	return NIL;
}

SMOOTHXMLNode *SMOOTHXMLNode::AddNode(SMOOTHString nodeName, SMOOTHString nodeContent)
{
	SMOOTHXMLNode	*node = new SMOOTHXMLNode();

	node->SetParentNode(this);

	node->SetName(nodeName);
	node->SetContent(nodeContent);

	if (GetNOfNodes() > 0)
	{
		GetNthNode(GetNOfNodes() - 1)->SetNextNode(node);

		node->SetPrevNode(GetNthNode(GetNOfNodes() - 1));
	}

	node->SetNodeID(subnodes.AddEntry(node));

	return node;
}

SMOOTHXMLNode *SMOOTHXMLNode::SetNode(SMOOTHString nodeName, SMOOTHString nodeContent)
{
	SMOOTHXMLNode	*node = GetNodeByName(nodeName);

	if (node == NIL)
	{
		node = new SMOOTHXMLNode();

		node->SetParentNode(this);

		node->SetName(nodeName);
		node->SetContent(nodeContent);

		if (GetNOfNodes() > 0)
		{
			GetNthNode(GetNOfNodes() - 1)->SetNextNode(node);

			node->SetPrevNode(GetNthNode(GetNOfNodes() - 1));
		}

		node->SetNodeID(subnodes.AddEntry(node));
	}
	else
	{
		node->SetContent(nodeContent);
	}

	return node;
}

SMOOTHInt SMOOTHXMLNode::RemoveNode(SMOOTHXMLNode *node)
{
	if (node == NIL)
	{
		return SMOOTH::Error;
	}
	else
	{
		subnodes.DeleteEntry(node->GetNodeID());

		if (node->GetNextNode() != NIL)
		{
			node->GetNextNode()->SetPrevNode(NIL);

			if (node->GetPrevNode() != NIL) node->GetNextNode()->SetPrevNode(node->GetPrevNode());
		}

		if (node->GetPrevNode() != NIL)
		{
			node->GetPrevNode()->SetNextNode(NIL);

			if (node->GetNextNode() != NIL) node->GetPrevNode()->SetNextNode(node->GetNextNode());
		}

		delete node;
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLNode::RemoveNodeByName(SMOOTHString nodeName)
{
	return RemoveNode(GetNodeByName(nodeName));
}

#endif
