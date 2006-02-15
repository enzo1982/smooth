 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DIRECTORY_
#define _H_OBJSMOOTH_DIRECTORY_

namespace smooth
{
	class Directory;
};

#include "file.h"

namespace smooth
{
	class SMOOTHAPI Directory
	{
		private:
			String			 dirName;
			String			 dirPath;

			Array<Directory>	 directories;
			Array<File>		 files;
		public:
						 Directory(const String &, const String & = NIL);
						 Directory(const int = NIL);
						 Directory(const Directory &);

						~Directory();

			const String		&GetDirectoryName();
			const String		&GetDirectoryPath();

			const Array<File>	&GetFiles();
			const Array<Directory>	&GetDirectories();

			const Array<File>	&GetFilesByPattern(const String &);

			DateTime		 GetCreateTime();

			Bool			 Exists();

			Int			 Create();

			Int			 Copy(const String &);
			Int			 Move(const String &);

			Int			 Delete();
			Int			 Empty();

			operator		 String() const;

			static Directory	 GetActiveDirectory();
			static Int		 SetActiveDirectory(const Directory &);
	};
};

#endif
