#include "dialogCreateFace.h"
#include "ui_dialogCreateFace.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "MainWidgets/preWindow.h"
#include "geometry/geometrySet.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include "GeometryCommand/GeoCommandCreateFace.h"
#include "GeometryCommand/GeoCommandList.h"
#include <vtkProperty.h>
#include <vtkActor.h>
#include <QMessageBox>
#include <QDebug>
#include "geometry/geometryParaFace.h"
#include "python/PyAgent.h"

namespace GeometryWidget
{
	CreateFaceDialog::CreateFaceDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GeoDialogBase(m,pre)
	{
		_ui = new Ui::CreateFaceDialog;
		_ui->setupUi(this);
		init();
		
	}

	CreateFaceDialog::CreateFaceDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
		: GeoDialogBase(m, p)
	{
		_ui = new Ui::CreateFaceDialog;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		this->setWindowTitle("Edit Face");
		init();


	}
	void CreateFaceDialog::init()
	{
		this->translateButtonBox(_ui->buttonBox);

		if (!_isEdit)
		{
			int id = Geometry::GeometrySet::getMaxID() + 1;
			_ui->namelineEdit->setPlaceholderText(QString("Face_%1").arg(id));
		}
		else
		{
			if (_editSet == nullptr) return;
			emit hideGeometry(_editSet);

			_ui->namelineEdit->setText(_editSet->getName());
			_ui->namelineEdit->setEnabled(false);
			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaFace* p = dynamic_cast<Geometry::GeometryParaFace*>(bp);
			if (p == nullptr) return;
			QMultiHash<Geometry::GeometrySet*, int> shapeHash;
			shapeHash = p->getShapeHash();
			_shapeHash = shapeHash;
			QList<Geometry::GeometrySet*> setList = shapeHash.keys();
			int k = setList.size();
			for (int i = 0; i < setList.size(); ++i)
			{
				Geometry::GeometrySet* set = setList.at(i);
				if (set == nullptr) return;
				int shapes = shapeHash.value(set);
				emit highLightGeometryEdge(set, shapes, &_actors);
			}
	
			QString label = QString(tr("Selected edge(%1)")).arg(shapeHash.size());
			_ui->edgelabel->setText(label);
			
		}
	}
	CreateFaceDialog::~CreateFaceDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit setSelectMode((int)ModuleBase::None);
		emit updateGraphOptions();
	}

	void CreateFaceDialog::on_geoSelectCurve_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometryCurve);
	}



	void CreateFaceDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void CreateFaceDialog::reject()
	{
		QDialog::reject();
		this->close();
	}

	void CreateFaceDialog::accept()
	{

		QString name = _ui->namelineEdit->text();
	    if (name.isEmpty())
			name = _ui->namelineEdit->placeholderText();
		bool ok = !name.isEmpty();
		
		if (!ok || _shapeHash.size() < 1)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Input Wrong !"));
			return;
		}

		QStringList codes{};
				
		codes += QString("face = CAD.Face()");
		if (_isEdit)
		{
			codes += QString("face.setEditID(%1)").arg(_editSet->getID());
		}
		else
			codes += QString("face.setName('%1')").arg(name);
		

		QList<Geometry::GeometrySet*> sets = _shapeHash.uniqueKeys();
		for (int i = 0; i < sets.size(); ++i)
		{
			auto s = sets.at(i);
			int id = s->getID();
			QList<int> indexs = _shapeHash.values(s);
			for(int var : indexs)
				codes += QString("face.appendEdge(%1,%2)").arg(id).arg(var);
		}

		if (_isEdit)
			codes += QString("face.edit()");
		else
			codes += QString("face.create()");
		_pyAgent->submit(codes);
/*

		Command::CommandCreateFace* c = new Command::CommandCreateFace(_mainWindow, _preWindow);
		c->setShapeList(_shapeHash);
		c->setName(name);
		c->setActor(_actors);
		int edges = _actors.size();
		c->setEdges(edges);
		if (_isEdit) c->setEditData(_editSet);

		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed ! "));
			return;
		}*/
		QDialog::accept();
		this->close();
	}

	void CreateFaceDialog::selectActorShape(vtkActor* ac, int shape, Geometry::GeometrySet* set)
	{
		QColor color;
		if (_actors.contains(ac))
		{
			color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
			_actors.removeOne(ac);
			_shapeHash.remove(set, shape);
		}
		else
		{
			color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
			_actors.append(ac);
			_shapeHash.insert(set, shape);
		}
	
		ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		

		QString label = QString(tr("Selected edge(%1)")).arg(_actors.size());
		_ui->edgelabel->setText(label);

		
	}


}
