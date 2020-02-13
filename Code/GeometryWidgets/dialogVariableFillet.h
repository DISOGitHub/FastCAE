#ifndef DIALOGVARIABLEFILLET_H_
#define DIALOGVARIABLEFILLET_H_

#include "geoDialogBase.h"
#include <QMap>

namespace Ui
{
	class VariableFilletDialog;
}

namespace Geometry
{
	class GeometrySet;
}

class vtkActor;

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI VariableFilletDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		VariableFilletDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		VariableFilletDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~VariableFilletDialog();

	private:
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;
		void updateTab();
		void init();
		
	private slots:
	    void selectActorShape(vtkActor*, int, Geometry::GeometrySet*) override;
		void on_geoSelectCurve_clicked();
		void on_buttonAdd_clicked();
		void on_addLR_clicked();

	private:
		Ui::VariableFilletDialog* _ui{};
		Geometry::GeometrySet* _edgeSet{};
		int _edgeIndex{ -1 };
		vtkActor* _actor{};
		QList<vtkActor*> _actors;
		QMap<double, double> _radiusMap{};
	};


}




#endif