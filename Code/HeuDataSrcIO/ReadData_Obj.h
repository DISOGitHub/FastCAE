#ifndef READDATAOBJ_H
#define READDATAOBJ_H
#include "global_define.h"
#include "qstring.h"
#include "qtimer.h"
#include "ReadData_Base.h"

#include "vtkOBJImporter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkRectilinearGrid.h"
#include "vtkUnstructuredGrid.h"
#include "vtkRenderWindow.h"
class ReadData_Obj :public ReadData_Base
{
public:
	bool Read(QString tep_filename) override;
	ReadData_Obj();
	~ReadData_Obj();
	void setRenderWindow(vtkRenderWindow* tep_ren_win) { ren_win = tep_ren_win; };
private:
	vtkOBJImporter* model_import;
	vtkRenderWindow* ren_win;
};

#endif // HEUDATAREADVTK_H
