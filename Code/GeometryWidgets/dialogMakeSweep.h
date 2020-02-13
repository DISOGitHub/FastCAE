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

class vtkActor;
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
		void selectActorShape(vtkActor* ac, int shape, Geometry::GeometrySet* set) override;
		void on_geoSelectCurve_clicked();
		void on_geoSelectCurve_1_clicked();

	private:
		void closeEvent(QCloseEvent *e);
		void accept();
		void reject();
		void selectSection(vtkActor* ac, int shape, Geometry::GeometrySet* set);
		void selectPath(vtkActor* ac, int shape, Geometry::GeometrySet* set);
		void init();

	private:
		Ui::SweepDialog* _ui{};
		bool _selectSection{ false }; 
		bool _selectPath{ false };

		QList<vtkActor*> _sectionActors{};
		QMultiHash<Geometry::GeometrySet*, int> _sectionEdgeHash{};

		vtkActor* _pathActor{};
		QPair<Geometry::GeometrySet*, int> _pathEdge{};
	};
}

#endif