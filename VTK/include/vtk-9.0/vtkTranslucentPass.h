/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTranslucentPass.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkTranslucentPass
 * @brief   Render the translucent polygonal geometry
 * with property key filtering.
 *
 * vtkTranslucentPass renders the translucent polygonal geometry of all the
 * props that have the keys contained in vtkRenderState.
 *
 * This pass expects an initialized depth buffer and color buffer.
 * Initialized buffers means they have been cleared with farest z-value and
 * background color/gradient/transparent color.
 *
 * @sa
 * vtkRenderPass vtkDefaultPass
 */

#ifndef vtkTranslucentPass_h
#define vtkTranslucentPass_h

#include "vtkDefaultPass.h"
#include "vtkRenderingOpenGL2Module.h" // For export macro

class VTKRENDERINGOPENGL2_EXPORT vtkTranslucentPass : public vtkDefaultPass
{
public:
  static vtkTranslucentPass* New();
  vtkTypeMacro(vtkTranslucentPass, vtkDefaultPass);
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
  vtkTranslucentPass();

  /**
   * Destructor.
   */
  ~vtkTranslucentPass() override;

private:
  vtkTranslucentPass(const vtkTranslucentPass&) = delete;
  void operator=(const vtkTranslucentPass&) = delete;
};

#endif
