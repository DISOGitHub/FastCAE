#ifndef __GEOMETRYVIEWDATA_H__
#define __GEOMETRYVIEWDATA_H__

#include <QList>
#include <QHash>
#include <Standard_Handle.hxx>
#include "moduleBase/ModuleType.h"

class vtkPolyData;
class TopoDS_TShape;
class QColor;
class vtkPoints;
class vtkCell;
class vtkCellArray;
class vtkKdTree;

namespace Geometry
{
	class GeometryData;
	class GeometrySet;
}

namespace MainWidget
{
	class GeometryViewObject;
	class GeoSetViewObject;
	class GeometryViewObject;

	class GeometryViewData
	{
	public:
		GeometryViewData() = default;
		~GeometryViewData();

		QList<vtkPolyData*> transferToPoly(Geometry::GeometrySet* gset);
		void removeViewObjs(Geometry::GeometrySet* gset);

		void updateGraphOption();
		void highLight(Geometry::GeometrySet* set, bool on);
		void highLightFace(Geometry::GeometrySet* set, int index, bool on);
		void highLightEdge(Geometry::GeometrySet* set, int index, bool on);
		void highLightPoint(Geometry::GeometrySet* set, int index, bool on);
		void highLightSolid(Geometry::GeometrySet* set, int index, bool on);

		GeometryViewObject* getSolidViewObj(vtkPolyData* facePoly, int cellIndex);
		GeometryViewObject* getFaceViewObj(vtkPolyData* facePoly, int cellIndex);
		GeometryViewObject* getEdgeViewObj(vtkPolyData* facePoly, int cellIndex);
		GeometryViewObject* getPointViewObj(vtkPolyData* facePoly, int pointIndex);

		QList<GeometryViewObject*> getViewObjectByStates(int selectStates, int states);

		void preHighLight(GeometryViewObject* vob);
		GeometryViewObject*  getPreHighLightObj();

	private:
		vtkPolyData* transferFace(Geometry::GeometrySet* gset);
		vtkPolyData* transferEdge(Geometry::GeometrySet* gset);
		vtkPolyData* transferPoint(Geometry::GeometrySet* gset);
		GeoSetViewObject* getGeosetObj(Geometry::GeometrySet* set);


		vtkCell* getLineCellIn(vtkPolyData* p);
		int getIndexInPoly(vtkPolyData* cell, vtkPolyData* poly, QList<int> lineIndexs);
		bool isSameCell(vtkKdTree* partTree, vtkCell* cell, vtkPolyData* allPoly);

	private:
		GeometryViewObject* _preViewObjct{};
		QHash<Geometry::GeometrySet*, GeoSetViewObject* > _viewObjs{};
	};


}

#endif
