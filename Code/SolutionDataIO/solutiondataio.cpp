#include "solutiondataio.h"

SolutionDataIO::SolutionDataIO()
{
	tep_grid = NULL;
	tep_zoneGrid = NULL;
	zoneCell_map.clear();
	blockDataSet = NULL;
}

SolutionDataIO::~SolutionDataIO()
{

}

bool SolutionDataIO::read_Points(int num_points,vtkUnstructuredGrid* tep_grid1)
{
	if (tep_grid1 == NULL)
		return false;
	int tep_num_points = num_points;
	if (tep_num_points<1)
	{
		rFile.close();
		return false;
	}
	vtkPoints *points = vtkPoints::New();
	QString line_str, tep_str;
	QStringList tep_strList;
	bool ok;
	while (tep_num_points)
	{
		if (rFile.atEnd())
			break;
		line_str = rFile.readLine().trimmed();
		if (line_str == "")
			continue;
		tep_strList = line_str.split(" ", QString::SkipEmptyParts);
		if (tep_strList.count() == 3)
		{
			points->InsertNextPoint(tep_strList.at(0).toDouble(&ok), tep_strList.at(1).toDouble(&ok), tep_strList.at(2).toDouble(&ok));

		}
		tep_num_points--;
	}
	if (points->GetNumberOfPoints() == num_points)
	{
		tep_grid1->SetPoints(points);
		return true;
	}
	else
		return false;
}

bool SolutionDataIO::read_Cells(int num_cells, vtkUnstructuredGrid* tep_grid1)
{
	if (tep_grid1 == NULL)
		return false;
	int tep_num_cells = num_cells;
	if (tep_num_cells < 1)
		return false;
	QString line_str, tep_str;
	QStringList tep_strList;
	bool ok;
	while (tep_num_cells)
	{
		if (rFile.atEnd())
			break;
		line_str = rFile.readLine().trimmed();
		if (line_str == "")
			continue;
		tep_strList = line_str.split(" ", QString::SkipEmptyParts);
		if (tep_strList.count() >1)
		{
			int cell_type = tep_strList.at(0).toInt(&ok);
			vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
			for (int i = 1; i < tep_strList.count(); i++)
				idList->InsertNextId(tep_strList.at(i).toInt(&ok));
			tep_grid1->InsertNextCell(cell_type, idList);

		}
		tep_num_cells --;
	}
	if (tep_grid1->GetNumberOfCells() == num_cells)
		return true;
	else
		return false;
}

bool SolutionDataIO::read_scalarPValue(int num_scalarPVal, QString name, vtkUnstructuredGrid* tep_grid1)
{
	if (tep_grid1 == NULL)
		return false;
	int tep_num_scalarPVal=num_scalarPVal;
	if (tep_num_scalarPVal != cur_num_points)
		return false;
	vtkFloatArray* tep_arr = vtkFloatArray::New();
	tep_arr->SetName(name.toStdString().data());
	tep_arr->SetNumberOfComponents(1);
	QString line_str, tep_str;
	QStringList tep_strList;
	bool ok;
	while (tep_num_scalarPVal)
	{
		if (rFile.atEnd())
			break;
		line_str = rFile.readLine().trimmed();
		if (line_str == "")
			continue;
		tep_strList = line_str.split(" ", QString::SkipEmptyParts);
		if (tep_strList.count()==1)
		{
			tep_arr->InsertNextTuple1(tep_strList.at(0).toDouble(&ok));
		}
		tep_num_scalarPVal--;
	}
	if (tep_arr->GetNumberOfTuples() == cur_num_points)
	{
		tep_grid1->GetPointData()->AddArray(tep_arr);
		return true;
	}
	else
		return false;
}

bool SolutionDataIO::read_scalarCValue(int num_scalarCVal, QString name, vtkUnstructuredGrid* tep_grid1)
{
	if (tep_grid1 == NULL)
		return false;
	int tep_num_scalarCVal=num_scalarCVal;
	if (tep_num_scalarCVal != cur_num_cells)
		return false;
	vtkFloatArray* tep_arr = vtkFloatArray::New();
	tep_arr->SetName(name.toStdString().data());
	tep_arr->SetNumberOfComponents(1);
	QString line_str, tep_str;
	QStringList tep_strList;
	bool ok;
	while (tep_num_scalarCVal)
	{
		if (rFile.atEnd())
			break;
		line_str = rFile.readLine().trimmed();
		if (line_str == "")
			continue;
		tep_strList = line_str.split(" ", QString::SkipEmptyParts);
		if (tep_strList.count() == 1)
		{
			tep_arr->InsertNextTuple1(tep_strList.at(0).toDouble(&ok));
		}
		tep_num_scalarCVal--;
	}
	if (tep_arr->GetNumberOfTuples() == cur_num_cells)
	{
		tep_grid1->GetCellData()->AddArray(tep_arr);
		return true;
	}
	else
		return false;
}

