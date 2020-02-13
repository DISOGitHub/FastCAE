#include "dialogMakeSweep.h"
#include "ui_dialogMakeSweep.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include <vtkActor.h>
#include <vtkProperty.h>
#include <QMessageBox>
#include "geometry/geometrySet.h"
#include "GeometryCommand/GeoCommandList.h"
#include "GeometryCommand/GeoCommandMakeSweep.h"
#include "python/PyAgent.h"
#include "geometry/geometryParaSweep.h"

namespace GeometryWidget
{
	SweepDialog::SweepDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::SweepDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
	}

	SweepDialog::SweepDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set)
		: GeoDialogBase(m, pre)
	{
		_ui = new Ui::SweepDialog;
		_ui->setupUi(this);
		this->translateButtonBox(_ui->buttonBox);
		_isEdit = true;
		_editSet = set;
		init();
	}

	SweepDialog::~SweepDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit setSelectMode((int)ModuleBase::None);
		emit updateGraphOptions();
	}
	void SweepDialog::init()
	{
		if (_editSet == nullptr) return;
		Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
		Geometry::GeometryParaSweep* p = dynamic_cast<Geometry::GeometryParaSweep*>(bp);
		if (p == nullptr) return;
		emit hideGeometry(_editSet);
		_pathEdge = p->getPath();
		_sectionEdgeHash = p->getShapeHash();
		
		QList<Geometry::GeometrySet*> setList = _sectionEdgeHash.keys();
		int k = setList.size();
		for (int i = 0; i < setList.size(); ++i)
		{
			Geometry::GeometrySet* set = setList.at(i);
			QList<int> edlist = _sectionEdgeHash.values(set);
			if (set == nullptr) return;
			int shapes = _sectionEdgeHash.value(set);
			for(int var : edlist)
			{
				emit highLightGeometryEdge(set, var, &_sectionActors);
			}
			
		}
		QList<vtkActor*> acs;
		emit highLightGeometryEdge(_pathEdge.first, _pathEdge.second,&acs);
		_pathActor = acs.at(0);

		QString label = QString(tr("Selected edge(%1)")).arg(_sectionEdgeHash.size());
		_ui->edglabel->setText(label);
		if (_pathEdge.first != nullptr)
		{
			QString label = QString(tr("Selected edge(%1)")).arg(1);
			_ui->pathlabel->setText(label);
		}

	}
	void SweepDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void SweepDialog::accept()
	{
		bool ok = true;

		if (_pathActor == nullptr || _sectionActors.size() < 1)
			ok = false;

		bool isSolid = _ui->solidCheckBox->isChecked();
/*

		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		Command::GeoCommandMakeSweep* command = new Command::GeoCommandMakeSweep(_mainWindow, _preWindow);
		command->setSection(_sectionEdgeHash);
		command->setPath(_pathEdge);
		command->isSolid(isSolid);

		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}*/


		QStringList codes{};
		codes += QString("sweep = CAD.Sweep()");
		if (_isEdit)
			codes += QString("sweep.setEditID(%1)").arg(_editSet->getID());

		QList<Geometry::GeometrySet*> sets = _sectionEdgeHash.uniqueKeys();
		for (int i = 0; i < sets.size(); ++i)
		{
			auto s = sets.at(i);
			int id = s->getID();
			QList<int> indexs = _sectionEdgeHash.values(s);
			for(int var : indexs)
				codes += QString("sweep.appendEdge(%1,%2)").arg(id).arg(var);
		}
		int pathedge = _pathEdge.second;
		int pathset = _pathEdge.first->getID();
		codes += QString("sweep.setPath(%1,%2)").arg(pathset).arg(pathedge);

		QString solidstr{};
		if (isSolid) solidstr = "Yes";
		else solidstr = "No";
		codes += QString("sweep.isSolid('%1')").arg(solidstr);

		if (_isEdit)
			codes += QString("sweep.edit()");
		else
			codes += QString("sweep.create()");

		_pyAgent->submit(codes);
		QDialog::accept();
		this->close();
	}

	void SweepDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;
			emit showGeometry(_editSet);

		}

		QDialog::reject();
		this->close();
	}

	void SweepDialog::selectActorShape(vtkActor* ac, int shape, Geometry::GeometrySet* set)
	{

		if (_selectPath && !_selectSection)
			selectPath(ac, shape, set);
		else if (_selectSection && !_selectPath)
			selectSection(ac, shape, set);
	}

	void SweepDialog::on_geoSelectCurve_clicked()
	{
		_selectPath = false;
		_selectSection = true;
		emit setSelectMode((int)ModuleBase::GeometryCurve);
	}

	void SweepDialog::on_geoSelectCurve_1_clicked()
	{
		_selectPath = true;
		_selectSection = false;
		emit setSelectMode((int)ModuleBase::GeometryCurve);
	}

	void SweepDialog::selectSection(vtkActor* ac, int index, Geometry::GeometrySet* set)
	{
		QColor color;
		if (_sectionActors.contains(ac))
		{
			color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
			_sectionActors.removeOne(ac);
			_sectionEdgeHash.remove(set, index);
		}
		else
		{
			color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
			_sectionActors.append(ac);
			_sectionEdgeHash.insert(set, index);
		}

		ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

		QString label = QString(tr("Selected edge(%1)")).arg(_sectionActors.size());
		_ui->edglabel->setText(label);
	}

	void SweepDialog::selectPath(vtkActor* ac, int shape, Geometry::GeometrySet* set)
	{
		if (ac == nullptr) return;
		if (_sectionActors.contains(ac)) return;

		QColor color;
		if (_pathActor != nullptr)
		{
			color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
			_pathActor ->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			_pathActor = nullptr;
			_pathEdge.first = nullptr;
			_pathEdge.second = shape;
		}
		
		color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		_pathActor = ac;
		_pathEdge.first = set;
		_pathEdge.second = shape;
		
		_pathActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

		QString label = QString(tr("Selected edge(%1)")).arg(1);
		_ui->pathlabel->setText(label);
	}



}