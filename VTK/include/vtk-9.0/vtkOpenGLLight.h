/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkOpenGLLight
 * @brief   OpenGL light
 *
 * vtkOpenGLLight is a concrete implementation of the abstract class vtkLight.
 * vtkOpenGLLight interfaces to the OpenGL rendering library.
 */

#ifndef vtkOpenGLLight_h
#define vtkOpenGLLight_h

#include "vtkLight.h"
#include "vtkRenderingOpenGL2Module.h" // For export macro

class vtkOpenGLRenderer;

class VTKRENDERINGOPENGL2_EXPORT vtkOpenGLLight : public vtkLight
{
public:
  static vtkOpenGLLight* New();
  vtkTypeMacro(vtkOpenGLLight, vtkLight);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Implement base class method.
   */
  void Render(vtkRenderer* ren, int light_index) override;

protected:
  vtkOpenGLLight() {}
  ~vtkOpenGLLight() override {}

private:
  vtkOpenGLLight(const vtkOpenGLLight&) = delete;
  void operator=(const vtkOpenGLLight&) = delete;
};

#endif
