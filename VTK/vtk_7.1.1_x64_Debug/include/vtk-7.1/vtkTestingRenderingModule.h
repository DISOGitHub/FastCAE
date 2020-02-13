
#ifndef VTKTESTINGRENDERING_EXPORT_H
#define VTKTESTINGRENDERING_EXPORT_H

#ifdef VTKTESTINGRENDERING_STATIC_DEFINE
#  define VTKTESTINGRENDERING_EXPORT
#  define VTKTESTINGRENDERING_NO_EXPORT
#else
#  ifndef VTKTESTINGRENDERING_EXPORT
#    ifdef vtkTestingRendering_EXPORTS
        /* We are building this library */
#      define VTKTESTINGRENDERING_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKTESTINGRENDERING_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKTESTINGRENDERING_NO_EXPORT
#    define VTKTESTINGRENDERING_NO_EXPORT 
#  endif
#endif

#ifndef VTKTESTINGRENDERING_DEPRECATED
#  define VTKTESTINGRENDERING_DEPRECATED __declspec(deprecated)
#  define VTKTESTINGRENDERING_DEPRECATED_EXPORT VTKTESTINGRENDERING_EXPORT __declspec(deprecated)
#  define VTKTESTINGRENDERING_DEPRECATED_NO_EXPORT VTKTESTINGRENDERING_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKTESTINGRENDERING_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkRenderingCoreModule.h"

#endif
