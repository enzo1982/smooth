#if defined(WIN32) && !defined(__CYGWIN__)
#include "win32config.h"
#else
#define HAVE_STDINT_H 1

#define HAVE_VA_COPY 1
#endif
