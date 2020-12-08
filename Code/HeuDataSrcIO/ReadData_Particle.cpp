#include "ReadData_Particle.h"
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
#include "vtkPolyData.h"
#define MAX_PROPVAL_NUM 20
ReadData_Particle::ReadData_Particle()
{
}

ReadData_Particle::~ReadData_Particle()
{
}

bool ReadData_Particle::Read(QString tep_filename)
{
	fileName = tep_filename;
	QFile file(tep_filename);
	if (!file.exists())
		return false;
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkFloatArray> valData[MAX_PROPVAL_NUM];
	for (int i = 0; i < MAX_PROPVAL_NUM; i++)
	{
		valData[i] = NULL;
	}
	QString tepstr;
	QStringList tep_list;
	QStringList valList;
	vtkPolyData *polyData = vtkPolyData::New();
	polyData->SetPoints(points);
	//dataSet = static_cast<vtkDataSet*>(polyData);
	bool ok;
	int tep_num_column = 0;
	int num_xyz = 0;
	int num_val = 0;
	while (!file.atEnd())
	{
		tepstr = file.readLine().trimmed();
		if ((tepstr.indexOf("Title") >= 0) || (tepstr.indexOf("zone") >= 0) || (tepstr == ""))
			continue;
		if (tepstr.toLower().indexOf("variables") >= 0)
		{
			tep_list = tepstr.split("=", QString::SkipEmptyParts);
			if (tep_list.count() != 2)
				return false;
			tepstr = tep_list.at(1).trimmed();
			tep_list = tepstr.split(",", QString::SkipEmptyParts);
			tep_num_column = tep_list.count();
			if (tep_num_column < 2)
				return false;
			valList.clear();
			for (int i = 0; i < tep_num_column; i++)
			{
				if ((tep_list.at(i).toLower().trimmed() == "x") || (tep_list.at(i).toLower().trimmed() == "\"x\"")
					|| (tep_list.at(i).toLower().trimmed() == "y") || (tep_list.at(i).toLower().trimmed() == "\"y\"")
					|| (tep_list.at(i).toLower().trimmed() == "z") || (tep_list.at(i).toLower().trimmed() == "\"z\""))
				{
					num_xyz++;
					continue;
				}
				valList.append(tep_list.at(i));
				valData[num_val] = vtkSmartPointer<vtkFloatArray>::New();
				valData[num_val]->SetName(tep_list.at(i).toLocal8Bit().data());
				num_val++;
			}
			if (num_xyz < 2)
				return false;
			continue;
		}
		
		tep_list = tepstr.split(" ", QString::SkipEmptyParts);
		if (tep_list.count() != valList.count() + num_xyz)
			continue;
		if (num_xyz==2)
			points->InsertNextPoint(tep_list.at(0).trimmed().toDouble(&ok), tep_list.at(1).trimmed().toDouble(&ok),0);
		else if (num_xyz == 3)
			points->InsertNextPoint(tep_list.at(0).trimmed().toDouble(&ok), tep_list.at(1).trimmed().toDouble(&ok), tep_list.at(2).trimmed().toDouble(&ok));
		for (int i = 0; i < valList.count(); i++)
		{
			if (valData[i] != NULL)
				valData[i]->InsertNextTuple1(tep_list.at(i +num_xyz).trimmed().toFloat(&ok));
		}
	}
	file.close();
	for (int i = 0; i < valList.count(); i++)
	{
		if (valData[i] != NULL)
		{
			polyData->GetPointData()->AddArray(valData[i]);
		}
	}
	dataSet = static_cast<vtkDataSet*>(polyData);
	set_GridType(dPolyData);
	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = 0;
	if (mNodeCount > 0)
	{
		//ReadVarsList(dataSet);
		return true;
	}
	else
		return false;
}

