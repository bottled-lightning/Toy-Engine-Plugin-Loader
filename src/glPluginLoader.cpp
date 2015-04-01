#include "glPluginLoader.h"

// library-loading specific functions
#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h> 
#elif __unix
#   include <dlfcn.h>
#endif



