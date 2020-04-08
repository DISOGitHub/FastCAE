#include "dialogMeasureDistance.h"
#include "ui_dialogMeasureDistance.h"
#include "MainWidgets/preWindow.h"
#include "mainWindow/mainWindow.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include <vtkActor.h>
#include <vtkProperty.h>
#include "geometry/geometrySet.h"
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>

namespace GeometryWidget
{

	MeasureDistanceDialog::MeasureDistanceDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::MeasureaDistanceDialog;
		_ui->setupUi(this);
		_point[0] = _point[1] = _point[2] = 3e66;

		connect(p, SIGNAL(selectGeoActorShape(vtkActor*, int, Geometry::GeometrySet*)), this, SLOT(selectPoint(vtkActor*, int, Geometry::GeometrySet*)));
	}

	MeasureDistanceDialog::~MeasureDistanceDialog()
	{
		if (nullptr != _ui) delete _ui;
		
		emit updateGraphOptions();
		emit setSelectMode(int(ModuleBase::None));
	}

	void MeasureDistanceDialog::on_closeButton_clicked()
	{
		this->close();
		delete this;
	}

	void MeasureDistanceDialog::on_geoSelectPoint_clicked()
	{
		emit setSelectMode(int(ModuleBase::GeometryPoint));
	}

	void MeasureDistanceDialog::selectActorShape(vtkActor* actor, int index, Geometry::GeometrySet* set)
	{
		if (actor == nullptr || set == nullptr) return;
		QList<vtkActor*> actors;
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
		emit highLightGeometryPoint(set, index, &actors);
		_actors.append(actor);

		

		if (_actors.size() > 2)
		{
			vtkActor* ac = _actors.first();
			ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			_actors.removeFirst();
		}
		
		_ui->topedgelabel->setText(QString(tr("Selected Object(%1)")).arg(_actors.size()));

		TopoDS_Shape* shape = set->getShape();
		TopExp_Explorer ptExp(*shape, TopAbs_VERTEX);
		for (int k = 0; k < index; ++k) ptExp.Next();
		const TopoDS_Vertex& vertex = TopoDS::Vertex(ptExp.Current());
		gp_Pnt pt = BRep_Tool::Pnt(vertex);
		gp_Pnt bef(_point[0], _point[1], _point[2]);
		if (_actors.size() != 2)
		{
			_point[0] = pt.X();
			_point[1] = pt.Y();
			_point[2] = pt.Z();
			return;
		}

		double d = bef.Distance(pt);
		_ui->disLabel->setText(QString::number(d));

		_point[0] = pt.X();
		_point[1] = pt.Y();
		_point[2] = pt.Z();
	}

}