/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCollapseVerticesByArray.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkThresholdGraph
 * @brief   Returns a subgraph of a vtkGraph.
 *
 *
 * Requires input array, lower and upper threshold. This filter than
 * extracts the subgraph based on these three parameters.
 */

#ifndef vtkThresholdGraph_h
#define vtkThresholdGraph_h

#include "vtkGraphAlgorithm.h"
#include "vtkInfovisCoreModule.h" // For export macro

class VTKINFOVISCORE_EXPORT vtkThresholdGraph : public vtkGraphAlgorithm
{
public:
  static vtkThresholdGraph* New();
  vtkTypeMacro(vtkThresholdGraph, vtkGraphAlgorithm);

  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get/Set lower threshold. This would be the value against which
   * edge or vertex data array value will be compared.
   */
  vtkGetMacro(LowerThreshold, double);
  vtkSetMacro(LowerThreshold, double);
  //@}

  //@{
  /**
   * Get/Set upper threshold. This would be the value against which
   * edge or vertex data array value will be compared.
   */
  vtkGetMacro(UpperThreshold, double);
  vtkSetMacro(UpperThreshold, double);
  //@}

protected:
  vtkThresholdGraph();
  ~vtkThresholdGraph() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  double LowerThreshold;
  double UpperThreshold;

  vtkThresholdGraph(const vtkThresholdGraph&) = delete;
  void operator=(const vtkThresholdGraph&) = delete;
};

#endif // vtkThresholdGraph_h
