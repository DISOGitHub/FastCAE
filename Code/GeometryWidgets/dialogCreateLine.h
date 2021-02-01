#ifndef CREATEEDGEDIALLOG_H_
#define CREATEEDGEDIALLOG_H_

#include "geoDialogBase.h"

namespace Ui
{
	class CreateLineDialog;
}


namespace GeometryWidget
{

	class GeoPointWidget;

	class GEOMETRYWIDGETSAPI CreateLineDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateLineDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		CreateLineDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~CreateLineDialog();

	private:
		void init();
//		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;


		bool getEndPoint(double* c);
		bool getEndPointByDirection(double* ept);

	private slots:
	    void optionChanged(int index);
		void pointButtomClicked(GeoPointWidget* w);
		void on_radioButtonUser();

	private:
		Ui::CreateLineDialog* _ui{};

		GeoPointWidget* _startPoint{};
		GeoPointWidget* _endPoint{};

	};
}


#endif