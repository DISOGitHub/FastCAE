/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPolyDataNormals.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPolyDataNormals
 * @brief   compute normals for polygonal mesh
 *
 * vtkPolyDataNormals is a filter that computes point and/or cell normals
 * for a polygonal mesh. The user specifies if they would like the point
 * and/or cell normals to be computed by setting the ComputeCellNormals
 * and ComputePointNormals flags.
 *
 * The computed normals (a vtkFloatArray) are set to be the active normals
 * (using SetNormals()) of the PointData and/or the CellData (respectively)
 * of the output PolyData. The name of these arrays is "Normals", so they
 * can be retrieved either with
 * vtkArrayDownCast<vtkFloatArray>(output->GetPointData()->GetNormals())
 * or with
 * vtkArrayDownCast<vtkFloatArray>(output->GetPointData()->GetArray("Normals"))
 *
 * The filter can reorder polygons to insure consistent
 * orientation across polygon neighbors. Sharp edges can be split and points
 * duplicated with separate normals to give crisp (rendered) surface definition.
 * It is also possible to globally flip the normal orientation.
 *
 * The algorithm works by determining normals for each polygon and then
 * averaging them at shared points. When sharp edges are present, the edges
 * are split and new points generated to prevent blurry edges (due to
 * Gouraud shading).
 *
 * @warning
 * Normals are computed only for polygons and triangle strips. Normals are
 * not computed for lines or vertices.
 *
 * @warning
 * Triangle strips are broken up into triangle polygons. You may want to
 * restrip the triangles.
 *
 * @sa
 * For high-performance rendering, you could use vtkTriangleMeshPointNormals
 * if you know that you have a triangle mesh which does not require splitting
 * nor consistency check on the cell orientations.
 *
 */

#ifndef vtkPolyDataNormals_h
#define vtkPolyDataNormals_h

#include "vtkFiltersCoreModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class vtkFloatArray;
class vtkIdList;
class vtkPolyData;

class VTKFILTERSCORE_EXPORT vtkPolyDataNormals : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPolyDataNormals, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Construct with feature angle=30, splitting and consistency turned on,
   * flipNormals turned off, and non-manifold traversal turned on.
   * ComputePointNormals is on and ComputeCellNormals is off.
   */
  static vtkPolyDataNormals* New();

  //@{
  /**
   * Specify the angle that defines a sharp edge. If the difference in
   * angle across neighboring polygons is greater than this value, the
   * shared edge is considered "sharp".
   */
  vtkSetClampMacro(FeatureAngle, double, 0.0, 180.0);
  vtkGetMacro(FeatureAngle, double);
  //@}

  //@{
  /**
   * Turn on/off the splitting of sharp edges.
   */
  vtkSetMacro(Splitting, vtkTypeBool);
  vtkGetMacro(Splitting, vtkTypeBool);
  vtkBooleanMacro(Splitting, vtkTypeBool);
  //@}

  //@{
  /**
   * Turn on/off the enforcement of consistent polygon ordering.
   */
  vtkSetMacro(Consistency, vtkTypeBool);
  vtkGetMacro(Consistency, vtkTypeBool);
  vtkBooleanMacro(Consistency, vtkTypeBool);
  //@}

  //@{
  /**
   * Turn on/off the automatic determination of correct normal
   * orientation. NOTE: This assumes a completely closed surface
   * (i.e. no boundary edges) and no non-manifold edges. If these
   * constraints do not hold, all bets are off. This option adds some
   * computational complexity, and is useful if you don't want to have
   * to inspect the rendered image to determine whether to turn on the
   * FlipNormals flag. However, this flag can work with the FlipNormals
   * flag, and if both are set, all the normals in the output will
   * point "inward".
   */
  vtkSetMacro(AutoOrientNormals, vtkTypeBool);
  vtkGetMacro(AutoOrientNormals, vtkTypeBool);
  vtkBooleanMacro(AutoOrientNormals, vtkTypeBool);
  //@}

  //@{
  /**
   * Turn on/off the computation of point normals.
   */
  vtkSetMacro(ComputePointNormals, vtkTypeBool);
  vtkGetMacro(ComputePointNormals, vtkTypeBool);
  vtkBooleanMacro(ComputePointNormals, vtkTypeBool);
  //@}

  //@{
  /**
   * Turn on/off the computation of cell normals.
   */
  vtkSetMacro(ComputeCellNormals, vtkTypeBool);
  vtkGetMacro(ComputeCellNormals, vtkTypeBool);
  vtkBooleanMacro(ComputeCellNormals, vtkTypeBool);
  //@}

  //@{
  /**
   * Turn on/off the global flipping of normal orientation. Flipping
   * reverves the meaning of front and back for Frontface and Backface
   * culling in vtkProperty.  Flipping modifies both the normal
   * direction and the order of a cell's points.
   */
  vtkSetMacro(FlipNormals, vtkTypeBool);
  vtkGetMacro(FlipNormals, vtkTypeBool);
  vtkBooleanMacro(FlipNormals, vtkTypeBool);
  //@}

  //@{
  /**
   * Turn on/off traversal across non-manifold edges. This will prevent
   * problems where the consistency of polygonal ordering is corrupted due
   * to topological loops.
   */
  vtkSetMacro(NonManifoldTraversal, vtkTypeBool);
  vtkGetMacro(NonManifoldTraversal, vtkTypeBool);
  vtkBooleanMacro(NonManifoldTraversal, vtkTypeBool);
  //@}

  //@{
  /**
   * Set/get the desired precision for the output types. See the documentation
   * for the vtkAlgorithm::DesiredOutputPrecision enum for an explanation of
   * the available precision settings.
   */
  vtkSetClampMacro(OutputPointsPrecision, int, SINGLE_PRECISION, DEFAULT_PRECISION);
  vtkGetMacro(OutputPointsPrecision, int);
  //@}

protected:
  vtkPolyDataNormals();
  ~vtkPolyDataNormals() override {}

  // Usual data generation method
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  double FeatureAngle;
  vtkTypeBool Splitting;
  vtkTypeBool Consistency;
  vtkTypeBool FlipNormals;
  vtkTypeBool AutoOrientNormals;
  vtkTypeBool NonManifoldTraversal;
  vtkTypeBool ComputePointNormals;
  vtkTypeBool ComputeCellNormals;
  int NumFlips;
  int OutputPointsPrecision;

private:
  vtkIdList* Wave;
  vtkIdList* Wave2;
  vtkIdList* CellIds;
  vtkIdList* CellPoints;
  vtkIdList* NeighborPoints;
  vtkIdList* Map;
  vtkPolyData* OldMesh;
  vtkPolyData* NewMesh;
  int* Visited;
  vtkFloatArray* PolyNormals;
  double CosAngle;

  // Uses the list of cell ids (this->Wave) to propagate a wave of
  // checked and properly ordered polygons.
  void TraverseAndOrder(void);

  // Check the point id give to see whether it lies on a feature
  // edge. If so, split the point (i.e., duplicate it) to topologically
  // separate the mesh.
  void MarkAndSplit(vtkIdType ptId);

private:
  vtkPolyDataNormals(const vtkPolyDataNormals&) = delete;
  void operator=(const vtkPolyDataNormals&) = delete;
};

#endif
