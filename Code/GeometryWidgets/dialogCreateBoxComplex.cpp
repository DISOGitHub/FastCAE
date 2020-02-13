#include "dialogCreateBoxComplex.h"
#include "ui_dialogCreateBoxComplex.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <QMessageBox>
#include "GeometryCommand/GeoCommandCreateBoxComplex.h"
#include "GeometryCommand/GeoCommandList.h"


namespace GeometryWidget
{
	CreateBoxComplexDialog::CreateBoxComplexDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
	GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateBoxComplex;
		_ui->setupUi(this);
		int id = Geometry::GeometrySet::getMaxID() + 1;
		_ui->namelineEdit->setPlaceholderText(QString("BoxComplex_%1").arg(id));
	}



	CreateBoxComplexDialog::~CreateBoxComplexDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateBoxComplexDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}


		

	void CreateBoxComplexDialog::reject()
	{
		QDialog::reject();
		this->close();
	}
	

	void CreateBoxComplexDialog::accept()
	{
		bool ok = true;
		double loc[3]{0,0,0};
		double cy1r, cy1h,fr, box1_w, box2_w,box2_h,box3_w;
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
			fr = _ui->lineEdit_FR->text().toDouble(&ok);
			if (fr < 1e-6) ok = false;
			cy1h = _ui->lineEdit_CY1H->text().toDouble(&ok);
			if (cy1h< 1e-6) ok = false;
			box1_w = _ui->lineEdit_Box1W->text().toDouble(&ok);
			if (cy1h < 1e-6) ok = false;
			box2_w = _ui->lineEdit_Box2W->text().toDouble(&ok);
			if (cy1h < 1e-6) ok = false;
			box2_h = _ui->lineEdit_Box2H->text().toDouble(&ok);
			if (cy1h < 1e-6) ok = false;
			box3_w = _ui->lineEdit_Box3W->text().toDouble(&ok);
			if (cy1h < 1e-6) ok = false;
			
		}
		if (cy1r > box1_w) ok = false;
		if (cy1r < fr) ok = false;
		{
		}
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		auto c = new Command::GeoCommandCreateBoxComplex(_mainWindow, _preWindow);
		c->setName(name);
		c->setLocation(loc);
		c->setRandH(cy1r,cy1h,fr,box1_w,box2_w,box2_h,box3_w);

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