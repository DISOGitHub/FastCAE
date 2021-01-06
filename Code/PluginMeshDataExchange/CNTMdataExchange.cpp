#include "CNTMdataExchange.h"
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
	CNTMdataExchange::CNTMdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId) :
		MeshThreadBase(fileName, operation, mw),
		_fileName(fileName),
		_meshData(MeshData::getInstance()),
		_operation(operation),
		_modelId(modelId)
	{

	}

	CNTMdataExchange::~CNTMdataExchange()
	{
		if (_stream != nullptr) delete _stream;
	}

	QString CNTMdataExchange::readLine()
	{		
		while (_threadRuning)
		{
			if (_stream->atEnd())
			{
				//_threadRuning = false;
				return QString();
			}
			QString line = _stream->readLine().simplified().toLower();
			if (line.isEmpty()) continue;
			return line;
		}	
		return QString();
	}

	bool CNTMdataExchange::read()
	{
		QFileInfo info(_fileName);
		if (!info.exists()) return false;
		QString name = info.fileName();
		QString path = info.filePath();
		QFile file(_fileName);
		if (!file.open(QIODevice::ReadOnly)) return false;
		_stream = new QTextStream(&file);
		_gird = vtkUnstructuredGrid::New();

		QString line;
		bool success = false;
		while (!_stream->atEnd())
		{
			if (!_threadRuning) return false;

			line = this->readLine();
			if (line == "nnodes")
			{
				line = readLine();
				_nodeNum = line.toInt();
			}
			else if (line == "nelements")
			{
				line = readLine();
				_cellNum = line.toInt();
			}
			else if (line == "nodal coordinates")
			{
				success = this->readNodes();
			}
			else if (success && line == "elements")
			{
				success = this->readElements();
			}
		}

		if (!success)
		{
			_gird->Delete();
			_gird = nullptr;
			return false;
		}

		MeshKernal* k = new MeshKernal;
		k->setName(name);
		k->setPath(path);
		k->setMeshData(_gird);
		_meshData->appendMeshKernal(k);
		
		return success;
	}

	bool CNTMdataExchange::write()
	{
		if (_modelId < 1)	return false;
	}

	bool CNTMdataExchange::readNodes()
	{
		QString line = readLine();
		line = readLine();

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
			const int id = lineInfo.at(0).toInt();
			_nodeIdIndex.insert(id, i);
			for (int j = 0; j < 3; ++j)
			{
				coor[j] = lineInfo.at(j + 1).toDouble(&success);
				if (!success) return false;
			}

			points->InsertNextPoint(coor);
		}
		_gird->SetPoints(points);
		return success;
	}

	bool CNTMdataExchange::readElements()
	{
		QString line = readLine();
		line = readLine();

		bool ok = false;
		for (int i = 0; i < _cellNum; ++i)
		{
			if (!_threadRuning) return false;

			line = readLine();
			VTKCellType tp = VTK_EMPTY_CELL;
			QStringList sids = line.split(" ");
			if (sids.size() <3) continue;
			int type = sids.at(1).toInt();
			switch (type)
			{
			case 3: tp = VTK_TRIANGLE; break;
			case 4: tp = VTK_QUAD; break;
			default: break;
			}

			vtkSmartPointer<vtkIdList> indexList = vtkSmartPointer<vtkIdList>::New();
			for (int j = 2; j < sids.size(); ++j)
			{
				int nodeid = sids.at(j).toInt(&ok);
				const int index = _nodeIdIndex.value(nodeid);
				if (!ok) return false;
				indexList->InsertNextId(index);
			}
			_gird->InsertNextCell(tp, indexList);
		}
		return true;
	}

	void CNTMdataExchange::run()
	{				
		ModuleBase::ThreadTask::run();
		bool result = false;
		switch (_operation)
		{
		case MESH_READ:
			emit showInformation(tr("Import CNTM Mesh File From \"%1\"").arg(_fileName));
			result = read();
			setReadResult(result);
			break;
		case MESH_WRITE:
			emit showInformation(tr("Export CNTM Mesh File From \"%1\"").arg(_fileName));
//			result = write();
//			setWriteResult(result);
			break;
		}
		defaultMeshFinished();
	}
}
