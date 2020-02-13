#ifndef _GEOMETRYDUALOGFACTORY_H__
#define _GEOMETRYDUALOGFACTORY_H__

#include "geometryWidgetsAPI.h"

class QDialog;

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
	class GeometrySet;
}

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI GeometryDialogFactory
	{
	public:
		GeometryDialogFactory() = default;
		~GeometryDialogFactory() = default;

		static QDialog* editGeometryDialog(GUI::MainWindow* m, MainWidget::PreWindow* p,  Geometry::GeometrySet* set);

	};


}

#endif // !_GEOMETRYDUALOGFACTORY_H_
