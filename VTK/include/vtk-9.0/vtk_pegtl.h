/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_pegtl.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_pegtl_h
#define vtk_pegtl_h

/* Use the pegtl library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkpegtl 0

#if VTK_MODULE_USE_EXTERNAL_vtkpegtl
# define VTK_PEGTL(x) <tao/x>
#else
# include <vtkpegtl/include/tao/pegtl.hpp>
# define VTK_PEGTL(x) <vtkpegtl/include/tao/x>
#endif

#include VTK_PEGTL(pegtl.hpp)

#endif
