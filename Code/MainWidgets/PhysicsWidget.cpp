#include "PhysicsWidget.h"
#include "mainWindow/mainWindow.h"
#include "DataProperty/modelTreeItemType.h"
#include "DialogCreatePhysicsModel.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include <QMenu>
#include <QAction>
#include <assert.h>
#include <QDebug>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include "ProjectTree/ProjectTreeWithBasicNode.h"
#include "QApplication"
#include "QFileDialog"
#include "settings/busAPI.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GlobalConfig.h"
#include "ProjectTreeExtend/ProjectTreeExtend.h"
#include "DialogCreateMaterial.h"
#include "Material/MaterialSingletion.h"
#include "Material/Material.h"
#include "python/PyAgent.h"
#include <QTextCodec>
#include "PropertyTable.h"
#include "IO/ProjectTemplete.h"
#include "ProjectTreeFactory.h"

namespace MainWidget
{

	PhysicsWidget::PhysicsWidget(GUI::MainWindow* mainwindow, QWidget* parent /* = 0  */) : _mainWindow(mainwindow)
	{
		_materialRoot = new QTreeWidgetItem(this, TreeItemType::MaterialRoot);
		_materialRoot->setText(0, tr("Materials"));
		_materialRoot->setIcon(0, QIcon(":/QUI/icon/material.png"));
		_physicsModelRoot = new QTreeWidgetItem(this, TreeItemType::PhyaicsModelRoot);
		_physicsModelRoot->setText(0, tr("Case"));
		_physicsModelRoot->setIcon(0, QIcon(":/QUI/icon/physics.png"));
		this->addTopLevelItem(_materialRoot);
		this->addTopLevelItem(_physicsModelRoot);
		init();
		
		connect(this, SIGNAL(endaleGraphWinKey(bool)), _mainWindow, SIGNAL(enableGraphWindowKeyBoard(bool)));
	}
	PhysicsWidget::~PhysicsWidget()
	{
		this->clearData();
		this->clear();
	}
	void PhysicsWidget::clearData()
	{
		QList<ProjectTree::ProjectTreeBase*> trees = _idTreeHash.values();
		for (int i = 0; i < trees.size(); ++i)
		{
			delete  trees.at(i);
		}
		_idTreeHash.clear();
	}
	void PhysicsWidget::init()
	{
		this->setHeaderHidden(true);

		///注册了TreeWidget 左键单击
		connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(singleClicked(QTreeWidgetItem*, int)));
		///注册了TreeWidget 左键双击
		connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(doubleClicked(QTreeWidgetItem*, int)));
		///注册鼠标事件传递通知给mainWindow
		connect(this, SIGNAL(mouseEvent(int, QTreeWidgetItem*, int)), _mainWindow, SIGNAL(treeMouseEvent(int, QTreeWidgetItem*, int)));
		///注册由mainWindow传递通知过来更新树
		connect(_mainWindow, SIGNAL(updatePhysicsTreeSignal()), this, SLOT(updateTree()));
		/*鼠标事件 */
		connect(_mainWindow, SIGNAL(treeMouseEvent(int, QTreeWidgetItem*, int)), this, SLOT(on_TreeMouseEvent(int, QTreeWidgetItem*, int)));
		/*创建物理模型 */
		connect(_mainWindow, SIGNAL(createPhysiceModelSig()), this, SLOT(createPhysicsModel()));
		/*显示属性窗口 */
		connect(this, SIGNAL(disPlayProp(DataProperty::DataBase*)), _mainWindow, SIGNAL(updateProperty(DataProperty::DataBase*)));
		//	求解
		connect(this, SIGNAL(solveProject(int)), _mainWindow, SLOT(solveProject(int)));
		 
		connect(this, SIGNAL(updateActionStates()), _mainWindow, SIGNAL(updateActionStatesSig()));

		connect(_mainWindow, SIGNAL(updateMaterialTreeSig()), this, SLOT(updateMaterialTree()));

		connect(this, SIGNAL(updatePhysicsTreePySig()), this, SLOT(updatePhysicsTree()));
