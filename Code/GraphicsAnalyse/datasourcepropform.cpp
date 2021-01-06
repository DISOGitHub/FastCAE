#include "datasourcepropform.h"
#include "ui_datasourcepropform.h"
#include "qfileinfo.h"
#include "qcolordialog.h"
#include "qfiledialog.h"
#include "LoadActionFile.h"
DataSourcePropForm::DataSourcePropForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataSourcePropForm)
{
    ui->setupUi(this);
	ui->centerX_Cone_lineEdit->setVisible(false);
	ui->CenterX_Cylinder_lineEdit->setVisible(false);
	ui->centerY_Cone_lineEdit->setVisible(false);
	ui->CenterY_Cylinder_lineEdit->setVisible(false);
	ui->centerZ_Cone_lineEdit->setVisible(false);
	ui->CenterZ_Cylinder_lineEdit->setVisible(false);
	ui->center_Cone_label->setVisible(false);
	ui->direction_Cone_label->setVisible(false);
	ui->center_Cylinder_label->setVisible(false);
	ui->lineType_tabWidget->tabBar()->hide();
	ui->comModel_groupBox->setVisible(false);
}

DataSourcePropForm::~DataSourcePropForm()
{
    delete ui;
}

void DataSourcePropForm::updateForm()
{
	if (block_showMap.count() > 1)
	{
		ui->grid_blocks_comboBox->setEnabled(true);
		ui->grid_blocks_comboBox->setCurrentIndex(cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks);
	}
	else
	{
		ui->grid_blocks_comboBox->setEnabled(false);
		ui->grid_blocks_comboBox->setCurrentIndex(0);
	}
	on_grid_blocks_comboBox_currentIndexChanged(cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks);
	//ui->block_groupBox->setVisible(false);////////////add for multi zone /////////////////////////
	if (suffix_filename != "moor")
	{
		ui->lineType_groupBox->setVisible(false);
		if ((cur_pipelineObj->mBaseGrid!=NULL)&&(cur_pipelineObj->mBaseGrid->GetNumberOfCells() == 0))//if (cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mCellCount == 0)
		{
			ui->particles_groupBox->setVisible(true);
			ui->particlesType_comboBox->setCurrentIndex(cur_pipelineObj->mPipeLineObjProp.particles_propData.type);
			ui->particlesRadius_doubleSpinBox->setRange(cur_pipelineObj->mPipeLineObjProp.particles_propData.min_radius, cur_pipelineObj->mPipeLineObjProp.particles_propData.max_radius);
			ui->particlesRadius_doubleSpinBox->setSingleStep(cur_pipelineObj->mPipeLineObjProp.particles_propData.max_radius*0.01);
			ui->particlesRadius_doubleSpinBox->setValue(cur_pipelineObj->mPipeLineObjProp.particles_propData.radius);
			ui->particlesRes_spinBox->setValue(cur_pipelineObj->mPipeLineObjProp.particles_propData.resolution);
		}
		else
		{
			ui->particles_groupBox->setVisible(false);
		}
	}
	else
	{
		ui->particles_groupBox->setVisible(false);
		ui->lineType_groupBox->setVisible(true);
		if (mooringLine_propData->glyph_data.type == 0)
		{
			ui->lineType_comboBox->setCurrentIndex(mooringLine_propData->glyph_data.type);
			ui->lineType_tabWidget->setVisible(true);
			ui->lineType_tabWidget->setCurrentIndex(mooringLine_propData->glyph_data.type);
			ui->scaleFactor_doubleSpinBox->setValue(mooringLine_propData->glyph_data.scaleFactor);
		}
		else
		{
			ui->lineType_comboBox->setCurrentIndex(1);
			ui->lineType_tabWidget->setVisible(false);
		}
		ui->seaBottom_checkBox->setChecked(mooringLine_propData->flag_seaBottom);
		ui->seaBotColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)")
			.arg(mooringLine_propData->seaBottomColor.r).arg(mooringLine_propData->seaBottomColor.g).arg(mooringLine_propData->seaBottomColor.b));
	}
	ui->displace_checkBox->setChecked(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile);
	ui->disPara_groupBox->setEnabled(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile);
	ui->displace_lineEdit->setText(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.path_AniDisplaceFile);
	ui->pos_spinBox->setRange(0, cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count());
	ui->pos_spinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.start_pos);
	ui->pos_spinBox->setSingleStep(1);
	ui->interval_spinBox->setRange(0,1000);
	ui->interval_spinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval);
	ui->interval_spinBox->setSingleStep(1);
	ui->zoomFactor_doubleSpinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor);
	ui->gravityCenter_checkBox->setChecked(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity);
	QString tep_str;
	ui->gravityX_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x, 'g', 6));
	ui->gravityY_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y, 'g', 6));
	ui->gravityZ_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z, 'g', 6));
}

