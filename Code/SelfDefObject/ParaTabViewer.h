#ifndef _PARATABVIEWER_H_
#define _PARATABVIEWER_H_

#include "QFDialog.h"
#include "SelfDefParaWidgetBase.h"
// #include <vector>
// using namespace std;

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class ParameterTable;
}
namespace Ui
{
	class ParaTabViewer;
}

namespace SelfDefObj
{
	class ParaTableWidget;

	class ParaTabViewer : public QFDialog, public SelfDefParaWidgetBase
	{
		Q_OBJECT
	public:
		ParaTabViewer(GUI::MainWindow* m, DataProperty::ParameterTable* data);
		~ParaTabViewer();

	private:
		void init();
		void apply();
		
	private slots:
		void on_out_OKButton_clicked();
		void on_out_applyButton_clicked();
		void on_out_cancelButton_clicked();
		void on_out_importButton_clicked();
		void on_out_exportButton_clicked();
	
	private:
		DataProperty::ParameterTable* readCSV(QString fileName);
		void fillTab(DataProperty::ParameterTable* table);
		QString generateCode() override;

	private:
		Ui::ParaTabViewer* _ui{};
		ParaTableWidget* _tableWidget{};
		DataProperty::ParameterTable* _data{};
//		vector<QString>linesStr;
	};
}

#endif