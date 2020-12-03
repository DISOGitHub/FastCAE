// -*- c++ -*-
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMathConfigure.h.in

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*----------------------------------------------------------------------------
 Copyright (c) Sandia Corporation
 See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.
----------------------------------------------------------------------------*/

#ifndef vtkMathConfigure_h
#define vtkMathConfigure_h

#include <cmath>
#include <float.h> // for _isnan, _finite on Win32

#define VTK_HAS_ISNAN
#define VTK_HAS_STD_ISNAN
/* #undef VTK_HAS__ISNAN */
#define VTK_HAS_ISINF
#define VTK_HAS_STD_ISINF
#define VTK_HAS_ISFINITE
#define VTK_HAS_STD_ISFINITE
/* #undef VTK_HAS_FINITE */
/* #undef VTK_HAS__FINITE */

// Check for undetected macro versions of isnan(), isinf(), isfinite()
#if !defined(VTK_HAS_ISNAN) && defined(isnan)
# define VTK_HAS_ISNAN 1
#endif
#if !defined(VTK_HAS_ISINF) && defined(isinf)
# define VTK_HAS_ISINF 1
#endif
#if !defined(VTK_HAS_ISFINITE) && defined(isfinite)
# define VTK_HAS_ISFINITE 1
#endif
#if !defined(VTK_HAS_FINITE) && defined(finite)
# define VTK_HAS_FINITE 1
#endif

// Make macros from _isnan(), _finite() if they exist (there is no _isinf)
#if !defined(VTK_HAS_ISNAN) && (defined(VTK_HAS__ISNAN) || defined(_isnan))
# define isnan(x) _isnan(x)
# define VTK_HAS_ISNAN 1
#endif
#if !defined(VTK_HAS_FINITE) && (defined(VTK_HAS__FINITE) || defined(_finite))
# define finite(x) _finite(x)
# define VTK_HAS_FINITE 1
#endif

// The CUDA compiler(nvcc) is a secondary compiler that is used beside
// your host compiler. While your host compiler can support std::isnan and
// other functions, the CUDA compiler doesn't. Because the CUDA compiler is
// given both host and device code to parse it will then fail when it sees
// std::isnan because it is in the vtkMath header and not the
// implementation file. To get around this issue we check __CUDACC__ which
// is only set when we are compiling with the CUDA compiler
#ifdef __CUDACC__

#if defined(VTK_HAS_STD_ISINF)
#undef VTK_HAS_STD_ISINF
#endif

#if defined(VTK_HAS_STD_ISNAN)
#undef VTK_HAS_STD_ISNAN
#endif

#if defined(VTK_HAS_STD_ISFINITE)
#undef VTK_HAS_STD_ISFINITE
#endif

#endif

#endif //vtkMathConfigure_h
