#ifndef _GEOMETRYVIEWOBJECT_H__
#define _GEOMETRYVIEWOBJECT_H__

#include <QList>
#include <QColor>
#include <Standard_Handle.hxx>

class vtkPolyData;
class TopoDS_TShape;
class QColor;

namespace  Geometry
{
	class GeometrySet;
}

namespace  MainWidget
{
	class GeoSetViewObject;

	class GeometryViewObject
	{
	public:
		enum ViewObjType { Point, Edge, Face, Solid };
		enum ViewStates { Normal, PreHigh, HighLigh };
		GeometryViewObject(ViewObjType type, int beg, int end, Handle(TopoDS_TShape) ts);
		~GeometryViewObject() = default;
		void getRange(int& beg, int &end);
		void appendSolidFace(int f);
		QList<int> getSolidFace();
		void setIndex(int index);
		int getIndex();
		Geometry::GeometrySet* getGeometySet();
		GeoSetViewObject* getGeoSetViewObject();
		ViewStates getStates();
		void setStates(ViewStates s);

		void setConnectionData(vtkPolyData* poly, GeoSetViewObject* setViewObj);
		void resetColor();
		void preHighLight();
		void highLight();

	private:
		void setObjectColor(QColor color);
		void setSolidColor(QColor color);

	private:
		ViewObjType _type{ Solid };
		ViewStates _states{ Normal };
		int _index{ -1 };
		int _begin{ 0 };
		int _end{ 0 };
		QList<int> _solidFaces{};
		Handle(TopoDS_TShape) _tshape {};

		vtkPolyData* _polys{};
		GeoSetViewObject* _setViewObj{};
	};
}

#endif
