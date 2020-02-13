#include "MeshWidget.h"
#include "mainWindow/mainWindow.h"
#include "DataProperty/modelTreeItemType.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QFile>
#include "preWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/MeshConfig.h"
#include "settings/busAPI.h"
#include "SolverControl/MesherControlerBase.h"
#include "DialogMeshSetMerge.h"
#include "DialogMeshRename.h"

namespace MainWidget
{
	MeshWidget::MeshWidget(GUI::MainWindow* mw, QWidget* parent /* = nullptr */) 
		:_mainWindow(mw)
	{
		_data = MeshData::MeshData::getInstance();
		this->setHeaderHidden(true);
		_meshRoot = new QTreeWidgetItem(this, TreeItemType::MeshRoot);
		_meshRoot->setText(0, tr("Mesh"));
		_meshRoot->setIcon(0, QIcon(":/QUI/icon/mesh.png"));
		_meshRoot->setExpanded(true);
		_setRoot = new QTreeWidgetItem(this, TreeItemType::MeshSetRoot);
		_setRoot->setText(0, tr("Set"));
		_setRoot->setIcon(0, QIcon(":/QUI/icon/mesh.png"));
		_setRoot->setExpanded(true);
		this->addTopLevelItem(_meshRoot);
		this->addTopLevelItem(_setRoot);

		connect(_mainWindow, SIGNAL(updateMeshTreeSig()), this, SLOT(updateMeshTree()));
		connect(_mainWindow, SIGNAL(updateSetTreeSig()), this, SLOT(updateMeshSetTree()));
		connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(singleClicked(QTreeWidgetItem*, int)));
		connect(this, SIGNAL(updateDisplay(int, bool)), _mainWindow, SIGNAL(updateMeshDispalyStateSig(int, bool)));
		connect(this, SIGNAL(removeMeshData(int)), _mainWindow, SIGNAL(removeMeshActorSig(int)));
		connect(this, SIGNAL(updateActionStates()), _mainWindow, SIGNAL(updateActionStatesSig()));
		connect(this, SIGNAL(disPlayProp(DataProperty::DataBase*)), _mainWindow, SIGNAL(updateProperty(DataProperty::DataBase*)));
		connect(this, SIGNAL(higtLightSet(MeshData::MeshSet*)), _mainWindow, SIGNAL(highLightSetSig(MeshData::MeshSet*)));
		connect(this, SIGNAL(higtLightKernal(MeshData::MeshKernal*)), _mainWindow, SIGNAL(highLightKernelSig(MeshData::MeshKernal*)));
		connect(this, SIGNAL(dispalyParaWidget(QWidget *)), _mainWindow, SIGNAL(updateParaWidget(QWidget*)));
		connect(this, SIGNAL(clearHighLight()), _mainWindow, SIGNAL(clearHighLightSig()));
		connect(this, SIGNAL(startMesherPySig(QString)), this, SLOT(startMesher(QString)));

		
	}
	MeshWidget::~MeshWidget()
	{
		this->clear();
	}
	void MeshWidget::updateTree()
	{
		updateMeshTree();
		updateMeshSetTree();
	}
	void MeshWidget::updateMeshTree()
	{
		_meshRoot->takeChildren();

		_meshRoot->setText(0, tr("Mesh"));
		const int n = _data->getKernalCount();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshKernal* k = _data->getKernalAt(i);
			const QString name = k->getName();
			const int id = k->getID();
			const bool visable = k->isVisible();
			QTreeWidgetItem* item = new QTreeWidgetItem(_meshRoot, TreeItemType::MeshChild);
			item->setData(0, Qt::UserRole, id);
			Qt::CheckState state = Qt::Checked;
			if (!visable) state = Qt::Unchecked;
			item->setText(0, name);
			item->setCheckState(0, state);
			item->setIcon(0, QIcon(":/QUI/icon/mesh.png"));
		}

