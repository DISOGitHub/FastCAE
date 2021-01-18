#ifndef _DIALOGMAKESWEEP_H_
#define _DIALOGMAKESWEEP_H_

#include "geoDialogBase.h"
#include <QList>
#include <QPair>
#include <QMultiHash>

namespace Ui
{
	class SweepDialog;
}

namespace Geometry
{
	class GeometrySet;
}

class TopoDS_Shape;

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI SweepDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		SweepDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		SweepDialog(GUI::MainWindow* m, MainWidget::PreWindow* pre, Geometry::GeometrySet* set);
		~SweepDialog();


	private slots:
		void shapeSlected(Geometry::GeometrySet* set, int shape) override;
		void on_geoSelectCurve_clicked();
		void on_geoSelectCurve_1_clicked();

	private:
//		void closeEvent(QCloseEvent *e);
		void accept();
		void reject();
		void selectSection(int shape, Geometry::GeometrySet* set);
		void selectPath(int shape, Geometry::GeometrySet* set);
		void init();

	private:
		Ui::SweepDialog* _ui{};
		bool _selectSection{ false }; 
		bool _selectPath{ false };
		bool _solid{ true};
		QMultiHash<Geometry::GeometrySet*, int> _sectionEdgeHash{};
		QPair<Geometry::GeometrySet*, int> _pathEdge{};
	};
}

#endif