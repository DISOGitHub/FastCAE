/*********************************
ProjectWindow
2017.09.12
libaojun
************************************ */

#ifndef _CONTROLPANEL_H_
#define _CONTROLPANEL_H_

#include "mainWidgetsAPI.h"
#include "moduleBase/dockWidgetBase.h"

class PipelineObjTreeDockWidget;
class PipelineObjPropDockWidget;

namespace Ui
{
	class ControlPanel;
}
namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class DataBase;
}
namespace MainWidget
{
	class PhysicsWidget;
	class MeshWidget;
	class GeometryTreeWidget;
	class PropertyTable;

	class MAINWIDGETSAPI ControlPanel : public ModuleBase::DockWidgetBase//public QDockWidget, public CModuleBase
	{
		Q_OBJECT
	public:
		ControlPanel(GUI::MainWindow *mainwindow);
		virtual ~ControlPanel();
		virtual void reTranslate() override;

	signals:
		void updateActionStates();
		//void updataPropertyTab(DataProperty::DataBase* popList);

	public:
		/*添加模型树 */
		//		void addTreeWidget(PhysicsWidget* treewidget);
		// 		/*设置属性窗口的头 */
		// 		void setHorizontalHeader(const QString label1, const QString label2);
		// 		/*设置属性窗口的头 */
		// 		void setHorizontalHeader(const QStringList _lables);
		///更新两个post widget
		void updatePostWidget(QWidget* tree, QWidget* prop);
		//清空属性窗widget
		void clearWidget();
		//根据配置信息更新开放接口
		void registerEnabledModule();
		//添加窗口
		void addTabWidgetPlugin(QWidget* w, QString name);
		//移除窗口
		void removeTabWidgetPlugin(QWidget* w);
		//设置当前窗口
		void setCurrentWidget(QWidget* w);

	public slots :
		//鼠标点击事件，包括左键单击双击及右键菜单
		virtual void on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int proID);
		//更新属性框
		void updataPropertyTab(DataProperty::DataBase* popList);
		//设置属性窗当前页
		void changePropTabByProjectPage(int index);
		//更新属性窗体
		void updataParaWidget(QWidget* w);
	private:
		void Init();

		void resizeEvent(QResizeEvent *);
		// 		void fillProp();
		// 		void fillPara();

	protected:
		Ui::ControlPanel* _ui{};
		PhysicsWidget* _physicsWidget{};
		GeometryTreeWidget* _geometryWidget{};
		MeshWidget* _meshWidget{};
		PropertyTable* _propTable{};

		QWidget* _postTree{};
		QWidget* _postProp{};
		DataProperty::DataBase* _data{};

		QWidget* _paraWidget{};

		QHash<QWidget*, QString>  _addinWidget{};

	};

}

#endif
