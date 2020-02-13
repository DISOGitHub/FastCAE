#include "ReadData_Stl.h"
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

ReadData_Stl::ReadData_Stl()
{
	
	mVtkDataSetReader =vtkSTLReader::New();
}

ReadData_Stl::~ReadData_Stl()
{
	if (mVtkDataSetReader != NULL)
	{
		mVtkDataSetReader->Delete();
		mVtkDataSetReader = NULL;
	}
	
}

bool ReadData_Stl::Read(QString tep_filename)
{
	fileName = tep_filename;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	mVtkDataSetReader->SetFileName(fileNamechar); mVtkDataSetReader->GlobalWarningDisplayOff();
	mVtkDataSetReader->Update();
	set_GridType(dPolyData);
	dataSet= mVtkDataSetReader->GetOutput();
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		return true;
	}
	return false;
}

