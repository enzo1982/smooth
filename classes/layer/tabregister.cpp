 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TABREGISTER_
#define __OBJSMOOTH_TABREGISTER_

#include <smooth/tabregister.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/layer.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_TABREGISTER = SMOOTH::RequestObjectID();

SMOOTHTabRegister::SMOOTHTabRegister(SMOOTHPoint pos, SMOOTHSize size)
{
	self = this;

	type = OBJ_TABREGISTER;
	containerType = OBJ_TABREGISTER;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(120 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(100 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
}

SMOOTHTabRegister::~SMOOTHTabRegister()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHTabRegister::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHLayer	*object = NIL;
	SMOOTHLayer	*prev = NIL;
	SMOOTHRect	 frame;
	SMOOTHRect	 textrect;
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;

	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		if (i > 0) prev = object;

		object = (SMOOTHLayer *) assocObjects.GetNthEntry(i);

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

	for (SMOOTHInt j = 0; j < nOfObjects; j++)
	{
		if (j > 0) prev = object;

		object = (SMOOTHLayer *) assocObjects.GetNthEntry(j);

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

				PaintPixel(dc, SMOOTHPoint(frame.left, frame.bottom), SMOOTH::Setup::DividerLightColor);

				PaintPixel(dc, SMOOTHPoint(frame.left, frame.top), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.left + 1, frame.top), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.left, frame.top + 1), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.left + 1, frame.top + 1), SMOOTH::Setup::DividerLightColor);

				PaintPixel(dc, SMOOTHPoint(frame.right, frame.top), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.right - 1, frame.top), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.right, frame.top + 1), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.right - 1, frame.top + 1), SMOOTH::Setup::DividerDarkColor);

				lineStart.x = frame.left + 1;
				lineStart.y = frame.bottom;
				lineEnd.x = frame.right;
				lineEnd.y = lineStart.y;

				Line(dc, lineStart, lineEnd, SMOOTH::Setup::BackgroundColor, PS_SOLID, 1);

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

				PaintPixel(dc, SMOOTHPoint(frame.right, frame.top), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.right - 1, frame.top), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.right, frame.top + 1), SMOOTH::Setup::BackgroundColor);
				PaintPixel(dc, SMOOTHPoint(frame.right - 1, frame.top + 1), SMOOTH::Setup::DividerDarkColor);

				lineStart.x = frame.left;
				lineStart.y = frame.bottom;
				lineEnd.x = frame.right + 1;
				lineEnd.y = lineStart.y;

				Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerLightColor, PS_SOLID, 1);

				if (j == 0)
				{
					PaintPixel(dc, SMOOTHPoint(frame.left, frame.top), SMOOTH::Setup::BackgroundColor);
					PaintPixel(dc, SMOOTHPoint(frame.left + 1, frame.top), SMOOTH::Setup::BackgroundColor);
					PaintPixel(dc, SMOOTHPoint(frame.left, frame.top + 1), SMOOTH::Setup::BackgroundColor);
					PaintPixel(dc, SMOOTHPoint(frame.left + 1, frame.top + 1), SMOOTH::Setup::DividerLightColor);
				}

				if (j > 0) if (prev->IsVisible())
				{
					lineStart.x = frame.left;
					lineStart.y = frame.top + 1;
					lineEnd.x = lineStart.x;
					lineEnd.y = frame.bottom;

					Line(dc, lineStart, lineEnd, SMOOTH::Setup::BackgroundColor, PS_SOLID, 1);

					PaintPixel(dc, SMOOTHPoint(frame.left, frame.top), SMOOTH::Setup::BackgroundColor);
					PaintPixel(dc, SMOOTHPoint(frame.left, frame.top + 1), SMOOTH::Setup::DividerLightColor);

					frame.left--;
				}
				else
				{
					PaintPixel(dc, SMOOTHPoint(frame.left, frame.top), SMOOTH::Setup::BackgroundColor);
					PaintPixel(dc, SMOOTHPoint(frame.left + 1, frame.top), SMOOTH::Setup::BackgroundColor);
					PaintPixel(dc, SMOOTHPoint(frame.left, frame.top + 1), SMOOTH::Setup::BackgroundColor);
					PaintPixel(dc, SMOOTHPoint(frame.left + 1, frame.top + 1), SMOOTH::Setup::DividerLightColor);
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

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHTabRegister::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHInt	 retVal = SMOOTH::Success;
	SMOOTHLayer	*object;
	SMOOTHLayer	*object2;
	SMOOTHRect	 frame;

	GetSize();

	frame.top	= realPos.y;
	frame.right	= realPos.x;
	frame.bottom	= realPos.y + METRIC_REGISTEROFFSETY;

	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		object = (SMOOTHLayer *) assocObjects.GetNthEntry(i);

		object->SetContainer(myContainer->GetContainerObject()->GetContainer());

		object->GetObjectProperties()->pos.x = realPos.x + 2;
		object->GetObjectProperties()->pos.y = realPos.y + METRIC_REGISTEROFFSETY + 2;
		object->GetObjectProperties()->size.cx = objectProperties->size.cx - 3;
		object->GetObjectProperties()->size.cy = objectProperties->size.cy - METRIC_REGISTEROFFSETY - 3;

		if (object->IsVisible())
		{
			if (object->Process(message, wParam, lParam) == SMOOTH::Break) return SMOOTH::Break;
		}
	}

	switch (message)
	{
		case SM_LBUTTONDOWN:
			for (SMOOTHInt i = 0; i < nOfObjects; i++)
			{
				object = (SMOOTHLayer *) assocObjects.GetNthEntry(i);

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
							for (SMOOTHInt j = 0; j < nOfObjects; j++)
							{
								object2 = (SMOOTHLayer *) assocObjects.GetNthEntry(j);

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

							retVal = SMOOTH::Break;

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

SMOOTHVoid SMOOTHTabRegister::GetSize()
{
	HDC		 dc = GetContext(NIL);
	SMOOTHLayer	*object;

	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		object = (SMOOTHLayer *) assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (!sizeSet.GetEntry(object->handle))
			{
				textSize.SetEntry(object->handle, GetTextSizeX(dc, object->GetObjectProperties()->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight));

				sizeSet.SetEntry(object->handle, SMOOTH::True);
			}
		}
	}

	FreeContext(NIL, dc);
}

SMOOTHInt SMOOTHTabRegister::RegisterObject(SMOOTHObject *object)
{
	if (!registered)	return SMOOTH::Error;
	if (object == NIL)	return SMOOTH::Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);
			sizeSet.AddEntry(SMOOTH::False, object->handle);
			textSize.AddEntry(0, object->handle);

			nOfObjects++;

			object->SetContainer(myContainer->GetContainerObject()->GetContainer());
			object->SetRegisteredFlag();

			if (nOfObjects == 1)	((SMOOTHLayer *) object)->Show();
			else			((SMOOTHLayer *) object)->Hide();

			return SMOOTH::Success;
		}
	}

	return SMOOTH::Error;
}

SMOOTHInt SMOOTHTabRegister::UnregisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	SMOOTHBool	 activateNew = SMOOTH::False;

	if (containerType == &object->possibleContainers)
	{
		if (nOfObjects > 0 && object->IsRegistered())
		{
			if (((SMOOTHLayer *) object)->IsVisible())
			{
				((SMOOTHLayer *) object)->Hide();

				activateNew = SMOOTH::True;
			}

			if (assocObjects.DeleteEntry(object->handle) == SMOOTH::True)
			{
				sizeSet.DeleteEntry(object->handle);
				textSize.DeleteEntry(object->handle);
				nOfObjects--;

				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				if (activateNew && nOfObjects > 0) ((SMOOTHLayer *) assocObjects.GetFirstEntry())->Show();

				return SMOOTH::Success;
			}
		}
	}

	return SMOOTH::Error;
}

#endif
