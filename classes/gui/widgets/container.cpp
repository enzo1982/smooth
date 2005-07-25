 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/container.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::Container::classID = S::Object::RequestClassID();

S::GUI::Container::Container() : containerType(this)
{
	containerType	= classID;

	backgroundColor	= -1;

	nullSurface	= new GUI::Surface();
	drawSurface	= nullSurface;
}

S::GUI::Container::~Container()
{
	Int	 nOfObjects = assocObjects.GetNOfEntries();

	for (Int i = 0; i < nOfObjects; i++)
	{
		UnregisterObject(assocObjects.GetFirstEntry());
	}

	assocObjects.RemoveAll();

	delete nullSurface;
}

S::GUI::Color S::GUI::Container::GetBackgroundColor()
{
	return backgroundColor;
}

S::Int S::GUI::Container::SetBackgroundColor(const Color &nColor)
{
	backgroundColor = nColor;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::Container::RegisterObject(Widget *widget)
{
	if (widget == NIL) return Failure;

	if (containerType == &widget->possibleContainers)
	{
		if (!widget->IsRegistered())
		{
			assocObjects.AddEntry(widget, widget->GetHandle());

			widget->SetContainer(this);
			widget->SetRegisteredFlag(True);

			widget->onRegister.Emit(this);

			return Success;
		}
	}

	return Failure;
}

S::Int S::GUI::Container::UnregisterObject(Widget *widget)
{
	if (widget == NIL) return Failure;

	if (containerType == &widget->possibleContainers)
	{
		if (widget->IsRegistered())
		{
			if (assocObjects.RemoveEntry(widget->GetHandle()) == True)
			{
				widget->onUnregister.Emit(this);

				widget->SetRegisteredFlag(False);
				widget->SetContainer(NIL);

				return Success;
			}
		}
	}

	return Failure;
}

S::Int S::GUI::Container::GetNOfObjects()
{
	return assocObjects.GetNOfEntries();
}

S::GUI::Widget *S::GUI::Container::GetNthObject(Int n)
{
	return assocObjects.GetNthEntry(n);
}

S::GUI::Widget *S::GUI::Container::RequestObject(Int objectHandle)
{
	return assocObjects.GetEntry(objectHandle);
}

S::Bool S::GUI::Container::IsContainerCompatible(Int objType)
{
	if (objType == classID)	return True;
	else			return False;
}

S::GUI::ContainerType S::GUI::Container::GetContainerType()
{
	return containerType;
}

S::GUI::Window *S::GUI::Container::GetContainerWindow()
{
	Widget	*widget = this;

	while (widget->GetObjectType() != Window::classID)
	{
		if (widget->GetContainer() == NIL) return NIL;

		widget = widget->GetContainer();

		if (widget == NIL) return NIL;
	}

	return (Window *) widget;
}

S::GUI::Surface *S::GUI::Container::GetDrawSurface()
{
	if (IsRegistered())	return container->GetDrawSurface();
	else			return drawSurface;
}
