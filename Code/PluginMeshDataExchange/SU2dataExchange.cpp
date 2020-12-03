#include "SU2dataExchange.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include <vtkUnstructuredGrid.h>
#include <vtkCellType.h>
#include <vtkIdList.h>
#include <vtkIdTypeArray.h>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>

namespace MeshData
{
	SU2dataExchange::SU2dataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId) :
		MeshThreadBase(fileName, operation, mw),
		_fileName(fileName),
		_meshData(MeshData::getInstance()),
		_operation(operation),
		_modelId(modelId)
	{

	}

	SU2dataExchange::~SU2dataExchange()
	{
		if (_stream != nullptr) delete _stream;
	}

	QString SU2dataExchange::readLine()
	{
		while (_threadRuning)
		{
			if (_stream->atEnd())
			{
				//_threadRuning = false;
				return QString();;
			}
			QString line = _stream->readLine().toLower().simplified();
			if (line.isEmpty()) continue;
			return line;
		}
		return QString();
	}

	bool SU2dataExchange::read()
	{
		QFileInfo info(_fileName);
		if (!info.exists()) return false;
		QString name = info.fileName();
		QString path = info.filePath();
		QFile file(_fileName);
		if (!file.open(QIODevice::ReadOnly)) return false;
		_stream = new QTextStream(&file);
		//_gird = vtkUnstructuredGrid::New();

		QString line;
		bool success = false;
		while (!_stream->atEnd())
		{
			if (!_threadRuning) return false;
			line = this->readLine();
			if (line.contains("zone="))
			{
				//line = readLine();
				//_nodeNum = line.toInt();
			}
			else if (line.contains("ndime="))
			{
				//line = readLine();
				//_nodeNum = line.toInt();
			}
			else if (line == "nelements")
			{
// 				line = readLine();
// 				_cellNum = line.toInt();
			}
			else if (line.contains("nelem="))
			{
				_cellNum = line.remove("nelem").remove("=").toInt();
				vtkUnstructuredGrid* grid = vtkUnstructuredGrid::New();
				_gridList.append(grid);
				success = this->readElements();
			}
			else if (success && line.contains("npoin"))
			{
				_nodeNum = line.remove("npoin").remove("=").toInt();
				success = this->readNodes();
				_gridCount++;
			}
		}

		if (!success)
		{
			//_gird->Delete();
			//_gird = nullptr;
			//return false;
			for (int i = 0; i < _gridCount; i++)
			{
				vtkUnstructuredGrid* grid = _gridList.at(i);
				if (grid == nullptr) continue;
				grid->Delete();
				grid = nullptr;
			}

			return false;
		}

// 		MeshKernal* k = new MeshKernal;
// 		k->setName(name);
// 		k->setPath(path);
// 		k->setMeshData(_gird);
// 		_meshData->appendMeshKernal(k);
		for (int i = 0; i < _gridCount; i++)
		{
			MeshKernal* k = new MeshKernal;
			k->setName(name);
			k->setPath(path);
			k->setMeshData(_gridList.at(i));
			_meshData->appendMeshKernal(k);
		}

		return success;
	}

	bool SU2dataExchange::readNodes()
	{
// 		QString line = readLine();
// 		line = readLine();
		QString line;
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		bool success = false;
		QStringList lineInfo;
		for (int i = 0; i < _nodeNum; ++i)
		{
			if (!_threadRuning) return false;
			line = readLine();
			lineInfo = line.split(" ");

			double coor[3] = { 0 };

			if (lineInfo.size() != 4) continue;
			const int id = lineInfo.at(3).toInt();
			//_nodeIdIndex.insert(id, i);
			for (int j = 0; j < 3; ++j)
			{
				coor[j] = lineInfo.at(j).toDouble(&success);
				if (!success) return false;
			}

			points->InsertNextPoint(coor);
		}
		//_gird->SetPoints(points);
		_gridList.at(_gridCount)->SetPoints(points);
		return true;
	}

	bool SU2dataExchange::readElements()
	{
// 		QString line = readLine();
// 		line = readLine();
		QString line;
		bool ok = false;
		for (int i = 0; i < _cellNum; ++i)
		{
			if (!_threadRuning) return false;
			line = readLine();

			VTKCellType tp = VTK_EMPTY_CELL;
			QStringList sids = line.split(" ");
			if (sids.size() < 3) continue;
			int type = sids.at(0).toInt();
			switch (type)
			{
			case 3: tp = VTK_TRIANGLE; break;
			case 4: tp = VTK_QUAD; break;
			case 10: tp = VTK_TETRA; break;
			default: break;
			}
			//tp = VTK_QUAD;//test
			vtkSmartPointer<vtkIdList> indexList = vtkSmartPointer<vtkIdList>::New();
			for (int j = 1; j < sids.size() - 1; ++j)
			{
				if (!_threadRuning) return false;
				int nodeid = sids.at(j).toInt(&ok);
// 				const int index = _nodeIdIndex.value(nodeid);
// 				if (!ok) return false;
//				indexList->InsertNextId(index);
				indexList->InsertNextId(nodeid);
			}
			//_gird->InsertNextCell(tp, indexList);
			_gridList.at(_gridCount)->InsertNextCell(tp, indexList);
		}
		return true;
	}

	void SU2dataExchange::run()
	{		
		ModuleBase::ThreadTask::run();
		bool result = false;
		switch (_operation)
		{
		case MESH_READ:
			emit showInformation(tr("Import SU2 Mesh File From \"%1\"").arg(_fileName));
			result = read();
			setReadResult(result);
			break;
		case MESH_WRITE:
			emit showInformation(tr("Export SU2 Mesh File From \"%1\"").arg(_fileName));
//			result = write();
//			setWriteResult(result);
			break;
		}
		defaultMeshFinished();
	}

	bool SU2dataExchange::write()
	{
		if (_modelId < 1)	return false;
	}
}
