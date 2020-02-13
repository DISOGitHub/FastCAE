/*********************************
ProjectWindow
2017.09.12
libaojun
*************************************/

#ifndef _PROJECTWINDOWBASE_H_
#define _PROJECTWINDOWBASE_H_

#include "moduleBaseAPI.h"
#include "dockWidgetBase.h"

namespace Ui
{
	class ProjectWindow;
}
namespace GUI
{
	class MainWindow;
}

namespace ModuleBase
{
	class ModelTreeWidgetBase;

	class MODULEBASEAPI ProjectWindowBase : public DockWidgetBase//public QDockWidget, public CModuleBase
	{
		Q_OBJECT
	public:
		ProjectWindowBase(GUI::MainWindow *mainwindow);
		virtual ~ProjectWindowBase();
		virtual void reTranslate() override;

	protected:
		/*添加模型树*/
		void addTreeWidget(ModelTreeWidgetBase* treewidget);
		/*设置属性窗口的头*/
		void setHorizontalHeader(const QString label1, const QString label2);
		/*设置属性窗口的头*/
		void setHorizontalHeader(const QStringList _lables);

		protected slots:
		virtual void on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int proID);

	protected:
		Ui::ProjectWindow* _ui{};
		ModelTreeWidgetBase* _modelTreeWidget{};

	private:
		void Init();
	};

}

#endif
