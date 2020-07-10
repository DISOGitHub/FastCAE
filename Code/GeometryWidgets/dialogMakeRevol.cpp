#include "dialogMakeRevol.h"
#include "ui_dialogMakeExtrusion.h"
#include "ui_dialogMakeRevol.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "GeometryCommand/GeoCommandCreateChamfer.h"
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandMakeRevol.h"
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
		this->setWindowTitle("Edit Revol");
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

		_shapeHash = p->getShapeHash();
		QList<Geometry::GeometrySet*> setList = _shapeHash.uniqueKeys();
		for (int i = 0; i < setList.size(); ++i)
		{
			QList<int> edlist = _shapeHash.values(setList[i]);
			Geometry::GeometrySet* set = setList.at(i);
			if (set == nullptr) return;
			for (int var : edlist)
			{
				emit highLightGeometryEdgeSig(set, var,true);
			}

		}
		QString edgestr = QString("Select edge(%1)").arg(_shapeHash.size());
		_ui->edgelabel->setText(edgestr);

		double basicpt[3]{};
		p->getBasicPoint(basicpt);
		_baseWidget->setCoordinate(basicpt);
		double degree = p->getDegree();
		_ui->lineEditDegree->setText(QString::number(degree));
		_ui->comboBoxOption->setCurrentIndex(p->getMethod());
		_ui->tabWidget->setCurrentIndex(p->getMethod());
		
		if (p->getMethod()==0)
		{
			_axisSet = p->getAxisEdge().first;
			_axisIndex = p->getAxisEdge().second;
			emit highLightGeometryEdgeSig(_axisSet, _axisIndex, true);
			if (_axisSet != nullptr)
				_ui->edgelabel_2->setText(QString("Selected edge(1)"));
		}
		else
		{
			double dir[3]{};
			p->getCoor(dir);

			if (dir[0] == 1 && dir[1] == 0 && dir[2] == 0)
			{
				_ui->radioButtonX->setChecked(true);
			}
			else if (dir[0] == 0 && dir[1] == 1 && dir[2] == 0)
			{
				_ui->radioButtonY->setChecked(true);
			}
			else if (dir[0] == 0 && dir[1] == 0 && dir[2] == 1)
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

	}

	void CreateRevolDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		if (_selectEdge)
		{
			bool legal{};
			if (_shapeHash.contains(set, index))
			{
				legal = false;
				_shapeHash.remove(set, index);
			}
			else
			{
				legal = true;
				_shapeHash.insert(set, index);
			}
			int n = _shapeHash.size();
			QString label = QString(tr("Selected edge(%1)")).arg(_shapeHash.size());
			_ui->edgelabel->setText(label);
			emit highLightGeometryEdgeSig(set, index, legal);
		}
		else if (_selectAxisEdge)
		{
			if (_axisSet != nullptr)
			{
				emit highLightGeometryEdgeSig(_axisSet, _axisIndex, false);
			}
			_axisSet = set;
			_axisIndex = index;
			emit highLightGeometryEdgeSig(_axisSet, _axisIndex, true);
			_ui->edgelabel_2->setText(tr("Selected Axis_edge(1)"));
		}
	}

	void CreateRevolDialog::on_geoSelectCurve_clicked()
	{
		emit setSelectMode(int(ModuleBase::GeometryCurve));
		_baseWidget->handleProcess(false);
		_selectEdge = true;
		_selectAxisEdge = false;

		if (_axisSet != nullptr)
			emit highLightGeometryEdgeSig(_axisSet, _axisIndex, false);

		QList<Geometry::GeometrySet*> setlist = _shapeHash.uniqueKeys();
		for (int i = 0; i < setlist.size(); i++)
		{
			QList<int> indexlist = _shapeHash.values(setlist[i]);
			for (int index : indexlist)
			{
				emit highLightGeometryEdgeSig(setlist[i], index, true);
			}
		}

	}
	void CreateRevolDialog::on_geoSelectCurve_1_clicked()
	{
		_selectEdge = false;
		_baseWidget->handleProcess(false);
		_selectAxisEdge = true;

		if (_axisSet != nullptr)
			emit highLightGeometryEdgeSig(_axisSet, _axisIndex, true);
					 
		QList<Geometry::GeometrySet*> setlist = _shapeHash.uniqueKeys();
		for (int i = 0; i < setlist.size(); i++)
		{
			QList<int> indexlist = _shapeHash.values(setlist[i]);
			for (int index : indexlist)
			{
				emit highLightGeometryEdgeSig(setlist[i], index, false);
			}
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
				ok = fabs(degree) < 0.0000001 ? false : true;
		}
		if (_ui->comboBoxOption->currentIndex() == 0 && _axisSet == nullptr)
			ok = false;
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
		bool reverse = _ui->reversecheckBox->isChecked();
		QString revesestr{};
		if (reverse) revesestr = "Yes";
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
