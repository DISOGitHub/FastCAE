#include "ReadData_Obj.h"
#include "qdebug.h"
#include "vtkRectilinearGridToPointSet.h"
#include "vtkSmartPointer.h"
#include "vtkNew.h"
#include "qdebug.h"
#include "vtkFloatArray.h"
#include <QDateTime>
#include <QTime>
#include "vtkUnstructuredGrid.h"
#include "vtkCell.h"
#include "vtkCellData.h"

ReadData_Obj::ReadData_Obj()
{	
	model_import = vtkOBJImporter::New();
}

ReadData_Obj::~ReadData_Obj()
{
	if (model_import != NULL)
	{
		model_import->Delete();
		model_import = NULL;
	}
	
}

bool ReadData_Obj::Read(QString tep_filename)
{
	QString obj_name, mtl_name;
	obj_name =tep_filename;
	mtl_name = obj_name;
	mtl_name.replace(".obj", ".mtl");
	QFile fileobj(obj_name);
	QFile filemtl(mtl_name);
	if ((!fileobj.exists()) || (!filemtl.exists()))
	{
		return false;
	}
	QByteArray ba = QStringToChar(obj_name);
	model_import->SetFileName(ba.data());
	ba = QStringToChar(mtl_name);
	model_import->SetFileNameMTL(ba.data());
	model_import->SetRenderWindow(ren_win);
	model_import->Update();
	set_GridType(dUNSTRUCTURED_GRID);
	return true;
}

