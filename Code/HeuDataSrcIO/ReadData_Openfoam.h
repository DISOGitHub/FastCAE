#ifndef READDATAOPENFOAM_H
#define READDATAOPENFOAM_H
#include "global_define.h"
#include "qstring.h"
#include "qtimer.h"
#include "ReadData_Base.h"
#include "vtkOpenFOAMReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkRectilinearGrid.h"
#include "vtkUnstructuredGrid.h"
class ReadData_OpenFoam :public ReadData_Base
{
public:
	bool Read(QString tep_filename) override;
	ReadData_OpenFoam();
	~ReadData_OpenFoam();
private:
	vtkOpenFOAMReader* mOpenFoamReader;

};

#endif // HEUDATAREADVTK_H
