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
		//������ʾʵ��
		void highLightGeometrySolidSig(Geometry::GeometrySet*, int, bool);
		//������и�������
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

		/*���²�ͬ���͵ľֲ��ܶȽ���*/
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
		/*���²�ͬ���͵ľֲ��ܶȽ���*/

		/*��Ӿֲ��ܶ����ݣ��޸ľֲ��ܶ�����*/
		void addPointsData();
		void addBoxData();
		void addBallData();
		void addCylinderData();
		void addSolidFieldsData();
		void addFrustumFieldData();
		/*��Ӿֲ��ܶ����ݣ��޸ľֲ��ܶ�����*/

		/*������Ϣչʾ*/
		void updateTab();
		void updateLocals(int n);
		void deleteItem(int row);		
		void operateLocalData(int row);
		/*������Ϣչʾ*/

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