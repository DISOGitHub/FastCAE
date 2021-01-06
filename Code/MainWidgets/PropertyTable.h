#ifndef _PROPERTYTAB_H_
#define _PROPERTYTAB_H_

#include <QWidget>
#include <QList>
#include <QHash>
#include <QThread>
#include "mainWidgetsAPI.h"

namespace DataProperty
{
	class DataBase;
	class ParameterBase;
}
namespace GUI
{
	class MainWindow;
}
namespace ConfigOption
{
	class ParameterObserver;
}
namespace Ui
{
	class PropTable;
}

class QTimer;
class QTableWidget;
class QTableWidgetItem;
class QPushButton;
class QSignalMapper;

namespace MainWidget
{
	class ParameterGroupLabel;
	class Monitor;

	class MAINWIDGETSAPI PropertyTable : public QWidget
	{
		Q_OBJECT
	public:
		PropertyTable(GUI::MainWindow* m, QWidget* parent = 0);
		~PropertyTable();
		//重置宽度
		void resize(int w = -1);
		//更新属性窗
		void updateTable(DataProperty::DataBase* data);
		//翻译
		void retranslate();


	private:
		void fillProp();
		void fillPara();
		void fillButton();
		void extendProp(bool extend);
		void extendPara(bool extend);
		void updateParaGroup(ParameterGroupLabel* w = nullptr);
		void clearObserver();
		void appendObserver(DataProperty::ParameterBase* p);

		private slots:
		void clickItem(QTableWidgetItem* item);
		void clickCell(int row, int col);
		void updateTable();

	private:
		Ui::PropTable* _ui{};
		GUI::MainWindow* _mainWindow{};
		QWidget* _parent{};
		DataProperty::DataBase* _data{};

		QTableWidgetItem* _propRoot{};
		QTableWidgetItem* _paraRoot{};
		QHash<QWidget*, QList<int>> _widgetRowHash{};

		QList<ConfigOption::ParameterObserver*> _observerList{};

		QList<QPushButton*> _buttonList{};
		QSignalMapper* _signalMapper{};

		Monitor* _monitor{};
	};


}

#endif