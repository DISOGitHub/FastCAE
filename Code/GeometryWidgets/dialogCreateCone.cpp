#include "dialogCreateCone.h"
#include "ui_dialogCreateCone.h"
#include "geometry/geometrySet.h"
#include "geoPointWidget.h"
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandCreateCone.h"
#include "geometry/geometryParaCone.h"
#include <QMessageBox>
#include "python/PyAgent.h"


namespace GeometryWidget
{
	CreateConeDialog::CreateConeDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateCone;
		_ui->setupUi(this);
		init();
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));

	}
	CreateConeDialog::CreateConeDialog(GUI::MainWindow*m, MainWidget::PreWindow*p, Geometry::GeometrySet*set) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateCone;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit Cone");
		init();
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
	}

	CreateConeDialog::~CreateConeDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateConeDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateConeDialog::init()
	{
		_pw = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->verticalLayout->addWidget(_pw);
		on_radioButtonUser();
		this->translateButtonBox(_ui->buttonBox);

		if (!_isEdit)
		{
			int id = Geometry::GeometrySet::getMaxID() + 1;
			_ui->namelineEdit->setPlaceholderText(QString("Cone_%1").arg(id));
		}
		else
		{
			if (_editSet == nullptr) return;
			_ui->namelineEdit->setText(_editSet->getName());
			_ui->namelineEdit->setEnabled(false);
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaCone* p = dynamic_cast<Geometry::GeometryParaCone*>(pb);
			if (p == nullptr) return;
			double loc[3] = { 0.0 }, radius1 = { 0.0 }, radius2 = {0.0},length = { 0.0 }, dir[3] = { 0.0 };
			p->getLocation(loc);
			p->getRadius1(&radius1);
			p->getRadius2(&radius2);
			p->getLength(&length);
			p->getDirection(dir);
			_pw->setCoordinate(loc);

			_ui->doubleSpinBoxR->setValue(radius1);
			_ui->doubleSpinBoxR2->setValue(radius2);
			_ui->doubleSpinBoxL->setValue(length);

			if (dir[0] != 0 && dir[1] == 0 && dir[2] == 0)
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

	void CreateConeDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	void CreateConeDialog::accept()
	{
		double corner[3] = { 0.0 };
		bool ok = _pw->getCoordinate(corner);

		QString name = _ui->namelineEdit->text();
		if (name.isEmpty())
			name = _ui->namelineEdit->placeholderText();
		if (ok)
			ok = !name.isEmpty();
		double r = _ui->doubleSpinBoxR->value();
		double r2 = _ui->doubleSpinBoxR2->value();
		if (fabs(r - r2) < 1e-6)
		{
			QMessageBox::warning(this, tr("Warning"), tr("The two radii are equal!"));
			return;
		}

		double l = _ui->doubleSpinBoxL->value();
		if (ok && l < 1e-6)
			ok = false;

		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

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
	


		/*Command::GeoCommandCreateCone* command = new Command::GeoCommandCreateCone(_mainWindow, _preWindow);
		command->setLocation(corner);
		command->setAxis(dir);
		command->setRadius(r, r2);
		command->setLength(l);
		command->setName(name);
		if (_isEdit) command->setEditData(_editSet);
		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}*/

		QStringList codes{};
		codes += QString("cone = CAD.Cone()");
		if (_isEdit)
			codes += QString("cone.setEditID(%1)").arg(_editSet->getID());
		else
			codes += QString("cone.setName('%1')").arg(name);
			codes += QString("cone.setLocation(%1,%2,%3)").arg(corner[0]).arg(corner[1]).arg(corner[2]);
			codes += QString("cone.setRadius(%1,%2)").arg(r).arg(r2);
			codes += QString("cone.setLength(%1)").arg(l);
			codes += QString("cone.setAxis(%1,%2,%3)").arg(dir[0]).arg(dir[1]).arg(dir[2]);
		if (_isEdit)
			codes += QString("cone.edit()");
		else
			codes += QString("cone.create()");
		_pyAgent->submit(codes);

		QDialog::accept();
		this->close();
	}

	void CreateConeDialog::on_radioButtonUser()
	{
		bool checked = _ui->radioButtonUser->isChecked();
		_ui->doubleSpinBoxX->setVisible(checked);
		_ui->doubleSpinBoxY->setVisible(checked);
		_ui->doubleSpinBoxZ->setVisible(checked);
	}

}