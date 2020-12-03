#ifndef __DIALOGTHERMALBOUNDARY_H__
#define __DIALOGTHERMALBOUNDARY_H__

#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class tbDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace ThermalFluid
{
	class TFCouplingTree;
	class ThermalBoundary;
	class ThermalTabTypeDialog;
	class BoundaryTabTypeData;
	class ThermalData;

	class ThermalBoundaryDialog : public QFDialog
	{
		Q_OBJECT

		public:
			ThermalBoundaryDialog(GUI::MainWindow* mw, ThermalBoundary* tb, TFCouplingTree* tree);
			~ThermalBoundaryDialog();
			ThermalBoundary* getCurrentTb();
			ThermalData* getThermalData();
			void iniCurrentItemDlg();
			void iniEmptyDlg();				
			QString getComboxCurrentText();

		private slots:	
			void currentIndexChangedSlot(QString type);
			void createTabWidgetSlot(QString type);
			void tbBox_OkBt_ClickedSlot();
			void tbBox_CancelBt_ClickedSlot();
			void tbBox_ApplyBt_ClickedSlot();
			void on_pbGetQValue_clicked();

		private:
			bool tbDlgInformation();
			void setCurrentTbData();
			void keyPressEvent(QKeyEvent *event);

		private:
			Ui::tbDialog* _ui{};
			TFCouplingTree* _tree{};
			ThermalBoundary* _currentTb{};			
			ThermalTabTypeDialog* _ttDialog{};	
			QString _originalType{};
			bool _isNew{ false };
			bool _hasSave{ false };
	};
}



#endif