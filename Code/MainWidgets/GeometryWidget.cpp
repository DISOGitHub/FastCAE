#include "GeometryWidget.h"
#include "mainWindow/mainWindow.h"
#include <QTreeWidgetItem>
#include "DataProperty/modelTreeItemType.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryDatum.h"
#include "MainWidgets/preWindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "DataProperty/PropertyBase.h"
#include "DataProperty/PropertyString.h"
#include "QApplication"
#include "QFile"
//#include "CreateGeometry.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GeometryConfig.h"
#include "ConfigOptions/MeshConfig.h"
#include "settings/busAPI.h"
#include "GeometryWidgets/geometryDialogFactory.h"
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandRemove.h"
#include "DialogGeometrysetRename.h"
#include "GeometryCommand/GeoCommandRemoveDatum.h"
#include <QMessageBox>

namespace MainWidget
{
	GeometryTreeWidget::GeometryTreeWidget(GUI::MainWindow* m, QWidget* parent /* = nullptr */)
	{
		_mainWindow = m;
		_data = Geometry::GeometryData::getInstance();
		this->setHeaderHidden(true);
		_root = new QTreeWidgetItem(this, TreeItemType::GeometryRoot);
		_root->setText(0, tr("Geometry"));
		_root->setIcon(0, QIcon(":/QUI/icon/geometry.png"));
		this->addTopLevelItem(_root);
		_root->setExpanded(true);

		_datumroot = new QTreeWidgetItem(this, TreeItemType::Datum);
		_datumroot->setText(0, tr("Datum"));
		_datumroot->setIcon(0, QIcon(":/QUI/icon/geometry.png"));
		this->addTopLevelItem(_datumroot);
		_datumroot->setExpanded(true);


		connect(_mainWindow, SIGNAL(updateGeometryTreeSig()), this, SLOT(updateTree()));
//		connect(_mainWindow, SIGNAL(updateGeometryTreeSig()), this, SLOT(updateTree()));
		connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(singleClicked(QTreeWidgetItem*, int)));
		connect(this, SIGNAL(updateDisplay(int, bool)), _mainWindow, SIGNAL(updateGeoDispalyStateSig(int, bool)));
		connect(this, SIGNAL(removeGeoData(int)), _mainWindow, SIGNAL(removeGeometryActorSig(int)));
		connect(this, SIGNAL(updateActionStates()), _mainWindow, SIGNAL(updateActionStatesSig()));
		connect(this, SIGNAL(showGeoDialog(QDialog*)), _mainWindow, SIGNAL(showDialogSig(QDialog*)));
		connect(this, SIGNAL(disPlayProp(DataProperty::DataBase*)), _mainWindow, SIGNAL(updateProperty(DataProperty::DataBase*)));
// 		connect(_mainWindow, SIGNAL(surfaceMeshSig(Geometry::GeometrySet*)), this, SLOT(surfaceMesh(Geometry::GeometrySet*)));
// 		connect(_mainWindow, SIGNAL(solidMeshSig(Geometry::GeometrySet*)), this, SLOT(solidMesh(Geometry::GeometrySet*)));
		connect(_mainWindow, SIGNAL(preWindowOpenedSig(MainWidget::PreWindow*)), this, SLOT(preWindowOpened(MainWidget::PreWindow*)));
		connect(_mainWindow, SIGNAL(closePreWindowSig()), this, SLOT(preWindowClosed()));
		connect(this, SIGNAL(clearHighLight()), _mainWindow, SIGNAL(updateGraphOptionsSig()));
//		connect(this, SIGNAL(updateGraphOptions()), m, SIGNAL(updateGraphOptionsSig()));
		connect(this, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)), _mainWindow, SIGNAL(highLightGeometrySetSig(Geometry::GeometrySet*, bool)));

		//connect(this, SIGNAL(removeDatumActor(Geometry::GeometryDatum*)),_preWindow , SIGNAL(removeDatumActor(Geometry::GeometryDatum*)));
		
	}

	GeometryTreeWidget::~GeometryTreeWidget()
	{
		if (_root != nullptr) delete _root;

// 		if (_geometry)
// 		{
// 			delete _geometry;
// 		}
// 		_geometry = NULL;
	}
	void GeometryTreeWidget::updateTree()
	{
		_root->takeChildren();

		removeItemWidget(_root, 0);
		delete _root;
		_root = new QTreeWidgetItem(this, TreeItemType::GeometryRoot);
		_root->setText(0, tr("Geometry"));
		_root->setIcon(0, QIcon(":/QUI/icon/geometry.png"));
		this->addTopLevelItem(_root);
		_root->setExpanded(true);
		_data->sort();
		const int n = _data->getGeometrySetCount();
		for (int i = 0; i < n; ++i)
		{
			Geometry::GeometrySet* gset = _data->getGeometrySetAt(i);
			const bool visable = gset->isVisible();
			QString name = gset->getName();
			name += QString("(%1)").arg(gset->getID());
			Qt::CheckState isvisable = Qt::Unchecked;
			if (visable) isvisable = Qt::Checked;
			QTreeWidgetItem* item = new QTreeWidgetItem(_root, GeometryRoot + 1);
			item->setCheckState(0, isvisable);
			item->setText(0, name);
			item->setIcon(0, QIcon(":/QUI/icon/geometry.png"));
		}

		_datumroot->takeChildren();
		removeItemWidget(_datumroot, 0);
		delete _datumroot;
		_datumroot = new QTreeWidgetItem(this, TreeItemType::Datum);
		_datumroot->setText(0, tr("Datum"));
		_datumroot->setIcon(0, QIcon(":/QUI/icon/geometry.png"));
		this->addTopLevelItem(_datumroot);
		_datumroot->setExpanded(true);

		QList<Geometry::GeometryDatum *>  datumList = _data->getGeometryDatum();
		for (int i = 0; i < datumList.size();++i)
		{ 
			Geometry::GeometryDatum* datum = datumList[i];
			QString name = datum->getName();
			const bool visable = datum->isVisible();
			Qt::CheckState isvisable = Qt::Unchecked;
			if (visable) isvisable = Qt::Checked;
			QTreeWidgetItem* item = new QTreeWidgetItem(_datumroot, DatumPlane);
			item->setCheckState(0, isvisable);
			item->setText(0, name);
			item->setIcon(0, QIcon(":/QUI/icon/geometry.png"));
		}


	}
	void GeometryTreeWidget::singleClicked(QTreeWidgetItem* item, int i)
	{
		_currentItem = currentItem();
		const int index = _root->indexOfChild(item);
		if (index < 0)
		{
			emit disPlayProp(nullptr); //清空属性框
			return;
		}
		bool visable = true;
		Qt::CheckState isvisable = item->checkState(0);
		if (isvisable == Qt::Unchecked) visable = false;

		auto geoset = _data->getGeometrySetAt(index);
		emit clearHighLight();
		emit highLightGeometrySet(geoset, true);

		_data->setVisable(index, visable);		
		emit updateDisplay(index, visable);
		emit disPlayProp(_data->getGeometrySetAt(index));
		if (!visable) emit clearHighLight();
	}
	void GeometryTreeWidget::contextMenuEvent(QContextMenuEvent *event)
	{
		_currentItem = currentItem();
		if (!_currentItem){ return; }
		if (_currentItem->type() == GeometryRoot + 1)
		{
			QMenu pop_menu;
			QAction* action = nullptr;

			action = pop_menu.addAction(QIcon(), tr("Edit"));
			action->setEnabled(isGeometryEditable());
			connect(action, SIGNAL(triggered()), this, SLOT(editGeometry()));

			action = pop_menu.addAction(QIcon(), tr("Rename"));
			connect(action, SIGNAL(triggered()), this, SLOT(changeName()));

			action = pop_menu.addAction(QIcon(), tr("Delete"));
			connect(action, SIGNAL(triggered()), this, SLOT(removeData()));

			pop_menu.exec(QCursor::pos());

		}
		else if (_currentItem->type() == GeometryRoot)
		{
			QMenu pop_menu;
			QAction* action = nullptr;

			ConfigOption::ConfigOption* option = ConfigOption::ConfigOption::getInstance();
			ConfigOption::GeometryConfig* geoconfig = option->getGeometryConfig();
			
			bool isimport = geoconfig->isImportGeometryEnabled();
			if (isimport)
			{
				action = pop_menu.addAction(QIcon(), tr("Import Geometry"));
				connect(action, SIGNAL(triggered()), _mainWindow, SLOT(on_importGeometry()));

				action = pop_menu.addAction(QIcon(), tr("Hide All"));
				connect(action, SIGNAL(triggered()), this, SLOT(hideAll()));

				action = pop_menu.addAction(QIcon(), tr("Show All"));
				connect(action, SIGNAL(triggered()), this, SLOT(showAll()));
			}
			
			pop_menu.exec(QCursor::pos());
		}
		else if (_currentItem->type() == DatumPlane)
		{
			QMenu pop_menu;
			QAction* action = nullptr;
			ConfigOption::ConfigOption* option = ConfigOption::ConfigOption::getInstance();
			ConfigOption::GeometryConfig* geoconfig = option->getGeometryConfig();

			bool isimport = geoconfig->isImportGeometryEnabled();
			if (isimport)
			{
				action = pop_menu.addAction(QIcon(), tr("Rename"));
				connect(action, SIGNAL(triggered()), this, SLOT(slot_datumPlaneRename()));
				action = pop_menu.addAction(QIcon(), tr("Delete"));
				connect(action, SIGNAL(triggered()), this, SLOT(slot_removeDatum()));
			}

			pop_menu.exec(QCursor::pos());
		}
	}

	void GeometryTreeWidget::removeData()
	{
		const int index = _root->indexOfChild(_currentItem);
		if (index < 0) return;
		Command::GeoCommandRemove* c = new Command::GeoCommandRemove(_mainWindow, _preWindow);
		c->setIndex(index);
		Command::GeoComandList::getInstance()->executeCommand(c);

		emit updateActionStates();
		emit clearHighLight();
	}

	void GeometryTreeWidget::changeName()
	{
		const int index = _root->indexOfChild(_currentItem);
		if (index < 0) return;
		Geometry::GeometrySet* gset = _data->getGeometrySetAt(index);
		GeometrysetRenameDialog dlg(_mainWindow, gset, _currentItem);
		dlg.exec();
		
	}

	void GeometryTreeWidget::slot_datumPlaneRename()
	{
		const int index = _datumroot->indexOfChild(_currentItem);
		if (index < 0) return;
		Geometry::GeometryDatum* datum=_data->getDatumPlaneByIndex(index);
		GeometrysetRenameDialog dlg(_mainWindow, datum, _currentItem);
		dlg.exec();
	}

	void GeometryTreeWidget::slot_removeDatum()
	{
		const int index = _datumroot->indexOfChild(_currentItem);
		if (index < 0) return;
		Command::GeoCommandRemoveDatum* c = new Command::GeoCommandRemoveDatum(_mainWindow, _preWindow);
		c->setIndex(index);
		Command::GeoComandList::getInstance()->executeCommand(c);
	}

	void GeometryTreeWidget::preWindowOpened(MainWidget::PreWindow* p)
	{
		_preWindow = p;
	}

	void GeometryTreeWidget::editGeometry()
	{
		const int index = _root->indexOfChild(_currentItem);
		if (index < 0) return;
		Geometry::GeometrySet * geometry = _data->getGeometrySetAt(index);

		QDialog* d = GeometryWidget::GeometryDialogFactory::editGeometryDialog(_mainWindow, _preWindow, geometry);
		if (d == nullptr) return;
		emit showGeoDialog(d);
	}

	bool GeometryTreeWidget::isGeometryEditable()
	{
		const int index = _root->indexOfChild(_currentItem);
		if (index < 0) return false;

		Geometry::GeometrySet * geometry = _data->getGeometrySetAt(index);
		if (geometry == nullptr) return false;
		return geometry->isEditable();
	}

	void GeometryTreeWidget::preWindowClosed()
	{
		_preWindow = nullptr;
	}

	void GeometryTreeWidget::registerEnabledModule()
	{

	}

	void GeometryTreeWidget::hideAll()
	{
		const int nc = _root->childCount();
		for (int i = 0; i < nc; ++i)
		{
			QTreeWidgetItem* item = _root->child(i);
			item->setCheckState(0, Qt::Unchecked);
		}

		const int nset = _data->getGeometrySetCount();
		for (int i = 0; i < nset; ++i)
		{
			_data->setVisable(i, false);
			emit updateDisplay(i, false);
			emit disPlayProp(_data->getGeometrySetAt(i));
		}
		emit clearHighLight();
	}

	void GeometryTreeWidget::showAll()
	{
		const int nc = _root->childCount();
		for (int i = 0; i < nc; ++i)
		{
			QTreeWidgetItem* item = _root->child(i);
			item->setCheckState(0, Qt::Checked);
		}

		const int nset = _data->getGeometrySetCount();
		for (int i = 0; i < nset; ++i)
		{
			_data->setVisable(i, true);
			emit updateDisplay(i, true);
			emit disPlayProp(_data->getGeometrySetAt(i));
		}
		emit clearHighLight();
	}


}