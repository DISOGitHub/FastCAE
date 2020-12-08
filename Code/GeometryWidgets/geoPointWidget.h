#ifndef  _GEOPOINTWIDGET_H_
#define  _GEOPOINTWIDGET_H_

#include <QWidget>
#include <QPair>
#include "GeometryWidgets/geometryWidgetsAPI.h"

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


namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI GeoPointWidget :public QWidget
	{
		Q_OBJECT
	public:
		GeoPointWidget(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoPointWidget();

		bool getCoordinate(double *coor);
		void handleProcess(bool h);
		void setCoordinate(double *coor);

	signals:
		void buttonCkicked(GeoPointWidget*);
		void setSelectMode(int);
		void clearGeometryHighLightSig();
		void highLightGeometryPointSig(Geometry::GeometrySet*, int, bool);
		

	private slots :
		void on_geoSelectPoint_clicked();
	    void selectPoint(Geometry::GeometrySet*, int);

	private:
		Ui::geoPointWidget* _ui;
		GUI::MainWindow* _mainWindow{};
		MainWidget::PreWindow* _preWindow{};
		QPair<Geometry::GeometrySet*, int> _selectedPoint{nullptr,-1};
		
		bool _handle{ true };
	};
}




#endif