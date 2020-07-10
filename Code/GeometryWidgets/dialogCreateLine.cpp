#include "dialogCreateLine.h"
#include "ui_dialogCreateLine.h"
#include "geoPointWidget.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include "GeometryCommand/GeoCommandCreateLine.h"
#include "GeometryCommand/GeoCommandList.h"
#include "geometry/geometryParaLine.h"
#include <QMessageBox>
#include <math.h>
#include "python/PyAgent.h"

namespace GeometryWidget
{
	CreateLineDialog::CreateLineDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateLineDialog;
		_ui->setupUi(this);
		init();
		connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(optionChanged(int)));
		connect(_startPoint, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointButtomClicked(GeoPointWidget*)));
		connect(_endPoint, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointButtomClicked(GeoPointWidget*)));
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		
	}

	CreateLineDialog::CreateLineDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateLineDialog;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit Line");
		init();
		connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(optionChanged(int)));
		connect(_startPoint, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointButtomClicked(GeoPointWidget*)));
		connect(_endPoint, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointButtomClicked(GeoPointWidget*)));
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));

	}

	CreateLineDialog::~CreateLineDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateLineDialog::init()
	{
		on_radioButtonUser();
		this->translateButtonBox(_ui->buttonBox);

		_startPoint = new GeoPointWidget(_mainWindow, _preWindow);
		_endPoint = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->verticalLayout->addWidget(_startPoint);
		_ui->ptlayout->addWidget(_endPoint);
		_ui->tabWidget->tabBar()->hide();
		if (!_isEdit)
		{
			int id = Geometry::GeometrySet::getMaxID() + 1;
			_ui->namelineEdit->setPlaceholderText(QString("Line_%1").arg(id));
		}
		else
		{
			if (_editSet == nullptr) return;
			emit hideGeometry(_editSet);
			_ui->namelineEdit->setText(_editSet->getName());
			_ui->namelineEdit->setEnabled(false);
			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaLine* p = dynamic_cast<Geometry::GeometryParaLine*>(bp);
			if (p == nullptr) return;

			int index{};
			double startpt[3]{0.0},endpt[3]{0.0};
			p->getStartPoint(startpt);
			p->getCoor(endpt);
			index=p->getOptionIndex();
			_startPoint->setCoordinate(startpt);
			_endPoint->setCoordinate(endpt);
			_ui->comboBoxOption->setCurrentIndex(index);
			_ui->tabWidget->setCurrentIndex(index);
	
			double length, dir[3]; bool reverse{false};
			length = p->getLength();
			p->getDirection(dir);
			_ui->lineEditLength->setText(QString::number(length));
			_ui->checkBoxReverse->setChecked(reverse);
			
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
	
	void CreateLineDialog::optionChanged(int index)
	{
		_ui->tabWidget->setCurrentIndex(index);
		if (index == 1)
		{
			emit setSelectMode(int(ModuleBase::None));
			_endPoint->handleProcess(false);
		}
		
	}

	void CreateLineDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateLineDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet != nullptr)
				emit showGeometry(_editSet);
		}
		QDialog::reject();
		this->close();
	}

	void CreateLineDialog::accept()
	{  
		bool ok = false;
		double startPoint[3] = { 0.0 };
		double endPoint[3] = { 0.0 };
		QString name = _ui->namelineEdit->text();
		ok = _startPoint->getCoordinate(startPoint);
		if (ok)
			ok = getEndPoint(endPoint);
		if (ok)
		{
			if (name.isEmpty())
				name = _ui->namelineEdit->placeholderText();
			ok = !name.isEmpty();
		}
		if (ok)
		{
			double dx = endPoint[0] - startPoint[0];
			double dy = endPoint[1] - startPoint[1];
			double dz = endPoint[2] - startPoint[2];

			double m = sqrt(dx*dx + dy* dy + dz* dz);
			if (m < 1e-6)  ok = false;
		}
		int index{ 0 };
		double coor[3]{0.0};
		double len;
		double dir[3] = { 0.0 };
		bool reverse{ false };
		if (ok)
		{
			index = _ui->comboBoxOption->currentIndex();
			
			ok=_endPoint->getCoordinate(coor);
			len = _ui->lineEditLength->text().toDouble(&ok);
			
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
			reverse = _ui->checkBoxReverse->isChecked();
		}

		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		QStringList codes{};
		codes += QString("line = CAD.Line()");
		if (_isEdit)
			codes += QString("line.setEditID(%1)").arg(_editSet->getID());
		else
			codes += QString("line.setName('%1')").arg(name);
		codes += QString("line.setStartPoint(%1,%2,%3)").arg(startPoint[0]).arg(startPoint[1]).arg(startPoint[2]);

		if (index==0)
			codes += QString("line.setEndPoint(%1,%2,%3)").arg(coor[0]).arg(coor[1]).arg(coor[2]);
		else
		{
			codes += QString("line.setLength(%1)").arg(len);
			codes += QString("line.setDirection(%1,%2,%3)").arg(dir[0]).arg(dir[1]).arg(dir[2]);
			codes += QString("line.setDirectionReverse(%1)").arg(reverse);
		}
		if (_isEdit)
			codes += QString("line.edit()");
		else
			codes += QString("line.create()");
		_pyAgent->submit(codes);
/*
		auto c = new Command::GeoCommandCreateLine(_mainWindow, _preWindow);
		c->setName(name);
		c->setPoints(startPoint, endPoint);
		c->setOptionIndex(index);
		c->setCoor(coor);
		c->setLength(len);
		c->setDir(dir);
		c->setReverse(reverse);
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

	void CreateLineDialog::pointButtomClicked(GeoPointWidget* w)
	{
		_startPoint->handleProcess(false);
		_endPoint->handleProcess(false);
		w->handleProcess(true);
	}

	void CreateLineDialog::on_radioButtonUser()
	{
		bool checked = _ui->radioButtonUser->isChecked();
		_ui->doubleSpinBoxX->setVisible(checked);
		_ui->doubleSpinBoxY->setVisible(checked);
		_ui->doubleSpinBoxZ->setVisible(checked);
	}


	bool CreateLineDialog::getEndPoint(double* c)
	{
		const int index = _ui->comboBoxOption->currentIndex();
		double endpt[3]{ 0.0 };
		bool ok = false;

		switch (index)
		{
		case 0:
			ok = _endPoint->getCoordinate(endpt); break;
		case 1:
			ok = this->getEndPointByDirection(endpt); break;
		default: break;
		}

		for (int i = 0; i < 3; ++i)
			c[i] = endpt[i];
		return ok; 

	}

	bool CreateLineDialog::getEndPointByDirection(double* ept)
	{
		QString text = _ui->lineEditLength->text();
		double length = text.toDouble();
		if (length < 1e-6) return false;
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

		if (_ui->checkBoxReverse->isChecked())
		{
			for (int i = 0; i < 3; ++i)
				dir[i] *= -1;
		}
		double mod = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
		double fac = length / mod;

		double ori[3] {0.0};
		bool ok = _startPoint->getCoordinate(ori);
		if (ok)
			for (int i = 0; i < 3; ++i)
				ept[i] = ori[i] + fac * dir[i];
		return ok;
	}

}