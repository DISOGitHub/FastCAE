#ifndef READDATAPARTICLE_H
#define READDATAPARTICLE_H
#include "global_define.h"
#include "qstring.h"
#include "qtimer.h"
#include "ReadData_Base.h"

#include "vtkDataSetReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkRectilinearGrid.h"
#include "vtkUnstructuredGrid.h"
class ReadData_Particle :public ReadData_Base
{
public:
	bool Read(QString tep_filename) override;
	ReadData_Particle();
	~ReadData_Particle();
private:
	//vtkDataSetReader* mVtkDataSetReader;
};

#endif // HEUDATAREADPARTICLE_H
