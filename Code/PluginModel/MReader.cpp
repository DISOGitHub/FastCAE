#include "MReader.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"

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

namespace Plugins
{
	MReader::MReader(QString& fileName)
	
	{
		_file = new QFile(fileName);
		_meshData = MeshData::MeshData::getInstance();
	}

	MReader::~MReader()
	{
		if (_file != nullptr) delete _file;
// 		if (_thermalData != nullptr) 
// 			delete _thermalData;
// 		_thermalData = nullptr;
	}

	double MReader::Val(const QString &rStr)
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

	QString MReader::Mid(const QString &rStr, const int position, const int size)
	{
		QByteArray array = rStr.toLocal8Bit();
		QByteArray midarray = array.mid(position, size);
		return QString::fromLocal8Bit(midarray);
	}

	QString MReader::Str(QString &rStr, const int index)
	{
		QStringList list = rStr.simplified().split(" ", QString::SkipEmptyParts);
		if (index > list.size())
			return "";
		else
			return list.at(index);
	}

	int MReader::InStr(const int position, const QString& rStr, const QString& c)
	{
		return rStr.indexOf(c, position, Qt::CaseSensitive);
	}

	bool MReader::read()
	{
		_codec = QTextCodec::codecForName("GBK");

		if (!_file->open(QIODevice::ReadOnly)) return false;
		int partIndex = 1;

		auto meshKernal = new MeshData::MeshKernal;

		const int mkID = meshKernal->getID();
		meshKernal->setName(QString("MFile_%1").arg(mkID));
		MeshData::MeshData::getInstance()->appendMeshKernal(meshKernal);
		vtkPolyData * gird = vtkPolyData::New();

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
				finalLine = readPart4(gird); break;//读取节点
			case 5:
				finalLine = readPart5(gird); break;//读取单元
			case 6:
				finalLine = readPart6(); break;
			case 7:
				finalLine = readPart7(); break;
			case 8:
				finalLine = readPart8(); break;
			case 9:
				finalLine = readPart9(mkID); break;//读取组件
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
			MeshData::MeshData::getInstance()->removeKernalByID(mkID);
			_file->close();
			delete _file;
			_file = nullptr;
			return false;
		}

		meshKernal->setMeshData(gird);
		MeshData::MeshData::getInstance()->generateDisplayDataSet();

	//	_modelData->setThermalData(_thermalData);

		_file->close();
		delete _file;
		_file = nullptr;
		return true;
	}

	QString MReader::readLine()
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

	QString MReader::readPart1()
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
			}
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString MReader::readPart2()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString MReader::readPart3()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString MReader::readPart4(vtkPolyData* grid)
	{
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();//vtk点集,几何结构

		vtkIdType index;
		QString line{};

		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
			QStringList linelist = line.split(" ");
			if (linelist.size() != 4) continue;

			const int nodeId = linelist[0].toInt();
			double nodeX = linelist[1].toDouble();
			double nodeY = linelist[2].toDouble();
			double nodeZ = linelist[3].toDouble();

			index = points->InsertNextPoint(nodeX, nodeY, nodeZ);
			_nodeIDIndex.insert(nodeId, index);
		}
		grid->SetPoints(points);
		return line;
	}

	QString MReader::readPart5(vtkPolyData* grid)
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
		}
		grid->SetPolys(cells);
		return line;
	}

	QString MReader::readPart6()
	{
		QString line;

		while (!_file->atEnd())
		{
			line = readLine().simplified();

			if (line == "-1" || line.isEmpty()) break;

			
		}

		//qDebug() << "CouplingList size" << _thermalData->getThermalCouplingList().size();
		return line;
	}

	QString MReader::readPart7()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString MReader::readPart8()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	QString MReader::readPart9(int kid)
	{
		QString line;
		int tcID = 0;
		while (!_file->atEnd())
		{
			line = readLine().simplified();
			if (line == "-1" || line.isEmpty()) break;

			if (_stringBuffer[BUFFERSIZE - 1].simplified() == "$" &&
				_stringBuffer[BUFFERSIZE - 2].simplified().startsWith("$ NAME cards") &&
				_stringBuffer[BUFFERSIZE - 3].simplified() == "$")
			{
				readMeshGroups(kid);
			}
			
			

		}

		return line;
	}


	QString MReader::readPart10()
	{
		QString line;
		while (!_file->atEnd())
		{
			line = readLine();
			if (line == "-1" || line.isEmpty()) break;
		}
		return line;
	}

	void MReader::readMeshGroups(int kid)
	{
		QString line{};
		bool read = true;

		while (!_file->atEnd())
		{
			if (read)
				line = readLine();
			read = true;
			if (line.startsWith("$")) return;
			if (line.startsWith("NAME2"))
			{
				line = readMeshGroup(kid, line);
				read = false;
			}
		}
	}

	QString MReader::readMeshGroup(int kid, QString firstLine)
	{

		const QString shortName = Mid(firstLine, 6, 7).simplified();
		const QString longname = Mid(firstLine, 13).simplified();
		const QString  setName = QString("%1(%2)").arg(shortName).arg(longname);

		auto set = new MeshData::MeshSet(setName, MeshData::Element);
		MeshData::MeshData::getInstance()->appendMeshSet(set);
		QString line{};
		while (!_file->atEnd())
		{
			line = this->readLine();
			if (line.startsWith("NAME2") || line.startsWith("$")) return line;
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




}
