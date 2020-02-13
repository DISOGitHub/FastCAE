#ifndef SKETCHCREATER_H_
#define SKETCHCREATER_H_

#include "geometryCommandAPI.h"
#include <QObject>
#include "moduleBase/ModuleType.h"
#include <QList>
#include <gp_Pnt.hxx>

class vtkActor;
class TopoDS_Shape;

namespace MainWidget
{
	class SketchViewProvider;
	class PreWindow;
}

namespace GUI
{
	class MainWindow;
}

namespace Geometry
{
	class GeometryData;
}

namespace Command
{
	class GeoComandList;

	class GEOMETRYCOMMANDAPI SketchCreater : public QObject
	{
		Q_OBJECT
	public:
		SketchCreater(MainWidget::SketchViewProvider* p, MainWidget::PreWindow* pre, GUI::MainWindow* m , vtkActor* actor);
		~SketchCreater();
		
		void setSketchType(ModuleBase::SketchType t);
		void appendPoint(gp_Pnt &pt);
		void setTempPoint(gp_Pnt &pt);
		void onMouseRightRelease();
		void onMouseMiddleRelease();

	signals:
		void reRender();

	private:
		void exec();
		bool createLine();
		bool createRect();
		bool creatCircle();
		bool createArc();
		bool createPolyline(bool close = false);
		bool creatSpline();

		void generateTempShape();
		TopoDS_Shape* generateLine();
		TopoDS_Shape* generateRect();
		TopoDS_Shape* generateCircle();
		TopoDS_Shape* generateArc();
		TopoDS_Shape* generatePolyLine();


	private:
		MainWidget::SketchViewProvider* _view{};
		MainWidget::PreWindow* _preWindow{};
		GUI::MainWindow* _mainWindow{};
		GeoComandList* _commandList{};

		Geometry::GeometryData* _geoData{};
		ModuleBase::SketchType _sketchType{ ModuleBase::SketchNone };
		QList<gp_Pnt> _pointList{};
//		QList<gp_Pnt>_polyPtList{};
		gp_Pnt _tempPnt{};
		vtkActor* _tempActor{};

	};

}


#endif