#include "inpReader.h"
#include "meshSingleton.h"
#include "meshKernal.h"
#include "meshSet.h"
#include <vtkUnstructuredGrid.h>
#include <vtkCellType.h>
#include <vtkIdList.h>
#include <vtkIdTypeArray.h>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <assert.h>

namespace MeshData
{
	InpReader::InpReader(const QString &filename)
	{
		_fileName = filename;
		_meshData = MeshData::getInstance();
	}
	InpReader::~InpReader()
	{
		if (_stream != nullptr) delete _stream;
	}

	void InpReader::readLine(QString &line)
	{
		while (!_stream->atEnd())
		{
			line = _stream->readLine().simplified().toLower();
			if (line.startsWith("**")) continue;
			break;
		}
	}

	bool InpReader::read()
	{
		QFileInfo info(_fileName);
		if (!info.exists()) return false;
		QString name = info.fileName();
		QString path = info.filePath();
		QFile file(_fileName);
		if (!file.open(QIODevice::ReadOnly)) return false;
		_stream = new QTextStream(&file);
		vtkSmartPointer<vtkUnstructuredGrid> dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
		QString line;
	
		do
		{
			this->readLine(line);
			if (line.startsWith("*node") && line.size()<6)
			{
				if (!readNodes(dataset, line))
				{
					dataset->Delete();
					return false;
				}
			}
			if (line.startsWith("*element"))
			{
				if (!readElements(dataset, line))
				{
					dataset->Delete();
					return false;
				}


				if (dataset != nullptr)
				{
					MeshKernal* k = new MeshKernal;
					k->setName(name);
					k->setPath(path);
					k->setMeshData((vtkDataSet*)dataset);
					_meshData->appendMeshKernal(k);
				}
			}
			if (line.startsWith("*nset"))
			{
				this->readNSet(line);
			}
			if (line.startsWith("*elset"))
			{
				this->readElSet(line);
			}

		} while (!_stream->atEnd());

	
		return true;
	}

	bool InpReader::readNodes(vtkUnstructuredGrid* g, QString &line)
	{
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		bool ok = false;
		int index = 0;
		while (!_stream->atEnd())
		{
			readLine(line);
			if (line.contains("*")) break;

			double coor[3] = { 0 };
			QStringList scoor = line.split(",");
			if (scoor.size() != 4) continue;
			for (int j = 0; j < 3; ++j)
			{
				coor[j] = scoor.at(j + 1).toDouble(&ok);
				if (!ok) return false;
			}

			points->InsertNextPoint(coor);
			int id = scoor.at(0).toInt(&ok);
			if (ok) _nodeIDIndex[id] = index;
			index++;
		}

		if (points->GetNumberOfPoints() > 2)
		{
			g->SetPoints(points);
			return true;
		}
		return false;
	}

	bool InpReader::readElements(vtkUnstructuredGrid* g, QString &line)
	{
		QStringList stype = line.split(",");
		//if (stype.size() != 2) return false;
		QString st = stype.at(1);
		st.remove("type=").remove(" ").simplified();
		VTKCellType tp = VTK_EMPTY_CELL;
//		qDebug() << st;
		if (st == "c3d4")
		{
			tp = VTK_TETRA;
		}

		if (tp == VTK_EMPTY_CELL) return false;

		bool ok = false;
		int index = 0;

		while (!_stream->atEnd())
		{
			this->readLine(line);
			if (line.startsWith("*")) break;
			vtkSmartPointer<vtkIdList> indexList = vtkSmartPointer<vtkIdList>::New();

			QStringList sids = line.split(",");
			for (int j = 1; j < sids.size(); ++j)
			{
				int nodeid = sids.at(j).toInt(&ok);
				assert(ok);
				int nodeindex = _nodeIDIndex.value(nodeid);
				indexList->InsertNextId(nodeindex);
			}
			
			g->InsertNextCell(tp, indexList);
			int eleID = sids.at(0).toInt(&ok);
			_elemIDIndex[eleID] = index;
			index++;
		}
		return true;
		
	}

