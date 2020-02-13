#include "curveaxispropdialog.h"
#include "ui_curveaxispropdialog.h"

curveAxisPropDialog::curveAxisPropDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::curveAxisPropDialog)
{
    ui->setupUi(this);
	axisPropPtr = NULL;
}

curveAxisPropDialog::~curveAxisPropDialog()
{
    delete ui;
}

void curveAxisPropDialog::initForm()
{
    if(axisID.toLower()=="x")
    {
        axisPropPtr=&(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0]);
    }
    else if(axisID.toLower()=="x2")
    {
		axisPropPtr = &(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1]);
    }
    else if(axisID.toLower()=="y")
    {
		axisPropPtr = &(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[0]);
    }
    else if(axisID.toLower()=="y2")
    {
		axisPropPtr = &(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[1]);
    }
	if (axisPropPtr == NULL)
		return;
	ui->axis_name_lineEdit1->setStyleSheet("QLineEdit{border-width:1;border-style:outset}");
	ui->axis_name_lineEdit1->setText(axisPropPtr->axis_name);

	ui->axis_fontType_comboBox1->setCurrentIndex(axisPropPtr->axis_font.font_type);

	ui->axis_fontColor_pushButton1->setText(tr("select color"));
	QColor tep_color = axisPropPtr->axis_font.font_color;
	ui->axis_fontColor_pushButton1->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue()));
	ui->axis_fontSize_spinBox1->setValue(axisPropPtr->axis_font.font_size);

	if ((axisID.toLower() == "x") || (axisID.toLower() == "x2"))
	{
		ui->xDataSource_groupBox->setEnabled(true);
		ui->axis_dataSource_comboBox1->setCurrentIndex(axisPropPtr->axis_dataSource);
    
		if (axisPropPtr->axis_dataSource == plotProp_dataSource::Index)
		{
			ui->fileDataSource_groupBox->setEnabled(false);
		}
		else
		{
			ui->fileDataSource_groupBox->setEnabled(true);
			ui->axis_dataFile_comboBox1->setCurrentIndex(axisPropPtr->axis_fileIndex);
			on_axis_dataFile_comboBox1_currentIndexChanged(axisPropPtr->axis_fileIndex);
		}
    }
    else
    {
        ui->xDataSource_groupBox->setEnabled(false);
    }
	QString str;
	ui->lineEdit_min->setText(str.setNum(axisPropPtr->axis_range[0],'g',6));
	ui->lineEdit_max->setText(str.setNum(axisPropPtr->axis_range[1], 'g', 6));
}

void curveAxisPropDialog::on_axis_fontColor_pushButton1_clicked()
{
	if (axisPropPtr == NULL)
		return;
	QColor init_color0 = axisPropPtr->axis_font.font_color;

	QColor tep_color = QColorDialog::getColor(init_color0, this, tr("color dialog"));
	if (!tep_color.isValid())
		return;
	axisPropPtr->axis_font.font_color = tep_color;
	int r = axisPropPtr->axis_font.font_color.red();
	int g = axisPropPtr->axis_font.font_color.green();
	int b = axisPropPtr->axis_font.font_color.blue();
    ui->axis_fontColor_pushButton1->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	emit sig_setAxisName(axisID);
}

void curveAxisPropDialog::on_axis_fontType_comboBox1_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if (axisPropPtr == NULL)
		return;
	axisPropPtr->axis_font.font_type = (plotProp_fontType)index;
	emit sig_setAxisName(axisID);
}

void curveAxisPropDialog::on_axis_fontSize_spinBox1_valueChanged(int index)
{
	if (axisPropPtr == NULL)
		return;
	axisPropPtr->axis_font.font_size = index;
	emit sig_setAxisName(axisID);
}

void curveAxisPropDialog::on_axis_name_lineEdit1_textChanged(QString text)
{
	if (axisPropPtr == NULL)
		return;
	axisPropPtr->axis_name = text;
    emit sig_setAxisName(axisID);
}

