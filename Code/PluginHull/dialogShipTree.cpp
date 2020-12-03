#include "dialogShipTree.h"
#include "ui_dialogHullCut.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <QMessageBox>
#include "GeoCommandMakeHullCut.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QTreeWidget>
#include "mainWindow/mainWindow.h"
#include <QDebug>
#include "DataProperty/modelTreeItemType.h"
#include "dialogModelImport.h"
#include "dialogConfigOceanCurrent.h"
#include "dialogConfigWave.h"
#include "dialogConfigWaveSpectrum.h"
#include "dialogConfigWind.h"
#include <QDialog>
#include "KitShipTreeItemType.h"
#include "dialogHullCut.h"
#include "settings/busAPI.h"
#include "ConfigOptions/GeometryConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "python/PyAgent.h"

namespace PluginShip
{
	ShipTreeDock::ShipTreeDock(GUI::MainWindow* m, QAction* ac) :
		DockWidgetBase(m, ModuleBase::ModuleType::ProjectWindows)
	{
		_ac = ac;
		init();
		this->repaintTitleBar();
	}

	ShipTreeDock::~ShipTreeDock()
	{
		
	}
	void ShipTreeDock::init()
	{
		QTreeWidget *treeWidget = new QTreeWidget;  
		QStringList headers;
		headers <<QString( tr("Ship Interaction Kit"));
		treeWidget->setHeaderLabels(headers);
		_treeWidget = treeWidget; 
		QStringList rootTextList;	
		rootTextList << tr("Hull") ;

		QTreeWidgetItem *root = new QTreeWidgetItem(treeWidget, rootTextList);
		QTreeWidgetItem* item1 = new QTreeWidgetItem(root, QStringList() << QString(tr("Model import")), ShipTreeItemType::ShipKitModelImport);
		QTreeWidgetItem* item2 = new QTreeWidgetItem(root, QStringList() << QString(tr("Type table")), ShipTreeItemType::ShipKitTypeTable);
		QTreeWidgetItem* item3 = new QTreeWidgetItem(root, QStringList() << QString(tr("Hull Cut")), ShipTreeItemType::ShipKitHullCut);
		QTreeWidgetItem* item4 = new QTreeWidgetItem(root, QStringList() << QString(tr("Parameter Config")), ShipTreeItemType::ParameterConfig);
		QTreeWidgetItem* item5 = new QTreeWidgetItem(root, QStringList() << QString(tr("ShipKit Post")), ShipTreeItemType::ShipKitPost);

		//破浪风树形结构。
		QTreeWidgetItem* itemwin = new QTreeWidgetItem(item4, QStringList() << QString(tr("Wind")), ShipTreeItemType::ParameterConfigWind);
		QTreeWidgetItem* itemwave = new QTreeWidgetItem(item4, QStringList() << QString(tr("Wave")), ShipTreeItemType::ParameterConfigWave);
		QTreeWidgetItem* itemoceancurrent = new QTreeWidgetItem(item4, QStringList() << QString(tr("OceanCurrent")), ShipTreeItemType::ParameterConfigOceanCurrent);
		QTreeWidgetItem* itemwavespectrum = new QTreeWidgetItem(item4, QStringList() << QString(tr("WaveSpectrum")), ShipTreeItemType::ParameterConfigWaveSpectrum);

		QList<QTreeWidgetItem *> rootList;
		rootList << root;
		treeWidget->insertTopLevelItems(0, rootList);  //将树形选项 添加入Tree控件
		setWidget(treeWidget);
		treeWidget->expandAll();
		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotOpenDialog(QTreeWidgetItem*, int)));
		
	}

	void ShipTreeDock::closeEvent(QCloseEvent *)
	{
		_ac->setEnabled(true);
	}

	void ShipTreeDock::slotOpenDialog(QTreeWidgetItem*item, int i)
	{
		QTreeWidgetItem* currentItem = _treeWidget->currentItem();
	
			switch (currentItem->type())
			{
				case  ShipTreeItemType::ShipKitModelImport:
				{
					QString dir = Setting::BusAPI::instance()->getWorkingDir();
					ConfigOption::GeometryConfig* geoconfig = ConfigOption::ConfigOption::getInstance()->getGeometryConfig();
					QString conSuffix = geoconfig->getImportSuffix().toLower();
					QStringList sl = conSuffix.split(";");
					conSuffix.clear();
					for (QString s : sl)
					{
						conSuffix += " *." + s;
					}
					conSuffix = QString("Geometry Files(%1)").arg(conSuffix);
					QString regSuffix{};

					QString title = tr("Import Geometry");
					QStringList filenames = QFileDialog::getOpenFileNames(this, title, dir, conSuffix + regSuffix + ";;All Files(*.*)");
					if (filenames.isEmpty()) return;

					QString files = filenames.join(",");
					QString pycode = QString("MainWindow.importGeometry(\"%1\")").arg(files);
					Py::PythonAagent::getInstance()->submit(pycode);
				}break;

				case  ShipTreeItemType::ShipKitTypeTable:
				{
					PluginShip::ModelImportDialog* dlg = new PluginShip::ModelImportDialog(_mainWindow);
					dlg->exec();
				}break;

				case  ShipTreeItemType::ShipKitHullCut:
				{
					PluginShip::HullCutDialog* dlg = new PluginShip::HullCutDialog(_mainWindow);
					dlg->show();
				}break;

				case  ShipTreeItemType::ParameterConfig:
					qDebug() << "ParameterConfig"; break;

				case  ShipTreeItemType::ParameterConfigWind:
				{
					PluginShip::ConfigWindDialog* dlg = new PluginShip::ConfigWindDialog(_mainWindow);
					dlg->show();
				} break;

				case  ShipTreeItemType::ParameterConfigWave:
				{
					PluginShip::ConfigWaveDialog* dlg = new PluginShip::ConfigWaveDialog(_mainWindow);
					dlg->show();
				} break;

				case  ShipTreeItemType::ParameterConfigOceanCurrent:
				{
					PluginShip::ConfigOceanCurrentDialog* dlg = new PluginShip::ConfigOceanCurrentDialog(_mainWindow);
					dlg->show();
				}break;

				case  ShipTreeItemType::ParameterConfigWaveSpectrum:
				{
					PluginShip::ConfigWaveSpectrumDialog* dlg = new PluginShip::ConfigWaveSpectrumDialog(_mainWindow);
					dlg->show();
				} break;

				case  ShipTreeItemType::ShipKitPost:
				{
					QString pycode = QString("MainWindow.openPost3D()");
					Py::PythonAagent::getInstance()->submit(pycode);
				} break;

				default:
					break;

		}
	}


}