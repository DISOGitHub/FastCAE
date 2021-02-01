#ifndef DIALOGMOVEFEATURE_H_
#define DIALOGMOVEFEATURE_H_

#include "geoDialogBase.h"

namespace Geometry
{
	class GeometrySet;
}

namespace Ui
{
	class MoveFeatureDialog;
}


namespace GeometryWidget
{
	class GeoPointWidget;

	class GEOMETRYWIDGETSAPI MoveFeatureDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		MoveFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		MoveFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~MoveFeatureDialog();

	private:
		void init();
//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;
		bool getVector(double* vec);

 	private slots:
 		void on_TypeChanged(int index);
 		void on_geoSelectSurface_clicked();
		void pointWidgetClicked(GeoPointWidget* w);
		void on_radioButtonUser();
		void shapeSlected(Geometry::GeometrySet* set, int index) override;

	private:
		Ui::MoveFeatureDialog* _ui{};
		GeoPointWidget*  _baseWidget{};
		GeoPointWidget* _endWidget{};

		bool _selectBody{ false };
		QMultiHash<Geometry::GeometrySet*, int> _bodysHash{};
		int _optionindex{};
		double _dir[3]{};




	};


}


#endif