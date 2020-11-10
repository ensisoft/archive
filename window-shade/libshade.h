

// $Id: libshade.h,v 1.1 2006/12/27 01:19:29 enska Exp $

#ifndef LIBSHADE_H
#define LIBSHADE_H

#include <windows.h>

#ifdef IMPORT_API
#define DLLFUNC __declspec(dllimport)
#else
#define DLLFUNC __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*install_func)();

DLLFUNC void install();
DLLFUNC void uninstall();

#ifdef __cplusplus
}
#endif
#endif

