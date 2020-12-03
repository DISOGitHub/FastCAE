#include "INPFWriter.h"
#include "TFCouplingData.h"
#include "ThermalData.h"
#include "TFMaterial.h"
#include "TFThickness.h"
#include "ThermalCoupling.h"
#include "ThermalBoundary.h"
#include "ControlTemperature.h"
#include "BoundaryTabTypeData.h"
#include <QDebug>
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkDataSet.h>
#include <vtkCell.h>
#include <vtkIdList.h>

namespace ThermalFluid{
	INPFWriter::INPFWriter(QString path, TFCouplingModelData* data) :_modelData(data)
	{
		if (_modelData == nullptr)
			return;
		_thermalData = _modelData->getThermalData();
		if (_thermalData == nullptr)
			return;

		QString desPath = path + "/INPF";

		//qDebug() << desPath;
		
		_file = new QFile(desPath);
	}


	INPFWriter::~INPFWriter()
	{
		if (_file != nullptr)
			delete _file;

		if (_writeStream != nullptr)
			delete _writeStream;
		_writeStream = nullptr;
	}

	bool INPFWriter::write()
	{
		if (!_file->open(QIODevice::WriteOnly | QIODevice::Truncate))
			return false;
		
		_writeStream = new QTextStream(_file);
		//_writeStream->setCodec("")

		int kernalID = _modelData->getThermalData()->getMeshKernalID();
		MeshData::MeshKernal * meshKernal = MeshData::MeshData::getInstance()->getKernalByID(kernalID);
		if (meshKernal == nullptr)
		{
			qDebug() << "kernalID is wrong";
			return false;
		}
		vtkDataSet* grid = meshKernal->getMeshData();

		writePart1();
		writePart2();
		writePart3();
		writePart4(grid);
		writePart5(grid);
		writePart6();
		writePart7();
		writePart8();
		writePart9(kernalID);
		writePart10();

		_file->close();
		delete _file;
		_file = nullptr;

		return true;
	}


	void INPFWriter::writePart1()
	{
		QString s;
		QStringList units;
		*_writeStream << "$" << endl;
		*_writeStream << "$ UNITS 10   ";
		units.append(QString::number(_thermalData->getXLength(), 'E'));
		units.append(QString::number(_thermalData->getXFroce(), 'E'));
		units.append(QString::number(_thermalData->getXTemper(), 'E'));
		
		*_writeStream << units.join("  ") << endl << "$" << endl << "-1" << endl;
	}

