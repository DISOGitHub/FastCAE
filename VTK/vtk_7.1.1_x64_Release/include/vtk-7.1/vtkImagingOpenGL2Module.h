
#ifndef VTKIMAGINGOPENGL2_EXPORT_H
#define VTKIMAGINGOPENGL2_EXPORT_H

#ifdef VTKIMAGINGOPENGL2_STATIC_DEFINE
#  define VTKIMAGINGOPENGL2_EXPORT
#  define VTKIMAGINGOPENGL2_NO_EXPORT
#else
#  ifndef VTKIMAGINGOPENGL2_EXPORT
#    ifdef vtkImagingOpenGL2_EXPORTS
        /* We are building this library */
#      define VTKIMAGINGOPENGL2_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIMAGINGOPENGL2_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIMAGINGOPENGL2_NO_EXPORT
#    define VTKIMAGINGOPENGL2_NO_EXPORT 
#  endif
#endif

#ifndef VTKIMAGINGOPENGL2_DEPRECATED
#  define VTKIMAGINGOPENGL2_DEPRECATED __declspec(deprecated)
#  define VTKIMAGINGOPENGL2_DEPRECATED_EXPORT VTKIMAGINGOPENGL2_EXPORT __declspec(deprecated)
#  define VTKIMAGINGOPENGL2_DEPRECATED_NO_EXPORT VTKIMAGINGOPENGL2_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKIMAGINGOPENGL2_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkRenderingOpenGL2Module.h"

#endif
