/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTextureMapToSphere.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkTextureMapToSphere
 * @brief   generate texture coordinates by mapping points to sphere
 *
 * vtkTextureMapToSphere is a filter that generates 2D texture coordinates by
 * mapping input dataset points onto a sphere. The sphere can either be user
 * specified or generated automatically. (The sphere is generated
 * automatically by computing the center (i.e., averaged coordinates) of the
 * sphere.)  Note that the generated texture coordinates range between
 * (0,1). The s-coordinate lies in the angular direction around the z-axis,
 * measured counter-clockwise from the x-axis. The t-coordinate lies in the
 * angular direction measured down from the north pole towards the south
 * pole.
 *
 * A special ivar controls how the s-coordinate is generated. If PreventSeam
 * is set to true, the s-texture varies from 0->1 and then 1->0 (corresponding
 * to angles of 0->180 and 180->360).
 *
 * @warning
 * The resulting texture coordinates will lie between (0,1), and the texture
 * coordinates are determined with respect to the modeler's x-y-z coordinate
 * system. Use the class vtkTransformTextureCoords to linearly scale and
 * shift the origin of the texture coordinates (if necessary).
 *
 * @sa
 * vtkTextureMapToPlane vtkTextureMapToCylinder
 * vtkTransformTexture vtkThresholdTextureCoords
 */

#ifndef vtkTextureMapToSphere_h
#define vtkTextureMapToSphere_h

#include "vtkDataSetAlgorithm.h"
#include "vtkFiltersTextureModule.h" // For export macro

class vtkDataSet;

class VTKFILTERSTEXTURE_EXPORT vtkTextureMapToSphere : public vtkDataSetAlgorithm
{
public:
  vtkTypeMacro(vtkTextureMapToSphere, vtkDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Create object with Center (0,0,0) and the PreventSeam ivar is set to
   * true. The sphere center is automatically computed.
   */
  static vtkTextureMapToSphere* New();

  //@{
  /**
   * Specify a point defining the center of the sphere.
   */
  vtkSetVector3Macro(Center, double);
  vtkGetVectorMacro(Center, double, 3);
  //@}

  //@{
  /**
   * Turn on/off automatic sphere generation. This means it automatically
   * finds the sphere center.
   */
  vtkSetMacro(AutomaticSphereGeneration, vtkTypeBool);
  vtkGetMacro(AutomaticSphereGeneration, vtkTypeBool);
  vtkBooleanMacro(AutomaticSphereGeneration, vtkTypeBool);
  //@}

  //@{
  /**
   * Control how the texture coordinates are generated. If PreventSeam is
   * set, the s-coordinate ranges from 0->1 and 1->0 corresponding to the
   * theta angle variation between 0->180 and 180->0 degrees. Otherwise, the
   * s-coordinate ranges from 0->1 between 0->360 degrees.
   */
  vtkSetMacro(PreventSeam, vtkTypeBool);
  vtkGetMacro(PreventSeam, vtkTypeBool);
  vtkBooleanMacro(PreventSeam, vtkTypeBool);
  //@}

  /*
   * Computes the center point of the data set if AutomaticSphereGeneration is set to true.
   */
  virtual void ComputeCenter(vtkDataSet* input);

protected:
  vtkTextureMapToSphere();
  ~vtkTextureMapToSphere() override {}

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  double Center[3];
  vtkTypeBool AutomaticSphereGeneration;
  vtkTypeBool PreventSeam;

private:
  vtkTextureMapToSphere(const vtkTextureMapToSphere&) = delete;
  void operator=(const vtkTextureMapToSphere&) = delete;
};

#endif
