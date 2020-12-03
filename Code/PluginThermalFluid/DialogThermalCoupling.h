#ifndef __DIALOGTHERMALCOUPLING_H__
#define __DIALOGTHERMALCOUPLING_H__

#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class tcDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace ThermalFluid
{
	class TFCouplingTree;
	class ThermalCoupling;

	class ThermalCouplingDialog : public QFDialog
	{
		Q_OBJECT

		public:
			ThermalCouplingDialog(GUI::MainWindow* mw, ThermalCoupling* tc, TFCouplingTree* tree);
			~ThermalCouplingDialog();
			void iniCurrentItemDlg();
			void iniEmptyDlg();

		private slots:
			void tcBox_OkBt_ClickedSlot();
			void tcBox_CancelBt_ClickedSlot();
			void tcBox_ApplyBt_ClickedSlot();

		private:
			bool tcDlgInformation();
			void setCurrentTcData();
			void keyPressEvent(QKeyEvent *event);

		private:
			Ui::tcDialog* _ui{};
			TFCouplingTree* _tree{};
			ThermalCoupling* _currentTc{};
			bool _isNew{ false };
			bool _hasSave{ false };
	};
}

#endif