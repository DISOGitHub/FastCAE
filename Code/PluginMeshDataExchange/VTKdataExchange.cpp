#include "VTKdataExchange.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
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

namespace MeshData
{
	VTKdataExchange::VTKdataExchange(const QString& fileName, MeshOperation operation, GUI::MainWindow *mw, int KernalId) :
		MeshThreadBase(fileName, operation, mw),
		_fileName(fileName),
		_meshData(MeshData::getInstance()),
		_operation(operation),
		_writeFileKid(KernalId)
	{

	}

	bool VTKdataExchange::read()
	{
		QFileInfo info(_fileName);
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
				if (!_threadRuning) return false;
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

	vtkDataSet* VTKdataExchange::readVTK()
	{
		if (!_threadRuning) return false;
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(_fileName);
		_vtkReader = vtkSmartPointer<vtkDataSetReader>::New();
		_vtkReader->SetFileName(ba);
		_vtkReader->Update();
		vtkSmartPointer<vtkDataSet> dataset = _vtkReader->GetOutput();
		return dataset;
	}

	vtkDataSet* VTKdataExchange::readSTL()
	{
		if (!_threadRuning) return false;
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(_fileName);
		_stlReader = vtkSmartPointer<vtkSTLReader>::New();
		_stlReader->SetFileName(ba);
		_stlReader->SetMerging(true);
		_stlReader->Update();
		return _stlReader->GetOutput();
	}

	vtkDataSet* VTKdataExchange::readTecplot()
	{
		if (!_threadRuning) return false;
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(_fileName);
		_tecplotReader = vtkSmartPointer<vtkTecplotReader>::New();
		_tecplotReader->SetFileName(ba);
		_tecplotReader->Update();
		vtkMultiBlockDataSet* mudataset = _tecplotReader->GetOutput();
	//	blockDataSet = mudataset;//add for multi zone test
		const int nblock = mudataset->GetNumberOfBlocks();
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

	bool VTKdataExchange::write()
	{
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(_fileName);
		vtkUnstructuredGrid* ungird = nullptr;
		vtkSmartPointer<vtkAppendFilter> filter = vtkSmartPointer<vtkAppendFilter>::New();
		if (_writeFileKid > 0)
		{
			MeshKernal* kernal = _meshData->getKernalByID(_writeFileKid);
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
				if (!_threadRuning) return false;
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

	void VTKdataExchange::run()
	{				
		ModuleBase::ThreadTask::run();
		bool result = false;
		switch (_operation)
		{
		case MESH_READ:
			emit showInformation(tr("Import VTK Mesh File From \"%1\"").arg(_fileName));
			result = read();
			setReadResult(result);
			break;
		case MESH_WRITE:
			emit showInformation(tr("Export VTK Mesh File To \"%1\"").arg(_fileName));
			result = write();
			setWriteResult(result);
			break;
		}
		defaultMeshFinished();
	}
}