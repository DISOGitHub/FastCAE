#ifndef __DIALOGTHERMALTABTYPE_H__
#define __DIALOGTHERMALTABTYPE_H__

#include "SelfDefObject/QFDialog.h"

class QTableWidgetItem;

namespace GUI
{
	class MainWindow;
}

namespace Ui
{
	class ttDialog;
}

namespace SelfDefObj
{
	class ParaTableWidget;
}

namespace ThermalFluid
{
	class BoundaryTabTypeData;
	class ThermalBoundaryDialog;

	class ThermalTabTypeDialog : public QFDialog
	{
		Q_OBJECT

		public:
			ThermalTabTypeDialog(GUI::MainWindow* mw, ThermalBoundaryDialog* tbDlg);
			~ThermalTabTypeDialog();
			void writeTbDataToTable();
			void writeEmptyDataToTable();
			void init(const QString& type);
			bool hasEmptyItem();
			BoundaryTabTypeData* saveOriTabTypeData();
			BoundaryTabTypeData* saveNewTabTypeData();
			int getMaxTabID();
			bool ttDlgInformation();
			BoundaryTabTypeData* iniATabTypeDataFromTableWidget();
			void setTabDataType(const QString& type);

		private:
			void writeARowData(QStringList& list, int row);
			void keyPressEvent(QKeyEvent *event);

		private slots:
			void itemChangedSlot(QTableWidgetItem* item);
			void createRowMenuSlot(QPoint point);
			void actionInsertRowSlot();
			void actionRemoveRowSlot();

		private:
			Ui::ttDialog* _ui{};
			SelfDefObj::ParaTableWidget* _tableWidget{};
			ThermalBoundaryDialog* _tbDlg{};
			BoundaryTabTypeData* _oldTabData{};
			BoundaryTabTypeData* _tmpTabData{};
			int _currentRow{ -1 };
	};
}
#endif