#include "KEYdataExchange.h"
#include "meshDataExchangePlugin.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include <QFile>
#include <QTextStream>
#include <vtkQuad.h>
#include <vtkHexahedron.h>

namespace MeshData
{
	KEYdataExchange::KEYdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId) :
		_meshData(MeshData::getInstance()),
		_operation(operation),
		_fileName(fileName),
		_stream(nullptr),
		MeshThreadBase(fileName, operation, mw),
		_modelId(modelId)
	{
		
	}

	KEYdataExchange::~KEYdataExchange()
	{
		if (_stream != nullptr)
		{
			delete _stream;
			_stream = nullptr;
		}
	}

	void KEYdataExchange::run()
	{
		ModuleBase::ThreadTask::run();
		bool result = false;
		switch (_operation)
		{
		case MESH_READ:
			emit showInformation(tr("Import KEY Mesh File From \"%1\"").arg(_fileName));
			result = read();
			setReadResult(result);
			break;
		case MESH_WRITE:
			emit showInformation(tr("Export KEY Mesh File From \"%1\"").arg(_fileName));
			result = write();
			setWriteResult(result);
			break;
		}		
		defaultMeshFinished();
	}

	bool KEYdataExchange::read()
	{
		QFile file(_fileName);
		if (!file.open(QIODevice::ReadOnly))	return false;
		_stream = new QTextStream(&file);

		QString aLine{};		
		vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
		auto mk = new MeshKernal;
		mk->setMeshData(grid);
		const int mkID = mk->getID();
		while (!_stream->atEnd())
		{
			if (!_threadRuning) return false;
			
			readLine(aLine);
			if (aLine.startsWith("*NODE"))		
				readNodes(grid, aLine);		
			else if (aLine.startsWith("$#SET_ELEMENT"))			
				readElements(grid, aLine);			
			else if (_preLine == "*SET_SOLID_LIST" && aLine.startsWith("$#"))		
				readMeshGroups(mkID, aLine);			
		}		
		if (grid->GetNumberOfPoints() < 1 || grid->GetNumberOfCells() < 1)
			return false;
		
		mk->setName(QString("KEY_%1").arg(mkID));
		mk->appendProperty("Points", (int)grid->GetNumberOfPoints());
		mk->appendProperty("Cells", (int)grid->GetNumberOfCells());
		_meshData->appendMeshKernal(mk);
		_meshData->generateDisplayDataSet();
		file.close();
		return true;
	}

	bool KEYdataExchange::readNodes(vtkSmartPointer<vtkUnstructuredGrid> grid, QString& aLine)
	{
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		while (!_stream->atEnd())
		{
			if (!_threadRuning) return false;
			readLine(aLine);
			QStringList lineList = aLine.split(',');
			if (lineList.size() == 4)
			{				
				double nodeX  = lineList[1].toDouble();
				double nodeY  = lineList[2].toDouble();
				double nodeZ  = lineList[3].toDouble();
				int    nodeID = lineList[0].toInt();
				points->InsertNextPoint(nodeX, nodeY, nodeZ);				
			}
			else break;
		}
		grid->SetPoints(points);
		return true;
	}

	bool KEYdataExchange::readElements(vtkSmartPointer<vtkUnstructuredGrid> grid, QString& aLine)
	{
		while (!_stream->atEnd())
		{
			if (!_threadRuning) return false;
			readLine(aLine);
			setGridCells(grid, aLine);
			if (aLine == "*ELEMENT_SOLID")	break;
			if (aLine == "*SET_SOLID_LIST")
			{
				_preLine = aLine;
				break;
			}
		}
		return true;
	}

	bool KEYdataExchange::readMeshGroups(const int mkID, QString& aLine)
	{
		_preLine.clear();
		QString setName = aLine.split('#').at(1).trimmed();
		MeshSet* ms = new MeshSet(setName, Element);
		if (ms == nullptr)	return false;
		_meshData->appendMeshSet(ms);
		readLine(aLine);
		int setID = aLine.toInt();
		while (!_stream->atEnd())
		{
			if (!_threadRuning) return false;
			readLine(aLine);
			if (aLine == "*END")	break;

			if (aLine == "*SET_SOLID_LIST")
			{
				_preLine = aLine;
				break;
			}
			else 
			{
				QStringList lineList = aLine.split(',');
				for (QString str : lineList)
				{
					int cellIndex = str.toInt() - 1;
					ms->appendMember(mkID, cellIndex);
				}
			}
		}
	}

	void KEYdataExchange::setGridCells(vtkSmartPointer<vtkUnstructuredGrid> grid, QString& aLine)
	{		
		while (!_stream->atEnd())
		{
			if (!_threadRuning) return;
			QStringList lineList = aLine.split(',');
			if (lineList.size() == 10)
			{
				int elementID = lineList[0].toInt();
				QString elementType = lineList[1];
				int nodeIndex0 = lineList[2].toInt() - 1;
				int nodeIndex1 = lineList[3].toInt() - 1;
				int nodeIndex2 = lineList[4].toInt() - 1;
				int nodeIndex3 = lineList[5].toInt() - 1;

				if (lineList[5] == lineList[6])
				{
					vtkSmartPointer<vtkQuad> aQuadCell = vtkSmartPointer<vtkQuad>::New();					
					aQuadCell->GetPointIds()->SetId(0, nodeIndex0);
					aQuadCell->GetPointIds()->SetId(1, nodeIndex1);
					aQuadCell->GetPointIds()->SetId(2, nodeIndex2);
					aQuadCell->GetPointIds()->SetId(3, nodeIndex3);					
					grid->InsertNextCell(aQuadCell->GetCellType(), aQuadCell->GetPointIds());
				}
				else
				{
					vtkSmartPointer<vtkHexahedron> aHexahedronCell = vtkSmartPointer<vtkHexahedron>::New();
					aHexahedronCell->GetPointIds()->SetId(0, nodeIndex0);
					aHexahedronCell->GetPointIds()->SetId(1, nodeIndex1);
					aHexahedronCell->GetPointIds()->SetId(2, nodeIndex2);
					aHexahedronCell->GetPointIds()->SetId(3, nodeIndex3);
					aHexahedronCell->GetPointIds()->SetId(4, lineList[6].toInt() - 1);
					aHexahedronCell->GetPointIds()->SetId(5, lineList[7].toInt() - 1);
					aHexahedronCell->GetPointIds()->SetId(6, lineList[8].toInt() - 1);
					aHexahedronCell->GetPointIds()->SetId(7, lineList[9].toInt() - 1);
					grid->InsertNextCell(aHexahedronCell->GetCellType(), aHexahedronCell->GetPointIds());
				}
				readLine(aLine);
			}
			else if (aLine == "*ELEMENT_SOLID" || aLine == "*SET_SOLID_LIST")	break;
		}
	}

	void KEYdataExchange::readLine(QString& aLine)
	{
		while (_threadRuning)
		{
			if (_stream->atEnd())
			{
				//_threadRuning = false;
				break;
			}
			aLine = _stream->readLine().simplified();
			if (aLine.isEmpty())	continue;
			break;
		}
	}

	bool KEYdataExchange::write()
	{
		//if (_modelId < 1)	return false;
		QFile file(_fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))	return false;
		_stream = new QTextStream(&file);
		int a = _meshData->getKernalCount();

		auto mk = _meshData->getKernalByID(_modelId);
		//_meshData->
		auto grid = mk->getMeshData();
		//vtkUnstructuredGrid* grid = dynamic_cast<vtkUnstructuredGrid*>(mk->getMeshData());
		//writePointPart(grid);
		//writeCellPart(grid);
		writeMeshPart();
		file.close();
		return true;
	}

	bool KEYdataExchange::writePointPart(vtkDataSet* grid)
	{
		int index{ 0 };
		int numOfPoint = grid->GetNumberOfPoints();
		*_stream << QString("   NODES   %1").arg(numOfPoint) << endl << endl;
		while (index < numOfPoint)
		{
			if (!_threadRuning) return false;
			double * array = grid->GetPoint(index);
			index++;
			*_stream << QString("   %1   %2   %3   %4").arg(index).arg(array[0]).arg(array[1]).arg(array[2]) << endl;
		}		
		return true;
	}

	bool KEYdataExchange::writeCellPart(vtkDataSet* grid)
	{
		int index{ 0 }, hexahedronCount{ 0 };
		int numOfCell = grid->GetNumberOfCells();
		*_stream << QString("   ELEMENTS   %1").arg(numOfCell) << endl << endl;
		while (index < numOfCell)
		{
			if (!_threadRuning) return false;
			vtkCell *cell = grid->GetCell(index);
			if (cell == nullptr) continue;
			if (cell->GetCellType() == VTK_HEXAHEDRON)
			{
				hexahedronCount++;
				vtkIdList* PointIdIndexs = cell->GetPointIds();
				*_stream << QString("   %1   %2   %3   %4   %5   %6   %7   %8   %9").arg(hexahedronCount).arg(PointIdIndexs->GetId(0) + 1).arg(PointIdIndexs->GetId(1) + 1).arg(PointIdIndexs->GetId(2) + 1).arg(PointIdIndexs->GetId(3) + 1).arg(PointIdIndexs->GetId(4) + 1).arg(PointIdIndexs->GetId(5) + 1).arg(PointIdIndexs->GetId(6) + 1).arg(PointIdIndexs->GetId(7) + 1) << endl;
			}
			index++;
		}
		return false;
	}

	bool KEYdataExchange::writeMeshPart()
	{
		QList<int> setIds = _meshData->getSetIDFromKernal(_modelId);
		for (int setId : setIds)
		{
			auto ms = _meshData->getMeshSetByID(setId);
		}	
		return false;
	}
}