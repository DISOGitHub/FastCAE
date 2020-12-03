#ifndef __DIALOGTHERMALMATERIAL_H__
#define __DIALOGTHERMALMATERIAL_H__

#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class tmDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace ThermalFluid
{
	class TFCouplingTree;
	class TFMaterial;

	class ThermalMaterialDialog : public QFDialog
	{
		Q_OBJECT

		public:
			ThermalMaterialDialog(GUI::MainWindow* mw, TFMaterial* tm, TFCouplingTree* tree);
			~ThermalMaterialDialog();
			void iniCurrentItemDlg();
			void iniEmptyDlg();

		private slots:
			void tmBox_OkBt_ClickedSlot();
			void tmBox_CancelBt_ClickedSlot();
			void tmBox_ApplyBt_ClickedSlot();

		private:
			bool tmDlgInformation();
			void setCurrentTmData();
			void keyPressEvent(QKeyEvent *event);

		private:
			Ui::tmDialog* _ui{};
			TFCouplingTree* _tree{};
			TFMaterial* _currentTm{};
			bool _isNew{ false };
			bool _hasSave{ false };
	};
}



#endif