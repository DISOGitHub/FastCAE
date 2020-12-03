/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMapperCollection.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkMapperCollection
 * @brief   an ordered list of mappers
 *
 * vtkMapperCollection represents and provides methods to manipulate a list of
 * mappers (i.e., vtkMapper and subclasses). The list is ordered and duplicate
 * entries are not prevented.
 *
 * @sa
 * vtkMapper vtkCollection
 */

#ifndef vtkMapperCollection_h
#define vtkMapperCollection_h

#include "vtkCollection.h"
#include "vtkMapper.h"              // Needed for direct access to mapper methods in
                                    // inline functions
#include "vtkRenderingCoreModule.h" // For export macro

class VTKRENDERINGCORE_EXPORT vtkMapperCollection : public vtkCollection
{
public:
  static vtkMapperCollection* New();
  vtkTypeMacro(vtkMapperCollection, vtkCollection);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Add an mapper to the bottom of the list.
   */
  void AddItem(vtkMapper* a) { this->vtkCollection::AddItem(a); }

  /**
   * Get the next mapper in the list.
   */
  vtkMapper* GetNextItem() { return static_cast<vtkMapper*>(this->GetNextItemAsObject()); }

  /**
   * Get the last mapper in the list.
   */
  vtkMapper* GetLastItem()
  {
    return this->Bottom ? static_cast<vtkMapper*>(this->Bottom->Item) : nullptr;
  }

  /**
   * Reentrant safe way to get an object in a collection. Just pass the
   * same cookie back and forth.
   */
  vtkMapper* GetNextMapper(vtkCollectionSimpleIterator& cookie)
  {
    return static_cast<vtkMapper*>(this->GetNextItemAsObject(cookie));
  }

protected:
  vtkMapperCollection() {}
  ~vtkMapperCollection() override {}

private:
  // hide the standard AddItem from the user and the compiler.
  void AddItem(vtkObject* o) { this->vtkCollection::AddItem(o); }

private:
  vtkMapperCollection(const vtkMapperCollection&) = delete;
  void operator=(const vtkMapperCollection&) = delete;
};

#endif
