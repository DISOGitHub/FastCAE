/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkHyperTreeGridOrientedGeometryCursor.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright Nonice for more information.

=========================================================================*/
/**
 * @class   vtkHyperTreeGridOrientedGeometryCursor
 * @brief   Objects for traversal a HyperTreeGrid.
 *
 * JB A REVOIR
 * NonOriented ne peut pas remonter plus haut qu'a sa creation.
 * Objects that can perform depth traversal of a hyper tree grid,
 * take into account more parameters (related to the grid structure) than
 * the compact hyper tree cursor implemented in vtkHyperTree can.
 * This is an abstract class.
 * Cursors are created by the HyperTreeGrid implementation.
 *
 * @sa
 * vtkHyperTreeCursor vtkHyperTree vtkHyperTreeGrid
 *
 * @par Thanks:
 * This class was written by Guenole Harel and Jacques-Bernard Lekien, 2014.
 * This class was re-written by Philippe Pebay, 2016.
 * JB This class was re-written for more optimisation by Jacques-Bernard Lekien,
 * Guenole Harel and Jerome Dubois, 2018.
 * This work was supported by Commissariat a l'Energie Atomique
 * CEA, DAM, DIF, F-91297 Arpajon, France.
 */

#ifndef vtkHyperTreeGridOrientedGeometryCursor_h
#define vtkHyperTreeGridOrientedGeometryCursor_h

#include "vtkCommonDataModelModule.h"      // For export macro
#include "vtkHyperTreeGridGeometryEntry.h" // Used internally
#include "vtkHyperTreeGridTools.h"         // for HasTree
#include "vtkObject.h"

#include <memory> // For std::shared_ptr
#include <vector> // For std::vector

class vtkHyperTree;
class vtkHyperTreeGrid;
class vtkHyperTreeGridScales;

class VTKCOMMONDATAMODEL_EXPORT vtkHyperTreeGridOrientedGeometryCursor : public vtkObject
{
public:
  vtkTypeMacro(vtkHyperTreeGridOrientedGeometryCursor, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  static vtkHyperTreeGridOrientedGeometryCursor* New();

  void Dump(ostream& os);

  /**
   * Create a copy of `this'.
   * \post results_exists:result!=0
   */
  virtual vtkHyperTreeGridOrientedGeometryCursor* Clone();

  /**
   * Initialize cursor at root of given tree index in grid.
   */
  void Initialize(vtkHyperTreeGrid* grid, vtkIdType treeIndex, bool create = false);

  /**
   * Initialize cursor at root of given tree index in grid.
   */
  void Initialize(vtkHyperTreeGrid* grid, vtkHyperTree* tree, unsigned int level,
    vtkHyperTreeGridGeometryEntry& entry);

  /**
   * JB
   */
  void Initialize(vtkHyperTreeGrid* grid, vtkHyperTree* tree, unsigned int level, vtkIdType index,
    double* origin);

  /**
   * JB
   */
  void Initialize(vtkHyperTreeGridOrientedGeometryCursor* cursor);

  //@{
  /**
   * Return if a Tree pointing exist
   */
  bool HasTree() const { return vtk::hypertreegrid::HasTree(*this); }
  //@}

  //@{
  /**
   * Set the hyper tree to which the cursor is pointing.
   */
  vtkHyperTree* GetTree() const { return this->Tree; }
  //@}

  /**
   * Return the index of the current vertex in the tree.
   */
  vtkIdType GetVertexId();

  /**
   * Return the global index (relative to the grid) of the
   * current vertex in the tree.
   */
  vtkIdType GetGlobalNodeIndex();

  /**
   * Return the dimension of the tree.
   * \post positive_result: result>0
   */
  unsigned char GetDimension();

  /**
   * Return the number of children for each node (non-vertex leaf) of the tree.
   * \post positive_number: result>0
   */
  unsigned char GetNumberOfChildren();

  /**
   * JB
   */
  void SetGlobalIndexStart(vtkIdType index);

  /**
   * JB
   */
  void SetGlobalIndexFromLocal(vtkIdType index);

  /**
   * JB
   */
  double* GetOrigin();
  double* GetSize();

  /**
   * JB
   */
  void GetBounds(double bounds[6]);
  void GetPoint(double point[3]);

  /**
   * Set the blanking mask is empty or not
   * \pre not_tree: tree
   */
  void SetMask(bool state);

  /**
   * Determine whether blanking mask is empty or not
   */
  bool IsMasked();

  /**
   * Is the cursor pointing to a leaf?
   */
  bool IsLeaf();

  /**
   * JB Fait chier normalement on devrait passer par GetEntry
   */
  void SubdivideLeaf();

  /**
   * Is the cursor at tree root?
   */
  bool IsRoot();

  /**
   * Get the level of the tree vertex pointed by the cursor.
   */
  unsigned int GetLevel();

  /**
   * Move the cursor to child `child' of the current vertex.
   * \pre not_tree: HasTree()
   * \pre not_leaf: !IsLeaf()
   * \pre valid_child: ichild>=0 && ichild<GetNumberOfChildren()
   * \pre depth_limiter: GetLevel() <= GetDepthLimiter()
   */
  void ToChild(unsigned char ichild);

protected:
  /**
   * Constructor
   * JB Just pour vtkHyperTreeGridNonOrientedVonNeumannSuperCursor et Moore
   */
  vtkHyperTreeGridOrientedGeometryCursor();

  /**
   * Destructor
   * JB Just pour vtkHyperTreeGridNonOrientedVonNeumannSuperCursor et Moore
   */
  ~vtkHyperTreeGridOrientedGeometryCursor() override;

  /**
   * JB Reference sur l'hyper tree grid parcouru actuellement.
   */
  vtkHyperTreeGrid* Grid;

  /**
   * JB
   */
  vtkHyperTree* Tree;

  /**
   * JB Storage of pre-computed per-level cell scales
   */
  std::shared_ptr<vtkHyperTreeGridScales> Scales;

  /**
   * JB
   */
  unsigned int Level;

  // Hyper tree grid to which the cursor is attached
  vtkHyperTreeGridGeometryEntry Entry;

private:
  vtkHyperTreeGridOrientedGeometryCursor(const vtkHyperTreeGridOrientedGeometryCursor&) = delete;
  void operator=(const vtkHyperTreeGridOrientedGeometryCursor&) = delete;
};
#endif
