/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkToolkits.h.in

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtkToolkits_h
#define vtkToolkits_h

/* This header is configured by VTK's build process.  */

/* The Hybrid and VolumeRendering kits are now switched with Rendering.  */
#ifdef VTK_USE_RENDERING
# define VTK_USE_HYBRID
# define VTK_USE_VOLUMERENDERING
#endif

/* #undef VTK_USE_VOLUMEPRO_1000 */

/*--------------------------------------------------------------------------*/
/* Other Configuration Options                                              */

/* Whether VTK is using vfw32 and if it supports video capture */
#define VTK_USE_VIDEO_FOR_WINDOWS
#define VTK_VFW_SUPPORTS_CAPTURE

/* Whether FFMPEG is found or not  */
/* #undef VTK_USE_FFMPEG_ENCODER */

/* Whether the user has linked in the MPEG2 library or not  */
/* #undef VTK_USE_MPEG2_ENCODER */

/* Whether the user wants to use the Ogg/Theora encoder or not */
/* #undef VTK_USE_OGGTHEORA_ENCODER */

#endif
