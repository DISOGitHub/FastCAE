#include "ParaExportMeshSetup.h"
#include "ui_ParaExportMeshSetup.h"
#include "DataManager.h"
namespace FastCAEDesigner{
	ParaExportMeshSetup::ParaExportMeshSetup(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::ParaExportMeshSetup)
	{
		//ui->BinaryExport->setVisible(false);
		ui->setupUi(this);
		ui->BinaryExport->setVisible(false);
		ui->chk_nastran->setEnabled(false);
		ui->chk_flunet->setEnabled(false);
		//ui->chk_gambit->setEnabled(false);
		ui->chk_tecplot->setEnabled(false);
		connect(ui->chk_selectall, SIGNAL(clicked()), this, SLOT(OnChkAllClicked()));
		connect(ui->btnOk_2, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel_2, SIGNAL(clicked()), this, SLOT(OnBtnCancelClicked()));
		//connect(ui->chk_flunet, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->chk_gambit, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		//connect(ui->chk_nastran, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		//connect(ui->chk_tecplot, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->chk_vtk, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		//初始化到处网格类型
		_suffix = FastCAEDesigner::DataManager::getInstance()->GetExportMeshSuffix();
		initExportSuffix(_suffix);
	}

	ParaExportMeshSetup::~ParaExportMeshSetup()
	{
		delete ui;
	}
	void ParaExportMeshSetup::OnChkAllClicked()
	{
		if (ui->chk_selectall->isChecked())
		{
			//ui->chk_nastran->setChecked(true);	
			ui->chk_gambit->setChecked(true);
			//ui->chk_flunet->setChecked(true);
			///ui->chk_stl->setChecked(true);
			//ui->chk_tecplot->setChecked(true);
			ui->chk_vtk->setChecked(true);
		}
		else
		{
			//ui->chk_nastran->setChecked(false);
			//ui->chk_flunet->setChecked(false);
			ui->chk_gambit->setChecked(false);
			//ui->chk_flunet->setChecked(false);
			//ui->chk_tecplot->setChecked(false);
			ui->chk_vtk->setChecked(false);
		}
	}
	void ParaExportMeshSetup::OnBtnOkClicked()
	{
		QStringList sl;
		QString str;
		if (ui->chk_gambit->isChecked())
		{
			sl.append("neu");
		}
		if (ui->chk_vtk->isChecked())
		{
			sl.append("vtk");
		}
	/*	QStringList sl;
		if (ui->chk_nastran->isChecked())
		{
			sl.append("cgns");
		}
		if (ui->chk_gambit->isChecked())
		{
			sl.append("neu");
		}
		if (ui->chk_flunet->isChecked())
		{
			sl.append("stl");
		}
		if (ui->chk_tecplot->isChecked())
		{
			sl.append("dat");
		}
		if (ui->chk_vtk->isChecked())
		{
			sl.append("vtk");
		}
		str = sl.join(";");*/
		str = sl.join(";");
		_suffix = str;
		FastCAEDesigner::DataManager::getInstance()->SetExportMesh(str);
		this->accept();
		close();
	}
	void ParaExportMeshSetup::OnBtnCancelClicked()
	{
		close();
	}

	void ParaExportMeshSetup::initExportSuffix(QString s)
	{
		int a = 0;
		QStringList sl = s.split(";");
		for (int i = 0; i < sl.count(); i++)
		{
			if (sl.at(i) == "neu")
			{
				ui->chk_gambit->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "vtk")
			{
				ui->chk_vtk->setChecked(true);
				a = a + 1;
			}
			/*
			if (sl.at(i) == "cgns")
			{
				ui->chk_nastran->setChecked(true);
			}
			
			if (sl.at(i) == "stl")
			{
				ui->chk_flunet->setChecked(true);
			}
			if (sl.at(i) == "dat")
			{
				ui->chk_tecplot->setChecked(true);
			}*/
			if (a == 2)
			{
				ui->chk_selectall->setChecked(true);
			}
		}
	}

	void ParaExportMeshSetup::CancelCheckAll()
	{
		if ((ui->chk_gambit->isChecked()) && (ui->chk_vtk->isChecked()))
		{
			ui->chk_selectall->setChecked(true);
		}
		else
		{
			ui->chk_selectall->setChecked(false);
		}
	}

	QString ParaExportMeshSetup::getExportSuffix()
	{
		return _suffix;
	}
}