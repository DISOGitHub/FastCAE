
#ifndef VTKWEBGLEXPORTER_EXPORT_H
#define VTKWEBGLEXPORTER_EXPORT_H

#ifdef VTKWEBGLEXPORTER_STATIC_DEFINE
#  define VTKWEBGLEXPORTER_EXPORT
#  define VTKWEBGLEXPORTER_NO_EXPORT
#else
#  ifndef VTKWEBGLEXPORTER_EXPORT
#    ifdef vtkWebGLExporter_EXPORTS
        /* We are building this library */
#      define VTKWEBGLEXPORTER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKWEBGLEXPORTER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKWEBGLEXPORTER_NO_EXPORT
#    define VTKWEBGLEXPORTER_NO_EXPORT 
#  endif
#endif

#ifndef VTKWEBGLEXPORTER_DEPRECATED
#  define VTKWEBGLEXPORTER_DEPRECATED __declspec(deprecated)
#  define VTKWEBGLEXPORTER_DEPRECATED_EXPORT VTKWEBGLEXPORTER_EXPORT __declspec(deprecated)
#  define VTKWEBGLEXPORTER_DEPRECATED_NO_EXPORT VTKWEBGLEXPORTER_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKWEBGLEXPORTER_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkIOExportModule.h"

#endif
