#include "DialogThermalTabType.h"
#include "ui_DialogThermalTabType.h"
#include "SelfDefObject/ParaTableWidget.h"
#include "ThermalBoundary.h"
#include "DialogThermalBoundary.h"
#include "BoundaryTabTypeData.h"
#include <qmessagebox.h>
#include <qmenu.h>
#include <qevent.h>
#include <QHeaderView.h>
namespace ThermalFluid
{
	ThermalTabTypeDialog::ThermalTabTypeDialog(GUI::MainWindow* mw, ThermalBoundaryDialog* tbDlg) 
		: QFDialog(mw),
		_tableWidget(new SelfDefObj::ParaTableWidget(mw)), 
		_ui(new Ui::ttDialog), 
		_tbDlg(tbDlg)
	{
		_ui->setupUi(this);
		_ui->gridLayout->addWidget(_tableWidget);
		_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(_tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(itemChangedSlot(QTableWidgetItem *)));
		connect(_tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createRowMenuSlot(QPoint)));
		init(_tbDlg->getComboxCurrentText());
	}

	ThermalTabTypeDialog::~ThermalTabTypeDialog()
	{
		delete _tableWidget;
		delete _ui;
		_tableWidget = nullptr;
		_ui = nullptr;
	}

	void ThermalTabTypeDialog::init(const QString& type)
	{
		_ui->labelID->clear();
		_ui->lineEdit_tabName->clear();
		setTabDataType(type);

		_tableWidget->clear();
		_tableWidget->setColumnCount(2);
		_tableWidget->setRowCount(1);

		QStringList labels;
		labels << tr("XValue") << tr("Time");
		_tableWidget->setHorizontalHeaderLabels(labels);
//		_tableWidget->setWindowTitle(tr("association table data"));
//		_tableWidget->horizontalHeader()->setStretchLastSection(true);//拉伸最后一个使齐自适应宽度
//		_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//		_tableWidget->setShowGrid(false);

		_tableWidget->horizontalHeader()->setSectionsClickable(false);
		_tableWidget->verticalHeader()->setSectionsClickable(false);
		_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表头自适应宽度
		_tableWidget->setFrameShape(QFrame::NoFrame);
		_tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
		_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		_tableWidget->setStyleSheet("selection-background-color:#EE7700;");
		_tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#EE7700;}");
	}

	bool ThermalTabTypeDialog::hasEmptyItem()
	{
		int rowCount =_tableWidget->rowCount();
		for (int i = 0; i < rowCount; i++)
		{
			if (_tableWidget->item(i, 0) == nullptr ||
				_tableWidget->item(i, 0)->text().isEmpty())
				return true;
			if (_tableWidget->item(i, 1) == nullptr ||
				_tableWidget->item(i, 1)->text().isEmpty())
				return true;
		}
		return false;
	}

	void ThermalTabTypeDialog::writeTbDataToTable()
	{
		int tabID = _tbDlg->getCurrentTb()->getTableTypeID();
		_oldTabData = _tbDlg->getThermalData()->getTabTypeData(tabID);
		_ui->labelID->setText(QString::number(_tbDlg->getCurrentTb()->getTableTypeID()));
		_ui->lineEdit_tabName->setText(_oldTabData->getName());
		QString type = _oldTabData->tabTypeToString(_oldTabData->getType());
		_ui->label_tabTypeShow->setText(type);
	
		int n = _oldTabData->getYXCount();
		_tableWidget->setRowCount(n);
		for (int row = 0; row < n; row++)
		{
			QStringList list;
			double* YX = _oldTabData->getYXValue(row);
			list << QString::number(YX[0], 'E', 7) << QString::number(YX[1], 'E', 7);
			writeARowData(list, row);
		}
	}

	void ThermalTabTypeDialog::writeEmptyDataToTable()
	{		
		_ui->labelID->setText(QString::number(getMaxTabID() + 1));
	}

	void ThermalTabTypeDialog::writeARowData(QStringList& list, int row)
	{
		QTableWidgetItem* item0 = new QTableWidgetItem(list.at(0));
		QTableWidgetItem* item1 = new QTableWidgetItem(list.at(1));
		_tableWidget->setItem(row, 0, item0);
		_tableWidget->setItem(row, 1, item1);
	}

	void ThermalTabTypeDialog::keyPressEvent(QKeyEvent *event)
	{
		switch (event->key())
		{
		case  Qt::Key_Escape:
			event->ignore();
			break;
		default:
			QWidget::keyPressEvent(event);
			break;
		}
	}

	void ThermalTabTypeDialog::itemChangedSlot(QTableWidgetItem* item)
	{
		item->setTextAlignment(Qt::AlignCenter);
	}

	void ThermalTabTypeDialog::createRowMenuSlot(QPoint point)
	{
		_currentRow = _tableWidget->verticalHeader()->logicalIndexAt(point);
		if (_currentRow < 0)
			_currentRow = _tableWidget->rowCount();

		QMenu pop_menu;
		QAction* action = nullptr;
		action = pop_menu.addAction(tr("Insert Row"));
		connect(action, SIGNAL(triggered()), this, SLOT(actionInsertRowSlot()));

		action = pop_menu.addAction(tr("Remove Row"));
		connect(action, SIGNAL(triggered()), this, SLOT(actionRemoveRowSlot()));
		pop_menu.exec(QCursor::pos());
	}

	void ThermalTabTypeDialog::actionInsertRowSlot()
	{
		_tableWidget->insertRow(_currentRow);
	}

	void ThermalTabTypeDialog::actionRemoveRowSlot()
	{
		if (_currentRow >= 0)
		{
			if (_currentRow < _tableWidget->rowCount())
				_tableWidget->removeRow(_currentRow);
			else
				_tableWidget->removeRow(_tableWidget->rowCount() - 1);
		}
	}

	BoundaryTabTypeData* ThermalTabTypeDialog::saveOriTabTypeData()
	{
		_oldTabData->clearYXList();
		_oldTabData->setName(_ui->lineEdit_tabName->text());		
		_oldTabData->setType(_oldTabData->stringToTabType(_ui->label_tabTypeShow->text()));
		int rowCount = _tableWidget->rowCount();
		for (int i = 0; i < rowCount; i++)
		{
			QString XValue = _tableWidget->item(i, 0)->text();
			QString time = _tableWidget->item(i, 1)->text();
			_oldTabData->appendYX(XValue.toDouble(),time.toDouble());
		}
		return _oldTabData;
	}

	BoundaryTabTypeData* ThermalTabTypeDialog::saveNewTabTypeData()
	{				
		BoundaryTabTypeData* newTabData = iniATabTypeDataFromTableWidget();
		_tbDlg->getCurrentTb()->setTableTypeID(newTabData->getID());
		_tbDlg->getThermalData()->appendTabTypeDataList(newTabData);
		return newTabData;
	}

	BoundaryTabTypeData* ThermalTabTypeDialog::iniATabTypeDataFromTableWidget()
	{
		BoundaryTabTypeData* tabData = new BoundaryTabTypeData;
		int ID = _ui->labelID->text().toInt();
		TabType type = tabData->stringToTabType(_ui->label_tabTypeShow->text());

		tabData->setID(ID);
		tabData->setName(_ui->lineEdit_tabName->text());
		tabData->setType(type);

		int rowCount = _tableWidget->rowCount();
		for (int i = 0; i < rowCount; i++)
		{
			QString XValue = _tableWidget->item(i, 0)->text();
			QString time = _tableWidget->item(i, 1)->text();
			tabData->appendYX(XValue.toDouble(), time.toDouble());
		}
		return tabData;
	}

	void ThermalTabTypeDialog::setTabDataType(const QString& type)
	{
		if (type == QObject::tr("INULL"))
			_ui->label_tabTypeShow->setText("TEMP");
		else
			_ui->label_tabTypeShow->setText("QNODE");
	}

	int ThermalTabTypeDialog::getMaxTabID()
	{
		auto tabList = _tbDlg->getThermalData()->getTabTypeDataList();
		BoundaryTabTypeData* tabData;
		QList<int> idList;
		foreach(tabData, tabList)
			idList << tabData->getID();

		qSort(idList.begin(), idList.end());
		return idList.last();
	}

	bool ThermalTabTypeDialog::ttDlgInformation()
	{
		if (_ui->lineEdit_tabName->text().isEmpty())
		{
			QMessageBox::information(this, tr("infomation"), tr("Please Enter Table Name!"));
			return false;
		}
		return true;
	}
}