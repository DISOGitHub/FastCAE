#ifndef GEOCOMMANDBASE_H_
#define GEOCOMMANDBASE_H_

#ifndef PI
#define PI 3.14159265358979323846
#endif

#include<QObject>
#include "geometryCommandAPI.h"
#include <QList>

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PreWindow;
}

namespace Geometry
{
	class GeometryData;
	class GeometrySet;
	class GeometryDatum;
}

class TopoDS_Shape;

namespace Command
{
	enum CommandType
	{
		D3Feature,
		Sketch,
	};

	class GeoComandList;

	class GEOMETRYCOMMANDAPI GeoCommandBase : public QObject
	{
		Q_OBJECT
	public:
		GeoCommandBase(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandBase() = default;

		virtual bool execute() = 0;
		virtual void undo();
		virtual void redo();
		virtual void releaseResult();
		TopoDS_Shape* getResultShape();
		CommandType getComamndType();
		void setEditData(Geometry::GeometrySet* set);


	signals:
		void updateGeoTree();
		void showSet(Geometry::GeometrySet* s, bool r = true);
		void showDatum(Geometry::GeometryDatum*);
		void removeDisplayActor(Geometry::GeometrySet* s);
		void removeDisplayDatumActor(Geometry::GeometryDatum*);
		void updateActions();

		

	protected:
		GUI::MainWindow* _mainWindow{};
		MainWidget::PreWindow* _preWindow{};
		CommandType _type{ D3Feature };
		TopoDS_Shape* _resShape{};

		Geometry::GeometryData* _geoData{};
		GeoComandList* _commandList{};
		
		bool _isEdit{ false };
		Geometry::GeometrySet* _editSet{};

	};
}

#endif