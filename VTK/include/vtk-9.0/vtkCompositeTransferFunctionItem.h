/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCompositeTransferFunctionItem.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef vtkCompositeTransferFunctionItem_h
#define vtkCompositeTransferFunctionItem_h

#include "vtkChartsCoreModule.h" // For export macro
#include "vtkColorTransferFunctionItem.h"

class vtkPiecewiseFunction;

// Description:
// vtkPlot::Color and vtkPlot::Brush have no effect here.
class VTKCHARTSCORE_EXPORT vtkCompositeTransferFunctionItem : public vtkColorTransferFunctionItem
{
public:
  static vtkCompositeTransferFunctionItem* New();
  vtkTypeMacro(vtkCompositeTransferFunctionItem, vtkColorTransferFunctionItem);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void SetOpacityFunction(vtkPiecewiseFunction* opacity);
  vtkGetObjectMacro(OpacityFunction, vtkPiecewiseFunction);

protected:
  vtkCompositeTransferFunctionItem();
  ~vtkCompositeTransferFunctionItem() override;

  // Description:
  // Reimplemented to return the range of the piecewise function
  void ComputeBounds(double bounds[4]) override;

  void ComputeTexture() override;
  vtkPiecewiseFunction* OpacityFunction;

private:
  vtkCompositeTransferFunctionItem(const vtkCompositeTransferFunctionItem&) = delete;
  void operator=(const vtkCompositeTransferFunctionItem&) = delete;
};

#endif
