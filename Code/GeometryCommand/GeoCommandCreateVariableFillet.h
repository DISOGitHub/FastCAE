#ifndef GEOCOMMAND_VARIABLEFILLET_H_
#define GEOCOMMAND_VARIABLEFILLET_H_

#include "GeoCommandBase.h"
#include <QString>
#include <QMap>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{
	class  GEOMETRYCOMMANDAPI CommandCreateVariableFillet : public GeoCommandBase
	{
	public:
		CommandCreateVariableFillet(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandCreateVariableFillet() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setShape(Geometry::GeometrySet* set, int index);
		void setBasicRadius(double d);
		void setRaiudMap(QMap<double, double> m);
	

	private:
		QString _name{ "Fillet_%1" };
		Geometry::GeometrySet* _edgeSet{};
		int _edgeIndex{};
		double _basicRadius{ 0.0 };
		QMap<double, double> _radiusMap{};

		Geometry::GeometrySet* _result{};
	
	};


}


#endif