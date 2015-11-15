/* This file is needed to compile our custom libxml2 on Linux and FreeBSD.
 */

#define HAVE_STDINT_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_CTYPE_H 1
#define HAVE_STDLIB_H 1
#define HAVE_ERRNO_H 1
#define HAVE_ICONV_H 1

#define ICONV_CONST const

#define HAVE_VA_COPY 1
