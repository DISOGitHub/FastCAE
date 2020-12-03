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
#include <Adaptor3d_Curve.hxx>
#include <CPnts_AbscissaPoint.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <QDebug>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

namespace GeometryWidget
{

	MeasureDistanceDialog::MeasureDistanceDialog(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoDialogBase(m, p)
	{
		_ui = new Ui::MeasureaDistanceDialog;
		_ui->setupUi(this);
		_point[0] = _point[1] = _point[2] = 3e66;
		connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TypeChanged(int)));
		connect(p, SIGNAL(selectGeoActorShape(vtkActor*, int, Geometry::GeometrySet*)), this, SLOT(selectPoint(vtkActor*, int, Geometry::GeometrySet*)));
		_ui->tabWidget->tabBar()->hide();
	}

	MeasureDistanceDialog::~MeasureDistanceDialog()
	{
		if (nullptr != _ui) delete _ui;
	}

	void MeasureDistanceDialog::on_TypeChanged(int index)
	{
		emit setSelectMode(int(ModuleBase::None));
		_ui->tabWidget->setCurrentIndex(index);
		_typeindex = index;
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

	void MeasureDistanceDialog::on_geoSelectCurve_clicked()
	{
		ImHighlightPoints();
		ImHighlightSurface();
		ImHighlightSolid();
		emit setSelectMode(int(ModuleBase::GeometryCurve));
	}

	void MeasureDistanceDialog::on_geoSelectSurface_1_clicked()
	{
		ImHighlightPoints();
		ImHighlightCurve();
		ImHighlightSolid();
		emit setSelectMode(int(ModuleBase::GeometrySurface));
	}

	void MeasureDistanceDialog::on_geoSelectSurface_clicked()
	{
		ImHighlightPoints();
		ImHighlightCurve();
		ImHighlightSurface();
		emit setSelectMode(int(ModuleBase::GeometryBody));
	}

	void MeasureDistanceDialog::shapeSlected(Geometry::GeometrySet* set, int index)
	{

		if ( set == nullptr && index < 0) return;
		switch (_typeindex)
		{
		case 0: DistanceTwoPts(set, index); break;
		case 1: LengthOfCurve(set, index); break;
		case 2: AreaOfSurface(set, index); break;
		case 3: VolumeOfSolid(set, index); break;
		default:break;
		}
		
	}

	void MeasureDistanceDialog::DistanceTwoPts(Geometry::GeometrySet* set, int index)
	{
		if (set == nullptr) return;
		emit highLightGeometryPointSig(set, index, true);
		QPair<Geometry::GeometrySet*, int> temp{ set, index };
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

	void MeasureDistanceDialog::LengthOfCurve(Geometry::GeometrySet* set, int index)
	{
		
		if (_curve.first != nullptr)
		{
			emit highLightGeometryEdgeSig(_curve.first, _curve.second, false);
			_curve.first = nullptr;
			_curve.second = index;
		}
		_curve.first = set;
		_curve.second = index;

		emit highLightGeometryEdgeSig(_curve.first, _curve.second, true);
		_ui->topedgelabel_2->setText(QString(tr("Selected Object(1)")));

		//计算曲线长度
		if (_curve.first == nullptr&&_curve.second < 0) return;
		TopoDS_Shape* shape = _curve.first->getShape();
		TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
		for (int indexExp = 0; indexExp < _curve.second && edgeExp.More(); edgeExp.Next(), ++indexExp);
		const TopoDS_Shape& edgeShape = edgeExp.Current();
		if (edgeShape.IsNull()) return;
		
		const TopoDS_Edge &oneedge = TopoDS::Edge(edgeShape);
		Standard_Real edgeLength = GCPnts_AbscissaPoint::Length(BRepAdaptor_Curve(oneedge));
		if (edgeLength < 0)return;
		_ui->disLabel_2->setText(QString::number(edgeLength));
		
	}

	void MeasureDistanceDialog::AreaOfSurface(Geometry::GeometrySet* set, int index)
	{
		if (_surface.first != nullptr)
		{
			emit highLightGeometryFaceSig(_surface.first, _surface.second, false);
			_surface.first = nullptr;
			_surface.second = index;
		}
		_surface.first = set;
		_surface.second = index;
		emit highLightGeometryFaceSig(_surface.first, _surface.second, true);
		_ui->topedgelabel_3->setText(QString(tr("Selected Object(1)")));

		//计算曲面面积
		if (_surface.first == nullptr&&_surface.second < 0) return;
		TopoDS_Shape* shape = _surface.first->getShape();
		TopExp_Explorer edgeExp(*shape, TopAbs_FACE);
		for (int indexExp = 0; indexExp < _surface.second && edgeExp.More(); edgeExp.Next(), ++indexExp);
		const TopoDS_Shape& faceShape = edgeExp.Current();  
		if (faceShape.IsNull()) return;
		const TopoDS_Face &oneface = TopoDS::Face(faceShape); 
		GProp_GProps aSurfaceProps;
		BRepGProp::SurfaceProperties(oneface, aSurfaceProps);

		double area = aSurfaceProps.Mass();
		if (area < 0) return;
		_ui->disLabel_3->setText(QString::number(area));
	}

	void MeasureDistanceDialog::VolumeOfSolid(Geometry::GeometrySet* set, int index)
	{ 
		if (_solid.first != nullptr)
		{
			emit highLightGeometrySolidSig(_solid.first, _solid.second, false);
			_solid.first = nullptr;
			_solid.second = index;
		}
		_solid.first = set;
		_solid.second = index;
		emit highLightGeometrySolidSig(_solid.first, _solid.second, true);
		_ui->topedgelabel_4->setText(QString(tr("Selected Object(1)")));

		//计算实体体积
		if (_solid.first == nullptr&&_solid.second < 0) return;
		TopoDS_Shape* shape = _solid.first->getShape();
		TopExp_Explorer solidExp(*shape, TopAbs_SOLID);
		for (int indexExp = 0; indexExp < _solid.second && solidExp.More(); solidExp.Next(), ++indexExp);
		const TopoDS_Shape& solidShape = solidExp.Current();
		if (solidShape.IsNull()) return;
		/*

		const TopoDS_Solid &onesolid = TopoDS::Solid(solidShape);*/
		GProp_GProps aSolidProps;
		
		BRepGProp::VolumeProperties(solidShape, aSolidProps);

		double volume = aSolidProps.Mass();
		if (volume < 0) return;
		_ui->disLabel_4->setText(QString::number(volume));

		/*TopoDS_Shape solidShape;
		GProp_GProps props;
		BRepGProp::VolumeProperties(solidShape, props);
		Standard_Real volume = props.Mass();*/
		
	}

	void MeasureDistanceDialog::ImHighlightPoints()
	{
		QList<QPair<Geometry::GeometrySet*, int>>::iterator it = _ptlist.begin();
		for (; it != _ptlist.end(); it++)
		{
			emit highLightGeometryPointSig(it->first, it->second, false);
		}
	}

	void MeasureDistanceDialog::ImHighlightCurve()
	{
		emit highLightGeometryEdgeSig(_curve.first, _curve.second, false);
	}

	void MeasureDistanceDialog::ImHighlightSurface()
	{
		emit highLightGeometryFaceSig(_surface.first, _surface.second, false);
	}

	void MeasureDistanceDialog::ImHighlightSolid()
	{
		emit highLightGeometrySolidSig(_solid.first, _solid.second, false);
	}


}