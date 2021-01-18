#ifndef EDITORBOUNDARYMODEL_H
#define EDITORBOUNDARYMODEL_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QCheckBox>
#include <QStringList>
#include <QSignalMapper>
#include <QList>
#include <QMap>

namespace Ui {
class EditorBoundaryModel;
}

namespace FastCAEDesigner
{
	class BoundaryModel;
}

namespace FastCAEDesigner
{

	class EditorBoundaryModel : public QDialog
	{
		Q_OBJECT

	public:
		EditorBoundaryModel(QTreeWidgetItem* treeItem, BoundaryModel* model, QWidget *parent = 0);
		~EditorBoundaryModel();

	private slots:
		void OnBtnOkClicked();
		void OnChkCommonConditionClicked(bool b);
		void OnChkCommonBoundaryClicked(bool b);
		void OnCheckboxGroupItemClicked(int index);

	private:
		void Init();
		void InitCheckBoxSlot();
		void UpdateDataToUi();
		void UpdateUiToData();
		void SetIsEdit(bool b);
		QString CreateBoundaryConditionParameter();
		void CreateConditionParameterQStringList(QStringList &pList, QList<QCheckBox*> chkList);
		void SetCheckBoxListState(QList<QCheckBox*> itemList, bool b);
		void InitCheckBoxGroup();
		bool InitCheckboxState(QStringList list, QList<QCheckBox*> chkList);
		bool GetCheckBoxGroupState(int groupIndex);
		bool GetCheckBoxGroupState(QList<QCheckBox*> itemList);
		void ConnectCheckBoxGroupSlot(int groupIndex, QList<QCheckBox*> itemList, QSignalMapper *signalMapper);


	private:
		Ui::EditorBoundaryModel *ui;
		BoundaryModel* _model;
		QTreeWidgetItem* _treeItem;
		QList<QCheckBox*> _commonConditionChkboxList;
		QList<QCheckBox*> _commonBoundaryChkboxList;
		QMap<QCheckBox*, QString> _keyDict;
	};

}
#endif // EDITORBOUNDARYMODEL_H
