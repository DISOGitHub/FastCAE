#ifndef CREATECYLINDRICALCOMPLEXDIALOG_H_
#define CREATECYLINDRICALCOMPLEXDIALOG_H_

#include "ComplexPluginAPI.h"
#include "GeometryWidgets/geoDialogBase.h"

namespace Ui
{
	class CreateCylindricalComplex;
}

namespace GeometryWidget
{
	

	class COMPLEXPLUGINAPI CreateCylindricalComplexDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		CreateCylindricalComplexDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CreateCylindricalComplexDialog();
	
	private:
		
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;


	private:
		Ui::CreateCylindricalComplex* _ui{};

	
	};
}



#endif