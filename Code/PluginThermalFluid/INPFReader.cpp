#include "INPFReader.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include "TFCouplingData.h"
#include "ThermalCoupling.h"
#include "ThermalBoundary.h"
#include "TFThickness.h"
#include "TFMaterial.h"
#include "ThermalData.h"
#include "TFRevMeshSet.h"
#include "ControlTemperature.h"
#include "BoundaryTabTypeData.h"
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkVertex.h>
#include <vtkTriangle.h>
#include <vtkQuad.h>
#include <QDomElement>
#include <QDomDocument>
#include <vtkLine.h>

#define BUFFERSIZE 4

namespace ThermalFluid
{
	INPFReader::INPFReader(QString& fileName, TFCouplingModelData* data)
		:_modelData(data)
	{
		_file = new QFile(fileName);
		_meshData = MeshData::MeshData::getInstance();
		//_thermalData = new ThermalData{};
		if (_modelData != nullptr)
			_thermalData = _modelData->getThermalData();
	}

	INPFReader::~INPFReader()
	{
		if (_file != nullptr) delete _file;
		// 		if (_thermalData != nullptr) 
		// 			delete _thermalData;
		// 		_thermalData = nullptr;
	}

	double INPFReader::Val(const QString &rStr)
	{
		if (rStr.isEmpty())	return 0;

		bool ok;
		double result = rStr.toDouble(&ok);
		if (ok)
			return result;
		else
		{
			/* INPF只会出现E+/E-,不会出现字母和E+/E-的组合状态
			*  但是会出现字母和数字组合的状态(不包括E+/E-)
			*  不会出现字母+(负数)
			不会出现字母+(+/-浮点数字) */
			QString tmp = "";
			for (int i = 0; i < rStr.length(); i++)
			{
				if (rStr[i] >= '0' && rStr[i] <= '9')
					tmp.append(rStr[i]);
			}
			return tmp.toDouble();
		}
	}

	QString INPFReader::Mid(const QString &rStr, const int position, const int size)
	{
		QByteArray array = rStr.toLocal8Bit();
		QByteArray midarray = array.mid(position, size);
		return QString::fromLocal8Bit(midarray);
	}

	QString INPFReader::Str(QString &rStr, const int index)
	{
		QStringList list = rStr.simplified().split(" ", QString::SkipEmptyParts);
		if (index > list.size())
			return "";
		else
			return list.at(index);
	}

	int INPFReader::InStr(const int position, const QString& rStr, const QString& c)
	{
		return rStr.indexOf(c, position, Qt::CaseSensitive);
	}

	bool INPFReader::read()
	{
		_codec = QTextCodec::codecForName("GBK");

		if (!_file->open(QIODevice::ReadOnly)) return false;
		int partIndex = 1;

		_thermalData->clear();

		auto meshKernal = new MeshData::MeshKernal;
		const int mkID = meshKernal->getID();
		meshKernal->setName(QString("INPF_%1").arg(mkID));
		MeshData::MeshData::getInstance()->appendMeshKernal(meshKernal);
		vtkPolyData* gird = vtkPolyData::New();

		while (!_file->atEnd())
		{
			QString finalLine;
			switch (partIndex)
			{
			case 1:
				finalLine = readPart1(); break;
			case 2:
				finalLine = readPart2(); break;
			case 3:
				finalLine = readPart3(); break;
			case 4:
				finalLine = readPart4(gird); break;
			case 5:
				finalLine = readPart5(gird); break;
			case 6:
				finalLine = readPart6(); break;
			case 7:
				finalLine = readPart7(); break;
			case 8:
				finalLine = readPart8(); break;
			case 9:
				finalLine = readPart9(mkID); break;
			case 10:
				finalLine = readPart10(); break;
			default:break;
			}

			if (finalLine == "-1") partIndex++;
			else if (finalLine.startsWith("ERROR"))
				return false;
			else if (finalLine.isEmpty()) break;

		}

		if (gird->GetNumberOfPoints() < 1 || gird->GetNumberOfCells() < 1)
		{
			gird->Delete();
			gird = nullptr;

			MeshData::MeshData::getInstance()->removeKernalByID(mkID);
			_file->close();
			delete _file;
			_file = nullptr;
			return false;
		}

		_thermalData->setMeshKernalID(mkID);
		meshKernal->setMeshData(gird);
		MeshData::MeshData::getInstance()->generateDisplayDataSet();

		_file->close();
		delete _file;
		_file = nullptr;
		return true;
	}

