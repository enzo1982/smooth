 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#if (defined UNICODE && !defined _UNICODE) || (defined _UNICODE && !defined UNICODE)
#error Either none or both of UNICODE and _UNICODE must be defined to compile SMOOTH applications!
#endif

#ifndef _H_OBJSMOOTH__
#define _H_OBJSMOOTH__

#define __SMOOTH__

#include "iolib-cxx.h"

#include "smooth/application.h"
#include "smooth/definitions.h"

#ifdef __WIN32__
#include "smooth/i18n.h"
#include "smooth/object.h"
#include "smooth/container.h"
#endif

#include "smooth/array.h"
#include "smooth/stk.h"

#ifdef __WIN32__
#include "smooth/graphics/window.h"
#include "smooth/titlebar.h"
#include "smooth/statusbar.h"
#include "smooth/menu.h"
#include "smooth/client.h"
#include "smooth/menubar.h"
#include "smooth/popupmenu.h"
#include "smooth/divider.h"
#include "smooth/layer.h"
#include "smooth/button.h"
#include "smooth/checkbox.h"
#include "smooth/optionbox.h"
#include "smooth/groupbox.h"
#include "smooth/text.h"
#include "smooth/activearea.h"
#include "smooth/hyperlink.h"
#include "smooth/image.h"
#include "smooth/arrows.h"
#include "smooth/tabwidget.h"
#include "smooth/slider.h"
#include "smooth/editbox.h"
#include "smooth/scrollbar.h"
#include "smooth/tree.h"
#include "smooth/list.h"
#include "smooth/progressbar.h"
#include "smooth/listbox.h"
#include "smooth/combobox.h"
#include "smooth/treeview.h"
#include "smooth/objectproperties.h"
#include "smooth/metrics.h"
#endif

#include "smooth/loop.h"
#include "smooth/version.h"

#ifdef __WIN32__
#include "smooth/mdiclient.h"
#include "smooth/mdiwindow.h"
#endif

#include "smooth/thread.h"

#ifdef __WIN32__
#include "smooth/toolwindow.h"
#endif

#include "smooth/mutex.h"
#include "smooth/semaphore.h"

#ifdef __WIN32__
#include "smooth/timer.h"
#endif

#include "smooth/string.h"

#ifdef __WIN32__
#include "smooth/tooltip.h"
#endif

#include "smooth/point.h"
#include "smooth/size.h"
#include "smooth/rect.h"
#include "smooth/line.h"

#ifdef __WIN32__
#include "smooth/drag.h"
#endif

#include "smooth/datetime.h"
#include "smooth/xml/attribute.h"
#include "smooth/xml/node.h"
#include "smooth/xml/document.h"
#include "smooth/types/bool.h"
#include "smooth/types/float.h"
#include "smooth/types/int.h"
#include "smooth/types/void.h"

#ifdef __WIN32__
#include "smooth/graphics/surface.h"
#include "smooth/filedlg.h"
#include "smooth/dirdlg.h"
#include "smooth/colordlg.h"
#endif

#include "smooth/binary.h"
#include "smooth/math.h"

#ifdef __WIN32__
#include "smooth/widget.h"
#endif

#include "smooth/system.h"
#include "smooth/setup.h"

#ifdef __WIN32__
#include "smooth/input.h"
#include "smooth/droparea.h"
#include "smooth/fontdlg.h"
#include "smooth/graphics/font.h"
#include "smooth/shortcut.h"
#endif

#include "smooth/extended.h"
#include "smooth/graphics/bitmap.h"
#include "smooth/graphics/bitmapbackend.h"

#if defined __WIN32__
#include "smooth/graphics/gdi/bitmapgdi.h"
#include "smooth/graphics/gdi/surfacegdi.h"
#include "smooth/graphics/gdi/windowgdi.h"
#endif

#endif
