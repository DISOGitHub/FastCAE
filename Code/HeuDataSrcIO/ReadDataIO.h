//#pragma execution_character_set("utf-8")
#ifndef READDATAIO_H
#define READDATAIO_H
#include "heudatasrcio_global.h"
#include "ReadData_Vtk.h"
#include "ReadData_MooringLine.h"
#include "qstring.h"
#include "qfileinfo.h"
#include <qlist.h>
#include <QVector>
#include "qmap.h"
#include "qdebug.h"
#include "qdir.h"
#include <vtkAutoInit.h>
#include "vtkActor.h"
#include "vtkActorCollection.h"
#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "vtkCommand.h"
#include "vtkCamera.h"
#include "vtkCellType.h"
#include "vtkCellData.h"
//using namespace std;

class  HEUDATASRCIOSHARED_EXPORT ReadDataIO
{
public:
	ReadDataIO();
	~ReadDataIO();
	bool LoadDataSource(QString pipelineObj_filaName);
	bool LoadFile(QString tep_filename); 
	vtkDataSet *getDataSet();
	vtkMultiBlockDataSet *getBlockDataSet();
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkIdTypeArray>>> getDataSetMap();
	QMap<vtkDataSet*, QString> getDataSetNameMap();
	int getNumberOfCgnsTimes();//cgns数据值有多少个时刻
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> getCgnsValue(int time);//获取第几个时刻的数据,第一个时刻为0
	Def_GridType getGridType();
	
	//add for mooringline
	int get_num_mooringline();
	ReadData_Base* mVtkReader{ nullptr };
private:
	QString mFilePath;
	//ReadData_Base* mVtkReader{ nullptr };
	//add for mooringLine
	int num_mooringLine;
};

#endif // HEUDATASRCIO_H