	QString INPFReader::readLine()
	{
		QString s;
		while (!_file->atEnd())
		{
			QByteArray l = _file->readLine();
			if (l.isEmpty()) continue;
			s = _codec->toUnicode(l);
			break;
		}

		_stringBuffer.append(s);
		if (_stringBuffer.size() > BUFFERSIZE)
			_stringBuffer.removeFirst();
		//		qDebug() << s;
		return s;
	}

	QString INPFReader::readPart1()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line.mid(0, 7) == "$ UNITS")
			{
				// 				_xLength = Val(Str(line, 3));
				// 				_xForce = Val(Str(line, 4));
				// 				_xTemper = Val(Str(line, 5));
				_thermalData->setCoorDataUnits(Val(Str(line, 3)), Val(Str(line, 4)), Val(Str(line, 5)));
			}
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString INPFReader::readPart2()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString INPFReader::readPart3()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString INPFReader::readPart4(vtkPolyData* grid)
	{
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		vtkIdType index;
		QString line{};
		const double xLength = _thermalData->getXLength();

		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
			QStringList linelist = line.split(" ");
			if (linelist.size() != 4) continue;

			const int nodeId = linelist[0].toInt();
			double nodeX = linelist[1].toDouble() / xLength * 1000;
			double nodeY = linelist[2].toDouble() / xLength * 1000;
			double nodeZ = linelist[3].toDouble() / xLength * 1000;

			index = points->InsertNextPoint(nodeX, nodeY, nodeZ);
			_nodeIDIndex.insert(nodeId, index);
		}
		grid->SetPoints(points);
		return line;
	}

	QString INPFReader::readPart5(vtkPolyData* grid)
	{
		vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

		QString line{};
		int eleIndex{ 0 };
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty())		break;
			if (line.contains("$") || line.contains("SPACE"))	continue;

			QStringList linelist = line.split(" ");
			int elemID = linelist[0].toInt();
			QString elemMatID = linelist[2];
			QString elemPropId = linelist[4];

			if (linelist.size() == 7)
			{
				vtkSmartPointer<vtkLine> lineCell = vtkSmartPointer<vtkLine>::New();
				int elemN1 = linelist[5].toInt();
				int elemN2 = linelist[6].toInt();

				lineCell->GetPointIds()->SetId(0, _nodeIDIndex.value(elemN1));
				lineCell->GetPointIds()->SetId(1, _nodeIDIndex.value(elemN2));
				eleIndex = cells->InsertNextCell(lineCell);
			}
			else if (linelist.size() == 8)
			{
				vtkSmartPointer<vtkTriangle> triCell = vtkSmartPointer<vtkTriangle>::New();

				int elemN1 = linelist[5].toInt();
				int elemN2 = linelist[6].toInt();
				int elemN3 = linelist[7].toInt();

				triCell->GetPointIds()->SetId(0, _nodeIDIndex.value(elemN1));
				triCell->GetPointIds()->SetId(1, _nodeIDIndex.value(elemN2));
				triCell->GetPointIds()->SetId(2, _nodeIDIndex.value(elemN3));
				eleIndex = cells->InsertNextCell(triCell);
			}
			else if (linelist.size() == 9)
			{
				vtkSmartPointer<vtkQuad> quadCell = vtkSmartPointer<vtkQuad>::New();

				int elemN1 = linelist[5].toInt();
				int elemN2 = linelist[6].toInt();
				int elemN3 = linelist[7].toInt();
				int elemN4 = linelist[8].toInt();

				quadCell->GetPointIds()->SetId(0, _nodeIDIndex.value(elemN1));
				quadCell->GetPointIds()->SetId(1, _nodeIDIndex.value(elemN2));
				quadCell->GetPointIds()->SetId(2, _nodeIDIndex.value(elemN3));
				quadCell->GetPointIds()->SetId(3, _nodeIDIndex.value(elemN4));
				eleIndex = cells->InsertNextCell(quadCell);
			}
			else continue;
			_elementIDIndex.insert(elemID, eleIndex);
			_thermalData->appendElementToMaterialHash(elemID, Val(elemMatID));
			_thermalData->appendElementToThicknessHash(elemID, Val(elemPropId));
		}
		grid->SetPolys(cells);
		return line;
	}

	QString INPFReader::readPart6()
	{
		QString line;

		while (!_file->atEnd())
		{
			line = readLine().simplified();

			if (line == "-1" || line.isEmpty()) break;

			if ((Mid(line, 0, 4) == "AREA") && (Mid(_stringBuffer[0], 0, 1) == "$") && (Mid(_stringBuffer[1], 0, 1) == "$")
				&& (Mid(_stringBuffer[2], 0, 1) == "$"))
			{
				QStringList  linelist = line.split(" ", QString::SkipEmptyParts);
				//	qDebug() << linelist.size();

				ThermalCoupling* tc = new ThermalCoupling{};

				int id = _stringBuffer[1].split(" ", QString::SkipEmptyParts).at(1).toInt();
				QString name = Mid(_stringBuffer[1].simplified(), InStr(0, _stringBuffer[1], "-") + 2);
				QString gOne = linelist.at(1);
				QString gTwo = linelist.at(4);
				QString type = linelist.at(7);
				double xValue = Val(linelist.at(6));

				double xl = _thermalData->getXLength();
				double xf = _thermalData->getXFroce();
				double xt = _thermalData->getXTemper();
				//	qDebug() << id << name << gOne << gTwo << type << xValue;

				tc->setID(id);
				tc->setName(name);
				tc->setGroupOne(gOne);
				tc->setGroupTwo(gTwo);
				tc->setType(tc->stringToCouplingType(type));
				tc->setXValue(xValue);
				tc->appendProperty(QObject::tr("GroupOne"), gOne);
				tc->appendProperty(QObject::tr("GroupTwo"), gTwo);
				tc->appendProperty(QObject::tr("tcType"), type);
				tc->appendProperty(QObject::tr("Computing coefficients"), xValue);
				tc->setCoe(xl, xf, xt);

				_thermalData->appendThermalCouplingList(tc);

			}
			else if ((Mid(line, 0, 4) == "AREA") && (Mid(_stringBuffer[0], 0, 1) == "$") && (Mid(_stringBuffer[1], 0, 1) == "$")
				&& (_stringBuffer[2].simplified() == "MESH 2"))
			{
				QStringList  linelist = line.split(" ", QString::SkipEmptyParts);
				//	qDebug() << linelist.size();

				ThermalCoupling* tc = new ThermalCoupling{};

				int id = _stringBuffer[0].split(" ", QString::SkipEmptyParts).at(1).toInt();
				QString name = Mid(_stringBuffer[0].simplified(), InStr(0, _stringBuffer[0], "-") + 2);
				QString gOne = linelist.at(1);
				QString gTwo = linelist.at(4);
				QString type = linelist.at(7);
				double xValue = Val(linelist.at(6));

				double xl = _thermalData->getXLength();
				double xf = _thermalData->getXFroce();
				double xt = _thermalData->getXTemper();
				//	qDebug() << id << name << gOne << gTwo << type << xValue;

				tc->setID(id);
				tc->setName(name);
				tc->setGroupOne(gOne);
				tc->setGroupTwo(gTwo);
				tc->setType(tc->stringToCouplingType(type));
				tc->setXValue(xValue);
				tc->appendProperty(QObject::tr("ThermalCouplingID"), id);
				tc->appendProperty(QObject::tr("ThermalCouplingName"), name);
				tc->appendProperty(QObject::tr("GroupOne"), gOne);
				tc->appendProperty(QObject::tr("GroupTwo"), gTwo);
				tc->appendProperty(QObject::tr("tcType"), type);
				tc->appendProperty(QObject::tr("Computing coefficients"), xValue);
				tc->setCoe(xl, xf, xt);

				_thermalData->appendThermalCouplingList(tc);

			}
		}
		return line;
	}

	QString INPFReader::readPart7()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString INPFReader::readPart8()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString INPFReader::readPart9(int kid)
	{
		QString line;
		int tcID = 0;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;

			QStringList linelist = line.split(" ", QString::SkipEmptyParts);

			if (linelist.at(0) == "PARAM" && linelist.at(1) == "UNITS")
			{
				_thermalData->setMaterialDataUnits(linelist.at(3).toDouble(), linelist.at(4).toDouble(), linelist.at(5).toDouble());
			}


			else if ((Mid(_stringBuffer[0], 0, 1) == "$") && (Mid(_stringBuffer[1], 0, 1) == "$")
				&& (Mid(_stringBuffer[2], 0, 1) == "$") && (linelist.at(0) == "PROP"))
			{
				readThickness(linelist);
			}

			else if ((Mid(_stringBuffer[0], 0, 1) == "$") && (Mid(_stringBuffer[1], 0, 1) == "$")
				&& (Mid(_stringBuffer[2], 0, 1) == "$") && (linelist.at(0) == "MAT"))
			{
				TFMaterial* tm = new TFMaterial{};
				QString name = Mid(_stringBuffer[1].simplified(), InStr(0, _stringBuffer[1], "-") + 2);
				int id = linelist.at(1).toInt();

				tm->setID(id);
				tm->setName(name);
				readMaterial(tm, line);
				_thermalData->appendMaterialList(tm);
			}

			else if ((Mid(_stringBuffer[0], 0, 1) == "$") && (Mid(_stringBuffer[1], 0, 1) == "$") && (Mid(_stringBuffer[2], 0, 1) == "$")
				&& (linelist.at(0) == "QNODE" || linelist.at(0) == "INTERP" || linelist.at(0) == "SINK"))
			{
				readThermalBoundary(linelist);
			}


			else if (_stringBuffer[BUFFERSIZE - 1].simplified() == "$" &&
				_stringBuffer[BUFFERSIZE - 2].simplified().startsWith("$ NAME cards") &&
				_stringBuffer[BUFFERSIZE - 3].simplified() == "$")
			{
				readMeshGroups(kid);
			}

			else if ((Mid(_stringBuffer[0], 0, 1) == "$") && (Mid(_stringBuffer[1], 0, 1) == "$")
				&& (Mid(_stringBuffer[2], 0, 1) == "$") && (linelist.at(0) == "THERMST"))
			{
				readControlTemperatur(linelist, tcID);
				tcID++;
			}

			else if ((Mid(_stringBuffer[2], 0, 1) == "$") && (linelist.at(0) == "TABTYPE") && (linelist.at(3) == "TIME"))
			{
				BoundaryTabTypeData* td = new BoundaryTabTypeData;

				QString name = Mid(_stringBuffer[1].simplified(), InStr(0, _stringBuffer[1], "-") + 2);
				int id = linelist.at(1).toInt();
				TabType type = td->stringToTabType(linelist.at(2));

				td->setName(name);
				td->setID(id);
				td->setType(type);
				readTabTypeData(td);

				_thermalData->appendTabTypeDataList(td);
			}

			else if ((Mid(_stringBuffer.last(), 0, 7) == "REVNODE" || Mid(_stringBuffer.last(), 0, 7) == "REVNOM ") &&
				Mid(_stringBuffer[2], 0, 5) == "NAME2" && Mid(_stringBuffer[1], 0, 1) == "$" && Mid(_stringBuffer[0],0, 1) == "$")
			{
				readRevGroup();
			}
		}

		qDebug() << "ThicknessList size is " << _thermalData->getThicknessList().size();
		qDebug() << "MaterialList size is " << _thermalData->getMaterialList().size();
		qDebug() << "BoundaryList size is " << _thermalData->getThermalBoundaryList().size();
		qDebug() << "TemperatureList size is " << _thermalData->getControlTemperatureListCount();
		qDebug() << "TabTypeDataList size is " << _thermalData->getTabTypeDataListCount();
		return line;
	}


	QString INPFReader::readPart10()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	void INPFReader::readMeshGroups(int kid)
	{
		QString line{};
		bool read = true;

		while (!_file->atEnd())
		{
			if (read)
				line = readLine();

			//qDebug() << line;
			read = true;
			if (line.startsWith("$") || line.simplified() == "-1") return;
			if (line.startsWith("NAME2") && !(line.contains("NAME2 SPACE")))
			{
				line = readMeshGroup(kid, line);
				read = false;
			}
		}
	}

	QString INPFReader::readMeshGroup(int kid, QString firstLine)
	{
		const QString shortName = Mid(firstLine, 6, 7).simplified();
		const QString longname = Mid(firstLine, 13).simplified();
		const QString setName = QString("%1(%2)").arg(shortName).arg(longname);

		auto set = new MeshData::MeshSet(setName, MeshData::Element);
		MeshData::MeshData::getInstance()->appendMeshSet(set);
		QString line{};
		while (!_file->atEnd())
		{
			line = this->readLine();
			if (line.startsWith("NAME2") || line.startsWith("$") || line.simplified() == "-1") return line;
			else if (line.startsWith("NAME "))
			{
				QStringList lineInfo = line.simplified().split(" ");
				bool ok = false;
				if (lineInfo.size() == 5)
				{
					const int start = lineInfo.at(2).toInt(&ok);
					if (!ok) continue;
					const int end = lineInfo.at(3).toInt(&ok);
					if (!ok) continue;
					const int step = lineInfo.at(4).toInt(&ok);
					if (!ok) continue;
					for (int id = start; id <= end; id += step)
					{
						int index = _elementIDIndex.value(id);
						set->appendMember(kid, index);
					}
				}
				else if (lineInfo.size() == 3)
				{
					const int id = lineInfo.at(2).toInt(&ok);
					if (!ok) continue;
					int index = _elementIDIndex.value(id);
					set->appendMember(kid, index);
				}
			}
		}
	}

	void INPFReader::readRevGroup()
	{
		TFRevMeshSet* rev = new TFRevMeshSet;
		MeshData::MeshData::getInstance()->appendMeshSet(rev);
		QString l2 = _stringBuffer.last().simplified();
		QString l1 = _stringBuffer[BUFFERSIZE - 2].simplified();
		QStringList infos = l1.split(" ");
		QString name = QString("%1(%2)").arg(infos[1]).arg(infos[2]);
		rev->setName(name);
		infos = l2.split(" ");
		rev->setCopy(infos.at(1));
		double v = infos.at(3).toDouble();
		rev->setEmissivity(v);
		v = infos.at(4).toDouble();
		rev->setReflectivity(v);
		if (infos.at(0) == "REVNOM")
			rev->isRemov(false);
		else
			rev->isRemov(true);

	}

	void INPFReader::readThickness(QStringList linelist)
	{
		double yl = _thermalData->getYLength();
		double yf = _thermalData->getYFroce();
		double yt = _thermalData->getYTemper();

		TFThickness* tn = new TFThickness{};
		QString name = Mid(_stringBuffer[1].simplified(), 2);
		int id = linelist.at(1).toInt();
		double value = linelist.at(3).toDouble() / yl * 1000;

		tn->setID(id);
		tn->setName(name);
		tn->setValue(value);

		_thermalData->appendThicknessList(tn);
	}

	void INPFReader::readMaterial(TFMaterial* tm, QString line)
	{
		while (!_file->atEnd())
		{
			if (line == "-1" || line.isEmpty() || line == "$") break;

			double y[3] = { 1 };
			y[0] = _thermalData->getYLength();
			y[1] = _thermalData->getYFroce();
			y[2] = _thermalData->getYTemper();

			QStringList linelist = line.split(" ", QString::SkipEmptyParts);
			tm->appendToPropertyHash(linelist.at(2), linelist.at(3));
			tm->setProperty(linelist.at(2), linelist.at(3), y);
			line = readLine().simplified();
		}
		tm->appendProperty(QObject::tr("RHO"), tm->getRho());
		tm->appendProperty(QObject::tr("CPP"), tm->getCpp());
		tm->appendProperty(QObject::tr("KTHERM"), tm->getKtherm());
		tm->appendProperty(QObject::tr("E"), tm->getEmiss());
		tm->appendProperty(QObject::tr("ABSORP"), tm->getAbsorp());
		tm->appendProperty(QObject::tr("SPECULARITY"), tm->getSpecularity());
		tm->appendProperty(QObject::tr("REMISS"), tm->getRemiss());
		tm->appendProperty(QObject::tr("RABSOR"), tm->getRabsor());
		tm->appendProperty(QObject::tr("RSOLSPE"), tm->getRsolspe());
	}


	void INPFReader::readThermalBoundary(QStringList linelist)
	{
		ThermalBoundary* tb = new ThermalBoundary(_thermalData);
		QString name = Mid(_stringBuffer[1].simplified(), InStr(0, _stringBuffer[1], "-") + 2);
		int id = _stringBuffer[1].split(" ", QString::SkipEmptyParts).at(1).toInt();
		QString group = linelist.at(1);
		ThermalType one = _thermalData->stringToThermalType(linelist.at(0));
		ThermalType two = ThermalType::NONE;
		if (linelist.at(0) != "SINK")
			two = _thermalData->stringToThermalType(linelist.at(4));
		double value = 0;
		if (linelist.at(0) == "INTERP")
			value = linelist.at(5).toDouble();
		else
			value = linelist.at(2).toDouble();
		
		tb->setID(id);
		tb->setName(name);
		tb->setGroup(group);
		tb->setXValue(value);
		tb->appendProperty(QObject::tr("GroupName"), group);

		if (linelist.at(0) == "INTERP")
		{
			int tid = linelist.at(3).toInt();
			double mul = linelist.at(5).toDouble();
			tb->setTableTypeID(tid);
			tb->setTypeAndUnit(two);
			tb->appendProperty(QObject::tr("XValue"), value);
			tb->setQ(two, mul);
		}
		else if (linelist.at(0) == "SINK")
		{
			QString line = readLine().simplified();

			if (line.split(" ", QString::SkipEmptyParts).at(0) != "INTERP")
				tb->setTypeAndUnit(one, two);
			tb->appendProperty(QObject::tr("XValue"), value);
			tb->setQ(one, two,value);
		}
		else
		{
			tb->setTypeAndUnit(one, two);
			tb->appendProperty(QObject::tr("XValue"), value);
			tb->setQ(one, two,value);
		}
		_thermalData->appendThermalBoundaryList(tb);
	}

	void INPFReader::readControlTemperatur(QStringList linelist, int id)
	{
		ControlTemperature* tc = new ControlTemperature(_thermalData);

		/*tcID++*/
		QString name = _stringBuffer[1].simplified().split(" ").at(3);
		QString hGroup = linelist.at(2);
		QString sGroup = linelist.at(1);
		double lt = linelist.at(3).toDouble();
		double ht = linelist.at(4).toDouble();
		ThermalType type = _thermalData->stringToThermalType(linelist.at(6));
		double v = linelist.at(5).toDouble();

		//		qDebug() << tcID << name << hGroup << sGroup << lt << ht << linelist.at(6) << v;

		tc->setID(id);
		tc->setName(name);
		tc->setHGroup(hGroup);
		tc->setSGroup(sGroup);
		tc->setHTemperature(ht);
		tc->setLTemperature(lt);
		tc->setTypeAndUnit(type);
		tc->setXValue(v);

		_thermalData->appendControlTemperatureList(tc);
	}

	void INPFReader::readTabTypeData(BoundaryTabTypeData* td)
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty() || (line == "$")) break;

			QStringList linelist = line.split(" ");
			//	qDebug() << linelist;


			double y = linelist.at(2).toDouble();
			double x = linelist.at(3).toDouble();

			td->appendYX(y, x);
		}
	}

}
