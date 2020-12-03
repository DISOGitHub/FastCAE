/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPCellDataToPointData.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPCellDataToPointData
 * @brief   Compute point arrays from cell arrays.
 *
 * Like it super class, this filter averages the cell data around
 * a point to get new point data.  This subclass requests a layer of
 * ghost cells to make the results invariant to pieces.  There is a
 * "PieceInvariant" flag that lets the user change the behavior
 * of the filter to that of its superclass.
 */

#ifndef vtkPCellDataToPointData_h
#define vtkPCellDataToPointData_h

#include "vtkCellDataToPointData.h"
#include "vtkFiltersParallelModule.h" // For export macro

class VTKFILTERSPARALLEL_EXPORT vtkPCellDataToPointData : public vtkCellDataToPointData
{
public:
  vtkTypeMacro(vtkPCellDataToPointData, vtkCellDataToPointData);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  static vtkPCellDataToPointData* New();

  //@{
  /**
   * To get piece invariance, this filter has to request an
   * extra ghost level.  By default piece invariance is on.
   */
  vtkSetMacro(PieceInvariant, vtkTypeBool);
  vtkGetMacro(PieceInvariant, vtkTypeBool);
  vtkBooleanMacro(PieceInvariant, vtkTypeBool);
  //@}

protected:
  vtkPCellDataToPointData();
  ~vtkPCellDataToPointData() override {}

  // Usual data generation method
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;
  int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  vtkTypeBool PieceInvariant;

private:
  vtkPCellDataToPointData(const vtkPCellDataToPointData&) = delete;
  void operator=(const vtkPCellDataToPointData&) = delete;
};

#endif