void curveAxisPropDialog::on_axis_dataSource_comboBox1_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if (axisPropPtr == NULL)
		return;
    if (index < 0)
        return;
    if (!index)
    {
		ui->fileDataSource_groupBox->setEnabled(false);
		axisPropPtr->axis_dataSource = (plotProp_dataSource)index;
		//ui->axis_dataFile_comboBox1->setEnabled(false);
		//ui->axis_dataColumn_comboBox1->setEnabled(false);
		ui->axis_dataFile_comboBox1->clear();
		ui->axis_dataColumn_comboBox1->clear();
        emit sig_xAxis_dataSource_change(axisID);
    }
    else
    {
		ui->fileDataSource_groupBox->setEnabled(true);
		axisPropPtr->axis_dataSource = (plotProp_dataSource)index;
		//ui->axis_dataFile_comboBox1->setEnabled(true);
		//ui->axis_dataColumn_comboBox1->setEnabled(true);
		ui->axis_dataFile_comboBox1->blockSignals(true);
		ui->axis_dataColumn_comboBox1->blockSignals(true);
		
		ui->axis_dataFile_comboBox1->clear();
        for (int i = 0; i<curve_data_ptr->curve_file_list.count(); i++)
        {
			ui->axis_dataFile_comboBox1->addItem(curve_data_ptr->curve_file_list.at(i).file_prop.Axis_filename);
        }
		//////////////////////////////////////
		ui->axis_dataColumn_comboBox1->clear();
		ui->axis_dataColumn_comboBox1->addItems(curve_data_ptr->curve_file_list.at(0).file_prop.columns_name_list);
		/////////////////////////////////////////
		if (ui->axis_dataFile_comboBox1->count()>0)
        {
			/*if (axisPropPtr->axis_fileColumn >0)
            {
				ui->axis_dataFile_comboBox1->setCurrentIndex(axisPropPtr->axis_fileIndex);
            }
            else
            {
				if (ui->axis_dataFile_comboBox1->currentIndex() == 0)
					on_axis_dataFile_comboBox1_currentIndexChanged(0);
				else
					ui->axis_dataFile_comboBox1->setCurrentIndex(0);
                
				axisPropPtr->axis_fileIndex = 0;
            }*/
			axisPropPtr->axis_fileIndex = 0;
			axisPropPtr->axis_fileColumn = 0;
			ui->axis_dataFile_comboBox1->setCurrentIndex(0);
			ui->axis_dataColumn_comboBox1->setCurrentIndex(0);
        }
		//ui->axis_dataFile_comboBox1->setEnabled(true);
		//ui->axis_dataColumn_comboBox1->setEnabled(true);
		ui->axis_dataFile_comboBox1->blockSignals(false);
		ui->axis_dataColumn_comboBox1->blockSignals(false);
        emit sig_xAxis_dataSource_change(axisID);
    }
}

void curveAxisPropDialog::on_axis_dataFile_comboBox1_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if (axisPropPtr == NULL)
		return;
    if (index < 0)
        return;
	axisPropPtr->axis_fileIndex = index;
	//ui->axis_dataFile_comboBox1->blockSignals(false);
	//ui->axis_dataColumn_comboBox1->setEnabled(false);
	ui->axis_dataColumn_comboBox1->clear();
    if (curve_data_ptr->curve_file_list.count()>index)
    {
        if (curve_data_ptr->curve_file_list.at(index).file_prop.columns_name_list.count()>0)
        {
			ui->axis_dataColumn_comboBox1->addItems(curve_data_ptr->curve_file_list.at(index).file_prop.columns_name_list);
			if (axisPropPtr->axis_fileColumn > 0)
				ui->axis_dataColumn_comboBox1->setCurrentIndex(axisPropPtr->axis_fileColumn);
            else
            {
				ui->axis_dataColumn_comboBox1->setCurrentIndex(0);
                on_axis_dataColumn_comboBox1_currentIndexChanged(0);
				axisPropPtr->axis_fileColumn = 0;
            }
        }
    }
	//ui->axis_dataColumn_comboBox1->setEnabled(true);
	//ui->axis_dataColumn_comboBox1->blockSignals(false);
	emit sig_xAxis_dataSource_change(axisID);
}

void curveAxisPropDialog::on_axis_dataColumn_comboBox1_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if (axisPropPtr == NULL)
		return;
    if (index < 0)
        return;
	//ui->axis_dataColumn_comboBox1->blockSignals(false);
	axisPropPtr->axis_fileColumn = index;
	emit sig_xAxis_dataSource_change(axisID);
}

void curveAxisPropDialog::on_lineEdit_min_editingFinished()
{
	axisPropPtr->axis_range[0] = ui->lineEdit_min->text().toDouble();
}

void curveAxisPropDialog::on_lineEdit_max_editingFinished()
{
	axisPropPtr->axis_range[1] = ui->lineEdit_max->text().toDouble();
}

void curveAxisPropDialog::on_rescale_pushButton_clicked()
{
	emit sig_setAxisRange(axisID);
}
