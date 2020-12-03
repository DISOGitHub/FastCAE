/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXMLPImageDataReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkXMLPImageDataReader
 * @brief   Read PVTK XML ImageData files.
 *
 * vtkXMLPImageDataReader reads the PVTK XML ImageData file format.
 * This reads the parallel format's summary file and then uses
 * vtkXMLImageDataReader to read data from the individual ImageData
 * piece files.  Streaming is supported.  The standard extension for
 * this reader's file format is "pvti".
 *
 * @sa
 * vtkXMLImageDataReader
 */

#ifndef vtkXMLPImageDataReader_h
#define vtkXMLPImageDataReader_h

#include "vtkIOXMLModule.h" // For export macro
#include "vtkXMLPStructuredDataReader.h"

class vtkImageData;

class VTKIOXML_EXPORT vtkXMLPImageDataReader : public vtkXMLPStructuredDataReader
{
public:
  vtkTypeMacro(vtkXMLPImageDataReader, vtkXMLPStructuredDataReader);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  static vtkXMLPImageDataReader* New();

  //@{
  /**
   * Get the reader's output.
   */
  vtkImageData* GetOutput();
  vtkImageData* GetOutput(int idx);
  //@}

  // For the specified port, copy the information this reader sets up in
  // SetupOutputInformation to outInfo
  void CopyOutputInformation(vtkInformation* outInfo, int port) override;

protected:
  vtkXMLPImageDataReader();
  ~vtkXMLPImageDataReader() override;

  double Origin[3];
  double Spacing[3];

  vtkImageData* GetPieceInput(int index);

  void SetupEmptyOutput() override;
  const char* GetDataSetName() override;
  void SetOutputExtent(int* extent) override;
  void GetPieceInputExtent(int index, int* extent) override;
  int ReadPrimaryElement(vtkXMLDataElement* ePrimary) override;

  // Setup the output's information.
  void SetupOutputInformation(vtkInformation* outInfo) override;

  vtkXMLDataReader* CreatePieceReader() override;
  int FillOutputPortInformation(int, vtkInformation*) override;

private:
  vtkXMLPImageDataReader(const vtkXMLPImageDataReader&) = delete;
  void operator=(const vtkXMLPImageDataReader&) = delete;
};

#endif
