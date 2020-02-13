
#ifndef VTKPYTHONINTERPRETER_EXPORT_H
#define VTKPYTHONINTERPRETER_EXPORT_H

#ifdef VTKPYTHONINTERPRETER_STATIC_DEFINE
#  define VTKPYTHONINTERPRETER_EXPORT
#  define VTKPYTHONINTERPRETER_NO_EXPORT
#else
#  ifndef VTKPYTHONINTERPRETER_EXPORT
#    ifdef vtkPythonInterpreter_EXPORTS
        /* We are building this library */
#      define VTKPYTHONINTERPRETER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKPYTHONINTERPRETER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKPYTHONINTERPRETER_NO_EXPORT
#    define VTKPYTHONINTERPRETER_NO_EXPORT 
#  endif
#endif

#ifndef VTKPYTHONINTERPRETER_DEPRECATED
#  define VTKPYTHONINTERPRETER_DEPRECATED __declspec(deprecated)
#  define VTKPYTHONINTERPRETER_DEPRECATED_EXPORT VTKPYTHONINTERPRETER_EXPORT __declspec(deprecated)
#  define VTKPYTHONINTERPRETER_DEPRECATED_NO_EXPORT VTKPYTHONINTERPRETER_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKPYTHONINTERPRETER_NO_DEPRECATED
#endif



#endif
