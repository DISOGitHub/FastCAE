
#ifndef VTKRENDERINGLICOPENGL2_EXPORT_H
#define VTKRENDERINGLICOPENGL2_EXPORT_H

#ifdef VTKRENDERINGLICOPENGL2_STATIC_DEFINE
#  define VTKRENDERINGLICOPENGL2_EXPORT
#  define VTKRENDERINGLICOPENGL2_NO_EXPORT
#else
#  ifndef VTKRENDERINGLICOPENGL2_EXPORT
#    ifdef vtkRenderingLICOpenGL2_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGLICOPENGL2_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKRENDERINGLICOPENGL2_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKRENDERINGLICOPENGL2_NO_EXPORT
#    define VTKRENDERINGLICOPENGL2_NO_EXPORT 
#  endif
#endif

#ifndef VTKRENDERINGLICOPENGL2_DEPRECATED
#  define VTKRENDERINGLICOPENGL2_DEPRECATED __declspec(deprecated)
#  define VTKRENDERINGLICOPENGL2_DEPRECATED_EXPORT VTKRENDERINGLICOPENGL2_EXPORT __declspec(deprecated)
#  define VTKRENDERINGLICOPENGL2_DEPRECATED_NO_EXPORT VTKRENDERINGLICOPENGL2_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKRENDERINGLICOPENGL2_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkRenderingOpenGL2Module.h"

/* AutoInit implementations.  */
#if defined(vtkRenderingLICOpenGL2_INCLUDE)
# include vtkRenderingLICOpenGL2_INCLUDE
#endif
#if defined(vtkRenderingLICOpenGL2_AUTOINIT)
# include "vtkAutoInit.h"
VTK_AUTOINIT(vtkRenderingLICOpenGL2)
#endif

#endif
