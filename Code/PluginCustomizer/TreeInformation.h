#ifndef TREEINFORMATION_H
#define TREEINFORMATION_H

#include <QDialog>
#include <QTreeWidgetItem>


namespace Ui {
class TreeInformation;
}

namespace FastCAEDesigner
{
	class ModelBase;
}

namespace FastCAEDesigner
{

	class TreeInformation : public QDialog
	{
		Q_OBJECT

	public:
		enum EWorkMode
		{
			EditMode = 1,
			CreateChilldNodeMode,
			CreateRootNodeMode,
		};

	public:
		TreeInformation(QWidget *parent = 0);
		TreeInformation(QTreeWidgetItem* treeItem, ModelBase* model, QWidget* parent = 0);
		~TreeInformation();

		void setNodeName(QString, QString);
		void setNodeIcon(QString);
		void setNodeEnable();
		
		QString GetChineseName();
		QString GetEnglishName();
		QString GetIconName();

		void SetWorkModel(EWorkMode model);
		void SetUsedChnNameList(QList<QString> nameList);
		void SetUsedEngNameList(QList<QString> nameList);

	private:
		void UpdateDataToUi();
		void UpdateUiToData();
		void Init();
		void SetIsEdit(bool b);
		void InitErrorList();
		int IsDataOk();
		//void CreateRootOkClicked();
		//void CreateChildOkClicked();
		//void EditOkClicked();
		bool FileNameIsAllow(QString fileName);

	private slots:
		void OnBtnOkClicked();
		void OnBtnLoadIconClicked();
		void OnTimeout();

	private:
		enum InputErrorCode
		{
			ChnNameIsEmpty = 1,
			EngNameIsEmpty,
			TheChnNameInUse,
			TheEngNameInUse,
			NameNotAllow,
			ChnNameNotAllow,
			EngNameNotAllow,
		};

	private:
		Ui::TreeInformation *ui;
		ModelBase* _model{nullptr};
		QTreeWidgetItem* _treeItem{nullptr};
		EWorkMode _workMode{ EditMode };
		QHash<int, QString> _errorList{};
		QList<QString> _usedChnNameList;
		QList<QString> _usedEngNameList;
		//20200324
		//QList<QString> _iconNameList;
	};
}

#endif // TREEINFO_H
