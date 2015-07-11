 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/document.h>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

S::XML::Document::Document()
{
	rootNode	= NIL;
	ownRoot		= False;
	encoding	= "UTF-8";
}

S::XML::Document::Document(const Document &oDocument)
{
	rootNode	= new Node(*oDocument.GetRootNode());
	encoding	= oDocument.encoding;
}

S::XML::Document::~Document()
{
	if (rootNode != NIL && ownRoot) delete rootNode;
}

S::XML::Node *S::XML::Document::GetRootNode() const
{
	return rootNode;
}

S::Int S::XML::Document::SetRootNode(Node *newRootNode)
{
	rootNode = newRootNode;
	ownRoot = False;

	return Success();
}

S::Int S::XML::Document::SetEncoding(const String &newEncoding)
{
	encoding = newEncoding;

	return Success();
}

S::Int S::XML::Document::LoadFile(const String &fileName)
{
	xmlTextReaderPtr reader = xmlNewTextReaderFilename(fileName.ConvertTo("UTF-8"));

	if (reader == NIL) { return Error(); }

	Int	 ret = 1;

	while ((ret = xmlTextReaderRead(reader)) == 1)
	{
		if (xmlTextReaderNodeType(reader) == XML_ELEMENT_NODE)
		{
			String	 inputFormat = String::SetInputFormat("UTF-8");

			rootNode = new Node(NIL);
			ownRoot = True;

			LoadNode(reader, rootNode);

			String::SetInputFormat(inputFormat);

			break;
		}
	}

	xmlFreeTextReader(reader);

	if (ret >= 0)	return Success();
	else		return Error();
}

S::Int S::XML::Document::ParseMemory(const Void *memory, Int size)
{
	xmlParserInputBufferPtr	 buffer = xmlParserInputBufferCreateMem((char *) memory, size, XML_CHAR_ENCODING_NONE);

	if (buffer == NIL) { return Error(); }

	xmlTextReaderPtr	 reader = xmlNewTextReader(buffer, NIL);

	if (reader == NIL) { xmlFreeParserInputBuffer(buffer); return Error(); }

	Int	 ret = 1;

	while ((ret = xmlTextReaderRead(reader)) == 1)
	{
		if (xmlTextReaderNodeType(reader) == XML_ELEMENT_NODE)
		{
			String	 inputFormat = String::SetInputFormat("UTF-8");

			rootNode = new Node(NIL);
			ownRoot = True;

			LoadNode(reader, rootNode);

			String::SetInputFormat(inputFormat);

			break;
		}
	}

	xmlFreeTextReader(reader);
	xmlFreeParserInputBuffer(buffer);

	if (ret >= 0)	return Success();
	else		return Error();
}

S::Int S::XML::Document::LoadNode(xmlTextReaderPtr reader, Node *node)
{
	node->SetName((const char *) xmlTextReaderConstName(reader));

	if (xmlTextReaderHasAttributes(reader))
	{
		while (xmlTextReaderMoveToNextAttribute(reader) == 1) node->SetAttribute((const char *) xmlTextReaderConstName(reader), (const char *) xmlTextReaderConstValue(reader));

		xmlTextReaderMoveToElement(reader);
	}

	if (!xmlTextReaderIsEmptyElement(reader))
	{
		Int	 nodeType = 0;

		if (xmlTextReaderRead(reader) == -1) return Error();

		while ((nodeType = xmlTextReaderNodeType(reader)) != XML_ELEMENT_DECL)
		{
			if	(nodeType == XML_ELEMENT_NODE)					  LoadNode(reader, node->AddNode(NIL));
			else if (nodeType == XML_TEXT_NODE || nodeType == XML_CDATA_SECTION_NODE) node->SetContent((const char *) xmlTextReaderConstValue(reader));

			if (xmlTextReaderRead(reader) == -1) return Error();
		}
	}

	return Success();
}

S::Int S::XML::Document::SaveFile(const String &fileName)
{
	xmlTextWriterPtr	 writer = xmlNewTextWriterFilename(fileName.ConvertTo("UTF-8"), 0);

	xmlTextWriterSetIndent(writer, 1);
	xmlTextWriterStartDocument(writer, "1.0", encoding, NIL);

	if (rootNode != NIL) SaveNode(writer, rootNode);

	xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);

	return Success();
}

S::Int S::XML::Document::SaveNode(xmlTextWriterPtr writer, Node *node)
{
	xmlTextWriterStartElement(writer, (xmlChar *) node->GetName().ConvertTo("UTF-8"));

	for (Int i = 0; i < node->GetNOfAttributes(); i++) xmlTextWriterWriteAttribute(writer, (xmlChar *) node->GetNthAttribute(i)->GetName().ConvertTo("UTF-8"), (xmlChar *) node->GetNthAttribute(i)->GetContent().ConvertTo("UTF-8"));

	xmlTextWriterWriteString(writer, (xmlChar *) node->GetContent().ConvertTo("UTF-8"));

	for (Int i = 0; i < node->GetNOfNodes(); i++) SaveNode(writer, node->GetNthNode(i));

	xmlTextWriterEndElement(writer);

	return Success();
}
