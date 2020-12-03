// Created on: 2004-10-29
// Created by: Alexander BORODIN
// Copyright (c) 2004-2014 OPEN CASCADE SAS
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

#ifndef Graphic3d_TRANS_MODE_FLAGS_HXX
#define Graphic3d_TRANS_MODE_FLAGS_HXX

//! Transform Persistence Mode defining whether to lock in object position, rotation and / or zooming relative to camera position.
enum Graphic3d_TransModeFlags
{
  Graphic3d_TMF_None           = 0x0000,                  //!< no persistence attributes (normal 3D object)
  Graphic3d_TMF_ZoomPers       = 0x0002,                  //!< object does not resize
  Graphic3d_TMF_RotatePers     = 0x0008,                  //!< object does not rotate;
  Graphic3d_TMF_TriedronPers   = 0x0020,                  //!< object behaves like trihedron - it is fixed at the corner of view and does not resizing (but rotating)
  Graphic3d_TMF_2d             = 0x0040,                  //!< object is defined in 2D screen coordinates (pixels) and does not resize, pan and rotate
  Graphic3d_TMF_ZoomRotatePers = Graphic3d_TMF_ZoomPers
                               | Graphic3d_TMF_RotatePers //!< object doesn't resize and rotate
};

#endif
