#ifndef LOCALSETTING_DIALOG_H_
#define LOCALSETTING_DIALOG_H_

#include <QDialog>
#include <QList>
#include <QButtonGroup>

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

enum LocalType
{
	UnDefineType = 0,
	Points,
	FieldsBox,
	FieldsBall,
	FieldsCylinder,
};

namespace Gmsh
{
	class LocalPoint;
	class GmshDialogBase;
	class LocalField;

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
		void updateInterface(int type);
		void onCustomChecked();

	private:
		void updateTab();
		void showEvent(QShowEvent * e) override;
		void updatePointsInterface();
		void updateBoxInterface();
		void updateBallInterface();
		void updateCylinderInterface();
		void updateValueInterface(int n);
		void showValueInterface(int n);
		void hideValueInterface(int n);

		void addPointsData();
		void addBoxData();
		void addBallData();
		void addCylinderData();

		void updatePoints(int n);
		void updateFields(int n, int index);
		void clearInterfaceData();
		void cylinderSelectVisiable(bool b = false);
		void cylinderAxisVisiable(bool b = false);

	private:
		Ui::LocalSettingDialog* _ui{};
		QList<LocalPoint*>* _points{};
		GeometryWidget::GeoPointWidget* _pointWidget{};
		QTableWidget* _tab{};
		QList<LocalField*>* _fields{};
		LocalType _localType{ Points };
		/*QButtonGroup* _bGroup{};*/
	};
}



#endif