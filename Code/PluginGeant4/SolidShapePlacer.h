#ifndef SOLIDSHAPEPLACER_H
#define SOLIDSHAPEPLACER_H

#include <QObject>

class TopoDS_Shape;

namespace GUI
{
	class MainWindow;
}

namespace Geometry
{
	class GeometryData;
	class GeometrySet;
}

namespace Geant4
{
	class StructureManager;
	class SolidsManager;

	class SolidShapePlacer : public QObject
	{
		Q_OBJECT
	public:
		SolidShapePlacer(GUI::MainWindow* m);
		~SolidShapePlacer() = default;

		void place();

	signals:
		void updateGeoTree();
		void showSet(Geometry::GeometrySet*, bool);

	private:
		Geometry::GeometrySet* Transform(TopoDS_Shape* s, double * r, double * p);
	
	private:
		GUI::MainWindow*  _mainWindow{};
		
		Geometry::GeometryData* _geoData{};
		StructureManager* _structureManager{};
		SolidsManager* _solidManager{};

	};

}


#endif // !SOLIDSHAPEPLACER_H
