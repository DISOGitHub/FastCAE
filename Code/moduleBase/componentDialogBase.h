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
		///为Available列表添加Item
		void appendItemToAvailableList(QString name, int id, QString icon = "");
		///为selecte列表添加Item
		void appendItemToSelectedList(QString name, int id, QString icon = "");
		///获取选择列表的对象ID和名称
		virtual QMap<int, QString> getSelectedItemIDNames();
		///应用
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
		///初始化列表，子类的构造函数中需要调用
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
