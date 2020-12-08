#ifndef READDATABASE_H
#define READDATABASE_H

#include "heudatasrcio_global.h"
#include "qstring.h"
#include "qdebug.h"
#include "qdir.h"
#include "global_define.h"
#include "vtkDataSet.h"
#include "vtkSmartPointer.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkIdTypeArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
class ReadData_Base
{
public:
	vtkDataSet* dataSet;  
	vtkMultiBlockDataSet* blockDataSet{};//tecplot 多区域
	//QMap<QString, vtkSmartPointer<vtkIdTypeArray>> cgnsBlockDataSet;//一个网格所对应的多区域
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkIdTypeArray>>> dataSetMap;//cgns的多个网格文件;,第一个时刻的值直接存在网格里
	QMap<vtkDataSet*, QString> dataSetNameMap;//cgns的多个网格标识
	int numberOfCgnsTimes;//cgns数据值有多少个时刻
	virtual QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> getCgnsValue(int time);//获取第几个时刻的数据,第一个时刻为0
	ReadData_Base();
	~ReadData_Base();
	virtual bool Read(QString);// lobaojun add QString as a parameter in order to override in subclass
	QByteArray QStringToChar(QString sourceStr);	
	Def_GridType get_GridType(){ return mGridType; };
	void set_GridType(Def_GridType type){ mGridType = type; };
	void set_numCells(int number) { mCellCount = number; };
	void set_numPoints(int number) { mNodeCount = number; };
private:
	Def_GridType mGridType;
	
protected:	
	QString fileName;
	int mNodeCount;//节点数	
	int mCellCount;//单元数
	void OutMessageFile(QString Msg);
};

#endif // HEUDATAREADERBASE_H
