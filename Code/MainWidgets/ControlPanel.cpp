#include "ControlPanel.h"
#include <ui_ControlPanel.h>
#include "PhysicsWidget.h"
#include "GeometryWidget.h"
#include "MeshWidget.h"
#include "PropertyTable.h"
#include <QObject>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLineEdit>
#include "mainWindow/mainWindow.h"
#include "DataProperty/ParameterList.h"
#include "DataProperty/PropertyList.h"
#include "DataProperty/PropertyInt.h"
#include "DataProperty/PropertyDouble.h"
#include "DataProperty/PropertyString.h"
#include "DataProperty/PropertyColor.h"
#include "DataProperty/PropertyBool.h"
#include "DataProperty/PropertyPoint.h"
#include "PipelineObjTreeDockWidget.h"
#include "PipelineObjPropDockWidget.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/PostConfig.h"
#include "MainWidgetsPy.h"
#include <QDebug>

namespace MainWidget
{
	ControlPanel::ControlPanel(GUI::MainWindow *mainwindow) :
		DockWidgetBase(mainwindow, ModuleBase::ModuleType::ProjectWindows)
	{
		_ui = new Ui::ControlPanel();
		_ui->setupUi(this);
		_physicsWidget = new PhysicsWidget(mainwindow);
		_geometryWidget = new GeometryTreeWidget(mainwindow);
		_meshWidget = new MeshWidget(mainwindow);
		_propTable = new PropertyTable(mainwindow, this);
		Init();
		repaintTitleBar();
		_ui->propTabWidget->tabBar()->hide();
		MainWidgetPy::init(mainwindow, this, _geometryWidget, _meshWidget, _physicsWidget);
	}

	ControlPanel::~ControlPanel()
	{
		delete _meshWidget;
		delete _physicsWidget;
		delete _geometryWidget;
		delete _propTable;
		delete _ui;
	}

	void ControlPanel::Init()
	{
		_ui->physicsLayout->addWidget(_physicsWidget);
		_ui->geometryLayout->addWidget(_geometryWidget);
		_ui->meshLayout->addWidget(_meshWidget);
		_ui->propLayout->addWidget(_propTable);
		_ui->projectTab->resize(300, 200);

		_ui->propTabWidget->setTabPosition(QTabWidget::South);

		//		registerEnabledModule();
		connect(_mainWindow, SIGNAL(updateProperty(DataProperty::DataBase*)), this, SLOT(updataPropertyTab(DataProperty::DataBase*)));
		connect(_ui->projectTab, SIGNAL(currentChanged(int)), this, SLOT(changePropTabByProjectPage(int)));
		connect(_mainWindow, SIGNAL(updateParaWidget(QWidget*)), this, SLOT(updataParaWidget(QWidget*)));
		connect(this, SIGNAL(updateActionStates()), _mainWindow, SIGNAL(updateActionStatesSig()));
		connect(_physicsWidget, SIGNAL(updatePropertyTableTree()), _propTable, SLOT(updateTable()));
		connect(_meshWidget, SIGNAL(removeCaseComponentSig(int)), _physicsWidget, SLOT(removeCaseComponentSlot(int)));
		connect(_meshWidget, SIGNAL(renameCaseComponentSig(int)), _physicsWidget, SLOT(renameCaseComponentSlot(int)));
		connect(_geometryWidget, SIGNAL(removeCaseComponentSig(int)), _physicsWidget, SLOT(removeCaseComponentSlot(int)));
		connect(_geometryWidget, SIGNAL(renameCaseComponentSig(int)), _physicsWidget, SLOT(renameCaseComponentSlot(int)));
	}
	void ControlPanel::resizeEvent(QResizeEvent *e)
	{
		int w = this->width();
		_propTable->resize(w);
		QDockWidget::resizeEvent(e);
	}


	void ControlPanel::reTranslate()
	{
		if (nullptr == _ui) return;

		const int index = _ui->propTabWidget->currentIndex();

		_ui->retranslateUi(this);
		updataPropertyTab(_data);
		DockWidgetBase::reTranslate();
		//		setHorizontalHeader( tr("Name"), tr("Property"));
		_propTable->retranslate();
		//		_physicsWidget->updateTree();
		_physicsWidget->reTranslate();
		if (_meshWidget != nullptr)
			_meshWidget->updateTree();
		if (_geometryWidget != nullptr)
			_geometryWidget->updateTree();

		_ui->propTabWidget->setCurrentIndex(index);
	}

