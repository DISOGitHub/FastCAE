#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include <QTreeWidget>
#include "preWindow.h"

namespace MeshData
{
	class MeshData;
	class MeshSet;
	class MeshKernal;

}

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class DataBase;
}

class QTreeWidgetItem;

namespace MainWidget
{
	class PreWindow;

	class MeshWidget : public QTreeWidget
	{
		Q_OBJECT
	public:
		MeshWidget(GUI::MainWindow* mw, QWidget* parent = nullptr);
		~MeshWidget();
		//ע�Ὺ�ſ��ýӿ�
		void registerEnabledModule();

	signals:
		//������ʾ״̬
		void updateDisplay(int index,bool visable);
		void updateMeshSetVisible(MeshData::MeshSet*);
		//�Ƴ���index��Kernal
		void removeMeshData(int index);
		void removeSetData(int index);
		//�������
		void higtLightSet(MeshData::MeshSet* set);
		//����Kernal
		void higtLightKernal(MeshData::MeshKernal* k);
		//��������
		void disPlayProp(DataProperty::DataBase* pops);
		//����״̬
		void updateActionStates();
		//��ʾ��������
		void dispalyParaWidget(QWidget*);
		void updatePreMeshActor();
		void startMesherPySig(QString mesher);
		//�������
		void clearHighLight();
		//�ع��༭
		void editMesh(int, int);
		//��ɾ��һ�����ʱ����Ӧ�������е����ҲӦ��ɾ��
		void removeCaseComponentSig(int componentID);
		//�����������е���(���item�ͱ߽�item)
		void renameCaseComponentSig(int componentID);

	//	void showDialog(QDialog*);

	public slots :
		//������
		void updateTree();
		//��������kernal����
		void updateMeshTree();
		//�����������
		void updateMeshSetTree();
		//��굥���¼�
		void singleClicked(QTreeWidgetItem*, int);
		//�Ƴ���ǰKernal
		void removeMeshData();
		///�Ƴ���ǰ���
		void removeCurrSetData();
		//�༭��ǰkernal
		void editMeshData();
		//
		void preWindowOpened(MainWidget::PreWindow* p);

	private slots:
		void startMesher(QString mesher);
		//����ȫ��
		void hideAll();
		//��ʾȫ��
		void showAll();
		//�ϲ����г�
		void mergeSet();
		//������
		void rename();
		//�ڵ�״̬�ı�
		void itemStatesChanged(QTreeWidgetItem*, int);

	private:
		void contextMenuEvent(QContextMenuEvent *event) override;
		bool isMeshEditable();

	private:
		GUI::MainWindow* _mainWindow{};
		MeshData::MeshData* _data{};
		QTreeWidgetItem* _meshRoot{};
		QTreeWidgetItem* _setRoot{};
		QTreeWidgetItem* _currentItem{};

		PreWindow* _preWindow{};
	};

}

#endif
