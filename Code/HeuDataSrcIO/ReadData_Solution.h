#ifndef READDATASOLUTION_H
#define READDATASOLUTION_H
#include "global_define.h"
#include "qstring.h"
#include "qtimer.h"
#include "ReadData_Base.h"
#include "solutiondataio.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkRectilinearGrid.h"
#include "vtkUnstructuredGrid.h"
class ReadData_Solution :public ReadData_Base
{
public:
	bool Read(QString tep_filename) override;
	ReadData_Solution();
	~ReadData_Solution();
	
private:
	SolutionDataIO* mSolutionReader;
};

#endif // HEUDATAREADSOLUTION_H
