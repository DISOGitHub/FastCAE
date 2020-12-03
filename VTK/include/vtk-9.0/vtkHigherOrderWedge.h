/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkHigherOrderWedge.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkHigherOrderWedge
 * @brief   A 3D cell that represents an arbitrary order HigherOrder wedge
 *
 * vtkHigherOrderWedge is a concrete implementation of vtkCell to represent a
 * 3D wedge using HigherOrder shape functions of user specified order.
 * A wedge consists of two triangular and three quadrilateral faces.
 * The first six points of the wedge (0-5) are the "corner" points
 * where the first three points are the base of the wedge. This wedge
 * point ordering is opposite the vtkWedge ordering though in that
 * the base of the wedge defined by the first three points (0,1,2) form
 * a triangle whose normal points inward (toward the triangular face (3,4,5)).
 * While this is opposite the vtkWedge convention it is consistent with
 * every other cell type in VTK. The first 2 parametric coordinates of the
 * HigherOrder wedge or for the triangular base and vary between 0 and 1. The
 * third parametric coordinate is between the two triangular faces and goes
 * from 0 to 1 as well.
 */

#ifndef vtkHigherOrderWedge_h
#define vtkHigherOrderWedge_h

#include <functional> //For std::function

#include "vtkCellType.h"              // For GetCellType.
#include "vtkCommonDataModelModule.h" // For export macro
#include "vtkNew.h"                   // For member variable.
#include "vtkNonLinearCell.h"
#include "vtkSmartPointer.h" // For member variable.

class vtkCellData;
class vtkDoubleArray;
class vtkWedge;
class vtkIdList;
class vtkPointData;
class vtkPoints;
class vtkVector3d;
class vtkVector3i;
class vtkHigherOrderCurve;
class vtkHigherOrderInterpolation;
class vtkHigherOrderQuadrilateral;
class vtkHigherOrderTriangle;

class VTKCOMMONDATAMODEL_EXPORT vtkHigherOrderWedge : public vtkNonLinearCell
{
public:
  vtkTypeMacro(vtkHigherOrderWedge, vtkNonLinearCell);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  int GetCellType() override = 0;
  int GetCellDimension() override { return 3; }
  int RequiresInitialization() override { return 1; }
  int GetNumberOfEdges() override { return 9; }
  int GetNumberOfFaces() override { return 5; }
  vtkCell* GetEdge(int edgeId) override = 0;
  void SetEdgeIdsAndPoints(int edgeId,
    const std::function<void(const vtkIdType&)>& set_number_of_ids_and_points,
    const std::function<void(const vtkIdType&, const vtkIdType&)>& set_ids_and_points);
  vtkCell* GetFace(int faceId) override = 0;

  void Initialize() override;

  int CellBoundary(int subId, const double pcoords[3], vtkIdList* pts) override;
  int EvaluatePosition(const double x[3], double closestPoint[3], int& subId, double pcoords[3],
    double& dist2, double weights[]) override;
  void EvaluateLocation(int& subId, const double pcoords[3], double x[3], double* weights) override;
  void Contour(double value, vtkDataArray* cellScalars, vtkIncrementalPointLocator* locator,
    vtkCellArray* verts, vtkCellArray* lines, vtkCellArray* polys, vtkPointData* inPd,
    vtkPointData* outPd, vtkCellData* inCd, vtkIdType cellId, vtkCellData* outCd) override;
  void Clip(double value, vtkDataArray* cellScalars, vtkIncrementalPointLocator* locator,
    vtkCellArray* polys, vtkPointData* inPd, vtkPointData* outPd, vtkCellData* inCd,
    vtkIdType cellId, vtkCellData* outCd, int insideOut) override;
  int IntersectWithLine(const double p1[3], const double p2[3], double tol, double& t, double x[3],
    double pcoords[3], int& subId) override;
  int Triangulate(int index, vtkIdList* ptIds, vtkPoints* pts) override;
  void Derivatives(
    int subId, const double pcoords[3], const double* values, int dim, double* derivs) override;
  void SetParametricCoords();
  double* GetParametricCoords() override;
  int GetParametricCenter(double center[3]) override;

  double GetParametricDistance(const double pcoords[3]) override;

  virtual void SetOrderFromCellData(
    vtkCellData* cell_data, const vtkIdType numPts, const vtkIdType cell_id);
  virtual void SetUniformOrderFromNumPoints(const vtkIdType numPts);
  virtual void SetOrder(const int s, const int t, const int u, const vtkIdType numPts);
  virtual const int* GetOrder();
  virtual int GetOrder(int i) { return this->GetOrder()[i]; }

  void InterpolateFunctions(const double pcoords[3], double* weights) override = 0;
  void InterpolateDerivs(const double pcoords[3], double* derivs) override = 0;

  bool SubCellCoordinatesFromId(vtkVector3i& ijk, int subId);
  bool SubCellCoordinatesFromId(int& i, int& j, int& k, int subId);
  static int PointIndexFromIJK(int i, int j, int k, const int* order);
  int PointIndexFromIJK(int i, int j, int k);
  bool TransformApproxToCellParams(int subCell, double* pcoords);
  bool TransformFaceToCellParams(int bdyFace, double* pcoords);

  static int GetNumberOfApproximatingWedges(const int* order);
  int GetNumberOfApproximatingWedges()
  {
    return vtkHigherOrderWedge::GetNumberOfApproximatingWedges(this->GetOrder());
  }
  virtual vtkHigherOrderQuadrilateral* getBdyQuad() = 0;
  virtual vtkHigherOrderTriangle* getBdyTri() = 0;
  virtual vtkHigherOrderCurve* getEdgeCell() = 0;
  virtual vtkHigherOrderInterpolation* getInterp() = 0;

protected:
  vtkHigherOrderWedge();
  ~vtkHigherOrderWedge() override;

  vtkWedge* GetApprox();
  void PrepareApproxData(
    vtkPointData* pd, vtkCellData* cd, vtkIdType cellId, vtkDataArray* cellScalars);
  vtkWedge* GetApproximateWedge(
    int subId, vtkDataArray* scalarsIn = nullptr, vtkDataArray* scalarsOut = nullptr);

  void GetTriangularFace(vtkHigherOrderTriangle* result, int faceId,
    const std::function<void(const vtkIdType&)>& set_number_of_ids_and_points,
    const std::function<void(const vtkIdType&, const vtkIdType&)>& set_ids_and_points);
  void GetQuadrilateralFace(vtkHigherOrderQuadrilateral* result, int faceId,
    const std::function<void(const vtkIdType&)>& set_number_of_ids_and_points,
    const std::function<void(const vtkIdType&, const vtkIdType&)>& set_ids_and_points);

  int Order[4];
  vtkSmartPointer<vtkPoints> PointParametricCoordinates;
  vtkSmartPointer<vtkWedge> Approx;
  vtkSmartPointer<vtkPointData> ApproxPD;
  vtkSmartPointer<vtkCellData> ApproxCD;
  vtkNew<vtkDoubleArray> CellScalars;
  vtkNew<vtkDoubleArray> Scalars;
  vtkNew<vtkPoints> TmpPts;
  vtkNew<vtkIdList> TmpIds;

private:
  vtkHigherOrderWedge(const vtkHigherOrderWedge&) = delete;
  void operator=(const vtkHigherOrderWedge&) = delete;
};

inline int vtkHigherOrderWedge::GetParametricCenter(double center[3])
{
  center[0] = center[1] = 1. / 3.;
  center[2] = 0.5;
  return 0;
}

#endif // vtkHigherOrderWedge_h
