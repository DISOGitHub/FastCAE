#include "MeshReader.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkIdTypeArray.h>
#include <QFileInfo>
#include <QFile>
#include <QDebug>

namespace Gmsh{
	MeshReader::MeshReader(QString file):
		_fileName(file)
	{
		_meshData = MeshData::MeshData::getInstance();
	}


	MeshReader::~MeshReader()
	{
		if (_stream != nullptr) delete _stream;
	}

	bool MeshReader::read()
	{
		QFileInfo info(_fileName);
		if (!info.exists()) return false;
		QString name = info.fileName();
		QString path = info.filePath();
		QFile file(_fileName);
		if (!file.open(QIODevice::ReadOnly)) return false;
		_stream = new QTextStream(&file);
		vtkUnstructuredGrid* dataset = vtkUnstructuredGrid::New();
		MeshData::MeshKernal* k = new MeshData::MeshKernal;
		QString line;

		
		while (!_stream->atEnd())
		{
			if (!line.startsWith("*")||line.startsWith("*head"))
				this->readLine(line);
			if (line.startsWith("*node") && line.size() < 6)
			{
				if (!readNodes(dataset, line))
				{
					dataset->Delete();
						
//					break;
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
				// 				if (dataset != nullptr)
				// 				{
				// 					MeshData::MeshKernal* k = new MeshData::MeshKernal;
				// 					k->setName(QString("Mesh_%1").arg(k->getID()));
				// 					k->setMeshData((vtkDataSet*)dataset);
				// 					_meshData->appendMeshKernal(k);
				// 				}
			}

			if (line.startsWith("*nset"))
			{
				this->readNSet(line, k);
			}
			if (line.startsWith("*elset"))
			{
				this->readElSet(line, k);
			}


		}


		if (dataset != nullptr)
		{
			k->setName(QString("Mesh_%1").arg(k->getID()));
			k->setMeshData((vtkDataSet*)dataset);
			_meshData->appendMeshKernal(k);
		}
		else
		{
			delete k;
			k = nullptr;
			return false;
		}


		int n = _meshData->getMeshSetCount();
		for (int i = 0; i < n; i++)
		{
			MeshData::MeshSet* set = _meshData->getMeshSetAt(i);
			if (set == nullptr)
				continue;

			set->generateDisplayDataSet();
		}


		return true;
	}

	void MeshReader::readLine(QString &line, bool b)
	{
		while (!_stream->atEnd())
		{
			line = _stream->readLine().simplified().toLower();

			if (line.startsWith("**")) continue;
		//	if (line.startsWith("*")) break;

			if (b && line.endsWith(","))
			{
				QString s;
				readLine(s);
			//	if (s.startsWith("*")) break;
				line = line + s;
			}
			
			break;
		}
	}

	bool MeshReader::readNodes(vtkUnstructuredGrid* g, QString &line)
	{
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		bool ok = false;
		/*int index = 0;*/
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
			if (ok) _nodeIDIndex[id] = _nIndex;
			_nIndex++;
		}

		if (points->GetNumberOfPoints() > 2)
		{
			g->SetPoints(points);
			return true;
		}
		return false;
	}

	bool MeshReader::readElements(vtkUnstructuredGrid* g, QString &line)
	{
		QStringList stype = line.split(",");
		//if (stype.size() != 2) return false;
		QString st = stype.at(1);
		st.remove("type=").remove(" ").simplified();
		VTKCellType tp = getVTKCellType(st);
		/*VTKCellType tp = VTK_EMPTY_CELL;*/
		//		qDebug() << st;
// 		if (st == "c3d4")
// 		{
// 			tp = VTK_TETRA;
// 		}

		if (tp == VTK_EMPTY_CELL) return false;

		bool ok = false;
		/*int index = 0;*/

		while (!_stream->atEnd())
		{
			this->readLine(line,true);
			if (line.startsWith("*element")) this->readElements(g, line);
			if (line.startsWith("*") && (!line.startsWith("*element")))
				return true;
			vtkSmartPointer<vtkIdList> indexList = vtkSmartPointer<vtkIdList>::New();

			QStringList sids = line.split(",");
			if (tp == VTK_QUAD && sids.size() == 10)
				tp = VTK_QUADRATIC_QUAD;

			for (int j = 1; j < sids.size(); ++j)
			{
				int nodeid = sids.at(j).toInt(&ok);
				assert(ok);
				int nodeindex = _nodeIDIndex.value(nodeid);
				indexList->InsertNextId(nodeindex);
			}

			g->InsertNextCell(tp, indexList);
			int eleID = sids.at(0).toInt(&ok);
			_elemIDIndex[eleID] = _eIndex;
			_eIndex++;
		}
		return true;
	}

