 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/tabwidget.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/layer.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_TABREGISTER = S::Object::RequestObjectID();

S::GUI::TabWidget::TabWidget(Point pos, Size size)
{
	self = this;

	type = OBJ_TABREGISTER;
	containerType = OBJ_TABREGISTER;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(120 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(100 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
}

S::GUI::TabWidget::~TabWidget()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::TabWidget::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC	 dc = GetContext(wnd);
	Point	 realPos = GetRealPosition();
	Layer	*object = NIL;
	Layer	*prev = NIL;
	Rect	 frame;
	Rect	 textrect;
	Point	 lineStart;
	Point	 lineEnd;

	for (Int i = 0; i < nOfObjects; i++)
	{
		if (i > 0) prev = object;

		object = (Layer *) assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			object->SetContainer(myContainer->GetContainerObject()->GetContainer());

			object->GetObjectProperties()->pos.x = realPos.x;
			object->GetObjectProperties()->pos.y = realPos.y + METRIC_REGISTEROFFSETY;
			object->GetObjectProperties()->size.cx = objectProperties->size.cx;
			object->GetObjectProperties()->size.cy = objectProperties->size.cy - METRIC_REGISTEROFFSETY;
		}
	}

	GetSize();

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx;
	frame.bottom	= realPos.y + objectProperties->size.cy;

	frame.top += METRIC_REGISTEROFFSETY;
	Frame(dc, frame, FRAME_UP);
	frame.top -= METRIC_REGISTEROFFSETY;

	frame.right	= frame.left;
	frame.bottom	= frame.top + METRIC_REGISTEROFFSETY;

	for (Int j = 0; j < nOfObjects; j++)
	{
		if (j > 0) prev = object;

		object = (Layer *) assocObjects.GetNthEntry(j);

		if (object != NIL)
		{
			object->GetObjectProperties()->pos.x = realPos.x + 2;
			object->GetObjectProperties()->pos.y = realPos.y + METRIC_REGISTEROFFSETY + 2;
			object->GetObjectProperties()->size.cx = objectProperties->size.cx - 3;
			object->GetObjectProperties()->size.cy = objectProperties->size.cy - METRIC_REGISTEROFFSETY - 3;

			if (object->IsVisible())
			{
				frame.left = frame.right;
				frame.right = frame.left + textSize.GetEntry(object->handle) + 2 * METRIC_REGISTERTEXTOFFSETX - 1;

				Frame(dc, frame, FRAME_UP);

				PaintPixel(dc, Point(frame.left, frame.bottom), Setup::DividerLightColor);

				PaintPixel(dc, Point(frame.left, frame.top), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.left + 1, frame.top), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.left, frame.top + 1), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.left + 1, frame.top + 1), Setup::DividerLightColor);

				PaintPixel(dc, Point(frame.right, frame.top), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.right - 1, frame.top), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.right, frame.top + 1), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.right - 1, frame.top + 1), Setup::DividerDarkColor);

				lineStart.x = frame.left + 1;
				lineStart.y = frame.bottom;
				lineEnd.x = frame.right;
				lineEnd.y = lineStart.y;

				Line(dc, lineStart, lineEnd, Setup::BackgroundColor, PS_SOLID, 1);

				textrect.left	= frame.left + METRIC_REGISTERTEXTOFFSETX;
				textrect.top	= frame.top + METRIC_REGISTERTEXTOFFSETY;
				textrect.right	= textrect.left + textSize.GetEntry(object->handle);
				textrect.bottom	= textrect.top + 20;

				::SetText(dc, object->GetObjectProperties()->text, textrect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

				object->Paint(SP_PAINT);

				frame.right--;
			}
			else
			{
				frame.top++;
				frame.left = frame.right + 1;

				frame.right = frame.left + textSize.GetEntry(object->handle) + 2 * METRIC_REGISTERTEXTOFFSETX - 3;

				if (j > 0) if (prev->IsVisible()) frame.left++;

				Frame(dc, frame, FRAME_UP);

				PaintPixel(dc, Point(frame.right, frame.top), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.right - 1, frame.top), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.right, frame.top + 1), Setup::BackgroundColor);
				PaintPixel(dc, Point(frame.right - 1, frame.top + 1), Setup::DividerDarkColor);

				lineStart.x = frame.left;
				lineStart.y = frame.bottom;
				lineEnd.x = frame.right + 1;
				lineEnd.y = lineStart.y;

				Line(dc, lineStart, lineEnd, Setup::DividerLightColor, PS_SOLID, 1);

				if (j == 0)
				{
					PaintPixel(dc, Point(frame.left, frame.top), Setup::BackgroundColor);
					PaintPixel(dc, Point(frame.left + 1, frame.top), Setup::BackgroundColor);
					PaintPixel(dc, Point(frame.left, frame.top + 1), Setup::BackgroundColor);
					PaintPixel(dc, Point(frame.left + 1, frame.top + 1), Setup::DividerLightColor);
				}

				if (j > 0) if (prev->IsVisible())
				{
					lineStart.x = frame.left;
					lineStart.y = frame.top + 1;
					lineEnd.x = lineStart.x;
					lineEnd.y = frame.bottom;

					Line(dc, lineStart, lineEnd, Setup::BackgroundColor, PS_SOLID, 1);

					PaintPixel(dc, Point(frame.left, frame.top), Setup::BackgroundColor);
					PaintPixel(dc, Point(frame.left, frame.top + 1), Setup::DividerLightColor);

					frame.left--;
				}
				else
				{
					PaintPixel(dc, Point(frame.left, frame.top), Setup::BackgroundColor);
					PaintPixel(dc, Point(frame.left + 1, frame.top), Setup::BackgroundColor);
					PaintPixel(dc, Point(frame.left, frame.top + 1), Setup::BackgroundColor);
					PaintPixel(dc, Point(frame.left + 1, frame.top + 1), Setup::DividerLightColor);
				}

				textrect.left	= frame.left + METRIC_REGISTERTEXTOFFSETX - 1;
				textrect.top	= frame.top + METRIC_REGISTERTEXTOFFSETY;
				textrect.right	= textrect.left + textSize.GetEntry(object->handle);
				textrect.bottom	= textrect.top + 20;

				::SetText(dc, object->GetObjectProperties()->text, textrect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

				frame.top--;
			}
		}
	}

	FreeContext(wnd, dc);

	return Success;
}

