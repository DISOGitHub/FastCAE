/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGeoProjection.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
/**
 * @class   vtkGeoProjection
 * @brief   Represent a projection from a sphere to a plane
 *
 *
 * This class uses the PROJ.4 library to represent geographic coordinate
 * projections.
 */

#ifndef vtkGeoProjection_h
#define vtkGeoProjection_h

#include "vtkGeovisCoreModule.h" // For export macro
#include "vtkObject.h"

struct PJconsts;
typedef struct PJconsts PJ;
typedef PJ* projPJ;

class VTKGEOVISCORE_EXPORT vtkGeoProjection : public vtkObject
{
public:
  static vtkGeoProjection* New();
  void PrintSelf(ostream& os, vtkIndent indent) override;
  vtkTypeMacro(vtkGeoProjection, vtkObject);

  /**
   * Returns the number of projections that this class offers.
   */
  static int GetNumberOfProjections();

  /**
   * Returns the name of one of the projections supported by this class.
   * You can pass these strings to SetName(char*).
   * @param projection the index of a projection, must be in [0,GetNumberOfProjections()[.
   */
  static const char* GetProjectionName(int projection);

  /**
   * Returns a description of one of the projections supported by this class.
   * @param projection the index of a projection, must be in [0,GetNumberOfProjections()[.
   */
  static const char* GetProjectionDescription(int projection);

  //@{
  /**
   * Set/get the short name describing the projection you wish to use.
   * This defaults to "latlong".
   * To get a list of valid values, use the GetNumberOfProjections() and
   * GetProjectionName(int) static methods.
   */
  vtkSetStringMacro(Name);
  vtkGetStringMacro(Name);
  //@}

  /**
   * Return the index of the current projection's type in the list of all projection types.
   * On error, this will return -1. On success, it returns a number in [0,GetNumberOfProjections()[.
   */
  int GetIndex();

  /**
   * Get the description of a projection.
   * This will return nullptr if the projection name is invalid.
   */
  const char* GetDescription();

  //@{
  /**
   * Set/get the longitude which corresponds to the central meridian of the projection.
   * This defaults to 0, the Greenwich Meridian.
   */
  vtkSetMacro(CentralMeridian, double);
  vtkGetMacro(CentralMeridian, double);
  //@}

  /**
   * Return a pointer to the PROJ.4 data structure describing this projection.
   * This may return nullptr if an invalid projection name or parameter set is specified.
   * If you invoke any methods on this vtkGeoProjection object, the PROJ.4 structure
   * this method returns may be freed, so you should not use the PROJ.4 structure
   * after changing any parameters. Also, you should not modify the PROJ.4 structure
   * on your own as it will then be out of sync with the vtkGeoProjection class.
   */
  projPJ GetProjection();

  /**
   * Add an optional parameter to the projection that will be computed or
   * replace it if already present.
   */
  void SetOptionalParameter(const char* key, const char* value);

  /**
   * Remove an optional parameter to the projection that will be computed
   */
  void RemoveOptionalParameter(const char*);

  /**
   * Return the number of optional parameters
   */
  int GetNumberOfOptionalParameters();

  /**
   * Return the number of optional parameters
   */
  const char* GetOptionalParameterKey(int index);

  /**
   * Return the number of optional parameters
   */
  const char* GetOptionalParameterValue(int index);

  /**
   * Clear all optional parameters
   */
  void ClearOptionalParameters();

  //@{
  /**
   * Set/Get/Clear projection string in PROJ.4 format.
   * This is a special case alternative to setting the projection name and
   * specifying parameters.
   *
   * \note If the PROJ4String is not empty, it supercedes the other parameters
   * and is used explicitly to instantiate the `projPJ` projection object.
   */
  vtkSetStringMacro(PROJ4String);
  vtkGetStringMacro(PROJ4String);
  //@}

protected:
  vtkGeoProjection();
  ~vtkGeoProjection() override;

  /**
   * Determine whether the current projection structure has any
   * changes pending and apply them if necessary.
   * Upon success, 0 is returned.
   * When an error occurs, a nonzero value is returned.
   */
  virtual int UpdateProjection();

  char* Name;
  double CentralMeridian;
  projPJ Projection;
  vtkMTimeType ProjectionMTime;
  char* PROJ4String;

private:
  vtkGeoProjection(const vtkGeoProjection&) = delete;
  void operator=(const vtkGeoProjection&) = delete;

  class vtkInternals;
  vtkInternals* Internals;
};

#endif // vtkGeoProjection_h
