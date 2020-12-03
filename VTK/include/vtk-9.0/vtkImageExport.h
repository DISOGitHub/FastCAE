/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkImageExport.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkImageExport
 * @brief   Export VTK images to third-party systems.
 *
 * vtkImageExport provides a way of exporting image data at the end
 * of a pipeline to a third-party system or to a simple C array.
 * Applications can use this to get direct access to the image data
 * in memory.  A callback interface is provided to allow connection
 * of the VTK pipeline to a third-party pipeline.  This interface
 * conforms to the interface of vtkImageImport.
 * In Python it is possible to use this class to write the image data
 * into a python string that has been pre-allocated to be the correct
 * size.
 * @sa
 * vtkImageImport
 */

#ifndef vtkImageExport_h
#define vtkImageExport_h

#include "vtkIOImageModule.h" // For export macro
#include "vtkImageAlgorithm.h"

class VTKIOIMAGE_EXPORT vtkImageExport : public vtkImageAlgorithm
{
public:
  static vtkImageExport* New();
  vtkTypeMacro(vtkImageExport, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Get the number of bytes required for the output C array.
   */
  vtkIdType GetDataMemorySize();

  //@{
  /**
   * Get the (x,y,z) index dimensions of the data.  Please note
   * that C arrays are indexed in decreasing order, i.e. array[z][y][x].
   */
  void GetDataDimensions(int* ptr);
  int* GetDataDimensions() VTK_SIZEHINT(3)
  {
    this->GetDataDimensions(this->DataDimensions);
    return this->DataDimensions;
  }
  //@}

  /**
   * Get the number of scalar components of the data.  Please note that
   * when you index into a C array, the scalar component index comes
   * last, i.e. array[z][y][x][c].
   */
  int GetDataNumberOfScalarComponents();

  /**
   * Get the scalar type of the data.  The scalar type of the C array
   * must match the scalar type of the data.
   */
  int GetDataScalarType();
  const char* GetDataScalarTypeAsString()
  {
    return vtkImageScalarTypeNameMacro(this->GetDataScalarType());
  }

  //@{
  /**
   * Get miscellaneous additional information about the data.
   */
  int* GetDataExtent() VTK_SIZEHINT(6);
  void GetDataExtent(int* ptr);
  double* GetDataSpacing() VTK_SIZEHINT(3);
  void GetDataSpacing(double* ptr);
  double* GetDataOrigin() VTK_SIZEHINT(3);
  void GetDataOrigin(double* ptr);
  double* GetDataDirection() VTK_SIZEHINT(9);
  void GetDataDirection(double* ptr);
  //@}

  /**
   * Get the input object from the image pipeline.
   */
  vtkImageData* GetInput();

  //@{
  /**
   * Set/Get whether the data goes to the exported memory starting
   * in the lower left corner or upper left corner.  Default: On.
   * When this flag is Off, the image will be flipped vertically
   * before it is exported.
   * WARNING: this flag is used only with the Export() method,
   * it is ignored by GetPointerToData().
   */
  vtkBooleanMacro(ImageLowerLeft, vtkTypeBool);
  vtkGetMacro(ImageLowerLeft, vtkTypeBool);
  vtkSetMacro(ImageLowerLeft, vtkTypeBool);
  //@}

  //@{
  /**
   * Set the void pointer of the C array to export the data to.
   * From python, you can specify a pointer to a string that is
   * large enough to hold the data.
   */
  void SetExportVoidPointer(void*);
  void* GetExportVoidPointer() { return this->ExportVoidPointer; }
  //@}

  //@{
  /**
   * The main interface: update the pipeline and export the image
   * to the memory pointed to by SetExportVoidPointer().  You can
   * also specify a void pointer when you call Export().
   */
  void Export() { this->Export(this->ExportVoidPointer); }
  virtual void Export(void*);
  //@}

  /**
   * An alternative to Export(): Use with caution.   Update the
   * pipeline and return a pointer to the image memory.  The
   * pointer is only valid until the next time that the pipeline
   * is updated.
   * WARNING: This method ignores the ImageLowerLeft flag.
   */
  void* GetPointerToData();

  /**
   * Get the user data that should be passed to the callback functions.
   */
  void* GetCallbackUserData();

  //@{
  /**
   * These are function pointer types for the pipeline connection
   * callbacks.  See further documentation in vtkImageImport.h.
   */
  typedef void (*UpdateInformationCallbackType)(void*);
  typedef int (*PipelineModifiedCallbackType)(void*);
  typedef int* (*WholeExtentCallbackType)(void*);
  typedef double* (*SpacingCallbackType)(void*);
  typedef double* (*OriginCallbackType)(void*);
  typedef double* (*DirectionCallbackType)(void*);
  typedef const char* (*ScalarTypeCallbackType)(void*);
  typedef int (*NumberOfComponentsCallbackType)(void*);
  typedef void (*PropagateUpdateExtentCallbackType)(void*, int*);
  typedef void (*UpdateDataCallbackType)(void*);
  typedef int* (*DataExtentCallbackType)(void*);
  typedef void* (*BufferPointerCallbackType)(void*);
  //@}

  //@{
  /**
   * Get pointers to the pipeline interface callbacks.
   */
  UpdateInformationCallbackType GetUpdateInformationCallback() const;
  PipelineModifiedCallbackType GetPipelineModifiedCallback() const;
  WholeExtentCallbackType GetWholeExtentCallback() const;
  SpacingCallbackType GetSpacingCallback() const;
  OriginCallbackType GetOriginCallback() const;
  DirectionCallbackType GetDirectionCallback() const;
  ScalarTypeCallbackType GetScalarTypeCallback() const;
  NumberOfComponentsCallbackType GetNumberOfComponentsCallback() const;
  PropagateUpdateExtentCallbackType GetPropagateUpdateExtentCallback() const;
  UpdateDataCallbackType GetUpdateDataCallback() const;
  DataExtentCallbackType GetDataExtentCallback() const;
  BufferPointerCallbackType GetBufferPointerCallback() const;
  //@}

protected:
  vtkImageExport();
  ~vtkImageExport() override;

  // This is called by the superclass.
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  virtual void UpdateInformationCallback();
  virtual int PipelineModifiedCallback();
  virtual void UpdateDataCallback();
  virtual int* WholeExtentCallback();
  virtual double* SpacingCallback();
  virtual double* OriginCallback();
  virtual double* DirectionCallback();
  virtual const char* ScalarTypeCallback();
  virtual int NumberOfComponentsCallback();
  virtual void PropagateUpdateExtentCallback(int*);
  virtual int* DataExtentCallback();
  virtual void* BufferPointerCallback();

  vtkTypeBool ImageLowerLeft;
  int DataDimensions[3];
  void* ExportVoidPointer;

  vtkMTimeType LastPipelineMTime;

private:
  vtkImageExport(const vtkImageExport&) = delete;
  void operator=(const vtkImageExport&) = delete;

  static void UpdateInformationCallbackFunction(void*);
  static int PipelineModifiedCallbackFunction(void*);
  static int* WholeExtentCallbackFunction(void*);
  static double* SpacingCallbackFunction(void*);
  static double* OriginCallbackFunction(void*);
  static double* DirectionCallbackFunction(void*);
  static const char* ScalarTypeCallbackFunction(void*);
  static int NumberOfComponentsCallbackFunction(void*);
  static void PropagateUpdateExtentCallbackFunction(void*, int*);
  static void UpdateDataCallbackFunction(void*);
  static int* DataExtentCallbackFunction(void*);
  static void* BufferPointerCallbackFunction(void*);

  vtkAlgorithm* GetInputAlgorithm();
  vtkInformation* GetInputInformation();
};

#endif