bool SolutionDataIO::read_normalPValue(int num_scalarPVal, QString name, vtkUnstructuredGrid* tep_grid1)
{
	if (tep_grid1 == NULL)
		return false;
	int tep_num_scalarPVal = num_scalarPVal;
	if (tep_num_scalarPVal != cur_num_points)
		return false;
	vtkFloatArray* tep_arr = vtkFloatArray::New();
	tep_arr->SetName(name.toStdString().data());
	tep_arr->SetNumberOfComponents(3);
	QString line_str, tep_str;
	QStringList tep_strList;
	bool ok;
	while (tep_num_scalarPVal)
	{
		if (rFile.atEnd())
			break;
		line_str = rFile.readLine().trimmed();
		if (line_str == "")
			continue;
		tep_strList = line_str.split(" ", QString::SkipEmptyParts);
		if (tep_strList.count() == 3)
		{
			tep_arr->InsertNextTuple3(tep_strList.at(0).toDouble(&ok), tep_strList.at(1).toDouble(&ok), tep_strList.at(2).toDouble(&ok));
		}
		tep_num_scalarPVal--;
	}
	if (tep_arr->GetNumberOfTuples() == cur_num_points)
	{
		tep_grid1->GetPointData()->AddArray(tep_arr);
		return true;
	}
	else
		return false;
}

bool SolutionDataIO::read_normalCValue(int num_scalarCVal, QString name, vtkUnstructuredGrid* tep_grid1)
{
	if (tep_grid1 == NULL)
		return false;
	int tep_num_scalarCVal = num_scalarCVal;
	if (tep_num_scalarCVal != cur_num_cells)
		return false;
	vtkFloatArray* tep_arr = vtkFloatArray::New();
	tep_arr->SetName(name.toStdString().data());
	tep_arr->SetNumberOfComponents(3);
	QString line_str, tep_str;
	QStringList tep_strList;
	bool ok;
	while (tep_num_scalarCVal)
	{
		if (rFile.atEnd())
			break;
		line_str = rFile.readLine().trimmed();
		if (line_str == "")
			continue;
		tep_strList = line_str.split(" ", QString::SkipEmptyParts);
		if (tep_strList.count() == 3)
		{
			tep_arr->InsertNextTuple3(tep_strList.at(0).toDouble(&ok), tep_strList.at(1).toDouble(&ok), tep_strList.at(2).toDouble(&ok));
		}
		tep_num_scalarCVal--;
	}
	if (tep_arr->GetNumberOfTuples() == cur_num_cells)
	{
		tep_grid1->GetCellData()->AddArray(tep_arr);
		return true;
	}
	else
		return false;
}

bool SolutionDataIO::read_ZonesCellID(int num_Ids, QString name)
{
	QString line_str;
	QStringList tep_strList;
	vtkSmartPointer<vtkIdTypeArray> tep_IdArr = vtkSmartPointer<vtkIdTypeArray>::New();
	QString tep_arrName = name; tep_arrName += "_cell000";
	tep_IdArr->SetName(tep_arrName.toStdString().data());
	tep_IdArr->SetNumberOfComponents(1);
	int div = num_Ids / Num_ZoneId_Per_Row;
	int mod = num_Ids % Num_ZoneId_Per_Row;
	if (mod)
		div++;
	bool ok;
	while (div)
	{
		if (rFile.atEnd())
			break;
		line_str = rFile.readLine().trimmed();
		if (line_str == "")
		{
			continue;
		}
		tep_strList = line_str.split(" ", QString::SkipEmptyParts);
		for (int k = 0; k < tep_strList.count(); k++)
		{
			tep_IdArr->InsertNextTuple1(tep_strList.at(k).toInt(&ok));
		}
		div--;
	}
	if (tep_IdArr->GetNumberOfTuples() == num_Ids)
	{
		zoneCell_map.insert(name, tep_IdArr);
		return true;
	}
	else
		return false;
}

