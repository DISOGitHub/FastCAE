#ifndef _DIALOGCREATECONE_H_
#define _DIALOGCREATECONE_H_

#include "geoDialogBase.h"


namespace Ui
{
	class CreateCone;
}

namespace GeometryWidget
{

	class GeoPointWidget;

	class GEOMETRYWIDGETSAPI CreateConeDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateConeDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		CreateConeDialog(GUI::MainWindow*m, MainWidget::PreWindow*p, Geometry::GeometrySet*set);
		~CreateConeDialog();

	private:
		void init();
//		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;

	private slots:
		void on_radioButtonUser();
	

	private:
		Ui::CreateCone* _ui{};
		GeoPointWidget* _pw{};

	};


}




#endif