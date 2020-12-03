/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtk_glew.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_glew_h
#define vtk_glew_h

/* Use the glew library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_vtkglew 0

#define GLEW_NO_GLU
#define VTK_OPENGL_ES_VERSION 30

/* Mobile device support. */
#define VTK_MODULE_vtkglew_GLES3 0

#ifdef __APPLE__
#define GLES_SILENCE_DEPRECATION
#include "TargetConditionals.h"
#endif

#define VTK_GLEW_SHARED 1
#if !VTK_GLEW_SHARED && !defined(GLEW_STATIC)
#define GLEW_STATIC
#endif

#if VTK_MODULE_vtkglew_GLES3
#  include <GLES3/gl3.h>
#elif TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#   include <OpenGLES/ES3/gl.h>
#elif VTK_MODULE_USE_EXTERNAL_vtkglew
# include <GL/glew.h>
# ifdef _WIN32
#  include <GL/wglew.h>
# endif
#else
# include <vtkglew/include/GL/glew.h>
# ifdef _WIN32
#  include <vtkglew/include/GL/wglew.h>
# endif
#endif

/* some fixes for both ES 2 and 3 */
#ifdef GL_ES_VERSION_3_0
#  define glDrawBuffer(arg)
#  define GL_BACK_LEFT 0
#  define GL_BACK_RIGHT 0
#  define GL_FRONT_LEFT 0
#  define GL_FRONT_RIGHT 0

/* this sends all the data each time as opposed to allowing a subset */
#  define glMultiDrawElements(mode, counts, type, indicies, primcount) \
      for (size_t eCount = 0; eCount < primcount; ++eCount) \
        { \
        glDrawElements(mode, *(counts + eCount), \
          type, (GLvoid *)(indicies[eCount])); \
        }
#endif

/*** deal with some GLES 3.0 specific issues ***/
#ifdef GL_ES_VERSION_3_0
#  define GLEW_ARB_vertex_array_object 1
#  define GLEW_ARB_instanced_arrays 1
#endif

#endif
