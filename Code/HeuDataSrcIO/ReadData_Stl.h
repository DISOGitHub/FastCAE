#ifndef READDATASTL_H
#define READDATASTL_H
#include "global_define.h"
#include "qstring.h"
#include "qtimer.h"
#include "ReadData_Base.h"

#include "vtkSTLReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkRectilinearGrid.h"
#include "vtkUnstructuredGrid.h"
class ReadData_Stl :public ReadData_Base
{
public:
	bool Read(QString tep_filename) override;
	ReadData_Stl();
	~ReadData_Stl();
private:
	vtkSTLReader* mVtkDataSetReader;
};

#endif // HEUDATAREADVTK_H
