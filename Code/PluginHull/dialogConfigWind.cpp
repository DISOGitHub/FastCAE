#include "dialogConfigWind.h"
#include "ui_dialogConfigWind.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <QMessageBox>
#include "GeoCommandMakeHullCut.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QTreeWidget>
#include "mainWindow/mainWindow.h"
#include <QDebug>
#include "DataProperty/modelTreeItemType.h"
#include <QPicture>
#include <qfile.h>

namespace PluginShip
{
	ConfigWindDialog::ConfigWindDialog(GUI::MainWindow *m) 
	{
		_ui = new Ui::ConfigWindDialog;
		_ui->setupUi(this);

		_ui->picLabel->setPixmap(QPixmap(":/qrc/current_profile.png"));
		_ui->picLabel->setAutoFillBackground(true);

		connect(_ui->windcomponentTableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(OnTableFreqItemClicked(QModelIndex)));
	}

	void ConfigWindDialog::OnTableFreqItemClicked(QModelIndex modelIndex)
	{
		int cur_row = _ui->windcomponentTableWidget->currentRow();
		if (cur_row < 0)
			return;
		if (_ui->windcomponentTableWidget->item(cur_row, 0) != 0)
		{
			_ui->windareaLineEdit->setText(_ui->windcomponentTableWidget->item(cur_row, 0)->text());
		}
		if (_ui->windcomponentTableWidget->item(cur_row, 1) != 0)
		{
			_ui->windfactorLineEdit->setText(_ui->windcomponentTableWidget->item(cur_row, 1)->text());
		}
		if (_ui->windcomponentTableWidget->item(cur_row, 2) != 0)
		{
			_ui->windcoordinatesxLineEdit->setText(_ui->windcomponentTableWidget->item(cur_row, 2)->text());
		}

		if (_ui->windcomponentTableWidget->item(cur_row, 3) != 0)
		{
			_ui->windcoordinatesyLineEdit->setText(_ui->windcomponentTableWidget->item(cur_row, 3)->text());
		}

		if (_ui->windcomponentTableWidget->item(cur_row, 4) != 0)
		{
			_ui->windcoordinateszLineEdit->setText(_ui->windcomponentTableWidget->item(cur_row, 4)->text());
		}
		if (_ui->windcomponentTableWidget->item(cur_row, 5) != 0)
		{
			_ui->windvectorxLineEdit->setText(_ui->windcomponentTableWidget->item(cur_row, 5)->text());
		}
		if (_ui->windcomponentTableWidget->item(cur_row, 6) != 0)
		{
			_ui->windvectoryLineEdit->setText(_ui->windcomponentTableWidget->item(cur_row, 6)->text());
		}
		if (_ui->windcomponentTableWidget->item(cur_row, 7) != 0)
		{
			_ui->windvectorzLineEdit->setText(_ui->windcomponentTableWidget->item(cur_row, 7)->text());
		}


	}
	
	ConfigWindDialog::~ConfigWindDialog()
	{
		
	}
}