#include "ReadData_Neu.h"

/*ReadData_Neu::ReadData_Neu()
{
	
	mVtkDataSetReader =vtkDataSetReader::New();
	mVtkDataSetReader->ReadAllColorScalarsOn();
	mVtkDataSetReader->ReadAllVectorsOn();
	//	mVtkDataSetReader->ReadAllFieldsOn();
}

ReadData_Neu::~ReadData_Neu()
{
	if (mVtkDataSetReader != NULL)
	{
		mVtkDataSetReader->Delete();
		mVtkDataSetReader = NULL;
	}
	
}

bool ReadData_Neu::Read(QString tep_filename)
{
	fileName = tep_filename;
	//qDebug() << "read fileName=  " << tep_filename << endl;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	mVtkDataSetReader->SetFileName(fileNamechar); mVtkDataSetReader->GlobalWarningDisplayOff();
	mVtkDataSetReader->Update();
	if (mVtkDataSetReader->IsFileUnstructuredGrid())
	{
		set_GridType(dUNSTRUCTURED_GRID);
	}
	else if (mVtkDataSetReader->IsFilePolyData())
	{
		set_GridType(dPolyData);
	}
	else if (mVtkDataSetReader->IsFileRectilinearGrid())
	{
		set_GridType(dRECTILINEAR_GRID);
	}
	else if (mVtkDataSetReader->IsFileStructuredGrid())
	{
		set_GridType(dSTRUCTURED_GRID);
	}
	dataSet= mVtkDataSetReader->GetOutput();
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		//ReadVarsList(dataSet);
		return true;
	}
	return false;
}*/
ReadData_Neu::ReadData_Neu()
{
}
ReadData_Neu::~ReadData_Neu()
{
	if (_stream) delete _stream;
}
QString ReadData_Neu::readLine()
{
	while (!_stream->atEnd())
	{
		QString line = _stream->readLine().simplified();
		if (line.isEmpty()) continue;
		return line;
	}
	return "";
}
bool ReadData_Neu::Read(QString tep_filename)
{
	_fileName = tep_filename;
	QFileInfo info(_fileName);
	if (!info.exists()) return false;
	QString name = info.fileName();
	QString path = info.filePath();
	QFile file(_fileName);
	if (!file.open(QIODevice::ReadOnly)) return false;
	_stream = new QTextStream(&file);
	if (!readBasicInfo()) return false;
	vtkUnstructuredGrid* tep_dataset = vtkUnstructuredGrid::New();
	if (!readPoints(tep_dataset))
	{
		tep_dataset->Delete();
		//			dataset = nullptr;
		return false;
	}
	if (!readElements(tep_dataset))
	{
		tep_dataset->Delete();
		return false;
	}
	file.close();

	dataSet = tep_dataset;
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		return true;
	}
	return false;
}
bool ReadData_Neu::readBasicInfo()
{
	QString line;
	while (!_stream->atEnd())
	{
		line = this->readLine();
		if (line.contains("NUMNP") && line.contains("NELEM"))
		{
			line = this->readLine();
			break;
		}
	}
	QStringList info = line.split(" ");
	_pointNum = info.at(0).toInt();
	_eleNum = info.at(1).toInt();
	return true;
}
bool ReadData_Neu::readPoints(vtkUnstructuredGrid* dateset)
{
	QString line;
	bool ok = false;
	while (!_stream->atEnd())
	{
		line = readLine();
		if (line.contains("NODAL COORDINATES")) break;
	}
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//		vtkPoints* points = vtkPoints::New();
	for (int i = 0; i < _pointNum; ++i)
	{
		line = readLine();
		double coor[3] = { 0 };
		QStringList scoor = line.split(" ");
		if (scoor.size() != 4) continue;
		for (int j = 0; j < 3; ++j)
		{
			coor[j] = scoor.at(j + 1).toDouble(&ok);
			if (!ok) return false;
		}
		points->InsertNextPoint(coor);
	}
	if (points->GetNumberOfPoints() != _pointNum)
	{
		points->Delete();
		return false;
	}
	dateset->SetPoints(points);
	return true;

}
bool ReadData_Neu::readElements(vtkUnstructuredGrid* dateset)
{
	QString line;
	while (!_stream->atEnd())
	{
		line = readLine();
		if (line.contains("ELEMENTS/CELLS")) break;
	}
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	for (int i = 0; i < _eleNum; ++i)
	{
		line = readLine();
		QStringList scell = line.split(" ");
		if (scell.size() == 7)
		{
			vtkSmartPointer<vtkQuad> cell = vtkSmartPointer<vtkQuad>::New();
			cell->GetPointIds()->SetId(0, scell[3].toInt() - 1);
			cell->GetPointIds()->SetId(1, scell[4].toInt() - 1);
			cell->GetPointIds()->SetId(2, scell[5].toInt() - 1);
			cell->GetPointIds()->SetId(3, scell[6].toInt() - 1);
			cells->InsertNextCell(cell);
		}
		else if (scell.size() == 6)
		{
			vtkSmartPointer<vtkTriangle> cell = vtkSmartPointer<vtkTriangle>::New();
			cell->GetPointIds()->SetId(0, scell[3].toInt() - 1);
			cell->GetPointIds()->SetId(1, scell[4].toInt() - 1);
			cell->GetPointIds()->SetId(2, scell[5].toInt() - 1);
			//		cell->GetPointIds()->SetId(3, scell[5].toInt()-1);
			cells->InsertNextCell(cell);
		}
		else continue;
		//			cells->InsertNextCell(cell);
	}
	if (cells->GetNumberOfCells() != _eleNum)
	{
		cells->Delete();
		return false;
	}
	dateset->SetCells(VTK_QUAD, cells);
	return true;
}
