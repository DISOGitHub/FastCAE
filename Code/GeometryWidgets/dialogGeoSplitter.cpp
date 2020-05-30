#include "dialogGeoSplitter.h"
#include "ui_dialogGeoSplitter.h"
#include <QMessageBox>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "python/PyAgent.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include <vtkActor.h>
#include <vtkProperty.h>
#include <BOPAlgo_Splitter.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include "GeometryCommand/GeoCommandGeoSplitter.h"
#include "GeometryCommand/GeoCommandList.h"
#include "geometry/geometryParaGeoSplitter.h"
namespace GeometryWidget
{
	GeoSplitterDialog::GeoSplitterDialog(GUI::MainWindow* m, MainWidget::PreWindow* p) :
		GeoDialogBase(m, p)
	{
		_ui = new Ui::GeoSplitterDialog;
		_ui->setupUi(this);
		
	}

	GeoSplitterDialog::GeoSplitterDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set) :
		GeoDialogBase(m,p)
	{
		_ui = new Ui::GeoSplitterDialog;
		_ui->setupUi(this);
		_isEdit = true;
		_editSet = set;
		init();

	}

	GeoSplitterDialog::~GeoSplitterDialog()
	{
		if (_ui != nullptr) delete _ui;
		emit setSelectMode((int)ModuleBase::None);
		emit updateGraphOptions();
	}
	void GeoSplitterDialog::init()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;

			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(bp);
			if (p == nullptr) return;
			auto oriset = p->getOriSet();
			if (oriset == nullptr) return;
			_body = oriset;
			QString text = QString(tr("Selected body(1)"));
			_ui->bodylabel->setText(text);

			emit hideGeometry(_editSet);
			emit showGeometry(oriset);
			emit highLightGeometrySet(oriset, true);
			_faceIndex = p->getFaceIndex();
			_faceBody = p->getFaceBody();
			if (_faceIndex >= 0 && _faceBody != nullptr)
			{
				emit highLightGeometryFace(_faceBody, _faceIndex, &_faceActor);
				_ui->bodylabel_2->setText(tr("Selected Plane(1)"));
			}
		}
			
	}

	
	void GeoSplitterDialog::closeEvent(QCloseEvent *e)
	{
		QDialog::closeEvent(e);
		delete this;
	}

	void GeoSplitterDialog::reject()
	{
		if (_isEdit)
		{
			if (_editSet == nullptr) return;


			Geometry::GeometryModelParaBase* bp = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(bp);
			if (p == nullptr) return;
			auto oriset = p->getOriSet();
			if (oriset == nullptr) return;

			emit hideGeometry(oriset);
			emit showGeometry(_editSet);
		}
		QDialog::reject();
		this->close();
	}

	void GeoSplitterDialog::accept()
	{
		bool success{ true };
		if (_faceBody == nullptr || _body == nullptr || _faceIndex < 0)
			success = false;
		if (!success) return;

		QStringList codes{};
		codes += QString("geosplitter = CAD.GeoSplitter()");

		if (_isEdit)
			codes += QString("geosplitter.setEditID(%1)").arg(_editSet->getID());
		codes += QString("geosplitter.setBody(%1)").arg(_body->getID());
		codes += QString("geosplitter.setFace(%1,%2)").arg(_faceIndex).arg(_faceBody->getID());
		if (_isEdit)
			codes += QString("geosplitter.edit()");
		else
			codes += QString("geosplitter.create()");
		_pyAgent->submit(codes);
	/*	Command::CommandGeoSplitter* c = new Command::CommandGeoSplitter(_mainWindow, _preWindow);
		c->setBody(_body);
		c->setFaceIndex(_faceIndex);
		c->setFaceBody(_faceBody);
		if (_isEdit) c->setEditData(_editSet);
		bool ok = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Create failed !"));
			return;
		}*/

		QDialog::accept();
		this->close();
		
	}

	void GeoSplitterDialog::on_geoSelectSurface_1_clicked()
	{	

		emit setSelectMode((int)ModuleBase::GeometrySurface);
		_selectPlane = true;
		_selectBody = false;
		if ((_faceActor.size() > 0) && (_faceActor[0] != nullptr))
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
			_faceActor[0]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		}

		if (_body != nullptr)
		{
			emit highLightGeometrySet(_body, false);
		}
	}

	void GeoSplitterDialog::on_geoSelectSurface_clicked()
	{
		emit setSelectMode((int)ModuleBase::GeometrySurface);
		_selectPlane = false;
		_selectBody = true;
		if ((_faceActor.size() > 0) && (_faceActor[0] != nullptr))
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
			_faceActor[0]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

		}
		if (_body != nullptr)
			emit highLightGeometrySet(_body, true);
		
	}

	void GeoSplitterDialog::selectActorShape(vtkActor* actor, int index, Geometry::GeometrySet* set)
	{
		if (_selectBody)
		{
			if (_body == set) return;
			emit highLightGeometrySet(_body, false);
			_body = set;
			emit highLightGeometrySet(_body, true);
			QString text = QString(tr("Selected body(1)"));
			_ui->bodylabel->setText(text);
		}
		else if (_selectPlane)
		{
			QColor color;
			if ((_faceActor.size() > 0) && (_faceActor[0] != nullptr))
			{
				color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
				_faceActor[0]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			}

			_faceActor.insert(0, actor);

			_faceIndex = index;
			_faceBody = set;

			color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
			_faceActor[0]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			_ui->bodylabel_2->setText(tr("Selected Plane(1)"));
		}
	}



	
}