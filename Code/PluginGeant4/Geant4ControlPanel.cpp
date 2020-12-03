#include "Geant4ControlPanel.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "DataProperty/modelTreeItemType.h"
#include "Geant4ShipTreeItemType.h"
#include "GDMLReader.h"
#include "DataManager.h"
#include "DefineManager.h"
#include "MaterialManager.h"
#include "SolidsManager.h"
#include "PreDefineBase.h"
#include "GDMLElement.h"
#include "GDMLIsotope.h"
#include "GDMLMaterial.h"
#include "SolidShapeBase.h"
#include "dialogDefine.h"
#include "dialogElements.h"
#include "dialogMaterial.h"
#include "dialogIsotope.h"
#include "DialogBox.h"
#include "StructureManager.h"
#include "Volume.h"
#include "Physvol.h"
#include "CreateSolidDialogFactory.h"
#include "PreDefineBase.h"
#include "SolidShapePlacer.h"
#include "PreWindowManager.h"
#include <QFileDialog>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include "settings/busAPI.h"
#include "Common.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "Geant4DataModel.h"
#include "dialogAddAnalysis.h"
#include "dialogAddVolume.h"
#include "dialogAddVolumeInterface.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverConfig.h"
#include "SolverControler.h"

namespace Geant4
{
	Geant4ControlPanel::Geant4ControlPanel(GUI::MainWindow* m) :_mainWindow(m)
	{

		//init();
		initProjectTree();
		connect(this, SIGNAL(updateProperty(DataProperty::DataBase*)), m, SIGNAL(updateProperty(DataProperty::DataBase*)));
		connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(singleClicked(QTreeWidgetItem*, int)));
	}

	Geant4ControlPanel::~Geant4ControlPanel()
	{
		this->clear();
	}
	void Geant4ControlPanel::init()
	{
		this->setHeaderHidden(true);

		QStringList rootTextList;	
		rootTextList << tr("Geant4") ;

		_root = new QTreeWidgetItem(this, rootTextList, Geant4TreeItemType::Geant4Root);
		_root->setIcon(0, QIcon(":/QUI/icon/physics.png"));
		_defineRoot = new QTreeWidgetItem(_root, QStringList(tr("Define")), Geant4TreeItemType::Geant4DefineRoot);
		_defineRoot->setIcon(0, QIcon("://QUI/icon/acce.png"));
		_materialRoot = new QTreeWidgetItem(_root, QStringList("Materials"), Geant4TreeItemType::Geant4MaterialRoot);
		_materialRoot->setIcon(0, QIcon("://QUI/icon/material.png"));
		_solidRoot = new QTreeWidgetItem(_root, QStringList("Solid"), Geant4TreeItemType::Geant4SolidRoot);
		_solidRoot->setIcon(0, QIcon("://QUI/geometry/chamfer.png"));
		_structureRoot = new QTreeWidgetItem(_root, QStringList("Structure"), Geant4TreeItemType::Geant4StructureRoot);
		_structureRoot->setIcon(0, QIcon("://QUI/icon/nodeset.png"));
		_trackRoot = new QTreeWidgetItem(_root, QStringList("Track"), Geant4TreeItemType::Geant4TrackRoot);
		_trackRoot->setIcon(0, QIcon("://QUI/icon/fulid.png"));
		_root->setExpanded(true);
	}

	void Geant4ControlPanel::initProjectTree()
	{
		this->setHeaderHidden(true);

		QStringList rootTextList;
		rootTextList << tr("Geant4");

		_root = new QTreeWidgetItem(this, rootTextList, Geant4TreeItemType::Geant4Root);
		_root->setIcon(0, QIcon(":/QUI/icon/physics.png"));
		
		_sourceNode = new QTreeWidgetItem(_root, QStringList(tr("Source")), Geant4TreeItemType::Geant4Source);
		_physicalNode = new QTreeWidgetItem(_root, QStringList(tr("Physical")), Geant4TreeItemType::Geant4Physical);
		_geometryNode = new QTreeWidgetItem(_root, QStringList(tr("Geometry")), Geant4TreeItemType::Geant4Geometry);
		_geometryEnergyThresholdNode = new QTreeWidgetItem(_root, QStringList(tr("Geometry Energy Threshold")), Geant4TreeItemType::Geant4PhysicalEnergyLimit);
		_calculateNode = new QTreeWidgetItem(_root, QStringList(tr("Calculate")), Geant4TreeItemType::Geant4Calculate);

		_sourceType = new QTreeWidgetItem(_sourceNode, QStringList(tr("Type")), Geant4TreeItemType::SourceType);
		_sourceAngle = new QTreeWidgetItem(_sourceNode, QStringList(tr("Angle")), Geant4TreeItemType::SourceAngle);
		_sourcePara = new QTreeWidgetItem(_sourceNode, QStringList(tr("Parameter")), Geant4TreeItemType::SourcePara);

		_defineRoot = new QTreeWidgetItem(_geometryNode, QStringList(tr("Define")), Geant4TreeItemType::Geant4DefineRoot);
		_defineRoot->setIcon(0, QIcon("://QUI/icon/acce.png"));
		_materialRoot = new QTreeWidgetItem(_geometryNode, QStringList("Materials"), Geant4TreeItemType::Geant4MaterialRoot);
		_materialRoot->setIcon(0, QIcon("://QUI/icon/material.png"));
		_solidRoot = new QTreeWidgetItem(_geometryNode, QStringList("Solid"), Geant4TreeItemType::Geant4SolidRoot);
		_solidRoot->setIcon(0, QIcon("://QUI/geometry/chamfer.png"));
		_structureRoot = new QTreeWidgetItem(_geometryNode, QStringList("Structure"), Geant4TreeItemType::Geant4StructureRoot);
		_structureRoot->setIcon(0, QIcon("://QUI/icon/nodeset.png"));
		_trackRoot = new QTreeWidgetItem(_geometryNode, QStringList("Track"), Geant4TreeItemType::Geant4TrackRoot);
		_trackRoot->setIcon(0, QIcon("://QUI/icon/fulid.png"));
		
		_root->setExpanded(true);
	}
	
	void Geant4ControlPanel::updateDefineTree()
	{
 		_defineRoot->takeChildren();
 		DefineManager* dm = DataManager::getInstance()->getDefineManager();
		const int n = dm->getDefineCount();
		for (int i=0;i<n; ++i)
		{
			auto def = dm->getPreDefineAt(i);
			QString name = def->getName();
			QTreeWidgetItem* item = new QTreeWidgetItem(_defineRoot, QStringList(name),Geant4TreeItemType::ParameterDefine);
			item->setData(0, Qt::UserRole, i);
			item->setIcon(0, QIcon(GetIconStringByDefineType(def->getType())));
		}
	}

	void Geant4ControlPanel::updateMaterialTree()
	{
		_materialRoot->takeChildren();
		MaterialManager* m_material = DataManager::getInstance()->getMaterialManager();
		int elesize = m_material->getElementCount();
		for (int i = 0; i < elesize; i++)
		{
			QString elename = m_material->getElementByIndex(i)->getName();
			QTreeWidgetItem* eleitem = new QTreeWidgetItem(_materialRoot, QStringList(elename), Geant4TreeItemType::ParameterElement);
			eleitem->setData(0, Qt::UserRole, i);
			eleitem->setIcon(0, QIcon("://QUI/icon/vector.png"));
		}
		int isosize = m_material->getIsotopeCount();
		for (int i = 0; i < isosize; i++)
		{
			QString mname = m_material->getIsotopeByIndex(i)->getName();
			QTreeWidgetItem* itemema = new QTreeWidgetItem(_materialRoot, QStringList(mname), Geant4TreeItemType::ParameterIsotope);
			itemema->setData(0, Qt::UserRole, i);
			itemema->setIcon(0, QIcon("://QUI/icon/streamline.png"));
		}
		int msize = m_material->getMaterialCount();
		for (int i = 0; i < msize; i++)
		{
			QString mname = m_material->getMaterialByIndex(i)->getName();
			QTreeWidgetItem* itemema = new QTreeWidgetItem(_materialRoot, QStringList(mname), Geant4TreeItemType::ParameterMaterial);
			itemema->setData(0, Qt::UserRole, i);
			itemema->setIcon(0, QIcon("://QUI/icon/wall.png"));
		}
	}

	void Geant4ControlPanel::updateSolidsTree()
	{
		_solidRoot->takeChildren();
		SolidsManager* solids = DataManager::getInstance()->getSolidsManager();

		int sSize = solids->getSolidCount();
		for (int i = 0; i < sSize; i++)
		{
			SolidShapeBase* geo = solids->getSolidAt(i);
			QString name = geo->getName();
			SolidType type = geo->getType();
			QTreeWidgetItem* item = new QTreeWidgetItem(_solidRoot, QStringList(name), Geant4TreeItemType::ParameterGeometry);
			item->setData(0, Qt::UserRole, i);
			item->setIcon(0, QIcon(GetIconStringBySolidType(type)));
		}
	}

	void Geant4ControlPanel::updateStructerTree()
	{
		_structureRoot->takeChildren();
		auto manager = DataManager::getInstance()->getStructManager();
		const int n = manager->getVolumeCount();
		for (int i =0;i<n; ++i)
		{
			auto vol = manager->getVolumeAt(i);
			QString vn = vol->getName();
			auto* vitem = new QTreeWidgetItem(_structureRoot, QStringList(vn), Geant4TreeItemType::GeoTypeVolume);
			vitem->setData(0, Qt::UserRole, i);
			vitem->setIcon(0, QIcon("://QUI/icon/bc.png"));
			const int np = vol->getPhysvolCount();
			for (int j = 0; j < np; ++j)
			{
				auto phy = vol->getPhysvolAt(j);
				QString ref = phy->getVolRef();
				auto* item = new QTreeWidgetItem(vitem, QStringList(ref), Geant4TreeItemType::GeoTypePhysvol);
				item->setData(0, Qt::UserRole, i);
				item->setData(0, Qt::UserRole + 1, j);
				item->setIcon(0, QIcon("://QUI/icon/fix.png"));
			}
		}
	}

	void Geant4ControlPanel::updateTrackTree()
	{
		_trackRoot->takeChildren();
		auto meshData = MeshData::MeshData::getInstance();
		const int n = meshData->getKernalCount();
		for (int i=0;i<n; ++i)
		{
			auto k = meshData->getKernalAt(i);
			QString name = k->getName();
			auto* item = new QTreeWidgetItem(_trackRoot, QStringList(name), Geant4TreeItemType::Geant4Track);
			item->setData(0, Qt::UserRole, k->getID());
			item->setIcon(0, QIcon("://QUI/icon/speed.png"));
		}

	}

	void Geant4ControlPanel::updateGeometryEnergyThresholdTree()
	{
		_geometryEnergyThresholdNode->takeChildren();
		auto solids = DataManager::getInstance()->getSolidsManager();
		int sSize = solids->getSolidCount();

		QMap<QString, _G4GeometryEnergyThreshold> g4GeoEnTh;
		for (int i = 0; i < sSize; i++)
		{
			SolidShapeBase* geo = solids->getSolidAt(i);
			QString name = geo->getName();
			QTreeWidgetItem* item = new QTreeWidgetItem(_geometryEnergyThresholdNode, QStringList(name), Geant4TreeItemType::Geant4PhEnThItem);
			g4GeoEnTh.insert(name, _G4GeometryEnergyThreshold());
		}
		DataManager::getInstance()->getGeant4DataModel()->setGeometriesEnergyThreshold(g4GeoEnTh);
	}

	void Geant4ControlPanel::singleClicked(QTreeWidgetItem*item, int i)
	{
		if (item == nullptr) return;
		Geant4TreeItemType  type = (Geant4TreeItemType)item->type();
		switch (type)
		{
		case ParameterDefine:
		{
			auto manager = DataManager::getInstance()->getDefineManager();
			int index = item->data(0, Qt::UserRole).toInt();
			auto d = manager->getPreDefineAt(index);
			emit updateProperty(d);
			break;
		}
		case GeoTypeVolume:
		{
			auto manager = DataManager::getInstance()->getStructManager();
			int index = item->data(0, Qt::UserRole).toInt();
			auto data = manager->getVolumeAt(index);
			emit updateProperty(data);
			break;
		}
			
		case GeoTypePhysvol:
		{
			auto manager = DataManager::getInstance()->getStructManager();
			int index = item->data(0, Qt::UserRole).toInt();
			int pindex = item->data(0, Qt::UserRole + 1).toInt();
			auto data = manager->getPhysvolAt(index, pindex);
			emit updateProperty(data);
			break;
		}
		case ParameterGeometry:
		{
			auto manager = DataManager::getInstance()->getSolidsManager();
			int index = item->data(0, Qt::UserRole).toInt();
			auto data = manager->getSolidAt(index);
			emit updateProperty(data);
			break;
		}
		case ParameterElement:
		case ParameterIsotope:
		case ParameterMaterial:
		{
			auto manager = DataManager::getInstance()->getMaterialManager();
			int index = item->data(0, Qt::UserRole).toInt();
			DataProperty::DataBase* d = nullptr;
			if (type == ParameterElement) d = manager->getElementByIndex(index);
			else if (type == ParameterIsotope) d = manager->getIsotopeByIndex(index);
			else if (type == ParameterMaterial) d = manager->getMaterialByIndex(index);
			emit updateProperty(d);
			break;
			
		}
		case SourceType:
		case SourceAngle:
		case SourcePara:
		case Geant4Physical:
		{
			emit updateGeant4Property(type);
			break;
		}
		case Geant4PhEnThItem:
		case Geant4CaAnItem:
		case Geant4CaAnHiItem:
		case Geant4CaAnNoItem:
		case Geant4CaAnRelationItem:
		{
			QString text = item->text(0);
			emit updateGeant4Property(type, item);
			break;
		}

		default: 
			emit clean();
			break;
		}
	}

	void Geant4ControlPanel::importGDML()
	{
		QString filename = QFileDialog::getOpenFileName(this, "Select a gdml", "", "GDML(*.gdml)");
		if (filename.isEmpty()) return;
		qDebug() << filename;
		GDMLReader reader(filename);
		bool ok = reader.read();
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("GDML import failed !"));
			return;
		}
		DataManager::getInstance()->getGeant4DataModel()->setGDMLFile(filename);
		updateDefineTree();
		updateMaterialTree();
		updateSolidsTree();
		updateGeometryEnergyThresholdTree();
		updateStructerTree();
		_mainWindow->getSubWindowManager()->openPreWindow();
		SolidShapePlacer p(_mainWindow);
		p.place();
	}

	void Geant4ControlPanel::saveGDML()
	{
		QString dir = Setting::BusAPI::instance()->getWorkingDir();
		QString fillter = QString("Project file(*.gdml)");
		QString title = QString("Open a project");
		QString filePath = QFileDialog::getSaveFileName(nullptr, title, dir, fillter);
		if (filePath.isEmpty()) return ;
		GDMLReader reader(filePath);
		bool ok = reader.write();
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("GDML save failed !"));
			return;
		}
	}

	void Geant4ControlPanel::contextMenuEvent(QContextMenuEvent *event)
	{
		auto item = this->currentItem();
		if (item == nullptr) return;
		QMenu pop_menu;
		QAction* action = nullptr;
		QAction* actionSave = nullptr;
		Geant4TreeItemType  type = (Geant4TreeItemType)item->type();
		switch (type)
		{
		case Geant4Root:
			action = pop_menu.addAction(tr("Solve"));
			connect(action, SIGNAL(triggered()), this, SLOT(SolveCalculate()));
			break;
		case Geant4Geometry:
			action = pop_menu.addAction(tr("Import GDML"));
			connect(action, SIGNAL(triggered()), this, SLOT(importGDML()));
			actionSave = pop_menu.addAction(tr("Export GDML"));
			connect(actionSave, SIGNAL(triggered()), this, SLOT(saveGDML()));
			break;
		case Geant4TrackRoot:
			action = pop_menu.addAction("Proton");
			connect(action, SIGNAL(triggered()), this, SLOT(addProton()));
			action = pop_menu.addAction("Electron");
			connect(action, SIGNAL(triggered()), this, SLOT(addElectron()));
			action = pop_menu.addAction("Photon");
			connect(action, SIGNAL(triggered()), this, SLOT(addPhoton()));
			break;
		case Geant4Track:
			action = pop_menu.addAction("Remove");
			connect(action, SIGNAL(triggered()), this, SLOT(removeTrack()));
			break;
		case Geant4Calculate:
			action = pop_menu.addAction(tr("Add Analysis"));
			connect(action, SIGNAL(triggered()), this, SLOT(addAnalysis()));
			break;
		case Geant4CaAnItem:
			action = pop_menu.addAction(tr("Add Histogram"));
			connect(action, SIGNAL(triggered()), this, SLOT(addHistogram()));
			action = pop_menu.addAction(tr("Add Volume"));
			connect(action, SIGNAL(triggered()), this, SLOT(addVolume()));
			action = pop_menu.addAction(tr("Add Volume Interface"));
			connect(action, SIGNAL(triggered()), this, SLOT(addVolumeInterface()));
			action = pop_menu.addAction(tr("Delete Analysis"));
			connect(action, SIGNAL(triggered()), this, SLOT(deleteAnalysis()));
			break;
		case Geant4CaAnVoItem:
			action = pop_menu.addAction(tr("Delete Volume"));
			connect(action, SIGNAL(triggered()), this, SLOT(deleteVolume()));
			break;
		case Geant4TreeItemType::ParameterPosition:
		case Geant4TreeItemType::ParameterRotation:
		case Geant4TreeItemType::ParameterElement:
		case Geant4TreeItemType::ParameterMaterial:
		case Geant4TreeItemType::ParameterIsotope:
		case Geant4TreeItemType::ParameterGeometry:
		case Geant4TreeItemType::ParameterDefine:
			action = pop_menu.addAction(tr("Edit"));
			connect(action, SIGNAL(triggered()), this, SLOT(editItem()));

		case Geant4PhEnThItem:
			action = pop_menu.addAction(tr("Delete Geometry"));
			connect(action, SIGNAL(triggered()), this, SLOT(deleteGeometry()));
			break;
		case Geant4CaAnHiItem:
			action = pop_menu.addAction(tr("Delete Geometry"));
			connect(action, SIGNAL(triggered()), this, SLOT(deleteHistogram()));
			break;
		case Geant4CaAnRelationItem:
			action = pop_menu.addAction(tr("Delete Geometry"));
			connect(action, SIGNAL(triggered()), this, SLOT(deleteVolumeInterface()));
			break;
		default:
			break;
		}
		pop_menu.exec(QCursor::pos());
	}

	void Geant4ControlPanel::editItem()
	{
		QTreeWidgetItem* item = this->currentItem();

		switch (item->type())
		{
		case Geant4TreeItemType::ParameterDefine:
		{
			int index = item->data(0, Qt::UserRole).toInt();
			PreDefineBase* data = DataManager::getInstance()->getDefineManager()->getPreDefineAt(index);
			CreateDefineDialog* dlg = new CreateDefineDialog(data);
			dlg->exec();
			break;
		}
		case Geant4TreeItemType::ParameterPosition:
		{
			int index = item->data(0, Qt::UserRole).toInt();
// 			Value3D* v = DataManager::getInstance()->getDefineManager()->getValueFromPositionList(index);
// 			CreateDefineDialog* dlg = new CreateDefineDialog(v,DefineType::PositionDefine);
// 			dlg->setWindowTitle(tr("Position Information"));
// 			dlg->exec();
			break;
		}
		case Geant4TreeItemType::ParameterRotation:
		{
			int index = item->data(0, Qt::UserRole).toInt();
// 			Value3D* v = DataManager::getInstance()->getDefineManager()->getValueFromRotationList(index);
// 			CreateDefineDialog* dlg = new CreateDefineDialog(v, DefineType::RotationDefine);
// 			dlg->setWindowTitle(tr("Rotation Information"));
// 			dlg->exec();
			break;
		}
		case Geant4TreeItemType::ParameterElement:
		{
			int index = item->data(0, Qt::UserRole).toInt();
			GDMLElement* onele = DataManager::getInstance()->getMaterialManager()->getElementByIndex(index);
			CreateElementsDialog* dlg = new CreateElementsDialog(onele);
			dlg->exec();
			break;
		}
		case Geant4TreeItemType::ParameterMaterial:
		{
			int index = item->data(0, Qt::UserRole).toInt();
			GDMLMaterial* material = DataManager::getInstance()->getMaterialManager()->getMaterialByIndex(index);
			CreateMaterialDialog* dlg = new CreateMaterialDialog(material);
			dlg->exec();
			break;
		}
		case  Geant4TreeItemType::ParameterIsotope:
		{

			int index = item->data(0, Qt::UserRole).toInt();
			GDMLIsotope* isotope = DataManager::getInstance()->getMaterialManager()->getIsotopeByIndex(index);
			CreateIsotopeDialog* dlg = new CreateIsotopeDialog(isotope);
			dlg->exec();
			break;
		}
		case Geant4TreeItemType::ParameterGeometry:
		{
			int index = item->data(0, Qt::UserRole).toInt();
			auto data = DataManager::getInstance()->getSolidsManager()->getSolidAt(index);
			if (data == nullptr)return;
			QDialog* dlg = CreateSolidDialogFactory::editSolidDialog(data);
			dlg->exec();
			break;
		}
		default:
			break;
		}
	}


	void Geant4ControlPanel::SolveCalculate()
	{
		//added by libaojun @ 2020/11/27
		auto solverOpts = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		const int nsolver = solverOpts->getSolverCount();
		if (nsolver < 1) return;
		auto s = solverOpts->getSolverAt(0);
		Geant4SolverControler* c = new Geant4SolverControler(_mainWindow, s);
		c->writeMacFile();
		c->startSolve();


	}

	void Geant4ControlPanel::addProton()
	{
		QString filename = QFileDialog::getOpenFileName(this, "Select a VTK File", "", "VTK(*.vtk)");
		if (filename.isEmpty()) return;
		DataManager::getInstance()->getPreWindowManager()->addTrack(1,filename);
		updateTrackTree();
	}

	void Geant4ControlPanel::addElectron()
	{
		QString filename = QFileDialog::getOpenFileName(this, "Select a VTK File", "", "VTK(*.vtk)");
		if (filename.isEmpty()) return;
		DataManager::getInstance()->getPreWindowManager()->addTrack(2, filename);
		updateTrackTree();
	}

	void Geant4ControlPanel::addPhoton()
	{
		QString filename = QFileDialog::getOpenFileName(this, "Select a VTK File", "", "VTK(*.vtk)");
		if (filename.isEmpty()) return;
		DataManager::getInstance()->getPreWindowManager()->addTrack(3, filename);
		updateTrackTree();
	}

	void Geant4ControlPanel::removeTrack()
	{
		QTreeWidgetItem* item = this->currentItem();
		int id = item->data(0, Qt::UserRole).toInt();
		bool ok = DataManager::getInstance()->getPreWindowManager()->removeTrack(id);
		if (ok) updateTrackTree();
		emit clean();
	}

	void Geant4ControlPanel::addAnalysis()
	{
		DialogAddAnalysis dlg(this);
		if (QDialog::Accepted == dlg.exec())
		{
			QString anaName = dlg.getAnalysisName();
			for (int i = 0; i < _calculateNode->childCount(); i++)
			{
				if (_calculateNode->child(i)->text(0) == anaName)
				{
					return;
				}
			}
			QTreeWidgetItem * item = new QTreeWidgetItem(_calculateNode, Geant4CaAnItem);
			item->setText(0, anaName);
			_G4AnalysisItem ana;
			ana.name = anaName;
			ana.type = (_G4AnalysisType)dlg.getAnalysisType();
			QTreeWidgetItem * noItem = new QTreeWidgetItem(item, Geant4CaAnNoItem);
			noItem->setText(0, tr("Normalization"));
			auto dataModel = DataManager::getInstance()->getGeant4DataModel();
			dataModel->addAnalysisItem(ana);
			item->setExpanded(true);
		}
	}

	void Geant4ControlPanel::deleteAnalysis()
	{
		auto item = currentItem();
		auto pItem = item->parent();
		if (item && pItem)
		{
			DataManager::getInstance()->getGeant4DataModel()->deleteAnalysisItem(item->text(0));
			pItem->removeChild(item);
		}
		emit clean();
	}

	void Geant4ControlPanel::addVolume()
	{
		auto item = currentItem();
		auto itemText = item->text(0);
		auto dataModel = DataManager::getInstance()->getGeant4DataModel();
		auto anas = dataModel->getAnalysisItems();
		auto ana = dataModel->getAnalysisItems().value(itemText);

		DialogAddVolume dlg(this, itemText);
		if (QDialog::Accepted == dlg.exec())
		{
			auto solids = DataManager::getInstance()->getSolidsManager();
			int solidNum = solids->getSolidCount();

			for (int iCnt = 0; iCnt < solidNum; ++iCnt)
			{
				auto solidName = solids->getSolidAt(iCnt)->getName();
				for (int i = 0; i < item->childCount(); i++)
				{
					if (solidName == item->child(i)->text(0))
					{
						QMessageBox::information(this, "Warning", "You have chosen one...");
						return;
					}
				}
			}

			if (item->type() == Geant4CaAnItem)
			{
				QString volName = dlg.getSelectedVolumeName();
				ana.volumes.append(volName);
				anas.insert(ana.name, ana);
				dataModel->setAnalysisItems(anas);

				QTreeWidgetItem * voItem = new QTreeWidgetItem(item, Geant4CaAnVoItem);
				voItem->setText(0, volName);
			}
			else
				Q_ASSERT(0);
		}
	}

	void Geant4ControlPanel::addVolumeInterface()
	{
		auto item = currentItem();
		auto itemText = item->text(0);
		auto dataModel = DataManager::getInstance()->getGeant4DataModel();
		auto anas = dataModel->getAnalysisItems();
		auto ana = dataModel->getAnalysisItems().value(itemText);
		DialogAddVolumeInterface dlg(this, ana.name, ana.volumeInterfaces);
		if (QDialog::Accepted == dlg.exec())
		{
			QString strA = dlg.getListA();
			QString strB = dlg.getListB();
			
			QTreeWidgetItem * curItem = currentItem();
			int num = 0;
			for (int i = 0; i < curItem->childCount(); i++)
			{
				if (curItem->child(i)->text(0) == "Relation")
					num++;
			}
			if (num == 0)
			{
				QTreeWidgetItem * item = new QTreeWidgetItem(curItem, Geant4CaAnRelationItem);
				item->setText(0, "Relation");
			}
			QList<_G4Relation> list = DataManager::getInstance()->getGeant4DataModel()->getRelationsItem(curItem->text(0));
			for(int i = 0; i < list.size(); i++)
			{
				if ((list.at(i).objA == strA && list.at(i).objB == strB) || (list.at(i).objA == strB && list.at(i).objB == strA))
				{
					QMessageBox::information(this, "Warning", "The correspondence already exists...");
					return;
				}
			}
			_G4Relation relation;
			relation.objA = strA;
			relation.objB = strB;
			DataManager::getInstance()->getGeant4DataModel()->setRelations(curItem->text(0), relation);
		}
	}

	void Geant4ControlPanel::deleteVolumeInterface()
	{
		auto item = currentItem();
		auto pItem = item->parent();
		if (item && pItem)
		{
			DataManager::getInstance()->getGeant4DataModel()->deleteAllRelationsItem();
			pItem->removeChild(item);
		}
		emit clean();
	}

	void Geant4ControlPanel::addHistogram()
	{
		DialogAddAnalysis dlg(this);
		dlg.renameWindow(tr("Add Histogram"));
		if (QDialog::Accepted == dlg.exec())
		{
			QString hisName = dlg.getAnalysisName();
			QTreeWidgetItem * hisItem = new QTreeWidgetItem(currentItem(), Geant4CaAnHiItem);
			hisItem->setText(0, hisName);
		}
	}

	void Geant4ControlPanel::deleteHistogram()
	{
		auto item = currentItem();
		auto pItem = item->parent();
		if (item && pItem)
		{
			DataManager::getInstance()->getGeant4DataModel()->deleteAnalysisItem(item->text(0));
			pItem->removeChild(item);
		}
		emit clean();
	}

	void Geant4ControlPanel::deleteVolume()
	{
		auto item = currentItem();
		auto pItem = item->parent();
		if (item && pItem && item->type() == Geant4CaAnVoItem && pItem->type() == Geant4CaAnItem)
		{
			auto dataModel = DataManager::getInstance()->getGeant4DataModel();
			dataModel->deleteAnalysisItemVolume(pItem->text(0), item->text(0));
			pItem->removeChild(item);
		}
		emit clean();
	}

	void Geant4ControlPanel::deleteGeometry()
	{
		auto item = currentItem();
		auto pItem = item->parent();
		if (item && pItem)
		{
			DataManager::getInstance()->getSolidsManager()->deleteSolidByName(item->text(0));
			DataManager::getInstance()->getGeant4DataModel()->deleteGeometriesEnergyThreshold(item->text(0));
			pItem->removeChild(item);
		}
		emit clean();
	}

}