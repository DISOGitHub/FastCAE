/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_libproj.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_libproj_h
#define vtk_libproj_h

/* Use the libproj library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtklibproj 0

#define VTK_LibPROJ_MAJOR_VERSION 4

#if VTK_LibPROJ_MAJOR_VERSION < 5
# define PROJ_VERSION_MAJOR 4
# define proj_list_operations pj_get_list_ref
#endif

#if VTK_MODULE_USE_EXTERNAL_vtklibproj
# if VTK_LibPROJ_MAJOR_VERSION >= 5
#  include <proj.h>
# endif
# if VTK_LibPROJ_MAJOR_VERSION < 6
#  include <projects.h>
# endif
# if VTK_LibPROJ_MAJOR_VERSION >= 6
#  define ACCEPT_USE_OF_DEPRECATED_PROJ_API_H 1
# endif
# include <proj_api.h>
# include <geodesic.h>
#else
# include <vtklibproj/src/projects.h>
# include <vtklibproj/src/proj_api.h>
# include <vtklibproj/src/geodesic.h>
#endif

#endif
