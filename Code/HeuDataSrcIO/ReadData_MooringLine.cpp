#include "ReadData_MooringLine.h"
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
ReadData_MooringLine::ReadData_MooringLine()
{
	num_mooringLine = 0;
}

ReadData_MooringLine::~ReadData_MooringLine()
{
	
}

int ReadData_MooringLine::get_num_mooringLines()
{
	return num_mooringLine;
}

bool ReadData_MooringLine::Read(QString tep_filename)
{
	QFile file(tep_filename);
	if (!file.exists())
		return false;
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	int old_num_column=-1;
	int new_num_column =-1;
	int prop_num_column = 0;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkFloatArray> moor_colorData[MAX_MOORING_PROP_COLUMN];
	for (int i = 0; i < MAX_MOORING_PROP_COLUMN; i++)
	{
		moor_colorData[i] = NULL;
	}
	
	bool flag_new_line = true;
	QString tepstr;
	QStringList tep_list;
	vtkPolyData *polyData = vtkPolyData::New();
	polyData->SetPoints(points);
	polyData->SetLines(cells);
	dataSet = static_cast<vtkDataSet*>(polyData);
	bool ok;
	while (!file.atEnd())
	{
		tepstr = file.readLine().trimmed();
		if ((tepstr.indexOf("mooring line") >= 0)|| (tepstr.indexOf("the simulation time") >= 0)||(tepstr==""))
			continue;
		/*if (tepstr.indexOf("Number of Mooring Lines") >= 0)
		{
			tep_list = tepstr.split("=", QString::SkipEmptyParts);
			if (tep_list.count() == 2)
			{
				tep_list.at(1).trimmed().toInt(&ok);
			}
			else
				return false;
		}*/
		if (tepstr.indexOf("node#") >=0)
		{
			flag_new_line = true;
			num_mooringLine++;
			tep_list = tepstr.split(" ", QString::SkipEmptyParts);
			if (tep_list.count() < 4)
				continue;
			new_num_column = tep_list.count();
			if (old_num_column == -1)
				old_num_column = new_num_column;
			if (new_num_column != old_num_column)
				return false;
			prop_num_column = new_num_column - 4;
			for (int i = 0; i < prop_num_column; i++)
			{
				if (moor_colorData[i] == NULL)
				{
					moor_colorData[i] = vtkSmartPointer<vtkFloatArray>::New();
					moor_colorData[i]->SetName(tep_list.at(4+i).toLocal8Bit().data());
				}
			}			
		}
		else
		{
			tep_list = tepstr.split(" ", QString::SkipEmptyParts);
			if (tep_list.count() < 4)
				continue;
			if (tep_list.count() != new_num_column)
				return false;
			points->InsertNextPoint(tep_list.at(1).trimmed().toDouble(&ok), tep_list.at(2).trimmed().toDouble(&ok), tep_list.at(3).trimmed().toDouble(&ok));
			for (int i = 0; i < new_num_column - 4; i++)
			{
				if (moor_colorData[i]!=NULL)
					moor_colorData[i]->InsertNextTuple1(tep_list.at(i + 4).trimmed().toFloat(&ok));
			}
			if (flag_new_line)
				flag_new_line = false;
			else
			{
				vtkSmartPointer<vtkLine> tep_Line = vtkSmartPointer<vtkLine>::New();
				tep_Line->GetPointIds()->SetId(0, points->GetNumberOfPoints() - 2);
				tep_Line->GetPointIds()->SetId(1, points->GetNumberOfPoints() - 1);
				cells->InsertNextCell(tep_Line);
				//std::cout << "cell_id:   " << cells->GetNumberOfCells() << "  ( " << points->GetNumberOfPoints() - 2 << "  , " << points->GetNumberOfPoints() - 1 << "  ) " << std::endl;
			}
		}
	}
	file.close();
	for (int i = 0; i < prop_num_column; i++)
	{
		if (moor_colorData[i] != NULL)
		{
			dataSet->GetPointData()->AddArray(moor_colorData[i]);
		}
	}
	set_GridType(dUNSTRUCTURED_GRID);
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		//ReadVarsList(dataSet);
		return true;
	}
	else
		return false;
}

