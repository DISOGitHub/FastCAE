/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInterpolatingSubdivisionFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkInterpolatingSubdivisionFilter
 * @brief   generate a subdivision surface using an Interpolating Scheme
 *
 * vtkInterpolatingSubdivisionFilter is an abstract class that defines
 * the protocol for interpolating subdivision surface filters.
 *
 * @par Thanks:
 * This work was supported by PHS Research Grant No. 1 P41 RR13218-01
 * from the National Center for Research Resources.
 *
 * @sa
 * vtkLinearSubdivisionFilter vtkButterflySubdivisionFilter
 */

#ifndef vtkInterpolatingSubdivisionFilter_h
#define vtkInterpolatingSubdivisionFilter_h

#include "vtkFiltersGeneralModule.h" // For export macro
#include "vtkSubdivisionFilter.h"

class vtkCellArray;
class vtkCellData;
class vtkIdList;
class vtkIntArray;
class vtkPointData;
class vtkPoints;
class vtkPolyData;

class VTKFILTERSGENERAL_EXPORT vtkInterpolatingSubdivisionFilter : public vtkSubdivisionFilter
{
public:
  vtkTypeMacro(vtkInterpolatingSubdivisionFilter, vtkSubdivisionFilter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkInterpolatingSubdivisionFilter();
  ~vtkInterpolatingSubdivisionFilter() override {}

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  virtual int GenerateSubdivisionPoints(
    vtkPolyData* inputDS, vtkIntArray* edgeData, vtkPoints* outputPts, vtkPointData* outputPD) = 0;
  void GenerateSubdivisionCells(
    vtkPolyData* inputDS, vtkIntArray* edgeData, vtkCellArray* outputPolys, vtkCellData* outputCD);
  int FindEdge(vtkPolyData* mesh, vtkIdType cellId, vtkIdType p1, vtkIdType p2,
    vtkIntArray* edgeData, vtkIdList* cellIds);
  vtkIdType InterpolatePosition(
    vtkPoints* inputPts, vtkPoints* outputPts, vtkIdList* stencil, double* weights);

private:
  vtkInterpolatingSubdivisionFilter(const vtkInterpolatingSubdivisionFilter&) = delete;
  void operator=(const vtkInterpolatingSubdivisionFilter&) = delete;
};

#endif
