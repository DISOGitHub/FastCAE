/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMergeArrays.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkMergeArrays
 * @brief   Multiple inputs with one output.
 *
 * vtkMergeArrays tries to put all arrays from all inputs into one output.
 * The output data object is the same as the first data input.
 * The filter checks for a consistent number of points and cells with
 * respect to the first input, but does not check any more. Any inputs
 * which do not have the correct number of points or cells are ignored
 * for that type of data set attribute. When adding new arrays, if there
 * is an existing array of the same name and attribute type, the new array
 * will have the name mangled to be the original array name plus
 * `_input_<inputid>` where `<inputid>` is the id/index of the input filter
 * that is providing that array.
 */

#ifndef vtkMergeArrays_h
#define vtkMergeArrays_h

#include "vtkFiltersGeneralModule.h" // For export macro
#include "vtkPassInputTypeAlgorithm.h"

#include <string> // Needed for protected method argument

class vtkDataSet;
class vtkFieldData;

class VTKFILTERSGENERAL_EXPORT vtkMergeArrays : public vtkPassInputTypeAlgorithm
{
public:
  static vtkMergeArrays* New();

  vtkTypeMacro(vtkMergeArrays, vtkPassInputTypeAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkMergeArrays();
  ~vtkMergeArrays() override;

  //@{
  /**
   * Given an existing set of output arrays and an array name and input data set
   * index, return an appropriate name to use for the output array. Returns true
   * if the name is a new name and false if not.
   */
  virtual bool GetOutputArrayName(
    vtkFieldData* arrays, const char* inArrayName, int inputIndex, std::string& outArrayName);
  //@}

  //@{
  /**
   * Add input field arrays to output, mangling output array names as needed
   * based on inputIndex.
   */
  void MergeArrays(int inputIndex, vtkFieldData* inputFD, vtkFieldData* outputFD);

  //@{
  /**
   * For a given input and index, add data arrays to the output. Returns 1 for
   * success and 0 for failure.
   */
  virtual int MergeDataObjectFields(vtkDataObject* input, int inputIndex, vtkDataObject* output);
  //@}

  // see algorithm for more info
  int FillInputPortInformation(int port, vtkInformation* info) override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkMergeArrays(const vtkMergeArrays&) = delete;
  void operator=(const vtkMergeArrays&) = delete;
};

#endif
