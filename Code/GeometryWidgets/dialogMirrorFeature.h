#ifndef DIALOG_MIRROR_FEATURE_H_
#define DIALOG_MIRROR_FEATURE_H_

#include "geoDialogBase.h"
#include <QMultiHash>
#include <QPair>

namespace Geometry
{
	class GeometrySet;
}

namespace Ui
{
	class MirrorFeatureDialog;
}

namespace GeometryWidget
{
	class GeoPointWidget;

	class GEOMETRYWIDGETSAPI MirorFeatureDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		MirorFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		MirorFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~MirorFeatureDialog();

	private:
		void init();
//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;
		bool getDirection(double* basePt, double* dir);

	private slots:
	    void on_TypeChanged(int index);
		void on_geoSelectSurface_clicked();
		void on_geoSelectSurface_1_clicked();
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void pointWidgetClicked(GeoPointWidget* g);
		void on_radioButtonUser();

	private:
		Ui::MirrorFeatureDialog* _ui{};
		GeoPointWidget* _pw{};

		bool _selectBody{ false };
		bool _selectPlane{ false };
		QMultiHash<Geometry::GeometrySet*, int> _bodysHash{};
		QPair <Geometry::GeometrySet*, int> _faceBodyPair{};
		int _typeindex{0};
		int _planeindex{0};

		double _basepoint[3]{};
		double _randomdir[3]{};

	};
}



#endif