#include "MeshThreadBase.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "python/PyAgent.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"

namespace MeshData
{
	MeshThreadBase::MeshThreadBase(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw) :
		ModuleBase::ThreadTask(mw),
		_fileName(fileName),
		_operation(operation)
	{
		
	}

	MeshThreadBase::~MeshThreadBase()
	{

	}

	void MeshThreadBase::defaultMeshFinished()
	{
		
		if (_threadRuning)
		{
			QString information{};
			ModuleBase::Message msg;
			if (_operation == MESH_READ)
			{
				if (_success)
				{
					emit _mainwindow->updateMeshTreeSig();
					emit _mainwindow->updateSetTreeSig();
					emit _mainwindow->updateActionStatesSig();
					emit _mainwindow->updateActionsStatesSig();
					emit _mainwindow->getSubWindowManager()->openPreWindowSig();
					emit _mainwindow->updatePreMeshActorSig();
					information = QString("Successful Import Mesh From \"%1\"").arg(_fileName);
					msg.type = ModuleBase::Normal_Message;
					msg.message = information;
					auto meshdata = MeshData::getInstance();
					meshdata->generateDisplayDataSet();
					const int nk = meshdata->getKernalCount();
					if (nk <= 0)return;
					auto k = meshdata->getKernalAt(nk - 1);
					if (k != nullptr) k->setPath(_fileName);
				}
				else
				{
					information = QString("Failed Import Mesh From \"%1\"").arg(_fileName);
					msg.type = ModuleBase::Error_Message;
					msg.message = information;
				}
			}
			else if (_operation == MESH_WRITE)
			{
				if (_success)
				{
					information = QString("Successful Export Mesh To \"%1\"").arg(_fileName);
					msg.type = ModuleBase::Normal_Message;
					msg.message = information;
				}
				else
				{
					information = QString("Failed Export Mesh To \"%1\"").arg(_fileName);
					msg.type = ModuleBase::Error_Message;
					msg.message = information;
				}
			}
			emit showInformation(information);
			emit _mainwindow->printMessageToMessageWindow(msg);
		}
// 		emit showInformation(information);
// 		emit _mainwindow->printMessageToMessageWindow(msg);
		ModuleBase::ThreadTask::threadTaskFinished();
		Py::PythonAagent::getInstance()->unLock();
	}

	void MeshThreadBase::setReadResult(bool result)
	{
		_success = result;
	}

	void MeshThreadBase::setWriteResult(bool result)
	{
		_success = result;
	}
}