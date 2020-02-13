#include "NeuReader.h"
#include "meshSingleton.h"
#include "meshKernal.h"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkQuad.h>
#include <vtkAppendFilter.h>
#include <vtkTriangle.h>
#include <vtkDataSet.h>
#include <QDebug>

namespace MeshData
{
	NeuReader::NeuReader(const QString &filename)
	{
		_fileName = filename;
		_meshData = MeshData::getInstance();
	}
	NeuReader::~NeuReader()
	{
		if (_stream) delete _stream;
	}
	QString NeuReader::readLine()
	{
		while (!_stream->atEnd())
		{
			QString line = _stream->readLine().simplified();
			if (line.isEmpty()) continue;
			return line;
		}
		return "";
	}
	bool NeuReader::read()
	{
		QFileInfo info(_fileName);
		if (!info.exists()) return false;
		QString name = info.fileName();
		QString path = info.filePath();
		QFile file(_fileName);
		if (!file.open(QIODevice::ReadOnly)) return false;
		_stream = new QTextStream(&file);
		if (!readBasicInfo()) return false;
		vtkSmartPointer<vtkUnstructuredGrid> dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
//		vtkUnstructuredGrid* dataset = vtkUnstructuredGrid::New();
		if (!readPoints(dataset))
		{
			dataset->Delete();
//			dataset = nullptr;
			return false;
		}
		if (!readElements(dataset))
		{
			dataset->Delete();
//			dataset = nullptr;
			return false;
		}
		file.close();

		if (dataset != nullptr)
		{
			MeshKernal* k = new MeshKernal;
			k->setName(name);
			k->setPath(path);
			k->setMeshData((vtkDataSet*)dataset);
			_meshData->appendMeshKernal(k);
			return true;
		}
		
		return false;

	}
	bool NeuReader::readBasicInfo()
	{
		QString line;
		while (!_stream->atEnd())
		{
			line = this->readLine();
			if (line.contains("NUMNP") && line.contains("NELEM"))
			{
				line = this->readLine();
				break;
			}
		}
		QStringList info = line.split(" ");
		_pointNum = info.at(0).toInt();
		_eleNum = info.at(1).toInt();
		return true;
	}
	bool NeuReader::readPoints(vtkUnstructuredGrid* dateset)
	{
		QString line;
		bool ok = false;
		while (!_stream->atEnd())
		{
			line = readLine();
			if (line.contains("NODAL COORDINATES")) break;
		}
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
//		vtkPoints* points = vtkPoints::New();
		for (int i = 0; i < _pointNum; ++i)
		{
			line = readLine();
			double coor[3] = { 0 };
			QStringList scoor = line.split(" ");
			if (scoor.size() != 4) continue;
			for (int j = 0; j < 3; ++j)
			{
				coor[j] = scoor.at(j + 1).toDouble(&ok);
				if (!ok) return false;
			}
			points->InsertNextPoint(coor);
		}
		if (points->GetNumberOfPoints() != _pointNum)
		{
			points->Delete();
			return false;
		}
		dateset->SetPoints(points);
		return true;

	}
	bool NeuReader::readElements(vtkUnstructuredGrid* dateset)
	{
		QString line;
		while (!_stream->atEnd())
		{
			line = readLine();
			if (line.contains("ELEMENTS/CELLS")) break;
		}
		vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
//		vtkCellArray* cells = vtkCellArray::New();
		for (int i = 0; i < _eleNum; ++i)
		{
			line = readLine();
			QStringList scell = line.split(" ");
//			vtkSmartPointer<vtkQuad> cell = vtkSmartPointer<vtkQuad>::New();
//			vtkQuad* cell = vtkQuad::New();
			if (scell.size() == 7)
			{
				vtkSmartPointer<vtkQuad> cell = vtkSmartPointer<vtkQuad>::New();
				cell->GetPointIds()->SetId(0, scell[3].toInt()-1);
				cell->GetPointIds()->SetId(1, scell[4].toInt()-1);
				cell->GetPointIds()->SetId(2, scell[5].toInt()-1);
				cell->GetPointIds()->SetId(3, scell[6].toInt()-1);
				cells->InsertNextCell(cell);
			}
			else if (scell.size() == 6)
			{
				vtkSmartPointer<vtkTriangle> cell = vtkSmartPointer<vtkTriangle>::New();
 				cell->GetPointIds()->SetId(0, scell[3].toInt()-1);
 				cell->GetPointIds()->SetId(1, scell[4].toInt()-1);
 				cell->GetPointIds()->SetId(2, scell[5].toInt()-1);
		//		cell->GetPointIds()->SetId(3, scell[5].toInt()-1);
				cells->InsertNextCell(cell);
			}
			else continue;
//			cells->InsertNextCell(cell);
		}
		if (cells->GetNumberOfCells() != _eleNum)
		{
			cells->Delete();
			return false;
		}
		dateset->SetCells(VTK_QUAD, cells);
		return true;
	}
	bool NeuReader::write(int id)
	{
		QFile file(_fileName);
		if (!file.open(QIODevice::WriteOnly)) return false;
		_stream = new QTextStream(&file);
		_stream->setCodec("UTF-8");
		writeBasicInfo();
		//combine dataset
		if (id == -1)
		{
			vtkSmartPointer<vtkAppendFilter> appfilter = vtkSmartPointer<vtkAppendFilter>::New();
			int n = _meshData->getKernalCount();
			for (int i = 0; i < n; ++i)
			{
				MeshKernal* k = _meshData->getKernalAt(i);
				vtkDataSet* dataset = k->getMeshData();
				appfilter->AddInputData(dataset);
			}
			appfilter->Update();
			vtkSmartPointer<vtkUnstructuredGrid> unsGrid = appfilter->GetOutput();
			writepoints(unsGrid);
			writeElements(unsGrid);
		}
		else
		{
			MeshKernal* k = _meshData->getKernalByID(id);
			if (k == nullptr) return false;
			vtkDataSet* dataset = k->getMeshData();

			writepoints(dataset);
			writeElements(dataset);
		}
		

		return true;
	}
	void NeuReader::writeBasicInfo()
	{
		*_stream << QString("        CONTROL INFO 2.4.6") << endl;
		*_stream << QString("** GAMBIT NEUTRAL FILE") << endl;
		*_stream << _fileName << endl;
		*_stream << QString("PROGRAM:                Gambit     VERSION:  2.4.6") << endl<<endl;

	}
	void NeuReader::writepoints(vtkDataSet* dateset)
	{
		*_stream << QString("     NUMNP     NELEM     NGRPS    NBSETS     NDFCD     NDFVL")<<endl;
		*_stream << QString("      %1      %2      1         0         2         3").arg(dateset->GetNumberOfPoints()).arg(dateset->GetNumberOfCells())<<endl;
		*_stream << QString("ENDOFSECTION") << endl;
		*_stream << QString("   NODAL COORDINATES 2.4.6") <<endl;
		const int n = dateset->GetNumberOfPoints();
		for (int i = 0; i < n; ++i)
		{
			_stream->setFieldWidth(10);
			_stream->setFieldAlignment(QTextStream::AlignRight);
			*_stream << i + 1;
			double coord[3] = { 0.0 };
			dateset->GetPoint(i, coord);
			_stream->setFieldWidth(20);
			for (int i = 0; i < 3; ++i)
			{
				*_stream << QString::number(coord[i], 'g', 11);
			}
			*_stream << endl;
		}
		_stream->setFieldWidth(0);
		*_stream << QString("ENDOFSECTION") << endl;
	}
	void NeuReader::writeElements(vtkDataSet* dateset)
	{
		*_stream << QString("      ELEMENTS/CELLS 2.4.6") << endl;
		const int n = dateset->GetNumberOfCells();
		for (int i = 0; i < n; ++i)
		{
			_stream->setFieldWidth(8);
			*_stream << i + 1;
			_stream->setFieldWidth(3);
			*_stream << 0;
			vtkCell* cell = dateset->GetCell(i);
			const int np = cell->GetNumberOfPoints();
			*_stream << np<<" ";
			_stream->setFieldWidth(8);
			for (int ip = 0; ip < np; ++ip)
			{
				int id = cell->GetPointId(ip);
				*_stream << id + 1;
			}
			*_stream << endl;
		}
		_stream->setFieldWidth(0);
		*_stream << QString("ENDOFSECTION");
	}



}