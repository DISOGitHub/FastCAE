#include "columndataform.h"
#include "ui_columndataform.h"

ColumnDataForm::ColumnDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColumnDataForm)
{
    ui->setupUi(this);
	this->setContentsMargins(0, 0, 0, 0);
    codec = QTextCodec::codecForName("GBK");
    curve_data_ptr=NULL;
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    cur_index_file=-1;
    cur_index_curve=-1;
}

ColumnDataForm::~ColumnDataForm()
{
    delete ui;
}

void ColumnDataForm::update_columnDataWidget(int index_file)
{
    if(curve_data_ptr==NULL)
        return;
    cur_index_file=index_file;
    QString tepstr;
    if(index_file>=curve_data_ptr->curve_file_list.count())
        return;
    curve_file_data tep_file_data=curve_data_ptr->curve_file_list.at(index_file);
    if(tep_file_data.file_prop.num_column<1)
        return;
	ui->tableWidget->setVisible(false);
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(tep_file_data.file_prop.num_column);
    ui->tableWidget->setRowCount(tep_file_data.file_prop.num_point+1);
    for(int i=0;i<tep_file_data.file_prop.num_column;i++)
    {
        tepstr=tep_file_data.file_prop.columns_name_list.at(i);
		QTableWidgetItem *item_des;
		item_des=new QTableWidgetItem(tepstr);
        //item_des->setTextAlignment(Qt::AlignHCenter);
        item_des->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(0,i,item_des);
    }
	
    for(int j=0;j<tep_file_data.file_prop.num_column;j++)
    {
        for(int i=0;i<tep_file_data.file_prop.num_point;i++)
        {
			QTableWidgetItem *item_val = new QTableWidgetItem(tepstr.setNum(tep_file_data.file_prop.column_val_data[j][i], 'g', 6));
			ui->tableWidget->setItem(i + 1, j, item_val);   
        }
    }
    ui->tableWidget->resizeColumnsToContents();
	ui->tableWidget->setVisible(true);
}

void ColumnDataForm::update_columnDataWidget_from_curveData(int index_file,QVector<int> curve_col_list)
{
    if(curve_data_ptr==NULL)
        return;
    cur_index_file=index_file;
    QString tepstr;
    if(index_file>=curve_data_ptr->curve_file_list.count())
        return;
    curve_file_data tep_file_data;
    tep_file_data=curve_data_ptr->curve_file_list.at(index_file);
    if(tep_file_data.file_prop.num_column<1)
        return;
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(tep_file_data.file_prop.num_column);
    ui->tableWidget->setRowCount(tep_file_data.file_prop.num_point+1);
    for(int i=0;i<tep_file_data.file_prop.num_column;i++)
    {
		tepstr = tep_file_data.file_prop.columns_name_list.at(i);
        QTableWidgetItem *item_des = new QTableWidgetItem(tepstr);
        //item_des->setTextAlignment(Qt::AlignHCenter);
        item_des->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(0,i,item_des);
    }
    for(int i=0;i<curve_col_list.count();i++)
    {
        ui->tableWidget->item(0,curve_col_list.at(i))->setCheckState(Qt::Checked);
    }
    for(int j=0;j<tep_file_data.file_prop.num_column;j++)
    {
        for(int i=0;i<tep_file_data.file_prop.num_point;i++)
        {
            QTableWidgetItem *item_val=new QTableWidgetItem(tepstr.setNum(tep_file_data.file_prop.column_val_data[j][i],'g',6));
            //item_val->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget->setItem(i+1,j,item_val);
        }
    }
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->resizeColumnsToContents();
}

void ColumnDataForm::on_tableWidget_clicked(const QModelIndex &index)
{
    if(curve_data_ptr==NULL)
        return;
    if(index.row()!=0)
        return;
    int tep_col=index.column();
	func_tableWidget_clicked(cur_index_file, tep_col);
}

void ColumnDataForm::set_tableWidget_checked(int tep_column_index,Qt::CheckState status)
{
	QTableWidgetItem *tep_item = new QTableWidgetItem;
	tep_item = ui->tableWidget->item(0, tep_column_index);
	tep_item->setCheckState(status);
}

void ColumnDataForm::func_tableWidget_clicked(int tep_file_index,int tep_column_index)
{
	QTableWidgetItem *tep_item = new QTableWidgetItem;
	tep_item = ui->tableWidget->item(0, tep_column_index);
	if (tep_item->checkState() == Qt::Checked)
	{
		emit sig_add_curve(tep_file_index, tep_column_index);
	}
	else
	{
		emit sig_del_curve(tep_file_index, tep_column_index);
	}
}

void ColumnDataForm::slot_update_clear_data()
{
	ui->tableWidget->clear();
}

void ColumnDataForm::reTranslate()
{
	ui->retranslateUi(this);
}