//bool SolutionDataIO::read_blockDataSet(vtkUnstructuredGrid* tep_grid1)
//{
//	QStringList tep_list = range.split(rangeLine, QString::SkipEmptyParts);
//	if (tep_list.count() != 2)
//		return false;
//	bool ok;
//	int startPos = tep_list.at(0).toInt(&ok);
//	int endPos = tep_list.at(1).toInt(&ok);
//
//	return true;
//}

vtkDataSet* SolutionDataIO::readSolutionData(QString fileName)
{
	//QFile file(fileName);
	vtkDataSet* tep_dataSet = NULL;
	rFile.setFileName(fileName);
	if (!rFile.open(QIODevice::ReadOnly))
		return tep_dataSet;
	QString line_str;
	QStringList tep_strList;
	tep_grid = vtkUnstructuredGrid::New();
	bool ok;
	while (!rFile.atEnd())
	{
		line_str = rFile.readLine().trimmed();
		if ((line_str == "") || (line_str.left(1) == "#"))
			continue;
		tep_strList = line_str.split(" ", QString::SkipEmptyParts);
		if ((tep_strList.at(0).trimmed() == pointsID) || (tep_strList.at(0).trimmed() == zonePointsID))
		{
			if (tep_strList.count() < 2)
			{
				rFile.close();
				return tep_dataSet;
			}
			cur_num_points = tep_strList.at(1).toInt(&ok);
			if (tep_strList.at(0).trimmed() == pointsID)
			{
				if (!read_Points(cur_num_points, tep_grid))
					return tep_dataSet;
			}
			else
			{
				if (!read_Points(cur_num_points, tep_zoneGrid))
					return tep_dataSet;
			}
		}
		else if ((tep_strList.at(0).trimmed() == cellsID) || (tep_strList.at(0).trimmed() == zoneCellsID))
		{
			if (tep_strList.count() < 2)
			{
				rFile.close();
				return tep_dataSet;
			}
			cur_num_cells = tep_strList.at(1).toInt(&ok);
			if (tep_strList.at(0).trimmed() == cellsID)
				read_Cells(cur_num_cells, tep_grid);
			else 
				read_Cells(cur_num_cells, tep_zoneGrid);
		}
		else if ((tep_strList.at(0).trimmed() == pScalarValID) || (tep_strList.at(0).trimmed() == pZoneScalarValID))
		{
			if (tep_strList.count() < 3)
			{
				rFile.close();
				return tep_dataSet;
			}
			int tep_num_val = tep_strList.at(2).toInt(&ok);
			if (tep_strList.at(0).trimmed() == pScalarValID)
				read_scalarPValue(tep_num_val, tep_strList.at(1).trimmed(), tep_grid);
			else
				read_scalarPValue(tep_num_val, tep_strList.at(1).trimmed(), tep_zoneGrid);
		}
		else if ((tep_strList.at(0).trimmed() == pNormalValID) || (tep_strList.at(0).trimmed() == pZoneNormalValID))
		{
			if (tep_strList.count() < 2)
			{
				rFile.close();
				return tep_dataSet;
			}
			int tep_num_val = tep_strList.at(2).toInt(&ok);
			if (tep_strList.at(0).trimmed() == pNormalValID)
				read_normalPValue(tep_num_val, tep_strList.at(1).trimmed(), tep_grid);
			else
				read_normalPValue(tep_num_val, tep_strList.at(1).trimmed(), tep_zoneGrid);
		}
		else if ((tep_strList.at(0).trimmed() == cScalarValID) || (tep_strList.at(0).trimmed() == cZoneScalarValID))
		{
			if (tep_strList.count() < 2)
			{
				rFile.close();
				return tep_dataSet;
			}
			int tep_num_val = tep_strList.at(2).toInt(&ok);
			if (tep_strList.at(0).trimmed() == cScalarValID)
				read_scalarCValue(tep_num_val, tep_strList.at(1).trimmed(), tep_grid);
			else
				read_scalarCValue(tep_num_val, tep_strList.at(1).trimmed(), tep_zoneGrid);
		}
		else if ((tep_strList.at(0).trimmed() == cNormalValID) || (tep_strList.at(0).trimmed() == cZoneNormalValID))
		{
			if (tep_strList.count() < 2)
			{
				rFile.close();
				return tep_dataSet;
			}
			int tep_num_val = tep_strList.at(2).toInt(&ok);
			if (tep_strList.at(0).trimmed() == cNormalValID)
				read_normalCValue(tep_num_val, tep_strList.at(1).trimmed(), tep_grid);
			else
				read_normalCValue(tep_num_val, tep_strList.at(1).trimmed(), tep_zoneGrid);
		}
		else if (tep_strList.at(0).trimmed() == zoneCellIDList)
		{
			if (tep_strList.count() < 2)
			{
				rFile.close();
				return tep_dataSet;
			}
			QString zoneName = tep_strList.at(1);
			int num_ids = tep_strList.at(2).toInt(&ok);
			read_ZonesCellID(num_ids, zoneName);
		}
		else if (tep_strList.at(0).trimmed()==zoneStartID)
		{
			tep_zoneGrid = vtkUnstructuredGrid::New();
		}
		else if (tep_strList.at(0).trimmed() == zoneEndID)
		{
			if (tep_zoneGrid != NULL)
			{
				vtkUnstructuredGrid* tep_grid2 = vtkUnstructuredGrid::New();
				tep_grid2->DeepCopy(tep_zoneGrid);
				vtkDataSet* tep_dts = vtkDataSet::SafeDownCast(tep_grid2);
				if (blockDataSet == NULL)
					blockDataSet = vtkMultiBlockDataSet::New();
				blockDataSet->SetBlock(blockDataSet->GetNumberOfBlocks(), tep_dts);
				tep_zoneGrid->Delete();
			}
			tep_zoneGrid = NULL;
		}
	}
	tep_dataSet = tep_grid;
	return tep_dataSet;
}

