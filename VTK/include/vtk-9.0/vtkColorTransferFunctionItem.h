/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkColorTransferFunctionItem.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef vtkColorTransferFunctionItem_h
#define vtkColorTransferFunctionItem_h

#include "vtkChartsCoreModule.h" // For export macro
#include "vtkScalarsToColorsItem.h"

class vtkColorTransferFunction;
class vtkImageData;

// Description:
// vtkPlot::Color, vtkPlot::Brush, vtkScalarsToColors::DrawPolyLine,
// vtkScalarsToColors::MaskAboveCurve have no effect here.
class VTKCHARTSCORE_EXPORT vtkColorTransferFunctionItem : public vtkScalarsToColorsItem
{
public:
  static vtkColorTransferFunctionItem* New();
  vtkTypeMacro(vtkColorTransferFunctionItem, vtkScalarsToColorsItem);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void SetColorTransferFunction(vtkColorTransferFunction* t);
  vtkGetObjectMacro(ColorTransferFunction, vtkColorTransferFunction);

protected:
  vtkColorTransferFunctionItem();
  ~vtkColorTransferFunctionItem() override;

  // Description:
  // Reimplemented to return the range of the lookup table
  void ComputeBounds(double bounds[4]) override;

  void ComputeTexture() override;
  vtkColorTransferFunction* ColorTransferFunction;

  /**
   * Override the histogram plotbar configuration
   * in order to set the color transfer function on it
   */
  bool ConfigurePlotBar() override;

private:
  vtkColorTransferFunctionItem(const vtkColorTransferFunctionItem&) = delete;
  void operator=(const vtkColorTransferFunctionItem&) = delete;
};

#endif
