#ifndef CNTMREADER_H_
#define CNTMREADER_H_

#include "ReadData_Base.h"
#include <QString>
#include <QHash>

class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;
class QTextStream;

class  ReadeData_Cntm : public ReadData_Base
{
public:
	ReadeData_Cntm() = default;
	~ReadeData_Cntm();

	bool Read(QString fileName) override;

private:
	QString readLine();
	bool readNodes();
	bool readElements();

private:
	QTextStream* _stream{};

	int _nodeNum{ 0 };
	int _cellNum{ 0 };
	QHash<int, int> _nodeIdIndex{};

	vtkUnstructuredGrid* _gird{};

};


#endif