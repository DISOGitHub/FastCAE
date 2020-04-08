#ifndef DIALOGMEASUREDISTANCE_H_
#define DIALOGMEASUREDISTANCE_H_
 
#include "geoDialogBase.h"

class vtkActor;

namespace Ui
{
	class MeasureaDistanceDialog;
}

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI MeasureDistanceDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		MeasureDistanceDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~MeasureDistanceDialog();

	private slots:
	    void on_closeButton_clicked();
		void on_geoSelectPoint_clicked();
		void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;

	private:
		Ui::MeasureaDistanceDialog* _ui{};
		double _point[3];
		QList<vtkActor*> _actors{};
	};

}


#endif
