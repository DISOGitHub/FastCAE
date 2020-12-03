/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDataObjectTreeRange.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef vtkDataObjectTreeRange_h
#define vtkDataObjectTreeRange_h

#include "vtkCompositeDataSetNodeReference.h"
#include "vtkDataObjectTree.h"
#include "vtkDataObjectTreeIterator.h"
#include "vtkMeta.h"
#include "vtkRange.h"
#include "vtkSmartPointer.h"

#include <cassert>

#ifndef __VTK_WRAP__

namespace vtk
{

// Pass these to vtk::Range(cds, options):
enum class DataObjectTreeOptions : unsigned int
{
  None = 0,
  SkipEmptyNodes = 1 << 1,  // Skip null datasets.
  VisitOnlyLeaves = 1 << 2, // Skip child composite datasets.
  TraverseSubTree = 1 << 3, // Descend into child composite datasets.
};

} // end namespace vtk (for bitflag op definition)

VTK_GENERATE_BITFLAG_OPS(vtk::DataObjectTreeOptions)

namespace vtk
{

namespace detail
{

struct DataObjectTreeRange;
struct DataObjectTreeIterator;

using DataObjectTreeIteratorReference =
  vtk::CompositeDataSetNodeReference<vtkDataObjectTreeIterator, DataObjectTreeIterator>;

struct DataObjectTreeIterator
  : public std::iterator<std::forward_iterator_tag, vtkDataObject*, int,
      DataObjectTreeIteratorReference, DataObjectTreeIteratorReference>
{
private:
  using Superclass = std::iterator<std::forward_iterator_tag, vtkDataObject*, int,
    DataObjectTreeIteratorReference, DataObjectTreeIteratorReference>;
  using InternalIterator = vtkDataObjectTreeIterator;
  using SmartIterator = vtkSmartPointer<InternalIterator>;

public:
  using iterator_category = typename Superclass::iterator_category;
  using value_type = typename Superclass::value_type;
  using difference_type = typename Superclass::difference_type;
  using pointer = typename Superclass::pointer;
  using reference = typename Superclass::reference;

  DataObjectTreeIterator(const DataObjectTreeIterator& o)
    : Iterator(o.Iterator ? SmartIterator::Take(o.Iterator->NewInstance()) : nullptr)
  {
    this->CopyState(o.Iterator);
  }

  DataObjectTreeIterator(DataObjectTreeIterator&&) noexcept = default;

  DataObjectTreeIterator& operator=(const DataObjectTreeIterator& o)
  {
    this->Iterator = o.Iterator ? SmartIterator::Take(o.Iterator->NewInstance()) : nullptr;
    this->CopyState(o.Iterator);
    return *this;
  }

  DataObjectTreeIterator& operator++() // prefix
  {
    this->Increment();
    return *this;
  }

  DataObjectTreeIterator operator++(int) // postfix
  {
    DataObjectTreeIterator other(*this);
    this->Increment();
    return other;
  }

  reference operator*() const { return this->GetData(); }

  pointer operator->() const { return this->GetData(); }

  friend bool operator==(const DataObjectTreeIterator& lhs, const DataObjectTreeIterator& rhs)
  {
    // A null internal iterator means it is an 'end' sentinal.
    InternalIterator* l = lhs.Iterator;
    InternalIterator* r = rhs.Iterator;

    if (!r && !l)
    { // end == end
      return true;
    }
    else if (!r)
    { // right is end
      return l->IsDoneWithTraversal() != 0;
    }
    else if (!l)
    { // left is end
      return r->IsDoneWithTraversal() != 0;
    }
    else
    { // Both iterators are valid, check unique idx:
      return r->GetCurrentFlatIndex() == l->GetCurrentFlatIndex();
    }
  }

  friend bool operator!=(const DataObjectTreeIterator& lhs, const DataObjectTreeIterator& rhs)
  {
    return !(lhs == rhs); // let the compiler handle this one =)
  }

  friend void swap(DataObjectTreeIterator& lhs, DataObjectTreeIterator& rhs) noexcept
  {
    using std::swap;
    swap(lhs.Iterator, rhs.Iterator);
  }

  friend struct DataObjectTreeRange;

protected:
  // Note: This takes ownership of iter and manages its lifetime.
  // Iter should not be used past this point by the caller.
  DataObjectTreeIterator(SmartIterator&& iter) noexcept : Iterator(std::move(iter)) {}

  // Note: Iterators constructed using this ctor will be considered
  // 'end' iterators via a sentinal pattern.
  DataObjectTreeIterator() noexcept : Iterator{ nullptr } {}

private:
  void CopyState(InternalIterator* source)
  {
    if (source)
    {
      assert(this->Iterator != nullptr);
      this->Iterator->SetDataSet(source->GetDataSet());
      this->Iterator->SetSkipEmptyNodes(source->GetSkipEmptyNodes());
      this->Iterator->SetVisitOnlyLeaves(source->GetVisitOnlyLeaves());
      this->Iterator->SetTraverseSubTree(source->GetTraverseSubTree());
      this->Iterator->InitTraversal();
      this->AdvanceTo(source->GetCurrentFlatIndex());
    }
  }

  void AdvanceTo(const unsigned int flatIdx)
  {
    assert(this->Iterator != nullptr);
    assert(this->Iterator->GetCurrentFlatIndex() <= flatIdx);
    while (this->Iterator->GetCurrentFlatIndex() < flatIdx)
    {
      this->Increment();
    }
  }

  void Increment()
  {
    assert(this->Iterator != nullptr);
    assert(!this->Iterator->IsDoneWithTraversal());
    this->Iterator->GoToNextItem();
  }

  DataObjectTreeIteratorReference GetData() const
  {
    assert(this->Iterator != nullptr);
    assert(!this->Iterator->IsDoneWithTraversal());
    return DataObjectTreeIteratorReference{ this->Iterator };
  }

  mutable SmartIterator Iterator;
};

//------------------------------------------------------------------------------
// DataObjectTree range proxy.
struct DataObjectTreeRange
{
private:
  using InternalIterator = vtkDataObjectTreeIterator;
  using SmartIterator = vtkSmartPointer<InternalIterator>;

public:
  using size_type = int;
  using iterator = DataObjectTreeIterator;
  using const_iterator = DataObjectTreeIterator;
  using reference = DataObjectTreeIteratorReference;
  using const_reference = const DataObjectTreeIteratorReference;
  using value_type = vtkDataObject*;

