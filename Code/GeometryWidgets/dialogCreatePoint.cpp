#include "dialogCreatePoint.h"
#include "ui_dialogCreatePoint.h"
#include "geoPointWidget.h"
#include "geometry/geometrySet.h"
#include <QMessageBox>
#include "GeometryCommand/GeoCommandCreatePoint.h"
#include "GeometryCommand/GeoCommandList.h"
#include "geometry/geometryParaPoint.h"
#include "python/PyAgent.h"

namespace GeometryWidget
{
	CreatePointDialog::CreatePointDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::CreatePoint;
		_ui->setupUi(this);
		this->init();
	}

	CreatePointDialog::CreatePointDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::CreatePoint;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit Point");
		init();
	}

	CreatePointDialog::~CreatePointDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreatePointDialog::init()
	{
	
		_pw = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->verticalLayout->addWidget(_pw);
		this->translateButtonBox(_ui->buttonBox);

		if (!_isEdit)
		{
			int id = Geometry::GeometrySet::getMaxID() + 1;
			_ui->namelineEdit->setPlaceholderText(QString("Point_%1").arg(id));
		}
		else
		{
			if (_editSet == nullptr) return;
			_ui->namelineEdit->setText(_editSet->getName());
			_ui->namelineEdit->setEnabled(false);
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaPoint* p = dynamic_cast<Geometry::GeometryParaPoint*>(pb);
			if (p == nullptr) return;
			double para[3] = { 0.0 }, corner[3] = {0.0};
			p->getPara(para);
			p->getCorner(corner);
			_pw->setCoordinate(corner);
			_ui->lineEditX->setText(QString::number(para[0]));
			_ui->lineEditY->setText(QString::number(para[1]));
			_ui->lineEditZ->setText(QString::number(para[2]));

		}
	}

	void CreatePointDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreatePointDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	bool CreatePointDialog::getPara(double* coor)
	{
		bool ok = false;

		QString text = _ui->lineEditX->text();
		coor[0] = text.toDouble(&ok);
		if (!ok) return false;

		text = _ui->lineEditY->text();
		coor[1] = text.toDouble(&ok);
		if (!ok) return false;

		text = _ui->lineEditZ->text();
		coor[2] = text.toDouble(&ok);
		if (!ok) return false;

		return true;
	}

	void CreatePointDialog::accept()
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
		double coor[3] = { 0.0 };
		for (int i = 0; i < 3; ++i)
			coor[i] = corner[i] + para[i];

		QStringList codes{};
		codes += QString("point = CAD.Point()");
		if (_isEdit)
			codes += QString("point.setEditID(%1)").arg(_editSet->getID());
		else
			codes += QString("point.setName('%1')").arg(name);
		codes += QString("point.setLocation(%1,%2,%3)").arg(corner[0]).arg(corner[1]).arg(corner[2]);
		codes += QString("point.setOffset(%1,%2,%3)").arg(para[0]).arg(para[1]).arg(para[2]);

		if (_isEdit)
			codes += QString("point.edit()");
		else
			codes += QString("point.create()");
		_pyAgent->submit(codes);


		/*
		auto c = new Command::GeoCommandCreatePoint(_mainWindow, _preWindow);
		c->setCorner(corner);
		c->setPara(para);
		c->setName(name);
		c->setCoordinate(coor);
		if (_isEdit) c->setEditData(_editSet);
		ok = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed !"));
			return;
		}*/

		QDialog::accept();
		this->close();
	
	}

}
