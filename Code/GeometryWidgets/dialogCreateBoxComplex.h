#ifndef CREATEBOXCOMPLEXDIALOG_H_
#define CREATEBOXCOMPLEXDIALOG_H_

#include "geometryWidgetsAPI.h"
#include "geoDialogBase.h"


namespace Ui
{
	class CreateBoxComplex;
}

namespace GeometryWidget
{
	

	class GEOMETRYWIDGETSAPI CreateBoxComplexDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateBoxComplexDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CreateBoxComplexDialog();
	
	private:
		
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;


	private:
		Ui::CreateBoxComplex* _ui{};

	
	};
}



#endif