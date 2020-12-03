#include "PreWindowManager.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "MainWidgets/preWindow.h"
#include "MainWidgets/meshViewProvider.h"
#include "mainWindow/signalHandler.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include <QTextCodec>
#include <vtkDataSetReader.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkProperty.h>

namespace Geant4
{
	PreWindowManager::PreWindowManager(GUI::MainWindow* mw)
	{
		_mainWindow = mw;

		if (_mainWindow == nullptr) return;
		connect(mw, SIGNAL(preWindowOpenedSig(MainWidget::PreWindow*)), this, SLOT(preWindowOpened(MainWidget::PreWindow*)));
		connect(mw, SIGNAL(closePreWindowSig()), this, SLOT(preWindowClosed()));

		_preWindow = _mainWindow->getSubWindowManager()->getPreWindow();
		if (_preWindow != nullptr)
		{
			_viewProvider = _preWindow->getMeshViewProvider();
			connect(_mainWindow, SIGNAL(updateGraphOptionsSig()), this, SLOT(updataTrackProperty()));
		}
			
	}



	void PreWindowManager::preWindowOpened(MainWidget::PreWindow* p)
	{
		_preWindow = p;
		if (_preWindow != nullptr)
			_viewProvider = _preWindow->getMeshViewProvider();
		connect(_mainWindow, SIGNAL(updateGraphOptionsSig()), this, SLOT(updataTrackProperty()));
	}

	void PreWindowManager::preWindowClosed()
	{
		_preWindow = nullptr;
		_viewProvider = nullptr;
		disconnect(_mainWindow, SIGNAL(updateGraphOptionsSig()), this, SLOT(updataTrackProperty()));
	}



	int PreWindowManager::addTrack(int type, QString file)
	{
		int id = 0;

		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray ba = codec->fromUnicode(file);
		vtkSmartPointer<vtkDataSetReader> vtkReader = vtkSmartPointer<vtkDataSetReader>::New();
		vtkReader->SetFileName(ba);
		vtkReader->Update();
		vtkSmartPointer<vtkDataSet> dataset = vtkReader->GetOutput();

		if (dataset == nullptr) return 0;

		id = addTrack(type, dataset);

		return id;
	}

	int PreWindowManager::addTrack(int type, vtkDataSet* dataset)
	{
		auto meshData = MeshData::MeshData::getInstance();
		const int n = meshData->getKernalCount();
	    MeshData::MeshKernal* k = new MeshData::MeshKernal;
	    int id = k->getID();
	    k->setName(QString("Track_%1").arg(id));
	    k->setMeshData(dataset);
	    meshData->appendMeshKernal(k);
		switch (type)
		{
		case 1:
			k->setSpecificColor(true,0, 0, 1.0); break;
		case 2:
			k->setSpecificColor(true, 1.0, 0, 0); break;
		case 3:
			k->setSpecificColor(true, 0, 1.0, 0); break;
		default:break;
		}
	    emit _mainWindow->updateMeshTreeSig();

		if (_preWindow == nullptr)
			_mainWindow->getSubWindowManager()->openPreWindow();
		else
			_preWindow->updateMeshActor();

		_typeKernalHash.insert(type, k);

//		updataTrackProperty();

	   return id;
	}

	bool PreWindowManager::removeTrack(int id)
	{

		auto meshdata = MeshData::MeshData::getInstance();
		auto  k = meshdata->getKernalByID(id);
		if (k == nullptr)  return false;
		meshdata->removeKernalByID(id);
		emit _mainWindow->updateMeshTreeSig();
		_preWindow->updateMeshActor();
//		updataTrackProperty();
		return  true;
	}

}


