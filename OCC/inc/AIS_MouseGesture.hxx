// Copyright (c) 2019 OPEN CASCADE SAS
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

#ifndef _AIS_MouseGesture_HeaderFile
#define _AIS_MouseGesture_HeaderFile

#include <NCollection_DataMap.hxx>

//! Mouse gesture - only one can be active at one moment.
enum AIS_MouseGesture
{
  AIS_MouseGesture_NONE,            //!< no active gesture
  //
  AIS_MouseGesture_SelectRectangle, //!< rectangular selection
  AIS_MouseGesture_SelectLasso,     //!< polygonal   selection
  //
  AIS_MouseGesture_Zoom,            //!< view zoom gesture
  AIS_MouseGesture_Pan,             //!< view panning gesture
  AIS_MouseGesture_RotateOrbit,     //!< orbit rotation gesture
  AIS_MouseGesture_RotateView,      //!< view  rotation gesture
};

//! Map defining mouse gestures.
typedef NCollection_DataMap<unsigned int, AIS_MouseGesture> AIS_MouseGestureMap;

#endif // _AIS_MouseGesture_HeaderFile
