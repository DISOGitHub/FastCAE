#ifndef  _GEOMETRYSETVIEWDATA_H__
#define  _GEOMETRYSETVIEWDATA_H__

#include <QList>
#include <QHash>
#include <Standard_Handle.hxx>

class vtkPolyData;
class TopoDS_TShape;
class QColor;


namespace Geometry
{
	class GeometryData;
	class GeometrySet;
}

namespace MainWidget
{
	class GeometryViewObject;

	class GeoSetViewObject
	{
	public:
		GeoSetViewObject(Geometry::GeometrySet*);
		~GeoSetViewObject();
		Geometry::GeometrySet* getGeometrySet();

		void setFacePoly(vtkPolyData* p);
		vtkPolyData* getFacePoly();
		void setEdgePoly(vtkPolyData* p);
		vtkPolyData* getEdgePoly();
		void setPointPoly(vtkPolyData* p);
		vtkPolyData* getPointPoly();
		void genSolidViewObj();

		void appendFaceViewObj(int index, Handle(TopoDS_TShape) s, GeometryViewObject* v);
		void appendEdgeViewObj(int index, GeometryViewObject* v);
		void appendPointViewObj(int index, GeometryViewObject* v);
		void appendSolidViewObj(int index, GeometryViewObject* v);

		GeometryViewObject* getFaceViewObjByIndex(int index);

		GeometryViewObject* getFaceViewObjByCellIndex(int cellIndex);
		GeometryViewObject* getEdgeViewObjByCellIndex(int cellIndex);
		GeometryViewObject* getPointViewObjByCellIndex(int pointIndex);
		GeometryViewObject* getSolidViewObjByFaceIndex(int faceIndex);

		QList<GeometryViewObject*> getViewObjectByStates(int selectModel, int sates);

		void resetColor();
		void highLight(bool on);
		void highLightFace(int index, bool on);
		void highLightEdge(int index, bool on);
		void highLightPoint(int index, bool on);
		void highLightSolid(int index, bool on);

	private:
		Geometry::GeometrySet* _geoSet{};
		vtkPolyData* _facePoly{};
		vtkPolyData* _edgePoly{};
		vtkPolyData* _pointPoly{};

		QHash<GeometryViewObject*, Handle(TopoDS_TShape)> _faceObjHash{};
		QHash<int, GeometryViewObject*> _faceViewObjs{};
		QHash<int, GeometryViewObject*> _edgeViewObjs{};
		QHash<int, GeometryViewObject*> _pointViewObjs{};
		QHash<int, GeometryViewObject*> _solidViewObjs{};

	};


}



#endif
