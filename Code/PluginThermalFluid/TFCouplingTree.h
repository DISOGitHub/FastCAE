#ifndef __TFPLUGINMODELTREE_H_
#define __TFPLUGINMODELTREE_H_

#include "ThermalFluidAPI.h"
#include "ProjectTree/ProjectTreeWithBasicNode.h"

enum  TFCTreeItemType
{
	None = 0,
	ThermalRoot = 410,
	FluidRoot,
	ThermalBoundaryRoot,
	ThermalCouplingRoot,
	ThermalMaterialRoot,
	ThermalBoundaryChild,
	ThermalCouplingChild,
	ThermalMaterialChild,
};

namespace ThermalFluid
{
	class TFCouplingModelData;
	class ThermalBoundary;
	class ThermalCoupling;
	class ThermalData;
	class TFMaterial;

	class THERMALFLUIDAPI TFCouplingTree : public ProjectTree::ProjectTreeWithBasicNode
	{
		friend class ThermalBoundaryDialog;
		friend class ThermalCouplingDialog;
		friend class ThermalMaterialDialog;

		Q_OBJECT
	public:
		TFCouplingTree(GUI::MainWindow* m);
		~TFCouplingTree() = default;

		void createTree(QTreeWidgetItem* root, GUI::MainWindow* mainwindow) override;
		void updateTree() override;

	protected:
		void contextMenu(QMenu* menu) override;
		void reTranslate() override;
		void singleClicked() override;
		void doubleClicked() override;

	private slots:
		void slotInputINPF();
		void slotOutputINPF();
		//边界相关操作
		void on_actClearTbs_triggered();
		void on_actAppenedATb_triggered();
		void on_actEditTb_triggered();
		void on_actDeleteTb_triggered();
		void on_actInsertTb_triggered();
		//耦合相关操作
		void on_actClearTcs_triggered();
		void on_actAppenedATc_triggered();
		void on_actEditTc_triggered();
		void on_actDeleteTc_triggered();
		void on_actInsertTc_triggered();
		//材料相关操作
		void on_actClearTms_triggered();
		void on_actAppenedATm_triggered();
		void on_actEditTm_triggered();
		void on_actDeleteTm_triggered();
		void on_actInsertTm_triggered();

	private:
		void transferData()  override;
		void updateBoundaryTree();
		void updateCouplingTree();
		void updateMaterialTree();
		int getMaxTbID();
		int getMaxTcID();
		int getMaxTmID();
		ThermalBoundary* iniAThermalBoundary();
		ThermalCoupling* iniAThermalCoupling();
		TFMaterial* iniATFMaterial();

	private:
		TFCouplingModelData* _modelData{};
		ThermalData* _thermalData{};

		QTreeWidgetItem* _thermalRoot{};
		QTreeWidgetItem* _fluidRoot{};

		QTreeWidgetItem* _tBoundaryRoot{};
		QTreeWidgetItem* _tCouplingRoot{};
		QTreeWidgetItem* _tMaterialRoot{};
	};
}


#endif