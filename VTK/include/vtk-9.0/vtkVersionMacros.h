/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVersionMacros.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtkVersionMacros_h
#define vtkVersionMacros_h

/* Note: this file is deliberately both valid C and valid C++. */

#define VTK_MAJOR_VERSION 9
#define VTK_MINOR_VERSION 0
#define VTK_BUILD_VERSION 1
#define VTK_VERSION "9.0.1"

#define VTK_SOURCE_VERSION "vtk version " VTK_VERSION

#define VTK_VERSION_CHECK(major, minor, build)                                                     \
  (10000000000ULL * major + 100000000ULL * minor + build)
#define VTK_VERSION_NUMBER                                                                         \
  VTK_VERSION_CHECK(VTK_MAJOR_VERSION, VTK_MINOR_VERSION, VTK_BUILD_VERSION)

#endif
