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
		//����
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
	����������ݼ���
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
	��ʼ������ʵ��ͻ�׼�����ʾ
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
	����set������ʵ���״̬
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
	��ʾ����ʵ��
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
	��ʾ���еĵ�
	*/
	void GeometryViewProvider::showVertex(Geometry::GeometrySet* set)
	{
		TopoDS_Shape* shape = set->getShape();
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryPointColor();
		float size = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
		
		TopExp_Explorer ptExp(*shape, TopAbs_VERTEX);
		QList<Handle(TopoDS_TShape)> tshapelist;
		//��Ϲ�����
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
			//ɾ���ظ���
			vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
			cleanFilter->SetInputConnection(DS->GetOutputPort());
			cleanFilter->Update();
			//�������ݻ�ȡ
			vtkPolyData * polyData = cleanFilter->GetOutput();
			//�������ݵı���
			_actorPolydataHash.insert(Actor, polyData);
			//�������id��ű���
			_polydataShapeIdHash.insert(polyData, index);
			//
			appendFilter->AddInputData(polyData);
		}
		appendFilter->Update();
		//��Ⱦչʾ
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
	��ʾ���еı�
	*/
	void GeometryViewProvider::showEdge(Geometry::GeometrySet* set)
	{
		TopoDS_Shape* shape = set->getShape();
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometryCurveColor();
		float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();

		TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
		QList<Handle(TopoDS_TShape)> tshapelist;
		//��Ϲ�����
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
			//ɾ���ظ���
			vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
			cleanFilter->SetInputConnection(DS->GetOutputPort());
			cleanFilter->Update();
			//�������ݻ�ȡ
			vtkPolyData * polyData = cleanFilter->GetOutput();
			//���߶����ݵı���
			_actorPolydataHash.insert(Actor, polyData);
			//�������id��ű���
			_polydataShapeIdHash.insert(polyData, index);
			//
			appendFilter->AddInputData(polyData);
		}
		appendFilter->Update();
		//��Ⱦչʾ
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
	��ʾ���е���
	*/
	void GeometryViewProvider::showFace(Geometry::GeometrySet* set)
	{
		QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
		int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
		TopoDS_Shape* shape = set->getShape();

		TopExp_Explorer faceExp(*shape, TopAbs_FACE);
		QList<Handle(TopoDS_TShape)> tshapelist;
		//��Ϲ�����
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
			//ɾ���ظ���
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
			
			//����actor��Ӧ�ĵ�������
			_actorPolydataHash.insert(Actor, polyData);
			//������Ķ�Ӧ��id���
			_polydataShapeIdHash.insert(polyData, index);
			//�������Ӧ��handle��������
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
	�������ʵ�壨����������ݣ�
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
			//ʵ��Ļ�ȡ
			const TopoDS_Shape& solid = solidExp.Current();
			Handle(TopoDS_TShape) ts = solid.TShape();
			if (tsshapelist.contains(ts)) continue;
			tsshapelist.append(ts);
			//��Ϲ�����
			vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
			vtkPolyData* polyDataSet = vtkPolyData::New();
			//ʵ�����Ļ�ȡ
			TopExp_Explorer faceExp(solid, TopAbs_FACE);
			QList<Handle(TopoDS_TShape)> tshapelist;
			for (; faceExp.More();faceExp.Next())
			{
				const TopoDS_Shape& aface = faceExp.Current();
				Handle(TopoDS_TShape) tshape = aface.TShape();
				if (tshapelist.contains(tshape)) continue;
				tshapelist.append(tshape);
				//
				//�������ݻ�ȡ
				_polydataTShapeSetHash.insert(polyDataSet, tshape);
				//
				vtkPolyData * polyData = vtkPolyData::New();
				polyData->DeepCopy(_polydataTShapeHash.key(tshape));
				appendFilter->AddInputData(polyData);
				//������hash
			}
			appendFilter->Update();
			polyDataSet->DeepCopy(appendFilter->GetOutput());
			//�������id��ű���
			_polydataShapeIdHash.insert(polyDataSet, index);
			_solidPolyDatas.insert(set, polyDataSet);
		}
	}
	
	/*
	�������ʵ����󲢳�ʼ������
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
	��������actor�Ļ�������
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
	���ɾ��set�����ж���
	*/
	void GeometryViewProvider::removeActors(Geometry::GeometrySet* set)
	{
		//������и�������
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
	����ѡȡģʽ
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
	 ѡ��
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
			//�ж��Ѿ���ѡ��ɾ������
			QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//���е�ѡ��Ķ����polydata����
			if (setSelectPolyData.contains(_pre.first))
			{
				showActor(_pre.first, false);
				int index = _polydataShapeIdHash.value(_pre.first);
				_selectItems.remove(set, index);
				emit geoShapeSelected(set, index);
				//����Ԥѡ�����
				clearPre_();
				return;
			}
			if (_pre.second != nullptr)
			{
				//��ʾѡ�ж���
				if (!ctrlpress)
				{
					clearGeometryHighLight();
				}
				showActor(_pre.first, true);
				//���͵�ǰѡ��������
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
		//����Ԥѡ�����
		clearPre_();
	}
   
	/*
	 ����Ԥѡȡ����
	*/
	void GeometryViewProvider::clearPre_()
	{
		//����Ԥѡ�����
		_preWindow->RemoveActor(_pre.second);
		_preWindow->reRender();
		_pre.second = nullptr;
		_pre.first = nullptr;
	}
	/*
	Ԥѡ��
	*/
	void GeometryViewProvider::preSelectGeometry(vtkActor* ac, QVector<double*> points)
	{
		if (ac == nullptr)
		{
			if (_pre.second != nullptr)
			{
				//����Ԥѡ�����
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
				//����Ԥѡ�����
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
	�жϲ���ʾ��ǰԤѡ�����
	*/
	bool GeometryViewProvider::judgePreShowActor(vtkPolyData* pd)
	{
		if (pd == nullptr)
		{
			return  false;
		}
		//
		//�͵�ǰ��Ԥѡ�������ͬʱ
		if (pd == _pre.first)
		{
			return false;
		}
		if (_pre.second!= nullptr)
		{
			//����Ԥѡ�����
			clearPre_();
		}
		//�ж��Ƿ��Ѿ���ѡ��
		QColor preHighLight = Setting::BusAPI::instance()->getGraphOption()->getPreHighLightColor();
		QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//���е�ѡ��Ķ����polydata����
		if (setSelectPolyData.contains(pd))
		{
		     preHighLight = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
		}
		else {
			 preHighLight = Setting::BusAPI::instance()->getGraphOption()->getPreHighLightColor();
		}
		//��ʾ
		float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
		float size = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
		int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
		//��Ⱦ
		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
		Actor->SetMapper(Mapper);
		Actor->GetProperty()->SetColor(preHighLight.redF(), preHighLight.greenF(), preHighLight.blueF());
		Actor->GetProperty()->SetDiffuseColor(preHighLight.redF(), preHighLight.greenF(), preHighLight.blueF());
		Mapper->SetInputData(pd);
		switch (_selectType)
		{
		case ModuleBase::GeometryWinPoint://����ʾ
		case ModuleBase::GeometryPoint:
			Actor->GetProperty()->SetPointSize(size);
			Actor->GetProperty()->SetRepresentationToPoints();
			break;
		case ModuleBase::GeometryWinCurve://����ʾ
		case ModuleBase::GeometryCurve:
			Actor->GetProperty()->SetLineWidth(width);
			Actor->GetProperty()->SetRepresentationToWireframe();
			break;
		case ModuleBase::GeometryWinSurface://����ʾ
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
	����ѡ�е�Polydata
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
	 �ҵ���ǰʰȡ���ĵ�
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
	   �ҵ���ǰʰȡ���ı�
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
					vtkLine::DistanceToLine(pt, lineP0, lineP1, t, closest); //����㣬���㵽�ߵĴ�ֱͶӰ��
					// ʵ�ʾ���
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
	�ҵ���ǰʰȡ����
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
	�ҵ���ǰ���Ӧ��ʵ��
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
	 ��������ĸ�����ʾ
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

	//ʵ��ĸ�����ʾ
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
		//����Ԥѡ�����
		clearPre_();
	}

	vtkPolyData* GeometryViewProvider::getSolidPolyData(Geometry::GeometrySet* s, int id)
	{
		QList<vtkPolyData*> idPlolydata = _polydataShapeIdHash.keys(id);//����id��polydata�ļ���
		QList<vtkPolyData*> setPolydatas = _solidPolyDatas.values(s);//����set�ҵ�actor�ļ���
		vtkPolyData* pd = nullptr;
		for (int i = 0; i < setPolydatas.size(); ++i)
		{
			vtkPolyData * p = setPolydatas.at(i);
			bool ok = idPlolydata.contains(p);//�ж��Ƿ�����ʵ��actor
			if (ok)
			{
				pd = p;
				break;
			}
		}
		return pd;
	}
	/*
	��ĸ�����ʾ
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
		//����Ԥѡ�����
		clearPre_();
	}

	vtkPolyData* GeometryViewProvider::getPointPolyData(Geometry::GeometrySet* s, int id)
	{
		QList<vtkPolyData*> idPlolydata = _polydataShapeIdHash.keys(id);//����id��polydata�ļ���
		QList<vtkActor*> setActors = _setActors.values(s);//����set�ҵ�actor�ļ���
		vtkPolyData* pd = nullptr;
		for (int i = 0; i < setActors.size(); ++i)
		{
			vtkActor* a = setActors.at(i);
			bool ok = _vertexActors.contains(a);//�ж��Ƿ�������actor
			if (ok)
			{
				QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(a);
				for (int i = 0; i < polyDatas.size(); ++i)
				{
					vtkPolyData * polyData = polyDatas.at(i);
					if (idPlolydata.contains(polyData))//�жϵ�ǰ�����Ƿ�����id��Ӧ�������polydata
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
	�ߵĸ�����ʾ
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
		//����Ԥѡ�����
		clearPre_();
	}

	vtkPolyData* GeometryViewProvider::getEdgePolyData(Geometry::GeometrySet* s, int id)
	{
		QList<vtkPolyData*> idPlolydata = _polydataShapeIdHash.keys(id);//����id��polydata�ļ���
		QList<vtkActor*> setActors = _setActors.values(s);//����set�ҵ�actor�ļ���
		vtkPolyData* pd = nullptr;
		for (int i = 0; i < setActors.size(); ++i)
		{
			vtkActor* a = setActors.at(i);
			bool ok = _edgeActors.contains(a);//�ж��Ƿ�������actor
			if (ok)
			{
				QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(a);
				for (int i = 0; i < polyDatas.size(); ++i)
				{
					vtkPolyData * polyData = polyDatas.at(i);
					if (idPlolydata.contains(polyData))//�жϵ�ǰ�����Ƿ�����id��Ӧ�������polydata
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
	��ĸ�����ʾ
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
		//����Ԥѡ�����
		clearPre_();
	}

	vtkPolyData* GeometryViewProvider::getFacePolyData(Geometry::GeometrySet* s, int id)
	{
		QList<vtkPolyData*> idPlolydata = _polydataShapeIdHash.keys(id);//����id��polydata�ļ���
		QList<vtkActor*> setActors = _setActors.values(s);//����set�ҵ�actor�ļ���
		vtkPolyData* pd = nullptr;
		for (int i = 0; i < setActors.size(); ++i)
		{
			vtkActor* a = setActors.at(i);
			bool ok = _faceActors.contains(a);//�ж��Ƿ�������actor
			if (ok)
			{
				QList<vtkPolyData*> polyDatas = _actorPolydataHash.values(a);
				for (int i = 0; i < polyDatas.size(); ++i)
				{
					vtkPolyData * polyData = polyDatas.at(i);
					if (idPlolydata.contains(polyData))//�жϵ�ǰ�����Ƿ�����id��Ӧ�������polydata
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
	��ӻ�ɾ��actor
	*/
	void GeometryViewProvider::showActor(vtkPolyData* pd , bool state)
	{
		if (state)
		{
			QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//���е�ѡ��Ķ����polydata����
			if (setSelectPolyData.contains(pd))
			{
				return;
			}
			//��ʾ
			QColor colorhigh = Setting::BusAPI::instance()->getGraphOption()->getHighLightColor();
			float width = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
			float size = Setting::BusAPI::instance()->getGraphOption()->getGeoCurveWidth();
			QColor color = Setting::BusAPI::instance()->getGraphOption()->getGeometrySurfaceColor();
			int trans = Setting::BusAPI::instance()->getGraphOption()->getTransparency();
			//��Ⱦ
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
			case ModuleBase::GeometryWinPoint://����ʾ
			case ModuleBase::GeometryPoint:
				Actor->GetProperty()->SetPointSize(size);
				Actor->GetProperty()->SetRepresentationToPoints();
				break;
			case ModuleBase::GeometryWinCurve://����ʾ
			case ModuleBase::GeometryCurve:
				Actor->GetProperty()->SetLineWidth(width);
				Actor->GetProperty()->SetRepresentationToWireframe();
				break;
			case ModuleBase::GeometryWinSurface://����ʾ
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
			QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//���е�ѡ��Ķ����polydata����
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
		//����Ԥѡ�����
		clearPre_();
		clearGeometryHighLight();
	}

	/*
	������е��Ѿ�������ʾ����
	*/
	void GeometryViewProvider::clearGeometryHighLight()
	{
		RestoreGeoColor();
		QList<vtkActor *> setSelectActor = _selectPolydataActorHash.values();//���е�ѡ��Ķ����actor����
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
	��յ�ǰ������Ѿ���������
	*/
	void GeometryViewProvider::clearSelectActors(Geometry::GeometrySet* s)
	{
		RestoreGeoColor();
		QList<vtkPolyData *> setSelectPolyData = _selectPolydataActorHash.keys();//���е�ѡ��Ķ����polydata����
		QList<vtkActor*> setActors = _setActors.values(s);//����set�ҵ�actor�ļ���
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
	���û�׼����ʾģʽ
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
	��ʾ��׼ƽ��
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
			//actor��Ӧ��������
			_actorPolydataHash.insert(Actor, Mapper->GetInput());
			//����Ķ�Ӧ��id���
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
	���õ����壨�棩����ʾ������
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
		QList<vtkPolyData*> idPolydata = _polydataShapeIdHash.keys(geoindex);//����id��polydata�ļ���
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
		//��Ϲ�����
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
	��ԭ���ж�����ɫ����ո�����ʾ
	*/
	void GeometryViewProvider::RestoreGeoColor()
	{
		//��ԭ����actor����ɫ
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
	��ʾset�����ж���
	*/
	void GeometryViewProvider::showGeoSet(Geometry::GeometrySet* set, bool render)
	{
		clearSelectActors(set);
		TopoDS_Shape* shape = set->getShape();
		showShape(*shape, set, render);
	}

	/*
	 ��ʾselectItems�����ж���
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
