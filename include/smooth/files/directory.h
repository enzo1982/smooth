 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
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
						 Directory(String, String = NIL);
						 Directory(const int);
						 Directory(const Directory &);

						~Directory();

			String			 GetDirectoryName();
			String			 GetDirectoryPath();

			Array<File>		&GetFiles();
			Array<Directory>	&GetDirectories();

			DateTime		 GetCreateTime();

			Bool			 Exists();

			Int			 Create();

			Int			 Copy(String);
			Int			 Move(String);

			Int			 Delete();
			Int			 Empty();

			operator		 String() const;

			static Directory	 GetActiveDirectory();
			static Int		 SetActiveDirectory(const Directory &);
	};
};

#endif
