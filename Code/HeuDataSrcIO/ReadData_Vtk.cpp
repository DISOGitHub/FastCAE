#include "ReadData_Vtk.h"
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

ReadData_Vtk::ReadData_Vtk()
{
	
	/*mVtkDataSetReader =vtkDataSetReader::New();
	mVtkDataSetReader->ReadAllColorScalarsOn();
	mVtkDataSetReader->ReadAllVectorsOn();*/
	suffixName = "";
	mVtuReader = NULL;
	mVtrReader = NULL;
	mVtsReader = NULL;
	mVtpReader = NULL;
	mVtkDataSetReader = NULL;
}

ReadData_Vtk::~ReadData_Vtk()
{
	if (mVtkDataSetReader != NULL)
	{
		mVtkDataSetReader->Delete();
		mVtkDataSetReader = NULL;
	}
	if (mVtuReader != NULL)
	{
		mVtuReader->Delete();
		mVtuReader = NULL;
	}
	if (mVtsReader != NULL)
	{
		mVtsReader->Delete();
		mVtsReader = NULL;
	}
	if (mVtrReader != NULL)
	{
		mVtrReader->Delete();
		mVtrReader = NULL;
	}
	if (mVtpReader != NULL)
	{
		mVtpReader->Delete();
		mVtpReader = NULL;
	}
}

bool ReadData_Vtk::Read(QString tep_filename)
{
	if (suffixName == "vtk")
		ReadVtk(tep_filename);
	else if (suffixName == "vtu")
		ReadVtu(tep_filename);
	else if (suffixName == "vts")
		ReadVts(tep_filename);
	else if (suffixName == "vtr")
		ReadVtr(tep_filename);
	else if (suffixName == "vtp")
		ReadVtp(tep_filename);
	else
		return false;
}
bool ReadData_Vtk::ReadVtk(QString tep_filename)
{
	fileName = tep_filename;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	if (suffixName != "vtk")
		return false;
	if (mVtkDataSetReader == NULL)
	{
		mVtkDataSetReader = vtkDataSetReader::New();
		mVtkDataSetReader->ReadAllColorScalarsOn();
		mVtkDataSetReader->ReadAllVectorsOn();
	}
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
	dataSet = mVtkDataSetReader->GetOutput();
	if (dataSet == NULL)
		return false;
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		//ReadVarsList(dataSet);
		return true;
	}
	return false;
}
bool ReadData_Vtk::ReadVtu(QString tep_filename)
{
	fileName = tep_filename;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	if (suffixName != "vtu")
		return false;
	if (mVtuReader == NULL)
	{
		mVtuReader = vtkXMLUnstructuredGridReader::New();
	}
	mVtuReader->SetFileName(fileNamechar); mVtuReader->GlobalWarningDisplayOff();
	mVtuReader->Update();
	set_GridType(dUNSTRUCTURED_GRID);
	dataSet = mVtuReader->GetOutput();
	if (dataSet == NULL)
		return false;
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		//ReadVarsList(dataSet);
		return true;
	}
	return false;
}
bool ReadData_Vtk::ReadVts(QString tep_filename)
{
	fileName = tep_filename;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	if (suffixName != "vts")
		return false;
	if (mVtsReader == NULL)
	{
		mVtsReader = vtkXMLStructuredGridReader::New();
	}
	mVtsReader->SetFileName(fileNamechar); mVtsReader->GlobalWarningDisplayOff();
	mVtsReader->Update();
	set_GridType(dSTRUCTURED_GRID);
	dataSet = mVtsReader->GetOutput();
	if (dataSet == NULL)
		return false;
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		//ReadVarsList(dataSet);
		return true;
	}
	return false;
}
bool ReadData_Vtk::ReadVtr(QString tep_filename)
{
	fileName = tep_filename;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	if (suffixName != "vtr")
		return false;
	if (mVtrReader == NULL)
	{
		mVtrReader = vtkXMLRectilinearGridReader::New();
	}
	mVtrReader->SetFileName(fileNamechar); mVtrReader->GlobalWarningDisplayOff();
	mVtrReader->Update();
	set_GridType(dRECTILINEAR_GRID);
	dataSet = mVtrReader->GetOutput();
	if (dataSet == NULL)
		return false;
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		return true;
	}
	return false;
}
bool ReadData_Vtk::ReadVtp(QString tep_filename)
{
	fileName = tep_filename;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	if (suffixName != "vtp")
		return false;
	if (mVtpReader == NULL)
	{
		mVtpReader = vtkXMLPolyDataReader::New();
	}
	mVtpReader->SetFileName(fileNamechar); mVtpReader->GlobalWarningDisplayOff();
	mVtpReader->Update();
	set_GridType(dPolyData);
	dataSet =(vtkDataSet*) mVtpReader->GetOutput();
	if (dataSet == NULL)
		return false;
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		//ReadVarsList(dataSet);
		return true;
	}
	return false;
}
