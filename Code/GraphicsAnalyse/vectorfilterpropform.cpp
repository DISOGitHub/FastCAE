#include "vectorfilterpropform.h"
#include "ui_vectorfilterpropform.h"
vectorFilterPropForm::vectorFilterPropForm(PipelineObjPropWidget *parent) :
QWidget(parent), _parentWin(parent),
    ui(new Ui::vectorFilterPropForm)
{
    ui->setupUi(this);
}

vectorFilterPropForm::~vectorFilterPropForm()
{
    delete ui;
}

void vectorFilterPropForm::updateForm()
{
	QString tep_str;
	if (vectorFilter_propData->glyph_data.type<ui->glyphType_comboBox->count())
		ui->glyphType_comboBox->setCurrentIndex(vectorFilter_propData->glyph_data.type);
	else
		ui->glyphType_comboBox->setCurrentIndex(0);
	ui->numPoints_spinBox->setRange(0,cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mPointCount);//ui->mask_horizontalSlider->setValue(vectorFilter_propData->mask);
	ui->numPoints_spinBox->setValue(vectorFilter_propData->numPoints);
	//ui->mask_lineEdit->setText(tep_str.setNum(ui->numPoints_spinBox->value(), 10));//ui->mask_lineEdit->setText(tep_str.setNum(vectorFilter_propData->mask, 10));
	ui->tipRes_spinBox->setValue(vectorFilter_propData->glyph_data.arrow_data.tipRes);
	//ui->tipRes_lineEdit->setText(tep_str.setNum(vectorFilter_propData->glyph_data.arrow_data.tipRes, 10));
	ui->tipRad_doubleSpinBox->setValue(vectorFilter_propData->glyph_data.arrow_data.tipRad);
	//ui->tipRad_lineEdit->setText(tep_str.setNum(vectorFilter_propData->glyph_data.arrow_data.tipRad, 'g', 1));
	ui->tipLen_doubleSpinBox->setValue(vectorFilter_propData->glyph_data.arrow_data.tipRad);
	//ui->tipLen_lineEdit->setText(tep_str.setNum(vectorFilter_propData->glyph_data.arrow_data.tipRad, 'g', 2));
	ui->shaftRes_spinBox->setValue(vectorFilter_propData->glyph_data.arrow_data.tipRes);
	//ui->shaftRes_lineEdit->setText(tep_str.setNum(vectorFilter_propData->glyph_data.arrow_data.tipRes, 10));
	ui->shaftRad_doubleSpinBox->setValue(vectorFilter_propData->glyph_data.arrow_data.tipRad);
	//ui->shaftRad_lineEdit->setText(tep_str.setNum(vectorFilter_propData->glyph_data.arrow_data.tipRad, 'g', 2));
	ui->scalar_comboBox->blockSignals(true);
	ui->scalar_comboBox->clear();
	QIcon icon3 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
	ui->scalar_comboBox->addItem(icon3, "None", "val_on_point.png");
	for (int i = 0; i < vectorFilter_propData->scalar_list.count(); i++)
	{
		tep_str = vectorFilter_propData->scalar_list.at(i);
		if (tep_str.indexOf("_cell000") >= 0)
		{
			QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
			ui->scalar_comboBox->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
		}
		else
		{
			QIcon icon3 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
			ui->scalar_comboBox->addItem(icon3, tep_str, "val_on_point.png");
		}
	}
	ui->scalar_comboBox->blockSignals(false);
	ui->vector_comboBox->blockSignals(true);
	ui->vector_comboBox->clear();
	ui->vector_comboBox->addItem(icon3, "None", "val_on_point.png");
	
	for (int i = 0; i < vectorFilter_propData->vector_list.count(); i++)
	{
		tep_str = vectorFilter_propData->vector_list.at(i);
		if (tep_str.indexOf("_cell000") >= 0)
		{
			QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
			ui->vector_comboBox->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
		}
		else
		{
			QIcon icon3 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
			ui->vector_comboBox->addItem(icon3, tep_str, "val_on_point.png");
		}
	}
	ui->vector_comboBox->blockSignals(false);
	QString tep_str1 = vectorFilter_propData->scalarColName;
	if (tep_str1.indexOf("_cell000") >= 0)
		tep_str1 = tep_str1.left(tep_str1.length() -8);
	ui->scalar_comboBox->setCurrentText(tep_str1);
	tep_str1 = vectorFilter_propData->vectorColName;
	if (tep_str1.indexOf("_cell000") >= 0)
		tep_str1 = tep_str1.left(tep_str1.length() -8);
	ui->vector_comboBox->setCurrentText(tep_str1);
}