// 		for (int iCnt = 0; iCnt < _temp_meshs.size(); ++iCnt)
// 		{
// 			QFile file(_temp_meshs.at(iCnt).fromFile);
// 			if (!file.exists()){
// 				_temp_meshs.remove(iCnt);
// 				--iCnt;
// 				continue;
// 			}
// 			QTreeWidgetItem* item = new QTreeWidgetItem(_meshRoot, TreeItemType::MeshChild);
// 			Qt::CheckState state = Qt::Checked;
// 			QString name = _temp_meshs.at(iCnt).fromFile;
// 
// 			if (_temp_meshs.at(iCnt).type == 0)
// 			{	///<MG surface mesh
// 				name = name.split("/").at(name.split("/").size() - 1) + "-Surface";
// 			}
// 			//wzy20180528新增体网格
// 			else if (_temp_meshs.at(iCnt).type==2)
// 			{	///<MG solid mesh
// 				name = name.split("/").at(name.split("/").size() - 1) + "-Solid";
// 			}
// 			else if (_temp_meshs.at(iCnt).type == 1)
// 			{
// 				name = name.split("/").at(name.split("/").size() - 1) + "-Computational domain";
// 			}
// 
// 			item->setData(0, MESH_FROM_ROLE, MESH_FROM_GEOMETRY);
// 			item->setData(0, MESH_GEOMETRY_FILE_ROLE, _temp_meshs.at(iCnt).fromFile);
// 			item->setData(0, MESH_TYPE_ROLE, _temp_meshs.at(iCnt).type);
// 			
// 			item->setText(0, name);
// 			item->setCheckState(0, state);
// 			item->setIcon(0, QIcon(":/QUI/icon/mesh.png"));
// 
// 			if (iCnt == _temp_meshs.size() - 1)
// 			{	///<MG new add geometry to mesh
// 				setCurrentItem(item);
// 				singleClicked(item, 0);
// 			}
// 		}
	}
	void MeshWidget::updateMeshSetTree()
	{
		_setRoot->takeChildren();
		_setRoot->setText(0, tr("Set"));
		const int n = _data->getMeshSetCount();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshSet* s = _data->getMeshSetAt(i);
			const QString name = s->getName();
			const int  id = s->getID();
			QTreeWidgetItem* item = new QTreeWidgetItem(_setRoot, TreeItemType::MeshSetChild);
			item->setData(0, Qt::UserRole, id);
			item->setText(0, name);
			QString icon = "://QUI/icon/nodeset.png";
			if ((s->getSetType() == MeshData::Element) || (s->getSetType() == MeshData::BCZone))
				icon = "://QUI/icon/eleset.png";
			else if (s->getSetType() == MeshData::Family)
				icon = "://QUI/icon/family.png";
			item->setIcon(0, QIcon(icon));
		}
	}

	void MeshWidget::singleClicked(QTreeWidgetItem* item, int i)
	{
		_currentItem = this->currentItem();
		if (_currentItem == nullptr)
		{
			emit higtLightSet(nullptr);
			return;
		}

		if (_currentItem->type() == TreeItemType::MeshChild)
		{
			const int index = _meshRoot->indexOfChild(item);
			if (index < 0)
			{
				emit disPlayProp(nullptr);
				return;
			}

//			if (_currentItem->data(0, MESH_FROM_ROLE).toInt() != MESH_FROM_GEOMETRY)
// 			{

			MeshData::MeshKernal* k = _data->getKernalAt(index);
			if (k == nullptr) return;
			bool visable = true;
			Qt::CheckState state = item->checkState(0);
			if (state != Qt::Checked) visable = false;
			k->setVisible(visable);
			emit updateDisplay(index, visable);
			emit higtLightKernal(k);
			emit disPlayProp(k);
			if (!visable) emit clearHighLight();

		}
		else if (_currentItem->type() == TreeItemType::MeshSetChild)
		{
			const int index = _setRoot->indexOfChild(item);
			if (index <0)
			{
				emit disPlayProp(nullptr);
				emit higtLightSet(nullptr);
				return;
			}
			MeshData::MeshSet* set = _data->getMeshSetAt(index);
			emit disPlayProp(set);
			emit higtLightSet(set);
		}
	}

	void MeshWidget::contextMenuEvent(QContextMenuEvent *event)
	{
		_currentItem = currentItem();
		if (_currentItem == nullptr) return;
		QMenu pop_menu;
		QAction* action = nullptr;
		if ((_currentItem->type() == MeshChild) || (_currentItem->type() == MeshSetChild))
		{
//			action = pop_menu.addAction(QIcon(), tr("Delete"));
			action = pop_menu.addAction(QIcon(), tr("Rename"));
			connect(action, SIGNAL(triggered()), this, SLOT(rename()));
			if (_currentItem->type() == MeshChild)
			{
				action = pop_menu.addAction(QIcon(), tr("Remove"));
				connect(action, SIGNAL(triggered()), this, SLOT(removeMeshData()));
				
			}
			else if (_currentItem->type() == MeshSetChild)
			{
				action = pop_menu.addAction(QIcon(), tr("Remove"));
				connect(action, SIGNAL(triggered()), this, SLOT(removeSetData()));
			}

				
		}
		else if (_currentItem->type() == MeshRoot)
		{
			
			action = pop_menu.addAction(QIcon(), tr("Import"));
			connect(action, SIGNAL(triggered()), _mainWindow, SLOT(on_importMesh()));

			action = pop_menu.addAction(QIcon(), tr("Hide All"));
			connect(action, SIGNAL(triggered()), this, SLOT(hideAll()));

			action = pop_menu.addAction(QIcon(), tr("Show All"));
			connect(action, SIGNAL(triggered()), this, SLOT(showAll()));
		
		}
		else if (_currentItem->type() == MeshSetRoot)
		{
			action = pop_menu.addAction(QIcon(), tr("Merge/Cut"));
			connect(action, SIGNAL(triggered()), this, SLOT(mergeSet()));
		}

		pop_menu.exec(QCursor::pos());
		
	}
	void MeshWidget::removeMeshData()
	{
		///<MG temp 
		if (_currentItem->type() != TreeItemType::MeshChild) return;
//		{
		const int index = _meshRoot->indexOfChild(_currentItem);
		if (index < 0) return;
		_data->removeKernalAt(index);
		updateMeshTree();
		updateMeshSetTree();
		emit removeMeshData(index);
		emit disPlayProp(nullptr);
		emit updateActionStates();
		emit clearHighLight();
	}
	void MeshWidget::removeSetData()
	{
		const int index = _setRoot->indexOfChild(_currentItem);
		if (index < 0) return;
		_data->removeMeshSetAt(index);
		updateMeshSetTree();
		emit disPlayProp(nullptr);
		emit clearHighLight();
	}

	void MeshWidget::startMesher(QString mesher)
	{
		SolverControl::MesherControlerBase* w = new SolverControl::MesherControlerBase(_mainWindow, mesher);
		w->start();

	}

	void MeshWidget::registerEnabledModule()
	{
		ConfigOption::MeshConfig* meshconfig = ConfigOption::ConfigOption::getInstance()->getMeshConfig();
		bool isset = meshconfig->isComponentEnabled();
		_setRoot->setHidden(!isset);
	}

	void MeshWidget::hideAll()
	{
		const int nc = _meshRoot->childCount();
		for (int i = 0; i < nc; ++i)
		{
			QTreeWidgetItem* item = _meshRoot->child(i);
			item->setCheckState(0, Qt::Unchecked);
		}

		int nk =  _data->getKernalCount();
		for (int index = 0; index < nk; ++index)
		{
			MeshData::MeshKernal* k = _data->getKernalAt(index);
			if (k == nullptr) continue;;
			emit updateDisplay(index, false);
//			emit higtLightKernal(k);
//			emit disPlayProp(k);
		}
		emit disPlayProp(nullptr);
		emit clearHighLight();
	}

	void MeshWidget::showAll()
	{
		const int nc = _meshRoot->childCount();
		for (int i = 0; i < nc; ++i)
		{
			QTreeWidgetItem* item = _meshRoot->child(i);
			item->setCheckState(0, Qt::Unchecked);
		}

		int nk = _data->getKernalCount();
		for (int index = 0; index < nk; ++index)
		{
			MeshData::MeshKernal* k = _data->getKernalAt(index);
			if (k == nullptr) continue;;
			emit updateDisplay(index, true);
// 			emit higtLightKernal(k);
// 			emit disPlayProp(k);
		}
		emit disPlayProp(nullptr);
		emit clearHighLight();
	}

	void MeshWidget::mergeSet()
	{
		MeshSetMergeDialog dlg(_mainWindow, this);
		dlg.exec();
	}

	void MeshWidget::rename()
	{
		MeshRenameDialog dlg(_mainWindow, _currentItem);
		dlg.exec();
	}

}