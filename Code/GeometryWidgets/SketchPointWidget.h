#ifndef SKETCHWIDGETPOINT_H_
#define SKETCHWIDGETPOINT_H_

#include "geoDialogBase.h"
#include "geometryWidgetsAPI.h"

namespace Command
{
	class SketchCreater;
}

namespace Ui
{
	class SketchPointWidget;
}

namespace Geometry
{
	class GeometryData;
}


namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI SketchPointWidget : public GeoDialogBase
	{
		Q_OBJECT
	public:
		SketchPointWidget(GUI::MainWindow* m, MainWidget::PreWindow* pre, Command::SketchCreater* c);
		~SketchPointWidget();


	private slots:
	    void on_addButton_clicked();

	private:
		Command::SketchCreater* _creater{};
		Ui::SketchPointWidget* _ui{};

		Geometry::GeometryData* _geoData{};

	};

}



#endif // !SKETCHWIDGETPOINT_H_