	bool InpReader::readNSet(QString &line)
	{
		bool isgen = false;
		if (line.contains("generate")) isgen = true;

		QStringList sinfo = line.split(",");
		QString name = sinfo.at(1);
		name.remove("nset=");

		if (isgen)
			name = name + "_gen";
		
		MeshSet* set = new MeshSet(name, Node);
		const int c = _meshData->getKernalCount();
		MeshKernal* k = _meshData->getKernalAt(c - 1);
		if (k == nullptr) return false;
// 		vtkDataSet* dataset = k->getMeshData();
// 		set->setDataSet(dataset);
		const int kid = k->getID();
		
//		vtkSmartPointer<vtkIdTypeArray> array = vtkSmartPointer<vtkIdTypeArray>::New();
		while (!_stream->atEnd())
		{
			this->readLine(line);
			if (line.startsWith("*"))
			{
//				set->setIDList(array);
				_meshData->appendMeshSet(set);
			}
			if (line.startsWith("*nset"))
			{
				this->readNSet(line);
			}
			if (line.startsWith("*elset"))
			{
				this->readElSet(line);
			}
			if (line.startsWith("*") && (!line.startsWith("*nset")))
				return true;
			
			QStringList sid = line.split(",");
			if (isgen)
			{
				int beg = sid.at(0).toInt();
				int end = sid.at(1).toInt();
				for (int i = beg; i <= end; ++i)
				{
					if (!_nodeIDIndex.contains(i)) continue;
					int index = _nodeIDIndex.value(i);
//					array->InsertNextValue(index);
					set->appendMember(kid, index);
				}
				continue;
			}

			for (int i = 0; i < sid.size(); ++i)
			{
				int id = sid.at(i).toInt();
				int index = _nodeIDIndex.value(id);
//				array->InsertNextValue(index);
				set->appendMember(kid, index);
			}
		
		}

	}

	bool InpReader::readElSet(QString &line)
	{
		bool isgen = false;
		if (line.contains("generate")) isgen = true;

		QStringList sinfo = line.split(",");
		QString name = sinfo.at(1);
		name.remove("elset=");

		if (isgen)
			name = name + "_gen";

		MeshSet* set = new MeshSet(name, Element);
		const int c = _meshData->getKernalCount();
		MeshKernal* k = _meshData->getKernalAt(c - 1);
		if (k == nullptr) return false;
//		vtkDataSet* dataset = k->getMeshData();
//		set->setDataSet(dataset);
		const int kid = k->getID();

//		vtkSmartPointer<vtkIdTypeArray> array = vtkSmartPointer<vtkIdTypeArray>::New();
		while (!_stream->atEnd())
		{
			this->readLine(line);
			if (line.startsWith("*"))
			{
//				set->setIDList(array);
				_meshData->appendMeshSet(set);
			}
			if (line.startsWith("*elset"))
			{
				this->readElSet(line);
			}
			if (line.startsWith("*nset"))

			{
				this->readNSet(line);
			}
			if (line.startsWith("*") && (!line.startsWith("*elset")))
				return true;

			QStringList sid = line.split(",");
			if (isgen)
			{
				int beg = sid.at(0).toInt();
				int end = sid.at(1).toInt();
				for (int i = beg; i <= end; ++i)
				{
					if (!_elemIDIndex.contains(i)) continue;
					int index = _elemIDIndex.value(i);
//					array->InsertNextValue(index);
					set->appendMember(kid, index);
				}
				continue;
			}

			for (int i = 0; i < sid.size(); ++i)
			{
				int id = sid.at(i).toInt();
				int index = _elemIDIndex.value(id);
//				array->InsertNextValue(index);
				set->appendMember(kid, index);
			}

		}
	}





}