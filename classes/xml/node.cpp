 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/node.h>

S::Array<S::String> S::XML::Node::elementNames;

S::XML::Node::Node(const String &iName, const String &iContent)
{
	nodeID		= -1;

	nameIndex	= iName.ComputeCRC32();
	content		= iContent;

	attributes	= NIL;
	subnodes	= NIL;

	if (elementNames.GetEntry(nameIndex) == NIL) elementNames.AddEntry(iName, nameIndex);
}

S::XML::Node::~Node()
{
	Int	 nOfAttributes	= GetNOfAttributes();
	Int	 nOfNodes	= GetNOfNodes();

	for (Int i = 0; i < nOfAttributes; i++)	RemoveAttribute(GetNthAttribute(0));
	for (Int j = 0; j < nOfNodes; j++)	RemoveNode(GetNthNode(0));

	if (attributes != NIL)	delete attributes;
	if (subnodes != NIL)	delete subnodes;
}

S::Int S::XML::Node::GetNodeID() const
{
	return nodeID;
}

S::Int S::XML::Node::SetNodeID(Int newID)
{
	nodeID = newID;

	return Success();
}

const S::String &S::XML::Node::GetName() const
{
	return elementNames.GetEntry(nameIndex);
}

S::Int S::XML::Node::SetName(const String &newName)
{
	nameIndex = newName.ComputeCRC32();

	if (elementNames.GetEntry(nameIndex) == NIL) elementNames.AddEntry(newName, nameIndex);

	return Success();
}

const S::String &S::XML::Node::GetContent() const
{
	return content;
}

S::Int S::XML::Node::SetContent(const String &newContent)
{
	content = newContent;

	return Success();
}

S::Int S::XML::Node::GetNOfAttributes() const
{
	if (attributes == NIL) return 0;

	return attributes->GetNOfEntries();
}

S::XML::Attribute *S::XML::Node::GetNthAttribute(Int attributeNumber) const
{
	if (attributeNumber >= GetNOfAttributes()) return NIL;

	return attributes->GetNthEntry(attributeNumber);
}

S::XML::Attribute *S::XML::Node::GetAttributeByName(const String &attributeName) const
{
	Int	 nOfAttributes = GetNOfAttributes();

	for (Int i = 0; i < nOfAttributes; i++)
	{
		Attribute	*attribute = GetNthAttribute(i);

		if (attribute->GetName() == attributeName) return attribute;
	}

	return NIL;
}

S::XML::Attribute *S::XML::Node::SetAttribute(const String &attributeName, const String &attributeContent)
{
	Attribute	*attribute = GetAttributeByName(attributeName);

	if (attribute == NIL)
	{
		attribute = new Attribute(attributeName, attributeContent);

		if (attributes == NIL) attributes = new Array<Attribute *, Void *>();

		attribute->SetAttributeID(attributes->AddEntry(attribute));
	}
	else
	{
		attribute->SetContent(attributeContent);
	}

	return attribute;
}

S::Int S::XML::Node::RemoveAttribute(Attribute *attribute)
{
	if (attribute == NIL) return Error();

	attributes->RemoveEntry(attribute->GetAttributeID());

	delete attribute;

	return Success();
}

S::Int S::XML::Node::RemoveAttributeByName(const String &attributeName)
{
	return RemoveAttribute(GetAttributeByName(attributeName));
}

S::Int S::XML::Node::GetNOfNodes() const
{
	if (subnodes == NIL) return 0;

	return subnodes->GetNOfEntries();
}

S::XML::Node *S::XML::Node::GetNthNode(Int nodeNumber) const
{
	if (nodeNumber >= GetNOfNodes()) return NIL;

	return subnodes->GetNthEntry(nodeNumber);
}

S::XML::Node *S::XML::Node::GetNodeByName(const String &nodeName) const
{
	Int	 nOfNodes = GetNOfNodes();

	for (Int i = 0; i < nOfNodes; i++)
	{
		Node	*node = GetNthNode(i);

		if (node->GetName() == nodeName) return node;
	}

	return NIL;
}

S::XML::Node *S::XML::Node::AddNode(const String &iName, const String &iContent)
{
	Node	*node = new Node(iName, iContent);

	if (subnodes == NIL) subnodes = new Array<Node *, Void *>();

	node->SetNodeID(subnodes->AddEntry(node));

	return node;
}

S::Int S::XML::Node::RemoveNode(Node *node)
{
	if (node == NIL) return Error();

	subnodes->RemoveEntry(node->GetNodeID());

	delete node;

	return Success();
}

S::Int S::XML::Node::RemoveNodeByName(const String &nodeName)
{
	return RemoveNode(GetNodeByName(nodeName));
}
