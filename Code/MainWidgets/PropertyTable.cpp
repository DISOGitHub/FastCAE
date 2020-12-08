#include "PropertyTable.h"
#include "ui_PropertyTable.h"
#include <QHeaderView>
#include "DataProperty/DataBase.h"
#include "DataProperty/PropertyBase.h"
#include "DataProperty/ParameterGroup.h"
#include "SelfDefObject/ParaWidgetFactory.h"
#include "ParameterGroupLabel.h"
#include "ConfigOptions/ParameterObserver.h"
#include "settings/busAPI.h"
#include <QScrollBar>
#include <QTimer>
#include <QPushButton>
#include <QSignalMapper>
#include <QDebug>

namespace MainWidget
{
	PropertyTable::PropertyTable(GUI::MainWindow* m, QWidget* parent /*= 0*/)
		: _ui(new Ui::PropTable), _mainWindow(m), _parent(parent)
	{
		_ui->setupUi(this);
		_ui->propTable->setColumnCount(2);
		_ui->buttonWidget->setVisible(false);
		QStringList lables; lables << tr("Name") << tr("Value");
		_ui->propTable->setHorizontalHeaderLabels(lables);
		_ui->propTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		QHeaderView *header = _ui->propTable->verticalHeader();
		header->setHidden(true);// Òþ²ØÐÐºÅ
		_ui->propTable->resizeColumnsToContents();
		connect(_ui->propTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(clickItem(QTableWidgetItem*)));
		connect(_ui->propTable, SIGNAL(cellClicked(int, int)), this, SLOT(clickCell(int, int)));

		// 		_monitor = new Monitor(this, _ui->propTable);
		// 		_monitor->run();
	}
	PropertyTable::~PropertyTable()
	{
		_ui->propTable->clear();
		if (_ui != nullptr) delete _ui;
		delete _monitor;
		//		this->clearObserver();
	}

	void PropertyTable::resize(int w)
	{
		int totalwidth = w;
		if (w < 0) totalwidth = _parent->width();
		if (_ui->propTable->verticalScrollBar()->isVisible())
		{
			totalwidth -= _ui->propTable->verticalScrollBar()->width();
		}

		_ui->propTable->setColumnWidth(0, totalwidth / 2 - 4);
		_ui->propTable->setColumnWidth(1, totalwidth / 2 - 4);
	}

	void PropertyTable::retranslate()
	{
		QStringList lables; lables << tr("Name") << tr("Value");
		_ui->propTable->setHorizontalHeaderLabels(lables);
	}

	void PropertyTable::updateTable(DataProperty::DataBase* data)
	{
		_ui->propTable->clear();
		this->clearObserver();
		_widgetRowHash.clear();
		_ui->propTable->setRowCount(0);
		_ui->propTable->setColumnCount(2);
		QStringList lables; lables << tr("Name") << tr("Value");
		_ui->propTable->setHorizontalHeaderLabels(lables);
		_data = data;
		if (data == nullptr)
		{
			_ui->propTable->setRowCount(0);
			_ui->buttonWidget->setVisible(false);
			return;
		}
		const int nprop = data->getPropertyCount();
		int npara = data->getVisibleParaCount();
		const int ngroup = data->getVisiableParameterGroupCount();
		for (int i = 0; i < data->getParameterGroupCount(); ++i)
		{
			DataProperty::ParameterGroup* g = data->getParameterGroupAt(i);
			if (g == nullptr || !g->isVisible()) continue;
			npara += g->getVisibleParaCount();
		}

		int addline = 0;
		if (nprop > 0) addline++;
		if (npara > 0) addline++;
		_ui->propTable->setRowCount(nprop + npara + addline + ngroup);
		fillProp();
		fillPara();
		fillButton();
	}

	void PropertyTable::updateTable()
	{
		updateTable(_data);
	}

	void PropertyTable::fillProp()
	{
		const int nprop = _data->getPropertyCount();
		if (nprop < 1)return;
		_ui->propTable->setSpan(0, 0, 1, 2);
		_propRoot = new QTableWidgetItem();
		_propRoot->setText(tr("Basic Info"));
		_propRoot->setData(Qt::UserRole, 0);
		_propRoot->setIcon(QIcon(":/QUI/icon/expandL1.png"));
		_ui->propTable->setItem(0, 0, _propRoot);

		for (int i = 0; i < nprop; ++i)
		{
			auto p = _data->getPropertyAt(i);
			QString name = QString("     %1").arg(p->getName());
			_ui->propTable->setItem(i + 1, 0, new QTableWidgetItem(name));
			DataProperty::PropertyType type = p->getType();
			QTableWidgetItem*tableItem = new QTableWidgetItem(type);
			QVariant v = p->getVariant();
			tableItem->setData(0, v);

			if (tableItem != nullptr)
				_ui->propTable->setItem(i + 1, 1, tableItem);
		}
		extendProp(true);
	}

