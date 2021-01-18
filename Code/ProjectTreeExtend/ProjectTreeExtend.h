#include "ProjectTree/ProjectTreeWithBasicNode.h"
#include "ProjectTreeExtendAPI.h"

class QMenu;
class QSignalMapper;

namespace ProjectTree
{
	class PROJECTTREEEXTENDAPI ProjectTreeExtend :public ProjectTreeWithBasicNode
	{
		Q_OBJECT
	public:
		ProjectTreeExtend(GUI::MainWindow* mainwindow);
		~ProjectTreeExtend();

	protected:
		//鼠标右键事件
		virtual void contextMenu(QMenu* menu) override;
		//鼠标左键单击事件
		virtual void singleClicked() override;
		//鼠标左键双击事件
		virtual void doubleClicked() override;
		//创建树
		virtual void createTree(QTreeWidgetItem* root, GUI::MainWindow* mainwindow) override;
		//更新网格子树
		virtual void updateComponentSubTree() override;

	private slots:
     	void rootConMenu(QString );
		void itemConMenu(QString);

	private:
		QSignalMapper* _rootMapper{};
		QSignalMapper* _conMapper{};
	};


}