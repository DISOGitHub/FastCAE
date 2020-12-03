#ifndef CREATEBOXCOMPLEXDIALOG_H_
#define CREATEBOXCOMPLEXDIALOG_H_

#include "ComplexPluginAPI.h"
#include "GeometryWidgets/geoDialogBase.h"


namespace Ui
{
	class CreateBoxComplex;
}

namespace GeometryWidget
{
	

	class COMPLEXPLUGINAPI CreateBoxComplexDialog : public GeoDialogBase
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