#ifndef DIALOGBOOLOPERATION_H_
#define DIALOGBOOLOPERATION_H_

#include "geoDialogBase.h"
#include "GeometryCommand/GeoCommandBool.h"

namespace Ui
{
	class BoolOptionDialog;
}

namespace Geometry
{
	class GeometryData;
	class GeometrySet;
}

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI BoolOpertionDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		BoolOpertionDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		BoolOpertionDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~BoolOpertionDialog();
		void setType(BoolType t);

	private slots:
	    void on_geoSelectSurface_clicked();
	    void on_geoSelectSurface_1_clicked();
		void shapeSlected( Geometry::GeometrySet*, int) override;

	private:
		
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;
		QString booltypeToString(BoolType n);
	

	private:
		Ui::BoolOptionDialog* _ui{};
		BoolType _type{ BoolNone };

		bool _selectBody1{ false };
		bool _selectBody2{ false };

		QPair<Geometry::GeometrySet*, int> _bodypair1{};
		QPair<Geometry::GeometrySet*, int> _bodypair2{};

	};
}




#endif