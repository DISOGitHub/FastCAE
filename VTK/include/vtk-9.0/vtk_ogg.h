/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_ogg.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_ogg_h
#define vtk_ogg_h

/* Use the ogg library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkogg 0

#if VTK_MODULE_USE_EXTERNAL_vtkogg
# include <ogg/ogg.h>
#else
# include <vtkogg/include/ogg/ogg.h>
#endif

#endif
