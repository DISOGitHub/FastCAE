#ifndef DIALOGMAKEMATRIX_H_
#define DIALOGMAKEMATRIX_H_

#include "geoDialogBase.h"

namespace Geometry
{
	class GeometrySet;
}

namespace Ui
{
	class MakeMatrixDialog;
}
namespace GeometryWidget
{
	class GeoPointWidget;
	class GEOMETRYWIDGETSAPI MakeMatrixDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		MakeMatrixDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		MakeMatrixDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~MakeMatrixDialog();

	private:
		void init();
//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;
		bool getVector(double* vec);
		QList<double*> getAngleList();
		void getDir1(double* dir);
		void getDir2(double* dir);

 	private slots:
		void on_radioButtonUser();
		void on_radio2ButtonUser();
		void on_radio3ButtonUser();
		void on_TypeChanged(int index);
		void on_geoSelectSurface_clicked();
		void showLinearDir2InfoChbox();
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void pointWidgetClicked(GeoPointWidget* w);

	private:
		Ui::MakeMatrixDialog* _ui{};
		GeoPointWidget*  _baseWidget{};
	
		bool _selectBody{ false };
		QMultiHash<Geometry::GeometrySet*, int> _bodysHash{};
		double _axisdir[3]{};

		bool _selectLinear{ true };
		bool _selectWire{ false };
	};


}


#endif