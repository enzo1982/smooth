 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/window.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/titlebar.h>
#include <smooth/statusbar.h>
#include <smooth/popupmenu.h>
#include <smooth/divider.h>
#include <smooth/layer.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/toolwindow.h>
#include <smooth/timer.h>
#include <smooth/stk.h>
#include <smooth/color.h>
#include <smooth/objectproperties.h>
#include <smooth/menubar.h>
#include <smooth/system.h>
#include <smooth/mdiwindow.h>
#include <smooth/input.h>
#include <smooth/resources.h>
#include <smooth/binary.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::WindowBase::classID = S::Object::RequestClassID();
S::Int		 S::GUI::WindowBase::nOfActiveWindows = 0;

S::GUI::WindowBase::WindowBase(String title)
{
	self = this;

	containerType = classID;

	possibleContainers.AddEntry(Application::classID);

	exstyle		= 0;
	stay		= False;
	maximized	= False;

	nOfActiveWindows++;

	type = classID;

	if (title != NIL)	objectProperties->text = title;
	else			objectProperties->text = "smooth Application";

	value = 0;

	innerOffset = Rect(Point(3, 3), Size(0, 0));

	updateRect = Rect(Point(-1, -1), Size(0, 0));
	timedUpdateRect = Rect(Point(-1, -1), Size(0, 0));

	icon = NIL;

	created		= False;
	visible		= False;
	destroyed	= False;
	initshow	= False;

	objectProperties->size.cx = Math::Round(200 * Setup::FontSize);
	objectProperties->size.cy = Math::Round(200 * Setup::FontSize);

	trackMenu = NIL;
	paintTimer = NIL;

	mainLayer = new Layer();

	RegisterObject(mainLayer);

	minSize.cx = Math::Round(160 * Setup::FontSize);
	minSize.cy = METRIC_TITLEBARHEIGHT + 5;

	doQuit.Connect(&WindowBase::DummyExitProc, this);

	onCreate.SetParentObject(this);
	onPaint.SetParentObject(this);
	onResize.SetParentObject(this);
	onPeek.SetParentObject(this);
	onEvent.SetParentObject(this);
}

S::GUI::WindowBase::~WindowBase()
{
	UnregisterObject(mainLayer);
	DeleteObject(mainLayer);

	if (trackMenu != NIL)
	{
		UnregisterObject(trackMenu);
		DeleteObject(trackMenu);
	}

	if (onPeek.GetNOfConnectedSlots() > 0) peekLoop--;

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Bool S::GUI::WindowBase::DummyExitProc()
{
	return True;
}

S::Int S::GUI::WindowBase::SetMetrics(Point newPos, Size newSize)
{
	updateRect = Rect();

	objectProperties->pos.x		= Math::Round(newPos.x * Setup::FontSize);
	objectProperties->pos.y		= Math::Round(newPos.y * Setup::FontSize);
	objectProperties->size.cx	= Math::Round(newSize.cx * Setup::FontSize);
	objectProperties->size.cy	= Math::Round(newSize.cy * Setup::FontSize);

	return Success;
}

S::Void S::GUI::WindowBase::SetStyle(Int s)
{
	if (!created) style = style | s;
}

S::Void S::GUI::WindowBase::SetExStyle(Int es)
{
	if (!created) exstyle = exstyle | es;
}

S::Int S::GUI::WindowBase::SetIcon(const Bitmap &nIcon)
{
	Bitmap	 newIcon = nIcon;

	if (newIcon != NIL)
	{
		if (newIcon.GetSize().cx != 20 || newIcon.GetSize().cy != 20)
		{
			return Error;
		}
		else
		{
			icon = newIcon;

			icon.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

			return Success;
		}
	}
	else
	{
		return Success;
	}
}

S::GUI::Bitmap &S::GUI::WindowBase::GetIcon()
{
	return icon;
}

S::Int S::GUI::WindowBase::SetApplicationIcon(char *newIcon)
{
	return Success;
}

S::Int S::GUI::WindowBase::SetApplicationIcon(wchar_t *newIcon)
{
	return Success;
}

S::GUI::Layer *S::GUI::WindowBase::GetMainLayer()
{
	return mainLayer;
}

S::Int S::GUI::WindowBase::SetText(String title)
{
	objectProperties->text = title;

	if (created) Process(SM_WINDOWTITLECHANGED, 0, 0);

	return Success;
}

S::Int S::GUI::WindowBase::SetStatusText(String newStatus)
{
	for (Int i = 0; i < nOfObjects; i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Statusbar::classID)
		{
			((Statusbar *) object)->SetText(newStatus);

			return Success;
		}
	}

	return Error;
}

