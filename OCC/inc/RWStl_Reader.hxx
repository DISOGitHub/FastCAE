// Created: 2016-05-01
// Author: Andrey Betenev
// Copyright: Open CASCADE 2016
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

#ifndef _RWStl_Reader_HeaderFile
#define _RWStl_Reader_HeaderFile

#include <Message_ProgressIndicator.hxx>
#include <gp_XYZ.hxx>

//! An abstract class implementing procedure to read STL file.
//!
//! This class is not bound to particular data structure and can be used to read the file directly into arbitrary data model.
//! To use it, create descendant class and implement methods addNode() and addTriangle().
//!
//! Call method Read() to read the file. In the process of reading, the tool will call methods addNode() and addTriangle() to fill the mesh data structure.
//!
//! The nodes with equal coordinates are merged automatically  on the fly.
class RWStl_Reader : public Standard_Transient
{
  DEFINE_STANDARD_RTTIEXT(RWStl_Reader, Standard_Transient)
public:

  //! Reads data from STL file (either binary or Ascii).
  //! This function supports reading multi-domain STL files formed by concatenation of several "plain" files.
  //! The mesh nodes are not merged between domains.
  //! Unicode paths can be given in UTF-8 encoding.
  //! Format is recognized automatically by analysis of the file header.
  //! Returns true if success, false on error or user break.
  Standard_EXPORT Standard_Boolean Read (const char* theFile,
                                         const Handle(Message_ProgressIndicator)& theProgress);

  //! Guess whether the stream is an Ascii STL file, by analysis of the first bytes (~200).
  //! The function attempts to put back the read symbols to the stream which thus must support ungetc().
  //! Returns true if the stream seems to contain Ascii STL.
  Standard_EXPORT Standard_Boolean IsAscii (Standard_IStream& theStream);

  //! Reads STL data from binary stream.
  //! The stream must be opened in binary mode.
  //! Stops after reading the number of triangles recorded in the file header.
  //! Returns true if success, false on error or user break.
  Standard_EXPORT Standard_Boolean ReadBinary (Standard_IStream& theStream,
                                               const Handle(Message_ProgressIndicator)& theProgress);

  //! Reads data from the stream assumed to contain Ascii STL data.
  //! The stream can be opened either in binary or in Ascii mode.
  //! Reading stops at the position specified by theUntilPos,
  //! or end of file is reached, or when keyword "endsolid" is found.
  //! Empty lines are not supported and will read to reading failure.
  //! If theUntilPos is non-zero, reads not more than until that position.
  //! Returns true if success, false on error or user break.
  Standard_EXPORT Standard_Boolean ReadAscii (Standard_IStream& theStream,
                                              const std::streampos theUntilPos,
                                              const Handle(Message_ProgressIndicator)& theProgress);

public:

  //! Callback function to be implemented in descendant.
  //! Should create new node with specified coordinates in the target model, and return its ID as integer.
  virtual Standard_Integer AddNode (const gp_XYZ& thePnt) = 0;

  //! Callback function to be implemented in descendant.
  //! Should create new triangle built on specified nodes in the target model.
  virtual void AddTriangle (Standard_Integer theN1, Standard_Integer theN2, Standard_Integer theN3) = 0;

};

#endif
