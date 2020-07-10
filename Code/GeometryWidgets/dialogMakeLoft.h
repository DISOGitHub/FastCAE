#ifndef  _DIALOGMAKELOFT_H_
#define _DIALOGMAKELOFT_H_

#include "moduleBase/ModuleType.h"
#include "geoDialogBase.h"
#include <QMultiHash>

class TopoDS_Shape;

namespace Ui
{
	class CreateLoftDialog;
}


namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI CreateLoftDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateLoftDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		CreateLoftDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~CreateLoftDialog();

	private slots:
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void on_geoSelectCurve_clicked();
		void on_addButton_clicked();
		void on_removeButton_clicked();
		void tableClicked(int row, int col);

	private:
		void closeEvent(QCloseEvent *);
		void accept() override;
		void reject() override;
		void init();
		void updateTab();
		
	private:
		Ui::CreateLoftDialog* _ui{};


		QList<QMultiHash<Geometry::GeometrySet*, int>> _allShapes{};
		QMultiHash<Geometry::GeometrySet*, int> _tempShapes{};

	};



}
















































#endif 