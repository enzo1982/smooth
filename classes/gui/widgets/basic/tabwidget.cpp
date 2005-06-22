 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/tabwidget.h>
#include <smooth/definitions.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::TabWidget::classID = S::Object::RequestClassID();

S::GUI::TabWidget::TabWidget(Point iPos, Size iSize)
{
	type		= classID;
	containerType	= classID;

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 120;
	if (size.cy == 0) size.cy = 100;
}

S::GUI::TabWidget::~TabWidget()
{
}

S::Int S::GUI::TabWidget::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface	= container->GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Layer	*object		= NIL;
	Layer	*prev		= NIL;
	Rect	 frame;
	Rect	 textrect;
	Point	 lineStart;
	Point	 lineEnd;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (i > 0) prev = object;

		object = (Layer *) assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (object->GetContainer() == NIL)
			{
				object->SetContainer(container->GetContainer());
				object->SetRegisteredFlag(True);

				((Widget *) object)->onRegister.Emit(this);
			}

			object->pos.x = realPos.x;
			object->pos.y = realPos.y + 19;
			object->size.cx = size.cx;
			object->size.cy = size.cy - 19;
		}
	}

	GetLayersSize();

	frame = Rect(GetRealPosition(), size);

	frame.top += 19;
	surface->Frame(frame, FRAME_UP);
	frame.top -= 19;

	frame.right	= frame.left;
	frame.bottom	= frame.top + 19;

	for (Int j = 0; j < GetNOfObjects(); j++)
	{
		if (j > 0) prev = object;

		object = (Layer *) assocObjects.GetNthEntry(j);

		if (object != NIL)
		{
			object->pos.x = realPos.x + 2;
			object->pos.y = realPos.y + 21;
			object->size.cx = size.cx - 3;
			object->size.cy = size.cy - 22;

			if (object->IsVisible())
			{
				frame.left = frame.right;
				frame.right = frame.left + textSize.GetEntry(object->GetHandle()) + 14;

				frame.bottom++;

				surface->Frame(frame, FRAME_UP);

				frame.right--;
				frame.bottom--;

				if (Setup::rightToLeft) { frame.left++; frame.right++; }

				surface->SetPixel(Setup::rightToLeft ? frame.right : frame.left, frame.bottom, Setup::ClientColor);

				surface->SetPixel(frame.left, frame.top, Setup::BackgroundColor);
				surface->SetPixel(frame.left + 1, frame.top, Setup::BackgroundColor);
				surface->SetPixel(frame.left, frame.top + 1, Setup::BackgroundColor);
				surface->SetPixel(frame.left + 1, frame.top + 1, Setup::rightToLeft ? Setup::DividerDarkColor : Setup::ClientColor);

				surface->SetPixel(frame.right, frame.top, Setup::BackgroundColor);
				surface->SetPixel(frame.right - 1, frame.top, Setup::BackgroundColor);
				surface->SetPixel(frame.right, frame.top + 1, Setup::BackgroundColor);
				surface->SetPixel(frame.right - 1, frame.top + 1, Setup::rightToLeft ? Setup::ClientColor : Setup::DividerDarkColor);

				lineStart.x = frame.left + 1;
				lineStart.y = frame.bottom;
				lineEnd.x = frame.right;
				lineEnd.y = lineStart.y;

				surface->Line(lineStart, lineEnd, Setup::BackgroundColor);

				if (Setup::rightToLeft) { frame.left--; frame.right--; }

				textrect.left	= frame.left + 7;
				textrect.top	= frame.top + 2;
				textrect.right	= textrect.left + textSize.GetEntry(object->GetHandle());
				textrect.bottom	= textrect.top + 20;

				surface->SetText(object->GetText(), textrect, font);

				object->Paint(SP_PAINT);

				frame.right--;
			}
			else
			{
				frame.top++;
				frame.left = frame.right + 1;

				frame.right = frame.left + textSize.GetEntry(object->GetHandle()) + 12;

				if (j > 0) if (prev->IsVisible()) frame.left++;

				frame.bottom++;

				surface->Frame(frame, FRAME_UP);

				frame.right--;
				frame.bottom--;

				if (Setup::rightToLeft) { frame.left++; frame.right++; }

				surface->SetPixel(frame.right, frame.top, Setup::BackgroundColor);
				surface->SetPixel(frame.right - 1, frame.top, Setup::BackgroundColor);
				surface->SetPixel(frame.right, frame.top + 1, Setup::BackgroundColor);
				surface->SetPixel(frame.right - 1, frame.top + 1, Setup::rightToLeft ? Setup::ClientColor : Setup::DividerDarkColor);

				lineStart.x = frame.left;
				lineStart.y = frame.bottom;
				lineEnd.x = frame.right + 1;
				lineEnd.y = lineStart.y;

				surface->Line(lineStart, lineEnd, Setup::ClientColor);

				if (j == 0)
				{
					surface->SetPixel(frame.left, frame.top, Setup::BackgroundColor);
					surface->SetPixel(frame.left + 1, frame.top, Setup::BackgroundColor);
					surface->SetPixel(frame.left, frame.top + 1, Setup::BackgroundColor);
					surface->SetPixel(frame.left + 1, frame.top + 1, Setup::rightToLeft ? Setup::DividerDarkColor : Setup::ClientColor);
				}

				if (j > 0) if (prev->IsVisible())
				{
					lineStart.x = frame.left;
					lineStart.y = frame.top + 1;
					lineEnd.x = lineStart.x;
					lineEnd.y = frame.bottom;

					surface->Line(lineStart, lineEnd, Setup::BackgroundColor);

					surface->SetPixel(frame.left, frame.top, Setup::BackgroundColor);
					surface->SetPixel(frame.left, frame.top + 1, Setup::rightToLeft ? Setup::DividerDarkColor : Setup::ClientColor);

					frame.left--;
				}
				else
				{
					surface->SetPixel(frame.left, frame.top, Setup::BackgroundColor);
					surface->SetPixel(frame.left + 1, frame.top, Setup::BackgroundColor);
					surface->SetPixel(frame.left, frame.top + 1, Setup::BackgroundColor);
					surface->SetPixel(frame.left + 1, frame.top + 1, Setup::rightToLeft ? Setup::DividerDarkColor : Setup::ClientColor);
				}

				if (Setup::rightToLeft) { frame.left--; frame.right--; }

				textrect.left	= frame.left + 6;
				textrect.top	= frame.top + 2;
				textrect.right	= textrect.left + textSize.GetEntry(object->GetHandle());
				textrect.bottom	= textrect.top + 20;

				surface->SetText(object->GetText(), textrect, font);

				frame.top--;
			}
		}
	}

	return Success;
}

