/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPolyVertex.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPolyVertex
 * @brief   cell represents a set of 0D vertices
 *
 * vtkPolyVertex is a concrete implementation of vtkCell to represent a
 * set of 3D vertices.
 */

#ifndef vtkPolyVertex_h
#define vtkPolyVertex_h

#include "vtkCell.h"
#include "vtkCommonDataModelModule.h" // For export macro

class vtkVertex;
class vtkIncrementalPointLocator;

class VTKCOMMONDATAMODEL_EXPORT vtkPolyVertex : public vtkCell
{
public:
  static vtkPolyVertex* New();
  vtkTypeMacro(vtkPolyVertex, vtkCell);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * See the vtkCell API for descriptions of these methods.
   */
  int GetCellType() override { return VTK_POLY_VERTEX; }
  int GetCellDimension() override { return 0; }
  int GetNumberOfEdges() override { return 0; }
  int GetNumberOfFaces() override { return 0; }
  vtkCell* GetEdge(int vtkNotUsed(edgeId)) override { return nullptr; }
  vtkCell* GetFace(int vtkNotUsed(faceId)) override { return nullptr; }
  int CellBoundary(int subId, const double pcoords[3], vtkIdList* pts) override;
  void Contour(double value, vtkDataArray* cellScalars, vtkIncrementalPointLocator* locator,
    vtkCellArray* verts, vtkCellArray* lines, vtkCellArray* polys, vtkPointData* inPd,
    vtkPointData* outPd, vtkCellData* inCd, vtkIdType cellId, vtkCellData* outCd) override;
  void Clip(double value, vtkDataArray* cellScalars, vtkIncrementalPointLocator* locator,
    vtkCellArray* verts, vtkPointData* inPd, vtkPointData* outPd, vtkCellData* inCd,
    vtkIdType cellId, vtkCellData* outCd, int insideOut) override;
  int EvaluatePosition(const double x[3], double closestPoint[3], int& subId, double pcoords[3],
    double& dist2, double weights[]) override;
  void EvaluateLocation(int& subId, const double pcoords[3], double x[3], double* weights) override;
  int IntersectWithLine(const double p1[3], const double p2[3], double tol, double& t, double x[3],
    double pcoords[3], int& subId) override;
  int Triangulate(int index, vtkIdList* ptIds, vtkPoints* pts) override;
  void Derivatives(
    int subId, const double pcoords[3], const double* values, int dim, double* derivs) override;
  int IsPrimaryCell() override { return 0; }
  //@}

  /**
   * Return the center of the point cloud in parametric coordinates.
   */
  int GetParametricCenter(double pcoords[3]) override;

protected:
  vtkPolyVertex();
  ~vtkPolyVertex() override;

  vtkVertex* Vertex;

private:
  vtkPolyVertex(const vtkPolyVertex&) = delete;
  void operator=(const vtkPolyVertex&) = delete;
};

#endif
