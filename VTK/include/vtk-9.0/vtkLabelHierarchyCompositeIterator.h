/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkLabelHierarchyCompositeIterator.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
/**
 * @class   vtkLabelHierarchyCompositeIterator
 * @brief   Iterator over sub-iterators
 *
 *
 * Iterates over child iterators in a round-robin order. Each iterator may
 * have its own count, which is the number of times it is repeated until
 * moving to the next iterator.
 *
 * For example, if you initialize the iterator with
 * <pre>
 * it->AddIterator(A, 1);
 * it->AddIterator(B, 3);
 * </pre>
 * The order of iterators will be A,B,B,B,A,B,B,B,...
 */

#ifndef vtkLabelHierarchyCompositeIterator_h
#define vtkLabelHierarchyCompositeIterator_h

#include "vtkLabelHierarchyIterator.h"
#include "vtkRenderingLabelModule.h" // For export macro

class vtkIdTypeArray;
class vtkLabelHierarchy;
class vtkPolyData;

class VTKRENDERINGLABEL_EXPORT vtkLabelHierarchyCompositeIterator : public vtkLabelHierarchyIterator
{
public:
  vtkTypeMacro(vtkLabelHierarchyCompositeIterator, vtkLabelHierarchyIterator);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  static vtkLabelHierarchyCompositeIterator* New();

  /**
   * Adds a label iterator to this composite iterator.
   * The second optional argument is the number of times to repeat the iterator
   * before moving to the next one round-robin style. Default is 1.
   */
  virtual void AddIterator(vtkLabelHierarchyIterator* it) { this->AddIterator(it, 1); }
  virtual void AddIterator(vtkLabelHierarchyIterator* it, int count);

  /**
   * Remove all iterators from this composite iterator.
   */
  virtual void ClearIterators();

  /**
   * Initializes the iterator. lastLabels is an array holding labels
   * which should be traversed before any other labels in the hierarchy.
   * This could include labels placed during a previous rendering or
   * a label located under the mouse pointer. You may pass a null pointer.
   */
  void Begin(vtkIdTypeArray*) override;

  /**
   * Advance the iterator.
   */
  void Next() override;

  /**
   * Returns true if the iterator is at the end.
   */
  bool IsAtEnd() override;

  /**
   * Retrieves the current label id.
   */
  vtkIdType GetLabelId() override;

  /**
   * Retrieve the current label hierarchy.
   */
  vtkLabelHierarchy* GetHierarchy() override;

  /**
   * Retrieve the coordinates of the center of the current hierarchy node
   * and the size of the node.
   * Nodes are n-cubes, so the size is the length of any edge of the cube.
   * This is used by BoxNode().
   */
  void GetNodeGeometry(double ctr[3], double& size) override;

  /**
   * Not implemented.
   */
  void BoxNode() override {}

  /**
   * Not implemented.
   */
  void BoxAllNodes(vtkPolyData*) override {}

protected:
  vtkLabelHierarchyCompositeIterator();
  ~vtkLabelHierarchyCompositeIterator() override;

  class Internal;
  Internal* Implementation;

private:
  vtkLabelHierarchyCompositeIterator(const vtkLabelHierarchyCompositeIterator&) = delete;
  void operator=(const vtkLabelHierarchyCompositeIterator&) = delete;
};

#endif // vtkLabelHierarchyCompositeIterator_h
