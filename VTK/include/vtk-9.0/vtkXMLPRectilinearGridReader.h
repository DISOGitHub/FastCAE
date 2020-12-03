/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXMLPRectilinearGridReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkXMLPRectilinearGridReader
 * @brief   Read PVTK XML RectilinearGrid files.
 *
 * vtkXMLPRectilinearGridReader reads the PVTK XML RectilinearGrid
 * file format.  This reads the parallel format's summary file and
 * then uses vtkXMLRectilinearGridReader to read data from the
 * individual RectilinearGrid piece files.  Streaming is supported.
 * The standard extension for this reader's file format is "pvtr".
 *
 * @sa
 * vtkXMLRectilinearGridReader
 */

#ifndef vtkXMLPRectilinearGridReader_h
#define vtkXMLPRectilinearGridReader_h

#include "vtkIOXMLModule.h" // For export macro
#include "vtkXMLPStructuredDataReader.h"

class vtkRectilinearGrid;

class VTKIOXML_EXPORT vtkXMLPRectilinearGridReader : public vtkXMLPStructuredDataReader
{
public:
  vtkTypeMacro(vtkXMLPRectilinearGridReader, vtkXMLPStructuredDataReader);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  static vtkXMLPRectilinearGridReader* New();

  //@{
  /**
   * Get the reader's output.
   */
  vtkRectilinearGrid* GetOutput();
  vtkRectilinearGrid* GetOutput(int idx);
  //@}

protected:
  vtkXMLPRectilinearGridReader();
  ~vtkXMLPRectilinearGridReader() override;

  vtkRectilinearGrid* GetPieceInput(int index);

  void SetupEmptyOutput() override;
  const char* GetDataSetName() override;
  void SetOutputExtent(int* extent) override;
  void GetPieceInputExtent(int index, int* extent) override;
  int ReadPrimaryElement(vtkXMLDataElement* ePrimary) override;
  void SetupOutputData() override;
  int ReadPieceData() override;
  vtkXMLDataReader* CreatePieceReader() override;
  void CopySubCoordinates(
    int* inBounds, int* outBounds, int* subBounds, vtkDataArray* inArray, vtkDataArray* outArray);
  int FillOutputPortInformation(int, vtkInformation*) override;

  // The PCoordinates element with coordinate information.
  vtkXMLDataElement* PCoordinatesElement;

private:
  vtkXMLPRectilinearGridReader(const vtkXMLPRectilinearGridReader&) = delete;
  void operator=(const vtkXMLPRectilinearGridReader&) = delete;
};

#endif
