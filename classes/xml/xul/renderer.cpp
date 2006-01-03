 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/renderer.h>
#include <smooth/xml/xul/window.h>
#include <smooth/xml/document.h>

S::XML::XUL::Renderer::Renderer() : Widget(NIL)
{
	window = NIL;
}

S::XML::XUL::Renderer::~Renderer()
{
	CloseXUL();
}

S::Int S::XML::XUL::Renderer::LoadXUL(const String &fileName)
{
	CloseXUL();

	Int		 rVal	= Error();
	Document	*xul	= new Document();

	xul->LoadFile(fileName);

	if (xul->GetRootNode() != NIL) rVal = RenderXUL(xul);

	delete xul;

	return rVal;
}

S::Int S::XML::XUL::Renderer::CloseXUL()
{
	if (window != NIL) delete window;

	window = NIL;

	return Success();
}

S::GUI::Widget *S::XML::XUL::Renderer::GetWidget()
{
	if (window == NIL) return NIL;

	return window->GetWidget();
}

S::Int S::XML::XUL::Renderer::RenderXUL(Document *xul)
{
	if (xul->GetRootNode()->GetName() == "window")
	{
		window = new XUL::Window(xul->GetRootNode());

		return Success();
	}

	return Error();
}
