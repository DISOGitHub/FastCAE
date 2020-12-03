#include "dialogMakeSweep.h"
#include "ui_dialogMakeSweep.h"
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
		_solid = p->getSloid();
		_ui->solidCheckBox->setChecked(_solid);

		QList<Geometry::GeometrySet*> setList = _sectionEdgeHash.uniqueKeys();
		for (int i = 0; i < setList.size(); ++i)
		{
			Geometry::GeometrySet* set = setList.at(i);
			QList<int> edlist = _sectionEdgeHash.values(set);
			if (set == nullptr) return;
			for(int var : edlist)
			{
				emit highLightGeometryEdgeSig(set, var, true);
			}
			
		}
		emit highLightGeometryEdgeSig(_pathEdge.first, _pathEdge.second, true);
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

		if ( _sectionEdgeHash.size() < 1||_pathEdge.first==nullptr)
			ok = false;
		
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}
		/*
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
		_solid = _ui->solidCheckBox->isChecked();
		if (_solid) solidstr = "Yes";
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

	void SweepDialog::shapeSlected(Geometry::GeometrySet* set, int shape)
	{
		if (_selectPath && !_selectSection)
			selectPath(shape, set);
		else if (_selectSection && !_selectPath)
			selectSection(shape, set);
	}

	void SweepDialog::on_geoSelectCurve_clicked()
	{
		_selectPath = false;
		_selectSection = true;
		emit setSelectMode((int)ModuleBase::GeometryCurve);
		for (QMultiHash<Geometry::GeometrySet*, int>::iterator iter = _sectionEdgeHash.begin(); iter != _sectionEdgeHash.end(); ++iter)		{			emit highLightGeometryEdgeSig(iter.key(), iter.value(), true);		}
	}

	void SweepDialog::on_geoSelectCurve_1_clicked()
	{
		_selectPath = true;
		_selectSection = false;
		emit setSelectMode((int)ModuleBase::GeometryCurve);
		if (_pathEdge.first!=nullptr)
		{
			emit highLightGeometryEdgeSig(_pathEdge.first, _pathEdge.second, true);
		}
	}

	void SweepDialog::selectSection(int index, Geometry::GeometrySet* set)
	{
		QColor color;
		if (_sectionEdgeHash.contains(set,index))
		{

			emit highLightGeometryEdgeSig(set, index, false);
			_sectionEdgeHash.remove(set, index);
		}
		else
		{
			emit highLightGeometryEdgeSig(set, index, true);
			_sectionEdgeHash.insert(set, index);
		}


		QString label = QString(tr("Selected edge(%1)")).arg(_sectionEdgeHash.size());
		_ui->edglabel->setText(label);
	}

	void SweepDialog::selectPath(int shape, Geometry::GeometrySet* set)
	{
		if (_sectionEdgeHash.contains(set,shape)) return;

		if (_pathEdge.first != nullptr)
		{
			emit highLightGeometryEdgeSig(_pathEdge.first, _pathEdge.second, false);
			_pathEdge.first = nullptr;
			_pathEdge.second = shape;
		}
		_pathEdge.first = set;
		_pathEdge.second = shape;
		emit highLightGeometryEdgeSig(_pathEdge.first, _pathEdge.second, true);

		QString label = QString(tr("Selected edge(%1)")).arg(1);
		_ui->pathlabel->setText(label);
	}

}