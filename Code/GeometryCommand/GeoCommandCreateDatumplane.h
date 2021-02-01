#ifndef GEOCOMMANDCREATEDATUMPLANE_H_
#define GEOCOMMANDCREATEDATUMPLANE_H_

#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"

class TopoDS_Shape;

namespace Geometry
{
	class GeometrySet;
	class GeometryDatum;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandCreateDatumplane : public GeoCommandBase
	{
	public:
		GeoCommandCreateDatumplane(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandCreateDatumplane() = default;

		bool execute()override;
		void undo() override;
		void redo() override;
		void releaseResult() override;
		void setAxis(double* axis);
		void setPlane(double* loc, double* dir);
		void setFaceBody(Geometry::GeometrySet* face, int index);
	private:
		TopoDS_Shape* common(TopoDS_Shape* shape1, TopoDS_Shape* shape2);

	private:

		double _axis[3];
		double _planeDirection[3];
		double _planeLocation[3];
		Geometry::GeometrySet* _face;
		int _faceIndex{ -1 };
		//Geometry::GeometrySet* _result{};
		Geometry::GeometryDatum* _result{};



	};














}
























#endif
