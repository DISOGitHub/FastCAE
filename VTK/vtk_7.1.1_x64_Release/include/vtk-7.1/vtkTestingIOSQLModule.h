
#ifndef VTKTESTINGIOSQL_EXPORT_H
#define VTKTESTINGIOSQL_EXPORT_H

#ifdef VTKTESTINGIOSQL_STATIC_DEFINE
#  define VTKTESTINGIOSQL_EXPORT
#  define VTKTESTINGIOSQL_NO_EXPORT
#else
#  ifndef VTKTESTINGIOSQL_EXPORT
#    ifdef vtkTestingIOSQL_EXPORTS
        /* We are building this library */
#      define VTKTESTINGIOSQL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKTESTINGIOSQL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKTESTINGIOSQL_NO_EXPORT
#    define VTKTESTINGIOSQL_NO_EXPORT 
#  endif
#endif

#ifndef VTKTESTINGIOSQL_DEPRECATED
#  define VTKTESTINGIOSQL_DEPRECATED __declspec(deprecated)
#  define VTKTESTINGIOSQL_DEPRECATED_EXPORT VTKTESTINGIOSQL_EXPORT __declspec(deprecated)
#  define VTKTESTINGIOSQL_DEPRECATED_NO_EXPORT VTKTESTINGIOSQL_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKTESTINGIOSQL_NO_DEPRECATED
#endif



#endif