  DataObjectTreeRange(
    vtkDataObjectTree* cds, DataObjectTreeOptions opts = DataObjectTreeOptions::None)
    : DataObjectTree(cds)
    , Options(opts)
  {
    assert(this->DataObjectTree);
  }

  vtkDataObjectTree* GetDataObjectTree() const noexcept { return this->DataObjectTree; }

  DataObjectTreeOptions GetOptions() const noexcept { return this->Options; }

  // This is O(N), since the size requires traversal due to various options.
  size_type size() const
  {
    size_type result = 0;
    auto iter = this->NewIterator();
    iter->InitTraversal();
    while (!iter->IsDoneWithTraversal())
    {
      ++result;
      iter->GoToNextItem();
    }
    return result;
  }

  iterator begin() const { return DataObjectTreeIterator{ this->NewIterator() }; }

  iterator end() const { return DataObjectTreeIterator{}; }

  // Note: These return mutable objects because const vtkObject are unusable.
  const_iterator cbegin() const { return DataObjectTreeIterator{ this->NewIterator() }; }

  // Note: These return mutable objects because const vtkObjects are unusable.
  const_iterator cend() const { return DataObjectTreeIterator{}; }

private:
  SmartIterator NewIterator() const
  {
    using Opts = vtk::DataObjectTreeOptions;

    auto result = SmartIterator::Take(this->DataObjectTree->NewTreeIterator());
    result->SetSkipEmptyNodes((this->Options & Opts::SkipEmptyNodes) != Opts::None);
    result->SetVisitOnlyLeaves((this->Options & Opts::VisitOnlyLeaves) != Opts::None);
    result->SetTraverseSubTree((this->Options & Opts::TraverseSubTree) != Opts::None);
    result->InitTraversal();
    return result;
  }

  mutable vtkSmartPointer<vtkDataObjectTree> DataObjectTree;
  DataObjectTreeOptions Options;
};

}
} // end namespace vtk::detail

#endif // __VTK_WRAP__

#endif // vtkDataObjectTreeRange_h

// VTK-HeaderTest-Exclude: vtkDataObjectTreeRange.h