S::Int S::GUI::TabWidget::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		return Failure;
	if (!active || !IsVisible())	return Success;

	Window	*wnd		= container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface	= wnd->GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Int	 retVal		= Success;
	Layer	*object;
	Layer	*object2;
	Rect	 frame;

	GetLayersSize();

	frame.top	= realPos.y;
	frame.right	= realPos.x;
	frame.bottom	= realPos.y + 19;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		object = (Layer *) assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (object->GetContainer() == NIL)
			{
				object->SetContainer(container->GetContainer());
				object->SetRegisteredFlag(True);

				((Widget *) object)->onRegister.Emit(this);
			}

			object->pos.x = realPos.x + 2;
			object->pos.y = realPos.y + 21;
			object->size.cx = size.cx - 3;
			object->size.cy = size.cy - 22;

			if (object->IsVisible())
			{
				if (object->Process(message, wParam, lParam) == Break) return Break;
			}
		}
	}

	switch (message)
	{
		case SM_LBUTTONDOWN:
			for (Int i = 0; i < GetNOfObjects(); i++)
			{
				object = (Layer *) assocObjects.GetNthEntry(i);

				if (object != NIL)
				{
					if (!object->IsVisible())
					{
						frame.top++;
						frame.left = frame.right + 1;

						if (i == 0) frame.left--;

						frame.right = frame.left + textSize.GetEntry(object->GetHandle()) + 12;

						if (wnd->IsMouseOn(frame))
						{
							surface->StartPaint(Rect(Point(Setup::rightToLeft ? (wnd->size.cx - (realPos.x + size.cx)) : realPos.x, realPos.y), size));

							for (Int j = 0; j < GetNOfObjects(); j++)
							{
								object2 = (Layer *) assocObjects.GetNthEntry(j);

								if (object2 != NIL)
								{
									if (object2->IsVisible())
									{
										object2->Hide();
									}
								}
							}

							wnd->SetUpdateRect(Rect(Point(Setup::rightToLeft ? (wnd->size.cx - (realPos.x + size.cx)) : realPos.x, realPos.y), size));

							Hide();

							object->Show();

							Show();

							wnd->SetUpdateRect(Rect(Point(-1, -1), Size(0, 0)));

							surface->EndPaint();

							retVal = Break;

							break;
						}

						frame.top--;
					}
					else
					{
						frame.left = frame.right;
						frame.right = frame.left + textSize.GetEntry(object->GetHandle()) + 12;
					}
				}
			}
			break;
	}

	return retVal;
}

S::Void S::GUI::TabWidget::GetLayersSize()
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Layer	*object = (Layer *) assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (!sizeSet.GetEntry(object->GetHandle()))
			{
				textSize.SetEntry(object->GetHandle(), font.GetTextSizeX(object->GetText()));

				sizeSet.SetEntry(object->GetHandle(), True);
			}
		}
	}
}

S::Int S::GUI::TabWidget::SelectTab(Int layerid)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Layer	*object = (Layer *) assocObjects.GetNthEntry(i);

		if (object->GetHandle() == layerid)	object->Show();
		else					object->Hide();
	}

	return Success;
}

S::Int S::GUI::TabWidget::RegisterObject(Widget *object)
{
	if (!IsRegistered())	return Failure;
	if (object == NIL)	return Failure;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->GetHandle());
			sizeSet.AddEntry(False, object->GetHandle());
			textSize.AddEntry(0, object->GetHandle());

			if (GetNOfObjects() == 1)	object->Show();
			else				object->Hide();

			return Success;
		}
	}

	return Failure;
}

S::Int S::GUI::TabWidget::UnregisterObject(Widget *object)
{
	if (object == NIL) return Failure;

	Bool	 activateNew = False;

	if (containerType == &object->possibleContainers)
	{
		if (GetNOfObjects() > 0 && object->IsRegistered())
		{
			if (object->IsVisible())
			{
				object->Hide();

				activateNew = True;
			}

			object->onUnregister.Emit(this);

			if (assocObjects.RemoveEntry(object->GetHandle()) == True)
			{
				sizeSet.RemoveEntry(object->GetHandle());
				textSize.RemoveEntry(object->GetHandle());

				object->SetRegisteredFlag(False);
				object->SetContainer(NIL);

				if (activateNew && GetNOfObjects() > 0) assocObjects.GetFirstEntry()->Show();

				return Success;
			}
		}
	}

	return Failure;
}
