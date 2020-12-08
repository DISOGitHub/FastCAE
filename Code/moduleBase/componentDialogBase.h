#ifndef COMPONENTSELECTDIALOGBASE_H
#define COMPONENTSELECTDIALOGBASE_H

#include "moduleBase/moduleBaseAPI.h"
#include <QMap>
#include <QString>
#include "SelfDefObject/QFDialog.h"
//#include "DialogBase.h"

class QBoxLayout;

namespace DataProperty
{
	class DataBase;
}
namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class ComponentDialogBase;
}

namespace ModuleBase
{
	class MODULEBASEAPI ComponentSelectDialogBase : public QFDialog
	{
		Q_OBJECT
	public:
		ComponentSelectDialogBase(GUI::MainWindow* mainwindow, DataProperty::DataBase* avaliableData, DataProperty::DataBase* selectData);
		~ComponentSelectDialogBase();

	protected:
		///ΪAvailable�б����Item
		void appendItemToAvailableList(QString name, int id, QString icon = "");
		///Ϊselecte�б����Item
		void appendItemToSelectedList(QString name, int id, QString icon = "");
		///��ȡѡ���б�Ķ���ID������
		virtual QMap<int, QString> getSelectedItemIDNames();
		///Ӧ��
		virtual void accept() override;
		void setCustomLayout(QBoxLayout* l);

		///<MG set can selected item maximum number
		void setSelectedItemMaxNumber(int max,const QString & say);

// 		///<MG add item for the selected list
// 		void appendItemToSelectedList(QString name, int id, QString icon = "");

		///<MG set text of the add button
		void setAddButtonText(const QString & text);

		///<MG set text of the remove button
		void setRemoveButtonText(const QString & text);
	private:
		///��ʼ���б�����Ĺ��캯������Ҫ����
		virtual void init() = 0;
		

	private slots:
		virtual void on_addButton_clicked();
		virtual void on_removeButton_clicked();

	protected:
		Ui::ComponentDialogBase* _ui{};
		GUI::MainWindow* _mainWindow{};
		DataProperty::DataBase* _availableData{};
		DataProperty::DataBase* _selectedData{};
		int _selectedItemMaxSize{ 9999 };
		QString _selectedOutOfRangeWord;
	};
}


#endif
