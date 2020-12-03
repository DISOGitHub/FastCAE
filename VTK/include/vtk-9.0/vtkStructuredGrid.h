/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkStructuredGrid.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkStructuredGrid
 * @brief   topologically regular array of data
 *
 * vtkStructuredGrid is a data object that is a concrete implementation of
 * vtkDataSet. vtkStructuredGrid represents a geometric structure that is a
 * topologically regular array of points. The topology is that of a cube that
 * has been subdivided into a regular array of smaller cubes. Each point/cell
 * can be addressed with i-j-k indices. Examples include finite difference
 * grids.
 *
 * The order and number of points must match that specified by the dimensions
 * of the grid. The point order increases in i fastest (from 0<=i<dims[0]),
 * then j (0<=j<dims[1]), then k (0<=k<dims[2]) where dims[] are the
 * dimensions of the grid in the i-j-k topological directions. The number of
 * points is dims[0]*dims[1]*dims[2]. The same is true for the cells of the
 * grid. The order and number of cells must match that specified by the
 * dimensions of the grid. The cell order increases in i fastest (from
 * 0<=i<(dims[0]-1)), then j (0<=j<(dims[1]-1)), then k (0<=k<(dims[2]-1))
 * The number of cells is (dims[0]-1)*(dims[1]-1)*(dims[2]-1).
 *
 * vtkStructuredGrid has the ability to blank,
 * or "turn-off" points and cells in the dataset. This is done by setting
 * vtkDataSetAttributes::HIDDENPOINT or vtkDataSetAttributes::HIDDENCELL
 * in the ghost array for each point / cell that needs to be blanked.
 */

#ifndef vtkStructuredGrid_h
#define vtkStructuredGrid_h

#include "vtkCommonDataModelModule.h" // For export macro
#include "vtkPointSet.h"

#include "vtkStructuredData.h" // Needed for inline methods

class vtkEmptyCell;
class vtkHexahedron;
class vtkLine;
class vtkQuad;
class vtkUnsignedCharArray;
class vtkVertex;

class VTKCOMMONDATAMODEL_EXPORT vtkStructuredGrid : public vtkPointSet
{
public:
  static vtkStructuredGrid* New();

  vtkTypeMacro(vtkStructuredGrid, vtkPointSet);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Return what type of dataset this is.
   */
  int GetDataObjectType() override { return VTK_STRUCTURED_GRID; }

  /**
   * Copy the geometric and topological structure of an input poly data object.
   */
  void CopyStructure(vtkDataSet* ds) override;

  //@{
  /**
   * Standard vtkDataSet API methods. See vtkDataSet for more information.
   */
  vtkIdType GetNumberOfPoints() override { return vtkPointSet::GetNumberOfPoints(); }
  double* GetPoint(vtkIdType ptId) VTK_SIZEHINT(3) override
  {
    return this->vtkPointSet::GetPoint(ptId);
  }
  void GetPoint(vtkIdType ptId, double p[3]) override { this->vtkPointSet::GetPoint(ptId, p); }
  vtkCell* GetCell(vtkIdType cellId) override;
  vtkCell* GetCell(int i, int j, int k) override;
  void GetCell(vtkIdType cellId, vtkGenericCell* cell) override;
  void GetCellBounds(vtkIdType cellId, double bounds[6]) override;
  int GetCellType(vtkIdType cellId) override;
  vtkIdType GetNumberOfCells() override;
  void GetCellPoints(vtkIdType cellId, vtkIdList* ptIds) override;
  void GetPointCells(vtkIdType ptId, vtkIdList* cellIds) override
  {
    vtkStructuredData::GetPointCells(ptId, cellIds, this->GetDimensions());
  }
  void Initialize() override;
  int GetMaxCellSize() override { return 8; } // hexahedron is the largest
  void GetCellNeighbors(vtkIdType cellId, vtkIdList* ptIds, vtkIdList* cellIds) override;
  void GetCellNeighbors(vtkIdType cellId, vtkIdList* ptIds, vtkIdList* cellIds, int* seedLoc);
  //@}

  //@{
  /**
   * following methods are specific to structured grid
   */
  void SetDimensions(int i, int j, int k);
  void SetDimensions(const int dim[3]);
  //@}

  //@{
  /**
   * Get dimensions of this structured points dataset.
   */
  virtual int* GetDimensions() VTK_SIZEHINT(3);
  virtual void GetDimensions(int dim[3]);
  //@}

  /**
   * Return the dimensionality of the data.
   */
  int GetDataDimension();

  //@{
  /**
   * Different ways to set the extent of the data array.  The extent
   * should be set before the "Scalars" are set or allocated.
   * The Extent is stored in the order (X, Y, Z).
   */
  void SetExtent(int extent[6]);
  void SetExtent(int x1, int x2, int y1, int y2, int z1, int z2);
  vtkGetVector6Macro(Extent, int);
  //@}

