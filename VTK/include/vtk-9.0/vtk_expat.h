/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_expat.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_expat_h
#define vtk_expat_h

/* Use the expat library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkexpat 0

#if VTK_MODULE_USE_EXTERNAL_vtkexpat
# include <expat.h>
#else
/* #undef XML_STATIC */
# include <vtkexpat/lib/expat.h>
#endif

#endif
