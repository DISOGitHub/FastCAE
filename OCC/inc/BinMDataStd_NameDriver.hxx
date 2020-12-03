// Created on: 2002-11-19
// Created by: Edward AGAPOV
// Copyright (c) 2002-2014 OPEN CASCADE SAS
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

#ifndef _BinMDataStd_NameDriver_HeaderFile
#define _BinMDataStd_NameDriver_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <BinMDF_ADriver.hxx>
#include <Standard_Boolean.hxx>
#include <BinObjMgt_RRelocationTable.hxx>
#include <BinObjMgt_SRelocationTable.hxx>
class Message_Messenger;
class TDF_Attribute;
class BinObjMgt_Persistent;


class BinMDataStd_NameDriver;
DEFINE_STANDARD_HANDLE(BinMDataStd_NameDriver, BinMDF_ADriver)

//! TDataStd_Name attribute Driver.
class BinMDataStd_NameDriver : public BinMDF_ADriver
{

public:

  
  Standard_EXPORT BinMDataStd_NameDriver(const Handle(Message_Messenger)& theMessageDriver);
  
  Standard_EXPORT Handle(TDF_Attribute) NewEmpty() const Standard_OVERRIDE;
  
  //! persistent -> transient (retrieve)
  Standard_EXPORT Standard_Boolean Paste (const BinObjMgt_Persistent& Source, const Handle(TDF_Attribute)& Target, BinObjMgt_RRelocationTable& RelocTable) const Standard_OVERRIDE;
  
  //! transient -> persistent (store)
  Standard_EXPORT void Paste (const Handle(TDF_Attribute)& Source, BinObjMgt_Persistent& Target, BinObjMgt_SRelocationTable& RelocTable) const Standard_OVERRIDE;




  DEFINE_STANDARD_RTTIEXT(BinMDataStd_NameDriver,BinMDF_ADriver)

protected:




private:




};







#endif // _BinMDataStd_NameDriver_HeaderFile
