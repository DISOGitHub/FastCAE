/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_theora.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_libtheora_h
#define vtk_libtheora_h

/* Use the theora library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtktheora 0

#if VTK_MODULE_USE_EXTERNAL_vtktheora
# include <theora/theora.h>
# include <theora/theoradec.h>
# include <theora/theoraenc.h>
#else
# include <vtktheora/include/theora/theora.h>
# include <vtktheora/include/theora/theoradec.h>
# include <vtktheora/include/theora/theoraenc.h>
#endif

#endif
