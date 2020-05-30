#ifndef DIALOGGEOSPLITTER_H_
#define DIALOGGEOSPLITTER_H_

#include "geoDialogBase.h"

namespace Geometry
{
	class GeometrySet;
}

namespace Ui
{
	class GeoSplitterDialog;
}
class vtkActor;
namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI GeoSplitterDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		GeoSplitterDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		GeoSplitterDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~GeoSplitterDialog();

	private:
		void init();
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

 	private slots:

		/*选择分割面*/
		void on_geoSelectSurface_1_clicked();
		/*选择几何体*/
		void on_geoSelectSurface_clicked();

		void selectActorShape(vtkActor* actor, int index, Geometry::GeometrySet* set) override;

	private:
		Ui::GeoSplitterDialog* _ui{};
		bool _selectBody{ false };
		bool _selectPlane{ false };
		int _faceIndex{ -1 };
		Geometry::GeometrySet* _faceBody{};

		Geometry::GeometrySet*_body{};
		QList<vtkActor*> _faceActor{};

	};


}


#endif