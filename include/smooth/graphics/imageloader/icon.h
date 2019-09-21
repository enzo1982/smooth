 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_IMAGELOADER_ICON
#define H_OBJSMOOTH_IMAGELOADER_ICON

namespace smooth
{
	namespace GUI
	{
		class ImageLoaderIcon;
	};
};

#include "imageloader.h"

namespace smooth
{
	namespace GUI
	{
		class ImageLoaderIcon : public ImageLoader
		{
			private:
				Bool		 IsAlphaChannelSet() const;

				Void		 SetBackgroundColor(const Color &);
				Void		 SetAlphaChannel(Int);
			public:
						 ImageLoaderIcon(const String &);
						~ImageLoaderIcon();

				const Bitmap	&Load();
		};
	};
};

#endif
