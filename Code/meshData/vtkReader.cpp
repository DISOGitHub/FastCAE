#include "vtkReader.h"
#include "meshSingleton.h"
#include "meshKernal.h"
#include <vtkSmartPointer.h>
#include <vtkDataSetReader.h>
#include <vtkDataSet.h>
#include <vtkSTLReader.h>
#include <vtkTecplotReader.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkAppendFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkAppendFilter.h>
#include <QFileInfo>
#include <QTextCodec>
#include <QDebug>

namespace MeshData
{
	VTKReader::VTKReader(const QString& filename)
	{
		_filename = filename;
		_meshData = MeshData::getInstance();
	}
	bool VTKReader::read()
	{
		QFileInfo info(_filename);
		if (!info.exists()) return false;
		QString name = info.fileName();
		QString path = info.filePath();
		QString suffix = info.suffix().toLower();
		vtkDataSet* dataset = nullptr;

		if (suffix == "vtk")
		{
			dataset = readVTK();
		}
		else if (suffix == "stl")
		{
			dataset = readSTL();
		}
		else if (suffix == "dat")
		{
			dataset = readTecplot();
		}
		if (dataset != nullptr)
		{
			MeshKernal* k = new MeshKernal;
			k->setName(name);
			k->setPath(path);

			int nKernal = _meshData->getKernalCount();
			for (int iKernal = 0; iKernal < nKernal; ++iKernal)
			{
				MeshKernal* temp = _meshData->getKernalAt(iKernal);
				if (temp->getPath() == path)
				{	///<MG same file update
					_meshData->removeKernalAt(iKernal);
					break;
				}
			}

			k->setMeshData(dataset);
			_meshData->appendMeshKernal(k);
			return true;
		}
		return false;
	}

	vtkDataSet* VTKReader::readVTK()
	{
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(_filename);
		_vtkReader = vtkSmartPointer<vtkDataSetReader>::New();
		_vtkReader->SetFileName(ba);
		_vtkReader->Update();
		vtkSmartPointer<vtkDataSet> dataset = _vtkReader->GetOutput();
		return dataset;
	}

	vtkDataSet* VTKReader::readSTL()
	{
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(_filename);
		_stlReader = vtkSmartPointer<vtkSTLReader>::New();
		_stlReader->SetFileName(ba);
		_stlReader->SetMerging(true);
		_stlReader->Update();
		return _stlReader->GetOutput();
	}
	vtkDataSet* VTKReader::readTecplot()
	{
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(_filename);
		_tecplotReader = vtkSmartPointer<vtkTecplotReader>::New();
		_tecplotReader->SetFileName(ba);
		_tecplotReader->Update();
		vtkMultiBlockDataSet* mudataset = _tecplotReader->GetOutput();
	//	blockDataSet = mudataset;//add for multi zone test
		const int nblock = mudataset->GetNumberOfBlocks();
		qDebug() << nblock;
		if (nblock <= 0) return nullptr;
		vtkAppendFilter* filter = vtkAppendFilter::New();
		for (int i = 0; i < nblock; ++i)
		{
			vtkDataSet* dataset = vtkDataSet::SafeDownCast(mudataset->GetBlock(i));
			filter->AddInputData(dataset);
		}
		filter->Update();  //put multi zone together
	//	set_GridType(dUNSTRUCTURED_GRID);//mGridType = dUNSTRUCTURED_GRID;
		return vtkDataSet::SafeDownCast(filter->GetOutput());
	}
	bool VTKReader::write(int kenerlID)
	{
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(_filename);
		vtkUnstructuredGrid* ungird = nullptr;
		vtkSmartPointer<vtkAppendFilter> filter = vtkSmartPointer<vtkAppendFilter>::New();
		if (kenerlID > 0)
		{
			MeshKernal* kernal = _meshData->getKernalByID(kenerlID);
			if (kernal != nullptr)
			{
				vtkDataSet* dataset = kernal->getMeshData();
				if (dataset != nullptr)
					filter->AddInputData(dataset);
			}
		}
		else
		{
			const int n = _meshData->getKernalCount();
			for (int i = 0; i < n; ++i)
			{
				MeshKernal* kernal = _meshData->getKernalAt(i);
				if (kernal != nullptr)
				{
					vtkDataSet* dataset = kernal->getMeshData();
					if (dataset != nullptr)
						filter->AddInputData(dataset);
				}
			}
		}
		filter->Update();
		ungird = filter->GetOutput();
		vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
		writer->SetInputData(ungird);
		writer->SetFileTypeToBinary();
		writer->SetFileName(ba);
		writer->Write();

		return true;
	}

}