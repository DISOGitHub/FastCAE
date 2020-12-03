/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkHierarchicalBoxDataSetAlgorithm.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkHierarchicalBoxDataSetAlgorithm
 * @brief   superclass for algorithms that
 * produce vtkHierarchicalBoxDataSet as output.
 *
 * Algorithms that take any type of data object (including composite dataset)
 * and produce a vtkHierarchicalBoxDataSet in the output can subclass from this
 * class.
 */

#ifndef vtkHierarchicalBoxDataSetAlgorithm_h
#define vtkHierarchicalBoxDataSetAlgorithm_h

#include "vtkAlgorithm.h"
#include "vtkCommonExecutionModelModule.h" // For export macro

class vtkHierarchicalBoxDataSet;
class VTKCOMMONEXECUTIONMODEL_EXPORT vtkHierarchicalBoxDataSetAlgorithm : public vtkAlgorithm
{
public:
  static vtkHierarchicalBoxDataSetAlgorithm* New();
  vtkTypeMacro(vtkHierarchicalBoxDataSetAlgorithm, vtkAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get the output data object for a port on this algorithm.
   */
  vtkHierarchicalBoxDataSet* GetOutput();
  vtkHierarchicalBoxDataSet* GetOutput(int);
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
  vtkHierarchicalBoxDataSetAlgorithm();
  ~vtkHierarchicalBoxDataSetAlgorithm() override;

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
  vtkHierarchicalBoxDataSetAlgorithm(const vtkHierarchicalBoxDataSetAlgorithm&) = delete;
  void operator=(const vtkHierarchicalBoxDataSetAlgorithm&) = delete;
};

#endif