S::String S::GUI::WindowBase::GetStatusText()
{
	for (Int i = 0; i < nOfObjects; i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Statusbar::classID)
		{
			return ((Statusbar *) object)->GetText();
		}
	}

	return NIL;
}

S::Int S::GUI::WindowBase::Show()
{
	if (!created) Create();

	initshow	= True;
	visible		= True;

	return Success;
}

S::Int S::GUI::WindowBase::Hide()
{
	if (!created) Create();

	initshow	= True;
	visible		= False;

	return Success;
}

S::Int S::GUI::WindowBase::Maximize()
{
	maximized = True;

	return Success;
}

S::Int S::GUI::WindowBase::Restore()
{
	maximized = False;

	return Success;
}

S::Bool S::GUI::WindowBase::IsMaximized()
{
	if (!created) return False;

	return maximized;
}

S::Rect S::GUI::WindowBase::GetWindowRect()
{
	if (maximized)	return nonmaxrect;
	else		return Rect(objectProperties->pos, objectProperties->size);
}

S::Rect S::GUI::WindowBase::GetClientRect()
{
	return innerOffset;
}

S::Rect S::GUI::WindowBase::GetUpdateRect()
{
	if (timedUpdateRect.left == -1	&&
	    timedUpdateRect.top == -1	&&
	    timedUpdateRect.right == -1	&&
	    timedUpdateRect.bottom == -1)	return updateRect;
	else					return timedUpdateRect;
}

S::Int S::GUI::WindowBase::SetUpdateRect(Rect newUpdateRect)
{
	updateRect = newUpdateRect;

	return Success;
}

S::Int S::GUI::WindowBase::SetMinimumSize(Size newMinSize)
{
	minSize = newMinSize;

	return Success;
}

S::Int S::GUI::WindowBase::SetMaximumSize(Size newMaxSize)
{
	maxSize = newMaxSize;

	return Success;
}

S::Bool S::GUI::WindowBase::Create()
{
	if (registered && !created)
	{
		created = True;
		visible = False;

		CalculateOffsets();

		drawSurface = new Surface();
		drawSurface->SetSize(objectProperties->size);

		onCreate.Emit();

		return True;
	}

	if (onPeek.GetNOfConnectedSlots() > 0) peekLoop++;

	return False;
}

S::Int S::GUI::WindowBase::Stay()
{
	if (!registered) return value;

	SetFlags(flags | WF_MODAL);

	if (!created)	Create();
	if (!visible)	Show();

	stay	= True;

	return value;
}

S::Int S::GUI::WindowBase::Close()
{
	destroyed = True;

	return Success;
}

S::Bool S::GUI::WindowBase::IsInUse()
{
	return (created && !destroyed);
}

S::Int S::GUI::WindowBase::Process(Int message, Int wParam, Int lParam)
{
	if (!created) return Success;

	EnterProtectedRegion();

	if (!(message == SM_MOUSEMOVE && wParam == 1)) onEvent.Emit(message, wParam, lParam);

	switch (message)
	{
		case SM_EXECUTEPEEK:
			onPeek.Emit();

			LeaveProtectedRegion();

			return 0;
		case SM_RBUTTONDOWN:
			{
				Menu	*track = getTrackMenu.Call(MouseX(), MouseY());

				if (track != NIL)
				{
					trackMenu = new PopupMenu(track);

					trackMenu->GetObjectProperties()->pos.x = MouseX();
					trackMenu->GetObjectProperties()->pos.y = MouseY();

					trackMenu->onClick.Connect(&WindowBase::PopupProc, this);

					RegisterObject(trackMenu);

					LeaveProtectedRegion();

					return 0;
				}
			}

			break;
	}

	for (Int i = nOfObjects - 1; i >= 0; i--)
	{
		Object	*object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Widget::classID)
		{
			if (((Widget *) object)->Process(message, wParam, lParam) == Break)
			{
				LeaveProtectedRegion();

				return 0;
			}
		}
	}

	LeaveProtectedRegion();

	return -1;
}

