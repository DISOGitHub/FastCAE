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
#include <vtkCleanPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkGenericCell.h>
#include <vtkPolyLine.h>
#include <vtkPolyVertex.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkVertex.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkPlane.h>
#include <vtkMath.h>
#include <vtkSelectEnclosedPoints.h>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Iterator.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkIdTypeArray.h>
#include <vtkRemoveDuplicatePolys.h>
#include <BRepAdaptor_Surface.hxx>
#include <IVtkOCC_ShapeMesher.hxx>
#include <vtkTriangle.h>
#include <QDebug>
#include <TopExp.hxx>
#include <QMenu>
#include "geometryViewData.h"
#include "geometryViewObject.h"


namespace MainWidget
{
	GeometryViewProvider::GeometryViewProvider(GUI::MainWindow* mainwindow, PreWindow* renderWin) :
		_mainWindow(mainwindow), _preWindow(renderWin)
	{
		_geoData = Geometry::GeometryData::getInstance();
		connect(_preWindow, SIGNAL(showGeoSet(Geometry::GeometrySet*, bool)), this, SLOT(showGeoSet(Geometry::GeometrySet*, bool)));
		connect(_preWindow, SIGNAL(showDatum(Geometry::GeometryDatum*)), this, SLOT(showDatum(Geometry::GeometryDatum*)));
		connect(_preWindow, SIGNAL(removeGemoActors(Geometry::GeometrySet*)), this, SLOT(removeActors(Geometry::GeometrySet*)));
		connect(_preWindow, SIGNAL(removeGeoDatumActors(Geometry::GeometryDatum*)), this, SLOT(removeDatumActors(Geometry::GeometryDatum*)));
		connect(_preWindow, SIGNAL(selectGeometry(bool)), this, SLOT(selectGeometry(bool)));
		connect(_preWindow, SIGNAL(preSelectGeometry(vtkActor*, int)), this, SLOT(preSelectGeometry(vtkActor*, int)));
		connect(_preWindow, SIGNAL(setGeoSelectMode(int)), this, SLOT(setGeoSelectMode(int)));
		//
		connect(_mainWindow, SIGNAL(selectModelChangedSig(int)), this, SLOT(setGeoSelectMode(int)));
		connect(_mainWindow, SIGNAL(selectGeometryDisplay(bool, bool, bool)), this, SLOT(setGeometryDisplay(bool, bool, bool)));
		//新增
		connect(this, SIGNAL(geoShapeSelected(Geometry::GeometrySet*, int)), _preWindow, SIGNAL(geoShapeSelected(Geometry::GeometrySet*, int)));
		connect(_preWindow, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)), this, SLOT(highLightGeometrySet(Geometry::GeometrySet*, bool)));
		connect(_preWindow, SIGNAL(highLightGeometryPoint(Geometry::GeometrySet*, int, bool)), this, SLOT(highLightGeometryPoint(Geometry::GeometrySet*, int, bool)));
		connect(_preWindow, SIGNAL(highLightGeometryEdge(Geometry::GeometrySet*, int, bool)), this, SLOT(highLightGeometryEdge(Geometry::GeometrySet*, int, bool)));
		connect(_preWindow, SIGNAL(highLightGeometryFace(Geometry::GeometrySet*, int, bool)), this, SLOT(highLightGeometryFace(Geometry::GeometrySet*, int, bool)));
		connect(_preWindow, SIGNAL(highLightGeometrySolid(Geometry::GeometrySet*, int, bool)), this, SLOT(highLightGeometrySolid(Geometry::GeometrySet*, int, bool)));
		connect(_preWindow, SIGNAL(clearGeometryHighLight()), this, SLOT(clearAllHighLight()));
		connect(_preWindow, SIGNAL(clearAllHighLight()), this, SLOT(clearAllHighLight()));
		_viewData = new GeometryViewData;
		this->init();
	}

	GeometryViewProvider::~GeometryViewProvider()
	{
		if (_viewData != nullptr)
			delete _viewData;
	}

	void GeometryViewProvider::updateGeoActors()
	{

	}

	void GeometryViewProvider::updateGraphOption()
	{
		_viewData->updateGraphOption();
		QList<GeoViewObj> vieobjs = _geoViewHash.values();
		int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
		float ps = Setting::BusAPI::instance()->getGraphOption()->getGeoPointSize();
		float cw = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
		for (GeoViewObj object : vieobjs)
		{
			vtkActor* actor = nullptr;
			actor = object._faceObj.first;
			if (actor != nullptr) actor->GetProperty()->SetOpacity(1.0 - trans / 100.00);
			actor = object._edgeObj.first;
			if (actor != nullptr) actor->GetProperty()->SetLineWidth(cw);
			actor = object._pointObj.first;
			if (actor != nullptr) actor->GetProperty()->SetLineWidth(ps);
		}
		_preWindow->reRender();
	}

	void GeometryViewProvider::updateDiaplayStates(Geometry::GeometrySet* s, bool visibility)
	{
		if (!_geoViewHash.contains(s)) return;
		GeoViewObj setviewObj = _geoViewHash.value(s);
		vtkActor* ac = nullptr;
		ac = setviewObj._faceObj.first;
		if (ac != nullptr) ac->SetVisibility(visibility);
		ac = setviewObj._edgeObj.first;
		if (ac != nullptr) ac->SetVisibility(visibility);
		ac = setviewObj._pointObj.first;
		if (ac != nullptr) ac->SetVisibility(visibility);
		_preWindow->reRender();
	}

	QMultiHash<Geometry::GeometrySet*, int> GeometryViewProvider::getGeoSelectItems()
	{
		ModuleBase::SelectModel mod = _preWindow->getSelectModel();
		QList<GeometryViewObject*> views = _viewData->getViewObjectByStates((int)mod, (int)GeometryViewObject::HighLigh);
		QMultiHash<Geometry::GeometrySet*, int> setHash{};
		for (auto vs : views)
		{
			auto set = vs->getGeometySet();
			int index = vs->getIndex();
			setHash.insert(set, index);
		}
		return setHash;
	}

	void GeometryViewProvider::showGeoSet(Geometry::GeometrySet* set, bool render /*= true*/)
	{
		QList<vtkPolyData*> viewPolys = _viewData->transferToPoly(set);
		vtkPolyData* facePoly = viewPolys.at(0);
		vtkPolyData* edgePoly = viewPolys.at(1);
		vtkPolyData* pointPoly = viewPolys.at(2);
		GeoViewObj viewObj;
		if (facePoly != nullptr)
		{
			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper->SetInputData(facePoly);
			actor->SetMapper(mapper);
			bool vis = set->isVisible();
			bool show = Setting::BusAPI::instance()->getGraphOption()->isShowGeoSurface();
			actor->SetVisibility(show && vis);
			actor->SetPickable(false);
			actor->GetProperty()->SetRepresentationToSurface();
			_preWindow->AppendActor(actor, ModuleBase::D3, false);
			viewObj._faceObj = QPair<vtkActor*, vtkPolyData*>(actor, facePoly);
		}
		if (edgePoly != nullptr)
		{
			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper->SetInputData(edgePoly);
			actor->SetMapper(mapper);
			bool vis = set->isVisible();
			bool show = Setting::BusAPI::instance()->getGraphOption()->isShowGeoEdge();
			actor->SetVisibility(show && vis);
			actor->SetPickable(false);
			actor->GetProperty()->SetRepresentationToWireframe();
			//			actor->GetProperty()->EdgeVisibilityOn();
			float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
			actor->GetProperty()->SetLineWidth(width);
			_preWindow->AppendActor(actor, ModuleBase::D3, false);
			viewObj._edgeObj = QPair<vtkActor*, vtkPolyData*>(actor, edgePoly);
		}
		if (pointPoly != nullptr)
		{
			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			float size = Setting::BusAPI::instance()->getGraphOption()->getGeoPointSize();
			mapper->SetInputData(pointPoly);
			actor->SetMapper(mapper);
			bool vis = set->isVisible();
			bool show = Setting::BusAPI::instance()->getGraphOption()->isShowGeoPoint();
			actor->SetVisibility(show && vis);
			actor->SetPickable(false);
			actor->GetProperty()->SetRepresentationToPoints();
			actor->GetProperty()->SetPointSize(size);
			_preWindow->AppendActor(actor, ModuleBase::D3, false);
			viewObj._pointObj = QPair<vtkActor*, vtkPolyData*>(actor, pointPoly);
		}
		_geoViewHash.insert(set, viewObj);
		if (render)
			_preWindow->resetCamera();
	}

	void GeometryViewProvider::showDatum(Geometry::GeometryDatum* datm)
	{

	}

	void GeometryViewProvider::removeActors(Geometry::GeometrySet* set)
	{
		if (!_geoViewHash.contains(set)) return;
		GeoViewObj views = _geoViewHash.value(set);
		_geoViewHash.remove(set);
		vtkActor* ac = views._faceObj.first;
		_preWindow->RemoveActor(ac);
		ac = views._edgeObj.first;
		_preWindow->RemoveActor(ac);
		ac = views._pointObj.first;
		_preWindow->RemoveActor(ac);
		_preWindow->reRender();
		_viewData->removeViewObjs(set);
	}

	void GeometryViewProvider::setGeometryDisplay(bool v, bool c, bool f)
	{
		QList<Geometry::GeometrySet*> setList = _geoViewHash.keys();
		for (auto set : setList)
		{
			bool sv = set->isVisible();
			GeoViewObj viewObj = _geoViewHash.value(set);
			vtkActor* ac = nullptr;
			ac = viewObj._faceObj.first; if (ac != nullptr)  ac->SetVisibility(f && sv);
			ac = viewObj._edgeObj.first; if (ac != nullptr)  ac->SetVisibility(c && sv);
			ac = viewObj._pointObj.first; if (ac != nullptr)  ac->SetVisibility(v && sv);
		}
		_preWindow->reRender();
	}

	void GeometryViewProvider::setGeoSelectMode(int m)
	{
		_viewData->updateGraphOption();
		ModuleBase::SelectModel selectType = (ModuleBase::SelectModel)m;
		QList<GeoViewObj> viewObjs = _geoViewHash.values();
		vtkActor* actor = nullptr;
		for (GeoViewObj vobj : viewObjs)
		{
			actor = vobj._faceObj.first; if (actor != nullptr) actor->SetPickable(false);
			actor = vobj._edgeObj.first; if (actor != nullptr) actor->SetPickable(false);
			actor = vobj._pointObj.first; if (actor != nullptr) actor->SetPickable(false);
			switch (selectType)
			{
			case ModuleBase::GeometryBody:
			case ModuleBase::GeometrySurface:
			case ModuleBase::GeometryWinBody:
			case ModuleBase::GeometryWinSurface:
				actor = vobj._faceObj.first; if (actor != nullptr) actor->SetPickable(true); break;
			case ModuleBase::GeometryWinCurve:
			case ModuleBase::GeometryCurve:
				actor = vobj._edgeObj.first; if (actor != nullptr) actor->SetPickable(true); break;
			case ModuleBase::GeometryWinPoint:
			case ModuleBase::GeometryPoint:
				actor = vobj._pointObj.first; if (actor != nullptr) actor->SetPickable(true); break;
			default: break;
			}
		}
	}

	void GeometryViewProvider::selectGeometry(bool ctrlpress)
	{
		auto p = _viewData->getPreHighLightObj();
		if (p == nullptr)return;
		ModuleBase::SelectModel sm = _preWindow->getSelectModel();
		if ((int)sm >= (int)ModuleBase::GeometryWinBody && (int)sm <= (int)ModuleBase::GeometryWinPoint)
		{
			if (!ctrlpress) updateGraphOption();
			p->highLight();
			_viewData->preHighLight(nullptr);
		}
		else
		{
			auto set = p->getGeometySet();
			int index = p->getIndex();
			_viewData->preHighLight(nullptr);
			emit geoShapeSelected(set, index);
		}
	}

	void GeometryViewProvider::preSelectGeometry(vtkActor* ac, int index)
	{
		if (ac == nullptr || index < 0)
		{
			_viewData->preHighLight(nullptr);
			_preWindow->reRender();
			return;
		}

		ModuleBase::SelectModel selectMod = _preWindow->getSelectModel();
		vtkDataSet* dataSet = ac->GetMapper()->GetInputAsDataSet();
		vtkPolyData *poly = vtkPolyData::SafeDownCast(dataSet);
		if (poly == nullptr) return;
		GeometryViewObject *vobj = nullptr;
		switch (selectMod)
		{
		case ModuleBase::GeometryWinBody:
		case ModuleBase::GeometryBody:
			vobj = _viewData->getSolidViewObj(poly, index); break;
		case ModuleBase::GeometryWinSurface:
		case ModuleBase::GeometrySurface:
			vobj = _viewData->getFaceViewObj(poly, index); break;
		case ModuleBase::GeometryWinCurve:
		case ModuleBase::GeometryCurve:
			vobj = _viewData->getEdgeViewObj(poly, index); break;
		case ModuleBase::GeometryWinPoint:
		case ModuleBase::GeometryPoint:
			vobj = _viewData->getPointViewObj(poly, index); break;
		default: break;
		}
		if (vobj == nullptr)  return;
		_viewData->preHighLight(vobj);
		_preWindow->reRender();
	}

	void GeometryViewProvider::clearAllHighLight()
	{
		_viewData->updateGraphOption();
	}

	void GeometryViewProvider::highLightGeometrySet(Geometry::GeometrySet* s, bool on)
	{
		_viewData->highLight(s, on);
		_preWindow->reRender();
	}

	void GeometryViewProvider::highLightGeometryFace(Geometry::GeometrySet* s, int id, bool on)
	{
		_viewData->highLightFace(s, id, on);
		_preWindow->reRender();
	}

	void GeometryViewProvider::highLightGeometryEdge(Geometry::GeometrySet* s, int id, bool on)
	{
		_viewData->highLightEdge(s, id, on);
		_preWindow->reRender();
	}

	void GeometryViewProvider::highLightGeometryPoint(Geometry::GeometrySet* s, int id, bool on)
	{
		_viewData->highLightPoint(s, id, on);
		_preWindow->reRender();
	}

	void GeometryViewProvider::highLightGeometrySolid(Geometry::GeometrySet* s, int id, bool on)
	{
		_viewData->highLightSolid(s, id, on);
		_preWindow->reRender();
	}

	void GeometryViewProvider::init()
	{
		const int n = _geoData->getGeometrySetCount();
		for (int i = 0; i < n; ++i)
		{
			Geometry::GeometrySet* set = _geoData->getGeometrySetAt(i);
			TopoDS_Shape* shape = set->getShape();
			if (shape == nullptr) continue;
			showGeoSet(set, false);
		}
		_preWindow->resetCamera();
		QList<Geometry::GeometryDatum*> dl = _geoData->getGeometryDatum();
		for (auto da : dl)
		{
			TopoDS_Shape* shape = da->getShape();
			if (shape == nullptr) continue;
			showDatum(da);
		}
	}

}
