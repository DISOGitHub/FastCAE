#ifndef LOCALSETTING_DIALOG_H_
#define LOCALSETTING_DIALOG_H_

#include <QDialog>
#include <QList>
#include <QButtonGroup>
#include <QMultiHash>

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

namespace Geometry{
	class GeometrySet;
}

enum LocalSettingType
{
	UnDefineType = 0,
	Points,
	FieldsBox,
	FieldsBall,
	FieldsCylinder,
	FieldsSolid,
	FieldsFrustum,
};

namespace Gmsh
{
	class LocalPoint;
	class GmshDialogBase;
	class LocalField;
	class LocalDensity;
	class Cylinder;

	class LocalSettingDialog : public QDialog
	{
		Q_OBJECT
	public:
		LocalSettingDialog(GmshDialogBase*  parent,  GUI::MainWindow* m, MainWidget::PreWindow* p);
		~LocalSettingDialog();

	signals:
		void setSelectMode(int);
		//高亮显示实体
		void highLightGeometrySolidSig(Geometry::GeometrySet*, int, bool);
		//清空所有高亮对象
		void clearGeometryHighLightSig();

	private slots:
		void on_PointAddButton_clicked();
		void on_closeButton_clicked();
		void on_geoSelectSurface_clicked();
		void updateInterface(int type);
		void onCustomChecked();
		void changeData(int, int);
		void shapeSlected(Geometry::GeometrySet* set, int index);

	private:
		void showEvent(QShowEvent * e) override;

		/*更新不同类型的局部密度界面*/
		void updatePointsInterface();
		void updateBoxInterface();
		void updateBallInterface();
		void updateCylinderInterface();
		void updateCylinderAxisData(double xa, double ya, double za);
		void updateValueInterface(int n);
		void showValueInterface(int n);
		void hideValueInterface(int n);
		void updateSolidFieldInterface();
		void updateFrustumFieldInterface();
		void clearInterfaceData();
		void cylinderSelectVisiable(bool b = false);
		void cylinderAxisVisiable(bool b = false);
		/*更新不同类型的局部密度界面*/

		/*添加局部密度数据，修改局部密度数据*/
		void addPointsData();
		void addBoxData();
		void addBallData();
		void addCylinderData();
		void addSolidFieldsData();
		void addFrustumFieldData();
		/*添加局部密度数据，修改局部密度数据*/

		/*数据信息展示*/
		void updateTab();
		void updateLocals(int n);
		void deleteItem(int row);		
		void operateLocalData(int row);
		/*数据信息展示*/

	private:
		Ui::LocalSettingDialog* _ui{};
		QList<LocalPoint*>* _points{};
		GeometryWidget::GeoPointWidget* _pointWidget{};
		QTableWidget* _tab{};
		QList<LocalField*>* _fields{};
		LocalSettingType _localType{ Points };
		QList<LocalDensity*>* _locals{};
		LocalDensity* _currentLocal{};
		bool _selectSolid{ false };

		QMultiHash<Geometry::GeometrySet*, int> _solidHash{};
	};
}



#endif