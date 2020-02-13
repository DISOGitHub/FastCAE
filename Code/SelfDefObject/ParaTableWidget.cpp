#include "SelfDefObject/ParaTableWidget.h"
#include "mainWindow/mainWindow.h"
#include "LineEditDialog.h"
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QDebug>

namespace SelfDefObj
{
	ParaTableWidget::ParaTableWidget(GUI::MainWindow* m) :_mainWindow(m)
	{
		this->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(horizontalMenu(QPoint)));
		this->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(verticalMenu(QPoint)));
		
		connect(this, SIGNAL(lockGraphFocus(bool)), m, SIGNAL(enableGraphWindowKeyBoard(bool)));
	}

	void ParaTableWidget::horizontalMenu(QPoint p)
	{
		_currentColumn = horizontalHeader()->logicalIndexAt(p);
		if (_currentColumn < 0) return;
		QMenu pop_menu;
		QAction *action = nullptr;
		action = pop_menu.addAction(tr("Insert Column At Left"));
		connect(action, SIGNAL(triggered()), this, SLOT(insertColumnLeft()));
		action = pop_menu.addAction(tr("Insert Column At Right"));
		connect(action, SIGNAL(triggered()), this, SLOT(insertColumnRight()));
		action = pop_menu.addAction(tr("Rename"));
		connect(action, SIGNAL(triggered()), this, SLOT(renameColumn()));
		pop_menu.addSeparator();
		action = pop_menu.addAction(tr("Delete Column"));
		connect(action, SIGNAL(triggered()), this, SLOT(deleteColumn()));

		pop_menu.exec(QCursor::pos());
	}
	void ParaTableWidget::verticalMenu(QPoint p)
	{
		_currentRow = verticalHeader()->logicalIndexAt(p);
		if (_currentRow < 0) return;
		QMenu pop_menu;
		QAction* action = nullptr;
		action = pop_menu.addAction(tr("Insert Row At Above"));
		connect(action, SIGNAL(triggered()), this, SLOT(insertRowAbove()));
		action = pop_menu.addAction(tr("Insert Column At Below"));
		connect(action, SIGNAL(triggered()), this, SLOT(insertRowBelow()));
		pop_menu.addSeparator();
		action = pop_menu.addAction(tr("Delete Row"));
		connect(action, SIGNAL(triggered()), this, SLOT(deleteRow()));

		pop_menu.exec(QCursor::pos());
	}
	void ParaTableWidget::deleteRow()
	{
		if (_currentRow >= 0 && _currentRow < rowCount())
			this->removeRow(_currentRow);
	}
	void ParaTableWidget::deleteColumn()
	{
		if (_currentColumn >= 0 && _currentColumn < columnCount())
			this->removeColumn(_currentColumn);
	}
	void ParaTableWidget::insertRowAbove()
	{
		this->insertRow(_currentRow);
	}
	void ParaTableWidget::insertRowBelow()
	{
		this->insertRow(_currentRow + 1);
	}
	void ParaTableWidget::insertColumnLeft()
	{
		this->insertColumn(_currentColumn);
	}
	void ParaTableWidget::insertColumnRight()
	{
		this->insertColumn(_currentColumn + 1);
	}
	void ParaTableWidget::focusInEvent(QFocusEvent *event)
	{
		emit lockGraphFocus(false);
		QTableWidget::focusInEvent(event);
	}
	void ParaTableWidget::renameColumn()
	{
		QStringList titles;
		for (int j = 0; j < columnCount(); ++j)
		{
			QString t;
			QTableWidgetItem* item = horizontalHeaderItem(j);
			if (item != nullptr)
			{
				t = item->text();
			}
			titles.append(t);
		}
		QString text = titles.at(_currentColumn);

		LineEditDialog dlg(_mainWindow, text);
		dlg.setWindowTitle(tr("Rename"));
		dlg.setDescribe(tr("Name"));
		dlg.exec();

		qDebug() << text;
		titles.replace(_currentColumn, text);
		this->setHorizontalHeaderLabels(titles);
		
	}
}