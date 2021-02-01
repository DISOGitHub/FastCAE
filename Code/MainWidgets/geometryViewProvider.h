#ifndef GEOMETRYVIEWPROVIDER_H_
#define GEOMETRYVIEWPROVIDER_H_

#include <QObject>
#include <QPair>
#include <QHash>
#include "moduleBase/ModuleType.h"

class vtkActor;
class vtkPolyData;

namespace GUI
{
	class MainWindow;
}

namespace Geometry
{
	class GeometryData;
	class GeometrySet;
	class GeometryDatum;
}


namespace MainWidget
{
	class PreWindow;
	class GeometryViewData;

	class GeometryViewProvider: public QObject
	{
		Q_OBJECT
	public:
		GeometryViewProvider(GUI::MainWindow* mainwindow, PreWindow* preWin);
		~GeometryViewProvider();

		void updateGeoActors();
		void updateGraphOption();
		void updateDiaplayStates(Geometry::GeometrySet* s, bool visibility);
		QMultiHash<Geometry::GeometrySet*, int> getGeoSelectItems();

	public slots:
		void showGeoSet(Geometry::GeometrySet* set, bool render = true);
		void showDatum(Geometry::GeometryDatum* datm);
		void removeActors(Geometry::GeometrySet* set);
		void setGeometryDisplay(bool v, bool c, bool f);
		void setGeoSelectMode(int);


	signals:
		void geoShapeSelected(Geometry::GeometrySet*shape, int index);

	private slots:
	    //高亮显示函数
		void highLightGeometrySet(Geometry::GeometrySet* s, bool on);
		void highLightGeometryFace(Geometry::GeometrySet* s, int id, bool on);//高亮显示面
		void highLightGeometryEdge(Geometry::GeometrySet* s, int id, bool on);//高亮显示边
		void highLightGeometryPoint(Geometry::GeometrySet* s, int id, bool on);//高亮显示点
		void highLightGeometrySolid(Geometry::GeometrySet* s, int id, bool on);
		void selectGeometry(bool ctrlpress);
		void preSelectGeometry(vtkActor* ac, int index);
		void clearAllHighLight();

	private:
		void init();

	private:
		struct GeoViewObj
		{
			QPair<vtkActor*, vtkPolyData*> _faceObj{ nullptr,nullptr };
			QPair<vtkActor*, vtkPolyData*> _edgeObj{ nullptr,nullptr };
			QPair<vtkActor*, vtkPolyData*> _pointObj{ nullptr,nullptr };
		};

		PreWindow* _preWindow{};
		GUI::MainWindow* _mainWindow{};
		Geometry::GeometryData* _geoData{};
		GeometryViewData* _viewData{};

		QHash<Geometry::GeometrySet*, GeoViewObj> _geoViewHash{};
	};

}

#endif