void DataSourcePropForm::on_lineType_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	QString tep_str;
	mooringLine_propData->glyph_data.type = index;
	if (index == 0)
	{	
		//ui->lineType_comboBox->setCurrentIndex(mooringLine_propData->glyph_data.type);
		ui->lineType_tabWidget->setVisible(true);
		ui->lineType_tabWidget->setCurrentIndex(0);
		ui->tipRes_spinBox->setValue(mooringLine_propData->glyph_data.arrow_data.tipRes);
		ui->tipRad_doubleSpinBox->setValue(mooringLine_propData->glyph_data.arrow_data.tipRad);
		ui->tipLen_doubleSpinBox->setValue(mooringLine_propData->glyph_data.arrow_data.tipLen);
		ui->shaftRes_spinBox->setValue(mooringLine_propData->glyph_data.arrow_data.shaftRes);
		ui->shaftRad_doubleSpinBox->setValue(mooringLine_propData->glyph_data.arrow_data.shaftRad);
		ui->scaleFactor_doubleSpinBox->setValue(mooringLine_propData->glyph_data.scaleFactor);
	}
	else 
	{
		ui->lineType_tabWidget->setVisible(false);
		mooringLine_propData->glyph_data.type = -1;
	}
    /*else if (mooringLine_propData->glyph_data.type == 1)
	{
		ui->lineType_tabWidget->setVisible(true);
		ui->lineType_tabWidget->setCurrentIndex(1);
		ui->Res_cone_horizontalSlider->setValue(mooringLine_propData->glyph_data.cone_data.Res);
		ui->Res_cone_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cone_data.Res, 10));
		ui->RadCone_doubleSpinBox->setValue(mooringLine_propData->glyph_data.cone_data.Rad);
		ui->ConeHei_doubleSpinBox->setValue(mooringLine_propData->glyph_data.cone_data.Hei);
		ui->centerX_Cone_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cone_data.glyph_center.x, 'g', 2));
		ui->centerY_Cone_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cone_data.glyph_center.y, 'g', 2));
		ui->centerZ_Cone_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cone_data.glyph_center.z, 'g', 2));
		ui->DirX_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cone_data.glyph_direction.x, 'g', 2));
		ui->DirY_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cone_data.glyph_direction.y, 'g', 2));
		ui->DirZ_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cone_data.glyph_direction.z, 'g', 2));
		ui->lineType_tabWidget->setVisible(true);
	}
    else if (mooringLine_propData->glyph_data.type == 2)
	{
		ui->lineType_tabWidget->setVisible(true);
		ui->lineType_tabWidget->setCurrentIndex(2);
		ui->Res_Cylinder_horizontalSlider->setValue(mooringLine_propData->glyph_data.cylinder_data.Res);
		ui->Res_Cylinder_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cylinder_data.Res, 10));
		ui->RadCylinder_doubleSpinBox->setValue(mooringLine_propData->glyph_data.cylinder_data.Rad);
		ui->HeiCylinder_doubleSpinBox->setValue(mooringLine_propData->glyph_data.cylinder_data.Hei);
		ui->CenterX_Cylinder_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cylinder_data.glyph_center.x, 'g', 2));
		ui->CenterY_Cylinder_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cylinder_data.glyph_center.y, 'g', 2));
		ui->CenterZ_Cylinder_lineEdit->setText(tep_str.setNum(mooringLine_propData->glyph_data.cylinder_data.glyph_center.z, 'g', 2));
		ui->lineType_tabWidget->setVisible(true);
	}*/
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_tipRes_spinBox_valueChanged(int arg1)
{
    mooringLine_propData->glyph_data.arrow_data.tipRes = arg1;
    emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_tipRad_doubleSpinBox_valueChanged(double arg1)
{
    mooringLine_propData->glyph_data.arrow_data.tipRad = arg1;
    emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_tipLen_doubleSpinBox_valueChanged(double arg1)
{
    mooringLine_propData->glyph_data.arrow_data.tipLen = arg1;
    emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_shaftRes_spinBox_valueChanged(int arg1)
{
    mooringLine_propData->glyph_data.arrow_data.shaftRes = arg1;
    emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_shaftRad_doubleSpinBox_valueChanged(double arg1)
{
    mooringLine_propData->glyph_data.arrow_data.shaftRad = arg1;
    emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_Res_cone_horizontalSlider_sliderMoved(int position)
{
	QString tep_str;
	mooringLine_propData->glyph_data.cone_data.Res = position;
	ui->Res_cone_lineEdit->setText(tep_str.setNum(position, 10));
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_RadCone_doubleSpinBox_valueChanged(double arg1)
{
	mooringLine_propData->glyph_data.cone_data.Rad = arg1;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_ConeHei_doubleSpinBox_valueChanged(double arg1)
{
	mooringLine_propData->glyph_data.cone_data.Hei = arg1;
	emit sig_enable_applyBtn();
}


void DataSourcePropForm::on_centerX_Cone_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cone_data.glyph_center.x = ui->centerX_Cone_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_centerY_Cone_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cone_data.glyph_center.y = ui->centerY_Cone_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_centerZ_Cone_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cone_data.glyph_center.z = ui->centerZ_Cone_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_DirX_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cone_data.glyph_direction.x = ui->DirX_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_DirY_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cone_data.glyph_direction.y = ui->DirY_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_DirZ_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cone_data.glyph_direction.z = ui->DirZ_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_Res_Cylinder_horizontalSlider_sliderMoved(int position)
{
	QString tep_str;
	mooringLine_propData->glyph_data.cylinder_data.Res = position;
	ui->Res_Cylinder_lineEdit->setText(tep_str.setNum(position, 10));
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_RadCylinder_doubleSpinBox_valueChanged(double arg1)
{
	mooringLine_propData->glyph_data.cylinder_data.Rad = arg1;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_HeiCylinder_doubleSpinBox_valueChanged(double arg1)
{
	mooringLine_propData->glyph_data.cylinder_data.Hei = arg1;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_CenterX_Cylinder_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cylinder_data.glyph_center.x = ui->CenterX_Cylinder_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_CenterY_Cylinder_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cylinder_data.glyph_center.y = ui->CenterY_Cylinder_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_CenterZ_Cylinder_lineEdit_editingFinished()
{
	bool ok;
	mooringLine_propData->glyph_data.cylinder_data.glyph_center.z = ui->CenterZ_Cylinder_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_scaleFactor_doubleSpinBox_valueChanged(double arg1)
{
    mooringLine_propData->glyph_data.scaleFactor = arg1;
    emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_block_listWidget_clicked(const QModelIndex &index)
{
	int tep_row = index.row();
	if ((tep_row >= block_showMap.count()) || (tep_row >= ui->block_listWidget->count()))
		return;
	QListWidgetItem* tep_item = ui->block_listWidget->item(tep_row);
	if (!block_showMap.contains(tep_item->text()))
		return;
	bool old_status = block_showMap[tep_item->text()];
	bool new_status;
	if (tep_item->checkState() == Qt::Checked)
		new_status = true;
	else
		new_status = false;
	if (old_status == new_status)
		return;
	block_showMap.insert(tep_item->text(), new_status);
	cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.insert(tep_item->text(), new_status);
	cur_pipelineObj->flag_update_mBaseGrid = true;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_grid_blocks_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if (index == 1)
	{
		ui->block_groupBox->setVisible(true);
		ui->block_listWidget->clear();
		QMap<QString, bool>::iterator it;
		QString tep_key;
		bool tep_val;
		for (it = block_showMap.begin(); it != block_showMap.end(); it++)
		{
			tep_key = it.key();
			tep_val = it.value();
			QListWidgetItem *item = new QListWidgetItem;
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
			item->setText(tep_key);
			if (tep_val)
				item->setCheckState(Qt::Checked);
			else
				item->setCheckState(Qt::Unchecked);
			ui->block_listWidget->addItem(item);
		}
	}
	else if (index == 0)
	{
		ui->block_groupBox->setVisible(false);
	}
	cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks = index;
	cur_pipelineObj->flag_update_mBaseGrid = true;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::reTranslate()
{
	ui->retranslateUi(this);
}

void DataSourcePropForm::on_comModel_checkBox_clicked(bool checked)
{
	ui->comModel_comboBox->setVisible(checked);
	cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_comModel = checked;
	if (checked)
	{
		//update comModel_comboBox
		ui->comModel_comboBox->clear();
		for (int i = 0; i < g_ani_objList.count(); i++)
		{
			QFileInfo inf(g_ani_objList.at(i)->mPipeLineObjProp.pipelineObj_base_propData.fileName);
			if (inf.suffix() == "obj")
				ui->comModel_comboBox->addItem(g_ani_objList.at(i)->mPipeLineObjProp.pipelineObj_base_propData.fileName);
		}
		if (cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.comModel_fileName != "")
		{
			bool flag = false;
			for (int i = 0; i < ui->comModel_comboBox->count(); i++)
			{
				if (ui->comModel_comboBox->itemText(i) == cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.comModel_fileName)
				{
					ui->comModel_comboBox->setCurrentIndex(i);
					flag = true;
					break;
				}
			}
			if (!flag && (ui->comModel_comboBox->count()>0))
			{
				ui->comModel_comboBox->setCurrentIndex(0);
			}
		}
	}
}

void DataSourcePropForm::on_comModel_comboBox_currentIndexChanged(int index)
{
	if (index < 0)
		return;
	cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.comModel_fileName = ui->comModel_comboBox->currentText();
}
void DataSourcePropForm::on_particlesRadius_doubleSpinBox_valueChanged(double arg1)
{
	particles_propData->radius = arg1;
	emit sig_enable_applyBtn();
}
void DataSourcePropForm::on_particlesRes_spinBox_valueChanged(int arg1)
{
	particles_propData->resolution = arg1;
	emit sig_enable_applyBtn();
}
void DataSourcePropForm::on_seaBottom_checkBox_clicked(bool checked)
{
	mooringLine_propData->flag_seaBottom = checked;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_seaBotColor_pushButton_clicked()
{
	QColor init_color0 = QColor(mooringLine_propData->seaBottomColor.r, mooringLine_propData->seaBottomColor.g, mooringLine_propData->seaBottomColor.b);
	QColor color = QColorDialog::getColor(init_color0, this, tr("color dialog"));
	if (!color.isValid())
		return;
	int r = color.red();
	int g = color.green();
	int b = color.blue();
	mooringLine_propData->seaBottomColor.r = r;
	mooringLine_propData->seaBottomColor.g = g;
	mooringLine_propData->seaBottomColor.b = b;
	ui->seaBotColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	emit sig_enable_applyBtn();
}
void DataSourcePropForm::on_displace_checkBox_clicked(bool checked)
{
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile = checked;
	ui->disPara_groupBox->setEnabled(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile);
	if (!checked)
	{
		cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.path_AniDisplaceFile = " ";
		cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.clear();
		cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval = 1;
		cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.cur_pos = 0;
		cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.start_pos = 0;
		cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.dis_xh = 0;
		cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor = 1.0;
		cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity = false;
		//cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x = 0;
		//cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y = 0;
		//cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z = 0;
	}
	ui->displace_lineEdit->setText(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.path_AniDisplaceFile);
	ui->pos_spinBox->setRange(0, cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count());
	ui->pos_spinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.start_pos);
	ui->pos_spinBox->setSingleStep(1);
	ui->interval_spinBox->setRange(0, 1000);
	ui->interval_spinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval);
	ui->interval_spinBox->setSingleStep(1);
	ui->zoomFactor_doubleSpinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor);
	ui->gravityCenter_checkBox->setChecked(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity);
	ui->gravity_groupBox->setEnabled(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity);
	QString tep_str;
	ui->gravityX_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x, 'g', 6));
	ui->gravityY_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y, 'g', 6));
	ui->gravityZ_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z, 'g', 6));
	//emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_displace_pushButton_clicked()
{
	QString tepPath;
	tepPath = openDisDir;
	if (openDisDir != "")
	{
		QDir dir1(openDisDir);
		if (!dir1.exists())
			tepPath = "";
	}
    //QString tep_filename = QFileDialog::getOpenFileName(this, tr("open"), "", tr("file(*.dis *.csv)"));
	QString tep_filename = QFileDialog::getOpenFileName(this, tr("open"), tepPath, tr("file(*.dis *.csv)"));
	if (tep_filename.isEmpty())
		return;
	QFileInfo info1(tep_filename);
	openDisDir=info1.absolutePath();

	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.path_AniDisplaceFile = tep_filename;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.clear();
	if (tep_filename != NULL)
	{
		LoadActionFile* ptr = new LoadActionFile;
		QFileInfo tep_info(tep_filename);
		if (tep_info.suffix() == "dis")
		{
			if (ptr->Load(tep_filename, "Sway_GY"))
			{
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList = ptr->displaceList;
				cur_pipelineObj->func_init_displacement();
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.cur_pos = 0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.start_pos = 0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval = 1;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.dis_xh = 0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor = 1.0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity = false;
				//cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x = 0;
				//cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y = 0;
				//cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z = 0;
			}
			else
			{
				delete ptr;
				return;
			}
		}
		else if (tep_info.suffix() == "csv")
		{
			if (ptr->LoadCSV(tep_filename))
			{
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList = ptr->displaceList;
				cur_pipelineObj->func_init_displacement();
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.cur_pos = 0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.start_pos = 0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval = 1;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.dis_xh = 0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor = 1.0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity = false;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x = 0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y = 0;
				cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z = 0;
			}
			else
			{
				delete ptr;
				return;
			}
		}
		delete ptr;
	}
	else 
		return;
	ui->displace_lineEdit->setText(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.path_AniDisplaceFile);
	ui->pos_spinBox->setRange(0, cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count());
	ui->pos_spinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.start_pos);
	ui->pos_spinBox->setSingleStep(1);
	ui->interval_spinBox->setRange(0, 1000);
	ui->interval_spinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval);
	ui->interval_spinBox->setSingleStep(1);
	ui->zoomFactor_doubleSpinBox->setValue(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor);
	ui->gravityCenter_checkBox->setChecked(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity);
	ui->gravity_groupBox->setEnabled(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity);
	QString tep_str;
	ui->gravityX_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x, 'g', 6));
	ui->gravityY_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y, 'g', 6));
	ui->gravityZ_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z, 'g', 6));
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).time;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Roll_Gxb;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Pitch_Gyb;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Yaw_Gzb;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Surge_GX;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Sway_GY;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(0).Heave_GZ;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace = true;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_pos_spinBox_valueChanged(int arg1)
{
	if (arg1 >= cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count())
		return;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.dis_xh = 0;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.start_pos = arg1;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.cur_pos = arg1;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(arg1).time;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(arg1).Roll_Gxb;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(arg1).Pitch_Gyb;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(arg1).Yaw_Gzb;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(arg1).Surge_GX;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(arg1).Sway_GY;
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ = cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(arg1).Heave_GZ;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_interval_spinBox_valueChanged(int arg1)
{
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval = arg1;
	ui->pos_spinBox->setValue(0);
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_zoomFactor_doubleSpinBox_valueChanged(double arg1)
{
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor = arg1;
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_gravityCenter_checkBox_clicked(bool checked)
{
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity = checked;
	ui->gravity_groupBox->setEnabled(checked);
	if (checked)
	{
		QString tep_str;
		ui->gravityX_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x,'g',6));
		ui->gravityY_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y, 'g', 6));
		ui->gravityZ_lineEdit->setText(tep_str.setNum(cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z, 'g', 6));
	}
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_gravityX_lineEdit_editingFinished()
{
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x = ui->gravityX_lineEdit->text().toDouble();
	emit sig_enable_applyBtn();
}
void DataSourcePropForm::on_gravityY_lineEdit_editingFinished()
{
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y = ui->gravityY_lineEdit->text().toDouble();
	emit sig_enable_applyBtn();
}
void DataSourcePropForm::on_gravityZ_lineEdit_editingFinished()
{
	cur_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z = ui->gravityZ_lineEdit->text().toDouble();
	emit sig_enable_applyBtn();
}

void DataSourcePropForm::on_particlesType_comboBox_currentIndexChanged(int index)
{
	particles_propData->type = index;
	emit sig_enable_applyBtn();
}
