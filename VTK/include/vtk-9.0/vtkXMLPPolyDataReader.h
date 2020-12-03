/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXMLPPolyDataReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkXMLPPolyDataReader
 * @brief   Read PVTK XML PolyData files.
 *
 * vtkXMLPPolyDataReader reads the PVTK XML PolyData file format.
 * This reads the parallel format's summary file and then uses
 * vtkXMLPolyDataReader to read data from the individual PolyData
 * piece files.  Streaming is supported.  The standard extension for
 * this reader's file format is "pvtp".
 *
 * @sa
 * vtkXMLPolyDataReader
 */

#ifndef vtkXMLPPolyDataReader_h
#define vtkXMLPPolyDataReader_h

#include "vtkIOXMLModule.h" // For export macro
#include "vtkXMLPUnstructuredDataReader.h"

class vtkPolyData;

class VTKIOXML_EXPORT vtkXMLPPolyDataReader : public vtkXMLPUnstructuredDataReader
{
public:
  vtkTypeMacro(vtkXMLPPolyDataReader, vtkXMLPUnstructuredDataReader);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  static vtkXMLPPolyDataReader* New();

  //@{
  /**
   * Get the reader's output.
   */
  vtkPolyData* GetOutput();
  vtkPolyData* GetOutput(int idx);
  //@}

protected:
  vtkXMLPPolyDataReader();
  ~vtkXMLPPolyDataReader() override;

  const char* GetDataSetName() override;
  void GetOutputUpdateExtent(int& piece, int& numberOfPieces, int& ghostLevel) override;
  vtkIdType GetNumberOfCellsInPiece(int piece) override;
  vtkIdType GetNumberOfVertsInPiece(int piece);
  vtkIdType GetNumberOfLinesInPiece(int piece);
  vtkIdType GetNumberOfStripsInPiece(int piece);
  vtkIdType GetNumberOfPolysInPiece(int piece);
  void SetupOutputTotals() override;

  void SetupOutputData() override;
  void SetupNextPiece() override;
  int ReadPieceData() override;

  void CopyArrayForCells(vtkDataArray* inArray, vtkDataArray* outArray) override;
  vtkXMLDataReader* CreatePieceReader() override;
  int FillOutputPortInformation(int, vtkInformation*) override;

  // The size of the UpdatePiece.
  vtkIdType TotalNumberOfVerts;
  vtkIdType TotalNumberOfLines;
  vtkIdType TotalNumberOfStrips;
  vtkIdType TotalNumberOfPolys;
  vtkIdType StartVert;
  vtkIdType StartLine;
  vtkIdType StartStrip;
  vtkIdType StartPoly;

private:
  vtkXMLPPolyDataReader(const vtkXMLPPolyDataReader&) = delete;
  void operator=(const vtkXMLPPolyDataReader&) = delete;
};

#endif
