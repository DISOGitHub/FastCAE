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
		connect(_preWindow, SIGNAL(selectGeometry(vtkActor*, bool)), this, SLOT(selectGeometry(vtkActor*,bool)));
		connect(_preWindow, SIGNAL(preSelectGeometry(vtkActor*, QVector<double*>)), this, SLOT(preSelectGeometry(vtkActor*, QVector<double*>)));
		//connect(_preWindow, SIGNAL(rightDownMenuSig()), this, SLOT(rightDownCreateMenu()));
		connect(_preWindow, SIGNAL(setGeoSelectMode(int)), this, SLOT(setGeoSelectMode(int)));
		//
		connect(_mainWindow, SIGNAL(selectModelChangedSig(int)), this, SLOT(setGeoSelectMode(int)));
		connect(_mainWindow, SIGNAL(selectGeometryDisplay(bool, bool, bool)), this, SLOT(setGeometryDisplay(bool, bool, bool)));
		//新增
		connect(this, SIGNAL(geoShapeSelected(Geometry::GeometrySet*, int)), _preWindow, SIGNAL(geoShapeSelected(Geometry::GeometrySet*, int )));
		connect(_preWindow, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)), this, SLOT(highLightGeometrySet(Geometry::GeometrySet*, bool)));
		connect(_preWindow, SIGNAL(highLightGeometryPoint(Geometry::GeometrySet*, int, bool)), this, SLOT(highLightGeometryPoint(Geometry::GeometrySet*, int, bool)));
		connect(_preWindow, SIGNAL(highLightGeometryEdge(Geometry::GeometrySet*, int, bool)), this, SLOT(highLightGeometryEdge(Geometry::GeometrySet*, int, bool)));
		connect(_preWindow, SIGNAL(highLightGeometryFace(Geometry::GeometrySet*, int, bool)), this, SLOT(highLightGeometryFace(Geometry::GeometrySet*, int, bool)));
		connect(_preWindow, SIGNAL(highLightGeometrySolid(Geometry::GeometrySet*, int, bool)), this, SLOT(highLightGeometrySolid(Geometry::GeometrySet*, int, bool)));
		connect(_preWindow, SIGNAL(clearGeometryHighLight()), this, SLOT(clearAllHighLight()));
		connect(_preWindow, SIGNAL(clearAllHighLight()), this, SLOT(clearAllHighLight()));
		//
		auto gp = Setting::BusAPI::instance()->getGraphOption();
		_showvertex = gp->isShowGeoPoint();
		_showedge = gp->isShowGeoEdge();
		_showface = gp->isShowGeoSurface();
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
		//
		clearAllSet();
	}

	/*
	清空所有数据集合
	*/
	void GeometryViewProvider::clearAllSet()
	{
		 clearGeometryHighLight();
		_solidPolyDatas.clear();
		_actorPolydataHash.clear();
		_polydataTShapeSetHash.clear();
		_polydataShapeIdHash.clear();
		_polydataTShapeHash.clear();
		_selectPolydataActorHash.clear();
		_actorShapeHash.clear();
	}

	/*
	初始化所有实体和基准面的显示
	*/
	void GeometryViewProvider::init()
	{
		const int n = _geoData->getGeometrySetCount();
		for (int i = 0; i < n; ++i)
		{
			Geometry::GeometrySet* set = _geoData->getGeometrySetAt(i);
			TopoDS_Shape* shape = set->getShape();
			if (shape == nullptr) continue;
			showShape(*shape, set, false);
			_preWindow->resetCamera();
		}
		QList<Geometry::GeometryDatum*> dl = _geoData->getGeometryDatum();
		for (auto da : dl)
		{
			TopoDS_Shape* shape = da->getShape();
			if (shape == nullptr) continue;
			showDatum(da);
		}
	}  

	/*
	更新set的所有实体的状态
	*/
	void GeometryViewProvider::updateDiaplayStates(Geometry::GeometrySet* s, bool visibility)
	{
		clearSelectActors(s);
		QList<vtkActor*> acs = _setActors.values(s);
		for (int i = 0; i < acs.size(); ++i)
		{
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

	/*
	显示所有实体
	*/
	void GeometryViewProvider::showShape(TopoDS_Shape& shape, Geometry::GeometrySet* set, bool render)
	{
		
		showVertex(set);
		showEdge(set);
		showFace(set);
		fillSolid(set);
		if (render)
			_preWindow->resetCamera();
	}

	/*
	显示所有的点
	*/
	void GeometryViewProvider::showVertex(Geometry::GeometrySet* set)
	{
		TopoDS_Shape* shape = set->getShape();
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
		float size = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
		
		TopExp_Explorer ptExp(*shape, TopAbs_VERTEX);
		QList<Handle(TopoDS_TShape)> tshapelist;
		//组合过滤器
		vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
		vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
		//
		for (int index= 0; ptExp.More(); ptExp.Next(), ++index)
		{
			const TopoDS_Shape& s = ptExp.Current();
			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.contains(ts)) continue;
			tshapelist.append(ts);
			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);
			//删除重复点
			vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
			cleanFilter->SetInputConnection(DS->GetOutputPort());
			cleanFilter->Update();
			//单面数据获取
			vtkPolyData * polyData = cleanFilter->GetOutput();
			//单点数据的保存
			_actorPolydataHash.insert(Actor, polyData);
			//单对象的id序号保存
			_polydataShapeIdHash.insert(polyData, index);
			//
			appendFilter->AddInputData(polyData);
		}
		appendFilter->Update();
		//渲染展示
		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		Mapper->SetInputConnection(appendFilter->GetOutputPort());
		Actor->SetMapper(Mapper);
		Actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		Actor->GetProperty()->SetPointSize(size);
		Actor->GetProperty()->SetRepresentationToPoints();
		_vertexActors.append(Actor);
		_setActors.insert(set, Actor);
		bool v = set->isVisible();
		Actor->SetVisibility(v);
		if (v)
			Actor->SetVisibility(_showvertex);
		Actor->SetPickable(false);

		_preWindow->AppendActor(Actor, ModuleBase::D3, false);
	}

	/*
	显示所有的边
	*/
	void GeometryViewProvider::showEdge(Geometry::GeometrySet* set)
	{
		TopoDS_Shape* shape = set->getShape();
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
		float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();

		TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
		QList<Handle(TopoDS_TShape)> tshapelist;
		//组合过滤器
		vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
		vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
		//
		for (int index =0 ; edgeExp.More(); edgeExp.Next(), ++index)
		{
			const TopoDS_Shape& s = edgeExp.Current();
			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.contains(ts)) continue;
			tshapelist.append(ts);
			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);
			//删除重复点
			vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
			cleanFilter->SetInputConnection(DS->GetOutputPort());
			cleanFilter->Update();
			//单面数据获取
			vtkPolyData * polyData = cleanFilter->GetOutput();
			//单线段数据的保存
			_actorPolydataHash.insert(Actor, polyData);
			//单对象的id序号保存
			_polydataShapeIdHash.insert(polyData, index);
			//
			appendFilter->AddInputData(polyData);
		}
		appendFilter->Update();
		//渲染展示
		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		Mapper->SetInputConnection(appendFilter->GetOutputPort());
		Actor->SetMapper(Mapper);
		Actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		Actor->GetProperty()->SetLineWidth(width);
		Actor->GetProperty()->SetRepresentationToWireframe();
		_edgeActors.append(Actor);
		_setActors.insert(set, Actor);
		bool v = set->isVisible();
		Actor->SetVisibility(v);
		if (v)
			Actor->SetVisibility(_showedge);
		Actor->SetPickable(false);
		_preWindow->AppendActor(Actor, ModuleBase::D3, false);
	}

	/*
	显示所有的面
	*/
	void GeometryViewProvider::showFace(Geometry::GeometrySet* set)
	{
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
		int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
		TopoDS_Shape* shape = set->getShape();

		TopExp_Explorer faceExp(*shape, TopAbs_FACE);
		QList<Handle(TopoDS_TShape)> tshapelist;
		//组合过滤器
		vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
		vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
		for (int index = 0; faceExp.More(); faceExp.Next(), ++index)
		{
			const TopoDS_Shape& s = faceExp.Current();
			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.contains(ts)) continue;
			tshapelist.append(ts);

			vtkPolyData* polyData = vtkPolyData::New();
			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);
			//删除重复点
			vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
			cleanFilter->SetInputConnection(DS->GetOutputPort());
			cleanFilter->Update();
		 
			vtkPolyData* tpolydata = cleanFilter->GetOutput();
			const int np = tpolydata->GetNumberOfPoints();
			const int nc = tpolydata->GetNumberOfCells();

			vtkPoints* points = vtkPoints::New();
			for (int i = 0; i < np; i++)
			{
				double* coor = tpolydata->GetPoint(i);
				points->InsertNextPoint(coor);
			}
			polyData->SetPoints(points);

			polyData->SetPoints(tpolydata->GetPoints());

			vtkCellArray* cells = vtkCellArray::New();
			for (int i = 0; i < nc; ++i)
			{
				vtkCell* cell = tpolydata->GetCell(i);
				vtkIdList* ceid = cell->GetPointIds();
				if (ceid->GetNumberOfIds() == 3)
				{
					vtkTriangle*  triangle = vtkTriangle::New();
					triangle->DeepCopy(cell);
					cells->InsertNextCell(triangle);
				}
			}
			polyData->SetPolys(cells);

			vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
			normals->SetInputData(polyData);
			normals->FlipNormalsOn();
			normals->Update();
			polyData->DeepCopy(normals->GetOutput());
			
			//合体actor对应的单面数据
			_actorPolydataHash.insert(Actor, polyData);
			//单对象的对应的id序号
			_polydataShapeIdHash.insert(polyData, index);
			//但对象对应的handle对象数据
			_polydataTShapeHash.insert(polyData, ts);
			//
			appendFilter->AddInputData(polyData);
		}
		appendFilter->Update();
		//
		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		Mapper->SetInputConnection(appendFilter->GetOutputPort());
		Actor->SetMapper(Mapper);
		Actor->GetProperty()->SetRepresentationToSurface();
		Actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
		Actor->GetProperty()->SetOpacity(1.0 - trans/100);
		bool v = set->isVisible();
		Actor->SetVisibility(v);
		if (v)
			Actor->SetVisibility(_showface);
		_faceActors.append(Actor);
		_setActors.insert(set, Actor);
		 Actor->SetPickable(false);
		_preWindow->AppendActor(Actor, ModuleBase::D3, false);
	}

	/*
	填充所有实体（包含面的数据）
	*/
	void GeometryViewProvider::fillSolid(Geometry::GeometrySet* set)
	{
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
		int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
		TopoDS_Shape* shape = set->getShape();
		TopExp_Explorer solidExp(*shape, TopAbs_SOLID);
		QList<Handle(TopoDS_TShape)> tsshapelist;
		for (int index = 0; solidExp.More(); solidExp.Next(), ++index)
		{
			//实体的获取
			const TopoDS_Shape& solid = solidExp.Current();
			Handle(TopoDS_TShape) ts = solid.TShape();
			if (tsshapelist.contains(ts)) continue;
			tsshapelist.append(ts);
			//组合过滤器
			vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
			vtkPolyData* polyDataSet = vtkPolyData::New();
			//实体的面的获取
			TopExp_Explorer faceExp(solid, TopAbs_FACE);
			QList<Handle(TopoDS_TShape)> tshapelist;
			for (; faceExp.More();faceExp.Next())
			{
				const TopoDS_Shape& aface = faceExp.Current();
				Handle(TopoDS_TShape) tshape = aface.TShape();
				if (tshapelist.contains(tshape)) continue;
				tshapelist.append(tshape);
				//
				//单面数据获取
				_polydataTShapeSetHash.insert(polyDataSet, tshape);
				//
				vtkPolyData * polyData = vtkPolyData::New();
				polyData->DeepCopy(_polydataTShapeHash.key(tshape));
				appendFilter->AddInputData(polyData);
				//建立起hash
			}
			appendFilter->Update();
			polyDataSet->DeepCopy(appendFilter->GetOutput());
			//单对象的id序号保存
			_polydataShapeIdHash.insert(polyDataSet, index);
			_solidPolyDatas.insert(set, polyDataSet);
		}
	}
	
	/*
	清空所有实体对象并初始化更新
	*/
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
		clearAllSet();
		init();
	}

	/*
	更新所有actor的绘制属性
	*/
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
			ac->GetProperty()->SetOpacity(1.0 - trans / 100.00);
		}
	}

	/*
	清空删除set的所有对象
	*/
	void GeometryViewProvider::removeActors(Geometry::GeometrySet* set)
	{
		//清除所有高亮对象
		clearSelectActors(set);
		//
		QList<vtkActor*> setActors = _setActors.values(set);
		const int n = setActors.size();
		for (int i = 0; i < n; ++i)
		{
			vtkActor* ac = setActors.at(i);
			removeAllSetActors(set, ac);
			_preWindow->RemoveActor(ac);
			if (_vertexActors.contains(ac))
				_vertexActors.removeOne(ac);
			else if (_edgeActors.contains(ac))
				_edgeActors.removeOne(ac);
			else if (_faceActors.contains(ac))
				_faceActors.removeOne(ac);
		}
		_preWindow->reRender();
		_setActors.remove(set);
	}

	void GeometryViewProvider::removeAllSetActors(Geometry::GeometrySet* set, vtkActor* ac)
	{
		for (int i = 0; i < _solidPolyDatas.values(set).size(); i++)
		{
			_polydataTShapeSetHash.remove(_solidPolyDatas.values(set)[i]);
			_polydataShapeIdHash.remove(_solidPolyDatas.values(set)[i]);
		}
		for (int i = 0; i < _actorPolydataHash.values(ac).size(); i++)
		{
			_polydataShapeIdHash.remove(_actorPolydataHash.values(ac)[i]);
			if (_faceActors.contains(ac))
			{
				_polydataTShapeHash.remove(_actorPolydataHash.values(ac)[i]);
			}
		}
		_solidPolyDatas.remove(set);
		_actorPolydataHash.remove(ac);
	}

