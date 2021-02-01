#include "geometryViewData.h"
#include "geometry/geometrySet.h"
#include <QDebug>
#include <vtkPolyData.h>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <vtkAppendPolyData.h>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkTriangle.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>
#include <vtkLine.h>
#include <vtkPolyLine.h>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <vtkKdTree.h>
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include "geometrySetViewData.h"
#include "geometryViewObject.h"
#include <vtkPolyDataWriter.h>


namespace MainWidget
{
	GeometryViewData::~GeometryViewData()
	{
		QList<GeoSetViewObject*> setViews = _viewObjs.values();
		for (auto v : setViews)
			delete v;
		_viewObjs.clear();
	}

	void GeometryViewData::removeViewObjs(Geometry::GeometrySet* gset)
	{
		if (!_viewObjs.contains(gset)) return;
		auto obj = _viewObjs.value(gset);
		_viewObjs.remove(gset);
		delete obj;
	}

	void GeometryViewData::updateGraphOption()
	{
		QList<GeoSetViewObject*> setViews = _viewObjs.values();
		for (auto setView : setViews)
			setView->resetColor();
		//		_preViewObjct = nullptr;
	}

	void GeometryViewData::highLight(Geometry::GeometrySet* set, bool on)
	{
		auto setView = _viewObjs.value(set);
		if (setView != nullptr)
			setView->highLight(on);
	}


	void GeometryViewData::highLightFace(Geometry::GeometrySet* set, int index, bool on)
	{
		auto setView = _viewObjs.value(set);
		if (setView != nullptr)
			setView->highLightFace(index, on);
	}


	void GeometryViewData::highLightEdge(Geometry::GeometrySet* set, int index, bool on)
	{
		auto setView = _viewObjs.value(set);
		if (setView != nullptr)
			setView->highLightEdge(index, on);
	}


	void GeometryViewData::highLightPoint(Geometry::GeometrySet* set, int index, bool on)
	{
		auto setView = _viewObjs.value(set);
		if (setView != nullptr)
			setView->highLightPoint(index, on);
	}


	void GeometryViewData::highLightSolid(Geometry::GeometrySet* set, int index, bool on)
	{
		auto setView = _viewObjs.value(set);
		if (setView != nullptr)
			setView->highLightSolid(index, on);
	}

	GeometryViewObject* GeometryViewData::getSolidViewObj(vtkPolyData* facePoly, int cellIndex)
	{
		auto face = this->getFaceViewObj(facePoly, cellIndex);
		if (face == nullptr) return  nullptr;
		int faceIndex = face->getIndex();
		GeoSetViewObject* sv = face->getGeoSetViewObject();
		if (sv == nullptr || faceIndex < 0) return nullptr;
		return sv->getSolidViewObjByFaceIndex(faceIndex);
	}

	GeometryViewObject* GeometryViewData::getFaceViewObj(vtkPolyData* facePoly, int cellIndex)
	{
		QList<GeoSetViewObject*> setViews = _viewObjs.values();
		GeoSetViewObject* setView = nullptr;
		for (auto view : setViews)
		{
			vtkPolyData* tpoly = view->getFacePoly();
			if (facePoly == tpoly)
			{
				setView = view;
				break;
			}
		}
		if (setView == nullptr) return nullptr;
		return setView->getFaceViewObjByCellIndex(cellIndex);
	}

	GeometryViewObject* GeometryViewData::getEdgeViewObj(vtkPolyData* edgePoly, int cellIndex)
	{
		QList<GeoSetViewObject*> setViews = _viewObjs.values();
		GeoSetViewObject* setView = nullptr;
		for (auto view : setViews)
		{
			vtkPolyData* tpoly = view->getEdgePoly();
			if (edgePoly == tpoly)
			{
				setView = view;
				break;
			}
		}
		if (setView == nullptr) return nullptr;
		return setView->getEdgeViewObjByCellIndex(cellIndex);
	}

	GeometryViewObject* GeometryViewData::getPointViewObj(vtkPolyData* pointPoly, int pointIndex)
	{
		QList<GeoSetViewObject*> setViews = _viewObjs.values();
		GeoSetViewObject* setView = nullptr;
		for (auto view : setViews)
		{
			vtkPolyData* tpoly = view->getPointPoly();
			if (pointPoly == tpoly)
			{
				setView = view;
				break;
			}
		}
		if (setView == nullptr) return nullptr;
		return setView->getPointViewObjByCellIndex(pointIndex);
	}

	QList<GeometryViewObject*> GeometryViewData::getViewObjectByStates(int selectStates, int states)
	{
		QList<GeometryViewObject*> objects;
		QList<GeoSetViewObject*> setList = _viewObjs.values();
		for (auto sv : setList)
		{
			if (sv == nullptr) continue;
			QList<GeometryViewObject*> v = sv->getViewObjectByStates(selectStates, states);
			objects.append(v);
		}
		return objects;
	}


