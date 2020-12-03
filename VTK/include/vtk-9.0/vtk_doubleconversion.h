/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_doubleconversion.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_doubleconversion_h
#define vtk_doubleconversion_h

/* Use the doubleconversion library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkdoubleconversion 0

#if VTK_MODULE_USE_EXTERNAL_vtkdoubleconversion
# define VTK_DOUBLECONVERSION_HEADER(header) <header>
#else
# define VTK_DOUBLECONVERSION_HEADER(header) <vtkdoubleconversion/double-conversion/header>
#endif

#endif
