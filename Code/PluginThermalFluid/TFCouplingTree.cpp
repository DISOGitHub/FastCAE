#include "TFCouplingTree.h"
#include "TFCouplingData.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "ThermalBoundary.h"
#include "ThermalCoupling.h"
#include "TFMaterial.h"
#include "settings/busAPI.h"
#include "INPFReader.h"
#include "INPFWriter.h"
#include "DialogThermalBoundary.h"
#include "DialogThermalCoupling.h"
#include "DialogThermalMaterial.h"
#include <QTreeWidgetItem>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <qdebug.h>

/*extern QString Mid(const QString &rStr, const int position, const int size = -1);*/

namespace ThermalFluid
{
	TFCouplingTree::TFCouplingTree(GUI::MainWindow* m)
		:ProjectTreeWithBasicNode(m)
	{
		
	}

	void TFCouplingTree::createTree(QTreeWidgetItem* root, GUI::MainWindow* mainwindow)
	{
		ProjectTreeBase::initBasicNode(root);
		_thermalRoot = new QTreeWidgetItem(_root, TFCTreeItemType::ThermalRoot);
		_thermalRoot->setText(0, QString(tr("Thermal")));
		_thermalRoot->setIcon(0, QIcon(":/TF/icon/tempure.png"));
		_fluidRoot = new QTreeWidgetItem(_root, TFCTreeItemType::FluidRoot);
		_fluidRoot->setText(0, QString(tr("Fluid")));
		_fluidRoot->setIcon(0, QIcon(":/TF/icon/fulid.png"));

		_tBoundaryRoot = new QTreeWidgetItem(_thermalRoot, TFCTreeItemType::ThermalBoundaryRoot);
		_tBoundaryRoot->setText(0, QString(tr("Thermal Boundary")));
		_tBoundaryRoot->setIcon(0, QIcon(":/TF/icon/tempure.png"));
		_tCouplingRoot = new QTreeWidgetItem(_thermalRoot, TFCTreeItemType::ThermalCouplingRoot);
		_tCouplingRoot->setText(0, QString(tr("Thermal Coupling")));
		_tCouplingRoot->setIcon(0, QIcon(":/TF/icon/tempure.png"));
		_tMaterialRoot = new QTreeWidgetItem(_thermalRoot, TFCTreeItemType::ThermalMaterialRoot);
		_tMaterialRoot->setText(0, QString(tr("Thermal Material")));
		_tMaterialRoot->setIcon(0, QIcon(":/TF/icon/tempure.png"));
		_root->setExpanded(true);

	}
	void TFCouplingTree::updateTree()
	{
		if (_modelData == nullptr)
			return;

		_thermalData = _modelData->getThermalData();

		if (_thermalData == nullptr)
			return;

		_tBoundaryRoot->takeChildren();
		_tCouplingRoot->takeChildren();
		_tMaterialRoot->takeChildren();

		updateBoundaryTree();
		updateCouplingTree();
		updateMaterialTree();
	}


	void TFCouplingTree::updateBoundaryTree()
	{
		const int n = _thermalData->getThermalBoundaryListCount();
		for (int i = 0; i < n; i++)
		{
			ThermalBoundary* data = _thermalData->getThermalBoundaryFromIndex(i);

			if (data == nullptr) continue;

			QTreeWidgetItem* aItem = new QTreeWidgetItem(_tBoundaryRoot, TFCTreeItemType::ThermalBoundaryChild);

			aItem->setText(0, QString("%1").arg(data->getName()));
			aItem->setData(0, Qt::UserRole, i);
			//item->setIcon(0, QIcon(":/TF/icon/tempure.png"));
		}
	}

	void TFCouplingTree::updateCouplingTree()
	{
		const int n = _thermalData->getThermalCouplingListCount();

		for (int i = 0; i < n; i++)
		{
			ThermalCoupling* data = _thermalData->getThermalCouplingFromIndex(i);

			if (data == nullptr)
				continue;

			QTreeWidgetItem* item = new QTreeWidgetItem(_tCouplingRoot, TFCTreeItemType::ThermalCouplingChild);
			item->setText(0, QString("%1").arg(data->getName()));
			item->setData(0, Qt::UserRole, i);
			//	item->setIcon(0, QIcon(":/TF/icon/tempure.png"));
		}
	}

