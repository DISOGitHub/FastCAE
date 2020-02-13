#include "geoPointWidget.h"
#include "ui_geoPointWidget.h"
#include "MainWidgets/preWindow.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/ModuleType.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include "geometry/geometrySet.h"
#include <QColor>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <QDebug>

namespace GeometryWidget
{
	GeoPointWidget::GeoPointWidget(GUI::MainWindow* m, MainWidget::PreWindow* p):
		_mainWindow(m), _preWindow(p)
	{
		_ui = new Ui::geoPointWidget;
		_ui->setupUi(this);
		connect(this, SIGNAL(setSelectMode(int)), _mainWindow, SIGNAL(selectModelChangedSig(int)));
		connect(p, SIGNAL(selectGeoActorShape(vtkActor*, int, Geometry::GeometrySet*)), this, SLOT(selectPoint(vtkActor*, int, Geometry::GeometrySet*)));
		connect(this, SIGNAL(updateGraphOptions()), m, SIGNAL(updateGraphOptionsSig()));
	}

	GeoPointWidget::~GeoPointWidget()
	{
		if (_ui != nullptr) delete _ui;
		emit updateGraphOptions();
		emit setSelectMode(int(ModuleBase::None));
	}

	bool GeoPointWidget::getCoordinate(double *coor)
	{
		bool ok = false;

		QString text = _ui->lineEditX->text();
		coor[0] = text.toDouble(&ok);
		if (!ok) return false;

		text = _ui->lineEditY->text();
		coor[1] = text.toDouble(&ok);
		if (!ok) return false;

		text = _ui->lineEditZ->text();
		coor[2] = text.toDouble(&ok);
		if (!ok) return false;

		return true;
	}

	void GeoPointWidget::on_geoSelectPoint_clicked()
	{
		emit setSelectMode(int(ModuleBase::GeometryPoint));
		emit buttonCkicked(this);
	}

	void GeoPointWidget::selectPoint(vtkActor* ac, int index, Geometry::GeometrySet* set)
	{
		if (!_handle) return;
		QColor color;
		if (_selectedActor != nullptr)
		{
			color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
			_selectedActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		}
		_selectedActor = ac;
		color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

		TopoDS_Shape* shape = set->getShape();
		TopExp_Explorer ptExp(*shape, TopAbs_VERTEX);
		for (int k = 0; k < index; ++k) ptExp.Next();
		const TopoDS_Vertex& vertex = TopoDS::Vertex(ptExp.Current());
		gp_Pnt pt = BRep_Tool::Pnt(vertex);

		double x = pt.X();
		double y = pt.Y();
		double z = pt.Z();

		_ui->lineEditX->setText(QString::number(x));
		_ui->lineEditY->setText(QString::number(y));
		_ui->lineEditZ->setText(QString::number(z));
 
	}

	void GeoPointWidget::handleProcess(bool h)
	{
		_handle = h;

	}

	void GeoPointWidget::setCoordinate(double *coor)
	{
		_ui->lineEditX->setText(QString::number(coor[0]));
		_ui->lineEditY->setText(QString::number(coor[1]));
		_ui->lineEditZ->setText(QString::number(coor[2]));
	}

}