#include "dialogMeasureDistance.h"
#include "ui_dialogMeasureDistance.h"
#include "MainWidgets/preWindow.h"
#include "mainWindow/mainWindow.h"
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

	void MeasureDistanceDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{
		if ( set == nullptr) return;
		emit highLightGeometryPointSig(set, index, true);
		QPair<Geometry::GeometrySet*, int> temp{set,index};
		_ptlist.push_back(temp);

		if (_ptlist.size() > 2)
		{
			QPair<Geometry::GeometrySet*, int> firstpair = _ptlist.front();
			Geometry::GeometrySet* geoset = firstpair.first;
			int setindex = firstpair.second;
			emit highLightGeometryPointSig(geoset, setindex, false);
			_ptlist.removeFirst();
		}
		_ui->topedgelabel->setText(QString(tr("Selected Object(%1)")).arg(_ptlist.size()));

		TopoDS_Shape* shape = set->getShape();
		TopExp_Explorer ptExp(*shape, TopAbs_VERTEX);
		for (int k = 0; k < index; ++k) ptExp.Next();
		const TopoDS_Vertex& vertex = TopoDS::Vertex(ptExp.Current());
		gp_Pnt pt = BRep_Tool::Pnt(vertex);
		gp_Pnt bef(_point[0], _point[1], _point[2]);
		if (_ptlist.size() != 2)
		{
			_point[0] = pt.X();
			_point[1] = pt.Y();
			_point[2] = pt.Z();
			this->setPointLocation(_point, nullptr);
			return;
		}

		double d = bef.Distance(pt);
		double p1[3] = { bef.X(), bef.Y(), bef.Z() };
		double p2[3] = { pt.X(), pt.Y(), pt.Z() };
		this->setPointLocation(p1, p2);

		_ui->disLabel->setText(QString::number(d));

		_point[0] = pt.X();
		_point[1] = pt.Y();
		_point[2] = pt.Z();
	}

	void MeasureDistanceDialog::setPointLocation(double* p1, double* p2)
	{
		const QString em{ "0.0" };
		_ui->X1->setText(em); _ui->Y1->setText(em); _ui->Z1->setText(em);
		_ui->X2->setText(em); _ui->Y2->setText(em); _ui->Z2->setText(em);

		if (p1 != nullptr)
		{
			_ui->X1->setText(QString::number(p1[0], 'f', 6)); 
			_ui->Y1->setText(QString::number(p1[1], 'f', 6));
			_ui->Z1->setText(QString::number(p1[2], 'f', 6));
		}
		if (p2 != nullptr)
		{
			_ui->X2->setText(QString::number(p2[0], 'f', 6));
			_ui->Y2->setText(QString::number(p2[1], 'f', 6));
			_ui->Z2->setText(QString::number(p2[2], 'f', 6));
		}
	}

}