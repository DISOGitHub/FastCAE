/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPPolyDataNormals.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPPolyDataNormals
 * @brief   compute normals for polygonal mesh
 *
 */

#ifndef vtkPPolyDataNormals_h
#define vtkPPolyDataNormals_h

#include "vtkFiltersParallelModule.h" // For export macro
#include "vtkPolyDataNormals.h"

class VTKFILTERSPARALLEL_EXPORT vtkPPolyDataNormals : public vtkPolyDataNormals
{
public:
  vtkTypeMacro(vtkPPolyDataNormals, vtkPolyDataNormals);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  static vtkPPolyDataNormals* New();

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
  vtkPPolyDataNormals();
  ~vtkPPolyDataNormals() override {}

  // Usual data generation method
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  vtkTypeBool PieceInvariant;

private:
  vtkPPolyDataNormals(const vtkPPolyDataNormals&) = delete;
  void operator=(const vtkPPolyDataNormals&) = delete;
};

#endif
