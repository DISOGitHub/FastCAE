/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_zlib.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_zlib_h
#define vtk_zlib_h

/* Use the zlib library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkzlib 0

#if VTK_MODULE_USE_EXTERNAL_vtkzlib
# include <zlib.h>
#else
# include <vtkzlib/zlib.h>
#endif

#endif
