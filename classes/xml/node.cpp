 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <libxml/parser.h>

#include <smooth/object.h>
#include <smooth/xml/node.h>

S::XML::Node::Node()
{
	nextNode	= NIL;
	prevNode	= NIL;

	parentDoc	= NIL;
	parentNode	= NIL;

	nodeID		= -1;
	name		= NIL;
	content		= NIL;
}

S::XML::Node::~Node()
{
	Int	 nOfAttributes	= GetNOfAttributes();
	Int	 nOfNodes	= GetNOfNodes();

	for (Int i = 0; i < nOfAttributes; i++)	RemoveAttribute(GetNthAttribute(0));
	for (Int j = 0; j < nOfNodes; j++)	RemoveNode(GetNthNode(0));
}

S::XML::Document *S::XML::Node::GetParentDocument()
{
	return parentDoc;
}

S::Int S::XML::Node::SetParentDocument(S::XML::Document *newParentDoc)
{
	parentDoc = newParentDoc;

	return Success;
}

S::XML::Node *S::XML::Node::GetParentNode()
{
	return parentNode;
}

S::Int S::XML::Node::SetParentNode(Node *newParentNode)
{
	parentNode = newParentNode;

	return Success;
}

S::Int S::XML::Node::GetNodeID()
{
	return nodeID;
}

S::Int S::XML::Node::SetNodeID(Int newID)
{
	nodeID = newID;

	return Success;
}

S::XML::Node *S::XML::Node::GetNextNode()
{
	return nextNode;
}

S::XML::Node *S::XML::Node::GetPrevNode()
{
	return prevNode;
}

S::Int S::XML::Node::SetNextNode(Node *newNode)
{
	nextNode = newNode;

	return Success;
}

S::Int S::XML::Node::SetPrevNode(Node *newNode)
{
	prevNode = newNode;

	return Success;
}

S::String S::XML::Node::GetName()
{
	return name;
}

S::Int S::XML::Node::SetName(String newName)
{
	name = newName;

	return Success;
}

S::String S::XML::Node::GetContent()
{
	return content;
}

S::Int S::XML::Node::SetContent(String newContent)
{
	content = newContent;

	return Success;
}

S::Int S::XML::Node::GetNOfAttributes()
{
	return attributes.GetNOfEntries();
}

S::XML::Attribute *S::XML::Node::GetNthAttribute(Int attributeNumber)
{
	if (attributeNumber >= GetNOfAttributes()) return NIL;

	return attributes.GetNthEntry(attributeNumber);
}

S::XML::Attribute *S::XML::Node::GetAttributeByName(String attributeName)
{
	if (GetNOfAttributes() == 0) return NIL;

	Attribute	*attribute	= GetNthAttribute(0);
	Int		 nOfAttributes	= GetNOfAttributes();

	for (Int i = 1; i < nOfAttributes; i++)
	{
		if (attribute->GetName() == attributeName) return attribute;

		attribute = attribute->GetNextAttribute();
	}

	if (attribute->GetName() == attributeName) return attribute;

	return NIL;
}

S::XML::Attribute *S::XML::Node::SetAttribute(String attributeName, String attributeContent)
{
	Attribute	*attribute = GetAttributeByName(attributeName);

	if (attribute == NIL)
	{
		attribute = new Attribute();

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

S::Int S::XML::Node::RemoveAttribute(Attribute *attribute)
{
	if (attribute == NIL)
	{
		return Error;
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

	return Success;
}

S::Int S::XML::Node::RemoveAttributeByName(String attributeName)
{
	return RemoveAttribute(GetAttributeByName(attributeName));
}

S::Int S::XML::Node::GetNOfNodes()
{
	return subnodes.GetNOfEntries();
}

S::XML::Node *S::XML::Node::GetNthNode(Int nodeNumber)
{
	if (nodeNumber >= GetNOfNodes()) return NIL;

	return subnodes.GetNthEntry(nodeNumber);
}

S::XML::Node *S::XML::Node::GetNodeByName(String nodeName)
{
	if (GetNOfNodes() == 0) return NIL;

	Node	*node		= GetNthNode(0);
	Int	 nOfNodes	= GetNOfNodes();

	for (Int i = 1; i < nOfNodes; i++)
	{
		if (node->GetName() == nodeName) return node;

		node = node->GetNextNode();
	}

	if (node->GetName() == nodeName) return node;

	return NIL;
}

S::XML::Node *S::XML::Node::AddNode(String nodeName, String nodeContent)
{
	Node	*node = new Node();

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

S::XML::Node *S::XML::Node::SetNode(String nodeName, String nodeContent)
{
	Node	*node = GetNodeByName(nodeName);

	if (node == NIL)
	{
		node = new Node();

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

S::Int S::XML::Node::RemoveNode(Node *node)
{
	if (node == NIL)
	{
		return Error;
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

	return Success;
}

S::Int S::XML::Node::RemoveNodeByName(String nodeName)
{
	return RemoveNode(GetNodeByName(nodeName));
}
