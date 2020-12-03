#include "ParaTabViewer.h"
#include "ui_ParaTabViewer.h"
#include "DataProperty/ParameterTable.h"
#include <QStringList>
#include "ParaTableWidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "settings/busAPI.h"

namespace SelfDefObj
{
	ParaTabViewer::ParaTabViewer(GUI::MainWindow* m, DataProperty::ParameterTable* data)
		: QFDialog(m), _data(data), _ui(new Ui::ParaTabViewer), _tableWidget(new ParaTableWidget(m)), SelfDefParaWidgetBase(data)
	{
		_ui->setupUi(this);
		_ui->tabLayout->addWidget(_tableWidget);
		this->init();
		_inited = true;
	}
	ParaTabViewer::~ParaTabViewer()
	{
		if (_tableWidget != nullptr)
		{
			_tableWidget->clear();
			delete _tableWidget;
		}
		if (_ui != nullptr) delete _ui;
	}

	void ParaTabViewer::init()
	{
		_tableWidget->clear();
		QString title = _data->getDescribe();
		QString lang = Setting::BusAPI::instance()->getLanguage();
		if (lang.toLower() == "chinese")
			title = _data->getChinese();

		this->setWindowTitle(title);
		const int r = _data->getRowCount();
		const int c = _data->getColumnCount();
		_tableWidget->setRowCount(r);
		_tableWidget->setColumnCount(c);

		QStringList t = _data->getTitle();
		_tableWidget->setHorizontalHeaderLabels(t);
	

		for (int i = 0; i < r; ++i)
		{
			for (int j = 0; j < c; ++j)
			{
				QString d = _data->getValue(i, j);
				_tableWidget->setItem(i, j, new QTableWidgetItem( d ));
				
			}

		}

	}
	

	void ParaTabViewer::apply()
	{
		submitCode();
	}
	void ParaTabViewer::on_out_applyButton_clicked()
	{
		this->apply();
	}
	void ParaTabViewer::on_out_cancelButton_clicked()
	{
		this->reject();
	}
	void ParaTabViewer::on_out_OKButton_clicked()
	{
		this->apply();
		this->accept();
	}

	DataProperty::ParameterTable* ParaTabViewer::readCSV(QString fileName)
	{
		QFile file(fileName);
		QString line;

		
		//--打开文件成功 
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return nullptr;

		DataProperty::ParameterTable* tempTab = new DataProperty::ParameterTable;
		
		QTextStream textStream(&file);
		
//		int row = 0;

		_tableWidget->clear();

		std::vector<std::vector<double>> data{};
		QStringList title{};
		bool isfirst = true;
		while (!textStream.atEnd())
		{
			line = textStream.readLine().simplified();
			if (line.isEmpty()) continue;
			QStringList splint = line.split(",");
			std::vector<double> dline{};
			
			for (int i = 0; i < splint.size(); ++i)
			{
				QString s = splint.at(i);
				bool ok = false;
				double d = s.toDouble(&ok);
				if (isfirst && (!ok))
				{
					title.append(s);
				}
				else
					dline.push_back(d);
			}
			if (dline.size()>0)
				data.push_back(dline);
			isfirst = false;
		}
		tempTab->setData(data);
		tempTab->setTitle(title);
		return tempTab;
	}
	
	void ParaTabViewer::fillTab(DataProperty::ParameterTable* table)
	{
		if (table == nullptr) return;
		const int r = table->getRowCount();
		const int c = table->getColumnCount();
		QStringList t = table->getTitle();

		if (r*c == 0) return;
		_tableWidget->clear();
		_tableWidget->setRowCount(r);
		_tableWidget->setColumnCount(c);
		_tableWidget->setHorizontalHeaderLabels(t);

		for (int i = 0; i < r; ++i)
		{
			for (int j = 0; j < c; ++j)
			{
				QString d = table->getValue(i, j);
				_tableWidget->setItem(i, j, new QTableWidgetItem(d));
			}
		}
		delete table;
	}

	void ParaTabViewer::on_out_importButton_clicked()
	{
		//---获取文件名
		QString fileName = QFileDialog::getOpenFileName(this, NULL, NULL, "*.csv ");

		//---打开文件并读取文件内容
		fillTab(readCSV(fileName));
	}

	void ParaTabViewer::on_out_exportButton_clicked()
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath(),tr("Files (*.csv)"));//getSaveFileName（）的四个参数？？

		if (fileName.isEmpty()) return;

		//获得csv文件流

		QFile file(fileName);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))  return;

		QTextStream out(&file);//输出文件流
		int row = _tableWidget->rowCount();//数据表行数
		int col = _tableWidget->columnCount();//数据列数

		//导入表头
		for (int i = 0; i < col; i++)
		{
			QString qs = _tableWidget->horizontalHeaderItem(i)->text();//获得水平表头->内容
			if (i==col-1)
			{
				out << qs << endl;
			}
			else
			{
				out << qs << ",";
			}
			
		}
		
		//导入内容
	
		for (int i = 0; i < row; i++)

		{

			for (int j = 0; j < col; j++)

			{

				QString string = _tableWidget->item(i, j)->text();
				if (j==col-1)
				{
					out << string << endl;
				}
				else
				{
					out << string << ",";// 写入文件
				}
			}
		}
		file.close();

	}
	
	QString ParaTabViewer::generateCode()
	{
		QString code = SelfDefParaWidgetBase::generateCode();
		QString text;

		const int r = _tableWidget->rowCount();
		const int c = _tableWidget->columnCount();
		text = QString("%1,%2;").arg(r).arg(c);

		QStringList titles;
		for (int j = 0; j < c; ++j)
		{
			QString t;
			QTableWidgetItem* item = _tableWidget->horizontalHeaderItem(j);
			if (item != nullptr)
			{
				t = item->text();
			}
			titles.append(t);
		}
		text.append(titles.join(","));
		text.append(";");

		for (int i = 0; i < r; ++i)
		{
			QStringList rowText;
			for (int j = 0; j < c; ++j)
			{
				QTableWidgetItem* item = _tableWidget->item(i, j);
				double d = 0.0;
				if (item != nullptr)
				{
					QString t = item->text();
//					d = t.toDouble();
					rowText.append(t);
				}
			}
			QString r = QString("{%1}").arg(rowText.join(","));
			text.append(r);
		}
		
		_data->setTitle(titles);

		code.replace("%Value%", text);
		return code;
	}


}