	void TFCouplingTree::updateMaterialTree()
	{
		const int n = _thermalData->getMaterialListCount();

		for (int i = 0; i < n; i++)
		{
			TFMaterial* data = _thermalData->getMaterialFromIndex(i);

			if (data == nullptr)
				continue;

			QTreeWidgetItem* item = new QTreeWidgetItem(_tMaterialRoot, TFCTreeItemType::ThermalMaterialChild);
			item->setText(0, QString("%1").arg(data->getName()));
			item->setData(0, Qt::UserRole, i);
			//	item->setIcon(0, QIcon(":/TF/icon/tempure.png"));
		}
	}

	void TFCouplingTree::contextMenu(QMenu* menu)
	{
		ProjectTreeBase::contextMenu(menu);
		TFCTreeItemType type = (TFCTreeItemType)_currentItem->type();
		switch (type)
		{
		case TFCTreeItemType::ThermalRoot:
		{
			QAction *inputAction = menu->addAction(tr("Import INPF"));
			QAction *outputAction = menu->addAction(tr("Export INPF"));
			connect(inputAction, SIGNAL(triggered()), this, SLOT(slotInputINPF()));
			connect(outputAction, SIGNAL(triggered()), this, SLOT(slotOutputINPF()));
			break;
		}
		case TFCTreeItemType::ThermalBoundaryRoot:
		{
			QAction *actClearTbs = menu->addAction(tr("Clear All ThermalBoundarys"));
			QAction *actAppenedATb = menu->addAction(tr("Add A ThermalBoundary"));
			connect(actClearTbs, SIGNAL(triggered()), this, SLOT(on_actClearTbs_triggered()));
			connect(actAppenedATb, SIGNAL(triggered()), this, SLOT(on_actAppenedATb_triggered()));
			break;
		}
		case TFCTreeItemType::ThermalCouplingRoot:
		{
			QAction *actClearTcs = menu->addAction(tr("Clear All ThermalCouplings"));
			QAction *actAppenedATc = menu->addAction(tr("Add A ThermalCoupling"));
			connect(actClearTcs, SIGNAL(triggered()), this, SLOT(on_actClearTcs_triggered()));
			connect(actAppenedATc, SIGNAL(triggered()), this, SLOT(on_actAppenedATc_triggered()));
			break;
		}
		case TFCTreeItemType::ThermalMaterialRoot:
		{
			QAction *actClearTms = menu->addAction(tr("Clear All ThermalMaterials"));
			QAction *actAppenedATm = menu->addAction(tr("Add A ThermalMaterial"));
			connect(actClearTms, SIGNAL(triggered()), this, SLOT(on_actClearTms_triggered()));
			connect(actAppenedATm, SIGNAL(triggered()), this, SLOT(on_actAppenedATm_triggered()));
			break;
		}
		case TFCTreeItemType::ThermalBoundaryChild:
		{
			QAction* actEditTb = menu->addAction(tr("Edit This ThermalBoundary"));
			QAction* actDeleteTb = menu->addAction(tr("Delete This ThermalBoundary"));
			//QAction* actInsertTb = menu->addAction(tr("Insert A ThermalBoundary"));
			connect(actEditTb, SIGNAL(triggered()), this, SLOT(on_actEditTb_triggered()));
			connect(actDeleteTb, SIGNAL(triggered()), this, SLOT(on_actDeleteTb_triggered()));
			//connect(actInsertTb, SIGNAL(triggered()), this, SLOT(on_actInsertTb_triggered()));
			break;
		}
		case TFCTreeItemType::ThermalCouplingChild:
		{
			QAction* actEditTc = menu->addAction(tr("Edit This ThermalCoupling"));
			QAction* actDeleteTc = menu->addAction(tr("Delete This ThermalCoupling"));
			//QAction* actInsertTc = menu->addAction(tr("Insert A ThermalCoupling"));
			connect(actEditTc, SIGNAL(triggered()), this, SLOT(on_actEditTc_triggered()));
			connect(actDeleteTc, SIGNAL(triggered()), this, SLOT(on_actDeleteTc_triggered()));
			//connect(actInsertTc, SIGNAL(triggered()), this, SLOT(on_actInsertTc_triggered()));
			break;
		}
		case TFCTreeItemType::ThermalMaterialChild:
		{
			QAction* actEditTm = menu->addAction(tr("Edit This ThermalMaterial"));
			QAction* actDeleteTm = menu->addAction(tr("Delete This ThermalMaterial"));
			//QAction* actInsertTm = menu->addAction(tr("Insert A ThermalMaterial"));
			connect(actEditTm, SIGNAL(triggered()), this, SLOT(on_actEditTm_triggered()));
			connect(actDeleteTm, SIGNAL(triggered()), this, SLOT(on_actDeleteTm_triggered()));
			//connect(actInsertTm, SIGNAL(triggered()), this, SLOT(on_actInsertTm_triggered()));
			break;
		}
		default:
			break;
		}
	}

