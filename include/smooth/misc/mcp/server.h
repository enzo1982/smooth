 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_MCP_SERVER
#define H_OBJSMOOTH_MCP_SERVER

namespace smooth
{
	namespace MCP
	{
		class Server;
	};

	namespace Threads
	{
		class Thread;
		class Mutex;
	};
};

#include "json.h"

namespace smooth
{
	namespace MCP
	{
		/* Function run on the GUI thread. Fills in the result and
		 * returns False to report an error with the result text.
		 */
		typedef Bool (*GUIFunction)(const JSON &, JSON &);

		/* MCP server using the stdio transport, enabled with --mcp:stdio.
		 *
		 * Messages are read on a separate thread, while tools access the
		 * GUI on the event processing thread through a job queue.
		 */
		class Server
		{
			private:
				class Job;

				static String			 mode;

				static Threads::Thread		*thread;
				static Threads::Mutex		*mutex;

				static Array<Job *, Void *>	 jobs;

				static Int			 ReadMessages(Threads::Thread *);
				static Void			 HandleMessage(const char *);

				static Void			 SendMessage(const JSON &);
				static Void			 SendError(const JSON &, Int, const String &);

				static Void			 ProcessJobs();
			public:
				/* Set the mode given with --mcp:<mode>.
				 */
				static Void			 SetMode(const String &);

				static Int			 Start();
				static Int			 Free();

				/* Run a function on the GUI thread and wait for it up to
				 * the given time in milliseconds. Returns False if it is
				 * still running when the time is up.
				 */
				static Bool			 RunOnGUIThread(GUIFunction, const JSON &, JSON &, Bool &, Int);
		};
	};
};

#endif
