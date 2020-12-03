/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkExtractTensorComponents.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkExtractTensorComponents
 * @brief   extract parts of tensor and create a scalar, vector, normal, or texture coordinates.
 *
 * vtkExtractTensorComponents is a filter that extracts components of
 * a tensor to create a scalar, vector, normal, or texture coords. For
 * example, if the tensor contains components of stress, then you
 * could extract the normal stress in the x-direction as a scalar
 * (i.e., tensor component (0,0).
 *
 * To use this filter, you must set some boolean flags to control
 * which data is extracted from the tensors, and whether you want to
 * pass the tensor data through to the output. Also, you must specify
 * the tensor component(s) for each type of data you want to
 * extract. The tensor component(s) is(are) specified using matrix notation
 * into a 3x3 matrix. That is, use the (row,column) address to specify
 * a particular tensor component; and if the data you are extracting
 * requires more than one component, use a list of addresses. (Note
 * that the addresses are 0-offset -> (0,0) specifies upper left
 * corner of the tensor.)
 *
 * There are two optional methods to extract scalar data. You can
 * extract the determinant of the tensor, or you can extract the
 * effective stress of the tensor. These require that the ivar
 * ExtractScalars is on, and the appropriate scalar extraction mode is
 * set.
 */

#ifndef vtkExtractTensorComponents_h
#define vtkExtractTensorComponents_h

#include "vtkDataSetAlgorithm.h"
#include "vtkFiltersExtractionModule.h" // For export macro

#define VTK_EXTRACT_COMPONENT 0
#define VTK_EXTRACT_EFFECTIVE_STRESS 1
#define VTK_EXTRACT_DETERMINANT 2

class VTKFILTERSEXTRACTION_EXPORT vtkExtractTensorComponents : public vtkDataSetAlgorithm
{
public:
  vtkTypeMacro(vtkExtractTensorComponents, vtkDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Construct object to extract nothing and to not pass tensor data
   * through the pipeline.
   */
  static vtkExtractTensorComponents* New();

  //@{
  /**
   * Boolean controls whether tensor data is passed through to output.
   */
  vtkSetMacro(PassTensorsToOutput, vtkTypeBool);
  vtkGetMacro(PassTensorsToOutput, vtkTypeBool);
  vtkBooleanMacro(PassTensorsToOutput, vtkTypeBool);
  //@}

  //@{
  /**
   * Boolean controls whether scalar data is extracted from tensor.
   */
  vtkSetMacro(ExtractScalars, vtkTypeBool);
  vtkGetMacro(ExtractScalars, vtkTypeBool);
  vtkBooleanMacro(ExtractScalars, vtkTypeBool);
  //@}

  //@{
  /**
   * Specify the (row,column) tensor component to extract as a scalar.
   */
  vtkSetVector2Macro(ScalarComponents, int);
  vtkGetVectorMacro(ScalarComponents, int, 2);
  //@}

  //@{
  /**
   * Specify how to extract the scalar. You can extract it as one of
   * the components of the tensor, as effective stress, or as the
   * determinant of the tensor. If you extract a component make sure
   * that you set the ScalarComponents ivar.
   */
  vtkSetMacro(ScalarMode, int);
  vtkGetMacro(ScalarMode, int);
  void SetScalarModeToComponent() { this->SetScalarMode(VTK_EXTRACT_COMPONENT); }
  void SetScalarModeToEffectiveStress() { this->SetScalarMode(VTK_EXTRACT_EFFECTIVE_STRESS); }
  void SetScalarModeToDeterminant() { this->SetScalarMode(VTK_EXTRACT_DETERMINANT); }
  void ScalarIsComponent() { this->SetScalarMode(VTK_EXTRACT_COMPONENT); }
  void ScalarIsEffectiveStress() { this->SetScalarMode(VTK_EXTRACT_EFFECTIVE_STRESS); }
  void ScalarIsDeterminant() { this->SetScalarMode(VTK_EXTRACT_DETERMINANT); }
  //@}

  //@{
  /**
   * Boolean controls whether vector data is extracted from tensor.
   */
  vtkSetMacro(ExtractVectors, vtkTypeBool);
  vtkGetMacro(ExtractVectors, vtkTypeBool);
  vtkBooleanMacro(ExtractVectors, vtkTypeBool);
  //@}

  //@{
  /**
   * Specify the ((row,column)0,(row,column)1,(row,column)2) tensor
   * components to extract as a vector.
   */
  vtkSetVector6Macro(VectorComponents, int);
  vtkGetVectorMacro(VectorComponents, int, 6);
  //@}

  //@{
  /**
   * Boolean controls whether normal data is extracted from tensor.
   */
  vtkSetMacro(ExtractNormals, vtkTypeBool);
  vtkGetMacro(ExtractNormals, vtkTypeBool);
  vtkBooleanMacro(ExtractNormals, vtkTypeBool);
  //@}

  //@{
  /**
   * Boolean controls whether normal vector is converted to unit normal
   * after extraction.
   */
  vtkSetMacro(NormalizeNormals, vtkTypeBool);
  vtkGetMacro(NormalizeNormals, vtkTypeBool);
  vtkBooleanMacro(NormalizeNormals, vtkTypeBool);
  //@}

  //@{
  /**
   * Specify the ((row,column)0,(row,column)1,(row,column)2) tensor
   * components to extract as a vector.
   */
  vtkSetVector6Macro(NormalComponents, int);
  vtkGetVectorMacro(NormalComponents, int, 6);
  //@}

  //@{
  /**
   * Boolean controls whether texture coordinates are extracted from tensor.
   */
  vtkSetMacro(ExtractTCoords, vtkTypeBool);
  vtkGetMacro(ExtractTCoords, vtkTypeBool);
  vtkBooleanMacro(ExtractTCoords, vtkTypeBool);
  //@}

  //@{
  /**
   * Set the dimension of the texture coordinates to extract.
   */
  vtkSetClampMacro(NumberOfTCoords, int, 1, 3);
  vtkGetMacro(NumberOfTCoords, int);
  //@}

  //@{
  /**
   * Specify the ((row,column)0,(row,column)1,(row,column)2) tensor
   * components to extract as a vector. Up to NumberOfTCoords
   * components are extracted.
   */
  vtkSetVector6Macro(TCoordComponents, int);
  vtkGetVectorMacro(TCoordComponents, int, 6);
  //@}

protected:
  vtkExtractTensorComponents();
  ~vtkExtractTensorComponents() override {}

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  vtkTypeBool PassTensorsToOutput;

  vtkTypeBool ExtractScalars;
  vtkTypeBool ExtractVectors;
  vtkTypeBool ExtractNormals;
  vtkTypeBool ExtractTCoords;

  int ScalarMode;
  int ScalarComponents[2];

  int VectorComponents[6];

  vtkTypeBool NormalizeNormals;
  int NormalComponents[6];

  int NumberOfTCoords;
  int TCoordComponents[6];

private:
  vtkExtractTensorComponents(const vtkExtractTensorComponents&) = delete;
  void operator=(const vtkExtractTensorComponents&) = delete;
};

#endif
