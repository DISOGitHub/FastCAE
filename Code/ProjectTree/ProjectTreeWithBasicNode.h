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
namespace Geometry
{
	class GeoComponent;
}

namespace ProjectTree
{
	
	class PROJECTTREEAPI ProjectTreeWithBasicNode : public ProjectTreeBase
	{
		Q_OBJECT
	public:
		ProjectTreeWithBasicNode(GUI::MainWindow* mainwindow);
		~ProjectTreeWithBasicNode();
		//�鿴��ͼ
		void viewCounterPost(QString variable);
		//�鿴����
		void viewVectorPost(QString variable);
		virtual void reTranslate() override;
		virtual void updateTree() override;
		//���������ڵ�
		virtual void updateGeometrySubTree();
		virtual void updateComponentSubTree();
//		virtual void updateMeshSetSubTree();
//		virtual void updateGeoComponentTree();
		virtual void updateBCSubTree();
		virtual void updateReportTree();
		virtual void updateMonitorTree();
		virtual void updatePostTree();
		//��ȡ���������ID(������������ͼ������)
		const QList<int>& getComponentIDList();
		//ͨ�����IDɾ�������е����������
		void removeCaseComponentByID(int);
		//ͨ�����ID�����������е����������
		void renameCaseComponentByID(int);

	signals:
		void highLightSet(MeshData::MeshSet* set);
		void openRealTimeWin(Post::RealTimeWindowBase* w, int id);
		void highLightGeoComponent(Geometry::GeoComponent*);
		void clearAllHighLightSig();
		void addComponentRootItemSig();

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

		//bool readInForm();

	protected slots:
	    virtual void importGeometry();
		virtual void importComponents();
		virtual void removeItem();
		virtual void addBC();
		virtual void createReport();
		virtual void assignMaterial();
		virtual void viewRealTimeWindow();
		virtual void viewPlot2D(QString variable);
		virtual void viewCounter(QString variable);
		virtual void viewVector(QString variable);
		virtual void closePostWindow(Post::PostWindowBase* w);
		void addComponentRootItemSlot();

	protected:
		QTreeWidgetItem* _geometryRootItem{};
		QTreeWidgetItem* _ComponentRootItem{};
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

		QList<QTreeWidgetItem*> _ComponentItems;
	};


}

#endif
