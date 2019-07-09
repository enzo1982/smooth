 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_FILE
#define H_OBJSMOOTH_FILE

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
		public:
					 File();
					 File(const String &, const String & = NIL);
					 File(const File &);

					~File();

			const String	&GetFileName() const;
			const String	&GetFilePath() const;

			Int64		 GetFileSize() const;

			DateTime	 GetCreationTime() const;
			DateTime	 GetWriteTime() const;
			DateTime	 GetAccessTime() const;

			Bool		 SetWriteTime(const DateTime &);
			Bool		 SetAccessTime(const DateTime &);

			Bool		 Exists() const;

			Int		 Create();

			Int		 Copy(const File &);
			Int		 Move(const File &);

			Int		 Delete();
			Int		 Truncate();

			operator	 String() const;
	};
};

#endif
