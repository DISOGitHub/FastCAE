/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_netcdf.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_netcdf_h
#define vtk_netcdf_h

/* Use the netcdf library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtknetcdf 0

#if VTK_MODULE_USE_EXTERNAL_vtknetcdf
# include <netcdf.h>
#else
# include <vtknetcdf/include/netcdf.h>
#endif

#endif