S::Int S::GUI::WindowBase::Paint(Int message)
{
	if (!registered)	return Error;
	if (!created)		return Success;
	if (!visible)		return Success;

	if ((updateRect.right - updateRect.left == 0) || (updateRect.bottom - updateRect.top == 0)) return Success;

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

	if (updateRect.left < 2)				updateRect.left		= 2;
	if (updateRect.top < 2)					updateRect.top		= 2;
	if (objectProperties->size.cx - updateRect.right < 2)	updateRect.right	= objectProperties->size.cx - 2;
	if (objectProperties->size.cy - updateRect.bottom < 2)	updateRect.bottom	= objectProperties->size.cy - 2;

	if (message == SP_UPDATE)
	{
		surface->PaintRect(updateRect);
	}
	else
	{
		surface->StartPaint(updateRect);
		surface->Box(updateRect, GetSysColor(COLOR_BTNFACE), FILLED);

		Widget	*lastWidget = NIL;
		Point	 doublebar1;
		Point	 doublebar2;
		Int	 bias = 0;
		Int	 topoffset = 3;
		Int	 rightobjcount = 0;
		Int	 leftobjcount = 0;
		Int	 btmobjcount = 0;
		Int	 topobjcount = 0;

		for (Int i = 0; i < nOfObjects; i++)
		{
			Object	*object = assocObjects.GetNthEntry(i);

			if (object->GetObjectType() == Widget::classID)
			{
				if (object->GetObjectProperties()->orientation == OR_TOP)
				{
					topobjcount++;

					lastWidget = (Widget *) object;

					if (((Widget *) object)->subtype == WO_SEPARATOR)
					{
						bias = -3;

						topoffset += object->GetObjectProperties()->size.cy + 3;

						doublebar1.x = 4;
						doublebar1.y = topoffset - 2;
						doublebar2.x = objectProperties->size.cx - 4;
						doublebar2.y = doublebar1.y;

						if (icon != NIL) doublebar1.x += METRIC_TITLEBARHEIGHT - 2;
						if (Setup::rightToLeft) doublebar1.x++;

						surface->Bar(doublebar1, doublebar2, OR_HORZ);
					}
					else
					{
						bias = 0;

						topoffset += object->GetObjectProperties()->size.cy;
					}
				}
				else if (object->GetObjectProperties()->orientation == OR_BOTTOM)
				{
					btmobjcount++;
				}
				else if (object->GetObjectProperties()->orientation == OR_LEFT)
				{
					leftobjcount++;
				}
				else if (object->GetObjectProperties()->orientation == OR_RIGHT)
				{
					rightobjcount++;
				}
			}
		}

		if (topobjcount > 0)
		{
			doublebar1.x = 4;
			doublebar1.y = innerOffset.top - 2 + bias;
			doublebar2.x = objectProperties->size.cx - 4;

			if (topobjcount > 0) if (lastWidget->subtype == WO_NOSEPARATOR) doublebar1.y -= 3;

			doublebar2.y = doublebar1.y;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);

			doublebar1.y = doublebar1.y + 2;
			doublebar2.y = doublebar2.y + 2;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);
		}

		if (btmobjcount > 0)
		{
			doublebar1.x = 4;
			doublebar1.y = objectProperties->size.cy - innerOffset.bottom;
			doublebar2.x = objectProperties->size.cx - 4;
			doublebar2.y = doublebar1.y;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);

			doublebar1.y = doublebar1.y + 2;
			doublebar2.y = doublebar2.y + 2;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);
		}

		if (leftobjcount > 0)
		{
			doublebar1.x = innerOffset.left - 3;
			doublebar1.y = innerOffset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = objectProperties->size.cy - innerOffset.bottom - 2;

			surface->Bar(doublebar1, doublebar2, OR_VERT);
		}

		if (rightobjcount > 0)
		{
			doublebar1.x = objectProperties->size.cx - innerOffset.right + 1;
			doublebar1.y = innerOffset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = objectProperties->size.cy - innerOffset.bottom - 2;

			surface->Bar(doublebar1, doublebar2, OR_VERT);
		}

		for (Int j = 0; j < nOfObjects; j++)
		{
			Object	*object = assocObjects.GetNthEntry(j);

			if (object == NIL) continue;

			if (object->GetObjectType() == Widget::classID)
			{
				if (((Widget *) object)->IsVisible() && ((Widget *) object)->IsAffected(updateRect) && object->GetObjectType() != Layer::classID) ((Widget *) object)->Paint(SP_PAINT);
			}
		}

		if ((message == SP_DELAYED) && (flags & WF_DELAYPAINT))
		{
			if (paintTimer != NIL) DeleteObject(paintTimer);

			if (timedUpdateRect.left == -1 && timedUpdateRect.top == -1 && timedUpdateRect.right == -1 && timedUpdateRect.bottom == -1)
			{
				timedUpdateRect = updateRect;
			}
			else
			{
				timedUpdateRect.left	= (Int) Math::Min(timedUpdateRect.left, updateRect.left);
				timedUpdateRect.top	= (Int) Math::Min(timedUpdateRect.top, updateRect.top);
				timedUpdateRect.right	= (Int) Math::Max(timedUpdateRect.right, updateRect.right);
				timedUpdateRect.bottom	= (Int) Math::Max(timedUpdateRect.bottom, updateRect.bottom);
			}

			paintTimer = new Timer();
			paintTimer->onInterval.Connect(&WindowBase::PaintTimer, this);
			paintTimer->Start(50);
		}
		else if (paintTimer == NIL)
		{
			timedUpdateRect = updateRect;

			PaintTimer();
		}

		surface->EndPaint();
	}

	LeaveProtectedRegion();

	return Success;
}

