#include "dialogCreateCylinder.h"
#include "ui_dialogCreateCylinder.h"
#include "geometry/geometrySet.h"
#include "geoPointWidget.h"
#include "geometry/geometryModelParaBase.h"
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandCreateCylinder.h"
#include "geometry/geometryParaCylinder.h"
#include <QMessageBox>
#include <QDebug>
#include "python/PyAgent.h"

namespace GeometryWidget
{
	CreateCylinderDialog::CreateCylinderDialog(GUI::MainWindow* m, MainWidget::PreWindow* p):
		GeoDialogBase(m,p)
	{
		_ui = new Ui::CreateCylinder;
		_ui->setupUi(this);
		init();
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
	}

	CreateCylinderDialog::CreateCylinderDialog(GUI::MainWindow*m, MainWidget::PreWindow*p, Geometry::GeometrySet*set):
		GeoDialogBase(m,p)
	{
		_ui = new Ui::CreateCylinder;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit Cylinder");
		init();
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
	}

	CreateCylinderDialog::~CreateCylinderDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateCylinderDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateCylinderDialog::init()
	{	
		_pw = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->verticalLayout->addWidget(_pw);
		on_radioButtonUser();
		this->translateButtonBox(_ui->buttonBox);

		if (!_isEdit)
		{
			int id = Geometry::GeometrySet::getMaxID() + 1;
			_ui->namelineEdit->setPlaceholderText(QString("Cylinder_%1").arg(id));
		}
		else
		{
			if (_editSet == nullptr) return;
			_ui->namelineEdit->setText(_editSet->getName());
			_ui->namelineEdit->setEnabled(false);
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaCylinder* p = dynamic_cast<Geometry::GeometryParaCylinder*>(pb);
			if (p == nullptr) return;
			double loc[3] = { 0.0 }, radius = { 0.0 }, length = {0.0}, dir[3] = { 0.0 };
			p->getLocation(loc);
			p->getRadius(&radius);
			p->getLength(&length);
			p->getDirection(dir);
			_pw->setCoordinate(loc);

			_ui->doubleSpinBoxR->setValue(radius);
			_ui->doubleSpinBoxL->setValue(length);

			if (dir[0]!=0&&dir[1]==0&&dir[2]==0)
			{
				_ui->radioButtonX->setChecked(true);
			}
			else if (dir[0] == 0 && dir[1] != 0 && dir[2] == 0)
			{
				_ui->radioButtonY->setChecked(true);
			}
			else if (dir[0] == 0 && dir[1] == 0 && dir[2] != 0)
			{
				_ui->radioButtonZ->setChecked(true);
			}
			else
			{
				_ui->radioButtonUser->setChecked(true);
				_ui->doubleSpinBoxX->setVisible(true);
				_ui->doubleSpinBoxY->setVisible(true);

				_ui->doubleSpinBoxZ->setVisible(true);
				_ui->doubleSpinBoxX->setValue(dir[0]);
				_ui->doubleSpinBoxY->setValue(dir[1]);
				_ui->doubleSpinBoxZ->setValue(dir[2]);
			}
		}
	}

	void CreateCylinderDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	void CreateCylinderDialog::accept()
	{
		double corner[3] = { 0.0 };
		bool ok = _pw->getCoordinate(corner);
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		double r = _ui->doubleSpinBoxR->value();
		double l = _ui->doubleSpinBoxL->value();

		double dir[3] = { 0.0 };
		if (_ui->radioButtonX->isChecked())
			dir[0] = 1.0;
		else if (_ui->radioButtonY->isChecked())
			dir[1] = 1.0;
		else if (_ui->radioButtonZ->isChecked())
			dir[2] = 1.0;
		else
		{
			dir[0] = _ui->doubleSpinBoxX->value();
			dir[1] = _ui->doubleSpinBoxY->value();
			dir[2] = _ui->doubleSpinBoxZ->value();
		}
		QString name = _ui->namelineEdit->text();
		if (name.isEmpty())
			name = _ui->namelineEdit->placeholderText();

// 		Command::GeoCommandCreateCylinder* command = new Command::GeoCommandCreateCylinder(_mainWindow, _preWindow);
// 		command->setLocation(corner);
// 		command->setAxis(dir);
// 		command->setRadius(r);
// 		command->setLength(l);
// 		command->setName(name);
// 		if (_isEdit) command->setEditData(_editSet);
// 		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
// 		if (!success)
// 		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
// 			return;
// 		}
		QStringList codes{};
		codes += QString("cylinder = CAD.Cylinder()");
		if (_isEdit)
			codes += QString("cylinder.setEditID(%1)").arg(_editSet->getID());
		else
			codes += QString("cylinder.setName('%1')").arg(name);
			codes += QString("cylinder.setLocation(%1,%2,%3)").arg(corner[0]).arg(corner[1]).arg(corner[2]);
			codes += QString("cylinder.setRadius(%1)").arg(r);
			codes += QString("cylinder.setLength(%1)").arg(l);
			codes += QString("cylinder.setAxis(%1,%2,%3)").arg(dir[0]).arg(dir[1]).arg(dir[2]);
		if (_isEdit)
			codes += QString("cylinder.edit()");
		else
			codes += QString("cylinder.create()");

		_pyAgent->submit(codes);

		QDialog::accept();
		this->close();
	}

	void CreateCylinderDialog::on_radioButtonUser()
	{
		
		bool checked = _ui->radioButtonUser->isChecked();
		_ui->doubleSpinBoxX->setVisible(checked);
		_ui->doubleSpinBoxY->setVisible(checked);
		_ui->doubleSpinBoxZ->setVisible(checked);
	}

}

