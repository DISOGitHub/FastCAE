
#ifndef VTKGUISUPPORTQTOPENGL_EXPORT_H
#define VTKGUISUPPORTQTOPENGL_EXPORT_H

#ifdef VTKGUISUPPORTQTOPENGL_STATIC_DEFINE
#  define VTKGUISUPPORTQTOPENGL_EXPORT
#  define VTKGUISUPPORTQTOPENGL_NO_EXPORT
#else
#  ifndef VTKGUISUPPORTQTOPENGL_EXPORT
#    ifdef vtkGUISupportQtOpenGL_EXPORTS
        /* We are building this library */
#      define VTKGUISUPPORTQTOPENGL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKGUISUPPORTQTOPENGL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKGUISUPPORTQTOPENGL_NO_EXPORT
#    define VTKGUISUPPORTQTOPENGL_NO_EXPORT 
#  endif
#endif

#ifndef VTKGUISUPPORTQTOPENGL_DEPRECATED
#  define VTKGUISUPPORTQTOPENGL_DEPRECATED __declspec(deprecated)
#  define VTKGUISUPPORTQTOPENGL_DEPRECATED_EXPORT VTKGUISUPPORTQTOPENGL_EXPORT __declspec(deprecated)
#  define VTKGUISUPPORTQTOPENGL_DEPRECATED_NO_EXPORT VTKGUISUPPORTQTOPENGL_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKGUISUPPORTQTOPENGL_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkRenderingOpenGL2Module.h"

#endif
