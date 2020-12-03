/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkLabelHierarchyAlgorithm.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkLabelHierarchyAlgorithm
 * @brief   Superclass for algorithms that produce only label hierarchies as output
 *
 *
 * vtkLabelHierarchyAlgorithm is a convenience class to make writing algorithms
 * easier. It is also designed to help transition old algorithms to the new
 * pipeline architecture. There are some assumptions and defaults made by this
 * class you should be aware of. This class defaults such that your filter
 * will have one input port and one output port. If that is not the case
 * simply change it with SetNumberOfInputPorts etc. See this class
 * constructor for the default. This class also provides a FillInputPortInfo
 * method that by default says that all inputs will be DataObjects. If that
 * isn't the case then please override this method in your subclass. This
 * class breaks out the downstream requests into separate functions such as
 * RequestData and RequestInformation.  You should
 * implement RequestData( request, inputVec, outputVec) in subclasses.
 */

#ifndef vtkLabelHierarchyAlgorithm_h
#define vtkLabelHierarchyAlgorithm_h

#include "vtkAlgorithm.h"
#include "vtkRenderingLabelModule.h" // For export macro

class vtkDataSet;
class vtkLabelHierarchy;

class VTKRENDERINGLABEL_EXPORT vtkLabelHierarchyAlgorithm : public vtkAlgorithm
{
public:
  static vtkLabelHierarchyAlgorithm* New();
  vtkTypeMacro(vtkLabelHierarchyAlgorithm, vtkAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get the output data object for a port on this algorithm.
   */
  vtkLabelHierarchy* GetOutput();
  vtkLabelHierarchy* GetOutput(int);
  virtual void SetOutput(vtkDataObject* d);
  //@}

  /**
   * see vtkAlgorithm for details
   */
  vtkTypeBool ProcessRequest(
    vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  // this method is not recommended for use, but lots of old style filters use it
  vtkDataObject* GetInput();
  vtkDataObject* GetInput(int port);
  vtkLabelHierarchy* GetLabelHierarchyInput(int port);

  //@{
  /**
   * Assign a data object as input. Note that this method does not
   * establish a pipeline connection. Use SetInputConnection() to
   * setup a pipeline connection.
   */
  void SetInputData(vtkDataObject*);
  void SetInputData(int, vtkDataObject*);
  //@}

  //@{
  /**
   * Assign a data object as input. Note that this method does not
   * establish a pipeline connection. Use AddInputConnection() to
   * setup a pipeline connection.
   */
  void AddInputData(vtkDataObject*);
  void AddInputData(int, vtkDataObject*);
  //@}

protected:
  vtkLabelHierarchyAlgorithm();
  ~vtkLabelHierarchyAlgorithm() override;

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestDataObject(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector);

  // convenience method
  virtual int RequestInformation(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector);

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector);

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*);

  // see algorithm for more info
  int FillOutputPortInformation(int port, vtkInformation* info) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;

private:
  vtkLabelHierarchyAlgorithm(const vtkLabelHierarchyAlgorithm&) = delete;
  void operator=(const vtkLabelHierarchyAlgorithm&) = delete;
};

#endif
