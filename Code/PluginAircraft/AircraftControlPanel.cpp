#include "AircraftControlPanel.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "AircraftTreeItemType.h"
#include <QFileDialog>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include "settings/busAPI.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverConfig.h"
#include "dialogAircraft.h"
#include "dialogAerodynamic.h"
#include "dialogEngine.h"
namespace Aircraft
{
	AircraftControlPanel::AircraftControlPanel(GUI::MainWindow* m) :_mainWindow(m)
	{

		initProjectTree();
		connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(singleClicked(QTreeWidgetItem*, int)));
	}

	AircraftControlPanel::~AircraftControlPanel()
	{
		this->clear();
	}
	void AircraftControlPanel::init()
	{
		this->setHeaderHidden(true);
		
		QStringList rootTextList;	
		rootTextList << tr("Aircraft") ;

		_root = new QTreeWidgetItem(this, rootTextList, AircraftTreeItemType::AircraftRoot);
		_AircraftPara = new QTreeWidgetItem(_root, QStringList(tr("AircraftParameter")), AircraftTreeItemType::AircraftParameter);
		_AerodynamicPara = new QTreeWidgetItem(_root, QStringList("AerodynamicPara"), AircraftTreeItemType::AerodynamicPara);
		_EnginePara = new QTreeWidgetItem(_root, QStringList("EnginePara"), AircraftTreeItemType::EnginePara);
		//_solidRoot->setIcon(0, QIcon("://QUI/geometry/chamfer.png"));
		_root->setExpanded(true);
	}



	void AircraftControlPanel::initProjectTree()
	{
		this->setHeaderHidden(true);

		QStringList rootTextList;
		rootTextList << tr("Aircraft");
		_root = new QTreeWidgetItem(this, rootTextList, AircraftTreeItemType::AircraftRoot);
		_AircraftPara = new QTreeWidgetItem(_root, QStringList(tr("AircraftParameter")), AircraftTreeItemType::AircraftParameter);
		_AerodynamicPara = new QTreeWidgetItem(_root, QStringList("AerodynamicPara"), AircraftTreeItemType::AerodynamicPara);
		_EnginePara = new QTreeWidgetItem(_root, QStringList("EnginePara"), AircraftTreeItemType::EnginePara);
		_root->setExpanded(true);
	}
	
	

	void AircraftControlPanel::singleClicked(QTreeWidgetItem*item, int i)
	{
		if (item == nullptr) return;
		AircraftTreeItemType  type = (AircraftTreeItemType)item->type();
		switch (type)
		{
		case AircraftParameter:
		{
			
			auto dlg = new CreateAircraftDialog();
			this->showDialog(dlg);
			break;
		}
		case AerodynamicPara:
		{
			auto dlg = new CreateAerodynamicDialog();
			this->showDialog(dlg);
			break;
		}
			
		case EnginePara:
		{
			auto dlg = new CreateEngineDialog();
			this->showDialog(dlg);
			break;
		}
	
		default: 
			//emit clean();
			break;
		}
	}

	void AircraftControlPanel::SolveCalculate()
	{

	}

	void AircraftControlPanel::contextMenuEvent(QContextMenuEvent *event)
	{
		auto item = this->currentItem();
		if (item == nullptr) return;
		QMenu pop_menu;
		QAction* action = nullptr;
		QAction* actionSave = nullptr;
		AircraftTreeItemType  type = (AircraftTreeItemType)item->type();
		switch (type)
		{
		case AircraftRoot:
			action = pop_menu.addAction(tr("Solve"));
			connect(action, SIGNAL(triggered()), this, SLOT(SolveCalculate()));
			break;
		default:
			break;
		}
		pop_menu.exec(QCursor::pos());
	}
	void AircraftControlPanel::showDialog(QDialog* d)
	{
		int mx = _mainWindow->pos().x();
		int my = _mainWindow->pos().y();
		d->move(mx + 150, my + 150);
		d->exec();
	}

}