	void PropertyTable::fillPara()
	{
		this->clearObserver();
		const int nprop = _data->getPropertyCount();
		int npara = _data->getVisibleParaCount();
		int ngroup = _data->getVisiableParameterGroupCount();
		bool isEnglish = true;
		QString lang = Setting::BusAPI::instance()->getLanguage();
		if (lang.toLower() == "chinese")
			isEnglish = false;

		if (npara + ngroup == 0) return;
		int offsetRow = 0;
		if (nprop > 0) offsetRow = nprop + 1;

		_ui->propTable->setSpan(offsetRow, 0, 1, 2);
		_paraRoot = new QTableWidgetItem();
		_paraRoot->setText(tr("Parameters"));
		_paraRoot->setData(Qt::UserRole, 0);
		_paraRoot->setIcon(QIcon(":/QUI/icon/expandL1.png"));
		_ui->propTable->setItem(offsetRow, 0, _paraRoot);
		int currertrow = offsetRow + 1;
		ngroup = _data->getParameterGroupCount();
		for (int i = 0; i < ngroup; ++i)
		{
			DataProperty::ParameterGroup* g = _data->getParameterGroupAt(i);
			if (g == nullptr) continue;
			if (!g->isVisible())continue;
			_ui->propTable->setSpan(currertrow, 0, 1, 2);
			ParameterGroupLabel* l = new ParameterGroupLabel;
			l->setIcon(":/QUI/icon/expandL1.png");
			l->setText(g->getDescribe());
			_ui->propTable->setCellWidget(currertrow, 0, l);
			currertrow++;
			const int pcount = g->getParameterCount();
			QList<int> rowlist;
			for (int j = 0; j < pcount; ++j)
			{
				DataProperty::ParameterBase* p = g->getParameterAt(j);
				if ((p != nullptr && p->isVisible()))
				{
					QString text;
					if (isEnglish)
						text = p->getDescribe();
					else text = p->getChinese();
					if (text.isEmpty())
						text = p->getDescribe();
					QString name = QString("          %1").arg(text);
					_ui->propTable->setItem(currertrow, 0, new QTableWidgetItem(name));
					QWidget* w = SelfDefObj::ParaWidgetFactory::generateWidgetByPara(_mainWindow, p);
					if (w != nullptr)
						_ui->propTable->setCellWidget(currertrow, 1, w);
					rowlist.append(currertrow);
					this->appendObserver(p);
					currertrow++;

				}
			}
			_widgetRowHash[l] = rowlist;
		}

		npara = _data->getParameterCount();
		for (int i = 0; i < npara; ++i)
		{
			auto p = _data->getParameterAt(i);
			if ((!p->isVisible()) || p == nullptr) continue;
			QString text;
			if (isEnglish)
				text = p->getDescribe();
			else text = p->getChinese();
			if (text.isEmpty())
				text = p->getDescribe();
			QString name = QString("     %1").arg(text);
			_ui->propTable->setItem(currertrow, 0, new QTableWidgetItem(name));
			QWidget* w = SelfDefObj::ParaWidgetFactory::generateWidgetByPara(_mainWindow, p);
			if (w != nullptr)
				_ui->propTable->setCellWidget(currertrow, 1, w);
			this->appendObserver(p);
			currertrow++;
		}
		_ui->propTable->setRowCount(currertrow);
		//		this->extendPara(true);
	}

	void PropertyTable::fillButton()
	{
		//clear
		for (int i = 0; i < _buttonList.size(); ++i)
		{
			QPushButton* b = _buttonList.at(i);
			_ui->buttonLayout->removeWidget(b);
			delete b;
		}
		_buttonList.clear();
		if (_signalMapper != nullptr)
		{
			delete _signalMapper;
			_signalMapper = nullptr;
		}

		//fill && connect

		bool isshow = _data->isContainsButton();
		_ui->buttonWidget->setVisible(isshow);
		if (!isshow) return;
		bool isEnglish = true;
		QString lang = Setting::BusAPI::instance()->getLanguage();
		if (lang.toLower() == "chinese")
			isEnglish = false;
		_signalMapper = new QSignalMapper;
		QStringList buttonText;
		if (isEnglish)
			buttonText = _data->getButtonText();
		else
			buttonText = _data->getButtonChinese();
		if (buttonText.isEmpty())
			buttonText = _data->getButtonText();

		for (int i = 0; i < buttonText.size(); ++i)
		{
			QString t = buttonText.at(i);
			QPushButton* button = new QPushButton(t);
			_ui->buttonLayout->addWidget(button);
			_buttonList.append(button);
			connect(button, SIGNAL(clicked()), _signalMapper, SLOT(map()));
			_signalMapper->setMapping(button, t);
		}
		connect(_signalMapper, SIGNAL(mapped(QString)), _data, SLOT(onButtonClicked(QString)));

	}

