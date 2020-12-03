/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPointSet.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPointSet
 * @brief   abstract class for specifying dataset behavior
 *
 * vtkPointSet is an abstract class that specifies the interface for
 * datasets that explicitly use "point" arrays to represent geometry.
 * For example, vtkPolyData, vtkUnstructuredGrid, and vtkStructuredGrid
 * require point arrays to specify point positions, while vtkImageData
 * represents point positions implicitly (and hence is not a subclass
 * of vtkImageData).
 *
 * Note: The vtkPolyData and vtkUnstructuredGrid datasets (derived classes of
 * vtkPointSet) are often used in geometric computation (e.g.,
 * vtkDelaunay2D).  In most cases during filter execution the output geometry
 * and/or topology is created once and provided as output; however in a very
 * few cases the underlying geometry/topology may be created and then
 * incrementally modified. This has implications on the use of supporting
 * classes like locators and cell links topological structures which may be
 * required to support incremental editing operations. Consequently, there is
 * a flag, Editable, that controls whether the dataset can be incrementally
 * edited after it is initially created. By default, and for performance
 * reasons, vtkPointSet derived classes are created as non-editable.  The few
 * methods that require incremental editing capabilities are documented in
 * derived classes.
 *
 * Another important feature of vtkPointSet classes is the use of an internal
 * locator to speed up certain operations like FindCell(). Depending on the
 * application and desired performance, different locators (either a cell or
 * point locator) of different locator types may be used, along with different
 * strategies for using the locators to perform various operations. See
 * the class vtkFindCellStrategy for more information
 *
 * @sa
 * vtkPolyData vtkStructuredGrid vtkUnstructuredGrid vtkFindCellStrategy
 */

#ifndef vtkPointSet_h
#define vtkPointSet_h

#include "vtkCommonDataModelModule.h" // For export macro
#include "vtkDataSet.h"

#include "vtkPoints.h" // Needed for inline methods

class vtkAbstractPointLocator;
class vtkAbstractCellLocator;

class VTKCOMMONDATAMODEL_EXPORT vtkPointSet : public vtkDataSet
{
public:
  //@{
  /**
   * Standard methdos for type information and printing.
   */
  vtkTypeMacro(vtkPointSet, vtkDataSet);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Specify whether this dataset is editable after creation. Meaning, once
   * the points and cells are defined, can the dataset be incrementally
   * modified. By default, this dataset is non-editable (i.e., "static")
   * after construction. The reason for this is performance: cell links and
   * locators can be built (and destroyed) much faster is it is known that
   * the data is static (see vtkStaticCellLinks, vtkStaticPointLocator,
   * vtkStaticCellLocator).
   */
  vtkSetMacro(Editable, bool);
  vtkGetMacro(Editable, bool);
  vtkBooleanMacro(Editable, bool);
  //@}

  /**
   * Reset to an empty state and free any memory.
   */
  void Initialize() override;

  /**
   * Copy the geometric structure of an input point set object.
   */
  void CopyStructure(vtkDataSet* pd) override;

  //@{
  /**
   * See vtkDataSet for additional information.
   */
  vtkIdType GetNumberOfPoints() override;
  void GetPoint(vtkIdType ptId, double x[3]) override { this->Points->GetPoint(ptId, x); }
  vtkIdType FindPoint(double x[3]) override;
  vtkIdType FindPoint(double x, double y, double z) { return this->vtkDataSet::FindPoint(x, y, z); }
  vtkIdType FindCell(double x[3], vtkCell* cell, vtkIdType cellId, double tol2, int& subId,
    double pcoords[3], double* weights) override;
  vtkIdType FindCell(double x[3], vtkCell* cell, vtkGenericCell* gencell, vtkIdType cellId,
    double tol2, int& subId, double pcoords[3], double* weights) override;
  //@}

  /**
   * See vtkDataSet for additional information.
   * WARNING: Just don't use this error-prone method, the returned pointer
   * and its values are only valid as long as another method invocation is not
   * performed. Prefer GetPoint() with the return value in argument.
   */
  double* GetPoint(vtkIdType ptId) VTK_SIZEHINT(3) override { return this->Points->GetPoint(ptId); }

  /**
   * Return an iterator that traverses the cells in this data set.
   */
  vtkCellIterator* NewCellIterator() override;

  //@{
  /**
   * Build the internal point locator . In a multi-threaded environment, call
   * this method in a single thread before using FindCell() or FindPoint().
   */
  void BuildPointLocator();
  void BuildLocator() { this->BuildPointLocator(); }
  //@}

  /**
   * Build the cell locator. In a multi-threaded environment,
   * call this method in a single thread before using FindCell().
   */
  void BuildCellLocator();

  //@{
  /**
   * Set / get an instance of vtkAbstractPointLocator which is used to
   * support the FindPoint() and FindCell() methods. By default a
   * vtkStaticPointLocator is used, unless the class is set as Editable, in
   * which case a vtkPointLocator is used.
   */
  virtual void SetPointLocator(vtkAbstractPointLocator*);
  vtkGetObjectMacro(PointLocator, vtkAbstractPointLocator);
  //@}

  //@{
  /**
   * Set / get an instance of vtkAbstractCellLocator which may be used
   * when a vtkCellLocatorStrategy is used during a FindCelloperation.
   */
  virtual void SetCellLocator(vtkAbstractCellLocator*);
  vtkGetObjectMacro(CellLocator, vtkAbstractCellLocator);
  //@}

  /**
   * Get MTime which also considers its vtkPoints MTime.
   */
  vtkMTimeType GetMTime() override;

  /**
   * Compute the (X, Y, Z)  bounds of the data.
   */
  void ComputeBounds() override;

  /**
   * Reclaim any unused memory.
   */
  void Squeeze() override;

  //@{
  /**
   * Specify point array to define point coordinates.
   */
  virtual void SetPoints(vtkPoints*);
  vtkGetObjectMacro(Points, vtkPoints);
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

  //@{
  /**
   * Overwritten to handle the data/locator loop
   */
  void Register(vtkObjectBase* o) override;
  void UnRegister(vtkObjectBase* o) override;
  //@}

  //@{
  /**
   * Retrieve an instance of this class from an information object.
   */
  static vtkPointSet* GetData(vtkInformation* info);
  static vtkPointSet* GetData(vtkInformationVector* v, int i = 0);
  //@}

protected:
  vtkPointSet();
  ~vtkPointSet() override;

  bool Editable;
  vtkPoints* Points;
  vtkAbstractPointLocator* PointLocator;
  vtkAbstractCellLocator* CellLocator;

  void ReportReferences(vtkGarbageCollector*) override;

private:
  void Cleanup();

  vtkPointSet(const vtkPointSet&) = delete;
  void operator=(const vtkPointSet&) = delete;
};

inline vtkIdType vtkPointSet::GetNumberOfPoints()
{
  if (this->Points)
  {
    return this->Points->GetNumberOfPoints();
  }
  else
  {
    return 0;
  }
}

#endif
