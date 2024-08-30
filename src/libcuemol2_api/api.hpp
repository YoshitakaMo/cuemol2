#pragma once

#ifdef WIN32
// for Windows
#  ifdef LIBCUEMOL_EXPORTS
#    define LIBCUEMOL_API __declspec(dllexport)
#  elif defined(LINK_SHARED)
#    define LIBCUEMOL_API __declspec(dllimport)
#  else
#    define LIBCUEMOL_API
#  endif
#else
// other platforms
#  define LIBCUEMOL_API
#endif

#include <common.h>