/*
	void GeometryViewProvider::transferFaceToPolyData(const TopoDS_Shape & s, vtkPolyData* polys)
	{
		TopoDS_Face theFace = TopoDS::Face(s);
		BRepAdaptor_Surface aFaceAdaptor(theFace);
		GeomAbs_SurfaceType aType = aFaceAdaptor.GetType();

		IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);

		if (aType == GeomAbs_Plane)
		{
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);
			
			vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
			cleanFilter->SetInputConnection(DS->GetOutputPort());
			cleanFilter->Update();

			vtkPolyData* tpolydata = cleanFilter->GetOutput();
			const int np = tpolydata->GetNumberOfPoints();
			const int nc = tpolydata->GetNumberOfCells();

			vtkPoints* points = vtkPoints::New();
			for (int i = 0; i < np; i++)
			{
				double* coor = tpolydata->GetPoint(i);
				points->InsertNextPoint(coor);
			}
			polys->SetPoints(points);

			polys->SetPoints(tpolydata->GetPoints());

			vtkCellArray* cells = vtkCellArray::New();
			for (int i = 0; i < nc; ++i)
			{
				vtkCell* cell = tpolydata->GetCell(i);
				vtkIdList* ceid = cell->GetPointIds();
				if (ceid->GetNumberOfIds() == 3)
				{
					vtkTriangle*  triangle = vtkTriangle::New();
					triangle->DeepCopy(cell);
					cells->InsertNextCell(triangle);
				}
			}
			polys->SetPolys(cells);

			vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
			normals->SetInputData(polys);
			normals->FlipNormalsOn();
			normals->Update();
			polys->DeepCopy(normals->GetOutput());
		}
		else
		{
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);

			vtkSmartPointer<IVtkTools_DisplayModeFilter> DMFilter = vtkSmartPointer<IVtkTools_DisplayModeFilter>::New();
			DMFilter->SetInputConnection(DS->GetOutputPort());
			DMFilter->SetDisplayMode(DM_Shading);

			vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
			normals->SetInputConnection(DMFilter->GetOutputPort());
			normals->FlipNormalsOn();
			normals->Update();
			polys->DeepCopy(normals->GetOutput());
		}
 
	}
*/

	/*
	设置选取模式
	*/
	void GeometryViewProvider::setGeoSelectMode(int m)
	{
		if (m > ModuleBase::GeometryPoint || m == -1 )
		{
			clearGeometryHighLight();
			return;
		}
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
	}

	void GeometryViewProvider::setPickable(QList<vtkActor*> acs, bool visibility)
	{
		for (int i = 0; i < acs.size(); ++i)
		{
			vtkActor* ac = acs.at(i);
			ac->SetPickable(visibility);
		}
	}
	
	/*
	 选中
	*/
	void GeometryViewProvider::selectGeometry(vtkActor* ac,bool ctrlpress)
	{
		Geometry::GeometrySet* set = _setActors.key(ac);
		if (set == nullptr)
		{
			clearPre_();
			return;
		}
		bool winselect = false;
		switch (_selectType)
		{
		case ModuleBase::GeometryWinPoint: winselect = true;
		case ModuleBase::GeometryPoint:
			if (!_vertexActors.contains(ac)) return;
				break;
		case ModuleBase::GeometryWinCurve:  winselect = true;
		case ModuleBase::GeometryCurve:
			if (!_edgeActors.contains(ac)) return;
				break;
		case ModuleBase::GeometryWinSurface: winselect = true;
		case ModuleBase::GeometrySurface:
			if (!_faceActors.contains(ac)) return;
				break;
		case ModuleBase::GeometryWinBody:winselect = true;
		case ModuleBase::GeometryBody:
			if (!_faceActors.contains(ac)) return;
				break;
		default:
			break;
		}
		if (winselect)
		{
			//判断已经被选中删除高亮
			QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//所有的选择的对象的polydata数据
			if (setSelectPolyData.contains(_pre.first))
			{
				showActor(_pre.first, false);
				int index = _polydataShapeIdHash.value(_pre.first);
				_selectItems.remove(set, index);
				emit geoShapeSelected(set, index);
				//清理预选择对象
				clearPre_();
				return;
			}
			if (_pre.second != nullptr)
			{
				//显示选中对象
				if (!ctrlpress)
				{
					clearGeometryHighLight();
				}
				showActor(_pre.first, true);
				//发送当前选择对象序号
				int index = _polydataShapeIdHash.value(_pre.first);
				_selectItems.insert(set, index);
				emit geoShapeSelected(set, index);
			}
		} else {
			if (_pre.second != nullptr)
			{
				RestoreGeoColor();
				int index = _polydataShapeIdHash.value(_pre.first);
				emit geoShapeSelected(set, index);
			}
		}
		//清理预选择对象
		clearPre_();
	}
   
	/*
	 清理预选取对象
	*/
	void GeometryViewProvider::clearPre_()
	{
		//清理预选择对象
		_preWindow->RemoveActor(_pre.second);
		_preWindow->reRender();
		_pre.second = nullptr;
		_pre.first = nullptr;
	}
	/*
	预选中
	*/
	void GeometryViewProvider::preSelectGeometry(vtkActor* ac, QVector<double*> points)
	{
		if (ac == nullptr)
		{
			if (_pre.second != nullptr)
			{
				//清理预选择对象
				clearPre_();
			}
			return;
		}
		//
		Geometry::GeometrySet* set = _setActors.key(ac);
		if (set == nullptr)
		{
			if (_pre.second != nullptr)
			{
				//清理预选择对象
				clearPre_();
			}
			return;
		}
		switch (_selectType)
		{
		case ModuleBase::GeometryWinPoint:
		case ModuleBase::GeometryPoint:
			if (!_vertexActors.contains(ac))  return;
				break;
		case ModuleBase::GeometryWinCurve: 
		case ModuleBase::GeometryCurve:
			if (!_edgeActors.contains(ac))  return;
				break;
		case ModuleBase::GeometryWinSurface: 
		case ModuleBase::GeometrySurface:
			if (!_faceActors.contains(ac)) return;
				break;
		case ModuleBase::GeometryWinBody: 
		case ModuleBase::GeometryBody:
			if (!_faceActors.contains(ac)) return;
				break;
		default:
			break;
		}
		vtkPolyData* currentPolyData = vtkPolyData::New();
		bool result = findSelectPolydata(ac, points, currentPolyData);
		if (!result)
		{
			return;
		}
		judgePreShowActor(currentPolyData);
	}

	/*
	判断并显示当前预选择对象
	*/
	bool GeometryViewProvider::judgePreShowActor(vtkPolyData* pd)
	{
		if (pd == nullptr)
		{
			return  false;
		}
		//
		//和当前的预选择对象相同时
		if (pd == _pre.first)
		{
			return false;
		}
		if (_pre.second!= nullptr)
		{
			//清理预选择对象
			clearPre_();
		}
		//判断是否已经被选中
		QColor preHighLight = Setting::BusAPI::instance()->getGraphOption()->getPreHighLightColor();
		QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//所有的选择的对象的polydata数据
		if (setSelectPolyData.contains(pd))
		{
		     preHighLight = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		}
		else {
			 preHighLight = Setting::BusAPI::instance()->getGraphOption()->getPreHighLightColor();
		}
		//显示
		float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
		float size = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
		int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
		//渲染
		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
		Actor->SetMapper(Mapper);
		Actor->GetProperty()->SetColor(preHighLight.redF(), preHighLight.greenF(), preHighLight.blueF());
		Actor->GetProperty()->SetDiffuseColor(preHighLight.redF(), preHighLight.greenF(), preHighLight.blueF());
		Mapper->SetInputData(pd);
		switch (_selectType)
		{
		case ModuleBase::GeometryWinPoint://点显示
		case ModuleBase::GeometryPoint:
			Actor->GetProperty()->SetPointSize(size);
			Actor->GetProperty()->SetRepresentationToPoints();
			break;
		case ModuleBase::GeometryWinCurve://边显示
		case ModuleBase::GeometryCurve:
			Actor->GetProperty()->SetLineWidth(width);
			Actor->GetProperty()->SetRepresentationToWireframe();
			break;
		case ModuleBase::GeometryWinSurface://面显示
		case ModuleBase::GeometrySurface:
			Actor->GetProperty()->SetOpacity(100);
			Actor->GetProperty()->SetRepresentationToSurface();
			break;
		case ModuleBase::GeometryWinBody:
		case ModuleBase::GeometryBody:
			Actor->GetProperty()->SetOpacity(100);
			Actor->GetProperty()->SetRepresentationToSurface();
			break;
		default:
			break;
		}
		 Actor->SetPickable(false);
		_pre.second = Actor;
		_pre.first = pd;
		_preWindow->AppendActor(_pre.second, ModuleBase::D3, false);
		_preWindow->reRender();
		return true;
	}


	/*
	查找选中的Polydata
	*/
	bool GeometryViewProvider::findSelectPolydata(vtkActor * ac, QVector<double*> points, vtkPolyData * &findPolydata)
	{
		bool result = false;
		switch (_selectType)
		{
		case ModuleBase::GeometryWinPoint:
		case ModuleBase::GeometryPoint:
		{
			result = findCurrentPointPolydata(ac, points, findPolydata);
			break;
		}	
		case ModuleBase::GeometryWinCurve:
		case ModuleBase::GeometryCurve:
		{
			float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
			double minDis = 1000;
			result = findCurrentLinePolydata(ac, points, minDis, findPolydata);
			break;
		}
		case ModuleBase::GeometryWinSurface:
		case ModuleBase::GeometrySurface:
		{
			result = findcurrentFacePolydata(ac, points, findPolydata);
			break;
		}
		case ModuleBase::GeometryWinBody:
		case ModuleBase::GeometryBody:
		{
			vtkPolyData * facePolydata;
			result = findcurrentFacePolydata(ac, points, facePolydata);
			if (result)
			{
				result = findcurrentSolidPolydata(facePolydata, findPolydata);
			}
			break;
		}
		default:
			break;
		}
		return result;
	}

	/*
	 找到当前拾取到的点
	*/
	bool GeometryViewProvider::findCurrentPointPolydata(vtkActor * ac, QVector<double*> points, vtkPolyData*&findPolydata)
	{
		QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(ac);
		bool result = false;
		for (int i = 0; i < polyDatas.size(); ++i)
		{
			vtkPolyData* polyData = polyDatas.at(i);
			for (vtkIdType index = 0; index < polyData->GetNumberOfPoints(); index++)
			{
				double pt[3], p0[3];
				polyData->GetPoint(index, p0);
				pt[0] = points[0][0];
				pt[1] = points[0][1];
				pt[2] = points[0][2];
				if (pt[0] == p0[0] &&
					pt[1] == p0[1] &&
					pt[2] == p0[2])
				{
					findPolydata = polyData;
					return true;
				}
			}
		}
		return false;
	}

	/*
	   找到当前拾取到的边
	*/
	bool GeometryViewProvider::findCurrentLinePolydata(vtkActor * ac, QVector<double*> points, double &minDis, vtkPolyData* &findPolyData)
	{
		double pt[3];
		pt[0] = points[0][0];
		pt[1] = points[0][1];
		pt[2] = points[0][2];
		QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(ac);
		int currentId = -1;
		for (int index = 0; index < polyDatas.size(); ++index)
		{
			vtkPolyData* polyData = polyDatas.at(index);
			if (polyData == nullptr)
			{
				continue;
			}
			for (vtkIdType i = 0; i < polyData->GetNumberOfLines(); i++)
			{
				vtkIdList* pts = vtkIdList::New();
				polyData->GetLines()->GetCell(i, pts);
				if (pts->GetNumberOfIds() < 2)
				{
					continue;
				}
				for (vtkIdType j = 0; j < pts->GetNumberOfIds() - 1; j++)
				{
					vtkIdType number0 = pts->GetId(j);
					vtkIdType number1 = pts->GetId(j + 1);
					double lineP0[3], lineP1[3];
					polyData->GetPoint(number0, lineP0);
					polyData->GetPoint(number1, lineP1);
					double t;
					double closest[3];
					vtkLine::DistanceToLine(pt, lineP0, lineP1, t, closest); //最近点，即点到线的垂直投影点
					// 实际距离
					double squaredDistance = sqrt(vtkMath::Distance2BetweenPoints(pt, closest));
					if (squaredDistance < minDis)
					{
						minDis = squaredDistance;
						currentId = index;
					}
				}
			}
		}
		if (currentId != -1)
		{
			findPolyData = polyDatas.at(currentId);
			return true;
		}
		return false;
	}

	/*
	找到当前拾取的面
	*/
	bool GeometryViewProvider::findcurrentFacePolydata(vtkActor *ac, QVector<double*> points, vtkPolyData* &findPolydata)
	{
		QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(ac);
		bool result = false;
		for (int index = 0; index < polyDatas.size(); ++index)
		{
			vtkPolyData* polyData = polyDatas.at(index);
			bool status[10];
			for (int i = 0; i < 10; i++)
			{
				status[i] = false;
			}
			int state = 0;
			for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++)
			{
				double p0[3];
				polyData->GetPoint(i, p0);
				for (int j = 0; j < points.size(); j++)
				{
					if (status[j] == true)
					{
						continue;
					}
					if (p0[0]== points[j][0] && 
						p0[1] == points[j][1] &&
						p0[2] == points[j][2])
					{
						state ++;
						status[j] = true;
					}
				}
				if (state == points.size())
				{
					findPolydata = polyData;
					return true;
				}
			}    
		}
		return false;
	}

	/*
	找到当前面对应的实体
	*/
	bool GeometryViewProvider::findcurrentSolidPolydata(vtkPolyData* facePolydata, vtkPolyData* &findPolydata)
	{
		Handle(TopoDS_TShape) ts = _polydataTShapeHash.value(facePolydata);
		QList<vtkPolyData *> vtkPolydataSet = _polydataTShapeSetHash.keys();
		for (int i = 0; i < vtkPolydataSet.size(); i++)
		{
			QList<Handle(TopoDS_TShape)> tShapeSet = _polydataTShapeSetHash.values(vtkPolydataSet[i]);
			for (int j = 0; j < tShapeSet.size(); j++)
			{
				if (ts == tShapeSet[j])
				{
					findPolydata = vtkPolydataSet[i];
					return true;
				}
			}
		}
		return false;
	}

	/*
	 对于主体的高亮显示
	*/
	void GeometryViewProvider::highLightGeometrySet(Geometry::GeometrySet* s, bool on)
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

	//实体的高亮显示
	void GeometryViewProvider::highLightGeometrySolid(Geometry::GeometrySet* s, int id, bool on)
	{
		vtkPolyData* pd = getSolidPolyData(s, id);
		if (pd == nullptr) return;
		if (on)
		{
			_selectItems.insert(s, id);
		}
		else {
			_selectItems.remove(s, id);
		}
		showActor(pd, on);
		//清理预选择对象
		clearPre_();
	}

	vtkPolyData* GeometryViewProvider::getSolidPolyData(Geometry::GeometrySet* s, int id)
	{
		QList<vtkPolyData*> idPlolydata = _polydataShapeIdHash.keys(id);//根据id找polydata的集合
		QList<vtkPolyData*> setPolydatas = _solidPolyDatas.values(s);//根据set找到actor的集合
		vtkPolyData* pd = nullptr;
		for (int i = 0; i < setPolydatas.size(); ++i)
		{
			vtkPolyData * p = setPolydatas.at(i);
			bool ok = idPlolydata.contains(p);//判断是否属于实体actor
			if (ok)
			{
				pd = p;
				break;
			}
		}
		return pd;
	}
	/*
	点的高亮显示
	*/
	void GeometryViewProvider::highLightGeometryPoint(Geometry::GeometrySet* s, int id, bool on)
	{
		vtkPolyData* pd = getPointPolyData(s, id);
		if (pd == nullptr) return;
		if (on)
		{
			_selectItems.insert(s, id);
		}
		else {
			_selectItems.remove(s, id);
		}
		showActor(pd, on);
		//清理预选择对象
		clearPre_();
	}

	vtkPolyData* GeometryViewProvider::getPointPolyData(Geometry::GeometrySet* s, int id)
	{
		QList<vtkPolyData*> idPlolydata = _polydataShapeIdHash.keys(id);//根据id找polydata的集合
		QList<vtkActor*> setActors = _setActors.values(s);//根据set找到actor的集合
		vtkPolyData* pd = nullptr;
		for (int i = 0; i < setActors.size(); ++i)
		{
			vtkActor* a = setActors.at(i);
			bool ok = _vertexActors.contains(a);//判断是否属于线actor
			if (ok)
			{
				QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(a);
				for (int i = 0; i < polyDatas.size(); ++i)
				{
					vtkPolyData * polyData = polyDatas.at(i);
					if (idPlolydata.contains(polyData))//判断当前单线是否属于id对应集合里的polydata
					{
						pd = polyData;
						break;
					}
				}
			}
		}
		return pd;
	}
	/*
	边的高亮显示
	*/
	void GeometryViewProvider::highLightGeometryEdge(Geometry::GeometrySet* s, int id, bool on)
	{
		vtkPolyData* pd = getEdgePolyData(s, id);
		if (pd == nullptr) return;
		if (on)
		{
			_selectItems.insert(s, id);
		}
		else {
			_selectItems.remove(s, id);
		}
		showActor(pd, on);
		//清理预选择对象
		clearPre_();
	}

	vtkPolyData* GeometryViewProvider::getEdgePolyData(Geometry::GeometrySet* s, int id)
	{
		QList<vtkPolyData*> idPlolydata = _polydataShapeIdHash.keys(id);//根据id找polydata的集合
		QList<vtkActor*> setActors = _setActors.values(s);//根据set找到actor的集合
		vtkPolyData* pd = nullptr;
		for (int i = 0; i < setActors.size(); ++i)
		{
			vtkActor* a = setActors.at(i);
			bool ok = _edgeActors.contains(a);//判断是否属于线actor
			if (ok)
			{
				QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(a);
				for (int i = 0; i < polyDatas.size(); ++i)
				{
					vtkPolyData * polyData = polyDatas.at(i);
					if (idPlolydata.contains(polyData))//判断当前单线是否属于id对应集合里的polydata
					{
						pd = polyData;
						break;
					}
				}
			}
		}
		return pd;
	}

	/*
	面的高亮显示
	*/
	void GeometryViewProvider::highLightGeometryFace(Geometry::GeometrySet* s, int id, bool on)
	{
		vtkPolyData* pd  = getFacePolyData(s, id);
		if (pd == nullptr) return;
		if (on)
		{
			_selectItems.insert(s, id);
		}
		else {
			_selectItems.remove(s, id);
		}
		showActor(pd, on);
		//清理预选择对象
		clearPre_();
	}

	vtkPolyData* GeometryViewProvider::getFacePolyData(Geometry::GeometrySet* s, int id)
	{
		QList<vtkPolyData*> idPlolydata = _polydataShapeIdHash.keys(id);//根据id找polydata的集合
		QList<vtkActor*> setActors = _setActors.values(s);//根据set找到actor的集合
		vtkPolyData* pd = nullptr;
		for (int i = 0; i < setActors.size(); ++i)
		{
			vtkActor* a = setActors.at(i);
			bool ok = _faceActors.contains(a);//判断是否属于线actor
			if (ok)
			{
				QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(a);
				for (int i = 0; i < polyDatas.size(); ++i)
				{
					vtkPolyData * polyData = polyDatas.at(i);
					if (idPlolydata.contains(polyData))//判断当前单线是否属于id对应集合里的polydata
					{
						pd = polyData;
						break;
					}
				}
			}
		}
		return pd;
	}

	/*
	添加或删除actor
	*/
	void GeometryViewProvider::showActor(vtkPolyData* pd , bool state)
	{
		if (state)
		{
			QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//所有的选择的对象的polydata数据
			if (setSelectPolyData.contains(pd))
			{
				return;
			}
			//显示
			QColor colorhigh = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
			float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
			float size = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
			int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
			//渲染
			vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
			vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
			normals->SetInputData(pd);
			normals->FlipNormalsOn();
			Mapper->SetInputConnection(normals->GetOutputPort());
			Actor->SetMapper(Mapper);
			Actor->GetProperty()->SetColor(colorhigh.redF(), colorhigh.greenF(), colorhigh.blueF());
			//
			switch (_selectType)
			{
			case ModuleBase::GeometryWinPoint://点显示
			case ModuleBase::GeometryPoint:
				Actor->GetProperty()->SetPointSize(size);
				Actor->GetProperty()->SetRepresentationToPoints();
				break;
			case ModuleBase::GeometryWinCurve://边显示
			case ModuleBase::GeometryCurve:
				Actor->GetProperty()->SetLineWidth(width);
				Actor->GetProperty()->SetRepresentationToWireframe();
				break;
			case ModuleBase::GeometryWinSurface://面显示
			case ModuleBase::GeometrySurface:
				Actor->GetProperty()->SetOpacity(1.0 - trans / 100);
				Actor->GetProperty()->SetRepresentationToSurface();
				break;
			case ModuleBase::GeometryWinBody:
			case ModuleBase::GeometryBody:
				Actor->GetProperty()->SetOpacity(1.0 - trans / 100);
				Actor->GetProperty()->SetRepresentationToSurface();
				break;
			default:
				break;
			}
			Actor->SetPickable(false);
			_selectPolydataActorHash.insert(pd, Actor);
			_preWindow->AppendActor(Actor, ModuleBase::D3, false);
		} else {
			QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//所有的选择的对象的polydata数据
			if (!setSelectPolyData.contains(pd))
			{
				return;
			}
			vtkActor * ac = _selectPolydataActorHash.value(pd);
			_preWindow->RemoveActor(ac);
			_selectPolydataActorHash.remove(pd);
		}
		_preWindow->reRender();
	}

	void GeometryViewProvider::clearAllHighLight()
	{
		//清理预选择对象
		clearPre_();
		clearGeometryHighLight();
	}

	/*
	清空所有的已经高亮显示对象
	*/
	void GeometryViewProvider::clearGeometryHighLight()
	{
		RestoreGeoColor();
		QList<vtkActor *> setSelectActor = _selectPolydataActorHash.values();//所有的选择的对象的actor数据
		for (int i = 0; i < setSelectActor.size(); i++)
		{
			vtkActor * ac = setSelectActor.at(i);
			_preWindow->RemoveActor(ac);
			_preWindow->reRender();
		}
		_selectPolydataActorHash.clear();
		_selectItems.clear();
	}

	/*
	清空当前主体的已经高亮对象
	*/
	void GeometryViewProvider::clearSelectActors(Geometry::GeometrySet* s)
	{
		RestoreGeoColor();
		QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//所有的选择的对象的polydata数据
		QList<vtkActor*> setActors = _setActors.values(s);//根据set找到actor的集合
		for (int i = 0; i < setActors.size(); ++i)
		{
			vtkActor* ac = setActors.at(i);
			QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(ac);
			for (int i = 0; i < polyDatas.size(); ++i)
			{
				vtkPolyData * polyData = polyDatas.at(i);
				if (setSelectPolyData.contains(polyData))
				{
					vtkActor * ac = _selectPolydataActorHash.value(polyData);
					_preWindow->RemoveActor(ac);
					_preWindow->reRender();
					_selectPolydataActorHash.remove(polyData);
				}
			}
		}
		_selectItems.clear();
	}

	void GeometryViewProvider::removeDatumActors(Geometry::GeometryDatum* plane)
	{
		Geometry::DatumType type = plane->getDatumType();
		QList<vtkActor*> setActors = _setActors.values(plane);

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

		_setActors.remove(plane);
		_preWindow->reRender();

	}
	/*
	设置基准面显示模式
	*/
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

	/*
	显示基准平面
	*/
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
			//
			_faceActors.append(Actor);
			//actor对应的面数据
			_actorPolydataHash.insert(Actor, Mapper->GetInput());
			//对象的对应的id序号
			_polydataShapeIdHash.insert(Mapper->GetInput(), index);
			//
			_setActors.insert(datum, Actor);

		     //
			_actorShapeHash.insert(Actor, index);
			Actor->SetPickable(false);
			Actor->SetVisibility(visible);
			_preWindow->AppendActor(Actor, ModuleBase::D3, false);
		}
		_preWindow->reRender();
	}
 
	/*
	设置点线体（面）的显示与隐藏
	*/
	void GeometryViewProvider::setGeometryDisplay( bool v, bool c, bool f)
	{
		clearGeometryHighLight();
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

	/*void GeometryViewProvider::rightDownCreateMenu()
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
		QMultiHash<Geometry::GeometrySet*, int>::iterator it = _selectItems.begin();
		for (; it != _selectItems.end(); it++)
		{

			Geometry::GeometrySet* set = it.key();
			int index = it.value();
			/ *for (auto set : _selectItems.keys())
			{
			for (int index : _selectItems.values(set))
			{* /
			switch (_selectType)
			{
			case ModuleBase::GeometryWinPoint:
			{
				hideGeometry(set, _vertexActors, index);
				break;
			}
			case ModuleBase::GeometryWinCurve:
			{
				hideGeometry(set, _edgeActors, index);
				break;
			}
			case ModuleBase::GeometryWinSurface:
			{
				hideGeometry(set, _faceActors, index);
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

			/ *}* /
		}
		
		_preWindow->reRender();
		clearGeometryHighLight();
	}*/

	void GeometryViewProvider::hideGeometry(Geometry::GeometrySet* set, QList<vtkActor*> actors,int geoindex)
	{
		QList<vtkPolyData*> idPolydata = _polydataShapeIdHash.keys(geoindex);//根据id找polydata的集合
		vtkActor*currentActor = nullptr;
		vtkPolyData*currentPolydata = nullptr;
		for (int i = 0; i < idPolydata.size(); i++)
		{
			vtkPolyData* polydata = idPolydata[i];
			vtkActor* actor = _actorPolydataHash.key(polydata);
			if (polydata == nullptr || actor == nullptr)
			{
				continue;
			}
			if (actors.contains(actor))
			{
				currentActor = actor;
				currentPolydata = polydata;
				break;
			}
		}
		if (currentActor == nullptr || currentPolydata == nullptr)
		{
			return;
		}
		QList<vtkPolyData*> polydatas = _actorPolydataHash.values(currentActor);
		//组合过滤器
		vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
		for (int i = 0; polydatas.size();i++)
		{
			vtkPolyData* polydata = polydatas[i];
			if (polydata == currentPolydata)
			{
				continue;
			}
			if (polydata == nullptr)
			{
				continue;
			}
			appendFilter->AddInputData(polydata);
		}
		appendFilter->Update();
		currentActor->GetMapper()->SetInputConnection(appendFilter->GetOutputPort());
	}

	/*
	还原所有对象颜色并清空高亮显示
	*/
	void GeometryViewProvider::RestoreGeoColor()
	{
		//还原所有actor的颜色
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
		for (auto var : _faceActors)
		{
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
			int tp = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
			var->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
			var->GetProperty()->SetOpacity(1.0 - tp / 100.0);
		}
		_preWindow->reRender();
	}

	/*
	显示set的所有对象
	*/
	void GeometryViewProvider::showGeoSet(Geometry::GeometrySet* set, bool render)
	{
		clearSelectActors(set);
		TopoDS_Shape* shape = set->getShape();
		showShape(*shape, set, render);
	}

	/*
	 显示selectItems的所有对象
	*/
	void GeometryViewProvider::slotShowGeometryAll()
	{
		if (_selectItems.size() < 1) return;
		_hasShowed = false;
		for (auto set : _selectItems.keys())
			showGeoSet(set);
		_preWindow->reRender();
	}

	QMultiHash<Geometry::GeometrySet*, int>* GeometryViewProvider::getGeoSelectItems()
	{
		return &_selectItems;
	}

	void GeometryViewProvider::setGeoSelectItems(QMultiHash<Geometry::GeometrySet*, int> items)
	{
		_selectItems = items;
	}

}