	void GeometryViewData::preHighLight(GeometryViewObject* vob)
	{
		if (_preViewObjct != nullptr && _preViewObjct->getStates() == GeometryViewObject::HighLigh)
			_preViewObjct = nullptr;
		if (vob != nullptr && vob->getStates() == GeometryViewObject::HighLigh)return;
		if (vob == _preViewObjct && vob != nullptr) return;
		if (_preViewObjct != nullptr)
			_preViewObjct->resetColor();
		_preViewObjct = vob;
		if (_preViewObjct != nullptr)
			_preViewObjct->preHighLight();
	}


	MainWidget::GeometryViewObject* GeometryViewData::getPreHighLightObj()
	{
		return _preViewObjct;
	}

	QList<vtkPolyData*> GeometryViewData::transferToPoly(Geometry::GeometrySet* gset)
	{
		removeViewObjs(gset);
		QList<vtkPolyData*> polys;
		auto p = this->transferFace(gset);
		polys.append(p);
		p = this->transferEdge(gset);
		polys.append(p);
		p = this->transferPoint(gset);
		polys.append(p);
		if (_viewObjs.contains(gset))
			_viewObjs.value(gset)->genSolidViewObj();
		return polys;
	}

	vtkPolyData* GeometryViewData::transferFace(Geometry::GeometrySet* gset)
	{
		TopoDS_Shape* shape = gset->getShape();
		TopExp_Explorer faceExp(*shape, TopAbs_FACE);
		QList<Handle(TopoDS_TShape)> tshapelist;

		vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
		vtkPolyData* polyData = vtkPolyData::New();
		int beg = 0;
		for (int index = 0; faceExp.More(); faceExp.Next(), ++index)
		{
			const TopoDS_Shape& s = faceExp.Current();
			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.contains(ts)) continue;
			tshapelist.append(ts);

			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);

			vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
			cleanFilter->SetInputConnection(DS->GetOutputPort());
			cleanFilter->Update();
			vtkSmartPointer<vtkPolyData> tpolys = vtkSmartPointer<vtkPolyData>::New();
			vtkPolyData* tpolydata = cleanFilter->GetOutput();
			const int np = tpolydata->GetNumberOfPoints();
			const int nc = tpolydata->GetNumberOfCells();
			vtkPoints* points = vtkPoints::New();
			for (int i = 0; i < np; i++)
			{
				double* coor = tpolydata->GetPoint(i);
				points->InsertNextPoint(coor);
			}
			tpolys->SetPoints(points);
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
			tpolys->SetPolys(cells);

			vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
			normals->SetInputData(tpolys);
			normals->FlipNormalsOn();
			normals->Update();
			vtkPolyData* facePoly = normals->GetOutput();