S::Void S::GUI::WindowBase::CalculateOffsets()
{
	if (type == ToolWindow::classID) return;

	Object	*operat;
	Widget	*lastWidget = NIL;
	Int	 rightobjcount = 0;
	Int	 leftobjcount = 0;
	Int	 btmobjcount = 0;
	Int	 topobjcount = 0;
	Int	 i;

	innerOffset = Rect(Point(3, 3), Size(0, 0));

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_TOP)
		{
			topobjcount++;

			lastWidget = (Widget *) operat;

			operat->GetObjectProperties()->pos.x	= innerOffset.left;
			operat->GetObjectProperties()->pos.y	= innerOffset.top;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - innerOffset.left - innerOffset.right;

			innerOffset.top += operat->GetObjectProperties()->size.cy;

			if (((Widget *) operat)->subtype == WO_SEPARATOR) innerOffset.top += 3;
		}
	}

	if (topobjcount > 0)
	{
		innerOffset.top += 3;

		if (lastWidget->subtype == WO_NOSEPARATOR) innerOffset.top += 3;
	}

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_BOTTOM)
		{
			btmobjcount++;

			operat->GetObjectProperties()->pos.x	= innerOffset.left;
			operat->GetObjectProperties()->pos.y	= objectProperties->size.cy - innerOffset.bottom - operat->GetObjectProperties()->size.cy;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - innerOffset.left - innerOffset.right;

			innerOffset.bottom += operat->GetObjectProperties()->size.cy;
		}
	}

	if (btmobjcount > 0) innerOffset.bottom += 4;

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_LEFT)
		{
			leftobjcount++;

			operat->GetObjectProperties()->pos.x	= innerOffset.left;
			operat->GetObjectProperties()->pos.y	= innerOffset.top;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - innerOffset.top - innerOffset.bottom;

			innerOffset.left += operat->GetObjectProperties()->size.cx;
		}
	}

	if (leftobjcount > 0) innerOffset.left += 3;

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_RIGHT)
		{
			rightobjcount++;

			operat->GetObjectProperties()->pos.x	= objectProperties->size.cx - innerOffset.right - operat->GetObjectProperties()->size.cx;
			operat->GetObjectProperties()->pos.y	= innerOffset.top;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - innerOffset.top - innerOffset.bottom;

			innerOffset.right += operat->GetObjectProperties()->size.cx;
		}
	}

	if (rightobjcount > 0) innerOffset.right += 3;

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_CENTER)
		{
			operat->GetObjectProperties()->pos.x	= innerOffset.left;
			operat->GetObjectProperties()->pos.y	= innerOffset.top;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - innerOffset.left - innerOffset.right;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - innerOffset.top - innerOffset.bottom;
		}
	}
}

