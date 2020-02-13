#include "ReadData_Openfoam.h"
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

ReadData_OpenFoam::ReadData_OpenFoam()
{
	
	mOpenFoamReader = vtkOpenFOAMReader::New();
	//mOpenFoamReader->ReadAllColorScalarsOn();
	//mOpenFoamReader->ReadAllVectorsOn();
	//	mVtkDataSetReader->ReadAllFieldsOn();
}

ReadData_OpenFoam::~ReadData_OpenFoam()
{
	if (mOpenFoamReader != NULL)
	{
		mOpenFoamReader->Delete();
		mOpenFoamReader = NULL;
	}
	
}

bool ReadData_OpenFoam::Read(QString tep_filename)
{
	/*fileName = tep_filename;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	mVtkDataSetReader->SetFileName(fileNamechar);
	mVtkDataSetReader->Update();
	if (mVtkDataSetReader->IsFileUnstructuredGrid())
	{
		mGridType = dUNSTRUCTURED_GRID;
	}
	else if (mVtkDataSetReader->IsFilePolyData())
	{
		mGridType = dPolyData;
	}
	else if (mVtkDataSetReader->IsFileRectilinearGrid())
	{
		mGridType = dRECTILINEAR_GRID;
	}
	else if (mVtkDataSetReader->IsFileStructuredGrid())
	{
		mGridType = dSTRUCTURED_GRID;
	}
	dataSet= mVtkDataSetReader->GetOutput();
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		ReadVarsList();
		return true;
	}*/
	return false;
}

