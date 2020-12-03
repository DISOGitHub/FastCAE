/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_jsoncpp_fwd.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_jsoncpp_fwd_h
#define vtk_jsoncpp_fwd_h

/* Use the jsoncpp library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkjsoncpp 0

#if VTK_MODULE_USE_EXTERNAL_vtkjsoncpp
# include <json/>
#else
# include <vtkjsoncpp/json/json-forwards.h>
#endif

#endif
