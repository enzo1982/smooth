 /* The smooth Class Library
  * Copyright (C) 1998-2022 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_DIRECTORY
#define H_OBJSMOOTH_DIRECTORY

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
			static char			*directoryDelimiter;

			String				 dirName;
			String				 dirPath;

			mutable Array<Directory>	 directories;
			mutable Array<File>		 files;
		public:
							 Directory();
							 Directory(const String &, const String & = NIL);
							 Directory(const Directory &);

							~Directory();

			Directory &operator		 =(const Directory &);

			const String			&GetDirectoryName() const;
			const String			&GetDirectoryPath() const;

			const Array<File>		&GetFiles() const;
			const Array<Directory>		&GetDirectories() const;

			const Array<File>		&GetFilesByPattern(const String &) const;
			const Array<Directory>		&GetDirectoriesByPattern(const String &) const;

			DateTime			 GetCreateTime() const;

			Bool				 Exists() const;

			Int				 Create();

			Int				 Copy(const Directory &);
			Int				 Move(const Directory &);

			Int				 Delete();
			Int				 Empty();

			operator			 String() const;

			static const char		*GetDirectoryDelimiter();
			static const char		*GetUnicodePathPrefix(const String &);

			static String			 MakeExtendedPath(const String &);
			static String			 StripExtendedPathPrefix(const String &);

			static Directory		 GetActiveDirectory();
			static Int			 SetActiveDirectory(const Directory &);
	};
};

#endif
