/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGenericDataArray.txx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtkGenericDataArray_txx
#define vtkGenericDataArray_txx

#include "vtkGenericDataArray.h"

#include "vtkIdList.h"
#include "vtkMath.h"
#include "vtkVariantCast.h"

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
double* vtkGenericDataArray<DerivedT, ValueTypeT>::GetTuple(vtkIdType tupleIdx)
{
  assert(!this->LegacyTuple.empty() && "Number of components is nonzero.");
  this->GetTuple(tupleIdx, &this->LegacyTuple[0]);
  return &this->LegacyTuple[0];
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::GetTuple(vtkIdType tupleIdx, double* tuple)
{
  for (int c = 0; c < this->NumberOfComponents; ++c)
  {
    tuple[c] = static_cast<double>(this->GetTypedComponent(tupleIdx, c));
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InterpolateTuple(
  vtkIdType dstTupleIdx, vtkIdList* ptIndices, vtkAbstractArray* source, double* weights)
{
  // First, check for the common case of typeid(source) == typeid(this). This
  // way we don't waste time redoing the other checks in the superclass, and
  // can avoid doing a dispatch for the most common usage of this method.
  DerivedT* other = vtkArrayDownCast<DerivedT>(source);
  if (!other)
  {
    // Let the superclass handle dispatch/fallback.
    this->Superclass::InterpolateTuple(dstTupleIdx, ptIndices, source, weights);
    return;
  }

  int numComps = this->GetNumberOfComponents();
  if (other->GetNumberOfComponents() != numComps)
  {
    vtkErrorMacro("Number of components do not match: Source: "
      << other->GetNumberOfComponents() << " Dest: " << this->GetNumberOfComponents());
    return;
  }

  vtkIdType numIds = ptIndices->GetNumberOfIds();
  vtkIdType* ids = ptIndices->GetPointer(0);

  for (int c = 0; c < numComps; ++c)
  {
    double val = 0.;
    for (vtkIdType tupleId = 0; tupleId < numIds; ++tupleId)
    {
      vtkIdType t = ids[tupleId];
      double weight = weights[tupleId];
      val += weight * static_cast<double>(other->GetTypedComponent(t, c));
    }
    ValueType valT;
    vtkMath::RoundDoubleToIntegralIfNecessary(val, &valT);
    this->InsertTypedComponent(dstTupleIdx, c, valT);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InterpolateTuple(vtkIdType dstTupleIdx,
  vtkIdType srcTupleIdx1, vtkAbstractArray* source1, vtkIdType srcTupleIdx2,
  vtkAbstractArray* source2, double t)
{
  // First, check for the common case of typeid(source) == typeid(this). This
  // way we don't waste time redoing the other checks in the superclass, and
  // can avoid doing a dispatch for the most common usage of this method.
  DerivedT* other1 = vtkArrayDownCast<DerivedT>(source1);
  DerivedT* other2 = other1 ? vtkArrayDownCast<DerivedT>(source2) : nullptr;
  if (!other1 || !other2)
  {
    // Let the superclass handle dispatch/fallback.
    this->Superclass::InterpolateTuple(
      dstTupleIdx, srcTupleIdx1, source1, srcTupleIdx2, source2, t);
    return;
  }

  if (srcTupleIdx1 >= source1->GetNumberOfTuples())
  {
    vtkErrorMacro("Tuple 1 out of range for provided array. "
                  "Requested tuple: "
      << srcTupleIdx1
      << " "
         "Tuples: "
      << source1->GetNumberOfTuples());
    return;
  }

  if (srcTupleIdx2 >= source2->GetNumberOfTuples())
  {
    vtkErrorMacro("Tuple 2 out of range for provided array. "
                  "Requested tuple: "
      << srcTupleIdx2
      << " "
         "Tuples: "
      << source2->GetNumberOfTuples());
    return;
  }

  int numComps = this->GetNumberOfComponents();
  if (other1->GetNumberOfComponents() != numComps)
  {
    vtkErrorMacro("Number of components do not match: Source: "
      << other1->GetNumberOfComponents() << " Dest: " << this->GetNumberOfComponents());
    return;
  }
  if (other2->GetNumberOfComponents() != numComps)
  {
    vtkErrorMacro("Number of components do not match: Source: "
      << other2->GetNumberOfComponents() << " Dest: " << this->GetNumberOfComponents());
    return;
  }

  const double oneMinusT = 1. - t;
  double val;
  ValueType valT;

  for (int c = 0; c < numComps; ++c)
  {
    val = other1->GetTypedComponent(srcTupleIdx1, c) * oneMinusT +
      other2->GetTypedComponent(srcTupleIdx2, c) * t;
    vtkMath::RoundDoubleToIntegralIfNecessary(val, &valT);
    this->InsertTypedComponent(dstTupleIdx, c, valT);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::SetComponent(
  vtkIdType tupleIdx, int compIdx, double value)
{
  // Reimplemented for efficiency (base impl allocates heap memory)
  this->SetTypedComponent(tupleIdx, compIdx, static_cast<ValueType>(value));
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
double vtkGenericDataArray<DerivedT, ValueTypeT>::GetComponent(vtkIdType tupleIdx, int compIdx)
{
  // Reimplemented for efficiency (base impl allocates heap memory)
  return static_cast<double>(this->GetTypedComponent(tupleIdx, compIdx));
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::RemoveTuple(vtkIdType id)
{
  if (id < 0 || id >= this->GetNumberOfTuples())
  {
    // Nothing to be done
    return;
  }
  if (id == (this->GetNumberOfTuples() - 1))
  {
    // To remove last item, just decrease the size by one
    this->RemoveLastTuple();
    return;
  }

  // This is a very slow implementation since it uses generic API. Subclasses
  // are encouraged to provide a faster implementation.
  assert(((this->GetNumberOfTuples() - id) - 1) /* (length) */ > 0);

  int numComps = this->GetNumberOfComponents();
  vtkIdType fromTuple = id + 1;
  vtkIdType toTuple = id;
  vtkIdType endTuple = this->GetNumberOfTuples();
  for (; fromTuple != endTuple; ++toTuple, ++fromTuple)
  {
    for (int comp = 0; comp < numComps; ++comp)
    {
      this->SetTypedComponent(toTuple, comp, this->GetTypedComponent(fromTuple, comp));
    }
  }
  this->SetNumberOfTuples(this->GetNumberOfTuples() - 1);
  this->DataChanged();
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::SetVoidArray(void*, vtkIdType, int)
{
  vtkErrorMacro("SetVoidArray is not supported by this class.");
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::SetVoidArray(void*, vtkIdType, int, int)
{
  vtkErrorMacro("SetVoidArray is not supported by this class.");
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::SetArrayFreeFunction(void (*)(void*))
{
  vtkErrorMacro("SetArrayFreeFunction is not supported by this class.");
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void* vtkGenericDataArray<DerivedT, ValueTypeT>::WriteVoidPointer(vtkIdType, vtkIdType)
{
  vtkErrorMacro("WriteVoidPointer is not supported by this class.");
  return nullptr;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
typename vtkGenericDataArray<DerivedT, ValueTypeT>::ValueType*
vtkGenericDataArray<DerivedT, ValueTypeT>::WritePointer(vtkIdType id, vtkIdType number)
{
  return static_cast<ValueType*>(this->WriteVoidPointer(id, number));
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
int vtkGenericDataArray<DerivedT, ValueTypeT>::GetDataType() const
{
  return vtkTypeTraits<ValueType>::VTK_TYPE_ID;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
int vtkGenericDataArray<DerivedT, ValueTypeT>::GetDataTypeSize() const
{
  return static_cast<int>(sizeof(ValueType));
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
bool vtkGenericDataArray<DerivedT, ValueTypeT>::HasStandardMemoryLayout() const
{
  // False by default, AoS should set true.
  return false;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void* vtkGenericDataArray<DerivedT, ValueTypeT>::GetVoidPointer(vtkIdType)
{
  vtkErrorMacro("GetVoidPointer is not supported by this class.");
  return nullptr;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
typename vtkGenericDataArray<DerivedT, ValueTypeT>::ValueType*
vtkGenericDataArray<DerivedT, ValueTypeT>::GetPointer(vtkIdType id)
{
  return static_cast<ValueType*>(this->GetVoidPointer(id));
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkIdType vtkGenericDataArray<DerivedT, ValueTypeT>::LookupValue(vtkVariant valueVariant)
{
  bool valid = true;
  ValueType value = vtkVariantCast<ValueType>(valueVariant, &valid);
  if (valid)
  {
    return this->LookupTypedValue(value);
  }
  return -1;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkIdType vtkGenericDataArray<DerivedT, ValueTypeT>::LookupTypedValue(ValueType value)
{
  return this->Lookup.LookupValue(value);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::LookupValue(vtkVariant valueVariant, vtkIdList* ids)
{
  ids->Reset();
  bool valid = true;
  ValueType value = vtkVariantCast<ValueType>(valueVariant, &valid);
  if (valid)
  {
    this->LookupTypedValue(value, ids);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::LookupTypedValue(ValueType value, vtkIdList* ids)
{
  ids->Reset();
  this->Lookup.LookupValue(value, ids);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::ClearLookup()
{
  this->Lookup.ClearLookup();
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::DataChanged()
{
  this->Lookup.ClearLookup();
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::SetVariantValue(
  vtkIdType valueIdx, vtkVariant valueVariant)
{
  bool valid = true;
  ValueType value = vtkVariantCast<ValueType>(valueVariant, &valid);
  if (valid)
  {
    this->SetValue(valueIdx, value);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkVariant vtkGenericDataArray<DerivedT, ValueTypeT>::GetVariantValue(vtkIdType valueIdx)
{
  return vtkVariant(this->GetValue(valueIdx));
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertVariantValue(
  vtkIdType valueIdx, vtkVariant valueVariant)
{
  bool valid = true;
  ValueType value = vtkVariantCast<ValueType>(valueVariant, &valid);
  if (valid)
  {
    this->InsertValue(valueIdx, value);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkTypeBool vtkGenericDataArray<DerivedT, ValueTypeT>::Allocate(
  vtkIdType size, vtkIdType vtkNotUsed(ext))
{
  // Allocator must updated this->Size and this->MaxId properly.
  this->MaxId = -1;
  if (size > this->Size || size == 0)
  {
    this->Size = 0;

    // let's keep the size an integral multiple of the number of components.
    size = size < 0 ? 0 : size;
    int numComps = this->GetNumberOfComponents() > 0 ? this->GetNumberOfComponents() : 1;
    double ceilNum = ceil(static_cast<double>(size) / static_cast<double>(numComps));
    vtkIdType numTuples = static_cast<vtkIdType>(ceilNum);
    // NOTE: if numTuples is 0, AllocateTuples is expected to release the
    // memory.
    if (this->AllocateTuples(numTuples) == false)
    {
      vtkErrorMacro(
        "Unable to allocate " << size << " elements of size " << sizeof(ValueType) << " bytes. ");
#if !defined NDEBUG
      // We're debugging, crash here preserving the stack
      abort();
#elif !defined VTK_DONT_THROW_BAD_ALLOC
      // We can throw something that has universal meaning
      throw std::bad_alloc();
#else
      // We indicate that alloc failed by return
      return 0;
#endif
    }
    this->Size = numTuples * numComps;
  }
  this->DataChanged();
  return 1;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkTypeBool vtkGenericDataArray<DerivedT, ValueTypeT>::Resize(vtkIdType numTuples)
{
  int numComps = this->GetNumberOfComponents();
  vtkIdType curNumTuples = this->Size / (numComps > 0 ? numComps : 1);
  if (numTuples > curNumTuples)
  {
    // Requested size is bigger than current size.  Allocate enough
    // memory to fit the requested size and be more than double the
    // currently allocated memory.
    numTuples = curNumTuples + numTuples;
  }
  else if (numTuples == curNumTuples)
  {
    return 1;
  }
  else
  {
    // Requested size is smaller than current size.  Squeeze the
    // memory.
    this->DataChanged();
  }

  assert(numTuples >= 0);

  if (!this->ReallocateTuples(numTuples))
  {
    vtkErrorMacro("Unable to allocate " << numTuples * numComps << " elements of size "
                                        << sizeof(ValueType) << " bytes. ");
#if !defined NDEBUG
    // We're debugging, crash here preserving the stack
    abort();
#elif !defined VTK_DONT_THROW_BAD_ALLOC
    // We can throw something that has universal meaning
    throw std::bad_alloc();
#else
    // We indicate that malloc failed by return
    return 0;
#endif
  }

  // Allocation was successful. Save it.
  this->Size = numTuples * numComps;

  // Update MaxId if we truncated:
  if ((this->Size - 1) < this->MaxId)
  {
    this->MaxId = (this->Size - 1);
  }

  return 1;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::SetNumberOfComponents(int num)
{
  this->vtkDataArray::SetNumberOfComponents(num);
  this->LegacyTuple.resize(num);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::SetNumberOfTuples(vtkIdType number)
{
  vtkIdType newSize = number * this->NumberOfComponents;
  if (this->Allocate(newSize, 0))
  {
    this->MaxId = newSize - 1;
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::Initialize()
{
  this->Resize(0);
  this->DataChanged();
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::Squeeze()
{
  this->Resize(this->GetNumberOfTuples());
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::SetTuple(
  vtkIdType dstTupleIdx, vtkIdType srcTupleIdx, vtkAbstractArray* source)
{
  // First, check for the common case of typeid(source) == typeid(this). This
  // way we don't waste time redoing the other checks in the superclass, and
  // can avoid doing a dispatch for the most common usage of this method.
  DerivedT* other = vtkArrayDownCast<DerivedT>(source);
  if (!other)
  {
    // Let the superclass handle dispatch/fallback.
    this->Superclass::SetTuple(dstTupleIdx, srcTupleIdx, source);
    return;
  }

  int numComps = this->GetNumberOfComponents();
  if (source->GetNumberOfComponents() != numComps)
  {
    vtkErrorMacro("Number of components do not match: Source: "
      << source->GetNumberOfComponents() << " Dest: " << this->GetNumberOfComponents());
    return;
  }

  for (int c = 0; c < numComps; ++c)
  {
    this->SetTypedComponent(dstTupleIdx, c, other->GetTypedComponent(srcTupleIdx, c));
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertTuples(
  vtkIdList* dstIds, vtkIdList* srcIds, vtkAbstractArray* source)
{
  // First, check for the common case of typeid(source) == typeid(this). This
  // way we don't waste time redoing the other checks in the superclass, and
  // can avoid doing a dispatch for the most common usage of this method.
  DerivedT* other = vtkArrayDownCast<DerivedT>(source);
  if (!other)
  {
    // Let the superclass handle dispatch/fallback.
    this->Superclass::InsertTuples(dstIds, srcIds, source);
    return;
  }

  if (dstIds->GetNumberOfIds() == 0)
  {
    return;
  }

  if (dstIds->GetNumberOfIds() != srcIds->GetNumberOfIds())
  {
    vtkErrorMacro("Mismatched number of tuples ids. Source: "
      << srcIds->GetNumberOfIds() << " Dest: " << dstIds->GetNumberOfIds());
    return;
  }

  int numComps = this->GetNumberOfComponents();
  if (other->GetNumberOfComponents() != numComps)
  {
    vtkErrorMacro("Number of components do not match: Source: "
      << other->GetNumberOfComponents() << " Dest: " << this->GetNumberOfComponents());
    return;
  }

  vtkIdType maxSrcTupleId = srcIds->GetId(0);
  vtkIdType maxDstTupleId = dstIds->GetId(0);
  for (int i = 0; i < dstIds->GetNumberOfIds(); ++i)
  {
    // parenthesis around std::max prevent MSVC macro replacement when
    // inlined:
    maxSrcTupleId = (std::max)(maxSrcTupleId, srcIds->GetId(i));
    maxDstTupleId = (std::max)(maxDstTupleId, dstIds->GetId(i));
  }

  if (maxSrcTupleId >= other->GetNumberOfTuples())
  {
    vtkErrorMacro("Source array too small, requested tuple at index "
      << maxSrcTupleId << ", but there are only " << other->GetNumberOfTuples()
      << " tuples in the array.");
    return;
  }

  vtkIdType newSize = (maxDstTupleId + 1) * this->NumberOfComponents;
  if (this->Size < newSize)
  {
    if (!this->Resize(maxDstTupleId + 1))
    {
      vtkErrorMacro("Resize failed.");
      return;
    }
  }

  // parenthesis around std::max prevent MSVC macro replacement when
  // inlined:
  this->MaxId = (std::max)(this->MaxId, newSize - 1);

  vtkIdType numTuples = srcIds->GetNumberOfIds();
  for (vtkIdType t = 0; t < numTuples; ++t)
  {
    vtkIdType srcT = srcIds->GetId(t);
    vtkIdType dstT = dstIds->GetId(t);
    for (int c = 0; c < numComps; ++c)
    {
      this->SetTypedComponent(dstT, c, other->GetTypedComponent(srcT, c));
    }
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertTuple(
  vtkIdType i, vtkIdType j, vtkAbstractArray* source)
{
  this->EnsureAccessToTuple(i);
  this->SetTuple(i, j, source);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertTuple(vtkIdType i, const float* source)
{
  this->EnsureAccessToTuple(i);
  this->SetTuple(i, source);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertTuple(vtkIdType i, const double* source)
{
  this->EnsureAccessToTuple(i);
  this->SetTuple(i, source);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertComponent(
  vtkIdType tupleIdx, int compIdx, double value)
{
  // Update MaxId to the inserted component (not the complete tuple) for
  // compatibility with InsertNextValue.
  vtkIdType newMaxId = tupleIdx * this->NumberOfComponents + compIdx;
  if (newMaxId < this->MaxId)
  {
    newMaxId = this->MaxId;
  }
  this->EnsureAccessToTuple(tupleIdx);
  assert("Sufficient space allocated." && this->MaxId >= newMaxId);
  this->MaxId = newMaxId;
  this->SetComponent(tupleIdx, compIdx, value);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkIdType vtkGenericDataArray<DerivedT, ValueTypeT>::InsertNextTuple(
  vtkIdType srcTupleIdx, vtkAbstractArray* source)
{
  vtkIdType nextTuple = this->GetNumberOfTuples();
  this->InsertTuple(nextTuple, srcTupleIdx, source);
  return nextTuple;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkIdType vtkGenericDataArray<DerivedT, ValueTypeT>::InsertNextTuple(const float* tuple)
{
  vtkIdType nextTuple = this->GetNumberOfTuples();
  this->InsertTuple(nextTuple, tuple);
  return nextTuple;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkIdType vtkGenericDataArray<DerivedT, ValueTypeT>::InsertNextTuple(const double* tuple)
{
  vtkIdType nextTuple = this->GetNumberOfTuples();
  this->InsertTuple(nextTuple, tuple);
  return nextTuple;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::GetTuples(
  vtkIdList* tupleIds, vtkAbstractArray* output)
{
  // First, check for the common case of typeid(source) == typeid(this). This
  // way we don't waste time redoing the other checks in the superclass, and
  // can avoid doing a dispatch for the most common usage of this method.
  DerivedT* other = vtkArrayDownCast<DerivedT>(output);
  if (!other)
  {
    // Let the superclass handle dispatch/fallback.
    this->Superclass::GetTuples(tupleIds, output);
    return;
  }

  int numComps = this->GetNumberOfComponents();
  if (other->GetNumberOfComponents() != numComps)
  {
    vtkErrorMacro("Number of components for input and output do not match.\n"
                  "Source: "
      << this->GetNumberOfComponents()
      << "\n"
         "Destination: "
      << other->GetNumberOfComponents());
    return;
  }

  vtkIdType* srcTuple = tupleIds->GetPointer(0);
  vtkIdType* srcTupleEnd = tupleIds->GetPointer(tupleIds->GetNumberOfIds());
  vtkIdType dstTuple = 0;

  while (srcTuple != srcTupleEnd)
  {
    for (int c = 0; c < numComps; ++c)
    {
      other->SetTypedComponent(dstTuple, c, this->GetTypedComponent(*srcTuple, c));
    }
    ++srcTuple;
    ++dstTuple;
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::GetTuples(
  vtkIdType p1, vtkIdType p2, vtkAbstractArray* output)
{
  // First, check for the common case of typeid(source) == typeid(this). This
  // way we don't waste time redoing the other checks in the superclass, and
  // can avoid doing a dispatch for the most common usage of this method.
  DerivedT* other = vtkArrayDownCast<DerivedT>(output);
  if (!other)
  {
    // Let the superclass handle dispatch/fallback.
    this->Superclass::GetTuples(p1, p2, output);
    return;
  }

  int numComps = this->GetNumberOfComponents();
  if (other->GetNumberOfComponents() != numComps)
  {
    vtkErrorMacro("Number of components for input and output do not match.\n"
                  "Source: "
      << this->GetNumberOfComponents()
      << "\n"
         "Destination: "
      << other->GetNumberOfComponents());
    return;
  }

  // p1-p2 are inclusive
  for (vtkIdType srcT = p1, dstT = 0; srcT <= p2; ++srcT, ++dstT)
  {
    for (int c = 0; c < numComps; ++c)
    {
      other->SetTypedComponent(dstT, c, this->GetTypedComponent(srcT, c));
    }
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkArrayIterator* vtkGenericDataArray<DerivedT, ValueTypeT>::NewIterator()
{
  vtkWarningMacro(<< "No vtkArrayIterator defined for " << this->GetClassName() << " arrays.");
  return nullptr;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkIdType vtkGenericDataArray<DerivedT, ValueTypeT>::InsertNextValue(ValueType value)
{
  vtkIdType nextValueIdx = this->MaxId + 1;
  if (nextValueIdx >= this->Size)
  {
    vtkIdType tuple = nextValueIdx / this->NumberOfComponents;
    this->EnsureAccessToTuple(tuple);
    // Since EnsureAccessToTuple will update the MaxId to point to the last
    // component in the last tuple, we move it back to support this method on
    // multi-component arrays.
    this->MaxId = nextValueIdx;
  }

  // Extending array without needing to reallocate:
  if (this->MaxId < nextValueIdx)
  {
    this->MaxId = nextValueIdx;
  }

  this->SetValue(nextValueIdx, value);
  return nextValueIdx;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertValue(vtkIdType valueIdx, ValueType value)
{
  vtkIdType tuple = valueIdx / this->NumberOfComponents;
  // Update MaxId to the inserted component (not the complete tuple) for
  // compatibility with InsertNextValue.
  vtkIdType newMaxId = valueIdx > this->MaxId ? valueIdx : this->MaxId;
  if (this->EnsureAccessToTuple(tuple))
  {
    assert("Sufficient space allocated." && this->MaxId >= newMaxId);
    this->MaxId = newMaxId;
    this->SetValue(valueIdx, value);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertTypedTuple(
  vtkIdType tupleIdx, const ValueType* t)
{
  if (this->EnsureAccessToTuple(tupleIdx))
  {
    this->SetTypedTuple(tupleIdx, t);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkIdType vtkGenericDataArray<DerivedT, ValueTypeT>::InsertNextTypedTuple(const ValueType* t)
{
  vtkIdType nextTuple = this->GetNumberOfTuples();
  this->InsertTypedTuple(nextTuple, t);
  return nextTuple;
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::InsertTypedComponent(
  vtkIdType tupleIdx, int compIdx, ValueType val)
{
  // Update MaxId to the inserted component (not the complete tuple) for
  // compatibility with InsertNextValue.
  vtkIdType newMaxId = tupleIdx * this->NumberOfComponents + compIdx;
  if (this->MaxId > newMaxId)
  {
    newMaxId = this->MaxId;
  }
  this->EnsureAccessToTuple(tupleIdx);
  assert("Sufficient space allocated." && this->MaxId >= newMaxId);
  this->MaxId = newMaxId;
  this->SetTypedComponent(tupleIdx, compIdx, val);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::GetValueRange(ValueType range[2], int comp)
{
  this->ComputeValueRange(range, comp);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
typename vtkGenericDataArray<DerivedT, ValueTypeT>::ValueType*
vtkGenericDataArray<DerivedT, ValueTypeT>::GetValueRange(int comp)
{
  this->LegacyValueRange.resize(2);
  this->GetValueRange(this->LegacyValueRange.data(), comp);
  return &this->LegacyValueRange[0];
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::GetFiniteValueRange(ValueType range[2], int comp)
{
  this->ComputeFiniteValueRange(range, comp);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
typename vtkGenericDataArray<DerivedT, ValueTypeT>::ValueType*
vtkGenericDataArray<DerivedT, ValueTypeT>::GetFiniteValueRange(int comp)
{
  this->LegacyValueRange.resize(2);
  this->GetFiniteValueRange(this->LegacyValueRange.data(), comp);
  return &this->LegacyValueRange[0];
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::FillTypedComponent(int compIdx, ValueType value)
{
  if (compIdx < 0 || compIdx >= this->NumberOfComponents)
  {
    vtkErrorMacro(<< "Specified component " << compIdx << " is not in [0, "
                  << this->NumberOfComponents << ")");
    return;
  }
  for (vtkIdType i = 0; i < this->GetNumberOfTuples(); ++i)
  {
    this->SetTypedComponent(i, compIdx, value);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::FillValue(ValueType value)
{
  for (int i = 0; i < this->NumberOfComponents; ++i)
  {
    this->FillTypedComponent(i, value);
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::FillComponent(int compIdx, double value)
{
  this->FillTypedComponent(compIdx, static_cast<ValueType>(value));
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkGenericDataArray<DerivedT, ValueTypeT>::vtkGenericDataArray()
{
  // Initialize internal data structures:
  this->Lookup.SetArray(this);
  this->SetNumberOfComponents(this->NumberOfComponents);
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
vtkGenericDataArray<DerivedT, ValueTypeT>::~vtkGenericDataArray()
{
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
bool vtkGenericDataArray<DerivedT, ValueTypeT>::EnsureAccessToTuple(vtkIdType tupleIdx)
{
  if (tupleIdx < 0)
  {
    return false;
  }
  vtkIdType minSize = (1 + tupleIdx) * this->NumberOfComponents;
  vtkIdType expectedMaxId = minSize - 1;
  if (this->MaxId < expectedMaxId)
  {
    if (this->Size < minSize)
    {
      if (!this->Resize(tupleIdx + 1))
      {
        return false;
      }
    }
    this->MaxId = expectedMaxId;
  }
  return true;
}

// The following introduces a layer of indirection that allows us to use the
// optimized range computation logic in vtkDataArrayPrivate.txx for common
// arrays, but fallback to computing the range at double precision and then
// converting to the valuetype for unknown array types, or for types where
// the conversion from double->ValueType doesn't lose precision.

template <typename ValueType>
class vtkAOSDataArrayTemplate;
template <typename ValueType>
class vtkSOADataArrayTemplate;

#ifdef VTK_USE_SCALED_SOA_ARRAYS
template <typename ValueType>
class vtkScaledSOADataArrayTemplate;
#endif

namespace vtk_GDA_detail
{

// Arrays templates with compiled-in support for value ranges in
// vtkGenericDataArray.cxx
template <typename ArrayType>
struct ATIsSupported : public std::false_type
{
};

template <typename ValueType>
struct ATIsSupported<vtkAOSDataArrayTemplate<ValueType> > : public std::true_type
{
};

template <typename ValueType>
struct ATIsSupported<vtkSOADataArrayTemplate<ValueType> > : public std::true_type
{
};

#ifdef VTK_USE_SCALED_SOA_ARRAYS
template <typename ValueType>
struct ATIsSupported<vtkScaledSOADataArrayTemplate<ValueType> > : public std::true_type
{
};
#endif

// ValueTypes with compiled-in support for value ranges in
// vtkGenericDataArray.cxx
template <typename ValueType>
struct VTIsSupported : public std::false_type
{
};
template <>
struct VTIsSupported<long> : public std::true_type
{
};
template <>
struct VTIsSupported<unsigned long> : public std::true_type
{
};
template <>
struct VTIsSupported<long long> : public std::true_type
{
};
template <>
struct VTIsSupported<unsigned long long> : public std::true_type
{
};

// Full array types with compiled-in support for value ranges in
// vtkGenericDataArray.cxx
template <typename ArrayType, typename ValueType>
struct IsSupported
  : public std::integral_constant<bool,
      (ATIsSupported<ArrayType>::value && VTIsSupported<ValueType>::value)>
{
};

} // end namespace vtk_GDA_detail

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::ComputeValueRange(ValueType range[2], int comp)
{
  using namespace vtk_GDA_detail;
  using Supported = IsSupported<DerivedT, ValueTypeT>;

  // For array / value types without specific implementations compiled into
  // vtkGenericDataArray.cxx, fall back to the GetRange computations in
  // vtkDataArray. In these cases, either a) the ValueType's full range is
  // expressable as a double, or b) we aren't aware of the array type.
  // This reduces the number of specialized range implementations we need to
  // compile, and is also faster since we're able to cache the GetValue
  // computation (See #17666).
  if (!Supported::value)
  {
    double tmpRange[2];
    this->ComputeRange(tmpRange, comp);
    range[0] = static_cast<ValueType>(tmpRange[0]);
    range[1] = static_cast<ValueType>(tmpRange[1]);
    return;
  }

  range[0] = vtkTypeTraits<ValueType>::Max();
  range[1] = vtkTypeTraits<ValueType>::Min();

  if (comp > this->NumberOfComponents)
  {
    return;
  }

  if (comp < 0 && this->NumberOfComponents == 1)
  {
    comp = 0;
  }

  // TODO this should eventually cache the results, but we do not have support
  // for all of the information keys we need to cover all possible value types.
  if (comp < 0)
  {
    this->ComputeVectorValueRange(range);
  }
  else
  {
    this->LegacyValueRangeFull.resize(this->NumberOfComponents * 2);
    if (this->ComputeScalarValueRange(this->LegacyValueRangeFull.data()))
    {
      range[0] = this->LegacyValueRangeFull[comp * 2];
      range[1] = this->LegacyValueRangeFull[comp * 2 + 1];
    }
  }
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
void vtkGenericDataArray<DerivedT, ValueTypeT>::ComputeFiniteValueRange(
  ValueType range[2], int comp)
{
  using namespace vtk_GDA_detail;
  using Supported = IsSupported<DerivedT, ValueTypeT>;

  // For array / value types without specific implementations compiled into
  // vtkGenericDataArray.cxx, fall back to the GetRange computations in
  // vtkDataArray. In these cases, either a) the ValueType's full range is
  // expressable as a double, or b) we aren't aware of the array type.
  // This reduces the number of specialized range implementations we need to
  // compile, and is also faster since we're able to cache the GetValue
  // computation (See #17666).
  if (!Supported::value)
  {
    double tmpRange[2];
    this->ComputeFiniteRange(tmpRange, comp);
    range[0] = static_cast<ValueType>(tmpRange[0]);
    range[1] = static_cast<ValueType>(tmpRange[1]);
    return;
  }

  range[0] = vtkTypeTraits<ValueType>::Max();
  range[1] = vtkTypeTraits<ValueType>::Min();

  if (comp > this->NumberOfComponents)
  {
    return;
  }

  if (comp < 0 && this->NumberOfComponents == 1)
  {
    comp = 0;
  }

  // TODO this should eventually cache the results, but we do not have support
  // for all of the information keys we need to cover all possible value types.
  if (comp < 0)
  {
    this->ComputeFiniteVectorValueRange(range);
  }
  else
  {
    this->LegacyValueRangeFull.resize(this->NumberOfComponents * 2);
    if (this->ComputeFiniteScalarValueRange(this->LegacyValueRangeFull.data()))
    {
      range[0] = this->LegacyValueRangeFull[comp * 2];
      range[1] = this->LegacyValueRangeFull[comp * 2 + 1];
    }
  }
}

namespace vtk_GDA_detail
{

template <typename ArrayType, typename ValueType, typename Tag>
bool ComputeScalarValueRangeImpl(ArrayType* array, ValueType* range, Tag tag, std::true_type)
{
  return ::vtkDataArrayPrivate::DoComputeScalarRange(array, range, tag);
}

template <typename ArrayType, typename ValueType, typename Tag>
bool ComputeScalarValueRangeImpl(ArrayType* array, ValueType* range, Tag tag, std::false_type)
{
  // Compute the range at double precision.
  std::size_t numComps = static_cast<size_t>(array->GetNumberOfComponents());
  std::vector<double> tmpRange(numComps * 2);
  if (!::vtkDataArrayPrivate::DoComputeScalarRange(
        static_cast<vtkDataArray*>(array), tmpRange.data(), tag))
  {
    return false;
  }

  for (std::size_t i = 0; i < numComps * 2; ++i)
  {
    range[i] = static_cast<ValueType>(tmpRange[i]);
  }

  return true;
}

template <typename ArrayType, typename ValueType, typename Tag>
bool ComputeVectorValueRangeImpl(ArrayType* array, ValueType range[2], Tag tag, std::true_type)
{
  return ::vtkDataArrayPrivate::DoComputeVectorRange(array, range, tag);
}

template <typename ArrayType, typename ValueType, typename Tag>
bool ComputeVectorValueRangeImpl(ArrayType* array, ValueType range[2], Tag tag, std::false_type)
{
  // Compute the range at double precision.
  double tmpRange[2];
  if (!::vtkDataArrayPrivate::DoComputeVectorRange(
        static_cast<vtkDataArray*>(array), tmpRange, tag))
  {
    return false;
  }

  range[0] = static_cast<ValueType>(tmpRange[0]);
  range[1] = static_cast<ValueType>(tmpRange[1]);

  return true;
}

} // namespace vtk_GDA_detail

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
bool vtkGenericDataArray<DerivedT, ValueTypeT>::ComputeScalarValueRange(ValueType* ranges)
{
  using namespace vtk_GDA_detail;
  using Supported = IsSupported<DerivedT, ValueTypeT>;
  return ComputeScalarValueRangeImpl(
    static_cast<DerivedT*>(this), ranges, vtkDataArrayPrivate::AllValues{}, Supported{});
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
bool vtkGenericDataArray<DerivedT, ValueTypeT>::ComputeVectorValueRange(ValueType range[2])
{
  using namespace vtk_GDA_detail;
  using Supported = IsSupported<DerivedT, ValueTypeT>;
  return ComputeVectorValueRangeImpl(
    static_cast<DerivedT*>(this), range, vtkDataArrayPrivate::AllValues{}, Supported{});
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
bool vtkGenericDataArray<DerivedT, ValueTypeT>::ComputeFiniteScalarValueRange(ValueType* range)
{
  using namespace vtk_GDA_detail;
  using Supported = IsSupported<DerivedT, ValueTypeT>;
  return ComputeScalarValueRangeImpl(
    static_cast<DerivedT*>(this), range, vtkDataArrayPrivate::FiniteValues{}, Supported{});
}

//-----------------------------------------------------------------------------
template <class DerivedT, class ValueTypeT>
bool vtkGenericDataArray<DerivedT, ValueTypeT>::ComputeFiniteVectorValueRange(ValueType range[2])
{
  using namespace vtk_GDA_detail;
  using Supported = IsSupported<DerivedT, ValueTypeT>;
  return ComputeVectorValueRangeImpl(
    static_cast<DerivedT*>(this), range, vtkDataArrayPrivate::FiniteValues{}, Supported{});
}

#undef vtkGenericDataArrayT

#endif // header guard
