#include "dialogCreateSphere.h"
#include "ui_dialogCreateSphere.h"
#include "geoPointWidget.h"
#include "geometry/geometrySet.h"
#include "GeometryCommand/GeoCommandCreateSphere.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QMessageBox>
#include "geometry/geometryParaSphere.h"
#include "python/PyAgent.h"

namespace GeometryWidget
{
	CreateSphereDialog::CreateSphereDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateSphere;
		_ui->setupUi(this);
		init();
	}

	CreateSphereDialog::CreateSphereDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateSphere;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit Sphere");
		init();
	}

	CreateSphereDialog::~CreateSphereDialog()
	{
		if (_ui != nullptr)
			delete _ui;
	}

	void CreateSphereDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateSphereDialog::init()
	{
		_pw = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->verticalLayout->addWidget(_pw);
		this->translateButtonBox(_ui->buttonBox);

		if (!_isEdit)
		{
			int id = Geometry::GeometrySet::getMaxID() + 1;
			_ui->namelineEdit->setPlaceholderText(QString("Sphere_%1").arg(id));
		}
		else
		{
			if (_editSet == nullptr) return;
			_ui->namelineEdit->setText(_editSet->getName());
			_ui->namelineEdit->setEnabled(false);
			Geometry::GeometryModelParaBase* pb = _editSet->getParameter();
			Geometry::GeometryParaSphere* p = dynamic_cast<Geometry::GeometryParaSphere*>(pb);
			if (p == nullptr) return;
			double loc[3] = { 0.0 };
			p->getLocation(loc);
			_ui->doubleSpinBoxR->setValue(p->getRadius());
			_pw->setCoordinate(loc);

		}

	}

	void CreateSphereDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	void CreateSphereDialog::accept()
	{
		double corner[3] = { 0.0 };
		bool ok = _pw->getCoordinate(corner);

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

		double r = _ui->doubleSpinBoxR->value();
/*

		Command::GeoCommandCreateSphere* c = new Command::GeoCommandCreateSphere(_mainWindow, _preWindow);
		c->setLocation(corner);
		c->setName(name);
		c->setRadius(r);
		if (_isEdit) c->setEditData(_editSet);
		Command::GeoComandList::getInstance()->executeCommand(c);
*/
		QStringList codes{};
		codes += QString("sphere = CAD.Sphere()");
		if (_isEdit)
			codes += QString("sphere.setEditID(%1)").arg(_editSet->getID());
		else
			codes += QString("sphere.setName('%1')").arg(name);
		codes += QString("sphere.setLocation(%1,%2,%3)").arg(corner[0]).arg(corner[1]).arg(corner[2]);
		codes += QString("sphere.setRadius(%1)").arg(r);
		if (_isEdit)
			codes += QString("sphere.edit()");
		else
			codes += QString("sphere.create()");

		_pyAgent->submit(codes);
		QDialog::accept();
		this->close();
	}


}
