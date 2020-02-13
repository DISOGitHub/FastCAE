#include "ReadData_Solution.h"
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

ReadData_Solution::ReadData_Solution()
{
	
	mSolutionReader = new SolutionDataIO;
}

ReadData_Solution::~ReadData_Solution()
{
	if (mSolutionReader != NULL)
	{
		delete mSolutionReader;
		mSolutionReader = NULL;
	}
	
}

bool ReadData_Solution::Read(QString tep_filename)
{
	fileName = tep_filename;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	dataSet=mSolutionReader->readSolutionData(tep_filename);
	blockDataSet = mSolutionReader->blockDataSet;
	if (mSolutionReader->zoneCell_map.count() > 0)
	{
		dataSetMap.insert(dataSet, mSolutionReader->zoneCell_map);
	}
	set_GridType(dUNSTRUCTURED_GRID);
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		return true;
	}
	return false;
}

