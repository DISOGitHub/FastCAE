
#ifndef VTKRENDERINGOPENVR_EXPORT_H
#define VTKRENDERINGOPENVR_EXPORT_H

#ifdef VTKRENDERINGOPENVR_STATIC_DEFINE
#  define VTKRENDERINGOPENVR_EXPORT
#  define VTKRENDERINGOPENVR_NO_EXPORT
#else
#  ifndef VTKRENDERINGOPENVR_EXPORT
#    ifdef vtkRenderingOpenVR_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGOPENVR_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKRENDERINGOPENVR_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKRENDERINGOPENVR_NO_EXPORT
#    define VTKRENDERINGOPENVR_NO_EXPORT 
#  endif
#endif

#ifndef VTKRENDERINGOPENVR_DEPRECATED
#  define VTKRENDERINGOPENVR_DEPRECATED __declspec(deprecated)
#  define VTKRENDERINGOPENVR_DEPRECATED_EXPORT VTKRENDERINGOPENVR_EXPORT __declspec(deprecated)
#  define VTKRENDERINGOPENVR_DEPRECATED_NO_EXPORT VTKRENDERINGOPENVR_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKRENDERINGOPENVR_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkInteractionStyleModule.h"
#include "vtkRenderingOpenGL2Module.h"
#include "vtkRenderingOpenGL2Module.h"

/* AutoInit implementations.  */
#if defined(vtkRenderingOpenVR_INCLUDE)
# include vtkRenderingOpenVR_INCLUDE
#endif
#if defined(vtkRenderingOpenVR_AUTOINIT)
# include "vtkAutoInit.h"
VTK_AUTOINIT(vtkRenderingOpenVR)
#endif

#endif
