/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkFreeTypeLabelRenderStrategy.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkFreeTypeLabelRenderStrategy
 * @brief   Renders labels with freetype
 *
 *
 * Uses the FreeType to render labels and compute label sizes.
 * This strategy may be used with vtkLabelPlacementMapper.
 */

#ifndef vtkFreeTypeLabelRenderStrategy_h
#define vtkFreeTypeLabelRenderStrategy_h

#include "vtkLabelRenderStrategy.h"
#include "vtkRenderingLabelModule.h" // For export macro

class vtkActor2D;
class vtkTextRenderer;
class vtkTextMapper;

class VTKRENDERINGLABEL_EXPORT vtkFreeTypeLabelRenderStrategy : public vtkLabelRenderStrategy
{
public:
  void PrintSelf(ostream& os, vtkIndent indent) override;
  vtkTypeMacro(vtkFreeTypeLabelRenderStrategy, vtkLabelRenderStrategy);
  static vtkFreeTypeLabelRenderStrategy* New();

  /**
   * The free type render strategy currently does not support rotation.
   */
  bool SupportsRotation() override { return false; }

  /**
   * The free type render strategy currently does not support bounded size labels.
   */
  bool SupportsBoundedSize() override { return false; }

  /**
   * Compute the bounds of a label. Must be performed after the renderer is set.
   */
  void ComputeLabelBounds(vtkTextProperty* tprop, vtkStdString label, double bds[4]) override
  {
    this->Superclass::ComputeLabelBounds(tprop, label, bds);
  }
  void ComputeLabelBounds(vtkTextProperty* tprop, vtkUnicodeString label, double bds[4]) override;

  /**
   * Render a label at a location in world coordinates.
   * Must be performed between StartFrame() and EndFrame() calls.
   */
  void RenderLabel(int x[2], vtkTextProperty* tprop, vtkStdString label) override
  {
    this->Superclass::RenderLabel(x, tprop, label);
  }
  void RenderLabel(int x[2], vtkTextProperty* tprop, vtkStdString label, int width) override
  {
    this->Superclass::RenderLabel(x, tprop, label, width);
  }
  void RenderLabel(int x[2], vtkTextProperty* tprop, vtkUnicodeString label) override;
  void RenderLabel(int x[2], vtkTextProperty* tprop, vtkUnicodeString label, int width) override
  {
    this->Superclass::RenderLabel(x, tprop, label, width);
  }

  /**
   * Release any graphics resources that are being consumed by this strategy.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  void ReleaseGraphicsResources(vtkWindow* window) override;

protected:
  vtkFreeTypeLabelRenderStrategy();
  ~vtkFreeTypeLabelRenderStrategy() override;

  vtkTextRenderer* TextRenderer;
  vtkTextMapper* Mapper;
  vtkActor2D* Actor;

private:
  vtkFreeTypeLabelRenderStrategy(const vtkFreeTypeLabelRenderStrategy&) = delete;
  void operator=(const vtkFreeTypeLabelRenderStrategy&) = delete;
};

#endif