	bool MeshReader::readNSet(QString &line, MeshData::MeshKernal* k)
	{
		bool isgen = false;
		if (line.contains("generate")) isgen = true;

		QStringList sinfo = line.split(",");
		QString name = sinfo.at(1);
		name.remove("nset=");

		if (isgen)
			name = name + "_gen";

		MeshData::MeshSet* set = new MeshData::MeshSet(name, MeshData::Node);
// 		const int c = _meshData->getKernalCount();
// 		MeshData::MeshKernal* k = _meshData->getKernalAt(c - 1);
		if (k == nullptr) return false;
		// 		vtkDataSet* dataset = k->getMeshData();
		// 		set->setDataSet(dataset);
		const int kid = k->getID();
		
		//		vtkSmartPointer<vtkIdTypeArray> array = vtkSmartPointer<vtkIdTypeArray>::New();
		while (!_stream->atEnd())
		{
			this->readLine(line);
			/*if (line.startsWith("*"))break;*/
// 			{
// 				//				set->setIDList(array);
// 				_meshData->appendMeshSet(set);
// 			}
// 			if (line.startsWith("*nset"))
// 			{
// 				this->readNSet(line/*,k*/);
// 			}
// 			if (line.startsWith("*elset"))
// 			{
// 				this->readElSet(line/*,k*/);
// 			}
			if (line.startsWith("*") /*&& (!line.startsWith("*nset"))*/)
			{
				_meshData->appendMeshSet(set);
				return true;
			}

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

		//set->generateDisplayDataSet();
		_meshData->appendMeshSet(set);

		return true;
	}

	bool MeshReader::readElSet(QString &line, MeshData::MeshKernal* k)
	{
		bool isgen = false;
		if (line.contains("generate")) isgen = true;

		QStringList sinfo = line.split(",");
		QString name = sinfo.at(1);
		name.remove("elset=");

		if (isgen)
			name = name + "_gen";

		MeshData::MeshSet* set = new MeshData::MeshSet(name, MeshData::Element);
// 		const int c = _meshData->getKernalCount();
// 		MeshData::MeshKernal* k = _meshData->getKernalAt(c - 1);
		if (k == nullptr) return false;
		//		vtkDataSet* dataset = k->getMeshData();
		//		set->setDataSet(dataset);
		const int kid = k->getID();

		//		vtkSmartPointer<vtkIdTypeArray> array = vtkSmartPointer<vtkIdTypeArray>::New();
		while (!_stream->atEnd())
		{
			this->readLine(line);
/*			if (line.startsWith("*"))break;*/
// 			{
// 				//				set->setIDList(array);
// 				_meshData->appendMeshSet(set);
// 			}
// 			if (line.startsWith("*elset"))
// 			{
// 				this->readElSet(line/*,k*/);
// 			}
// 			if (line.startsWith("*nset"))
// 
// 			{
// 				this->readNSet(line/*,k*/);
// 			}
			if (line.startsWith("*")/* && (!line.startsWith("*elset"))*/)
			{
				_meshData->appendMeshSet(set);
				return true;
			}
				

			QStringList sid = line.split(",");
			if (line.endsWith(","))
				sid.removeLast();

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

// 				if (!judgetIndex(index))
// 					qDebug() << "chongfu" << index;
			}

			
		}

		/*set->generateDisplayDataSet();*/
		_meshData->appendMeshSet(set);

		return true;
	}

	VTKCellType MeshReader::getVTKCellType(QString s)
	{
		VTKCellType vt = VTK_EMPTY_CELL;
		if (s == "t3d2") vt = VTK_LINE;
		else if (s == "cps3") vt = VTK_TRIANGLE;
		else if (s == "cps4") vt = VTK_QUAD;
		else if (s == "c3d4") vt = VTK_TETRA;
		else if (s == "c3d8") vt = VTK_HEXAHEDRON;
		else if (s == "t3d3") vt = VTK_QUADRATIC_EDGE;
		else if (s == "cps6") vt = VTK_QUADRATIC_TRIANGLE;
		else if (s == "c3d10") vt = VTK_QUADRATIC_TETRA;
		else if (s == "c3d27") vt = VTK_TRIQUADRATIC_HEXAHEDRON;

		return vt;
	}

// 	bool MeshReader::judgetIndex(int d)
// 	{
// 		if (_indexList.contains(d))		
// 			return false;
// 		
// 			
// 
// 		_indexList.append(d);
// 		return true;
// 		
// 	}

}