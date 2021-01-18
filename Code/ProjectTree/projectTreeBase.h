/******************
ProjectTreeBase操作
***************** */

#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QObject>
#include "DataProperty/modelTreeItemType.h"
#include "ProjectTreeAPI.h"

class QTreeWidgetItem;
class QTime;
class QMenu;

namespace ModelData
{
	class ModelDataBase;
}

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class DataBase;
}
namespace Post
{
	class PostWindowBase;
	class RealTimeWindowBase;
}
namespace ConfigOption
{
	class TreeItem;
	class ProjectTreeInfo;
}

namespace ProjectTree
{
	

	enum SolveStatus
	{
		UnSolved = 1,
		Solving,
		Finished
	};

	class PROJECTTREEAPI ProjectTreeBase : public QObject
	{
		Q_OBJECT
	public:
		ProjectTreeBase(GUI::MainWindow *mainwindow);
		virtual ~ProjectTreeBase();
		//ProjectTree* copy();
//		void copy(ProjectTreeBase* tree);
		//从配置文件中拷贝信息
		void copy(ConfigOption::ProjectTreeInfo* info);
		void setOutputFileName(QString fileName);
		//设置名称
		void setName(const QString &name);
		//获取名称
		QString getName();
		//设置树的类型
		void setType(ProjectTreeType type);
		//获取树的类型
		ProjectTreeType type() const{ return _treeType; }
		//根据配置项追加节点
		void appendItem(ConfigOption::TreeItem* item);
		//创建树
		virtual void createTree(QTreeWidgetItem* root, GUI::MainWindow* mainwindow);
		//设置模型数据
		virtual void setData(ModelData::ModelDataBase* data);
		//获取模型数据
		ModelData::ModelDataBase* getData();
		///清空QList<TreeItem*> _itemList{} 会产生严重后果，慎重调用
		void clearTreeItems();
		//鼠标事件
		void on_MouseEvent(int eventType, QTreeWidgetItem* item);
		//跟新树
		virtual void updateTree();
		//根据类型更新子树
		virtual void updateTreeByType(const TreeItemType type);
		//获取求解状态
		SolveStatus getSolveStatus();
		//翻译
		virtual void reTranslate();
		//设置中文名称
		void setChinese(QString c);
		//获取中文名称
		QString getChinese();
		//设置不可见节点
		void setDisableItems(QStringList s);
		//获取不可见节点
		QStringList getDisableItems();
		//设置当前item
		void setCurrentItem(QTreeWidgetItem* item);
		//文件是否存在
		static bool isFileExist(QString fileName);
		//右键菜单
		virtual void contextMenu(QMenu* menu);

	protected slots:
		//mainwindow 发送的求解模型信号
		void solveBegin(int proIndex);
		virtual void solveFinished(int proid);
		void realTimeWindowOpened(Post::RealTimeWindowBase* w, int id);
		void realTimeWindowClosed(Post::RealTimeWindowBase* w);

	signals:
		/*显示属性 */
		void disPlayProp(DataProperty::DataBase* pops);
		//显示参数widget
		void dispalyParaWidget(QWidget* w);
		///打开后处理窗口
		void openPostWindowSig(Post::PostWindowBase* w);
		///展示PostWindow，根据ID和类型过滤
		void showPostWindowInfo(int id, int type);
		//求解状态改变
		void solveStatusChanged();
		

	protected:
		virtual void initBasicNode(QTreeWidgetItem* root);

		virtual void singleClicked(/*QTreeWidgetItem* item, TreeItemType type */);
	 	virtual void doubleClicked(/*QTreeWidgetItem* item, TreeItemType type */);
		virtual void createContextMenu(/*QTreeWidgetItem* item, TreeItemType type */);
		
		///根据类型获取树节点，根节点为nullptr时遍历全树
		QList<QTreeWidgetItem*> getItemByType(const TreeItemType type,QTreeWidgetItem *root = nullptr);

	private:
		virtual void transferData(){}

	protected:
		QString _name{};
		ProjectTreeType _treeType{ ProjectTreeType::UnDefined };
		QList<ConfigOption::TreeItem*> _itemList{};
		
		QTreeWidgetItem *_root{};
		QTreeWidgetItem* _currentItem{};
		GUI::MainWindow* _mainWindow{};
		QHash<QString, QTreeWidgetItem*> _textItemHash{};
		ModelData::ModelDataBase* _data{};
		SolveStatus _solveStatus{ UnSolved };
		QTime* _timer{};
		QString _outputFile{};
		QString _chinese{};
		QStringList _disableItems{};
		
		Post::RealTimeWindowBase* _realTimeWin{};
	};
}


#endif