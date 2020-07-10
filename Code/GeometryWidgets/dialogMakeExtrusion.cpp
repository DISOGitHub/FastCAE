#include "dialogMakeExtrusion.h"
#include "ui_dialogMakeExtrusion.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandMakeExtrusion.h"
#include <QMessageBox>
#include <QDebug>
#include <QColor>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include "geometry/geometrySet.h"
#include "geometry/geometryParaExtrusion.h"
#include "python/PyAgent.h"


namespace GeometryWidget
{
	
	CreateExtrusionDialog::CreateExtrusionDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateExtrusion;
		_ui->setupUi(this);
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		this->translateButtonBox(_ui->buttonBox);
		on_radioButtonUser();
	}

	CreateExtrusionDialog::CreateExtrusionDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateExtrusion;
		_ui->setupUi(this);
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		this->translateButtonBox(_ui->buttonBox);
		on_radioButtonUser();
		_isEdit = true;
		_editSet = set;
		init();
	}

	CreateExtrusionDialog::~CreateExtrusionDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit setSelectMode((int)ModuleBase::None);
		emit updateGraphOptions();
	}

	void CreateExtrusionDialog::shapeSlected(Geometry::GeometrySet* set,  int index)
	{
		if (_shapeHash.contains(set, index))
		{
			emit highLightGeometryEdgeSig(set, index, false);
			_shapeHash.remove(set, index);
		}
		else
		{
			_shapeHash.insert(set, index);
			emit highLightGeometryEdgeSig(set, index,true);
		}

		QString label = QString(tr("Selected edge(%1)")).arg(_shapeHash.size());
		_ui->edglabel->setText(label);

	}

	void CreateExtrusionDialog::on_geoSelectCurve_clicked()
	{
		emit setSelectMode(int(ModuleBase::GeometryCurve));

		for (QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _shapeHash.begin(); iter != _shapeHash.end(); ++iter)		{			emit highLightGeometryEdgeSig(iter.key(), iter.value(), true);		}
	}

	void CreateExtrusionDialog::on_radioButtonUser()
	{
		bool checked = _ui->radioButtonUser->isChecked();
		_ui->doubleSpinBoxX->setVisible(checked);
		_ui->doubleSpinBoxY->setVisible(checked);
		_ui->doubleSpinBoxZ->setVisible(checked);
	}

	void CreateExtrusionDialog::init()
	{
		if (_editSet == nullptr) return;
		Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
		Geometry::GeometryParaExtrusion* p = dynamic_cast<Geometry::GeometryParaExtrusion*>(bp);
		if (p == nullptr) return;
		emit hideGeometry(_editSet);

		_shapeHash = p->getShapeHash();
		for(QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _shapeHash.begin(); iter != _shapeHash.end(); ++ iter)		{			emit highLightGeometryEdgeSig(iter.key(), iter.value(), true);		}
		QString label = QString(tr("Selected edge(%1)")).arg(_shapeHash.size());
		_ui->edglabel->setText(label);
		double dis = p->getDistance();
		_ui->lineEditDistance->setText(QString::number(dis));

		double dir[3]{0.0};
		p->getDirection(dir);
		if (p->getReverse() == true)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int i = 0; i < 3; ++i)
					dir[i] *= -1;
			}
		}
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
		_ui->reversecheckBox->setChecked(p->getReverse());
		_ui->solidCheckBox->setChecked(p->getSloid());
	}

	void CreateExtrusionDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateExtrusionDialog::accept()
	{
		bool ok = true;
		double distance{ 0.0 };
		double direction[3]{0.0};
		double dir[3] = { 0.0 };
		bool isSolid = _ui->solidCheckBox->isChecked();

		if (_shapeHash.size() < 1) ok = false;

		if (ok)
		{
			QString text = _ui->lineEditDistance->text();
			distance = text.toDouble(&ok);
			if (distance < 1e-6) ok = false;
		}

		if (ok)
		{

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
			for (int i = 0; i < 3;++i)
			{
				direction[i] = dir[i];
			}
			if (_ui->reversecheckBox->isChecked())
			{
				for (int i = 0; i < 3; ++i)
					dir[i] *= -1;
			}
		}

		double mod = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
		if (mod < 1e-6) ok = false;
		if (ok)
		{
			double factor = distance / mod;
			for (int i = 0; i < 3; ++i)
				dir[i] *= factor;
		}		

		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		
		int id = Geometry::GeometrySet::getMaxID() + 1;
		_extruName = (QString("Extrusion_%1").arg(id));
	/*	Command::GeoCommandMakeExtrusion* command = new Command::GeoCommandMakeExtrusion(_mainWindow, _preWindow);
		command->setShapeList(_shapeHash);
		command->setAxis(dir);
		command->setName(_extruName);
		command->isSolid(isSolid);

		command->setDirection(direction);
		command->setReverse(_ui->reversecheckBox->isCheckable());
		command->setDistance(distance);
		
		if (_isEdit) command->setEditData(_editSet);

		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}*/

		QStringList codes{};
		codes += QString("extrusion = CAD.Extrusion()");

		if (_isEdit)
			codes += QString("extrusion.setEditID(%1)").arg(_editSet->getID());

		codes += QString("extrusion.setName('%1')").arg(_extruName);
		QList<Geometry::GeometrySet*> sets = _shapeHash.uniqueKeys();
		for (int i = 0; i < sets.size(); ++i)
		{
			auto s = sets.at(i);
			int id = s->getID();
			QList<int> indexs = _shapeHash.values(s);
			for(int var : indexs)
				codes += QString("extrusion.appendEdge(%1,%2)").arg(id).arg(var);
		}
		
		codes += QString("extrusion.setDistance(%1)").arg(distance);
		codes += QString("extrusion.setDirection(%1,%2,%3)").arg(direction[0]).arg(direction[1]).arg(direction[2]);
		
		bool reverse = _ui->reversecheckBox->isChecked();
		QString revesestr{};
		if (reverse == false) revesestr = "No";
		else revesestr = "Yes";
		codes += QString("extrusion.Reverse('%1')").arg(revesestr);

		QString solidstr{};
		if (isSolid == 1) solidstr = "Yes";
		else solidstr = "No";
		codes += QString("extrusion.GenerateSolid('%1')").arg(solidstr);

		if (_isEdit)
			codes += QString("extrusion.edit()");
		else
			codes += QString("extrusion.create()");

		_pyAgent->submit(codes);
		QDialog::accept();
		this->close(); 
	}

	void CreateExtrusionDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			emit showGeometry(_editSet);
		}
		QDialog::reject();
		this->close();
	}

}
