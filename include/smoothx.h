 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

#include "smooth/application.h"
#include "smooth/definitions.h"
#include "smooth/i18n.h"
#include "smooth/object.h"
#include "smooth/container.h"
#include "smooth/array.h"
#include "smooth/stk.h"
#include "smooth/window.h"
#include "smooth/titlebar.h"
#include "smooth/statusbar.h"
#include "smooth/menu.h"
#include "smooth/client.h"
#include "smooth/menubar.h"
#include "smooth/popupmenu.h"
#include "smooth/divisionbar.h"
#include "smooth/layer.h"
#include "smooth/button.h"
#include "smooth/checkbox.h"
#include "smooth/optionbox.h"
#include "smooth/groupbox.h"
#include "smooth/text.h"
#include "smooth/activearea.h"
#include "smooth/hyperlink.h"
#include "smooth/bitmap.h"
#include "smooth/arrows.h"
#include "smooth/tabregister.h"
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
#include "smooth/loop.h"
#include "smooth/version.h"
#include "smooth/mdiclient.h"
#include "smooth/mdiwindow.h"
#include "smooth/thread.h"
#include "smooth/toolwindow.h"
#include "smooth/mutex.h"
#include "smooth/semaphore.h"
#include "smooth/timer.h"
#include "smooth/string.h"
#include "smooth/stream.h"
#include "smooth/tooltip.h"
#include "smooth/point.h"
#include "smooth/size.h"
#include "smooth/rect.h"
#include "smooth/line.h"
#include "smooth/drag.h"
#include "smooth/date.h"
#include "smooth/time.h"
#include "smooth/xml/attribute.h"
#include "smooth/xml/node.h"
#include "smooth/xml/document.h"
#include "smooth/types/bool.h"
#include "smooth/types/byte.h"
#include "smooth/types/float.h"
#include "smooth/types/int.h"
#include "smooth/types/short.h"
#include "smooth/types/void.h"
#include "smooth/surface.h"
#include "smooth/surfacegdi.h"
#include "smooth/filedlg.h"
#include "smooth/dirdlg.h"
#include "smooth/colordlg.h"

#endif