void vectorFilterPropForm::on_glyphType_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	vectorFilter_propData->glyph_data.type = index;
	emit sig_enable_applyBtn(); 
	emit _parentWin->sig_python_vector_glyph_type(cur_pipelineObj, index);
}

void vectorFilterPropForm::on_tipRes_spinBox_valueChanged(int arg1)
{
    vectorFilter_propData->glyph_data.arrow_data.tipRes = arg1;
    emit sig_enable_applyBtn();
	emit _parentWin->sig_python_vector_glyph_tipRes(cur_pipelineObj, arg1);
}

void vectorFilterPropForm::on_tipRad_doubleSpinBox_valueChanged(double arg1)
{
    vectorFilter_propData->glyph_data.arrow_data.tipRad =arg1;;
    emit sig_enable_applyBtn();
	emit _parentWin->sig_python_vector_glyph_tipRad(cur_pipelineObj, arg1);
}

void vectorFilterPropForm::on_tipLen_doubleSpinBox_valueChanged(double arg1)
{
    vectorFilter_propData->glyph_data.arrow_data.tipLen =arg1;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_vector_glyph_tipLen(cur_pipelineObj, arg1);
}

void vectorFilterPropForm::on_shaftRes_spinBox_valueChanged(int arg1)
{
    vectorFilter_propData->glyph_data.arrow_data.shaftRes = arg1;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_vector_glyph_shaftRes(cur_pipelineObj, arg1);
}

void vectorFilterPropForm::on_shaftRad_doubleSpinBox_valueChanged(double arg1)
{
    vectorFilter_propData->glyph_data.arrow_data.shaftRad = arg1;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_vector_glyph_shaftRad(cur_pipelineObj, arg1);
}

void vectorFilterPropForm::on_scalar_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	vectorFilter_propData->scalarColName = ui->scalar_comboBox->currentText();
	if (ui->scalar_comboBox->itemData(index).toString().indexOf("val_on_cell.png") >= 0)
		vectorFilter_propData->scalarColName += "_cell000";
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_vector_scalar(cur_pipelineObj, vectorFilter_propData->scalarColName);
}

void vectorFilterPropForm::on_vector_comboBox_currentIndexChanged(int index)
{
	vectorFilter_propData->vectorColName = ui->vector_comboBox->currentText();
	if (ui->vector_comboBox->itemData(index).toString().indexOf("val_on_cell.png") >= 0)
		vectorFilter_propData->vectorColName += "_cell000";
	bool flag = false;
	if (vectorFilter_propData->vectorColName != "None")
		flag = true;
	if (flag)
	{
		cur_pipelineObj->func_initGlyphVectorArr(cur_pipelineObj->mBaseGrid);
	}
	_parentWin->func_update_colorComboBox_glyphVector(flag);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_vector_normal(cur_pipelineObj, vectorFilter_propData->vectorColName);
}

//void vectorFilterPropForm::on_mask_horizontalSlider_sliderMoved(int position)
//{
//	QString tep_str;
//	vectorFilter_propData->mask = position;
//	ui->mask_lineEdit->setText(tep_str.setNum(vectorFilter_propData->mask,10));
//	emit sig_enable_applyBtn();
//}

void vectorFilterPropForm::on_numPoints_spinBox_valueChanged(int arg1)
{
	QString tep_str;
	/*if (!arg1)
		vectorFilter_propData->mask = 0;
	else
		vectorFilter_propData->mask = cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mPointCount / ui->numPoints_spinBox->value();*/
	vectorFilter_propData->numPoints = ui->numPoints_spinBox->value();
    //ui->mask_lineEdit->setText(tep_str.setNum(ui->numPoints_spinBox->value(), 10));
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_vector_numPoints(cur_pipelineObj, vectorFilter_propData->numPoints);
}

void vectorFilterPropForm::reTranslate()
{
	ui->retranslateUi(this);
}




