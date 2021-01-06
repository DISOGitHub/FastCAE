#ifndef GEOCOMMANDBOOL_H_
#define GEOCOMMANDBOOL_H_

#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include <QPair>

enum BoolType
{
	BoolNone,
	BoolCut,
	BoolFause,
	BoolCommon,
};

class TopoDS_Shape;

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI CommandBool : public GeoCommandBase
	{
	public:
		CommandBool(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandBool() = default;
		
		void setType(BoolType t);
		//void setInputBody(Geometry::GeometrySet* b1, Geometry::GeometrySet* b2);
		void setSolid1(QPair <Geometry::GeometrySet*, int> solid1);
		void setSolid2(QPair <Geometry::GeometrySet*, int> solid2);
		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;
		

	private:
		TopoDS_Shape* cut();
		TopoDS_Shape* fause();
		TopoDS_Shape* common();
			
	private:
 	/*	Geometry::GeometrySet* _body1{};
 		Geometry::GeometrySet* _body2{};*/
		QPair <Geometry::GeometrySet*, int> _solid1{};
		QPair <Geometry::GeometrySet*, int> _solid2{};
		BoolType _type{ BoolNone };

		Geometry::GeometrySet* _result{};
	};
}


#endif