
#ifndef VTKGUISUPPORTMFC_EXPORT_H
#define VTKGUISUPPORTMFC_EXPORT_H

#ifdef VTKGUISUPPORTMFC_STATIC_DEFINE
#  define VTKGUISUPPORTMFC_EXPORT
#  define VTKGUISUPPORTMFC_NO_EXPORT
#else
#  ifndef VTKGUISUPPORTMFC_EXPORT
#    ifdef vtkGUISupportMFC_EXPORTS
        /* We are building this library */
#      define VTKGUISUPPORTMFC_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKGUISUPPORTMFC_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKGUISUPPORTMFC_NO_EXPORT
#    define VTKGUISUPPORTMFC_NO_EXPORT 
#  endif
#endif

#ifndef VTKGUISUPPORTMFC_DEPRECATED
#  define VTKGUISUPPORTMFC_DEPRECATED __declspec(deprecated)
#  define VTKGUISUPPORTMFC_DEPRECATED_EXPORT VTKGUISUPPORTMFC_EXPORT __declspec(deprecated)
#  define VTKGUISUPPORTMFC_DEPRECATED_NO_EXPORT VTKGUISUPPORTMFC_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKGUISUPPORTMFC_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkRenderingOpenGL2Module.h"

#endif