	void ControlPanel::on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int proID)
	{
		Q_UNUSED(evevntType);
		Q_UNUSED(item);
		Q_UNUSED(proID);
	}

	void ControlPanel::updataPropertyTab(DataProperty::DataBase* data)
	{
		_ui->propTabWidget->setCurrentIndex(0);
		_propTable->updateTable(data);
		emit resizeEvent(nullptr);
	}

	void ControlPanel::updatePostWidget(QWidget* tree, QWidget* prop)
	{
		if (_postTree != nullptr)
		{
			_ui->postLayout->removeWidget(_postTree);
			_postTree->setParent(nullptr);
			_postTree = nullptr;
		}
		if (_postProp != nullptr)
		{
			_ui->postPropLayout->removeWidget(_postProp);
			_postProp->setParent(nullptr);
			_postProp = nullptr;
		}

		// 		_ui->projectTab->setCurrentIndex(2);
		// 		_ui->propTabWidget->setCurrentIndex(0);

		if (tree != nullptr)
		{
			_ui->postLayout->addWidget(tree);
			_postTree = tree;
			//			_ui->projectTab->setCurrentIndex(3);
		}
		if (prop != nullptr)
		{
			_ui->postPropLayout->addWidget(prop);
			_postProp = prop;
			//			_ui->propTabWidget->setCurrentIndex(1);
		}

	}
	void ControlPanel::changePropTabByProjectPage(int index)
	{
		QWidget* w = _ui->projectTab->widget(index);
		if (w == _ui->post_tab)
		{
			_ui->propTabWidget->setCurrentIndex(1);
		}
		else
		{
			_ui->propTabWidget->setCurrentIndex(0);
		}
	}
	void ControlPanel::updataParaWidget(QWidget* w)
	{
		_ui->propTabWidget->setCurrentIndex(2);
		if (_paraWidget != nullptr)
		{
			_ui->paraLayout->removeWidget(_paraWidget);
			delete _paraWidget;
			_paraWidget = nullptr;
		}
		if (w == nullptr) return;

		_paraWidget = w;
		_ui->paraLayout->addWidget(w);
	}
	void ControlPanel::registerEnabledModule()
	{
		ConfigOption::ConfigOption* option = ConfigOption::ConfigOption::getInstance();
		ConfigOption::PostConfig* pc = option->getPostConfig();
		_ui->projectTab->clear();

		bool ispost = pc->isPostEnabled();
		if (ispost)
			_ui->projectTab->insertTab(0, _ui->post_tab, QString());

		_ui->projectTab->insertTab(0, _ui->physics_tab, QString());

		bool ismesh = option->isMeshEnabled();
		if (ismesh)
		{
			_ui->projectTab->insertTab(0, _ui->mesh_tab, QString());
			_meshWidget->registerEnabledModule();
		}

		bool geo = option->isGeometryEnabled();
		if (geo)
		{
			_ui->projectTab->insertTab(0, _ui->geometry_tab, QString());
			_geometryWidget->registerEnabledModule();
		}

		_physicsWidget->updateMaterialStatus();
		_ui->projectTab->setCurrentWidget(_ui->physics_tab);

		QList<QWidget*> addinWidgets = _addinWidget.keys();
		for (QWidget* w : addinWidgets)
		{
			_ui->projectTab->addTab(w, _addinWidget.value(w));
			_ui->projectTab->setCurrentWidget(w);
		}

		_ui->retranslateUi(this);
	}

	void ControlPanel::addTabWidgetPlugin(QWidget* w, QString name)
	{   
		_addinWidget.insert(w, name);
	}
	

	void ControlPanel::removeTabWidgetPlugin(QWidget* w)
	{
		_addinWidget.remove(w);
	}

	void ControlPanel::setCurrentWidget(QWidget* w)
	{
		_ui->projectTab->setCornerWidget(w);
	}


	void ControlPanel::clearWidget()
	{
		//		_ui->propertyTable -> setRowCount(0);

		if (_paraWidget != nullptr)
		{
			_ui->paraLayout->removeWidget(_paraWidget);
			delete _paraWidget;
			_paraWidget = nullptr;
		}
	}


}
