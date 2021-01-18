#include "ParaImportGeometrySetup.h"
#include "ui_ParaImportGeometrySetup.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GeometryConfig.h"
#include "DataManager.h"
#include <QDebug>
namespace FastCAEDesigner
{

	ParaImportGeometrySetup::ParaImportGeometrySetup(bool is,QWidget *parent) :
		QDialog(parent), _isImport(is),
		ui(new Ui::ParaImportGeometrySetup)
	{
		ui->setupUi(this);

		connect(ui->chkAll, SIGNAL(clicked()), this, SLOT(OnChkAllClicked()));
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(OnBtnCancelClicked()));
		connect(ui->checkBox_2, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->checkBox_3, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->checkBox_4, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->checkBox_5, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->checkBox_6, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));

		Init();
	}

	ParaImportGeometrySetup::~ParaImportGeometrySetup()
	{
		delete ui;
	}

	void ParaImportGeometrySetup::Init()
	{
		if (_isImport)
		{
			this->setWindowTitle(tr("Import Geometry"));

			QString importGeoSuffix = FastCAEDesigner::DataManager::getInstance()->GetImportGeometrySuffix();
			initImportAndExportSuffix(importGeoSuffix);
		}
		else
		{
			this->setWindowTitle(tr("Export Geometry"));

			QString exportGeoSuffix = FastCAEDesigner::DataManager::getInstance()->GetExportGeometrySuffix();
			initImportAndExportSuffix(exportGeoSuffix);
		}
		
	}

	void ParaImportGeometrySetup::OnBtnOkClicked()
	{
		//auto geoconfig = FastCAEDesigner::DataManager::getInstance()->getGeoConfig();
		//QString s = this->windowTitle();
		QString str;
		QStringList sl;
		if (ui->checkBox_2->isChecked())
		{
			sl.append("stp");
		}
		if (ui->checkBox_3->isChecked())
		{
			sl.append("step");
		}
		if (ui->checkBox_4->isChecked())
		{
			sl.append("igs");
		}
		if (ui->checkBox_5->isChecked())
		{
			sl.append("iges");
		}
		if (ui->checkBox_6->isChecked())
		{
			sl.append("brep");
		}
		str = sl.join(";");
		_suffix = str;
		if (_isImport)
		{
			FastCAEDesigner::DataManager::getInstance()->SetImportGeometry(str);
			this->accept();
		}
		else
		{
			FastCAEDesigner::DataManager::getInstance()->SetExportGeometry(str);
			this->accept();
		}
		//geoconfig->setSuffix(str);
		close();
	}

	void ParaImportGeometrySetup::OnBtnCancelClicked()
	{
		close();
	}

	void ParaImportGeometrySetup::OnChkAllClicked()
	{
		if (ui->chkAll->isChecked())
		{
			ui->checkBox_2->setChecked(true);
			ui->checkBox_3->setChecked(true);
			ui->checkBox_4->setChecked(true);
			ui->checkBox_5->setChecked(true);
			ui->checkBox_6->setChecked(true);
		}
		else
		{
			ui->checkBox_2->setChecked(false);
			ui->checkBox_3->setChecked(false);
			ui->checkBox_4->setChecked(false);
			ui->checkBox_5->setChecked(false);
			ui->checkBox_6->setChecked(false);
		}
	}
	void ParaImportGeometrySetup::initImportAndExportSuffix(QString s)
	{
		int a = 0;
		QStringList sl = s.split(";");
		for (int i = 0; i < sl.count(); i++)
		{
			if (sl.at(i) == "stp")
			{
				ui->checkBox_2->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "step")
			{
				ui->checkBox_3->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "igs")
			{
				ui->checkBox_4->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "iges")
			{
				ui->checkBox_5->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "brep")
			{
				ui->checkBox_6->setChecked(true);
				a = a + 1;
			}
		}

		if (a == 5)
		{
			ui->chkAll->setChecked(true);
		}
	}
	void ParaImportGeometrySetup::CancelCheckAll()
	{
		if ((ui->checkBox_2->isChecked()) && (ui->checkBox_3->isChecked()) && (ui->checkBox_4->isChecked())
			&& (ui->checkBox_2->isChecked()) && (ui->checkBox_6->isChecked()))
		{
			ui->chkAll->setChecked(true);
		}
		else
		{
			ui->chkAll->setChecked(false);
		}
	}


	QString ParaImportGeometrySetup::getSuffix()
	{
		return _suffix;
	}
}