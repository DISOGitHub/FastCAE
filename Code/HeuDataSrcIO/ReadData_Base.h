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
	vtkMultiBlockDataSet* blockDataSet{};//tecplot ������
	//QMap<QString, vtkSmartPointer<vtkIdTypeArray>> cgnsBlockDataSet;//һ����������Ӧ�Ķ�����
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkIdTypeArray>>> dataSetMap;//cgns�Ķ�������ļ�;,��һ��ʱ�̵�ֱֵ�Ӵ���������
	QMap<vtkDataSet*, QString> dataSetNameMap;//cgns�Ķ�������ʶ
	int numberOfCgnsTimes;//cgns����ֵ�ж��ٸ�ʱ��
	virtual QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> getCgnsValue(int time);//��ȡ�ڼ���ʱ�̵�����,��һ��ʱ��Ϊ0
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
	int mNodeCount;//�ڵ���	
	int mCellCount;//��Ԫ��
	void OutMessageFile(QString Msg);
};

#endif // HEUDATAREADERBASE_H
