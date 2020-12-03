/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOpenGLVolumeLookupTables.txx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef vtkOpenGLVolumeLookupTables_txx
#define vtkOpenGLVolumeLookupTables_txx

#include "vtkOpenGLVolumeLookupTables.h"

#include "vtkObjectFactory.h"
#include "vtkWindow.h"

template <class T>
vtkStandardNewMacro(vtkOpenGLVolumeLookupTables<T>);

//-----------------------------------------------------------------------------
template <class T>
vtkOpenGLVolumeLookupTables<T>::~vtkOpenGLVolumeLookupTables()
{
  for (auto it = this->Tables.begin(); it != this->Tables.end(); ++it)
  {
    (*it)->Delete();
  }
}

//----------------------------------------------------------------------------
template <class T>
void vtkOpenGLVolumeLookupTables<T>::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  for (auto it = this->Tables.begin(); it != this->Tables.end(); ++it)
  {
    (*it)->PrintSelf(os, indent.GetNextIndent());
  }
}

//----------------------------------------------------------------------------
template <class T>
void vtkOpenGLVolumeLookupTables<T>::Create(std::size_t numberOfTables)
{
  this->Tables.reserve(static_cast<std::size_t>(numberOfTables));
  for (std::size_t i = 0; i < numberOfTables; ++i)
  {
    auto* const table = T::New();
    // table->Register(this);
    this->Tables.push_back(table);
  }
}

//----------------------------------------------------------------------------
template <class T>
T* vtkOpenGLVolumeLookupTables<T>::GetTable(std::size_t i) const
{
  if (i >= this->Tables.size())
  {
    return nullptr;
  }
  return this->Tables[i];
}

//----------------------------------------------------------------------------
template <class T>
std::size_t vtkOpenGLVolumeLookupTables<T>::GetNumberOfTables() const
{
  return this->Tables.size();
}

//----------------------------------------------------------------------------
template <class T>
void vtkOpenGLVolumeLookupTables<T>::ReleaseGraphicsResources(vtkWindow* win)
{
  for (auto it = this->Tables.begin(); it != this->Tables.end(); ++it)
  {
    (*it)->ReleaseGraphicsResources(win);
  }
}

#endif // vtkOpenGLVolumeLookupTables_txx
