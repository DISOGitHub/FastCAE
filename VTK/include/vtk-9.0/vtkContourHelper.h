/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkContourHelper.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkContourHelper
 * @brief   A utility class used by various contour filters
 *
 *  This is a simple utility class that can be used by various contour filters to
 *  produce either triangles and/or polygons based on the outputTriangles parameter
 *  When working with multidimensional dataset, it is needed to process cells
 *  from low to high dimensions.
 * @sa
 * vtkContourGrid vtkCutter vtkContourFilter
 */

#ifndef vtkContourHelper_h
#define vtkContourHelper_h

#include "vtkFiltersCoreModule.h" // For export macro
#include "vtkPolygonBuilder.h"    //for a member variable
#include "vtkSmartPointer.h"      //for a member variable

class vtkIncrementalPointLocator;
class vtkCellArray;
class vtkPointData;
class vtkCellData;
class vtkCell;
class vtkDataArray;
class vtkIdListCollection;

class VTKFILTERSCORE_EXPORT vtkContourHelper
{
public:
  vtkContourHelper(vtkIncrementalPointLocator* locator, vtkCellArray* verts, vtkCellArray* lines,
    vtkCellArray* polys, vtkPointData* inPd, vtkCellData* inCd, vtkPointData* outPd,
    vtkCellData* outCd, int estimatedSize, bool outputTriangles);
  ~vtkContourHelper();
  void Contour(vtkCell* cell, double value, vtkDataArray* cellScalars, vtkIdType cellId);

private:
  vtkContourHelper(const vtkContourHelper&) = delete;
  vtkContourHelper& operator=(const vtkContourHelper&) = delete;

  vtkIncrementalPointLocator* Locator;
  vtkCellArray* Verts;
  vtkCellArray* Lines;
  vtkCellArray* Polys;
  vtkPointData* InPd;
  vtkCellData* InCd;
  vtkPointData* OutPd;
  vtkCellData* OutCd;
  vtkSmartPointer<vtkCellData> TriOutCd;

  vtkCellArray* Tris;
  vtkPolygonBuilder PolyBuilder;
  vtkIdListCollection* PolyCollection;
  bool GenerateTriangles;
};

#endif
// VTK-HeaderTest-Exclude: vtkContourHelper.h