	void INPFWriter::writePart2()
	{
		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writePart3()
	{
		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writePart4(vtkDataSet* grid)
	{		
		const int n = grid->GetNumberOfPoints();
		for (int index = 0; index < n; index++)
		{
 			double *coor = grid->GetPoint( index );
			*_writeStream << QString("%1 %2 %3 %4").arg(index + 1).arg(coor[0]).arg(coor[1]).arg(coor[2]) << endl;
		}
		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writePart5(vtkDataSet* grid)
	{		
		const int n = grid->GetNumberOfCells();		
		for ( int index = 0; index < n; index++ )
		{
			vtkCell* cell = grid->GetCell( index );
			if ( cell == nullptr ) continue;
			vtkIdList* ids = cell->GetPointIds();

			*_writeStream << index + 1
						  << " SURFACE "
						  << _thermalData->getMaterialIDFromHash( index + 1 )
						  << " "
						  << '0.0'
						  << " "
						  << _thermalData->getThicknessIDFromHash( index + 1 )
						  << " "
						  << ids->GetId(0) + 1
						  << " "
						  << ids->GetId(1) + 1;

			if ( ids->GetNumberOfIds() == 2 )
			{
				*_writeStream << endl;
			}
			else if ( ids->GetNumberOfIds() == 3 )
			{		
				*_writeStream << " "
							  << ids->GetId(2) + 1
							  << endl;
			}
			else if ( ids->GetNumberOfIds() == 4 )
			{				
				*_writeStream << " " 
							  << ids->GetId(2) + 1
							  << " "
							  << ids->GetId(3) + 1
							  << endl;
			}
		}	
		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writePart6()
	{
		int n = _thermalData->getThermalCouplingListCount();
		for (int i = 0; i < n; i++)
		{
			ThermalCoupling* tc = _thermalData->getThermalCouplingFromIndex(i);

			if (tc == nullptr)
				continue;

			QStringList list;
			list.append("$");
			list.append(QString::number(tc->getID()));
			list.append("-");
			list.append(tc->getName());
			
			*_writeStream << "$" << endl << list.join(" ") << endl << "$" << endl;

			QStringList datalist;
			datalist.append("AREA");
			datalist.append(tc->getGroupOne());
			datalist.append("0");
			datalist.append("0");
			datalist.append(tc->getGroupTwo());
			datalist.append("0 ");
			datalist.append(QString::number(tc->getXValue(),'E'));
			datalist.append(tc->couplingTypeToString(tc->getType()));

			*_writeStream << datalist.join(" ") << endl;
		}

		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writePart7()
	{
		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writePart8()
	{
		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writePart9(int kernalID)
	{
		

		QStringList units;
		units.append(QString::number(_thermalData->getYLength(), 'E'));
		units.append(QString::number(_thermalData->getYFroce(), 'E'));
		units.append(QString::number(_thermalData->getYTemper(), 'E'));

		*_writeStream << "PARAM UNITS 10   " << units.join("  ") << endl;

		writeThickness();
		writeMaterial();
		writeTableTypeData();
		writeThermalBoundary();
		writeControlTemperature();
		writeMeshGroups(kernalID);

		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writePart10()
	{
		*_writeStream << "-1" << endl;
	}

	void INPFWriter::writeThickness()
	{
		int n = _thermalData->getThicknessListCount();
		for (int i = 0; i < n; i++)
		{
			TFThickness* tn = _thermalData->getThicknessFromIndex(i);

			if (tn == nullptr)
				continue;

			*_writeStream << "$" << endl << "$ " << tn->getName() << endl << "$" << endl;

			*_writeStream << fillSpaceToString("PROP", 10) << QString::number(tn->getID()) << " ";

			*_writeStream << fillSpaceToString("SHELL", 9) << QString::number(tn->getValue(), 'E') << "  ";

			*_writeStream << "0.0 0.0 0.0 0.0  " << QString::number(0.0, 'E') << endl;
		}
		
	}

	void INPFWriter::writeMaterial()
	{
		int n = _thermalData->getMaterialListCount();
		for (int i = 0; i < n; i++)
		{
			TFMaterial* tm = _thermalData->getMaterialFromIndex(i);
			if (tm == nullptr)
				continue;

			*_writeStream << "$" << endl;
			QStringList tlist;
			QString id = QString::number(tm->getID());
			tlist.append("$");
			tlist.append(id);
			tlist.append("-");
			tlist.append(tm->getName());
			*_writeStream << tlist.join(" ") << endl << "$" << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("PHASE", 16) << fillSpaceToString(tm->getPropertyValueFromHash("PHASE"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("RHO", 16) << tm->getPropertyValueFromHash("RHO") << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("CPP", 16) << tm->getPropertyValueFromHash("CPP") << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("KTHERM", 16) << tm->getPropertyValueFromHash("KTHERM") << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("ELECRES", 16) << tm->getPropertyValueFromHash("ELECRES") << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("VISC", 16) << tm->getPropertyValueFromHash("VISC") << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("HTLAT", 16) << tm->getPropertyValueFromHash("HTLAT") << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("CAPAPH", 16) << tm->getPropertyValueFromHash("CAPAPH") << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("TPHASE", 16) << fillSpaceToString(tm->getPropertyValueFromHash("TPHASE"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("E", 16) << fillSpaceToString(tm->getPropertyValueFromHash("E"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("REMISS", 16) << fillSpaceToString(tm->getPropertyValueFromHash("REMISS"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("ABSORPTIVITY", 16) << fillSpaceToString(tm->getPropertyValueFromHash("ABSORPTIVITY"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("RABSOR", 16) << fillSpaceToString(tm->getPropertyValueFromHash("RABSOR"), 11, false) + "\n";
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("SPECULARITY", 16) << fillSpaceToString(tm->getPropertyValueFromHash("SPECULARITY"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("RSOLSPE", 16) << fillSpaceToString(tm->getPropertyValueFromHash("RSOLSPE"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("IRSPEC", 16) << fillSpaceToString(tm->getPropertyValueFromHash("IRSPEC"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("RIRSPEC", 16) << fillSpaceToString(tm->getPropertyValueFromHash("RIRSPEC"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("TRANSMISS", 16) << fillSpaceToString(tm->getPropertyValueFromHash("TRANSMISS"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("IRTRANS", 16) << fillSpaceToString(tm->getPropertyValueFromHash("IRTRANS"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("IREFFRONT", 16) << fillSpaceToString(tm->getPropertyValueFromHash("IREFFRONT"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("IREFBACK", 16) << fillSpaceToString(tm->getPropertyValueFromHash("IREFBACK"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("SOLAREXTINCT", 16) << fillSpaceToString(tm->getPropertyValueFromHash("SOLAREXTINCT"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("IREXTINCT", 16) << fillSpaceToString(tm->getPropertyValueFromHash("IREXTINCT"), 11, false) << endl;
			*_writeStream << fillSpaceToString("MAT", 9) << id << " " << fillSpaceToString("ABLAT", 16) << fillSpaceToString(tm->getPropertyValueFromHash("ABLAT"), 11, false) << endl;
		}

	}

	void INPFWriter::writeThermalBoundary()
	{
		int n = _thermalData->getThermalBoundaryListCount();
		for (int i = 0; i < n; i++)
		{
			ThermalBoundary* tb = _thermalData->getThermalBoundaryFromIndex(i);
			if (tb == nullptr)
				continue;

			*_writeStream << "$" << endl;
			QStringList title;
			title.append("$");
			title.append(QString::number(tb->getID()));
			title.append("-");
			title.append(tb->getName());
			*_writeStream << title.join(" ") << endl << "$" << endl;			

			QStringList type = tb->getThermalType();

			QStringList data;
			if (type.at(0) == "INTERP")
			{
				data.append(type.at(0));
				data.append(tb->getGroup()+" ");
				data.append(fillSpaceToString("0", 6));
				data.append(QString::number(tb->getTableTypeID()));
				data.append(fillSpaceToString(type.at(1), 6));
				data.append(QString::number(tb->getXValue(), 'E', 5));
				*_writeStream << data.join(" ") << endl;
			}
			else if (type.at(0) == "QNODE")
			{
				data.append(type.at(0));
				data.append(tb->getGroup() + " ");
				data.append(QString::number(tb->getXValue(), 'E'));
				data.append("CONSTANT");
				data.append(type.at(1));
				
				*_writeStream << data.join(" ") << endl;
			}
			else if(type.at(0) == "SINK")
			{
				data.append(type.at(0));
				data.append(tb->getGroup() + " ");					
				data.append(QString::number(tb->getXValue(), 'E'));
				data.append("CONSTANT");

				*_writeStream << data.join(" ") << endl;
				if (type.size() == 2)
					*_writeStream << type.at(1) << endl;
			}

		}
	}

	void INPFWriter::writeControlTemperature()
	{
		int n = _thermalData->getControlTemperatureListCount();
		for (int i = 0; i < n; i++)
		{
			ControlTemperature* tc = _thermalData->getControlTemperatureFromIndex(i);
			if (tc == nullptr)
				continue;

			*_writeStream << "$" << endl;
			QStringList title;
			title.append("$");
			title.append("1"/*QString::number(tc->getID())*/);
			title.append("-");
			title.append(tc->getName());
			*_writeStream << title.join(" ") << endl << "$" << endl;

			QStringList dataG;
			dataG.append("THERMST");
			dataG.append(fillSpaceToString(tc->getSGroup(), 7));
			dataG.append(fillSpaceToString(tc->getHGroup(), 7));
			*_writeStream << dataG.join(" ") << "  ";

			QStringList data;
			data.append(QString::number(tc->getLTemperature(), 'E'));
			data.append(QString::number(tc->getHTemperature(), 'E'));
			data.append(QString::number(tc->getXValue(), 'E'));
			*_writeStream << data.join("  ") << " " << _thermalData->thermalTypeToString(tc->getThermalType()) << endl;
		}
		
	}

	void INPFWriter::writeTableTypeData()
	{
		int n = _thermalData->getTabTypeDataListCount();
		for (int i = 0; i < n; i++)
		{
			BoundaryTabTypeData* tb = _thermalData->getTabTypeDataFromIndex(i);
			if (tb == nullptr)
				continue;

			
			*_writeStream << "$" << endl;
			QStringList list;
			list.append("$");
			QString id = QString::number(tb->getID());
			list.append(id);
			list.append("-");
			list.append(tb->getName());
			*_writeStream << list.join(" ") << endl << "$" << endl;

			QStringList type;
			type.append(id);
			type.append(tb->tabTypeToString(tb->getType()));
			type.append("TIME");
			*_writeStream << fillSpaceToString("TABTYPE", 14) << type.join(" ") << endl;
			writeTableData(tb, id);
		}	
	}

	void INPFWriter::writeMeshGroups(int kid)
	{
		auto meshData = MeshData::MeshData::getInstance();
		int n = meshData->getMeshSetCount();	
		*_writeStream << "$" << endl << "$ NAME cards" << endl << "$" << endl;
		for ( int index = 0; index < n; index++ )	
		{	
			auto aSet = meshData->getMeshSetAt( index );	
			writeAMeshGroup( aSet,kid );
		}
	}

	void INPFWriter::writeAMeshGroup(MeshData::MeshSet* aSet,int kid)
	{
		QStringList groupNameList = aSet->getName().split('(');			
		QString shortName = groupNameList.at(0);
		QString longName = groupNameList.at(1).split(')').at(0);
		*_writeStream << "NAME2 " << shortName << " " << longName << endl;

		QList<int> list = aSet->getKernalMembers(kid);
		
		qSort(list.begin(), list.end());
		QList<DataCut> dataList = CutList(list);

		for (DataCut aCut : dataList)
		{
			*_writeStream << "NAME " << shortName << " " << aCut.start + 1;
			if (aCut.end > 0)
				*_writeStream << " " << aCut.end + 1 << " " << aCut.step;
			*_writeStream << endl;
		}
	}

	QString INPFWriter::fillSpaceToString(QString s, int size, bool b)
	{
		QByteArray array = s.toLocal8Bit();
		int n =array.length();

		if (b)
		{
			for (int i = n; i < size; i++)
			{
				s += " ";
			}
		}
		else
		{
			QString l;
			for (int i = n; i < size; i++)
			{
				l += " ";
			}
			s = l + s;
		}

		return s;
	}

	void INPFWriter::writeTableData(BoundaryTabTypeData* tb, QString id)
	{
		if (tb == nullptr)
			return;

		int n = tb->getYXCount();
		for (int i = 0; i < n; i++)
		{
			double* v = tb->getYXValue(i);

			QStringList data;
			data.append(id);
			data.append(QString::number(v[0], 'E'));
			data.append(QString::number(v[1], 'E'));

			*_writeStream << fillSpaceToString("TABDATA", 14) << data.join(" ") << endl;
		}
	}

	QList<DataCut> INPFWriter::CutList(QList<int> & datalist)
	{
		QList<DataCut> cuts;
		if (datalist.isEmpty()) return cuts;
		const int ncount = datalist.size();

		for (int i = 0; i < ncount; ++i)
		{
			int start = datalist[i];
			int startIndex = i;
			int step = -1;
			while (i < ncount)
			{
				if (i == ncount - 1)break;
				i++;
				if (i + 1 > ncount - 1) { step = datalist[i] - datalist[i - 1]; break; }
				const int r1 = datalist[i] - datalist[i - 1];
				const int r2 = datalist[i + 1] - datalist[i];
				if (r1 != r2) { step = r1; break; }
			}
			int end = datalist[i];
			if (i - startIndex == 1) step = end - start;
			if (i == startIndex) step = end = -1;

			DataCut c;
			c.start = start;
			c.end = end;
			c.step = step;
			cuts.append(c);
		}
		return cuts;
	}
}


