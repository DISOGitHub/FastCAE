/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_eigen.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_eigen_h
#define vtk_eigen_h

// VTK should only be using MPL2 Eigen code.
#ifndef EIGEN_MPL2_ONLY
#define EIGEN_MPL2_ONLY
#endif

/* Use the eigen library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkeigen 0

#if VTK_MODULE_USE_EXTERNAL_vtkeigen
# define VTK_EIGEN(x) <Eigen/x>
#else
# define VTK_EIGEN(x) <vtkeigen/eigen/x>
#endif

#endif
