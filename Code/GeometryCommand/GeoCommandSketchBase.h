#ifndef GEOCOMMANDSKETCHBASE_H_
#define GEOCOMMANDSKETCHBASE_H_

#include "GeoCommandBase.h"
#include <gp_Pnt.hxx>
#include <QList>

namespace MainWidget
{
	class SketchViewProvider;
}

namespace Command
{

	class GeocommandSketchBase : public GeoCommandBase
	{
		Q_OBJECT
	public:
		GeocommandSketchBase(MainWidget::SketchViewProvider* p, MainWidget::PreWindow* pre, GUI::MainWindow* m);
		~GeocommandSketchBase();

		void setPoints(QList<gp_Pnt> ptlist);
		void releaseResult() override;

	signals:
		void showSketch(TopoDS_Shape*);
		void removeSketch(TopoDS_Shape*);
	
	protected:
		MainWidget::SketchViewProvider* _sketchView{};
		QList<gp_Pnt> _pointList{};

	};

}



#endif