/******************
ProjectTreeBase操作
***************** */

#ifndef PROJECTTREEINFO_H
#define PROJECTTREEINFO_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QObject>
#include "DataProperty/modelTreeItemType.h"
#include "ConfigOptionsAPI.h"


namespace ConfigOption
{
	class TreeItem;

	class CONFIGOPTIONSAPI ProjectTreeInfo /*: public QObject*/
	{
//		Q_OBJECT
	public:
		ProjectTreeInfo(/*GUI::MainWindow *mainwindow*/) = default;
		virtual ~ProjectTreeInfo()=default;
		//ProjectTree* copy();
		void copy(ProjectTreeInfo* tree);
		void setOutputFileName(QString fileName);
		QString getOutputFileName();
		void setName(const QString &name);
		QString getName();
		void setType(ProjectTreeType type);
		ProjectTreeType type() const { return _treeType; }
		void appendItem(TreeItem* item);
// 		inline void createTree(QTreeWidgetItem* root, GUI::MainWindow* mainwindow);
// 		virtual void setData(ModelData::ModelDataBase* data);
//		ModelData::ModelDataBase* getData();
		///清空QList<TreeItem*> _itemList{} 会产生严重后果，慎重调用
		void clearTreeItems();

//		void on_MouseEvent(int eventType, QTreeWidgetItem* item);
// 		virtual void updateTree();
// 		virtual void updateTreeByType(const TreeItemType type);
//		SolveStatus getSolveStatus();
//		virtual void reTranslate();
		void setChinese(QString c);
		QString getChinese();
		void setDisableItems(QStringList s);
		QStringList getDisableItems();
		QList<TreeItem*> getItemList();
//		static bool isFileExist(QString fileName);

// 	protected slots:
// 		//mainwindow 发送的求解模型信号
// 		void solveBegin(int proIndex);
// 		virtual void solveFinished(int proid);
// 		void realTimeWindowOpened(Post::RealTimeWindowBase* w, int id);
// 		void realTimeWindowClosed(Post::RealTimeWindowBase* w);
// 
// 	signals:
// 		/*显示属性 */
// 		void disPlayProp(DataProperty::DataBase* pops);
// 		//显示参数widget
// 		void dispalyParaWidget(QWidget* w);
// 		///打开后处理窗口
// 		void openPostWindowSig(Post::PostWindowBase* w);
// 		///展示PostWindow，根据ID和类型过滤
// 		void showPostWindowInfo(int id, int type);
// 		//求解状态改变
// 		void solveStatusChanged();
// 		
// 
// 	protected:
// 		virtual void initBasicNode(QTreeWidgetItem* root);
// 
// 		virtual void singleClicked(/*QTreeWidgetItem* item, TreeItemType type */);
// 	 	virtual void doubleClicked(/*QTreeWidgetItem* item, TreeItemType type */);
// 		virtual void createContextMenu(/*QTreeWidgetItem* item, TreeItemType type */);
// 		virtual void contextMenu(QMenu* menu);
// 		///根据类型获取树节点，根节点为nullptr时遍历全树
// 		QList<QTreeWidgetItem*> getItemByType(const TreeItemType type,QTreeWidgetItem *root = nullptr);

	protected:
		QString _name{};
		ProjectTreeType _treeType{ ProjectTreeType::UnDefined };
		QList<TreeItem*> _itemList{};
		
//		QTreeWidgetItem *_root{};
//		QTreeWidgetItem* _currentItem{};
//		GUI::MainWindow* _mainWindow{};
//		QHash<QString, QTreeWidgetItem*> _textItemHash{};
//		ModelData::ModelDataBase* _data{};
//		SolveStatus _solveStatus{ UnSolved };
//		QTime* _timer{};
		QString _outputFile{};
		QString _chinese{};
		QStringList _disableItems{};
		
//		Post::RealTimeWindowBase* _realTimeWin{};
	};
}


#endif