			const int ncell = facePoly->GetNumberOfCells();
			if (ncell < 1) continue;
			GeometryViewObject* obj = new GeometryViewObject(GeometryViewObject::Face, beg, beg + ncell - 1, ts);
			beg += ncell;
			appendFilter->AddInputData(facePoly);
			auto setViewObj = this->getGeosetObj(gset);
			setViewObj->appendFaceViewObj(index, ts, obj);
		}

		appendFilter->Update();
		polyData->DeepCopy(appendFilter->GetOutput());
		auto setViewObj = this->getGeosetObj(gset);
		setViewObj->setFacePoly(polyData);
		const int npc = polyData->GetNumberOfCells();
		if (npc < 1) return  nullptr;
		return polyData;
	}

	vtkPolyData* GeometryViewData::transferEdge(Geometry::GeometrySet* gset)
	{
		TopoDS_Shape* shape = gset->getShape();

		TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
		QHash<int, Handle(TopoDS_TShape)> tshapelist;
		QHash<int, vtkPolyData*> indexPoly;
		vtkSmartPointer<vtkAppendPolyData> appendPoly = vtkSmartPointer<vtkAppendPolyData>::New();
		for (int index = 0; edgeExp.More(); edgeExp.Next(), ++index)
		{
			const TopoDS_Shape& s = edgeExp.Current();
			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.values().contains(ts))
				continue;
			tshapelist.insert(index, ts);
			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);
			DS->Update();
			vtkPolyData* p = DS->GetOutput();
			const int ncell = p->GetNumberOfCells();
			if (ncell < 2) continue;
			appendPoly->AddInputData(p);
			indexPoly.insert(index, p);
		}
		vtkSmartPointer <vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
		cleaner->SetInputConnection(appendPoly->GetOutputPort());
		cleaner->Update();
		vtkPolyData* tPoly = cleaner->GetOutput();
		vtkPolyData* newPoly = vtkPolyData::New();
		newPoly->DeepCopy(tPoly);

		QList<int> lineCell;
		const int ncell = newPoly->GetNumberOfCells();
		for (int i = 0; i < ncell; ++i)
		{
			vtkCell* cell = newPoly->GetCell(i);
			if (cell->GetNumberOfPoints() > 1)
				lineCell.append(i);
		}
		//qDebug() << lineCell;

		QList<vtkPolyData*> edges = indexPoly.values();
		for (auto edge : edges)
		{
			const int eindex = indexPoly.key(edge);
			Handle(TopoDS_TShape) ts = tshapelist.value(eindex);
			//			vtkCell* line = this->getLineCellIn(edge);
			int index = getIndexInPoly(edge, newPoly, lineCell);
			if (index < 0) continue;
			lineCell.removeOne(index);
			GeometryViewObject* obj = new GeometryViewObject(GeometryViewObject::Edge, index, index, ts);
			auto setViewObj = this->getGeosetObj(gset);
			setViewObj->appendEdgeViewObj(eindex, obj);
		}

		auto setViewObj = this->getGeosetObj(gset);
		setViewObj->setEdgePoly(newPoly);
		return newPoly;

	}

	vtkPolyData* GeometryViewData::transferPoint(Geometry::GeometrySet* gset)
	{
		TopoDS_Shape* shape = gset->getShape();
		TopExp_Explorer ptExp(*shape, TopAbs_VERTEX);
		QList<Handle(TopoDS_TShape)> tshapelist;
		vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
		int beg = 0;
		vtkPolyData* polyData = vtkPolyData::New();
		for (int index = 0; ptExp.More(); ptExp.Next(), ++index)
		{
			const TopoDS_Shape& s = ptExp.Current();
			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.contains(ts)) continue;
			tshapelist.append(ts);
			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(s);
			vtkSmartPointer<IVtkTools_ShapeDataSource> DS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
			DS->SetShape(aShapeImpl);
			DS->Update();
			vtkPolyData * tpolyData = DS->GetOutput();
			const int np = tpolyData->GetNumberOfPoints();
			if (np < 1) continue;
			GeometryViewObject* obj = new GeometryViewObject(GeometryViewObject::Point, beg, beg + np - 1, ts);
			beg += np;
			appendFilter->AddInputData(tpolyData);
			auto setViewObj = this->getGeosetObj(gset);
			setViewObj->appendPointViewObj(index, obj);
		}
		appendFilter->Update();
		polyData->DeepCopy(appendFilter->GetOutput());
		auto setViewObj = this->getGeosetObj(gset);
		setViewObj->setPointPoly(polyData);
		const int npc = polyData->GetNumberOfCells();
		if (npc < 1) return  nullptr;
		return polyData;
	}

	GeoSetViewObject* GeometryViewData::getGeosetObj(Geometry::GeometrySet* set)
	{
		if (_viewObjs.contains(set)) return _viewObjs.value(set);
		auto viewObj = new GeoSetViewObject(set);
		_viewObjs.insert(set, viewObj);
		return viewObj;
	}


	vtkCell* GeometryViewData::getLineCellIn(vtkPolyData* p)
	{
		const int n = p->GetNumberOfCells();
		for (int i = 0; i < n; ++i)
		{
			vtkCell* cel = p->GetCell(i);
			const int np = cel->GetNumberOfPoints();
			if (np > 1) return cel;
		}
		return nullptr;
	}


	int GeometryViewData::getIndexInPoly(vtkPolyData* part, vtkPolyData* poly, QList<int> lineIndexs)
	{
		if (part == nullptr || poly == nullptr) return -1;
		vtkCell* cell = getLineCellIn(part);
		if (cell == nullptr) return -1;
		const int oriCellType = cell->GetCellType();
		const int oriPointCount = cell->GetNumberOfPoints();

		vtkSmartPointer<vtkKdTree> kdTree = vtkSmartPointer<vtkKdTree>::New();
		kdTree->BuildLocatorFromPoints(part->GetPoints());

		for (int index : lineIndexs)
		{
			vtkCell* currentCell = poly->GetCell(index);
			if (currentCell->GetCellType() != oriCellType)  continue;
			if (currentCell->GetNumberOfPoints() != oriPointCount) continue;
			if (isSameCell(kdTree, currentCell, poly))
				return index;
		}
		return -1;
	}


	bool GeometryViewData::isSameCell(vtkKdTree* tree, vtkCell* cell2, vtkPolyData* allPoly)
	{
		const int npt = cell2->GetNumberOfPoints();
		for (int i = 0; i < npt; ++i)
		{
			int pindex = cell2->GetPointId(i);
			double* loc = allPoly->GetPoint(pindex);

			int index = tree->FindPoint(loc);
			if (index < 0)
				return false;
			if (i > 8) break;;
		}
		return true;

	}

}