bool SolutionDataIO::writeSolutionData(QString fileName, vtkDataSet* dataSet)
{
	if (dataSet == NULL)
		return false;
	if (!write_Points(fileName, dataSet,pointsID))
		return false;
	if (!write_Cells(fileName, dataSet, cellsID))
		return false;
	if (!write_Points_Value(fileName, dataSet, " "))
		return false;
	if (!write_Cells_Value(fileName, dataSet, " "))
		return false;
	return true;
}

bool SolutionDataIO::write_Points(QString fileName, vtkDataSet* dataSet, QString strID)
{
	int num_points = dataSet->GetNumberOfPoints();
	if (num_points < 1)
		return false;
	QFile file(fileName);
	if (strID == pointsID)
	{
		if (!file.open(QIODevice::WriteOnly))
			return false;
	}
	else
	{
		if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
			return false;
	}
	QTextStream ts(&file);
	if (strID == pointsID)
	{
		//write points
		ts << "#Resolution Data Version 1.0" << endl;
		ts << "#ASCII" << endl;
	}
	ts << strID << Data_Interval << num_points << Data_Interval << floatType << endl;
	double coor[3]; QString line_str, tep_str;
	for (vtkIdType i = 0; i < num_points; i++)
	{
		dataSet->GetPoint(i, coor);
		line_str = tep_str.setNum(coor[0], 'g', 6).leftJustified(20, ' '); //line_str += Data_Interval;
		line_str += tep_str.setNum(coor[1], 'g', 6).leftJustified(20, ' '); //line_str += Data_Interval;
		line_str += tep_str.setNum(coor[2], 'g', 6);
		ts << line_str << endl;
	}
	file.close();
	return true;
}

