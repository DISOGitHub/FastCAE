#include "ReadDataIO.h"
#include <QTextCodec>
#include <QDateTime>
#include <QTime>
#include "ReadData_Tecplot.h"
#include "ReadData_Cgns.h"
#include "ReadData_Solution.h"
#include "ReadData_Particle.h"
#include "ReadData_Igs.h"
#include "ReadData_Neu.h"
#include "ReadData_Stl.h"
#include "ReadData_Cntm.h"
#include "ReadData_ExodusII.h"
#include "vtkAppendFilter.h"

void getDataObject(vtkMultiBlockDataSet* md, QList<vtkDataObject*>& dataList)
{
    if (md == nullptr) return;
    const int n = md->GetNumberOfBlocks();
    for (int i = 0; i < n; ++i)
    {
        vtkDataObject*obj = md->GetBlock(i);
        if (obj == nullptr) continue;
        if (obj->IsA("vtkMultiBlockDataSet"))
        {
            vtkMultiBlockDataSet* mublock = vtkMultiBlockDataSet::SafeDownCast(obj);
            if (mublock != nullptr)
                getDataObject(mublock, dataList);
        }
        else
        {
            dataList.append(obj);
        }
    }
}


ReadDataIO::ReadDataIO()
{
	mVtkReader = NULL;
}

ReadDataIO::~ReadDataIO()
{
	if (mVtkReader != NULL)
	{
		delete mVtkReader;
		mVtkReader = NULL;
	}
}

inline float GetPdata(std::string& s)
{
	return std::atof(s.c_str());
}

vtkDataSet* ReadDataIO::getDataSet()
{
	if (mVtkReader->dataSet == NULL)
	{
		if (mVtkReader->blockDataSet != NULL)
		{
// 			int nblock = mVtkReader->blockDataSet->GetNumberOfBlocks();
// 			if (nblock > 0)
// 			{
// 				vtkAppendFilter* filter = vtkAppendFilter::New();
// 				for (int i = 0; i < nblock; ++i)
// 				{
// 					vtkDataSet* tep_dataset = vtkDataSet::SafeDownCast(mVtkReader->blockDataSet->GetBlock(i));
// 					filter->AddInputData(tep_dataset);
// 				}
// 				filter->Update();
// 				mVtkReader->set_GridType(dUNSTRUCTURED_GRID);
// 				vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
// 				tep_grid->DeepCopy(filter->GetOutput());
// 				mVtkReader->dataSet = tep_grid;
// 				filter->Delete();
// 				filter = NULL;			 
// 				mVtkReader->set_numPoints(mVtkReader->dataSet->GetNumberOfPoints());
// 				mVtkReader->set_numCells(mVtkReader->dataSet->GetNumberOfCells());
//			}
            QList<vtkDataObject*> objs;
            getDataObject(mVtkReader->blockDataSet, objs);
            vtkAppendFilter* filter = vtkAppendFilter::New();
            for (auto obj : objs)
                filter->AddInputData(obj);
            filter->Update();
            mVtkReader->set_GridType(dUNSTRUCTURED_GRID);
            vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
            tep_grid->DeepCopy(filter->GetOutput());
            mVtkReader->dataSet = tep_grid;
            filter->Delete();
            filter = NULL;
            mVtkReader->set_numPoints(mVtkReader->dataSet->GetNumberOfPoints());
            mVtkReader->set_numCells(mVtkReader->dataSet->GetNumberOfCells());
		}
		else if (getDataSetMap().count() >= 1)
		{
			mVtkReader->dataSet = getDataSetMap().firstKey();
		}
	}
	return mVtkReader->dataSet;  
}

vtkMultiBlockDataSet* ReadDataIO::getBlockDataSet()
{
	return mVtkReader->blockDataSet;
}

QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkIdTypeArray>>> ReadDataIO::getDataSetMap()
{
	return mVtkReader->dataSetMap;
}

QMap<vtkDataSet*, QString> ReadDataIO::getDataSetNameMap()
{
	return mVtkReader->dataSetNameMap;
}

