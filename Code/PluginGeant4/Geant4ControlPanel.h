#ifndef GEANT4CONTROLPANEL_H_
#define GEANT4CONTROLPANEL_H_

#include "Geant4PluginAPI.h"
#include "moduleBase/dockWidgetBase.h"
#include <QTreeWidget>

class QTreeWidgetItem;


namespace DataProperty
{
	class DataBase;
}

namespace Geant4
{
	
	class MaterialManager;
	
	class  Geant4ControlPanel :  public QTreeWidget
	{
		Q_OBJECT
	public:
		Geant4ControlPanel(GUI::MainWindow* m);
		~Geant4ControlPanel();
		
	signals:
		/*�������Կ� */
		void updateProperty(DataProperty::DataBase* data);

		void updateGeant4Property(int type);

		void updateGeant4Property(int type, QTreeWidgetItem * item);

		void clean();

	private:
		void init();
		void initProjectTree();
		//�Ҽ��˵�
		void contextMenuEvent(QContextMenuEvent *event) override;
		void updateDefineTree();
		void updateMaterialTree();
		void updateSolidsTree();
		void updateStructerTree();
		void updateTrackTree();
		void updateGeometryEnergyThresholdTree();

	private slots:
		//�����������¼�����
		void singleClicked(QTreeWidgetItem*item, int i);
		//����gdml
		void  importGDML();
		//����gdml
		void saveGDML();
		//�༭����
		void editItem();

		void SolveCalculate();

		void addProton();
		void addElectron();
		void addPhoton();
		void removeTrack();

		void addAnalysis();
		void deleteAnalysis();
		void addVolume();
		void addVolumeInterface();
		void deleteVolumeInterface();
		void addHistogram();
		void deleteHistogram();
		void deleteVolume();

		void deleteGeometry();

	private:
		GUI::MainWindow* _mainWindow{};

		QTreeWidgetItem* _root{};

		/*
			��Ŀ��ؽڵ㶨��
		*/

		QTreeWidgetItem * _sourceNode{};
		QTreeWidgetItem * _physicalNode{};
		QTreeWidgetItem * _geometryNode{};
		QTreeWidgetItem * _geometryEnergyThresholdNode{};
		QTreeWidgetItem * _calculateNode{};

		QTreeWidgetItem * _sourceType{};
		QTreeWidgetItem * _sourceAngle{};
		QTreeWidgetItem * _sourcePara{};

		/*
			��Ŀ��ؽڵ㶨�����
		*/

		QTreeWidgetItem* _defineRoot{};
		QTreeWidgetItem* _materialRoot{};
		QTreeWidgetItem* _solidRoot{};
		QTreeWidgetItem* _structureRoot{};
		QTreeWidgetItem* _trackRoot{};
		
	};
}



#endif