//		connect(this, SIGNAL(deleteCaseSig(int)), this, SLOT(deleteCase(int)));
		connect(this, SIGNAL(importProjectSig(int)), this, SLOT(importProject(int)));
//		connect(this, SIGNAL(caseRenameSig(int, QString)), this, SLOT(caseRename(int, QString)));
		connect(this, SIGNAL(showMessage(ModuleBase::Message)), _mainWindow, SIGNAL(printMessageToMessageWindow(ModuleBase::Message)));
		///<MG 节点变换
// 		connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(slot_selected_item_changed()));
// 
// 		connect(_mainWindow, SIGNAL(sig_action_open()), this, SLOT(slot_action_open()));

	}

	QTreeWidgetItem* PhysicsWidget::getProjectRoot(QTreeWidgetItem* item) const
	{
		if (nullptr == item && nullptr == _curretnItem)
			return nullptr;
		if (item == nullptr)
			item = _curretnItem;
		if (item->parent() == nullptr) return item;
		if (item->type() == TreeItemType::MaterialRoot || item->parent()->type() == TreeItemType::MaterialRoot) return item;

		while (item->type() != TreeItemType::ProjectRoot)
		{
			item = item->parent();
			if (item == nullptr) break;
		}
		if (item != nullptr) return item;
		return nullptr;

	}
	QTreeWidgetItem* PhysicsWidget::getProjectRoot(const int pid) const
	{
		int n = _physicsModelRoot->childCount();
		for (int i = 0; i < n; ++i)
		{
			QTreeWidgetItem* item = _physicsModelRoot->child(i);
			int id = item->data(0, Qt::UserRole).toInt();
			if (id == pid)
				return item;
		}
		return nullptr;
	}
	///contextMenuEvent   重写了底层函数   相当于右键点击
	void PhysicsWidget::contextMenuEvent(QContextMenuEvent *evnt)
	{
		_curretnItem = currentItem();
		if (!_curretnItem) return;
		QTreeWidgetItem* item = getProjectRoot(_curretnItem);
		if (nullptr == item)
			return;
		assert(item);
		const int proID = item->data(0, Qt::UserRole).toInt();
//		on_TreeMouseEvent(1, _curretnItem, proID);
		emit(mouseEvent(1, _curretnItem, proID));
	}
	void PhysicsWidget::singleClicked(QTreeWidgetItem* item, int col)
	{
		_curretnItem = item;
		QTreeWidgetItem* rootitem = getProjectRoot(_curretnItem);
		assert(rootitem);
		const int proID = rootitem->data(0, Qt::UserRole).toInt();
		bool e = false;
		switch (item->type())
		{
		case TreeItemType::ProjectRoot:
		{
			emit disPlayProp(ModelData::ModelDataSingleton::getinstance()->getModelByID(proID));
			e = true;
			break;
		}
		case TreeItemType::MaterialChild:
		{
			int materialID = item->data(0, Qt::UserRole).toInt();
			Material::Material * material = Material::MaterialSingleton::getInstance()->getMaterialByID(materialID);
			emit disPlayProp(material);
			e = true;
			break;
		}
		case TreeItemType::MaterialRoot:
		{
			emit disPlayProp(nullptr);
			e = true;
			break;
		}			
		case TreeItemType::PhyaicsModelRoot:
		{
			emit disPlayProp(nullptr);
			e = true;
			break;
		}			
// 		default:
// 				emit disPlayProp(nullptr);
		}		
		if (!e)    emit mouseEvent(0, _curretnItem, proID);
	}
	void PhysicsWidget::doubleClicked(QTreeWidgetItem* item, int col)
	{
		_curretnItem = item;
		QTreeWidgetItem* rootitem = getProjectRoot(_curretnItem);
		assert(rootitem);
		const int proID = rootitem->data(0, Qt::UserRole).toInt();
		emit mouseEvent(2, item, proID);
	}

	void PhysicsWidget::on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int id)
	{
		if (id == 0 || item->type() == TreeItemType::ProjectRoot || item->type() == TreeItemType::MaterialChild)
		{
			switch (evevntType)
			{
			case 0://左键单击
				on_singleClicked(item);
				break;
			case 1://右键单击
				createContextMenu(item);
				break;
			case 2://左键双击
				on_doubleClicked(item);
				break;
			default:
				assert(0);
				break;
			}
		}
		if (_idTreeHash.contains(id))
		{
			ProjectTree::ProjectTreeBase* tree = _idTreeHash[id];
			tree->on_MouseEvent(evevntType, item);
		}
	}
	void PhysicsWidget::on_singleClicked(QTreeWidgetItem* item)
	{

	}
	void PhysicsWidget::on_doubleClicked(QTreeWidgetItem* item)
	{

	}
	void PhysicsWidget::createContextMenu(QTreeWidgetItem* item)
	{
		singleClicked(item, -1);
		QMenu pop_menu;
		QAction *action = nullptr;

		if (item->type() == TreeItemType::MaterialRoot)
		{
			action = pop_menu.addAction(tr("Load From Material Lib"));
			connect(action, SIGNAL(triggered()), this, SLOT(slot_load_from_material_lib()));
			action = pop_menu.addAction(tr("Create Material"));
			connect(action, SIGNAL(triggered()), this, SLOT(slot_create_material()));
			pop_menu.addSeparator();
			action = pop_menu.addAction(tr("Remove From Material Lib"));
			connect(action, SIGNAL(triggered()), this, SLOT(slot_remove_from_material_lib()));
		}
		else if (item->type() == TreeItemType::MaterialChild)
		{
			action = pop_menu.addAction(tr("Delete Material"));
			connect(action, SIGNAL(triggered()), this, SLOT(slot_delete_material()));
			action = pop_menu.addAction(tr("Add to Material Lib"));
			connect(action, SIGNAL(triggered()), this, SLOT(slot_add_to_material_lib()));
		}
		else if (item->type() == TreeItemType::PhyaicsModelRoot)
		{
		
			action = pop_menu.addAction(tr("Create Case"));
			connect(action, SIGNAL(triggered()), this, SLOT(createPhysicsModel()));
			
		}
		else if (item->type() == TreeItemType::ProjectRoot)
		{
			
			action = pop_menu.addAction(tr("Delete Case"));
			connect(action, SIGNAL(triggered()), this, SLOT(deleteProject()));
			action = pop_menu.addAction(tr("Solve Case"));
			connect(action, SIGNAL(triggered()), this, SLOT(solveProject()));
			action = pop_menu.addAction(tr("Rename Case"));
			connect(action, SIGNAL(triggered()), this, SLOT(renameProject()));
			action = pop_menu.addAction(tr("Import Templete"));
			connect(action, SIGNAL(triggered()), this, SLOT(importProjectPy()));
			action = pop_menu.addAction(tr("Open Dir"));
			connect(action, SIGNAL(triggered()), this, SLOT(openProjectPath()));
			action = pop_menu.addAction(tr("Import INP File"));
			
			connect(action, SIGNAL(triggered()), _mainWindow, SLOT(on_importMesh()));
			const int modelId = _curretnItem->data(0, Qt::UserRole).toInt();
			action->setObjectName(QString("Only INP_%1").arg(modelId));

			action = pop_menu.addAction(tr("Export INP File"));
			connect(action, SIGNAL(triggered()), _mainWindow, SLOT(on_exportMesh()));
			action->setObjectName(QString("Only INP_%1").arg(modelId));
			int id = item->data(0, Qt::UserRole).toInt();
			auto tree = _idTreeHash.value(id);
			if (tree != nullptr)
			{
				pop_menu.addSeparator();
				tree->setCurrentItem(item);
				tree->contextMenu(&pop_menu);
			}
		}
		pop_menu.exec(QCursor::pos());
	}
	void PhysicsWidget::createPhysicsModel()
	{
		CreatePhysicsModel dlg(this,_mainWindow);
		dlg.exec();
	}
	void PhysicsWidget::updateMaterialTree()
	{
		_materialRoot->takeChildren();
		_materialRoot->setText(0, tr("Materials"));
		_materialRoot->setExpanded(true);

		Material::MaterialSingleton * s = Material::MaterialSingleton::getInstance();
		int n = s->getMaterialCount();

		for (int iCnt = 0; iCnt < n; ++iCnt)
		{
			Material::Material * material = s->getMaterialAt(iCnt);
			QTreeWidgetItem * materialItem = new QTreeWidgetItem(_materialRoot, TreeItemType::MaterialChild);
			materialItem->setText(0, material->getName());
			materialItem->setIcon(0, QIcon(":/QUI/icon/material.png"));
			materialItem->setData(0, Qt::UserRole, material->getID());
		}
		_materialRoot->setExpanded(true);
	}
	void PhysicsWidget::updatePhysicsTree()
	{
		_physicsModelRoot->takeChildren();
		_physicsModelRoot->setText(0, tr("Case"));
		_physicsModelRoot->setExpanded(true);

		QList<ProjectTree::ProjectTreeBase*> ptrees = _idTreeHash.values();
		for (int i = 0; i < ptrees.size(); ++i) delete ptrees.at(i);
		_idTreeHash.clear();

		ModelData::ModelDataSingleton *s = ModelData::ModelDataSingleton::getinstance();
		int n = s->getModelCount();
		for (int i = 0; i < n; ++i)
		{
			ModelData::ModelDataBase* model = s->getModelAt(i);
			const ProjectTreeType treeType = model->getTreeType();
			const int modelid = model->getID();
//			ConfigOption::ProjectTreeConfig* treeconfig = ConfigOption::ConfigOption::getInstance()->getTreeConfig();
			

			ProjectTree::ProjectTreeBase* tree = ProjectTreeFactory::creatTree(treeType, _mainWindow);
			if (tree == nullptr) continue;

			tree->setData(model);
			tree->createTree(_physicsModelRoot, _mainWindow);
			tree->updateTree();
			_idTreeHash[modelid] = tree;
		}
		_physicsModelRoot->setExpanded(true);
	}
	
	void PhysicsWidget::removeCaseComponentSlot(int cpID)
	{
		QHash<int, ProjectTree::ProjectTreeBase*>::iterator it;
		for (it = _idTreeHash.begin(); it != _idTreeHash.end(); ++it)
		{
			auto tree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*> (it.value());
			if(!tree->getComponentIDList().contains(cpID))	continue;
			tree->removeCaseComponentByID(cpID);
		}
	}

	void PhysicsWidget::renameCaseComponentSlot(int cpID)
	{
		QHash<int, ProjectTree::ProjectTreeBase*>::iterator it;
		for (it = _idTreeHash.begin(); it != _idTreeHash.end(); ++it)
		{
			auto tree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*> (it.value());
			if (!tree->getComponentIDList().contains(cpID))	continue;
			tree->renameCaseComponentByID(cpID);
		}
	}

	void PhysicsWidget::updateTree()
	{
		updateMaterialTree();
		updatePhysicsTree();
	}
	
	void PhysicsWidget::renameProject()
	{
		if (_curretnItem->type() != TreeItemType::ProjectRoot) return;
		emit endaleGraphWinKey(false);
//		this->grabKeyboard();
		this->editItem(_curretnItem);
		
	}
	
	void PhysicsWidget::deleteProject()
	{
		if (_curretnItem->type() != TreeItemType::ProjectRoot) return;
		QMessageBox::StandardButton b = QMessageBox::warning(this, tr("Warning"), tr("Confirm to delete?"), QMessageBox::Yes | QMessageBox::No);
		if (b == QMessageBox::No) return;

		const int id = _curretnItem->data(0, Qt::UserRole).toInt();

		QString code = QString("ControlPanel.deleteCase(%1)").arg(id);
		Py::PythonAagent::getInstance()->submit(code);
	}

	void PhysicsWidget::deleteCase(int id)
	{
		ModelData::ModelDataSingleton* modelSing = ModelData::ModelDataSingleton::getinstance();
		modelSing->removeModelByID(id);
		if (_curretnItem != nullptr)
		{
			_curretnItem->takeChildren();
			_physicsModelRoot->removeChild(_curretnItem);
			ProjectTree::ProjectTreeBase* tree = _idTreeHash.value(id);
			_idTreeHash.remove(id);
			if (tree != nullptr) delete tree;
		}
		else
		{
			updatePhysicsTree();
		}

		emit updateActionStates();
	}
	
	void PhysicsWidget::solveProject()
	{
		if (_curretnItem->type() != TreeItemType::ProjectRoot) return;
		const int id = _curretnItem->data(0, Qt::UserRole).toInt();
		if (id > 0) emit solveProject(id);
	}
	void PhysicsWidget::importProjectPy()
	{
		//IO::ProjectTemplete *template_file = new IO::ProjectTemplete();
		//QStringList file_text = template_file->getTempletefromFile();
		
		const int id = _curretnItem->data(0, Qt::UserRole).toInt();//获取ID
		QString code = QString("ControlPanel.importProject(%1)").arg(id);
		Py::PythonAagent::getInstance()->submit(code);

	}
	void PhysicsWidget::importProject(int id){
		IO::ProjectTemplete *template_file = new IO::ProjectTemplete();
		QStringList file_text = template_file->getTempletefromFile();
		//const int id = _curretnItem->data(0, Qt::UserRole).toInt();//liu
		template_file->importTemplete(id, file_text);
		ModuleBase::Message msg(ModuleBase::Normal_Message, tr("Template import successfully!"));
		emit showMessage(msg);
		emit updatePropertyTableTree();
		return ;
	}
	void PhysicsWidget::slot_load_from_material_lib()
	{
		Material::MaterialSingleton::getInstance()->loadFromMaterialLib(_mainWindow);
		//updateMaterialTree();
	}

	void PhysicsWidget::slot_remove_from_material_lib()
	{
		Material::MaterialSingleton::getInstance()->removeFromMAterialLib(_mainWindow);
	}

	void PhysicsWidget::slot_create_material()
	{
		CreateMaterialDialog dlg(_mainWindow);
		dlg.exec();
	/*	if (QDialog::Accepted == dlg.exec())
		{
			updateMaterialTree();
		}*/
	}


	void PhysicsWidget::slot_delete_material()
	{
		if (_curretnItem->type() != TreeItemType::MaterialChild) return;
		const int id = _curretnItem->data(0, Qt::UserRole).toInt();
		_curretnItem->takeChildren();
		_physicsModelRoot->removeChild(_curretnItem);

		Material::MaterialSingleton * s = Material::MaterialSingleton::getInstance();
		QString maname = s->getMaterialByID(id)->getName();
		//qDebug() << maname;
		//s->removeMaterialByID(id);
		QString code = QString("ControlPanel.DeleteMaterial(%1,'%2')").arg(id).arg(maname);
		Py::PythonAagent::getInstance()->submit(code);

	/*	emit updateActionStates();
		updateMaterialTree();
		emit disPlayProp(nullptr);*/
	}

	void PhysicsWidget::slot_add_to_material_lib()
	{
		if (_curretnItem == nullptr) return;
		if (_curretnItem->type() != TreeItemType::MaterialChild) return;
		const int id = _curretnItem->data(0, Qt::UserRole).toInt();

		Material::MaterialSingleton * s = Material::MaterialSingleton::getInstance();
		QString maname = s->getMaterialByID(id)->getName();
		//qDebug() << maname;
		//s->removeMaterialByID(id);
		QString code = QString("ControlPanel.AddMaterialToLib(%1,'%2')").arg(id).arg(maname);
		Py::PythonAagent::getInstance()->submit(code);



		//s->appendToMaterialLib(id);

	}

	
	void PhysicsWidget::caseRename(int pid,QString newname)
	{
		ModelData::ModelDataBase* model = ModelData::ModelDataSingleton::getinstance()->getModelByID(pid);
		if (nullptr == model)  return;
		model->setName(newname);
	
	}
	void PhysicsWidget::closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint)
	{
		if (_curretnItem->type() != TreeItemType::ProjectRoot) return;
		const int pid = _curretnItem->data(0, Qt::UserRole).toInt();
		if (pid < 1 || pid > ModelData::ModelDataBase::getMaxID()) return;
		ModelData::ModelDataBase* model = ModelData::ModelDataSingleton::getinstance()->getModelByID(pid);
		const QString oldname = model->getName();
		const QString newname = _curretnItem->text(0).simplified();
		bool exist = false;
		bool legal = true;
		if (newname.isEmpty() || (oldname == newname))
		{
			_curretnItem->setText(0, oldname);
		}
		else
		{
			ModelData::ModelDataSingleton* models = ModelData::ModelDataSingleton::getinstance();
			const int n = models->getModelCount();
			for (int i = 0; i < n; ++i)
			{
				ModelData::ModelDataBase* dataBase = models->getModelAt(i);
				QString name = dataBase->getName();
				if (name == newname)
				{
					exist = true;
					break;
				}
				
			}
			QRegExp rx("[//\*:<>?\"\|]");
			int pos = rx.indexIn(newname);
			if (pos > -1 || newname.contains("\\"))
			{
				QString info = tr("Name can not contains fellowing char: \\/:*?\"<>|!");
				QMessageBox::warning(this, tr("Warning"), info);
				legal = false;
			}
			else if (newname.toLocal8Bit().length()>20)
			{
				QString info = tr("Name contains too many characters !");
				QMessageBox::warning(this, tr("Warning"), info);
				legal = false;
			}

			if ((!exist) && legal)
			{
				_curretnItem->setText(0, newname);

				QString code = QString("ControlPanel.caseRename(%1,\"%2\")").arg(pid).arg(newname);
				qDebug() << code;
				Py::PythonAagent::getInstance()->submit(code);
			
			}
			else
			{
				_curretnItem->setText(0, oldname);
			}
			this->releaseKeyboard();
			emit endaleGraphWinKey(true);

			
		}
		this->setItemSelected(_curretnItem, true);
		emit disPlayProp(model);
		QTreeWidget::closeEditor(editor, hint);
		if (exist)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Case \"%1\" has already exist!").arg(newname));
		}
	}

	void PhysicsWidget::reTranslate()
	{
		_materialRoot->setText(0, tr("Materials"));
		_physicsModelRoot->setText(0, tr("Case"));

		QList<int> ids = _idTreeHash.keys();
		for (int i = 0; i < ids.size(); ++i)
		{
			ProjectTree::ProjectTreeBase* t = _idTreeHash.value(ids.at(i));
			if (t == nullptr) continue;
			t->reTranslate();
		}
	}

	ProjectTree::ProjectTreeBase* PhysicsWidget::getProjectByID(int id)
	{
		return _idTreeHash.value(id);
	}

	void PhysicsWidget::openProjectPath()
	{
		if (_curretnItem->type() != TreeItemType::ProjectRoot) return;
		const int id = _curretnItem->data(0, Qt::UserRole).toInt();

		ModelData::ModelDataSingleton* s = ModelData::ModelDataSingleton::getinstance();
		ModelData::ModelDataBase* m = s->getModelByID(id);

		QString path = m->getPath();
		QDir dir(path);
		if (!dir.exists())
			dir.mkpath(path);

		QDesktopServices::openUrl(QUrl::fromLocalFile(path));
	}

	void PhysicsWidget::updateMaterialStatus()
	{
		auto glo = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();
		bool ok = glo->isMaterialEnabled();
		_materialRoot->setHidden(!ok);
	}

}