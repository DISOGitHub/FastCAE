#ifndef EDITORCURVEMODEL_H
#define EDITORCURVEMODEL_H

#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSignalMapper>
#include <QTreeWidgetItem>
#include "Common.h"
#include <QToolButton>

namespace Ui {
class EditorCurveModel;
}
namespace ConfigOption{
	class PostCurve;
}

namespace FastCAEDesigner{
	class Post2DCurveModel;
	class Post3DFileModel;
	//class DataManager;

	class EditorCurveModel : public QDialog
	{
		Q_OBJECT

	public:
		explicit EditorCurveModel(QWidget *parent = 0);
		EditorCurveModel(QTreeWidgetItem*, Post2DCurveModel*, QWidget *parent = 0);
		EditorCurveModel(QTreeWidgetItem*, Post3DFileModel*, QWidget *parent = 0);
		~EditorCurveModel();

		//curve operation
		void Init();
		void InitFileName();
		QList<ConfigOption::PostCurve*> getCurveInfo();
		void createPushbuttonGroup(int);
		void updateCurveDataToUi();
		void insertRowtoTableWidget(QTableWidget*, int, ConfigOption::PostCurve*);
		//3d operation
		void Init_3D();
		void InitFileName_3D();
		void InitVariableList();
		void updateCurveDataToUi_3D();
		void createPushbuttonGroup_3D(int);
		void insertRowtoTableWidget_3D(int, QString,QString,QString);
		void updateRowtoTableWidget_3D(int, QString, QString, QString);
		void insertNodeScalar();
		void insertNodeVector();
		void insertCellScalar();
		void insertCellVector();
		void insertNameToList(QString,QString,QString);
		void replaceNameFromList(QString,QString, QString, QString);
		void removeNameFromList(QString, QString, QString);
		QStringList getNodeScalarList();
		QStringList getNodeVectorList();
		QStringList getCellScalarList();
		QStringList getCellVectorList();


		QList<QString> getNameUsedList();

	private:
		//Added xvdongming 2020-02-12
		//定义曲线类型的枚举变量
		enum EDataType 
		{
			ScalarNode = 1,
			VectorNode = 2,
			ScalarCell = 3,
			VectorCell = 4
		};
		//Added xvdongming 2020-02-12

		QToolButton* CreateButton(QString text, QString iconName);
		QPushButton* createDeleteButton();
		QPushButton* createEditButton();
		
		void InitCurveTypeStringName(EDataType type, QString& sDataType, QString& sType);//Added xvdongming 2020-02-12
		void  insertCurve(QStringList curveList, EDataType type); //Added xvdongming 2020-02-12

	private slots:
		//curve operation
		void deleteRow(int);
		void editRowInfo(int);
		void OnOkPBtnClicked();
		void OnInsertPBtnClicked();
		//3d operation
		void deleteRow_3D(int);
		void editRowInfo_3D(int);
		void OnOkPBtnClicked_3D();
		void OnInsertPBtnClicked_3D();

		void OnTimeout();


	private:
		//int _deletedRows = 0;
		Post2DCurveModel* _curveModel;
		Post3DFileModel* _fileModel;
		QList<ConfigOption::PostCurve*> _curveInfoList;
		QList<QString> _nameUsedList;

		QStringList _nodeScalarList{};
		QStringList _cellScalarList{};
		QStringList _nodeVectorList{};
		QStringList _cellVectorList{};

		QTreeWidgetItem* _treeWidgetItem;
		

	private:
		Ui::EditorCurveModel *ui;
	};
}
#endif // DATFILE_H
