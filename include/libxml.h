/*
 * libxml.h: internal header only used during the compilation of libxml
 *
 * See COPYRIGHT for the status of this software
 *
 * Author: breese@users.sourceforge.net
 */

#ifndef __XML_LIBXML_H__
#define __XML_LIBXML_H__

#ifdef __WIN32__
#include "win32config.h"
#else
#include <unistd.h>
#include <string.h>
#include <math.h>
#define _snprintf snprintf
#define _vsnprintf vsnprintf
#endif

#include <libxml/parser.h>
#include <libxml/xmlversion.h>

#include <stdio.h>

#endif /* ! __XML_LIBXML_H__ */
