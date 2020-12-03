// Created on: 1993-10-14
// Created by: Remi LEQUETTE
// Copyright (c) 1993-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _BRepAlgo_Common_HeaderFile
#define _BRepAlgo_Common_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <BRepAlgo_BooleanOperation.hxx>
class TopoDS_Shape;


//! Describes functions for performing a topological
//! common operation (Boolean intersection).
//! A Common object provides the framework for:
//! - defining the construction of a common shape,
//! - implementing the construction algorithm, and
//! - consulting the result.
class Standard_DEPRECATED("This class is deprecated - BRepAlgoAPI_Common should be used instead")
  BRepAlgo_Common  : public BRepAlgo_BooleanOperation
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Constructs the common part of shapes S1 and S2.
  Standard_EXPORT BRepAlgo_Common(const TopoDS_Shape& S1, const TopoDS_Shape& S2);




protected:





private:





};







#endif // _BRepAlgo_Common_HeaderFile
