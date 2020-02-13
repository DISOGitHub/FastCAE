
#ifndef VTKGUISUPPORTQTWEBKIT_EXPORT_H
#define VTKGUISUPPORTQTWEBKIT_EXPORT_H

#ifdef VTKGUISUPPORTQTWEBKIT_STATIC_DEFINE
#  define VTKGUISUPPORTQTWEBKIT_EXPORT
#  define VTKGUISUPPORTQTWEBKIT_NO_EXPORT
#else
#  ifndef VTKGUISUPPORTQTWEBKIT_EXPORT
#    ifdef vtkGUISupportQtWebkit_EXPORTS
        /* We are building this library */
#      define VTKGUISUPPORTQTWEBKIT_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKGUISUPPORTQTWEBKIT_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKGUISUPPORTQTWEBKIT_NO_EXPORT
#    define VTKGUISUPPORTQTWEBKIT_NO_EXPORT 
#  endif
#endif

#ifndef VTKGUISUPPORTQTWEBKIT_DEPRECATED
#  define VTKGUISUPPORTQTWEBKIT_DEPRECATED __declspec(deprecated)
#  define VTKGUISUPPORTQTWEBKIT_DEPRECATED_EXPORT VTKGUISUPPORTQTWEBKIT_EXPORT __declspec(deprecated)
#  define VTKGUISUPPORTQTWEBKIT_DEPRECATED_NO_EXPORT VTKGUISUPPORTQTWEBKIT_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKGUISUPPORTQTWEBKIT_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkViewsQtModule.h"

#endif