	void PropertyTable::clickItem(QTableWidgetItem* item)
	{
		if (item == nullptr) return;
		if (item == _propRoot)
		{
			int clickcount = item->data(Qt::UserRole).toInt();
			clickcount++;
			item->setData(Qt::UserRole, clickcount);
			if (clickcount % 2 == 0)
			{
				this->extendProp(true);
			}
			else
			{
				this->extendProp(false);
			}
		}
		else if (item == _paraRoot)
		{
			int clickcount = item->data(Qt::UserRole).toInt();
			clickcount++;
			item->setData(Qt::UserRole, clickcount);
			if (clickcount % 2 == 0)
			{
				this->extendPara(true);
			}
			else
			{
				this->extendPara(false);
			}
		}
	}

	void PropertyTable::extendProp(bool extend)
	{
		QString icon = "://QUI/icon/normalL1.png";
		if (extend) icon = ":/QUI/icon/expandL1.png";
		_propRoot->setIcon(QIcon(icon));
		const int nprop = _data->getPropertyCount();
		for (int i = 1; i <= nprop; ++i)
		{
			if (extend)
				_ui->propTable->showRow(i);
			else
				_ui->propTable->hideRow(i);

		}
	}

	void PropertyTable::extendPara(bool extend)
	{
		QString icon = "://QUI/icon/normalL1.png";
		if (extend) icon = ":/QUI/icon/expandL1.png";
		_paraRoot->setIcon(QIcon(icon));
		int begin = _ui->propTable->row(_paraRoot);
		int end = _ui->propTable->rowCount();
		for (int i = begin + 1; i <= end; ++i)
		{
			if (extend)
				_ui->propTable->showRow(i);
			else
				_ui->propTable->hideRow(i);
		}
		if (extend)
		{
			updateParaGroup();
		}
	}

	void PropertyTable::updateParaGroup(ParameterGroupLabel* lw)
	{
		if (lw == nullptr)
		{
			QList<QWidget*> widgetlist = _widgetRowHash.keys();
			const int n = widgetlist.size();
			for (int i = 0; i < n; ++i)
			{
				QWidget* w = widgetlist.at(i);
				ParameterGroupLabel* l = dynamic_cast<ParameterGroupLabel*>(w);
				QList<int> rows = _widgetRowHash.value(w);
				const int rowCount = rows.size();
				bool isextend = l->isExtend();
				for (int j = 0; j < rowCount; ++j)
				{
					int row = rows.at(j);
					if (isextend)
						_ui->propTable->showRow(row);
					else
						_ui->propTable->hideRow(row);
				}
			}
		}
		else
		{
			QList<int> rows = _widgetRowHash.value(lw);
			const int rowCount = rows.size();
			bool isextend = lw->isExtend();
			for (int j = 0; j < rowCount; ++j)
			{
				int row = rows.at(j);
				if (isextend)
					_ui->propTable->showRow(row);
				else
					_ui->propTable->hideRow(row);

			}
		}
	}

	void PropertyTable::clickCell(int row, int col)
	{
		QWidget* w = _ui->propTable->cellWidget(row, col);
		if (w == nullptr) return;
		ParameterGroupLabel* lw = dynamic_cast<ParameterGroupLabel*>(w);
		if (lw == nullptr) return;
		lw->clicked();
		updateParaGroup(lw);

	}

	void PropertyTable::clearObserver()
	{
		const int n = _observerList.size();
		for (int i = 0; i < n; ++i)
		{
			ConfigOption::ParameterObserver* obs = _observerList.at(i);
			disconnect(obs, SIGNAL(updateDisplay()), this, SLOT(updateTable()));
		}
		_observerList.clear();
	}

	void PropertyTable::appendObserver(DataProperty::ParameterBase* p)
	{
		QList<ConfigOption::ParameterObserver*> obslist = p->getObserverList();

		const int n = obslist.size();
		for (int i = 0; i < n; ++i)
		{
			ConfigOption::ParameterObserver* obs = obslist.at(i);
			if (!_observerList.contains(obs))
			{
				_observerList.append(obs);
				connect(obs, SIGNAL(updateDisplay()), this, SLOT(updateTable()));
			}
		}

	}
}



