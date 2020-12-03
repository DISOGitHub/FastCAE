/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXMLGenericDataObjectReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkXMLGenericDataObjectReader
 * @brief   Read any type of vtk data object
 *
 * vtkXMLGenericDataObjectReader reads any type of vtk data object encoded
 * in XML format.
 *
 * @sa
 * vtkGenericDataObjectReader
 */

#ifndef vtkXMLGenericDataObjectReader_h
#define vtkXMLGenericDataObjectReader_h

#include "vtkIOXMLModule.h" // For export macro
#include "vtkXMLDataReader.h"

class vtkHierarchicalBoxDataSet;
class vtkMultiBlockDataSet;
class vtkImageData;
class vtkPolyData;
class vtkRectilinearGrid;
class vtkStructuredGrid;
class vtkUnstructuredGrid;

class VTKIOXML_EXPORT vtkXMLGenericDataObjectReader : public vtkXMLDataReader
{
public:
  vtkTypeMacro(vtkXMLGenericDataObjectReader, vtkXMLDataReader);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  static vtkXMLGenericDataObjectReader* New();

  //@{
  /**
   * Get the reader's output.
   */
  vtkDataObject* GetOutput();
  vtkDataObject* GetOutput(int idx);
  //@}

  //@{
  /**
   * Get the output as various concrete types. This method is typically used
   * when you know exactly what type of data is being read.  Otherwise, use
   * the general GetOutput() method. If the wrong type is used nullptr is
   * returned.  (You must also set the filename of the object prior to
   * getting the output.)
   */
  vtkHierarchicalBoxDataSet* GetHierarchicalBoxDataSetOutput();
  vtkImageData* GetImageDataOutput();
  vtkMultiBlockDataSet* GetMultiBlockDataSetOutput();
  vtkPolyData* GetPolyDataOutput();
  vtkRectilinearGrid* GetRectilinearGridOutput();
  vtkStructuredGrid* GetStructuredGridOutput();
  vtkUnstructuredGrid* GetUnstructuredGridOutput();
  //@}

  /**
   * Overridden method.
   */
  vtkIdType GetNumberOfPoints() override;

  /**
   * Overridden method.
   */
  vtkIdType GetNumberOfCells() override;

  /**
   * Overridden method. Not Used. Delegated.
   */
  void SetupEmptyOutput() override;

  /**
   * This method can be used to find out the type of output expected without
   * needing to read the whole file.
   */
  virtual int ReadOutputType(const char* name, bool& parallel);

protected:
  vtkXMLGenericDataObjectReader();
  ~vtkXMLGenericDataObjectReader() override;

  /**
   * Overridden method. Not used. Return "vtkDataObject".
   */
  const char* GetDataSetName() override;

  int RequestDataObject(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  virtual int RequestUpdateExtent(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector);

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int FillOutputPortInformation(int, vtkInformation*) override;

  vtkXMLReader* Reader; // actual reader

private:
  vtkXMLGenericDataObjectReader(const vtkXMLGenericDataObjectReader&) = delete;
  void operator=(const vtkXMLGenericDataObjectReader&) = delete;
};

#endif
