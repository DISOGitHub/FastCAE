#include "dialogCreateBox.h"
#include "ui_dialogCreateBox.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include "geometry/geometryParaBox.h"
#include "geometry/geometryModelParaBase.h"
#include "geoPointWidget.h"
#include "GeometryCommand/GeoCommandCreateBox.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include "python/PyAgent.h"


namespace GeometryWidget
{
	CreateBoxDialog::CreateBoxDialog(GUI::MainWindow* m, MainWidget::PreWindow* p):
		GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateBox;
		_ui->setupUi(this);
		init();
	}

	CreateBoxDialog::CreateBoxDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set):
		GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateBox;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle(tr("Edit Box"));
		init();

	}

	CreateBoxDialog::~CreateBoxDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateBoxDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateBoxDialog::init()
	{
		_pw = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->verticalLayout->addWidget(_pw);

		this->translateButtonBox(_ui->buttonBox);

		if (!_isEdit)
		{
			int id = Geometry::GeometrySet::getMaxID() + 1;
			_ui->namelineEdit->setPlaceholderText(QString("Box_%1").arg(id));
		}
		else
		{
			if (_editSet == nullptr) return;
			_ui->namelineEdit->setText(_editSet->getName());
			_ui->namelineEdit->setEnabled(false);
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaBox* p = dynamic_cast<Geometry::GeometryParaBox*>(pb);
			if (p == nullptr) return;
			double loc[3] = { 0.0 }, para[3] = { 0.0 };
			p->getLocation(loc);
			p->getGeoPara(para);
			_pw->setCoordinate(loc);

			_ui->lineEditL->setText(QString::number(para[0]));
			_ui->lineEditW->setText(QString::number(para[1]));
			_ui->lineEditH->setText(QString::number(para[2]));
		}

		
	}

	void CreateBoxDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	bool CreateBoxDialog::getPara(double* coor)
	{
		bool ok = false;

		QString text = _ui->lineEditL->text();
		coor[0] = text.toDouble(&ok);
		if (!ok) return false;

		text = _ui->lineEditW->text();
		coor[1] = text.toDouble(&ok);
		if (!ok) return false;

		text = _ui->lineEditH->text();
		coor[2] = text.toDouble(&ok);
		if (!ok) return false;

		return true;
	}

	void CreateBoxDialog::accept()
	{
		double corner[3] = { 0.0 };
		double para[3] = { 0.0 };
		bool ok = _pw->getCoordinate(corner);

		if (ok)
			ok = getPara(para);
		
		QString name = _ui->namelineEdit->text();
		if (name.isEmpty())
			name = _ui->namelineEdit->placeholderText();
		if (ok)
			ok = !name.isEmpty();

		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		
		QStringList codes{};
		codes += QString("box = CAD.Box()");
		if (_isEdit)
			codes += QString("box.setEditID(%1)").arg(_editSet->getID());
		else
			codes += QString("box.setName('%1')").arg(name);
		codes += QString("box.setLocation(%1,%2,%3)").arg(corner[0]).arg(corner[1]).arg(corner[2]);
		codes += QString("box.setPara(%1,%2,%3)").arg(para[0]).arg(para[1]).arg(para[2]); 
		
		if (_isEdit)
			codes += QString("box.edit()");
		else
			codes += QString("box.create()");
	
		_pyAgent->submit(codes);
		
// 
// 		Command::CommandCreateBox* c = new Command::CommandCreateBox(_mainWindow, _preWindow);
// 		c->setName(name);
// 		c->setLocation(corner);
// 		c->setGeoPara(para);
// 
// 		if (_isEdit) c->setEditData(_editSet);
// 
// 		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
// 		if (!success)
// 		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Create failed !"));
// 			return;
// 		}


		QDialog::accept();
		this->close();
	}


}