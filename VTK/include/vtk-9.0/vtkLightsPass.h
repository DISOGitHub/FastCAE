/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkLightsPass.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkLightsPass
 * @brief   Implement the lights render pass.
 *
 * Render the lights.
 *
 * This pass expects an initialized camera.
 * It disables all the lights, apply transformations for lights following the
 * camera, and turn on the enables lights.
 *
 * @sa
 * vtkRenderPass
 */

#ifndef vtkLightsPass_h
#define vtkLightsPass_h

#include "vtkRenderPass.h"
#include "vtkRenderingOpenGL2Module.h" // For export macro

class vtkOpenGLRenderWindow;

class VTKRENDERINGOPENGL2_EXPORT vtkLightsPass : public vtkRenderPass
{
public:
  static vtkLightsPass* New();
  vtkTypeMacro(vtkLightsPass, vtkRenderPass);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Perform rendering according to a render state \p s.
   * \pre s_exists: s!=0
   */
  void Render(const vtkRenderState* s) override;

protected:
  /**
   * Default constructor.
   */
  vtkLightsPass();

  /**
   * Destructor.
   */
  ~vtkLightsPass() override;

private:
  vtkLightsPass(const vtkLightsPass&) = delete;
  void operator=(const vtkLightsPass&) = delete;
};

#endif
