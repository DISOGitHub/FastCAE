#ifndef  _GEOPOINTWIDGET_H_
#define  _GEOPOINTWIDGET_H_

#include <QWidget>

namespace GUI
{
	class MainWindow;
}
namespace MainWidget
{
	class PreWindow;
}
namespace Ui
{
	class geoPointWidget;
}
namespace Geometry
{
	class GeometrySet;
}

class vtkActor;

namespace GeometryWidget
{
	class  GeoPointWidget:public QWidget
	{
		Q_OBJECT
	public:
		GeoPointWidget(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoPointWidget();

		bool getCoordinate(double *coor);
		void handleProcess(bool h);
		void setCoordinate(double *coor);

	signals:
		void setSelectMode(int);
		void updateGraphOptions();
		void buttonCkicked(GeoPointWidget*);

	private slots :
		void on_geoSelectPoint_clicked();
	    void selectPoint(vtkActor*, int, Geometry::GeometrySet*);

	private:
		Ui::geoPointWidget* _ui;
		GUI::MainWindow* _mainWindow{};
		MainWidget::PreWindow* _preWindow{};
		bool _handle{ true };

		vtkActor* _selectedActor{};
	};
}




#endif