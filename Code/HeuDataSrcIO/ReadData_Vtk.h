#ifndef READDATAVTK_H
#define READDATAVTK_H
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
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkXMLRectilinearGridReader.h"
#include "vtkXMLPolyDataReader.h"
class ReadData_Vtk :public ReadData_Base
{
public:
	bool Read(QString tep_filename) override;
	ReadData_Vtk();
	~ReadData_Vtk();
	void setSuffixName(QString name) { suffixName = name; };
private:
	vtkDataSetReader* mVtkDataSetReader;
	vtkXMLUnstructuredGridReader *mVtuReader;
	vtkXMLStructuredGridReader *mVtsReader;
	vtkXMLRectilinearGridReader *mVtrReader;
	vtkXMLPolyDataReader *mVtpReader;
	QString suffixName;
	bool ReadVtk(QString tep_filename);
	bool ReadVtu(QString tep_filename);
	bool ReadVtr(QString tep_filename);
	bool ReadVts(QString tep_filename);
	bool ReadVtp(QString tep_filename);
};

#endif // HEUDATAREADVTK_H
