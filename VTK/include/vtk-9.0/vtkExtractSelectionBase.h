/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkExtractSelectionBase.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkExtractSelectionBase
 * @brief   abstract base class for all extract selection
 * filters.
 *
 * vtkExtractSelectionBase is an abstract base class for all extract selection
 * filters. It defines some properties common to all extract selection filters.
 */

#ifndef vtkExtractSelectionBase_h
#define vtkExtractSelectionBase_h

#include "vtkDataObjectAlgorithm.h"
#include "vtkFiltersGeneralModule.h" // For export macro

class VTKFILTERSGENERAL_EXPORT vtkExtractSelectionBase : public vtkDataObjectAlgorithm
{
public:
  vtkTypeMacro(vtkExtractSelectionBase, vtkDataObjectAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Convenience method to specify the selection connection (2nd input
   * port)
   */
  void SetSelectionConnection(vtkAlgorithmOutput* algOutput)
  {
    this->SetInputConnection(1, algOutput);
  }

  //@{
  /**
   * This flag tells the extraction filter not to convert the selected
   * output into an unstructured grid, but instead to produce a vtkInsidedness
   * array and add it to the input dataset. Default value is false(0).
   */
  vtkSetMacro(PreserveTopology, vtkTypeBool);
  vtkGetMacro(PreserveTopology, vtkTypeBool);
  vtkBooleanMacro(PreserveTopology, vtkTypeBool);
  //@}

protected:
  vtkExtractSelectionBase();
  ~vtkExtractSelectionBase() override;

  /**
   * Sets up empty output dataset
   */
  int RequestDataObject(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  int FillInputPortInformation(int port, vtkInformation* info) override;

  vtkTypeBool PreserveTopology;

private:
  vtkExtractSelectionBase(const vtkExtractSelectionBase&) = delete;
  void operator=(const vtkExtractSelectionBase&) = delete;
};

#endif
