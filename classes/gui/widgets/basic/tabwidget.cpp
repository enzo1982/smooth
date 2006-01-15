 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/tabwidget.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::TabWidget::classID = S::Object::RequestClassID();

S::GUI::TabWidget::TabWidget(const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type	= classID;

	if (GetWidth() == 0) SetWidth(120);
	if (GetHeight() == 0) SetHeight(100);
}

S::GUI::TabWidget::~TabWidget()
{
}

S::Int S::GUI::TabWidget::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

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

		object = (Layer *) GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetContainer() == NIL)
			{
				object->SetContainer(container->GetContainer());
				object->SetRegisteredFlag(True);

				object->onRegister.Emit(this);
			}
		}
	}

	GetLayersSize();

	frame = Rect(GetRealPosition(), GetSize());

	frame.top += 19;
	surface->Frame(frame, FRAME_UP);
	frame.top -= 19;

	frame.right	= frame.left;
	frame.bottom	= frame.top + 19;

	for (Int j = 0; j < GetNOfObjects(); j++)
	{
		if (j > 0) prev = object;

		object = (Layer *) GetNthObject(j);

		if (object != NIL)
		{
			object->SetMetrics(Point(2, 21), GetSize() - Size(3, 22));

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

	return Success();
}

S::Int S::GUI::TabWidget::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())			return Error();
	if (!IsActive() || !IsVisible())	return Success();

	Window	*wnd		= container->GetContainerWindow();

	Surface	*surface	= wnd->GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Int	 retVal		= Success();
	Layer	*object;
	Layer	*object2;
	Rect	 frame;

	GetLayersSize();

	frame.top	= realPos.y;
	frame.right	= realPos.x;
	frame.bottom	= realPos.y + 19;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		object = (Layer *) GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetContainer() == NIL)
			{
				object->SetContainer(container->GetContainer());
				object->SetRegisteredFlag(True);

				object->onRegister.Emit(this);
			}

			object->SetMetrics(Point(2, 21), GetSize() - Size(3, 22));

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
				object = (Layer *) GetNthObject(i);

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
							surface->StartPaint(Rect(Point(Setup::rightToLeft ? (wnd->GetWidth() - (realPos.x + GetWidth())) : realPos.x, realPos.y), GetSize()));

							for (Int j = 0; j < GetNOfObjects(); j++)
							{
								object2 = (Layer *) GetNthObject(j);

								if (object2 != NIL)
								{
									if (object2->IsVisible())
									{
										object2->Hide();
									}
								}
							}

							wnd->SetUpdateRect(Rect(Point(Setup::rightToLeft ? (wnd->GetWidth() - (realPos.x + GetWidth())) : realPos.x, realPos.y), GetSize()));

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
		Layer	*object = (Layer *) GetNthObject(i);

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
		Layer	*object = (Layer *) GetNthObject(i);

		if (object->GetHandle() == layerid)	object->Show();
		else					object->Hide();
	}

	return Success();
}

S::Int S::GUI::TabWidget::RegisterObject(Widget *object)
{
	if (Widget::RegisterObject(object) == Success())
	{
		sizeSet.AddEntry(False, object->GetHandle());
		textSize.AddEntry(0, object->GetHandle());

		if (GetNOfObjects() == 1) object->Show();
		else			  object->Hide();

		return Success();
	}

	return Error();
}

S::Int S::GUI::TabWidget::UnregisterObject(Widget *object)
{
	if (object == NIL) return Error();

	Bool	 wasVisible = object->IsVisible();

	if (Widget::UnregisterObject(object) == Success())
	{
		sizeSet.RemoveEntry(object->GetHandle());
		textSize.RemoveEntry(object->GetHandle());

		if (wasVisible && GetNOfObjects() > 0) GetNthObject(0)->Show();

		return Success();
	}

	return Error();
}
