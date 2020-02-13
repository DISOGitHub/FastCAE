#ifndef _PROJECTTREEWITHBASICNODE_H_
#define _PROJECTTREEWITHBASICNODE_H_

#include "projectTreeBase.h"
#include "ProjectTreeAPI.h"

class QTreeWidgetItem;
class QSignalMapper;

namespace GUI
{
	class MainWindow;
}
namespace ModelData
{
	class ModelDataBaseExtend;
}
namespace MeshData
{
	class MeshSet;
}
namespace Post
{
	class Post2DWindowInterface;
	class Post3DWindowInterface;
	class PostWindowBase;
}

namespace ProjectTree
{
	
	class PROJECTTREEAPI ProjectTreeWithBasicNode : public ProjectTreeBase
	{
		Q_OBJECT
	public:
		ProjectTreeWithBasicNode(GUI::MainWindow* mainwindow);
		~ProjectTreeWithBasicNode();
		//查看云图
		void viewCounterPost(QString variable);
		//查看向量
		void viewVectorPost(QString variable);
		virtual void reTranslate() override;
		virtual void updateTree() override;
		//更新子树节点
		virtual void updateGeometrySubTree();
		virtual void updateMeshSubTree();
		virtual void updateBCSubTree();
		virtual void updateReportTree();
		virtual void updateMonitorTree();
		virtual void updatePostTree();


	signals:
		void highLightSet(MeshData::MeshSet* set);
		void openRealTimeWin(Post::RealTimeWindowBase* w, int id);

	protected:
		virtual void initBasicNode(QTreeWidgetItem* root) override;
		virtual void contextMenu(QMenu* menu) override;
		virtual void singleClicked() override;
		virtual void doubleClicked() override;
		virtual void setData(ModelData::ModelDataBase* data);
		virtual void d2PlotContextMenu(QMenu* menu);
		virtual void counterContextMenu(QMenu* menu);
		virtual void vectorContextMenu(QMenu* menu);
		virtual DataProperty::DataBase* getCurrentItemData();

	protected slots:
	    virtual void importGeometry();
		virtual void importMesh();
		virtual void removeItem();
		virtual void addBC();
		virtual void createReport();
		virtual void assignMaterial();
		virtual void viewRealTimeWindow();
		virtual void viewPlot2D(QString variable);
		virtual void viewCounter(QString variable);
		virtual void viewVector(QString variable);
		virtual void closePostWindow(Post::PostWindowBase* w);

	protected:
		QTreeWidgetItem* _geometryRootItem{};
		QTreeWidgetItem* _meshRootItem{};
		QTreeWidgetItem* _simulationSettingItem{};
		QTreeWidgetItem* _boundaryConditionItem{};
		QTreeWidgetItem* _solverSettingItem{};
		QTreeWidgetItem* _monitorRootItem{};
		QTreeWidgetItem* _postRootItem{};
		QTreeWidgetItem* _post2DItem{};
		QTreeWidgetItem* _postCounterItem{};
		QTreeWidgetItem* _postVectorItem{};
		QTreeWidgetItem* _postStreamLineItem{};
		QTreeWidgetItem* _post3DItem{};
		QTreeWidgetItem* _reportItem{};

		QSignalMapper* _postMapper{};
// 
// 		Post::Post2DWindowInterface* _post2DWindow{};
// 		Post::Post3DWindowInterface* _post3DWindow{};

		QMenu* _nodeMenu{};
		QMenu* _cellMeun{};

		ModelData::ModelDataBaseExtend* _modelDataExtend{};


	};


}

#endif
