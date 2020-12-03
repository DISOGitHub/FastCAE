/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOpenGLLabeledContourMapper.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkOpenGLLabeledContourMapper
 *
 * vtkOpenGLLabeledContourMapper is an override for vtkLabeledContourMapper
 * that implements stenciling using the OpenGL2 API.
 */

#ifndef vtkOpenGLLabeledContourMapper_h
#define vtkOpenGLLabeledContourMapper_h

#include "vtkLabeledContourMapper.h"
#include "vtkRenderingOpenGL2Module.h" // For export macro

class vtkMatrix4x4;
class vtkOpenGLHelper;

class VTKRENDERINGOPENGL2_EXPORT vtkOpenGLLabeledContourMapper : public vtkLabeledContourMapper
{
public:
  static vtkOpenGLLabeledContourMapper* New();
  vtkTypeMacro(vtkOpenGLLabeledContourMapper, vtkLabeledContourMapper);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Release graphics resources
   */
  void ReleaseGraphicsResources(vtkWindow* win) override;

protected:
  vtkOpenGLLabeledContourMapper();
  ~vtkOpenGLLabeledContourMapper() override;

  // We override this for compatibility with the OpenGL backend:
  // The old backend pushes actor matrices onto the matrix stack, so the text
  // actors already accounted for any transformations on this mapper's actor.
  // The new backend passes each actor's matrix to the shader individually, and
  // this mapper's actor matrix doesn't affect the label rendering.
  bool CreateLabels(vtkActor* actor) override;

  bool ApplyStencil(vtkRenderer* ren, vtkActor* act) override;
  bool RemoveStencil(vtkRenderer* ren) override;

  vtkOpenGLHelper* StencilBO;
  vtkMatrix4x4* TempMatrix4;

private:
  vtkOpenGLLabeledContourMapper(const vtkOpenGLLabeledContourMapper&) = delete;
  void operator=(const vtkOpenGLLabeledContourMapper&) = delete;
};

#endif
