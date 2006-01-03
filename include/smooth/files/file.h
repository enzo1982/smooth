 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_FILE_
#define _H_OBJSMOOTH_FILE_

namespace smooth
{
	class File;
};

#include "../definitions.h"
#include "../misc/string.h"
#include "../misc/datetime.h"

namespace smooth
{
	class SMOOTHAPI File
	{
		private:
			String		 fileName;
			String		 filePath;

			Int		 GetFileTime(FILETIME *, FILETIME *, FILETIME *);
		public:
					 File(String, String = NIL);
					 File(const int = NIL);
					 File(const File &);

					~File();

			String		 GetFileName();
			String		 GetFilePath();

			Int64		 GetFileSize();

			DateTime	 GetCreationTime();
			DateTime	 GetWriteTime();
			DateTime	 GetAccessTime();

			Bool		 Exists();

			Int		 Create();

			Int		 Copy(String);
			Int		 Move(String);

			Int		 Delete();
			Int		 Truncate();

			operator	 String() const;
	};
};

#endif
