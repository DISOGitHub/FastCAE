/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_libxml2.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_libxml2_h
#define vtk_libxml2_h

/* Use the libxml2 library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtklibxml2 0

/* #undef LIBXML_STATIC */

/* Macro to help include a header file from the libxml2 configured for
   VTK.  Since libxml2 has many public header files we cannot just
   include them all here.  Instead user code can include this header
   first and then use the macro to construct the name of the desired
   header:

   #include "vtk_libxml2.h"
   #include VTKLIBXML2_HEADER(xmlstring.h)
*/
#if VTK_MODULE_USE_EXTERNAL_vtklibxml2
#define VTKLIBXML2_HEADER(x)  <libxml/x>
#else
#define VTKLIBXML2_HEADER(x)  <vtklibxml2/include/libxml/x>
#endif

#include VTKLIBXML2_HEADER(xmlversion.h)

#endif