bool SolutionDataIO::write_Points_Value(QString fileName, vtkDataSet* dataSet, QString strID)
{
	int num_points = dataSet->GetNumberOfPoints();
	if (num_points < 1)
		return false;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
		return false;
	QTextStream ts(&file);

	QString line_str, tep_str;
	vtkPointData* tep_pointData = dataSet->GetPointData();
	if (tep_pointData != NULL)
	{
		int tep_num = tep_pointData->GetNumberOfArrays();
		if (tep_num > 0)
		{
			for (vtkIdType i = 0; i < tep_num; i++)
			{
				if ((QString(tep_pointData->GetArrayName(i)) == "vtkBlockColors")
					|| (QString(tep_pointData->GetArrayName(i)) == "Axes_X")
					|| (QString(tep_pointData->GetArrayName(i)) == "Axes_Y")
					|| (QString(tep_pointData->GetArrayName(i)) == "Axes_Z"))
					continue;
				vtkFloatArray* tep_arr = vtkFloatArray::SafeDownCast(tep_pointData->GetArray(tep_pointData->GetArrayName(i)));
				if (tep_arr->GetNumberOfTuples() != num_points)
					continue;
				int tep_components = tep_arr->GetNumberOfComponents();
				if ((tep_components != 1) && (tep_components != 3))
					return false;
				//ts << strID << Data_Interval << tep_pointData->GetArrayName(i) << endl;
				if (tep_components == 1)
				{
					if (strID==zoneID)
						ts << pZoneScalarValID << Data_Interval << tep_pointData->GetArrayName(i) <<Data_Interval<<tep_arr->GetNumberOfTuples()<< endl;
					else
						ts << pScalarValID << Data_Interval << tep_pointData->GetArrayName(i) << Data_Interval << tep_arr->GetNumberOfTuples() << endl;
					for (vtkIdType i = 0; i < tep_arr->GetNumberOfTuples(); i++)
					{
						line_str = tep_str.setNum(tep_arr->GetTuple1(i), 'g', 6);
						ts << line_str << endl;
					}
				}
				else if (tep_components == 3)
				{
					if (strID == zoneID)
						ts << pZoneNormalValID << Data_Interval << tep_pointData->GetArrayName(i) << Data_Interval << tep_arr->GetNumberOfTuples() << endl;
					else
						ts << pNormalValID << Data_Interval << tep_pointData->GetArrayName(i) << Data_Interval << tep_arr->GetNumberOfTuples() << endl;
					for (vtkIdType i = 0; i < tep_arr->GetNumberOfTuples(); i++)
					{
						double* tep_val = tep_arr->GetTuple3(i);
						line_str = tep_str.setNum(tep_val[0], 'g', 6).leftJustified(20,' ');
						line_str += tep_str.setNum(tep_val[1], 'g', 6).leftJustified(20, ' ');
						line_str += tep_str.setNum(tep_val[2], 'g', 6);
						ts << line_str << endl;
					}
				}
			}
		}
	}
	file.close();
	return true;
}

bool SolutionDataIO::write_Cells(QString fileName, vtkDataSet* dataSet, QString strID)
{
	//write cells
	int num_cells = dataSet->GetNumberOfCells();
	if (num_cells<1)
		return false;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
		return false;
	QTextStream ts(&file);
	ts << strID << Data_Interval << num_cells << endl;
	QString line_str, tep_str;
	for (vtkIdType i = 0; i < num_cells; i++)
	{
		vtkCell* tep_cell = dataSet->GetCell(i);
		int cell_type = tep_cell->GetCellType();
		line_str = tep_str.setNum(cell_type, 10).leftJustified(10, ' '); //line_str += Data_Interval;
		int tep_numCellPoint = tep_cell->GetNumberOfPoints();
		for (int j = 0; j < tep_numCellPoint; j++)
		{
			int tep_id = tep_cell->GetPointId(j);
			line_str += tep_str.setNum(tep_id, 10).leftJustified(10, ' ');
		}
		ts << line_str << endl;
	}
	file.close();
	return true;
}

bool SolutionDataIO::write_Cells_Value(QString fileName, vtkDataSet* dataSet, QString strID)
{
	int num_cells = dataSet->GetNumberOfCells();
	if (num_cells<1)
		return false;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
		return false;
	QTextStream ts(&file);
	vtkCellData* tep_cellData = dataSet->GetCellData();
	QString line_str, tep_str;
	if (tep_cellData != NULL)
	{
		int tep_num = tep_cellData->GetNumberOfArrays();
		if (tep_num > 0)
		{
			for (vtkIdType i = 0; i < tep_num; i++)
			{
				vtkFloatArray* tep_arr = vtkFloatArray::SafeDownCast(tep_cellData->GetArray(tep_cellData->GetArrayName(i)));
				if (tep_arr->GetNumberOfTuples() != num_cells)
					continue;
				int tep_components = tep_arr->GetNumberOfComponents();
				if ((tep_components != 1) && (tep_components != 3))
					return false;
				if (tep_components == 1)
				{
					if (strID == zoneID)
						ts << pZoneScalarValID << Data_Interval << tep_cellData->GetArrayName(i) << Data_Interval << tep_arr->GetNumberOfTuples() << endl;
					else
						ts << cScalarValID << Data_Interval << tep_cellData->GetArrayName(i) << Data_Interval << tep_arr->GetNumberOfTuples() << endl;
					for (vtkIdType i = 0; i < num_cells; i++)
					{
						line_str = tep_str.setNum(tep_arr->GetTuple1(i), 'g', 6);
						ts << line_str << endl;
					}
				}
				else if (tep_components == 3)
				{
					if (strID == zoneID)
						ts << pZoneNormalValID << Data_Interval << tep_cellData->GetArrayName(i) << Data_Interval << tep_arr->GetNumberOfTuples() << endl;
					else
						ts << cNormalValID << Data_Interval << tep_cellData->GetArrayName(i) << Data_Interval << tep_arr->GetNumberOfTuples() << endl;
					for (vtkIdType i = 0; i < num_cells; i++)
					{
						double* tep_val = tep_arr->GetTuple3(i);
						line_str = tep_str.setNum(tep_val[0], 'g', 6).leftJustified(20, ' '); 
						line_str += tep_str.setNum(tep_val[1], 'g', 6).leftJustified(20, ' '); 
						line_str += tep_str.setNum(tep_val[2], 'g', 6);
						ts << line_str << endl;
					}
				}
			}
		}
	}
	file.close();
	return true;
}

