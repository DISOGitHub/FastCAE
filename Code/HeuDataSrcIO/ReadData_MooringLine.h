#ifndef READMOORINGLINE_H
#define READMOORINGLINE_H
#include "global_define.h"
#include "qstring.h"
#include "qtimer.h"
#include "ReadData_Base.h"

#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkLine.h"
#include "vtkCellArray.h"
#define MAX_MOORING_PROP_COLUMN 10
class ReadData_MooringLine :public ReadData_Base
{
public:
	bool Read(QString tep_filename) override;
	ReadData_MooringLine();
	~ReadData_MooringLine();
	int get_num_mooringLines();
private:
	int num_mooringLine;

};

#endif // HEUDATAREADVTK_H
