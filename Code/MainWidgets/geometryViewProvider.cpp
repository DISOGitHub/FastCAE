#include "geometryViewProvider.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/preWindow.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryDatum.h"
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include <QColor>
#include <QList>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Iterator.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <QDebug>
#include <TopExp.hxx>
#include <QMenu>


namespace MainWidget
{
	GeometryViewProvider::GeometryViewProvider(GUI::MainWindow* mainwindow, PreWindow* renderWin):
		_mainWindow(mainwindow), _preWindow(renderWin)
	{
		_geoData = Geometry::GeometryData::getInstance();
		connect(_preWindow, SIGNAL(showGeoSet(Geometry::GeometrySet*,bool)), this, SLOT(showGeoSet(Geometry::GeometrySet*,bool)));
		connect(_preWindow, SIGNAL(showDatum(Geometry::GeometryDatum*)), this, SLOT(showDatum(Geometry::GeometryDatum*)));
		connect(_preWindow, SIGNAL(removeGemoActors(Geometry::GeometrySet*)), this, SLOT(removeActors(Geometry::GeometrySet*)));
		connect(_preWindow, SIGNAL(removeGeoDatumActors(Geometry::GeometryDatum*)), this, SLOT(removeDatumActors(Geometry::GeometryDatum*)));
		connect(_mainWindow, SIGNAL(selectModelChangedSig(int)), this, SLOT(setGeoSelectMode(int)));
		connect(_preWindow, SIGNAL(selectGeometry(vtkActor*,bool)), this, SLOT(selectGeometry(vtkActor*,bool)));
		connect(_mainWindow, SIGNAL(highLightGeometrySetSig(Geometry::GeometrySet*, bool)), this, SLOT(highLightGeoset(Geometry::GeometrySet*, bool)));
		connect(_mainWindow, SIGNAL(highLightGeometryPointSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)), this, SLOT(highLightGeoPoint(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
		connect(_mainWindow, SIGNAL(highLightGeometryEdgeSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)), this, SLOT(highLightGeoEdge(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
		connect(_mainWindow, SIGNAL(highLightGeometryFaceSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)), this, SLOT(highLightGeoFace(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
		connect(_mainWindow, SIGNAL(selectGeometryDisplay(bool, bool, bool)), this, SLOT(setGeometryDisplay(bool,bool,bool)));
		connect(_preWindow, SIGNAL(rightDownMenuSig()), this, SLOT(rightDownCreateMenu()));
// 		connect(_mainWindow, SIGNAL(selectGeoActiveSig(bool)), this, SLOT(activeSelectGeo(bool)));
// 		
// 		connect(_mainWindow, SIGNAL(selectGeoCloseSig(int)), this, SLOT(closeSelectGeo(int)));

		auto gp = Setting::BusAPI::instance()->getGraphOption();
		_showvertex = gp->isShowGeoPoint();
		_showedge = gp->isShowGeoEdge();
		_showface = gp->isShowGeoSurface();
		//connet(_mainWindow, SIGNAL(RestoreGeoSig(),this,SLOT(RestoreGeo))
//		init();
	}

	GeometryViewProvider::~GeometryViewProvider()
	{
		int c = _vertexActors.size();
		for (int i = 0; i < c; ++i)
		{
			vtkActor* ac = _vertexActors.at(i);
			_preWindow->RemoveActor(ac);
		}
		c = _edgeActors.size();
		for (int i = 0; i < c; ++i)
		{
			vtkActor* ac = _edgeActors.at(i);
			_preWindow->RemoveActor(ac);
		}
		c = _faceActors.size();
		for (int i = 0; i < c; ++i)
		{
			vtkActor* ac = _faceActors.at(i);
			_preWindow->RemoveActor(ac);
		}
		_faceActors.clear();
		_edgeActors.clear();
		_vertexActors.clear();
		_setActors.clear();
	}

	void GeometryViewProvider::init()
	{
		const int n = _geoData->getGeometrySetCount();
		for (int i = 0; i < n; ++i)
		{
			Geometry::GeometrySet* set = _geoData->getGeometrySetAt(i);
			TopoDS_Shape* shape = set->getShape();
			if (shape == nullptr) continue;
			showShape(*shape, set);

		}
		QList<Geometry::GeometryDatum*> dl = _geoData->getGeometryDatum();
		for (auto da : dl)
		{
			TopoDS_Shape* shape = da->getShape();
			if (shape == nullptr) continue;
			showDatum(da);
		}
	}  

	void GeometryViewProvider::showShape(TopoDS_Shape& shape, Geometry::GeometrySet* set, bool render)
	{
		showVertex(set);
		showEdge(set);
		showFace(set);
		if (render)
			_preWindow->resetCamera();
		
	}

	void GeometryViewProvider::showVertex(Geometry::GeometrySet* set)
	{
		TopoDS_Shape* shape = set->getShape();
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
		float size = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
		
		TopExp_Explorer ptExp(*shape, TopAbs_VERTEX);
		QList<Handle(TopoDS_TShape)> tshapelist;
		for (int index= 0; ptExp.More(); ptExp.Next(), ++index)
		{
			TopoDS_Shape s = ptExp.Current();
			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.contains(ts)) continue;
			tshapelist.append(ts);

			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);

			vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			Mapper->SetInputConnection(DS->GetOutputPort());
			vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
			Actor->SetMapper(Mapper);

			Actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			Actor->GetProperty()->SetPointSize(size);
			Actor->GetProperty()->SetRepresentationToPoints();
			_vertexActors.append(Actor);
			_actotShapeHash.insert(Actor, index);
			_setActors.insert(set, Actor);
			bool v = set->isVisible();
			Actor->SetVisibility(v);
			if (v)
				Actor->SetVisibility(_showvertex);
			Actor->SetPickable(false);

			_preWindow->AppendActor(Actor, ModuleBase::D3, false);

		}
	}
	void GeometryViewProvider::showEdge(Geometry::GeometrySet* set)
	{
		TopoDS_Shape* shape = set->getShape();
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
		float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();

		TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
		QList<Handle(TopoDS_TShape)> tshapelist;
		for (int index =0 ; edgeExp.More(); edgeExp.Next(), ++index)
		{
			TopoDS_Shape s = edgeExp.Current();

			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.contains(ts)) continue;
			tshapelist.append(ts);

			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);

			vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			Mapper->SetInputConnection(DS->GetOutputPort());
			vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
			Actor->SetMapper(Mapper);

			Actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			Actor->GetProperty()->SetLineWidth(width);
			Actor->GetProperty()->SetRepresentationToWireframe();
			_edgeActors.append(Actor);
			_actotShapeHash.insert(Actor,index);
			_setActors.insert(set, Actor);
		
			bool v = set->isVisible();
			Actor->SetVisibility(v);
			if (v)
				Actor->SetVisibility(_showedge);
			Actor->SetPickable(false);
			_preWindow->AppendActor(Actor, ModuleBase::D3, false);
		}

	}

	void GeometryViewProvider::showFace(Geometry::GeometrySet* set)
	{
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
		int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
		TopoDS_Shape* shape = set->getShape();

		TopExp_Explorer faceExp(*shape, TopAbs_FACE);
		QList<Handle(TopoDS_TShape)> tshapelist;
		for (int index =0 ; faceExp.More(); faceExp.Next(), ++index)
		{
			TopoDS_Shape s = faceExp.Current();

			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);

			vtkSmartPointer<IVtkTools_DisplayModeFilter> DMFilter = vtkSmartPointer<IVtkTools_DisplayModeFilter>::New();
			DMFilter->AddInputConnection(DS->GetOutputPort());
			DMFilter->SetDisplayMode(DM_Shading);

			vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
			normals->SetInputConnection(DMFilter->GetOutputPort());
			normals->FlipNormalsOn();

			vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			Mapper->SetInputConnection(normals->GetOutputPort());
			vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
			Actor->SetMapper(Mapper);
			Actor->GetProperty()->SetRepresentationToSurface();
			Actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			Actor->GetProperty()->SetOpacity(1.0-trans / 100.0);
			bool v = set->isVisible();
			Actor->SetVisibility(v);
			if (v)
				Actor->SetVisibility(_showface);
			_faceActors.append(Actor);
			_setActors.insert(set, Actor);
			_actotShapeHash.insert(Actor, index);
			Actor->SetPickable(false);
			_preWindow->AppendActor(Actor,ModuleBase::D3,false);
		}
	}
	
	void GeometryViewProvider::updateGeoActors()
	{
		int c = _vertexActors.size();
		for (int i = 0; i < c; ++i)
		{
			vtkActor* ac = _vertexActors.at(i);
			_preWindow->RemoveActor(ac);
		}
		c = _edgeActors.size();
		for (int i = 0; i < c; ++i)
		{
			vtkActor* ac = _edgeActors.at(i);
			_preWindow->RemoveActor(ac);
		}
		c = _faceActors.size();
		for (int i = 0; i < c; ++i)
		{
			vtkActor* ac = _faceActors.at(i);
			_preWindow->RemoveActor(ac);
		}
		_faceActors.clear();
		_edgeActors.clear();
		_vertexActors.clear();
		_setActors.clear();
		init();
	}

	void GeometryViewProvider::updateGraphOption()
	{
		QColor sc = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
		QColor cc = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
		QColor pc = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
		int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();

		float ps = Setting::BusAPI::instance()->getGraphOption()->getGeoPointSize();
		float cw = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();

		int n = _vertexActors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* ac = _vertexActors.at(i);
			ac->GetProperty()->SetColor(pc.redF(), pc.greenF(), pc.blueF());
			ac->GetProperty()->SetPointSize(ps);
		}
		n = _edgeActors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* ac = _edgeActors.at(i);
			ac->GetProperty()->SetColor(cc.redF(), cc.greenF(), cc.blueF());
			ac->GetProperty()->SetLineWidth(cw);
		}
		n = _faceActors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* ac = _faceActors.at(i);
			ac->GetProperty()->SetColor(sc.redF(), sc.greenF(), sc.blueF());
			ac->GetProperty()->SetOpacity(1.0-trans / 100.00);
		}
	}

	void GeometryViewProvider::showGeoSet(Geometry::GeometrySet* set, bool render)
	{
		TopoDS_Shape* shape = set->getShape();
		showShape(*shape, set,render);
	}

	void GeometryViewProvider::showDatum(Geometry::GeometryDatum* datum)
	{
		Geometry::DatumType type = datum->getDatumType();
		switch (type)
		{
		case Geometry::DatumPoint:
			break;
		case Geometry::DatumAxis:
			break;
		case Geometry::DatumPlane:
			showDatumPlane(datum); break;
		default:
			break;
		}
	}

	void GeometryViewProvider::removeActors(Geometry::GeometrySet* set)
	{
		QList<vtkActor*> setActors = _setActors.values(set);
		
		const int n = setActors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* ac = setActors.at(i);
			_preWindow->RemoveActor(ac);

			if (_vertexActors.contains(ac))
				_vertexActors.removeOne(ac);
			else if (_edgeActors.contains(ac))
				_edgeActors.removeOne(ac);
			else if (_faceActors.contains(ac))
				_faceActors.removeOne(ac);
			_actotShapeHash.remove(ac);
		}
		_preWindow->reRender();
		_setActors.remove(set);
	}
	void GeometryViewProvider::removeDatumActors(Geometry::GeometryDatum* plane)
	{
		Geometry::DatumType type = plane->getDatumType();
		QList<vtkActor*> setActors = _datumActors.values(plane);

		const int n = setActors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* ac = setActors.at(i);
			_preWindow->RemoveActor(ac);
			switch (type)
			{
			case Geometry::DatumPoint:
				_vertexActors.removeOne(ac); break;
			case  Geometry::DatumAxis:
				_edgeActors.removeOne(ac); break;
			case Geometry::DatumPlane:
				_faceActors.removeOne(ac); break;
			default:
				break;
			}
		}

		_datumActors.remove(plane);
		_preWindow->reRender();
		
	}

	void GeometryViewProvider::updateDiaplayStates(Geometry::GeometrySet* s, bool visibility)
	{
		QList<vtkActor*> acs = _setActors.values(s);
		for (int i = 0; i < acs.size(); ++i)
		{
//			acs.at(i)->SetVisibility(visibility);
			vtkActor* ac = acs.at(i);
			if (!visibility)
			{
				ac->SetVisibility(false); 
				continue;
			}
			if (_faceActors.contains(ac)) ac->SetVisibility(_showface);
			if (_edgeActors.contains(ac)) ac->SetVisibility(_showedge);
			if (_vertexActors.contains(ac)) ac->SetVisibility(_showvertex);
		}
		_preWindow->reRender();
	}

	void GeometryViewProvider::setGeoSelectMode(int m)
	{
		_selectItems.clear();
		if (m > ModuleBase::GeometryPoint) return;
		_selectType = (ModuleBase::SelectModel)m;
		
		this->setPickable(_vertexActors, false);
		this->setPickable(_edgeActors, false);
		this->setPickable(_faceActors, false);

		switch (_selectType)
		{
		case ModuleBase::GeometryBody:
		case ModuleBase::GeometryWinBody:
			this->setPickable(_faceActors, true);
			break;
		case ModuleBase::GeometrySurface:
		case ModuleBase::GeometryWinSurface:
			this->setPickable(_faceActors, true);
			break;
		case ModuleBase::GeometryCurve:
		case ModuleBase::GeometryWinCurve:
			this->setPickable(_edgeActors, true);
			break;
		case ModuleBase::GeometryPoint:
		case ModuleBase::GeometryWinPoint:
			this->setPickable(_vertexActors, true);
			break;
		default:
			break;
		}

		RestoreGeoColor();
	}

	void GeometryViewProvider::setPickable(QList<vtkActor*> acs, bool visibility)
	{
		for (int i = 0; i < acs.size(); ++i)
		{
			vtkActor* ac = acs.at(i);
			ac->SetPickable(visibility);
		}
	}

	void GeometryViewProvider::selectGeometry(vtkActor* ac,bool ctrlpress)
	{
	
		int shape = -1;
		bool winselect = false;
		switch (_selectType)
		{
		case ModuleBase::GeometryWinPoint: winselect = true;
		case ModuleBase::GeometryPoint:
			if (!_vertexActors.contains(ac)) return;
			shape = _actotShapeHash.value(ac);
			break;
		case ModuleBase::GeometryWinCurve:  winselect = true;
		case ModuleBase::GeometryCurve:
			if (!_edgeActors.contains(ac)) return;
			shape = _actotShapeHash.value(ac);
			break;
		case ModuleBase::GeometryWinSurface: winselect = true;
		case ModuleBase::GeometrySurface:
			if (!_faceActors.contains(ac)) return;
			shape = _actotShapeHash.value(ac);
			break;
		case ModuleBase::GeometryWinBody : winselect = true;
			break;
		default:
			break;
		}
		Geometry::GeometrySet* set = _setActors.key(ac);
		if (set != nullptr)
		{
			emit _preWindow->selectGeoActorShape(ac, shape, set);
		//	_preWindow->reRender();
		}

		if (!winselect)  return;

		for(auto var : _vertexActors)
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
			var->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		}
		for (auto var : _edgeActors)
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
			var->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		}
		for (auto var : _faceActors)
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
			var->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		}
			
		if (set == nullptr) return;

		if (!ctrlpress)
		{
			_selectItems.clear();
			_addActors.clear();
			_selectItems.insert(set, shape);
			_addActors.append(ac);
		}
		else
		{
			if (_addActors.contains(ac))
			{
				_addActors.removeOne(ac);
				_selectItems.remove(set, shape);
			}
			else
			{
				_selectItems.insert(set, shape);
				_addActors.append(ac);
			}
		}

		QColor colorhigh = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		if (_selectType == ModuleBase::GeometryBody || _selectType == ModuleBase::GeometryWinBody)
		{
			QList<Geometry::GeometrySet*> setList = _selectItems.uniqueKeys();
			for (auto set : setList)
			{
				QList<vtkActor*> setactors = _setActors.values(set);
				for (auto var : setactors)
				{
					var->GetProperty()->SetColor(colorhigh.redF(), colorhigh.greenF(), colorhigh.blueF());
				}
			}

		}
		else
		{
			for (auto actor : _addActors)
			{
				actor->GetProperty()->SetColor(colorhigh.redF(), colorhigh.greenF(), colorhigh.blueF());
			}
				
		}
			
		
		_preWindow->reRender();
	}






	void GeometryViewProvider::highLightGeoset(Geometry::GeometrySet* s, bool on)
	{
		if (s == nullptr) return;
		QList<vtkActor*> actors = _setActors.values(s);
		if (!on)
		{
			QColor color;
			for (int i = 0; i < actors.size(); ++i)
			{
				vtkActor* ac = actors.at(i);
				if (_vertexActors.contains(ac))
					color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
				else if (_edgeActors.contains(ac))
					color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
				else if (_faceActors.contains(ac))
					color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
				else return;

				ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			}
		}
		else
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
			for (int i = 0; i < actors.size(); ++i)
			{
				vtkActor* ac = actors.at(i);
				ac->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			}
		}
		_preWindow->reRender();
	}

	void GeometryViewProvider::showDatumPlane(Geometry::GeometryDatum* datum)
	{
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
		TopoDS_Shape* shape = datum->getShape();
		bool visible = datum->isVisible();
		visible = visible && _showface;
		TopExp_Explorer faceExp(*shape, TopAbs_FACE);
		QList<Handle(TopoDS_TShape)> tshapelist;
		for (int index = 0; faceExp.More(); faceExp.Next(), ++index)
		{
			TopoDS_Shape s = faceExp.Current();

			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);

			vtkSmartPointer<IVtkTools_DisplayModeFilter> DMFilter = vtkSmartPointer<IVtkTools_DisplayModeFilter>::New();
			DMFilter->AddInputConnection(DS->GetOutputPort());
			DMFilter->SetDisplayMode(DM_Shading);

		
			vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			Mapper->SetInputConnection(DMFilter->GetOutputPort());
			vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
			Actor->SetMapper(Mapper);
			Actor->GetProperty()->SetRepresentationToSurface();
			Actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			Actor->GetProperty()->SetOpacity(0.6);

			_faceActors.append(Actor);
			_datumActors.insert(datum, Actor);
			_setActors.insert(datum, Actor);
			_actotShapeHash.insert(Actor, index);
			Actor->SetPickable(false);
			Actor->SetVisibility(visible);
			_preWindow->AppendActor(Actor, ModuleBase::D3, false);
		}
		_preWindow->reRender();
	}

	void GeometryViewProvider::highLightGeoEdge(Geometry::GeometrySet* s, int id, QList<vtkActor*>* acs)
	{
		QList<vtkActor*> setActors = _setActors.values(s);
		QList<vtkActor*> idActors = _actotShapeHash.keys(id);
		
		vtkActor* ac = nullptr;
		for (int i = 0; i < idActors.size(); ++i)
		{
			vtkActor* a = idActors.at(i);
			bool ok = setActors.contains(a);
			ok = _edgeActors.contains(a);
			if (setActors.contains(a) && _edgeActors.contains(a))
			{
				ac = a;
				break;
			}
		}
		if (ac == nullptr) return;
		acs->append(ac);
		QColor c = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		ac->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
		_preWindow->reRender();
	}

	void GeometryViewProvider::highLightGeoPoint(Geometry::GeometrySet* s, int id, QList<vtkActor*>* acs)
	{
		QList<vtkActor*> setActors = _setActors.values(s);
		QList<vtkActor*> idActors = _actotShapeHash.keys(id);

		vtkActor* ac = nullptr;
		for (int i = 0; i < idActors.size(); ++i)
		{
			vtkActor* a = idActors.at(i);
			bool ok = setActors.contains(a);
			ok = _edgeActors.contains(a);
			if (setActors.contains(a) && _vertexActors.contains(a))
			{
				ac = a;
				break;
			}
		}
		if (ac == nullptr) return;
		acs->append(ac);
		QColor c = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		ac->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
		_preWindow->reRender();
	}

	void GeometryViewProvider::highLightGeoFace(Geometry::GeometrySet* s, int id, QList<vtkActor*>* acs)
	{
		QList<vtkActor*> setActors = _setActors.values(s);
		QList<vtkActor*> idActors = _actotShapeHash.keys(id);

		vtkActor* ac = nullptr;
		for (int i = 0; i < idActors.size(); ++i)
		{
			vtkActor* a = idActors.at(i);
			bool ok = setActors.contains(a);
			ok = _edgeActors.contains(a);
			if (setActors.contains(a) && _faceActors.contains(a))
			{
				ac = a;
				break;
			}
		}
		if (ac == nullptr) return;
		acs->append(ac);
		QColor c = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		ac->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
		ac->GetProperty()->SetOpacity(1.0);
		_preWindow->reRender();
	}
 
	void GeometryViewProvider::setGeometryDisplay( bool v, bool c, bool f)
	{
		auto gp = Setting::BusAPI::instance()->getGraphOption();
		_showvertex = gp->isShowGeoPoint();
		_showedge = gp->isShowGeoEdge();
		_showface = gp->isShowGeoSurface();
		for (auto var : _vertexActors) var->SetVisibility(v);
		for (auto var : _edgeActors) var->SetVisibility(c);
		for (auto var : _faceActors) var->SetVisibility(f);

		QList<Geometry::GeometrySet*> setList = _setActors.keys();
		for (auto set : setList)
		{
			bool v = set->isVisible();
			if (v) continue;
			QList<vtkActor*> acs = _setActors.values(set);
			for (auto ac : acs) ac->SetVisibility(false);
		}
		_preWindow->reRender();
	}

	void GeometryViewProvider::rightDownCreateMenu()
	{
		QMenu *pMenu = new QMenu(_preWindow);
		QAction *pNewTask = new QAction(tr("Hide"), this);
		pMenu->addAction(pNewTask);
		connect(pNewTask, SIGNAL(triggered()), this, SLOT(slotHideGeometry()));
		QAction *pShowTask = new QAction(tr("Show All"), this);
		pMenu->addAction(pShowTask);
		pShowTask->setEnabled(_hasShowed);
		connect(pShowTask, SIGNAL(triggered()), this, SLOT(slotShowGeometryAll()));
		
		pMenu->exec(QCursor::pos());
	
	}

	void GeometryViewProvider::slotHideGeometry()
	{
		if (_selectItems.size() < 1) return;
 		_hasShowed = true;
		for (auto set : _selectItems.keys())
		{
			for (int index : _selectItems.values(set))
			{
				switch (_selectType)
				{
					case ModuleBase::GeometryWinPoint:
					{
						vtkActor* ac = _vertexActors.at(index);
						ac->SetVisibility(false);
						break;
					}
					case ModuleBase::GeometryWinCurve:
					{
						vtkActor* ac = _edgeActors.at(index);
						ac->SetVisibility(false);
						break;
					}
					case ModuleBase::GeometryWinSurface:
					{
						vtkActor* ac = _faceActors.at(index);
						ac->SetVisibility(false);
						break;
					}
					case ModuleBase::GeometryWinBody:
					{
						QList<vtkActor*> setActors = _setActors.values(set);
						for (auto ac : setActors) 
							ac->SetVisibility(false);
						break;
					}
					default:  
						break;
					}

				}
			}
		_preWindow->reRender();
	}

	void GeometryViewProvider::slotShowGeometryAll()
	{
		if (_selectItems.size() < 1) return;
		_hasShowed = false;
		for (auto set : _selectItems.keys())
			showGeoSet(set);
		_preWindow->reRender();
	}

	void GeometryViewProvider::RestoreGeoColor()
	{
		for (auto var : _vertexActors)
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
			var->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		}
		for (auto var : _edgeActors)
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
			var->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		}
		for (auto var : _faceActors)
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
			int tp = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
			var->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			var->GetProperty()->SetOpacity(1.0 - tp / 100.0);
		}
		_preWindow->reRender();
		_selectItems.clear();
	}

// 	void GeometryViewProvider::activeSelectGeo(bool on)
// 	{
// 		_activeSeletGeo = on;
// 	}
// 
// 	void GeometryViewProvider::closeSelectGeo(int geomodel)
// 	{
// 		if (geomodel>-1)
// 		{
// 			_activeSeletGeo = false;
// 		}
// 	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryViewProvider::getGeoSelectItems()
	{
		return _selectItems;
	}

	void GeometryViewProvider::setGeoSelectItems(QMultiHash<Geometry::GeometrySet*, int> items)
	{
		_selectItems = items;
	}

}