int ReadDataIO::getNumberOfCgnsTimes()
{
	return mVtkReader->numberOfCgnsTimes;
}

QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> ReadDataIO::getCgnsValue(int time)
{
	return mVtkReader->getCgnsValue(time);
}

Def_GridType ReadDataIO::getGridType()
{
	return mVtkReader->get_GridType();
}


bool ReadDataIO::LoadFile(QString tep_filename)
{
	//char* fileName;
	QString errmsg = "succeed";
	if (tep_filename.isEmpty())
	{
		errmsg = QStringLiteral("fileName is empty，please select file again!");
		return false;
	}
	QFile file(tep_filename);
	if (file.exists()==false)
	{
		errmsg=QStringLiteral("file not exist!");
		return false;
	}
	//更改中文路径乱码问题
	//fileName = QTextCodec::codecForName("gb18030")->fromUnicode(tep_filename).data();
	QString suffix = QFileInfo(tep_filename).suffix();
	suffix = suffix.toLower();
	QString currentStr;
	if ((suffix == "vtk") || (suffix == "vtu") || (suffix == "vtr") || (suffix == "vts") || (suffix == "vtp"))
	{
		mVtkReader = new ReadData_Vtk;
		((ReadData_Vtk*)mVtkReader)->setSuffixName(suffix);
	}
	else if (suffix == "dat" || suffix == "pre" || suffix == "hot")
	{
		QFile file(tep_filename);
		if (!file.exists())
			return false;
		if (!file.open(QIODevice::ReadOnly))
		{
			return false;
		}
		QString tepstr;
		while (!file.atEnd())
		{
			tepstr = file.readLine().trimmed();
			QStringList tep_list;
			if (tepstr.toLower().indexOf("peridynamics simulation") >= 0) 
			{
				
					mVtkReader = new ReadData_Particle;
					tepstr = "Particles";
					break;
			}
			else if (tepstr.toLower().indexOf("variables") >= 0)
				break;
			/*if (tepstr.toLower().indexOf("variables") >= 0)
			{
				tep_list = tepstr.split(",");
				if (tep_list.count() == 2)
				{
					mVtkReader = new ReadData_Particle;
					tepstr = "Particles";
				}
				else if (tep_list.count() > 2)
				{
					QString str2 = tep_list.at(2).trimmed().toLower();
					if (str2.indexOf("z")<0)
					{
						mVtkReader = new ReadData_Particle;
						tepstr = "Particles";
					}
				}
				break;
			}*/
		}
		file.close();
		if ((tepstr != "Particles") && (tepstr != "PostProcessTecPlot"))
			mVtkReader = new ReadData_Tecplot;
	}		
	else if (suffix == "cgns")
		mVtkReader = new ReadData_Cgns;
	else if (suffix == "moor")
		mVtkReader = new ReadData_MooringLine;
	else if ((suffix == "igs")||(suffix=="iges"))
		mVtkReader = new ReadData_Igs;
	else if (suffix == "sol")
		mVtkReader = new ReadData_Solution;
	else if (suffix == "neu")
		mVtkReader = new ReadData_Neu;
	else if (suffix == "stl")
		mVtkReader = new ReadData_Stl;
    else if (suffix == "cntm")
        mVtkReader = new ReadeData_Cntm;
    else if (suffix == "e")
        mVtkReader = new ReadData_ExodusII;
	else
		return false;
	bool flag_read = mVtkReader->Read(tep_filename);
	if (flag_read)
	{
		if (suffix == "moor")
			num_mooringLine = ((ReadData_MooringLine*)mVtkReader)->get_num_mooringLines();
		return true;
	}
	else
	{
		errmsg = QObject::tr("read file failure!");
		delete mVtkReader;
		mVtkReader = nullptr;
		return false;
	}
}

bool ReadDataIO::LoadDataSource(QString pipelineObj_filaName)
{
	mFilePath = pipelineObj_filaName;
	return LoadFile(mFilePath);
}

//add for mooringline
int ReadDataIO::get_num_mooringline()
{
	return num_mooringLine;
}

