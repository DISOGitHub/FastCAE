#ifndef GEOMETRYWIDGET_H
#define GEOMETRYWIDGET_H

//#include "moduleBaseAPI.h"
#include <QTreeWidget>

class QTreeWidgetItem;

namespace DataProperty
{
	class DataBase;
}
namespace Geometry
{
	class GeometryData;
	class GeometrySet;
	class GeometryDatum;
	class GeoComponent;
}
namespace GUI
{
	class MainWindow;
}

// namespace GenerateGeometry
// {
// 	class GenerateGeometry;
// }

// namespace GeometryWidget
// {
// 	class GeometryDialogFactory;
// }

namespace MainWidget
{
	class PreWindow;

	class /*MODULEBASEAPI*/ GeometryTreeWidget : public QTreeWidget
	{
		Q_OBJECT
	public:
		GeometryTreeWidget(GUI::MainWindow* m, QWidget* parent = nullptr);
		~GeometryTreeWidget();
		//注册开放可用接口
		void registerEnabledModule();

	signals:
		//更新显示状态
		void updateDisplay(int index, bool visable);
		//移除第index个数据
		void removeGeoData(int index);
		//显示属性
		void disPlayProp(DataProperty::DataBase* pops);
		//更新action状态
		void updateActionStates();
		//通过主窗口统一接口显示非模态对话框
		void showGeoDialog(QDialog*);
		//高亮几何
		void highLightGeometrySet(Geometry::GeometrySet*, bool);
		//清除高亮
		void clearHighLight();
		//当删除一个组件时，对应的算例中的组件也应该删除
		void removeCaseComponentSig(int componentID);
		//重命名算例中的项(组件item和边界item)
		void renameCaseComponentSig(int componentID);

	public slots:
	    //更新树
		void updateTree();
		//鼠标单击事件
		void singleClicked(QTreeWidgetItem*, int);
		//清除当前数据
		void removeData();
		void changeName();
		void slot_datumPlaneRename();
		void slot_removeDatum();
		void preWindowOpened(MainWidget::PreWindow* p);
		void preWindowClosed();
		//编辑当前形状
		void editGeometry();
		//隐藏全部
		void hideAll();
		//显示全部
		void showAll();
		//节点状态改变
		void itemStatesChanged(QTreeWidgetItem*, int);
		//移除当前组件
		void removeCurrComponent();

	private:
		void contextMenuEvent(QContextMenuEvent *event) override;
		bool isGeometryEditable();

	private:
		GUI::MainWindow* _mainWindow{};
		QTreeWidgetItem* _root{};
		QTreeWidgetItem* _datumroot{};
		QTreeWidgetItem* _gcroot{};
		Geometry::GeometryData* _data{};
		QTreeWidgetItem* _currentItem{};
	
		PreWindow* _preWindow{};


	};

}


#endif
