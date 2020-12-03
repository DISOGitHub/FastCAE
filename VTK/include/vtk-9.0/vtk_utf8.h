/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_utf8.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_utf8_h
#define vtk_utf8_h

/* Use the utf8cpp library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkutf8 0

#if VTK_MODULE_USE_EXTERNAL_vtkutf8
# include <utf8.h>
#else
# include <vtkutf8/utf8.h>
namespace utf8 = vtk_utf8;
#endif

#endif
