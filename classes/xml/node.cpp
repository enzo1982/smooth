 /* The smooth Class Library
  * Copyright (C) 1998-2022 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/node.h>
#include <smooth/foreach.h>

S::Array<S::String> S::XML::Node::elementNames;

S::XML::Node::Node(const String &iName, const String &iContent)
{
	nodeID		= -1;

	nameIndex	= iName.ComputeCRC32();
	content		= iContent;

	attributes	= NIL;
	subnodes	= NIL;

	if (elementNames.Get(nameIndex) == NIL) elementNames.Add(iName, nameIndex);
}

S::XML::Node::Node(const Node &oNode)
{
	nodeID		= -1;

	nameIndex	= oNode.nameIndex;
	content		= oNode.content;

	attributes	= NIL;
	subnodes	= NIL;

	/* Copy attributes.
	 */
	if (oNode.attributes != NIL)
	{
		foreach (Attribute *oAttribute, *oNode.attributes)
		{
			Attribute	*attribute = new Attribute(*oAttribute);

			if (attributes == NIL) attributes = new Array<Attribute *, Void *>();

			attribute->SetAttributeID(attributes->Add(attribute));
		}
	}

	/* Copy subnodes.
	 */
	if (oNode.subnodes != NIL)
	{
		foreach (Node *oSubnode, *oNode.subnodes)
		{
			Node	*node = new Node(*oSubnode);

			if (subnodes == NIL) subnodes = new Array<Node *, Void *>();

			node->SetNodeID(subnodes->Add(node));
		}
	}
}

S::XML::Node::~Node()
{
	/* Delete attributes.
	 */
	if (attributes != NIL)
	{
		foreach (Attribute *attribute, *attributes) delete attribute;

		delete attributes;
	}

	/* Delete subnodes.
	 */
	if (subnodes != NIL)
	{
		foreach (Node *subnode, *subnodes) delete subnode;

		delete subnodes;
	}
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
	return elementNames.Get(nameIndex);
}

S::Int S::XML::Node::SetName(const String &newName)
{
	nameIndex = newName.ComputeCRC32();

	if (elementNames.Get(nameIndex) == NIL) elementNames.Add(newName, nameIndex);

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

	return attributes->Length();
}

S::XML::Attribute *S::XML::Node::GetNthAttribute(Int attributeNumber) const
{
	if (attributeNumber >= GetNOfAttributes()) return NIL;

	return attributes->GetNth(attributeNumber);
}

S::XML::Attribute *S::XML::Node::GetAttributeByName(const String &attributeName) const
{
	Int	 nOfAttributes = GetNOfAttributes();

	if (nOfAttributes == 0) return NIL;

	Int	 nameIndex     = attributeName.ComputeCRC32();

	for (Int i = 0; i < nOfAttributes; i++)
	{
		Attribute	*attribute = GetNthAttribute(i);

		if (attribute->nameIndex == nameIndex) return attribute;
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

		attribute->SetAttributeID(attributes->Add(attribute));
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

	attributes->Remove(attribute->GetAttributeID());

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

	return subnodes->Length();
}

S::XML::Node *S::XML::Node::GetNthNode(Int nodeNumber) const
{
	if (nodeNumber >= GetNOfNodes()) return NIL;

	return subnodes->GetNth(nodeNumber);
}

S::XML::Node *S::XML::Node::GetNodeByName(const String &nodeName) const
{
	Int	 nOfNodes  = GetNOfNodes();

	if (nOfNodes == 0) return NIL;

	Int	 nameIndex = nodeName.ComputeCRC32();

	for (Int i = 0; i < nOfNodes; i++)
	{
		Node	*node = GetNthNode(i);

		if (node->nameIndex == nameIndex) return node;
	}

	return NIL;
}

S::XML::Node *S::XML::Node::AddNode(const String &iName, const String &iContent)
{
	Node	*node = new Node(iName, iContent);

	if (subnodes == NIL) subnodes = new Array<Node *, Void *>();

	node->SetNodeID(subnodes->Add(node));

	return node;
}

S::Int S::XML::Node::RemoveNode(Node *node)
{
	if (node == NIL) return Error();

	subnodes->Remove(node->GetNodeID());

	delete node;

	return Success();
}

S::Int S::XML::Node::RemoveNodeByName(const String &nodeName)
{
	return RemoveNode(GetNodeByName(nodeName));
}
