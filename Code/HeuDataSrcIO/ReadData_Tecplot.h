#ifndef READDATATECPLOT_H
#define READDATATECPLOT_H

#include "ReadData_Base.h"

class vtkMultiBlockDataSet;
class vtkTecplotReader;
class vtkDataSet;
class QTextStream;

class ReadData_Tecplot : public ReadData_Base
{
public:
	ReadData_Tecplot() = default;
	~ReadData_Tecplot();
	bool Read(QString tep_filename) override;

private:
	void TecPreProcessing(QString fileName);
	void tecPostProcessing();
	void processingDataSet(vtkDataSet* d);
	bool isVectorComponent(QString name, QString& vecName, QString& comp);

	bool read1(QString fileName);
	QString readline();
	void readZone(QString zoneHead);

private:
	vtkMultiBlockDataSet* mudataset{};
	vtkTecplotReader* reader{};

	QTextStream* _textStream{};
	int _zoneNO{ 0 };
};


#endif
