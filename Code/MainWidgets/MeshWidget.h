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
		void removeSetData();

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

	private:
		GUI::MainWindow* _mainWindow{};
		MeshData::MeshData* _data{};
		QTreeWidgetItem* _meshRoot{};
		QTreeWidgetItem* _setRoot{};
		QTreeWidgetItem* _currentItem{};
	};

}

#endif