S::Int S::GUI::TabWidget::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Layer	*object;
	Layer	*object2;
	Rect	 frame;

	GetSize();

	frame.top	= realPos.y;
	frame.right	= realPos.x;
	frame.bottom	= realPos.y + METRIC_REGISTEROFFSETY;

	for (Int i = 0; i < nOfObjects; i++)
	{
		object = (Layer *) assocObjects.GetNthEntry(i);

		object->SetContainer(myContainer->GetContainerObject()->GetContainer());

		object->GetObjectProperties()->pos.x = realPos.x + 2;
		object->GetObjectProperties()->pos.y = realPos.y + METRIC_REGISTEROFFSETY + 2;
		object->GetObjectProperties()->size.cx = objectProperties->size.cx - 3;
		object->GetObjectProperties()->size.cy = objectProperties->size.cy - METRIC_REGISTEROFFSETY - 3;

		if (object->IsVisible())
		{
			if (object->Process(message, wParam, lParam) == Break) return Break;
		}
	}

	switch (message)
	{
		case SM_LBUTTONDOWN:
			for (Int i = 0; i < nOfObjects; i++)
			{
				object = (Layer *) assocObjects.GetNthEntry(i);

				if (object != NIL)
				{
					if (!object->IsVisible())
					{
						frame.top++;
						frame.left = frame.right + 1;

						if (i == 0) frame.left--;

						frame.right = frame.left + textSize.GetEntry(object->handle) + 2 * METRIC_REGISTERTEXTOFFSETX - 2;

						if (IsMouseOn(wnd->hwnd, frame, WINDOW))
						{
							for (Int j = 0; j < nOfObjects; j++)
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

							Hide();

							object->Show();

							Show();

							retVal = Break;

							break;
						}

						frame.top--;
					}
					else
					{
						frame.left = frame.right;
						frame.right = frame.left + textSize.GetEntry(object->handle) + 2 * METRIC_REGISTERTEXTOFFSETX - 2;
					}
				}
			}
			break;
	}

	return retVal;
}

S::Void S::GUI::TabWidget::GetSize()
{
	HDC	 dc = GetContext(NIL);
	Layer	*object;

	for (Int i = 0; i < nOfObjects; i++)
	{
		object = (Layer *) assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (!sizeSet.GetEntry(object->handle))
			{
				textSize.SetEntry(object->handle, GetTextSizeX(dc, object->GetObjectProperties()->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight));

				sizeSet.SetEntry(object->handle, True);
			}
		}
	}

	FreeContext(NIL, dc);
}

S::Int S::GUI::TabWidget::SelectTab(Int layerid)
{
	Layer	*object;

	for (Int i = 0; i < nOfObjects; i++)
	{
		object = (Layer *) assocObjects.GetNthEntry(i);

		if (object->handle == layerid)	object->Show();
		else				object->Hide();
	}

	return Success;
}

S::Int S::GUI::TabWidget::RegisterObject(Object *object)
{
	if (!registered)	return Error;
	if (object == NIL)	return Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);
			sizeSet.AddEntry(False, object->handle);
			textSize.AddEntry(0, object->handle);

			nOfObjects++;

			object->SetContainer(myContainer->GetContainerObject()->GetContainer());
			object->SetRegisteredFlag();

			if (nOfObjects == 1)	((Layer *) object)->Show();
			else			((Layer *) object)->Hide();

			return Success;
		}
	}

	return Error;
}

S::Int S::GUI::TabWidget::UnregisterObject(Object *object)
{
	if (object == NIL) return Error;

	Bool	 activateNew = False;

	if (containerType == &object->possibleContainers)
	{
		if (nOfObjects > 0 && object->IsRegistered())
		{
			if (((Layer *) object)->IsVisible())
			{
				((Layer *) object)->Hide();

				activateNew = True;
			}

			if (assocObjects.DeleteEntry(object->handle) == True)
			{
				sizeSet.DeleteEntry(object->handle);
				textSize.DeleteEntry(object->handle);
				nOfObjects--;

				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				if (activateNew && nOfObjects > 0) ((Layer *) assocObjects.GetFirstEntry())->Show();

				return Success;
			}
		}
	}

	return Error;
}