bool SolutionDataIO::write_Zones(QString fileName, QMap<QString, vtkSmartPointer<vtkIdTypeArray>> tep_zoneMap)
{
	//write cells
	int num_zones = tep_zoneMap.count();
	if (num_zones<1)
		return false;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
		return false;
	QTextStream ts(&file);
	QString line_str, tep_str;
	QMap<QString, vtkSmartPointer<vtkIdTypeArray>>::iterator it;
	for (it = tep_zoneMap.begin(); it != tep_zoneMap.end(); it++)
	{
		tep_str = it.key();
		vtkIdTypeArray* tep_zoneIdList=it.value();
		if (tep_zoneIdList == NULL)
			continue;
		int num_Ids = tep_zoneIdList->GetNumberOfTuples();
		if (num_Ids < 1)
			continue;
		ts << zoneCellIDList << Data_Interval << tep_str << Data_Interval << num_Ids << endl;
		int div, mod;
		div = num_Ids / Num_ZoneId_Per_Row;
		mod = num_Ids % Num_ZoneId_Per_Row;
		for (int i = 0; i < div; i++)
		{
			line_str = "";
			for (int j = 0; j < Num_ZoneId_Per_Row; j++)
			{
				line_str += tep_str.setNum(tep_zoneIdList->GetValue(i*Num_ZoneId_Per_Row+j),10);
				if (j < Num_ZoneId_Per_Row - 1)
					line_str += Data_Interval;
			}
			ts << line_str << endl;
		}
		line_str = "";
		for (int j = 0; j < mod;j++)
		{
			line_str += tep_str.setNum(tep_zoneIdList->GetValue(div*Num_ZoneId_Per_Row + j), 10);
			if (j < Num_ZoneId_Per_Row - 1)
				line_str += Data_Interval;
		}
		ts << line_str << endl;
	}
	file.close();
	return true;
}

bool SolutionDataIO::write_Zones(QString fileName, vtkMultiBlockDataSet* blockDataSet)
{
	if (blockDataSet == NULL)
		return false;
	int num_blocks = blockDataSet->GetNumberOfBlocks();
	if ( num_blocks< 1)
		return false;
	QString line_str, tep_str;
	for (int i = 0; i < num_blocks;i++)
	{
		if(!write_ZonesString(fileName, zoneStartID))
			return false;
		vtkDataSet* tep_dataSet = vtkDataSet::SafeDownCast(blockDataSet->GetBlock(i));
		if (!write_Points(fileName, tep_dataSet, zonePointsID))
		{
			if (!write_ZonesString(fileName, zoneEndID))
				return false;
			return false;
		}
		if(!write_Cells(fileName, tep_dataSet, zoneCellsID))
			return false;
		if (!write_Points_Value(fileName, tep_dataSet, zoneID))
			return false;
		if (!write_Cells_Value(fileName, tep_dataSet, zoneID))
			return false;
		if(!write_ZonesString(fileName, zoneEndID))
			return false;
	}
	return true;
}

bool SolutionDataIO::write_ZonesString(QString fileName, QString strID)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		file.close();
		return false;
	}
	QTextStream ts(&file);
	ts << strID << endl;
	file.close();
	return true;
}

