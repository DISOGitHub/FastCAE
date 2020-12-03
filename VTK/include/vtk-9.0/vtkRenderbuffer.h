/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRenderbuffer.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkRenderbuffer
 * @brief   Storage for FBO's
 *
 * Lightweight API to OpenGL Framebuffer Object EXT renderbuffers.
 */

#ifndef vtkRenderbuffer_h
#define vtkRenderbuffer_h

#include "vtkObject.h"
#include "vtkRenderingOpenGL2Module.h" // for export macro
#include "vtkWeakPointer.h"            // for render context

class vtkRenderWindow;
class vtkWindow;

class VTKRENDERINGOPENGL2_EXPORT vtkRenderbuffer : public vtkObject
{
public:
  static vtkRenderbuffer* New();
  vtkTypeMacro(vtkRenderbuffer, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Returns if the context supports the required extensions.
   * Extension will be loaded when the context is set.
   */
  static bool IsSupported(vtkRenderWindow* renWin);

  //@{
  /**
   * Get the name of the buffer for use opengl code.
   */
  vtkGetMacro(Handle, unsigned int);
  //@}

  //@{
  /**
   * Setting the context has the side affect of initializing OpenGL
   * required extensions and allocates an OpenGL name(handle) that is
   * released when the object is destroyed. NOTE: the reference count
   * to the passed in object is not incremented. Contex must be set
   * prior to other use.
   */
  void SetContext(vtkRenderWindow* win);
  vtkRenderWindow* GetContext();
  //@}

  /**
   * Sets up an RGBAF renderbufffer for use as a color attachment. Use mode
   * to control READ or DRAW operation.
   */
  int CreateColorAttachment(unsigned int width, unsigned int height);

  /**
   * Sets up an DEPTH renderbufffer for use as a color attachment. Use mode
   * to control READ or DRAW operation.
   */
  int CreateDepthAttachment(unsigned int width, unsigned int height);

  /**
   * Sets up an renderbufffer. Use mode to control READ or DRAW operation and
   * format to control the internal format. (see OpenGL doc for more info)
   */
  int Create(unsigned int format, unsigned int width, unsigned int height);
  int Create(unsigned int format, unsigned int width, unsigned int height, unsigned int samples);

  void ReleaseGraphicsResources(vtkWindow* win);

  // resizes an existing renderbuffer
  void Resize(unsigned int width, unsigned int height);

  //@{
  /**
   * Get the buffer dimensions.
   * These are the properties of the OpenGL renderbuffer this instance represents.
   */
  vtkGetMacro(Width, unsigned int);
  vtkGetMacro(Height, unsigned int);
  vtkGetMacro(Samples, unsigned int);
  //@}

protected:
  vtkRenderbuffer();
  ~vtkRenderbuffer() override;

  bool LoadRequiredExtensions(vtkRenderWindow* renWin);
  void Alloc();
  void Free();

  int DepthBufferFloat;

  unsigned int Width;
  unsigned int Height;
  unsigned int Format;
  unsigned int Samples;

private:
  unsigned int Handle;
  vtkWeakPointer<vtkRenderWindow> Context;

private:
  vtkRenderbuffer(const vtkRenderbuffer&) = delete;
  void operator=(const vtkRenderbuffer&) = delete;
};

#endif
