/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkFeatureEdges.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkFeatureEdges
 * @brief   extract interior, boundary, non-manifold, and/or
 *          sharp edges from polygonal data
 *
 * vtkFeatureEdges is a filter to extract special types of edges from
 * input polygonal data. These edges are either 1) boundary (used by
 * one polygon) or a line cell; 2) non-manifold (used by three or more
 * polygons); 3) feature edges (edges used by two triangles and whose
 * dihedral angle > FeatureAngle); or 4) manifold edges (edges used by
 * exactly two polygons). These edges may be extracted in any
 * combination. Edges may also be "colored" (i.e., scalar values assigned)
 * based on edge type. The cell coloring is assigned to the cell data of
 * the extracted edges.
 *
 * @warning
 * To see the coloring of the lines you may have to set the ScalarMode
 * instance variable of the mapper to SetScalarModeToUseCellData(). (This
 * is only a problem if there are point data scalars.)
 *
 * @sa
 * vtkExtractEdges
 */

#ifndef vtkFeatureEdges_h
#define vtkFeatureEdges_h

#include "vtkFiltersCoreModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class vtkIncrementalPointLocator;

class VTKFILTERSCORE_EXPORT vtkFeatureEdges : public vtkPolyDataAlgorithm
{
public:
  //@{
  /**
   * Standard methods for type information and printing.
   */
  vtkTypeMacro(vtkFeatureEdges, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  //@}

  /**
   * Construct an instance with feature angle = 30; all types of edges
   * (except manifold edges) are extracted and colored.
   */
  static vtkFeatureEdges* New();

  //@{
  /**
   * Methods for turning the extraction of all types of edges on;
   * and turning the extraction of all types of edges off.
   */
  void ExtractAllEdgeTypesOn();
  void ExtractAllEdgeTypesOff();
  //@}

  //@{
  /**
   * Turn on/off the extraction of boundary edges.
   */
  vtkSetMacro(BoundaryEdges, bool);
  vtkGetMacro(BoundaryEdges, bool);
  vtkBooleanMacro(BoundaryEdges, bool);
  //@}

  //@{
  /**
   * Turn on/off the extraction of feature edges.
   */
  vtkSetMacro(FeatureEdges, bool);
  vtkGetMacro(FeatureEdges, bool);
  vtkBooleanMacro(FeatureEdges, bool);
  //@}

  //@{
  /**
   * Specify the feature angle for extracting feature edges.
   */
  vtkSetClampMacro(FeatureAngle, double, 0.0, 180.0);
  vtkGetMacro(FeatureAngle, double);
  //@}

  //@{
  /**
   * Turn on/off the extraction of non-manifold edges.
   */
  vtkSetMacro(NonManifoldEdges, bool);
  vtkGetMacro(NonManifoldEdges, bool);
  vtkBooleanMacro(NonManifoldEdges, bool);
  //@}

  //@{
  /**
   * Turn on/off the extraction of manifold edges. This typically
   * correspond to interior edges.
   */
  vtkSetMacro(ManifoldEdges, bool);
  vtkGetMacro(ManifoldEdges, bool);
  vtkBooleanMacro(ManifoldEdges, bool);
  //@}

  //@{
  /**
   * Turn on/off the coloring of edges by type.
   */
  vtkSetMacro(Coloring, bool);
  vtkGetMacro(Coloring, bool);
  vtkBooleanMacro(Coloring, bool);
  //@}

  //@{
  /**
   * Set / get a spatial locator for merging points. By
   * default an instance of vtkMergePoints is used.
   */
  void SetLocator(vtkIncrementalPointLocator* locator);
  vtkGetObjectMacro(Locator, vtkIncrementalPointLocator);
  //@}

  /**
   * Create default locator. Used to create one when none is specified.
   */
  void CreateDefaultLocator();

  /**
   * Return MTime also considering the locator.
   */
  vtkMTimeType GetMTime() override;

  //@{
  /**
   * Set/get the desired precision for the output point type. See the documentation
   * for the vtkAlgorithm::DesiredOutputPrecision enum for an explanation of
   * the available precision settings.
   */
  vtkSetMacro(OutputPointsPrecision, int);
  vtkGetMacro(OutputPointsPrecision, int);
  //@}

protected:
  vtkFeatureEdges();
  ~vtkFeatureEdges() override;

  // Usual data generation method
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  double FeatureAngle;
  bool BoundaryEdges;
  bool FeatureEdges;
  bool NonManifoldEdges;
  bool ManifoldEdges;
  bool Coloring;
  int OutputPointsPrecision;
  vtkIncrementalPointLocator* Locator;

private:
  vtkFeatureEdges(const vtkFeatureEdges&) = delete;
  void operator=(const vtkFeatureEdges&) = delete;
};

#endif
