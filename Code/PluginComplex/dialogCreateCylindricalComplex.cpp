#include "dialogCreateCylindricalComplex.h"
#include "ui_dialogCreateCylindricalComplex.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <QMessageBox>
#include "GeoCommandCreateCylindricalComplex.h"
#include "GeometryCommand/GeoCommandList.h"



namespace GeometryWidget
{
	CreateCylindricalComplexDialog::CreateCylindricalComplexDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
	GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateCylindricalComplex;
		_ui->setupUi(this);
		int id = Geometry::GeometrySet::getMaxID() + 1;
		_ui->namelineEdit->setPlaceholderText(QString("Connecting Rod_%1").arg(id));
		this->translateButtonBox(_ui->buttonBox);
	}



	CreateCylindricalComplexDialog::~CreateCylindricalComplexDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateCylindricalComplexDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}


		

	void CreateCylindricalComplexDialog::reject()
	{
		QDialog::reject();
		this->close();
	}
	

	void CreateCylindricalComplexDialog::accept()
	{
		bool ok = true;
		double loc[3]{0,0,0};
		double cy1r, cy1h, cor, coh,  cy2h;
		QString name = _ui->namelineEdit->text();
		if (name.isEmpty())
			name = _ui->namelineEdit->placeholderText();
		if (ok)
		{
			loc[0] = _ui->lineEdit_X->text().toDouble(&ok);
			if (!ok) ok= false;
			loc[1] = _ui->lineEdit_Y->text().toDouble(&ok);
			if (!ok) ok=false;
			loc[2] = _ui->lineEdit_Z->text().toDouble(&ok);
			if (!ok) ok=false;
		}
		if (ok)
		{
			cy1r = _ui->lineEdit_CY1R->text().toDouble(&ok);
			if (cy1r< 1e-6) ok = false;
			cy1h = _ui->lineEdit_CY1H->text().toDouble(&ok);
			if (cy1h< 1e-6) ok = false;
			cor = _ui->lineEdit_COR->text().toDouble(&ok);
			if (cor< 1e-6) ok = false;
			coh = _ui->lineEdit_COH->text().toDouble(&ok);
			if (coh< 1e-6) ok = false;
			cy2h = _ui->lineEdit_CY2H->text().toDouble(&ok);
			if (cy2h< 1e-6) ok = false;
		}
		if (cy1r == cor)
		{
			QMessageBox::warning(this, tr("Warning"), tr("The two radius are equal! !"));
			return;
		}
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		auto c = new Command::GeoCommandCreateCylindricalComplex(_mainWindow, _preWindow);
		c->setName(name);
		c->setLocation(loc);
		c->setRandH(cy1r,cy1h,cor,coh,cy2h);

		ok = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed !"));
			return;
		}


		QDialog::accept();
		this->close();





	}


}