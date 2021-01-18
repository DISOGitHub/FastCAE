#include "ParaImportMeshSetup.h"
#include "ui_ParaImportMeshSetup.h"
#include "DataManager.h"
namespace FastCAEDesigner{
	ParaImportMeshSetup::ParaImportMeshSetup(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::ParaImportMeshSetup)
	{
		//ui->chk_binImport->setVisible(false);
		ui->setupUi(this);
		ui->chk_binImport->setVisible(false);
		connect(ui->chk_selectall, SIGNAL(clicked()), this, SLOT(OnChkAllClicked()));
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(OnBtnCancelClicked()));
		connect(ui->chk_cgns, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->chk_flunet, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->chk_gambit, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->chk_stl, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->chk_tecplot, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->chk_vtk, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		connect(ui->chk_inp, SIGNAL(clicked()), this, SLOT(CancelCheckAll()));
		//初始化导入网格类型
		QString importMeshSuffix = FastCAEDesigner::DataManager::getInstance()->GetImportMeshSuffix();
		
		initimportsuffix(importMeshSuffix);
	}

	ParaImportMeshSetup::~ParaImportMeshSetup()
	{
		delete ui;
	}
	void ParaImportMeshSetup::OnChkAllClicked()
	{
		if (ui->chk_selectall->isChecked())
		{
			ui->chk_cgns->setChecked(true);
			ui->chk_flunet->setChecked(true);
			ui->chk_gambit->setChecked(true);
			ui->chk_stl->setChecked(true);
			ui->chk_tecplot->setChecked(true);
			ui->chk_vtk->setChecked(true);
			ui->chk_inp->setChecked(true);
			ui->chk_cntm->setChecked(true);
			ui->chk_key->setChecked(true);
			ui->chk_su2->setChecked(true);
			ui->chk_pdb->setChecked(true);
		}
		else
		{
			ui->chk_cgns->setChecked(false);
			ui->chk_flunet->setChecked(false);
			ui->chk_gambit->setChecked(false);
			ui->chk_stl->setChecked(false);
			ui->chk_tecplot->setChecked(false);
			ui->chk_vtk->setChecked(false);
			ui->chk_inp->setChecked(false);
			ui->chk_cntm->setChecked(false);
			ui->chk_key->setChecked(false);
			ui->chk_su2->setChecked(false);
			ui->chk_pdb->setChecked(false);
		}
	}
	void ParaImportMeshSetup::OnBtnOkClicked()
	{
		QString str;
		QStringList sl;
		if (ui->chk_cgns->isChecked())
		{
			sl.append("cgns");
		}
		if (ui->chk_flunet->isChecked())
		{
			sl.append("msh");
		}
		if (ui->chk_gambit->isChecked())
		{
			sl.append("neu");
		}
		if (ui->chk_stl->isChecked())
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
		if (ui->chk_inp->isChecked())
		{
			sl.append("inp");
		}
		if (ui->chk_cntm->isChecked())
		{
			sl.append("cntm");
		}
		if (ui->chk_su2->isChecked())
		{
			sl.append("su2");
		}
		if (ui->chk_key->isChecked())
		{
			sl.append("key");
		}
		if (ui->chk_pdb->isChecked())
		{
			sl.append("pdb");
		}
		str = sl.join(";");
		FastCAEDesigner::DataManager::getInstance()->SetImportMesh(str);
		_sffix = str;
		this->accept();
		close();
	}
	void ParaImportMeshSetup::OnBtnCancelClicked()
	{
		close();
	}

	void ParaImportMeshSetup::initimportsuffix(QString s)
	{
		int a = 0;
		QStringList sl;
		sl = s.split(";");
		for (int i = 0; i < sl.count(); i++)
		{
			if (sl.at(i) == "cgns")
			{
				ui->chk_cgns->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "msh")
			{
				ui->chk_flunet->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "neu")
			{
				ui->chk_gambit->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "stl")
			{
				ui->chk_stl->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "dat")
			{
				ui->chk_tecplot->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "vtk")
			{
				ui->chk_vtk->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "inp")
			{
				ui->chk_inp->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "cntm")
			{
				ui->chk_cntm->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "su2")
			{
				ui->chk_su2->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "key")
			{
				ui->chk_key->setChecked(true);
				a = a + 1;
			}
			if (sl.at(i) == "pdb")
			{
				ui->chk_pdb->setChecked(true);
				a = a + 1;
			}
		}

		if (a == 11)
		{
			ui->chk_selectall->setChecked(true);
		}
	}

	void ParaImportMeshSetup::CancelCheckAll()
	{
		if ((ui->chk_cgns->isChecked()) && (ui->chk_flunet->isChecked()) && (ui->chk_gambit->isChecked()) &&
			(ui->chk_stl->isChecked()) && (ui->chk_tecplot->isChecked()) && (ui->chk_vtk->isChecked()) 
			&& (ui->chk_inp->isChecked()) && (ui->chk_cntm->isChecked()) && (ui->chk_su2->isChecked())
			&& (ui->chk_key->isChecked()) && (ui->chk_pdb->isChecked()))
		{
			ui->chk_selectall->setChecked(true);
		}
		else
		{
			ui->chk_selectall->setChecked(false);
		}
	}

	QString ParaImportMeshSetup::getSuffix()
	{
		return _sffix;
	}
}