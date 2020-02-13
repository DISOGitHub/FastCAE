#include "ReadData_Base.h"
#include "qdebug.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include <QDateTime>
#include <QTime>
#include "qtextcodec.h"

ReadData_Base::ReadData_Base()
{
	dataSet = NULL;
	blockDataSet = NULL;
	dataSetMap.clear();
	dataSetNameMap.clear();
	numberOfCgnsTimes = 0;
	mGridType = dUNSTRUCTURED_GRID;
}

void ReadData_Base::OutMessageFile(QString Msg)
{
	/*QString txt;
	QString strTime;
	strTime = QTime::currentTime().toString("hh:mm:ss:zzz");
	txt = QString("Release:" + strTime + "%1").arg(Msg);
	QFile outFile("./ReleaseLog.txt");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;*/
}

bool ReadData_Base::Read(QString)  // lobaojun add QString as a parameter in order to override in subclass
{
	return false;
}

QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> ReadData_Base::getCgnsValue(int time)
{
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> tep_map;
	return tep_map;
}

ReadData_Base::~ReadData_Base()
{
	
}

QByteArray ReadData_Base::QStringToChar(QString sourceStr)
{
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	QByteArray ba = codec->fromUnicode(sourceStr);
	return ba;
}


