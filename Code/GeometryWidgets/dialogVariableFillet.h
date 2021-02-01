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
//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;
		void updateTab();
		void init();
		
	private slots:
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void on_geoSelectCurve_clicked();
		void on_buttonAdd_clicked();
		void on_addLR_clicked();
		void on_removeButton_clicked();
		void tableClicked(int row, int col);

	private:
		Ui::VariableFilletDialog* _ui{};

		QPair<Geometry::GeometrySet*, int> _edgpair{};
		QMap<double, double> _radiusMap{};
		QPair<double, double> _selectRow{};
	};


}




#endif