#ifndef LOCALSETTING_DIALOG_H_
#define LOCALSETTING_DIALOG_H_

#include <QDialog>
#include <QList>

class QTableWidget;

namespace Ui
{
	class LocalSettingDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PreWindow;
}

namespace GeometryWidget
{
	class GeoPointWidget;
}

namespace Gmsh
{
	class LocalPoint;
	class GmshDialogBase;

	class LocalSettingDialog : public QDialog
	{
		Q_OBJECT
	public:
		LocalSettingDialog(GmshDialogBase*  parent,  GUI::MainWindow* m, MainWidget::PreWindow* p);
		~LocalSettingDialog();

	private slots:
		void on_PointAddButton_clicked();
		void on_closeButton_clicked();
		void deleteItem(int, int);

	private:
		void updateTab();
		void showEvent(QShowEvent * e) override;

	private:
		Ui::LocalSettingDialog* _ui{};
		QList<LocalPoint*>* _points{};
		GeometryWidget::GeoPointWidget* _pointWidget{};
		QTableWidget* _tab{};
	};
}



#endif