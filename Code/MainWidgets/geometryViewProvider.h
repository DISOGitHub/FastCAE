#ifndef GEOMETRYVIEWPROVIDER_H_
#define GEOMETRYVIEWPROVIDER_H_

#include <QObject>
#include <QMultiHash>
#include <QHash>
#include <QList>
#include "moduleBase/ModuleType.h"

class TopoDS_Shape;
class TopoDS_Edge;
class vtkActor;

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

	class GeometryViewProvider: public QObject
	{
		Q_OBJECT
	public:
		GeometryViewProvider(GUI::MainWindow* mainwindow, PreWindow* preWin);
		~GeometryViewProvider();

		void showShape(TopoDS_Shape& shape,Geometry::GeometrySet* set);
		void updateGeoActors();
		void updateGraphOption();
		void updateDiaplayStates(Geometry::GeometrySet* s, bool visibility);
		QMultiHash<Geometry::GeometrySet*, int> getGeoSelectItems();
		void setGeoSelectItems(QMultiHash<Geometry::GeometrySet*, int> items);

	public slots:
		void showGeoSet(Geometry::GeometrySet* set);
		void showDatum(Geometry::GeometryDatum* datm);
		void removeActors(Geometry::GeometrySet* set);
		void removeDatumActors(Geometry::GeometryDatum* plane);
		void setGeoSelectMode(int);
		void selectGeometry(vtkActor*,bool);
		void highLightGeoset(Geometry::GeometrySet* s, bool on);
		void highLightGeoPoint(Geometry::GeometrySet* s, int, QList<vtkActor*>*);
		void highLightGeoEdge(Geometry::GeometrySet* s, int, QList<vtkActor*>*);
		void highLightGeoFace(Geometry::GeometrySet* s, int, QList<vtkActor*>*);
		void setGeometryDisplay( bool v, bool c, bool f);
		//还原几何颜色。
		void RestoreGeoColor();
// 		//激活几何选取模式
// 		void activeSelectGeo(bool on);
// 		//关闭几何选取模式
// 		void closeSelectGeo(int geomodel);
	private:
		void init();
		void showVertex(Geometry::GeometrySet* set); 
		void showEdge(Geometry::GeometrySet* set);
		void showFace(Geometry::GeometrySet* set);
		void showDatumPlane(Geometry::GeometryDatum* datum);
		
		void setPickable(QList<vtkActor*> acs, bool visibility);


	private:
		PreWindow* _preWindow{};
		GUI::MainWindow* _mainWindow{};
		Geometry::GeometryData* _geoData{};
		//根据ctrlpress状态存放actor
		QList<vtkActor*> _addActors{};
		QList<vtkActor*> _vertexActors{};
		QList<vtkActor*> _edgeActors{};
		QList<vtkActor*> _faceActors{};
		bool _showvertex{ false };
		bool _showedge{ false };
		bool _showface{ true };
//		bool _activeSeletGeo{ false };

		QMultiHash<Geometry::GeometrySet*, vtkActor*> _setActors{};
		QMultiHash<Geometry::GeometryDatum*, vtkActor*> _datumActors{};

		QHash<vtkActor*, int> _actotShapeHash{};

		ModuleBase::SelectModel _selectType{ ModuleBase::None };

		QMultiHash<Geometry::GeometrySet*, int> _selectItems{};

	};

}



#endif