	void TFCouplingTree::reTranslate()
	{
		if (nullptr != _thermalRoot)
			_thermalRoot->setText(0, QString(tr("Thermal")));
		if (nullptr != _fluidRoot)
			_fluidRoot->setText(0, QString(tr("Fluid")));
		if (_tBoundaryRoot != nullptr)
			_tBoundaryRoot->setText(0, QString(tr("Thermal Boundary")));
		if (_tCouplingRoot != nullptr)
			_tCouplingRoot->setText(0, QString(tr("Thermal Coupling")));
		if (_tMaterialRoot != nullptr)
			_tMaterialRoot->setText(0, QString(tr("Thermal Material")));
	}

	void TFCouplingTree::transferData()
	{
		_modelData = dynamic_cast<TFCouplingModelData*>(_data);
	}

	void TFCouplingTree::slotInputINPF()
	{
		QString wkdir = Setting::BusAPI::instance()->getWorkingDir();
		QString file = QFileDialog::getOpenFileName(_mainWindow, tr("Input INPF"), wkdir, tr("INPF(*INPF*)"));
		if (file.isEmpty()) return;
		if (nullptr == _modelData) return;
		INPFReader reader(file, _modelData);
		bool ok = reader.read();
		if (!ok)
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("INPF read failed!"));
			return;
		}
		_mainWindow->getSubWindowManager()->openPreWindow();
		_mainWindow->getSubWindowManager()->updatePreMeshActor();
		emit _mainWindow->updateMeshTreeSig();
		emit _mainWindow->updateSetTreeSig();
		emit _mainWindow->updateActionStatesSig();
		//		updateActionsStates();
		updateTree();
	}

	void TFCouplingTree::singleClicked()
	{
		if (_currentItem == nullptr)	return;

		TFCTreeItemType type = (TFCTreeItemType)_currentItem->type();

		if (type == TFCTreeItemType::ThermalBoundaryChild)
		{
			int index = _currentItem->data(0, Qt::UserRole).toInt();
			ThermalBoundary* data = _thermalData->getThermalBoundaryFromIndex(index);
			if (data == nullptr)	return;
			emit disPlayProp(data);
			QString name = data->getGroup();
			auto set = TFCouplingModelData::getMeshSetByShortName(name);
			if (set != nullptr) emit _mainWindow->highLightSetSig(set);

			if (data->getThermalType().size() == 0)    return;
			if (data->getThermalType().at(0) == "INTERP")
			{
				int n = data->getQCount();
				for (int i = 0; i < n; i++)
					double* q = data->getQFromIndex(i);
			}
			else
			{
				int n = data->getQCount();
				for (int i = 0; i < n; i++)
					double* q = data->getQFromIndex(i);
			}

		}
		else if (type == TFCTreeItemType::ThermalCouplingChild)
		{
			int index = _currentItem->data(0, Qt::UserRole).toInt();

			ThermalCoupling* data = _thermalData->getThermalCouplingFromIndex(index);
			if (data == nullptr)	return;
			emit disPlayProp(data);

			QString one = data->getGroupOne();
			QString two = data->getGroupTwo();

			auto setOne = TFCouplingModelData::getMeshSetByShortName(one);
			if (setOne != nullptr) emit _mainWindow->highLightSetSig(setOne);
			auto setTwo = TFCouplingModelData::getMeshSetByShortName(two);
			if (setTwo != nullptr) emit _mainWindow->highLightSetSig(setTwo);
		}
		else if (type == TFCTreeItemType::ThermalMaterialChild)
		{
			int index = _currentItem->data(0, Qt::UserRole).toInt();
			TFMaterial* data = _thermalData->getMaterialFromIndex(index);
			if (data == nullptr)	return;
			emit disPlayProp(data);
		}
	}

	void TFCouplingTree::doubleClicked()
	{
		if (_currentItem == nullptr)	return;
		TFCTreeItemType type = (TFCTreeItemType)_currentItem->type();
		switch (type)
		{
		case TFCTreeItemType::ThermalBoundaryChild:
			on_actEditTb_triggered();
			break;
		case TFCTreeItemType::ThermalCouplingChild:
			on_actEditTc_triggered();
			break;
		case  TFCTreeItemType::ThermalMaterialChild:
			on_actEditTm_triggered();
			break;
		}
	}

	void TFCouplingTree::slotOutputINPF()
	{
		QString wkdir = Setting::BusAPI::instance()->getWorkingDir();
		QString file = QFileDialog::getExistingDirectory(_mainWindow, tr("Export INPF"), wkdir, QFileDialog::ShowDirsOnly);
		if (file.isEmpty()) return;
		if (nullptr == _modelData) return;
		INPFWriter writer(file, _modelData);
		bool ok = writer.write();
		if (!ok)
		{
			QMessageBox::warning(_mainWindow, tr("Warning"), tr("INPF Write failed!"));
			return;
		}
		else
		{
			QMessageBox::warning(_mainWindow, tr("Infomation"), tr("INPF Write successful!"));
			return;
		}
	}

	void TFCouplingTree::on_actClearTbs_triggered()
	{
		emit disPlayProp(nullptr);
		_thermalData->clearBoundaryList();
		_tBoundaryRoot->takeChildren();
	}

	void TFCouplingTree::on_actDeleteTb_triggered()
	{
		int index = _currentItem->data(0, Qt::UserRole).toInt();
		ThermalBoundary* tb = _thermalData->getThermalBoundaryFromIndex(index);
		if (!tb)	return;
		_thermalData->removeThermalBoundary(tb);
		_tBoundaryRoot->removeChild(_currentItem);
		int n = _tBoundaryRoot->childCount();
		for (int i = index; i < n; i++)
		{
			ThermalBoundary* data = _thermalData->getThermalBoundaryFromIndex(i);
			QTreeWidgetItem* aItem = _tBoundaryRoot->child(i);
			aItem->setText(0, QString("%1").arg(data->getName()));
			aItem->setData(0, Qt::UserRole, i);
		}
		emit disPlayProp(nullptr);
	}

	void TFCouplingTree::on_actAppenedATb_triggered()
	{
		ThermalBoundary* tb = iniAThermalBoundary();
		_thermalData->appendThermalBoundaryList(tb);
		ThermalBoundaryDialog dlg(_mainWindow, tb, this);
		dlg.iniEmptyDlg();
		dlg.exec();
	}

	void TFCouplingTree::on_actInsertTb_triggered()
	{
		ThermalBoundary* tb = iniAThermalBoundary();
		if (!tb)	return;
		emit disPlayProp(tb);
		int index = _tBoundaryRoot->indexOfChild(_currentItem);
		_thermalData->_tBoundaryList.insert(index, tb);

		QTreeWidgetItem* aItem = new QTreeWidgetItem(ThermalBoundaryChild);
		_tBoundaryRoot->insertChild(index, aItem);

		int n = _tBoundaryRoot->childCount();
		for (int i = index; i < n; i++)
		{
			ThermalBoundary* data = _thermalData->getThermalBoundaryFromIndex(i);
			QTreeWidgetItem* bItem = _tBoundaryRoot->child(i);
			bItem->setText(0, QString("%1").arg(data->getName()));
			bItem->setData(0, Qt::UserRole, i);
		}
		_tBoundaryRoot->child(index)->setSelected(true);
		_currentItem->setSelected(false);
	}

	void TFCouplingTree::on_actEditTb_triggered()
	{
		int index = _currentItem->data(0, Qt::UserRole).toInt();
		ThermalBoundary* tb = _thermalData->getThermalBoundaryFromIndex(index);
		if (!tb)	return;
		ThermalBoundaryDialog dlg(_mainWindow, tb, this);
		dlg.iniCurrentItemDlg();
		dlg.exec();
	}

	void TFCouplingTree::on_actClearTcs_triggered()
	{
		emit disPlayProp(nullptr);
		_thermalData->clearCouplingList();
		_tCouplingRoot->takeChildren();
	}

	void TFCouplingTree::on_actDeleteTc_triggered()
	{
		int index = _currentItem->data(0, Qt::UserRole).toInt();
		ThermalCoupling* tc = _thermalData->getThermalCouplingFromIndex(index);
		if (!tc)	return;
		_thermalData->removeThermalCoupling(tc);
		_tCouplingRoot->removeChild(_currentItem);
		int n = _tCouplingRoot->childCount();
		for (int i = index; i < n; i++)
		{
			ThermalCoupling* data = _thermalData->getThermalCouplingFromIndex(i);
			QTreeWidgetItem* aItem = _tCouplingRoot->child(i);
			aItem->setText(0, QString("%1").arg(data->getName()));
			aItem->setData(0, Qt::UserRole, i);
		}
		emit disPlayProp(nullptr);
	}

	void TFCouplingTree::on_actAppenedATc_triggered()
	{
		ThermalCoupling* tc = iniAThermalCoupling();
		_thermalData->appendThermalCouplingList(tc);
		ThermalCouplingDialog dlg(_mainWindow, tc, this);
		dlg.iniEmptyDlg();
		dlg.exec();
	}

	void TFCouplingTree::on_actInsertTc_triggered()
	{
		ThermalCoupling* tc = iniAThermalCoupling();
		if (!tc)	return;
		emit disPlayProp(tc);
		int index = _tCouplingRoot->indexOfChild(_currentItem);
		_thermalData->_tCouplingList.insert(index, tc);

		QTreeWidgetItem* aItem = new QTreeWidgetItem(ThermalCouplingChild);
		_tCouplingRoot->insertChild(index, aItem);

		int n = _tCouplingRoot->childCount();
		for (int i = index; i < n; i++)
		{
			ThermalCoupling* data = _thermalData->getThermalCouplingFromIndex(i);
			QTreeWidgetItem* bItem = _tCouplingRoot->child(i);
			bItem->setText(0, QString("%1").arg(data->getName()));
			bItem->setData(0, Qt::UserRole, i);
		}
		_tCouplingRoot->child(index)->setSelected(true);
		_currentItem->setSelected(false);
	}

	void TFCouplingTree::on_actEditTc_triggered()
	{
		int index = _currentItem->data(0, Qt::UserRole).toInt();
		ThermalCoupling* tc = _thermalData->getThermalCouplingFromIndex(index);
		if (!tc)	return;
		ThermalCouplingDialog dlg(_mainWindow, tc, this);
		dlg.iniCurrentItemDlg();
		dlg.exec();
	}

	void TFCouplingTree::on_actClearTms_triggered()
	{
		emit disPlayProp(nullptr);
		_thermalData->clearMaterialList();
		_tMaterialRoot->takeChildren();
	}

	void TFCouplingTree::on_actDeleteTm_triggered()
	{
		int index = _currentItem->data(0, Qt::UserRole).toInt();
		TFMaterial* tm = _thermalData->getMaterialFromIndex(index);
		if (!tm)	return;
		_thermalData->removeMaterial(tm);

		_tMaterialRoot->removeChild(_currentItem);
		int n = _tMaterialRoot->childCount();
		for (int i = index; i < n; i++)
		{
			TFMaterial* data = _thermalData->getMaterialFromIndex(i);
			QTreeWidgetItem* aItem = _tMaterialRoot->child(i);
			aItem->setText(0, QString("%1").arg(data->getName()));
			aItem->setData(0, Qt::UserRole, i);
		}
		emit disPlayProp(nullptr);
	}

	void TFCouplingTree::on_actAppenedATm_triggered()
	{
		TFMaterial* tm = iniATFMaterial();
		_thermalData->appendMaterialList(tm);
		ThermalMaterialDialog dlg(_mainWindow, tm, this);
		dlg.iniEmptyDlg();
		dlg.exec();
	}

	void TFCouplingTree::on_actInsertTm_triggered()
	{
		TFMaterial* tm = iniATFMaterial();
		if (!tm)	return;
		emit disPlayProp(tm);
		int index = _tMaterialRoot->indexOfChild(_currentItem);
		_thermalData->_tMaterialList.insert(index, tm);

		QTreeWidgetItem* aItem = new QTreeWidgetItem(ThermalMaterialChild);
		_tMaterialRoot->insertChild(index, aItem);

		int n = _tMaterialRoot->childCount();
		for (int i = index; i < n; i++)
		{
			TFMaterial* data = _thermalData->getMaterialFromIndex(i);
			QTreeWidgetItem* bItem = _tMaterialRoot->child(i);
			bItem->setText(0, QString("%1").arg(data->getName()));
			bItem->setData(0, Qt::UserRole, i);
		}
		_tMaterialRoot->child(index)->setSelected(true);
		_currentItem->setSelected(false);
	}

	void TFCouplingTree::on_actEditTm_triggered()
	{
		int index = _currentItem->data(0, Qt::UserRole).toInt();
		TFMaterial* tm = _thermalData->getMaterialFromIndex(index);
		if (!tm)	return;
		ThermalMaterialDialog dlg(_mainWindow, tm, this);
		dlg.iniCurrentItemDlg();
		dlg.exec();
	}

	int TFCouplingTree::getMaxTbID()
	{
		auto tbList = _thermalData->getThermalBoundaryList();
		if (tbList.size() == 0)	return 0;
		ThermalBoundary* tb{};
		QList<int> idList;
		foreach (tb, tbList)		
			idList << tb->getID();
		qSort(idList.begin(), idList.end());
		return idList.last();
	}

	int TFCouplingTree::getMaxTcID()
	{
		auto tcList = _thermalData->getThermalCouplingList();
		if (tcList.size() == 0)	return 0;
		ThermalCoupling* tc{};
		QList<int> idList;
		foreach(tc, tcList)
			idList << tc->getID();
		qSort(idList.begin(), idList.end());
		return idList.last();
	}

	int TFCouplingTree::getMaxTmID()
	{
		auto tmList = _thermalData->getMaterialList();
		if (tmList.size() == 0)	return 0;
		TFMaterial* tm{};
		QList<int> idList;
		foreach(tm, tmList)
			idList << tm->getID();
		qSort(idList.begin(), idList.end());
		return idList.last();
	}

	ThermalBoundary* TFCouplingTree::iniAThermalBoundary()
	{
		ThermalBoundary* tb = new ThermalBoundary(_thermalData);
		if (!tb)	return nullptr;
		int ID = getMaxTbID() + 1;
		QString name = QObject::tr("new ThermalBoundary") + ("%1");
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("yyyy.MM.dd");

		tb->setID(ID);
		tb->setName(name.arg(ID));
		tb->setGroup(current_date);
		tb->setTypeAndUnit(QString("NULL"));

		tb->appendProperty(QObject::tr("GroupName"), current_date);
		tb->appendProperty(QObject::tr("tbType"), QObject::tr("NULL"));
		tb->appendProperty(QObject::tr("Unit"), tr("NULL"));
		tb->appendProperty(QObject::tr("XValue"), 0.00);
		return tb;
	}

	ThermalCoupling* TFCouplingTree::iniAThermalCoupling()
	{
		ThermalCoupling* tc = new ThermalCoupling();
		if (!tc)	return nullptr;
		int ID = getMaxTcID() + 1;
		QString name = QObject::tr("new ThermalCoupling") + ("%1");

		tc->setID(ID);
		tc->setName(name.arg(ID));
		tc->setGroupOne("NULL");
		tc->setGroupTwo("NULL");	

		tc->appendProperty(QObject::tr("GroupOne"), tr("NULL"));
		tc->appendProperty(QObject::tr("GroupTwo"), tr("NULL"));
		tc->appendProperty(QObject::tr("tcType"), tr("NULL"));
		tc->appendProperty(QObject::tr("Computing coefficients"), 0.00);
		tc->appendProperty(QObject::tr("coefficient of heat transfer"), 0.00);
		return tc;
	}

	TFMaterial* TFCouplingTree::iniATFMaterial()
	{
		TFMaterial* tm = new TFMaterial();
		if (!tm)	return nullptr;
		int ID = getMaxTmID() + 1;
		QString name = QObject::tr("new ThermalMaterial") + ("%1");

		tm->setID(ID);
		tm->setName(name.arg(ID));

		tm->appendProperty(QObject::tr("RHO"), 0.00);
		tm->appendProperty(QObject::tr("CPP"), 0.00);
		tm->appendProperty(QObject::tr("KTHERM"), 0.00);
		tm->appendProperty(QObject::tr("E"), 0.00);
		tm->appendProperty(QObject::tr("ABSORP"), 0.00);
		tm->appendProperty(QObject::tr("SPECULARITY"), 0.00);
		tm->appendProperty(QObject::tr("REMISS"), 0.00);
		tm->appendProperty(QObject::tr("RABSOR"), 0.00);
		tm->appendProperty(QObject::tr("RSOLSPE"), 0.00);
		return tm;
	}
}

