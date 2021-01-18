#ifndef DIALOGMESHCHECKING_H
#define DIALOGMESHCHECKING_H

#include "SelfDefObject/QFDialog.h"
#include "mainWidgetsAPI.h"
#include <vtkCellType.h>
#include <QColor>

namespace Ui
{
	class MeshCheckingDialog;
}
namespace GUI
{
	class MainWindow;
}
namespace MeshData
{
	class MeshChecker;
}
namespace Post
{
	class PostCustomPlot;
}

class vtkDataSet;

namespace MainWidget
{
	class MAINWIDGETSAPI MeshCheckingDialog : public QFDialog
	{
		Q_OBJECT
	public:
		MeshCheckingDialog(GUI::MainWindow* m);
		~MeshCheckingDialog();

	signals:
		void highLight(vtkDataSet* d);
		void setDisplay(QString);

	private:
		void init();
		void refreshBasicInfo();
		void updatePlot();
		void closeEvent(QCloseEvent *e);

	private slots:
		void checkerFinished();
		void on_checkButton_clicked();
		void on_closeButton_clicked();
		void on_checkbox_changed();

	private:
		Ui::MeshCheckingDialog* _ui{};
		Post::PostCustomPlot* _customplot{};
		MeshData::MeshChecker* _checker{};

		static QHash<VTKCellType, QColor> TYPECOLORS;

	};


}



#endif