/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkStructuredGridReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkStructuredGridReader
 * @brief   read vtk structured grid data file
 *
 * vtkStructuredGridReader is a source object that reads ASCII or binary
 * structured grid data files in vtk format. (see text for format details).
 * The output of this reader is a single vtkStructuredGrid data object.
 * The superclass of this class, vtkDataReader, provides many methods for
 * controlling the reading of the data file, see vtkDataReader for more
 * information.
 * @warning
 * Binary files written on one system may not be readable on other systems.
 * @sa
 * vtkStructuredGrid vtkDataReader
 */

#ifndef vtkStructuredGridReader_h
#define vtkStructuredGridReader_h

#include "vtkDataReader.h"
#include "vtkIOLegacyModule.h" // For export macro

class vtkStructuredGrid;

class VTKIOLEGACY_EXPORT vtkStructuredGridReader : public vtkDataReader
{
public:
  static vtkStructuredGridReader* New();
  vtkTypeMacro(vtkStructuredGridReader, vtkDataReader);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get the output of this reader.
   */
  vtkStructuredGrid* GetOutput();
  vtkStructuredGrid* GetOutput(int idx);
  void SetOutput(vtkStructuredGrid* output);
  //@}

  /**
   * Read the meta information from the file (WHOLE_EXTENT).
   */
  int ReadMetaDataSimple(const std::string& fname, vtkInformation* metadata) override;

  /**
   * Actual reading happens here
   */
  int ReadMeshSimple(const std::string& fname, vtkDataObject* output) override;

protected:
  vtkStructuredGridReader();
  ~vtkStructuredGridReader() override;

  int FillOutputPortInformation(int, vtkInformation*) override;

private:
  vtkStructuredGridReader(const vtkStructuredGridReader&) = delete;
  void operator=(const vtkStructuredGridReader&) = delete;
};

#endif
