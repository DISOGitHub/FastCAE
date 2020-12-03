/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_jpeg.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_jpeg_h
#define vtk_jpeg_h

/* Use the jpeg library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkjpeg 0

#if VTK_MODULE_USE_EXTERNAL_vtkjpeg
  /* Work-around for a conflict between windows.h and jpeglib.h in cygwin.
     If ADDRESS_TAG_BIT is defined then BaseTsd.h has been included and
     INT32 has been defined with a typedef, so we must define XMD_H to
     prevent the jpeg header from defining it again.  */
# if defined(__CYGWIN__) && defined(ADDRESS_TAG_BIT) && !defined(XMD_H)
#  define XMD_H
#  define VTK_JPEG_XMD_H
# endif
# include <jpeglib.h>
# include <jerror.h>
#else
# include <vtkjpeg/jconfig.h>
# include <vtkjpeg/jmorecfg.h>
// Ensure jpeglib.h does not include jconfig.h nor jmorecfg.h from another include directory.
# define JCONFIG_INCLUDED
# define JMORECFG_INCLUDED
# include <vtkjpeg/jpeglib.h>
# include <vtkjpeg/jerror.h>
# undef JCONFIG_INCLUDED
# undef JMORECFG_INCLUDED
#endif

#endif