S::Int S::GUI::WindowBase::MouseX()
{
	if (Setup::rightToLeft)	return objectProperties->size.cx - (Input::MouseX() - objectProperties->pos.x) - 1;
	else			return Input::MouseX() - objectProperties->pos.x;
}

S::Int S::GUI::WindowBase::MouseY()
{
	return Input::MouseY() - objectProperties->pos.y;
}

S::Bool S::GUI::WindowBase::IsMouseOn(Rect rect)
{
	if ((MouseX() >= rect.left) && (MouseX() <= rect.right) && (MouseY() >= rect.top) && (MouseY() <= rect.bottom))	return True;
	else														return False;
}

S::Void S::GUI::WindowBase::PopupProc()
{
	if (trackMenu != NIL)
	{
		trackMenu->Hide();

		DeleteObject(trackMenu);

		trackMenu = NIL;
	}
}

S::Int S::GUI::WindowBase::RegisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);
			nOfObjects++;

			object->SetContainer(this);
			object->SetRegisteredFlag();

			if (object->GetObjectType() == Titlebar::classID)
			{
				if (!Binary::IsFlagSet(object->GetFlags(), TB_MAXBUTTON)) style = (style ^ WS_THICKFRAME) | WS_DLGFRAME;
			}
			else if (object->GetObjectType() == ToolWindow::classID)
			{
				if (Setup::rightToLeft)	object->GetObjectProperties()->pos.x = objectProperties->size.cx - ((object->GetObjectProperties()->pos.x - objectProperties->pos.x) + object->GetObjectProperties()->size.cx) + objectProperties->pos.x;
				((WindowBase *) object)->Create();
			}

			CalculateOffsets();

			if (object->GetObjectType() == Widget::classID)
			{
				((Widget *) object)->onRegister.Emit(this);
				((Widget *) object)->Show();
			}

			return Success;
		}
	}
	else
	{
		return mainLayer->RegisterObject(object);
	}

	return Error;
}

S::Int S::GUI::WindowBase::UnregisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (nOfObjects > 0 && object->IsRegistered())
		{
			if (assocObjects.RemoveEntry(object->handle) == True)
			{
				nOfObjects--;

				if (object->GetObjectType() == Widget::classID)
				{
					((Widget *) object)->onUnregister.Emit(this);
					((Widget *) object)->Hide();
				}

				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				CalculateOffsets();

				return Success;
			}
		}
	}
	else
	{
		return mainLayer->UnregisterObject(object);
	}

	return Error;
}

S::Void S::GUI::WindowBase::PaintTimer()
{
	if (paintTimer != NIL)
	{
		DeleteObject(paintTimer);

		paintTimer = NIL;
	}

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(timedUpdateRect);

	for (Int j = 0; j < nOfObjects; j++)
	{
		Object	*object = assocObjects.GetNthEntry(j);

		if (object == NIL) continue;

		if (object->GetObjectType() == Widget::classID)
		{
			if (((Widget *) object)->IsVisible() && ((Widget *) object)->IsAffected(timedUpdateRect) && object->GetObjectType() == Layer::classID) ((Widget *) object)->Paint(SP_PAINT);
		}
	}

	onPaint.Emit();

	surface->EndPaint();

	timedUpdateRect = Rect(Point(-1, -1), Size(0, 0));

	LeaveProtectedRegion();
}
