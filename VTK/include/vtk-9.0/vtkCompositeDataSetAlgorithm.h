/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCompositeDataSetAlgorithm.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkCompositeDataSetAlgorithm
 * @brief   Superclass for algorithms that produce only vtkCompositeDataSet as output
 *
 * Algorithms that take any type of data object (including composite dataset)
 * and produce a vtkCompositeDataSet in the output can subclass from this
 * class.
 */

#ifndef vtkCompositeDataSetAlgorithm_h
#define vtkCompositeDataSetAlgorithm_h

#include "vtkAlgorithm.h"
#include "vtkCommonExecutionModelModule.h" // For export macro

class vtkCompositeDataSet;

class VTKCOMMONEXECUTIONMODEL_EXPORT vtkCompositeDataSetAlgorithm : public vtkAlgorithm
{
public:
  static vtkCompositeDataSetAlgorithm* New();
  vtkTypeMacro(vtkCompositeDataSetAlgorithm, vtkAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get the output data object for a port on this algorithm.
   */
  vtkCompositeDataSet* GetOutput();
  vtkCompositeDataSet* GetOutput(int);
  //@}

  //@{
  /**
   * Assign a data object as input. Note that this method does not
   * establish a pipeline connection. Use SetInputConnection() to
   * setup a pipeline connection.
   */
  void SetInputData(vtkDataObject*);
  void SetInputData(int, vtkDataObject*);
  //@}

  /**
   * see vtkAlgorithm for details
   */
  vtkTypeBool ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

protected:
  vtkCompositeDataSetAlgorithm();
  ~vtkCompositeDataSetAlgorithm() override {}

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestDataObject(vtkInformation*, vtkInformationVector**, vtkInformationVector*)
  {
    return 1;
  }

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*)
  {
    return 1;
  }

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*)
  {
    return 1;
  }

  //@{
  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*)
  {
    return 1;
  }
  //@}

  // Create a default executive.
  vtkExecutive* CreateDefaultExecutive() override;

  // see algorithm for more info
  int FillOutputPortInformation(int port, vtkInformation* info) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;

  vtkDataObject* GetInput(int port);

private:
  vtkCompositeDataSetAlgorithm(const vtkCompositeDataSetAlgorithm&) = delete;
  void operator=(const vtkCompositeDataSetAlgorithm&) = delete;
};

#endif
