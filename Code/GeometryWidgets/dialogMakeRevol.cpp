#include "dialogMakeRevol.h"
#include "ui_dialogMakeExtrusion.h"
#include "ui_dialogMakeRevol.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include "GeometryCommand/GeoCommandCreateChamfer.h"
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandMakeRevol.h"
#include <vtkProperty.h>
#include <vtkActor.h>
#include <QMessageBox>
#include <QDebug>
#include <QColor>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include "geometry/geometrySet.h"
#include <BRepAdaptor_Curve.hxx>
#include <gp_Lin.hxx>
#include "python/PyAgent.h"
#include "geometry/geometryParaRevol.h"
namespace GeometryWidget
{
	
	CreateRevolDialog::CreateRevolDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateRevol;
		_ui->setupUi(this);
		init();
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		
		on_radioButtonUser();
		this->translateButtonBox(_ui->buttonBox);
	}

	CreateRevolDialog::CreateRevolDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set)
		: GeoDialogBase(m, pre)
	{
		_ui = new Ui::CreateRevol;
		_ui->setupUi(this);
		init();
		connect(_ui->radioButtonUser, SIGNAL(toggled(bool)), this, SLOT(on_radioButtonUser()));
		connect(_ui->comboBoxOption, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		on_radioButtonUser();
		this->translateButtonBox(_ui->buttonBox);
		_isEdit = true;
		_editSet = set;

		if (_editSet == nullptr) return;
		Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
		Geometry::GeometryParaRevol* p = dynamic_cast<Geometry::GeometryParaRevol*>(bp);
		if (p == nullptr) return;
		emit hideGeometry(_editSet);

		QMultiHash<Geometry::GeometrySet*, int> shapeHash;
		shapeHash = p->getShapeHash();
		_shapeHash = shapeHash;
		QList<Geometry::GeometrySet*> setList = shapeHash.keys();
		int k = setList.size();
		for (int i = 0; i < setList.size(); ++i)
		{
			QList<int> edlist = shapeHash.values(setList[i]);
			Geometry::GeometrySet* set = setList.at(i);
			if (set == nullptr) return;
			int shapes = shapeHash.value(set);
			for (int var : edlist)
			{
				emit highLightGeometryEdge(set, var, &_edgeActors);
			}

		}

		QString edgestr = QString("Select edge(%1)").arg(_shapeHash.size());
		_ui->edgelabel->setText(edgestr);

		double basicpt[3]{};
		p->getBasicPoint(basicpt);
		_baseWidget->setCoordinate(basicpt);
		
		double degree = p->getDegree();
		_ui->lineEditDegree->setText(QString::number(degree));


		_ui->tabWidget->setCurrentIndex(p->getMethod());

		
		if (p->getMethod()==0)
		{
			_axisSet = p->getAxisEdge().first;
			_axisIndex = p->getAxisEdge().second;
			QList<vtkActor*> templist;
			emit highLightGeometryEdge(_axisSet, _axisIndex, &templist);
			if (_axisSet != nullptr)
				_ui->edgelabel_2->setText(QString("Selected edge(1)"));
		}
		else
		{
			double dir[3]{};
			p->getCoor(dir);

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
		bool reverse = p->getReverse();
		_ui->reversecheckBox->setChecked(reverse);

		bool solid = p->getSolid();
		_ui->solidCheckBox->setChecked(solid);

	}

	void CreateRevolDialog::init()
	{
		_ui->tabWidget->tabBar()->hide();
		_baseWidget = new GeoPointWidget(_mainWindow, _preWindow);
		_ui->basicPointLayout->addWidget(_baseWidget);
		connect(_baseWidget, SIGNAL(buttonCkicked(GeoPointWidget*)), this, SLOT(pointWidgetClicked(GeoPointWidget*)));
	}

	CreateRevolDialog::~CreateRevolDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit setSelectMode((int)ModuleBase::None);
		emit updateGraphOptions();
	}

	void CreateRevolDialog::selectActorShape(vtkActor* ac, int index, Geometry::GeometrySet* set)
	{
		if (_selectEdge)
		{
			QColor color;
			if (_edgeActors.contains(ac))
			{
				color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
				_edgeActors.removeOne(ac);
				_shapeHash.remove(set, index);
			}
			else
			{
				color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
				_edgeActors.append(ac);
				_shapeHash.insert(set, index);
			}

			QString label = QString(tr("Selected edge(%1)")).arg(_edgeActors.size());
			_ui->edgelabel->setText(label); 
			ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		}
		else if (_selectAxisEdge)
		{
			QColor color;
			if (_axisActor != nullptr)
			{
				color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
				_axisActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			}
			color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
			_axisActor = ac;
			_axisActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

			_axisSet = set;
			_axisIndex = index;

			_ui->edgelabel_2->setText(tr("Selected Axis_edge(1)"));
		}
	}

	void CreateRevolDialog::on_geoSelectCurve_clicked()
	{
		emit setSelectMode(int(ModuleBase::GeometryCurve));
		_baseWidget->handleProcess(false);
		_selectEdge = true;
		_selectAxisEdge = false;

		QColor c = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
		if (_axisActor != nullptr)
			_axisActor->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());

		c = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		for (int i = 0; i < _edgeActors.size(); ++i)
		{
			auto ac = _edgeActors.at(i);
			ac->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
		}

	}
	void CreateRevolDialog::on_geoSelectCurve_1_clicked()
	{
		_selectEdge = false;
		_baseWidget->handleProcess(false);
		_selectAxisEdge = true;

		QColor c = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		if (_axisActor != nullptr)
			_axisActor->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
					 
		c = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
		for (int i = 0; i < _edgeActors.size(); ++i)
		{
			auto ac = _edgeActors.at(i);
			ac->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
		}

		emit setSelectMode((int)ModuleBase::GeometryCurve);
	}

	void CreateRevolDialog::on_radioButtonUser()
	{
		bool checked = _ui->radioButtonUser->isChecked();
		_ui->doubleSpinBoxX->setVisible(checked);
		_ui->doubleSpinBoxY->setVisible(checked);
		_ui->doubleSpinBoxZ->setVisible(checked);
	
	}

	void CreateRevolDialog::pointWidgetClicked(GeoPointWidget* w)
	{
		w->handleProcess(true);
		_selectAxisEdge = false;
		_selectEdge = false;
	}

	void CreateRevolDialog::on_TypeChanged(int index)
	{
		_ui->tabWidget->setCurrentIndex(index);
	}

	void CreateRevolDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateRevolDialog::accept()
	{
		double basicPoint[3] = { 0.0 };
		double degree{ 0.0 };
		bool isSolid = _ui->solidCheckBox->isChecked();
		

		bool ok = _baseWidget->getCoordinate(basicPoint);
		if (_shapeHash.size() < 1) ok = false;
		if (ok)
		{
			QString text = _ui->lineEditDegree->text();
			degree = text.toDouble(&ok);
			if (ok)
				ok = abs(degree) < 0.0000001 ? false : true;
		}		
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		
		int id = Geometry::GeometrySet::getMaxID() + 1;
		QString name = QString("Revol_%1").arg(id);
/*

		Command::GeoCommandMakeRevol* command = new Command::GeoCommandMakeRevol(_mainWindow, _preWindow);
		command->setShapeList(_shapeHash);
		command->setVector(vec);
		command->setName(name);
		command->setDegree(degree);
		command->setBasicPoint(basicPoint);
		command->isSolid(isSolid);

		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}*/
		
		QStringList codes{};
		codes += QString("revol = CAD.Revol()");

		if (_isEdit)
			codes += QString("revol.setEditID(%1)").arg(_editSet->getID());

		codes += QString("revol.setName('%1')").arg(name);

		QList<Geometry::GeometrySet*> sets = _shapeHash.uniqueKeys();
		for (int i = 0; i < sets.size(); ++i)
		{
			auto s = sets.at(i);
			int id = s->getID();
			QList<int> indexs = _shapeHash.values(s);
			for(int var : indexs)
				codes += QString("revol.appendEdge(%1,%2)").arg(id).arg(var);
		}
		codes += QString("revol.setBasicPoint(%1,%2,%3)").arg(basicPoint[0]).arg(basicPoint[1]).arg(basicPoint[2]);
		codes += QString("revol.setDegree(%1)").arg(degree);

		int optionindex = _ui->comboBoxOption->currentIndex();
		QString optionindexstr{};
		if (optionindex == 1) optionindexstr = "Coordinate";
		else optionindexstr = "Select On Geometry";
		codes += QString("revol.setAxisMethod('%1')").arg(optionindexstr);
		
		if (optionindex == 1)
		{
			double coor[3]{0.0};
			if (_ui->radioButtonX->isChecked())
				coor[0] = 1.0;
			else if (_ui->radioButtonY->isChecked())
				coor[1] = 1.0;
			else if (_ui->radioButtonZ->isChecked())
				coor[2] = 1.0;
			else
			{
				coor[0] = _ui->doubleSpinBoxX->value();
				coor[1] = _ui->doubleSpinBoxY->value();
				coor[2] = _ui->doubleSpinBoxZ->value();
			}
			codes += QString("revol.setCoordinate(%1,%2,%3)").arg(coor[0]).arg(coor[1]).arg(coor[2]);
		}
		else
			codes += QString("revol.selectAxisOnGeo(%1,%2)").arg(_axisSet->getID()).arg(_axisIndex);
		bool reverse = _ui->reversecheckBox->isCheckable();
		QString revesestr{};
		if (reverse == 0) revesestr = "Yes";
		else revesestr = "No";
		codes += QString("revol.Reverse('%1')").arg(revesestr);
		
		QString solidstr{};
		if (isSolid == 1) solidstr = "Yes";
		else solidstr = "No";
		codes += QString("revol.GenerateSolid('%1')").arg(solidstr);

		if (_isEdit)
			codes += QString("revol.edit()");
		else
			codes += QString("revol.create()");

		_pyAgent->submit(codes);



		QDialog::accept();
		this->close(); 
	}
	

	void CreateRevolDialog::reject()
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
