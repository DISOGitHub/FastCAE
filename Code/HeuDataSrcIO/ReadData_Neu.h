#ifndef READDATANEU_H
#define READDATANEU_H
#include "global_define.h"
#include "qstring.h"
#include "ReadData_Base.h"
#include "vtkDataSetReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkCellArray.h"
#include "vtkSmartPointer.h"
#include "qdebug.h"
#include "vtkFloatArray.h"
#include "vtkCell.h"
#include "vtkCellData.h"
#include "vtkQuad.h"
#include "vtkTriangle.h"

class ReadData_Neu :public ReadData_Base
{
public:
	ReadData_Neu();
	~ReadData_Neu();

	bool Read(QString tep_filename) override;

private:
	bool readBasicInfo();
	bool readPoints(vtkUnstructuredGrid* dateset);
	bool readElements(vtkUnstructuredGrid* dateset);
	QString readLine();
private:
	QString _fileName{};
	QTextStream *_stream{};
	int _pointNum{ -1 };
	int _eleNum{ -1 };
};
#endif // HEUDATAREADVTK_H
