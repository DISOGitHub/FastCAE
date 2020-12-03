/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_libharu.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_libharu_h
#define vtk_libharu_h

/* Use the libharu library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtklibharu 0

#define HPDF_DLL

#if VTK_MODULE_USE_EXTERNAL_vtklibharu
# include <hpdf.h>
#else
# include <vtklibharu/include/hpdf.h>
#endif

#endif // #ifndef vtk_libharu_h
