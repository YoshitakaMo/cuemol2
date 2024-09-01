// -*-Mode: C++;-*-
//
// Biomolecule analysis module
//  module loader/common impl
//

#pragma once

#ifdef WIN32

// for MS-Windows
#define DLLEXPORT __declspec(dllexport)

#ifdef MOLCLIENT_EXPORTS
# define MOLCLIENT_API __declspec(dllexport)
#elif defined(LINK_SHARED)
# define MOLCLIENT_API __declspec(dllimport)
#else
# define MOLCLIENT_API
#endif

#elif defined(MB_HAVE_GCC_VIS_ATTR)

// for non-MS platforms (gcc4)
#  ifdef MOLCLIENT_EXPORTS
#    define MOLCLIENT_API __attribute__ ((visibility ("default")))
#  else
#    define MOLCLIENT_API
#  endif

#else

// for non-MS platforms (without visattr)
#define MOLCLIENT_API

#endif // WIN32


namespace molclient {

/// Common Initialization for the molclient library
MOLCLIENT_API bool init();

/// Common Finalization for the molclient library
MOLCLIENT_API void fini();

}
