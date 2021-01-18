#include "ui_dialogAerodynamic.h"
#include "dialogAerodynamic.h"
#include "DataManager.h"
#include <QTableWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileDialog>
namespace Aircraft
{

	CreateAerodynamicDialog::CreateAerodynamicDialog()
	{

		_ui = new Ui::DialogAerodynamic;
		_ui->setupUi(this);
		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);
		_tableWidget = _ui->tableWidget;
		this->init();
		
	}

	void CreateAerodynamicDialog::init()
	{
		_ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		_ui->tableWidget->setRowCount(3);
		_ui->tableWidget->verticalHeader()->setVisible(false);
	}

	void CreateAerodynamicDialog::closeEvent(QCloseEvent *)
	{

	}

	void CreateAerodynamicDialog::reject()
	{
		this->close();
	}

	void CreateAerodynamicDialog::accept()
	{
		QDialog::accept();
		this->close();
	}

	void CreateAerodynamicDialog::fillTab()
	{
		if (_datalist.size() < 1) return;
		const int r = _datalist.size();
		const int c = _datalist[0].size();

		if (r*c == 0) return;
		_tableWidget->clear();
		_tableWidget->setRowCount(r);
		_tableWidget->setColumnCount(c);
		_tableWidget->setHorizontalHeaderLabels(_headlist);

		for (int i = 0; i < r; ++i)
		{
			for (int j = 0; j < c; ++j)
			{
				QString d = QString::number(_datalist[i][j]);
				_tableWidget->setItem(i, j, new QTableWidgetItem(d));
			}
		}
	}

	void CreateAerodynamicDialog::on_SaveButton_clicked()
	{ 
		QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath(), tr("Files (*.csv)"));
		if (fileName.isEmpty()) return;
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))  return;

		QTextStream out(&file);//输出文件流
		int row = _tableWidget->rowCount();//数据表行数
		int col = _tableWidget->columnCount();//数据列数

		//导入表头
		for (int i = 0; i < col; i++)
		{
			QString qs = _tableWidget->horizontalHeaderItem(i)->text();//获得 水平表头->内容
			if (i == col - 1) out << qs << endl;
			else out << qs << ",";

		}

		//导入内容
		for (int i = 0; i < row; i++)

		{
			for (int j = 0; j < col; j++)

			{
				QString string = _tableWidget->item(i, j)->text();
				if (j == col - 1) out << string << endl;
				else out << string << ",";
			}
		}
		file.close();
	}

	void CreateAerodynamicDialog::on_ImportButton_clicked()
	{

		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("*.csv"));
		if (fileName == "") return;
		QFile file(fileName);
		QString line{};

		//--打开文件成功 
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return ;
		QTextStream textStream(&file);
		int i = 0;
		bool ok = true;
		while (!textStream.atEnd())
		{
			QList<double> oneline{};
			line = textStream.readLine().simplified();
			if (line.isEmpty()) continue;
			QStringList splint = line.split(",");
			splint[0].toDouble(&ok);
			if (i == 0 && !ok)
			{
				_headlist = splint;
				continue;
			}
			else
			{
				for (QString str: splint)
					oneline.push_back(str.toDouble());
			}
			if(oneline.size()>0)
				_datalist.push_back(oneline);
			i++;
		}
		file.close();
		fillTab();
	}

}