  /**
   * Return the actual size of the data in kibibytes (1024 bytes). This number
   * is valid only after the pipeline has updated. The memory size
   * returned is guaranteed to be greater than or equal to the
   * memory required to represent the data (e.g., extra space in
   * arrays, etc. are not included in the return value). THIS METHOD
   * IS THREAD SAFE.
   */
  unsigned long GetActualMemorySize() override;

  //@{
  /**
   * Shallow and Deep copy.
   */
  void ShallowCopy(vtkDataObject* src) override;
  void DeepCopy(vtkDataObject* src) override;
  //@}

  /**
   * The extent type is a 3D extent
   */
  int GetExtentType() override { return VTK_3D_EXTENT; }

  //@{
  /**
   * Methods for supporting blanking of cells. Blanking turns on or off
   * points in the structured grid, and hence the cells connected to them.
   * These methods should be called only after the dimensions of the
   * grid are set.
   */
  void BlankPoint(vtkIdType ptId);
  void UnBlankPoint(vtkIdType ptId);
  //@}

  //@{
  /**
   * Methods for supporting blanking of cells. Blanking turns on or off
   * cells in the structured grid, and hence the points connected to them.
   * These methods should be called only after the dimensions of the
   * grid are set.
   */
  void BlankCell(vtkIdType ptId);
  void UnBlankCell(vtkIdType ptId);
  //@}

  /**
   * Return non-zero value if specified point is visible.
   * These methods should be called only after the dimensions of the
   * grid are set.
   */
  unsigned char IsPointVisible(vtkIdType ptId);

  /**
   * Return non-zero value if specified point is visible.
   * These methods should be called only after the dimensions of the
   * grid are set.
   */
  unsigned char IsCellVisible(vtkIdType cellId);

  /**
   * Returns 1 if there is any visibility constraint on the points,
   * 0 otherwise.
   */
  bool HasAnyBlankPoints() override;
  /**
   * Returns 1 if there is any visibility constraint on the cells,
   * 0 otherwise.
   */
  bool HasAnyBlankCells() override;

  /**
   * Given the node dimensions of this grid instance, this method computes the
   * node dimensions. The value in each dimension can will have a lowest value
   * of "1" such that computing the total number of cells can be achieved by
   * simply by cellDims[0]*cellDims[1]*cellDims[2].
   */
  void GetCellDims(int cellDims[3]);

  /**
   * Reallocates and copies to set the Extent to the UpdateExtent.
   * This is used internally when the exact extent is requested,
   * and the source generated more than the update extent.
   */
  void Crop(const int* updateExtent) override;

  //@{
  /**
   * Retrieve an instance of this class from an information object.
   */
  static vtkStructuredGrid* GetData(vtkInformation* info);
  static vtkStructuredGrid* GetData(vtkInformationVector* v, int i = 0);
  //@}

  /**
   * Get a point in the grid. If adjustForExtent is true, (i,j,k) is
   * interpreted as a position relative to the beginning of the extent.
   * If adjustForExtent is false, (i,j,k) is interpreted literally
   * and the (i,j,k) point of the grid is returned regardless of the
   * extent beginning.
   * The point coordinate is returned in 'p'.
   * The default adjustForExtent is true.
   */
  void GetPoint(int i, int j, int k, double p[3], bool adjustForExtent = true);

protected:
  vtkStructuredGrid();
  ~vtkStructuredGrid() override;

  // for the GetCell method
  vtkVertex* Vertex;
  vtkLine* Line;
  vtkQuad* Quad;
  vtkHexahedron* Hexahedron;
  vtkEmptyCell* EmptyCell;

  int Dimensions[3];
  int DataDescription;

  int Extent[6];

  /**
   * Compute the range of the scalars and cache it into ScalarRange
   * only if the cache became invalid (ScalarRangeComputeTime).
   */
  void ComputeScalarRange() override;

private:
  /**
   * For legacy compatibility. Do not use.
   */
  void GetCellNeighbors(vtkIdType cellId, vtkIdList& ptIds, vtkIdList& cellIds)
  {
    this->GetCellNeighbors(cellId, &ptIds, &cellIds);
  }

  // Internal method used by DeepCopy and ShallowCopy.
  void InternalStructuredGridCopy(vtkStructuredGrid* src);

  static unsigned char MASKED_CELL_VALUE;

private:
  vtkStructuredGrid(const vtkStructuredGrid&) = delete;
  void operator=(const vtkStructuredGrid&) = delete;
};

inline vtkIdType vtkStructuredGrid::GetNumberOfCells()
{
  vtkIdType nCells = 1;
  int dims[3];
  int i;

  this->GetDimensions(dims);
  for (i = 0; i < 3; i++)
  {
    if (dims[i] <= 0)
    {
      return 0;
    }
    if (dims[i] > 1)
    {
      nCells *= (dims[i] - 1);
    }
  }

  return nCells;
}

inline int vtkStructuredGrid::GetDataDimension()
{
  return vtkStructuredData::GetDataDimension(this->DataDescription);
}

#endif
