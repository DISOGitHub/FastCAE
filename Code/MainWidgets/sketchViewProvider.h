#ifndef SKETCHVIEWPROVIDER_H_
#define SKETCHVIEWPROVIDER_H_

#include <QObject>
#include "moduleBase/ModuleType.h"
#include <QHash>

class vtkActor;
class TopoDS_Shape;
class gp_Pnt;
class QDialog;

namespace Geometry
{
	class GeometryData;
}

namespace GUI
{
	class MainWindow;
}

namespace ModuleBase
{
	class PropPickerInteractionStyle;
}

namespace Command
{
	class SketchCreater;
}

namespace GeometryWidget
{
	class SketchPointWidget;
}

namespace MainWidget
{
	class PreWindow;

	class SketchViewProvider : public QObject
	{
		Q_OBJECT
	public:
		SketchViewProvider(GUI::MainWindow* mainwindow, PreWindow* preWin);
		~SketchViewProvider();

		void setSketchType( ModuleBase::SketchType t );
		void showSketchPlane(bool show);

	signals:
		void showSketch(TopoDS_Shape*);
		void hideSketch(TopoDS_Shape*);
		void showMessage(QString);
		void showDialog(QDialog*);

	private slots:
		void onMouseRelease(double* pt);
		void onMouseMove(double* pt);
		void onRightMoustRelease();
		void onMiddleMouseUp();

		void showSketchSlot(TopoDS_Shape* shape);
		void removeSketchSlot(TopoDS_Shape* shape);
		void removeSketchActors();

		void pointTo2D(gp_Pnt* p);

	private:
		GUI::MainWindow* _mainWindow{};
		PreWindow* _preWindow{};
		Geometry::GeometryData* _geoData{};
		GeometryWidget::SketchPointWidget* _pointWidget{};

		ModuleBase::SketchType _sketchType{ ModuleBase::SketchNone };
		ModuleBase::PropPickerInteractionStyle* _interactionStyle{};
		Command::SketchCreater* _creater{};
		vtkActor* _axisXActor{};
		vtkActor* _axisYActor{};
		vtkActor* _tempActor{};

		QHash<TopoDS_Shape*, vtkActor*> _sketchActorHash{};
	};

}


#endif