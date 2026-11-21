 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_MCP_TOOLS
#define H_OBJSMOOTH_MCP_TOOLS

namespace smooth
{
	namespace MCP
	{
		class Tools;
	};
};

#include "json.h"

namespace smooth
{
	namespace MCP
	{
		/* Tools for inspecting and controlling smooth applications.
		 */
		class Tools
		{
			public:
				static String		 GetInstructions();

				static JSON		 GetToolList();

				static Bool		 HasTool(const String &);

				/* Call a tool and return its result. Called on the
				 * MCP thread; tools access the GUI through jobs.
				 */
				static JSON		 CallTool(const String &, const JSON &);
		};
	};
};

#endif
