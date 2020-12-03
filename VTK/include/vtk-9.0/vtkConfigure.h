/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkConfigure.h.in

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtkConfigure_h
#define vtkConfigure_h

/* This header is configured by VTK's build process.  */

/*--------------------------------------------------------------------------*/
/* Compiler backend                                                         */
/* Be careful modifying this -- order is important.                         */
#if defined(_MSC_VER)
/* MSVC 2015+ can use a clang frontend, so we want to label it only as MSVC
 * and not MSVC and clang. */
#define VTK_COMPILER_MSVC

#elif defined(__INTEL_COMPILER)
/* Intel 14+ on OSX uses a clang frontend, so again we want to label them as
 * intel only, and not intel and clang. */
#define VTK_COMPILER_ICC

#elif defined(__PGI)
/* PGI reports as GNUC as it generates the same ABI, so we need to check for
 * it before gcc. */
#define VTK_COMPILER_PGI

#elif defined(__clang__)
/* Check for clang before GCC, as clang says it is GNUC since it has ABI
 * compliance and supports many of the same extensions. */
#define VTK_COMPILER_CLANG

#elif defined(__GNUC__)
/* Several compilers pretend to be GCC but have minor differences. To
 * compensate for that, we checked for those compilers first above. */
#define VTK_COMPILER_GCC
#define VTK_COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + \
                                  __GNUC_PATCHLEVEL__)
#endif

/*--------------------------------------------------------------------------*/
/* Platform Features                                                        */

/* Byte order.  */
/* All compilers that support Mac OS X define either __BIG_ENDIAN__ or
   __LITTLE_ENDIAN__ to match the endianness of the architecture being
   compiled for. This is not necessarily the same as the architecture of the
   machine doing the building. In order to support Universal Binaries on
   Mac OS X, we prefer those defines to decide the endianness.
   On other platforms we use the result of the TRY_RUN. */
#if !defined(__APPLE__)
/* #undef VTK_WORDS_BIGENDIAN */
#elif defined(__BIG_ENDIAN__)
# define VTK_WORDS_BIGENDIAN
#endif

/* Threading system.  */
/* #undef VTK_USE_PTHREADS */
#define VTK_USE_WIN32_THREADS
# define VTK_MAX_THREADS 64

/* vtkSMPTools back-end */
#define VTK_SMP_Sequential
#define VTK_SMP_BACKEND "Sequential"

/* Whether we require large files support.  */
/* #undef VTK_REQUIRE_LARGE_FILE_SUPPORT */

/*--------------------------------------------------------------------------*/
/* VTK Platform Configuration                                               */

/* Whether the target platform supports shared libraries.  */
/* #undef VTK_TARGET_SUPPORTS_SHARED_LIBS */

/* Whether we are building shared libraries.  */
#define VTK_BUILD_SHARED_LIBS

/* Whether vtkIdType is a 64-bit integer type (or a 32-bit integer type).  */
#define VTK_USE_64BIT_IDS

/* Whether MTime should use a 64-bit integer type on 32 bit builds.  */
/* #undef VTK_USE_64BIT_TIMESTAMPS */

#include "vtkVersionMacros.h" // removed by VTK_LEGACY_REMOVE

/* C++ compiler used.  */
#define VTK_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe"

/* Compatibility settings.  */
/* #undef VTK_LEGACY_REMOVE */
/* #undef VTK_LEGACY_SILENT */

/* Debug leaks support.  */
/* #undef VTK_DEBUG_LEAKS */

/* Debug for-range iterator support.  */
/* #undef VTK_DEBUG_RANGE_ITERATORS */

/* Debug for-range iterator support.  */
/* #undef VTK_ALWAYS_OPTIMIZE_ARRAY_ITERATORS */

/* Print warning on vtkArrayDispatch failure.  */
/* #undef VTK_WARN_ON_DISPATCH_FAILURE */

/* Should all New methods use the object factory override. */
/* #undef VTK_ALL_NEW_OBJECT_FACTORY */

/*--------------------------------------------------------------------------*/
/* Setup VTK based on platform features and configuration.                  */

/* The maximum length of a file name in bytes including the
 * terminating null.
 */
#if defined(PATH_MAX) // Usually defined on Windows
# define VTK_MAXPATH PATH_MAX
#elif defined(MAXPATHLEN)  // Usually defined on linux
# define VTK_MAXPATH MAXPATHLEN
#else
# define VTK_MAXPATH 32767 // Possible with Windows "extended paths"
#endif

/* Legacy macros that are not required as VTK requires a C++11 compiler */
#ifndef VTK_LEGACY_REMOVE
# define VTK_OVERRIDE override
# define VTK_FINAL final
# define VTK_DELETE_FUNCTION =delete
#endif

/** extern template declarations for C++11. Only MinGW32 doesn't support this
 */
#if !defined(__MINGW32__)
#define VTK_USE_EXTERN_TEMPLATE
#endif

#endif // vtkConfigure_h
