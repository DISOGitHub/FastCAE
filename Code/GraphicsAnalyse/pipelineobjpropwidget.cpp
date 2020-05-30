#include "pipelineobjpropwidget.h"
#include "ui_pipelineobjpropwidget.h"
#include "qicon.h"
#include "qcolordialog.h"
#include "qfiledialog.h"
#include "datasourcepropform.h"
#include "slicefilterpropform.h"
#include "FilterSlice.h"
#include "FilterClip.h"
#include "FilterVector.h"
#include "contourfilterpropform.h"
#include "vectorfilterpropform.h"
#include "refelctionfilterpropform.h"
#include "smoothfilterpropform.h"
#include "streamlinefilterpropform.h"
#include "calfilterpropform.h"
#include "qdebug.h"

PipelineObjPropWidget::PipelineObjPropWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PipelineObjPropWidget)
{
    ui->setupUi(this);
	ui->vectorColorXYZ_comboBox->setEnabled(false);
	openDisDir="";
	cur_pipelineObj=NULL;
	tep_scalarBar_Form = NULL;
	pipelineObj_propPtr = NULL;
    flag_propBtnExpand=false;
    flag_dispBtnExpand=false;
    flag_viewBtnpExpand=true;
	func_initRenderView();
	ui->color1_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(71).arg(136).arg(122));
	ui->color2_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(71).arg(136).arg(122));
	ui->btnspecularcolor->setStyleSheet(QString("QPushButton{background-color: rgb(%1,%2,%3); color:black;}").arg(255).arg(255).arg(255));
	ui->comboBox_backgroundType->setCurrentIndex(0);
	ui->BtnDisplayButton->setStyleSheet("QPushButton{text-align : left;}");
	ui->BtnPropertiesButton->setStyleSheet("QPushButton{text-align : left;}");
	ui->BtnViewButton->setStyleSheet("QPushButton{text-align : left;}");
	ui->BtnPropertiesButton->setIcon(QIcon(":/images/combine.png"));
	ui->BtnDisplayButton->setIcon(QIcon(":/images/combine.png"));
	ui->BtnViewButton->setIcon(QIcon(":/images/expand.png"));
	ui->pipelineObj_GloPropwidget->setVisible(false); 
	ui->groupBox->show();

	ui->camera_parallel_checkBox->setChecked(cur_renderview_global_prop_data.flag_camera_parallel);
	ui->orient_checkBox->setChecked(cur_renderview_global_prop_data.flag_axesVisible);
	ui->remark_checkBox->setChecked(cur_renderview_global_prop_data.remark_para.flag_remark);
	ui->remark_groupBox->setVisible(cur_renderview_global_prop_data.remark_para.flag_remark);
	if (cur_renderview_global_prop_data.remark_para.flag_remark)
	{
		ui->fontColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(cur_renderview_global_prop_data.remark_para.fontColor.red())
			.arg(cur_renderview_global_prop_data.remark_para.fontColor.green())
			.arg(cur_renderview_global_prop_data.remark_para.fontColor.blue()));
		ui->remark_textEdit->setText(cur_renderview_global_prop_data.remark_para.text);
	}
	ui->m_sliderOpacity->setRange(0, 100);
	ui->m_sliderOpacity->setValue(100);
	ui->m_edt_Opacity->setText("1.0");

	ui->m_slider_specular->setRange(0, 100);
	ui->m_slider_specular->setValue(0);
	ui->m_edt_specular->setText("0");
	ui->m_slider_ambient->setRange(0, 100);
	ui->m_slider_ambient->setValue(0);
	ui->m_edt_ambient->setText("0");
	ui->m_slider_diffuse->setRange(0, 100);
	ui->m_slider_diffuse->setValue(100);
	ui->m_edt_diffuse->setText("1.0");
	ui->m_silder_specularpower->setRange(0, 100);
	ui->m_silder_specularpower->setValue(100);
	ui->m_edt_specularpower->setText("100");
	cur_addWidget = NULL;
	properties_layout = new QVBoxLayout;
	ui->pipelineObj_propWidget->setLayout(properties_layout);
	ui->pipelineObj_propWidget->hide();
	ui->BtnAccept->setEnabled(false);
}
void PipelineObjPropWidget::func_initRenderView()
{
	cur_renderview_global_prop_data.back_color_type = 0;
	cur_renderview_global_prop_data.back_color1 = QColor(71, 136, 122);
	cur_renderview_global_prop_data.back_color2 = QColor(71, 136, 122);
	cur_renderview_global_prop_data.flag_axesVisible = true;
	cur_renderview_global_prop_data.flag_camera_parallel = false;
	cur_renderview_global_prop_data.flag_viewInteraction = false;
	cur_renderview_global_prop_data.image_file = "";
	cur_renderview_global_prop_data.remark_para.flag_remark = false;
	cur_renderview_global_prop_data.remark_para.fontColor = QColor(0,78,155);
	cur_renderview_global_prop_data.remark_para.fontSize = 6;
	cur_renderview_global_prop_data.remark_para.text = "This is a 3D window!";
	cur_renderview_global_prop_data.remark_para.position1[0] = 0.35;
	cur_renderview_global_prop_data.remark_para.position1[1] = 0.1;
	cur_renderview_global_prop_data.remark_para.position2[0] = 0.80;
	cur_renderview_global_prop_data.remark_para.position2[1] = 0.05;
}
PipelineObjPropWidget::~PipelineObjPropWidget()
{
    delete ui;
}
Ui::PipelineObjPropWidget* PipelineObjPropWidget::getUi() 
{ 
	return ui; 
}
void PipelineObjPropWidget::on_BtnPropertiesButton_clicked()
{
	if (cur_pipelineObj == NULL)
		return;
    if(flag_propBtnExpand)
    {
        flag_propBtnExpand=false;
        ui->BtnPropertiesButton->setIcon(QIcon(":/images/combine.png"));
        ui->pipelineObj_propWidget->hide();
    }
    else
    {
        flag_propBtnExpand=true;
        ui->BtnPropertiesButton->setIcon(QIcon(":/images/expand.png"));
        ui->pipelineObj_propWidget->show();
    }
}

void PipelineObjPropWidget::on_BtnDisplayButton_clicked()
{
	if (cur_pipelineObj == NULL)
		return;
    if(flag_dispBtnExpand)
    {
        flag_dispBtnExpand=false;
        ui->BtnDisplayButton->setIcon(QIcon(":/images/combine.png"));
        ui->pipelineObj_GloPropwidget->setVisible(false);
    }
    else
    {
        flag_dispBtnExpand=true;
        ui->BtnDisplayButton->setIcon(QIcon(":/images/expand.png"));
        ui->pipelineObj_GloPropwidget->setVisible(true);
    }
}

void PipelineObjPropWidget::on_BtnViewButton_clicked()
{
    if(flag_viewBtnpExpand)
    {
        flag_viewBtnpExpand=false;
        ui->BtnViewButton->setIcon(QIcon(":/images/combine.png"));
        ui->groupBox->hide();
    }
    else
    {
        flag_viewBtnpExpand=true;
        ui->BtnViewButton->setIcon(QIcon(":/images/expand.png"));
        ui->groupBox->show();
    }
}

void PipelineObjPropWidget::on_comboBox_backgroundType_currentIndexChanged(int index)
{
	if (index == -1)
		return;
    cur_renderview_global_prop_data.back_color_type=index;
    if(index==0)
    {
        //ui->color1_pushButton->setText(" ");
        ui->color2_pushButton->setEnabled(false);
        //ui->image_lineEdit->setEnabled(false);
        emit sig_change_global_view_prop();//cur_renderview_global_prop_data
    }
    else if(index==1)
    {
        //ui->color1_pushButton->setText("");
        ui->color2_pushButton->setEnabled(true);
		emit sig_change_global_view_prop();
        //ui->image_lineEdit->setEnabled(false);
    }
    /*else if(index==2)
    {
        ui->color1_pushButton->setText(QObject::tr("please select a background image."));
        ui->color2_pushButton->setEnabled(false);
        ui->image_lineEdit->setEnabled(true);
    }*/
	emit sig_python_view_backgroundType(index);

}

void PipelineObjPropWidget::on_color1_pushButton_clicked()
{
	if (ui->comboBox_backgroundType->currentIndex() != 2)
    {
		QColor init_color0 = cur_renderview_global_prop_data.back_color1;
		QColor color = QColorDialog::getColor(init_color0, this, tr("color dialog"));
		if (!color.isValid())
			return;
		cur_renderview_global_prop_data.back_color1 = color;
		int r = color.red();
		int g = color.green();
		int b = color.blue();
		ui->color1_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
		emit sig_change_global_view_prop();
		emit sig_python_view_backgroundColor(color);
    }
    else
    {
        //自定义文件
        QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), "",tr("backgound image(*.jpg)"));
        //ui->lineEdit->setText(fileName);
        cur_renderview_global_prop_data.image_file=fileName;
        emit sig_change_global_view_prop();
    }
}

void PipelineObjPropWidget::on_color2_pushButton_clicked()
{
	QColor init_color0 = cur_renderview_global_prop_data.back_color2;
	QColor color = QColorDialog::getColor(init_color0, this, tr("color dialog"));
	if (!color.isValid())
		return;
	cur_renderview_global_prop_data.back_color2 = color;
	int r = color.red();
	int g = color.green();
	int b = color.blue();
	ui->color2_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	emit sig_change_global_view_prop();
	emit sig_python_view_background2Color(color);
}

//void PipelineObjPropWidget::update_pipelineObjList_for_comModel(DataSourcePropForm *tep_widget, QList<PipelineObject*> tep_mPipelnObjs)
//{
//	tep_widget->mPipelnObjs.clear();
//	tep_widget->mPipelnObjs.append(tep_mPipelnObjs);
//}
void PipelineObjPropWidget::add_propForm(PipelineObject *tep_pipeObj)
{
	if (cur_addWidget != NULL)
	{
		properties_layout->removeWidget(cur_addWidget);
		delete cur_addWidget;
		cur_addWidget = NULL;
	}
	if (tep_pipeObj->GetObjectType() == dNumeric_DataSource)
	{
		DataSourcePropForm *tep_widget = new DataSourcePropForm(this);
		//update_pipelineObjList_for_comModel(tep_widget, glo_ani_objList);
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->block_showMap = tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap;
		tep_widget->m_colorList = tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
		tep_widget->suffix_filename = QFileInfo(tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileName).suffix();
		tep_widget->mooringLine_propData = &(tep_pipeObj->mPipeLineObjProp.mooringLine_propData);
		tep_widget->particles_propData = &(tep_pipeObj->mPipeLineObjProp.particles_propData);
		tep_widget->updateForm();
		cur_addWidget = tep_widget;
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_update_pipelineObj_colorPropWidget(QString, bool)), this, SLOT(slot_update_pipelineObj_colorPropWidget(QString, bool)));
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn()));
	}
	else if (tep_pipeObj->GetObjectType() == dSlice_DataSource)
	{
		sliceFilterPropForm *tep_widget = new sliceFilterPropForm(this);
		tep_widget->plane_propData = &(tep_pipeObj->mPipeLineObjProp.plane_propData);
		tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList = tep_pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->updateForm();
		cur_addWidget = tep_widget;
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_update_planeOrigin(PipelineObject *)), this, SLOT(slot_update_planeOrigin(PipelineObject *)));
		connect(tep_widget, SIGNAL(sig_update_planeNormal(PipelineObject *)), this, SLOT(slot_update_planeNormal(PipelineObject *)));
		connect(tep_widget, SIGNAL(sig_update_planeCameraNormal(PipelineObject *)), this, SLOT(slot_update_planeCameraNormal(PipelineObject *)));
		connect(tep_widget, SIGNAL(sig_update_planeVis(PipelineObject *)), this, SLOT(slot_update_planeVis(PipelineObject *)));
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn()));
		connect(((FilterSlice*)tep_pipeObj)->get_callback(), SIGNAL(sig_update_planeProp_widget(double*, double*)), tep_widget, SLOT(slot_update_planeProp_widget(double*, double*)));

	
	}
	else if (tep_pipeObj->GetObjectType() == dClip_DataSource)
	{
		sliceFilterPropForm *tep_widget = new sliceFilterPropForm(this);
		tep_widget->plane_propData = &(tep_pipeObj->mPipeLineObjProp.plane_propData);
		tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList = tep_pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->updateForm();
		cur_addWidget = tep_widget;
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_update_planeOrigin(PipelineObject *)), this, SLOT(slot_update_planeOrigin(PipelineObject *)));
		connect(tep_widget, SIGNAL(sig_update_planeNormal(PipelineObject *)), this, SLOT(slot_update_planeNormal(PipelineObject *)));
		connect(tep_widget, SIGNAL(sig_update_planeCameraNormal(PipelineObject *)), this, SLOT(slot_update_planeCameraNormal(PipelineObject *)));
		connect(tep_widget, SIGNAL(sig_update_planeVis(PipelineObject *)), this, SLOT(slot_update_planeVis(PipelineObject *)));
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn()));
		connect(((FilterClip*)tep_pipeObj)->get_callback(), SIGNAL(sig_update_planeProp_widget(double*, double*)), tep_widget, SLOT(slot_update_planeProp_widget(double*, double*)));

	}
	else if (tep_pipeObj->GetObjectType() == dContour_DataSource)
	{
		contourFilterPropForm *tep_widget = new contourFilterPropForm(this);
		tep_widget->contourFilter_propData = &(tep_pipeObj->mPipeLineObjProp.contourFilter_propData);
		tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList = tep_pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
		//tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append("Axes_Z");
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->updateForm();
		cur_addWidget = tep_widget;		
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn()));
	}
	else if (tep_pipeObj->GetObjectType() == dVector_DataSource)
	{
		vectorFilterPropForm *tep_widget = new vectorFilterPropForm(this);
		tep_widget->vectorFilter_propData = &(tep_pipeObj->mPipeLineObjProp.vectorFilter_propData);
		tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList.clear();
		//tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append("GlyphVector");
		tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList.append(tep_pipeObj->mPipeLineObjProp.vectorFilter_propData.scalar_list);
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->updateForm();
		cur_addWidget = tep_widget;
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn()));
	}
	else if (tep_pipeObj->GetObjectType() == dReflection_DataSource)
	{
		refelctionFilterPropForm *tep_widget = new refelctionFilterPropForm(this);
		tep_widget->reflectionFilter_propData = &(tep_pipeObj->mPipeLineObjProp.reflectionFilter_propData);
		tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList = tep_pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->updateForm();
		cur_addWidget = tep_widget;
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn()));
	}
	else if (tep_pipeObj->GetObjectType() == dCalculator_DataSource)
	{
		calFilterPropForm *tep_widget = new calFilterPropForm(this);
		tep_widget->calculatorFilter_propData = &(tep_pipeObj->mPipeLineObjProp.calculatorFilter_propData);
		//tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList = tep_pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->updateForm();
		cur_addWidget = tep_widget;
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn()));
	}
	else if (tep_pipeObj->GetObjectType() == dSmooth_DataSource)
	{
		smoothFilterPropForm *tep_widget = new smoothFilterPropForm(this);
		tep_widget->smoothFilter_propData = &(tep_pipeObj->mPipeLineObjProp.smoothFilter_propData);
		tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList = tep_pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->updateForm();
		cur_addWidget = tep_widget;
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn()));
	}
	else if (tep_pipeObj->GetObjectType() == dStreamLine_DataSource)
	{
		streamLineFilterPropForm *tep_widget = new streamLineFilterPropForm(this);
		tep_widget->streamLineFilter_propData = &(tep_pipeObj->mPipeLineObjProp.streamLineFilter_propData);
		tep_pipeObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList = tep_pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
		tep_widget->cur_pipelineObj = tep_pipeObj;
		tep_widget->updateForm();
		cur_addWidget = tep_widget;
		properties_layout->addWidget(tep_widget);
		connect(tep_widget, SIGNAL(sig_enable_applyBtn()), this, SLOT(slot_enable_applyBtn())); 
	}
}
void PipelineObjPropWidget::func_update_colorComboBox_glyphVector(bool flag)
{
	if (cur_pipelineObj == NULL)
		return;
	if (flag)
	{
		int index=ui->m_cmb_ColumnList->findText("GlyphVector");
		if (index < 0)
		{
			QIcon icon3 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
			ui->m_cmb_ColumnList->addItem(icon3, "GlyphVector", "val_on_point.png");
		}
		//ui->vectorColorXYZ_comboBox->setVisible(true);
	}
	else
	{
		int index = ui->m_cmb_ColumnList->findText("GlyphVector");
		if (index >= 0)
		{
			ui->m_cmb_ColumnList->removeItem(index);
		}
		//ui->vectorColorXYZ_comboBox->setVisible(false);
	}
	emit sig_update_toolBar_objProp_column_GlyphVector(flag);
}
void PipelineObjPropWidget::func_update_colorComboBox_calculator()
{
	if (cur_pipelineObj == NULL)
		return;
	ui->m_cmb_ColumnList->blockSignals(true);
	ui->m_cmb_ColumnList->clear();
	QString tep_str;
	QStringList listColumn = cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList;
	QIcon icon3 = QIcon::fromTheme("solidcolor.png", QIcon(":/images/solidcolor.png"));
	ui->m_cmb_ColumnList->addItem(icon3, "SolidColor", "solidcolor.png");
	for (int i = 0; i < listColumn.count(); i++)
	{
		tep_str = listColumn.at(i);
		if (tep_str.indexOf("_cell000") >= 0)
		{
			QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
			ui->m_cmb_ColumnList->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
		}
		else
		{
			if (tep_str == "vtkBlockColors")
			{
				//continue;// add for debug zone
				QIcon icon4 = QIcon::fromTheme("blockColors.png", QIcon(":/images/blockColors.png"));
				ui->m_cmb_ColumnList->addItem(icon4, "vtkBlockColors", "blockColors.png");
			}
			else
			{
				QIcon icon2 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
				ui->m_cmb_ColumnList->addItem(icon2, tep_str, "val_on_point.png");
			}
		}
	}
	ui->m_cmb_ColumnList->blockSignals(false);
	emit sig_update_toolBar_objProp_column_Calculator();
}
void PipelineObjPropWidget::slot_update_buildIn_propForm()
{
	if (cur_addWidget != NULL)
	{
		properties_layout->removeWidget(cur_addWidget);
		delete cur_addWidget;
		cur_addWidget = NULL;
	}
	ui->BtnPropertiesButton->setText(tr("Properties"));
	ui->BtnPropertiesButton->setIcon(QIcon(":/images/combine.png"));
	ui->pipelineObj_propWidget->hide();

	ui->BtnDisplayButton->setText(tr("Display"));
	ui->BtnDisplayButton->setIcon(QIcon(":/images/combine.png"));
	ui->pipelineObj_GloPropwidget->setVisible(false);

	flag_propBtnExpand = false;
	flag_dispBtnExpand = false;
	cur_pipelineObj = NULL;
}

void PipelineObjPropWidget::slot_update_pipelineObj_propForm(PipelineObject *tep_pipeObj)
{
	QString tep_str;
    cur_pipelineObj=tep_pipeObj;
	pipelineObj_propPtr = &(tep_pipeObj->mPipeLineObjProp);
	add_propForm(tep_pipeObj);
	//ui->pipelineObj_GloPropwidget->setVisible(true);
	if (tep_pipeObj->GetObjectType()==dNumeric_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1)")).arg( pipelineObj_propPtr->pipelineObj_base_propData.fileName));
	else if (tep_pipeObj->GetObjectType() == dClip_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->plane_propData.filterName));
	else if (tep_pipeObj->GetObjectType() == dSlice_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->plane_propData.filterName));
	else if (tep_pipeObj->GetObjectType() == dContour_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->contourFilter_propData.filterName));
	else if (tep_pipeObj->GetObjectType() == dVector_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->vectorFilter_propData.filterName));
	else if (tep_pipeObj->GetObjectType() == dReflection_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->reflectionFilter_propData.filterName));
	else if (tep_pipeObj->GetObjectType() == dCalculator_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->calculatorFilter_propData.filterName));
	else if (tep_pipeObj->GetObjectType() == dSmooth_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->smoothFilter_propData.filterName));
	else if (tep_pipeObj->GetObjectType() == dStreamLine_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->streamLineFilter_propData.filterName));
	
	//20190121/////////////////////////////////////////
	//ui->m_cmb_Representation->setEnabled(false);
	//20190121/////////////////////////////////////////
	ui->m_cmb_Representation->setCurrentIndex(pipelineObj_propPtr->pipelineObj_base_propData.m_enum_representationtype);

	ui->m_cmb_ColumnList->blockSignals(true);
	ui->m_cmb_ColumnList->clear();
	ui->m_cmb_ColumnList->setInsertPolicy(QComboBox::InsertAtBottom);
	QIcon icon0 = QIcon::fromTheme("solidcolor.png", QIcon(":/images/solidcolor.png"));
	ui->m_cmb_ColumnList->addItem(icon0, "SolidColor", "solidcolor.png");//ui->m_cmb_ColumnList->addItem("SolidColor");
	QStringList listColumn = pipelineObj_propPtr->pipelineObj_base_propData.m_colorList;
	for (int i = 0; i < listColumn.count(); i++)
	{
		tep_str = listColumn.at(i);
		if (tep_str.indexOf("_cell000") >= 0)
		{
			QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
			ui->m_cmb_ColumnList->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
		}
		else
		{
			if (tep_str == "vtkBlockColors")
			{
				//continue;// add for debug zone
				QIcon icon4 = QIcon::fromTheme("blockColors.png", QIcon(":/images/blockColors.png"));
				ui->m_cmb_ColumnList->addItem(icon4, "vtkBlockColors", "blockColors.png");
			}
			else
			{
				QIcon icon2 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
				ui->m_cmb_ColumnList->addItem(icon2, tep_str, "val_on_point.png");
			}
		}
	}
	ui->m_cmb_ColumnList->blockSignals(false);
	
	bool flag_xyz = false;
	QString tep_column = pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle;
	if (tep_column.indexOf("_cell000") >= 0)
	{
		tep_column = tep_column.left(tep_column.length() - 8);
		if (tep_pipeObj->mBaseGrid->GetCellData()->HasArray(tep_column.toStdString().data()))
		{
			vtkFloatArray* tep_aaa = (vtkFloatArray*)tep_pipeObj->mBaseGrid->GetCellData()->GetArray(tep_column.toStdString().data());
			if (tep_aaa->GetNumberOfComponents() == 3)
			{
				flag_xyz = true;
			}
		}
	}
	else if ((tep_pipeObj->mBaseGrid!=NULL) && (tep_pipeObj->mBaseGrid->GetPointData()->HasArray(tep_column.toStdString().data())))
	{
		vtkFloatArray* tep_aaa = (vtkFloatArray*)tep_pipeObj->mBaseGrid->GetPointData()->GetArray(tep_column.toStdString().data());
		if (tep_aaa->GetNumberOfComponents() == 3)
		{
			flag_xyz = true;
		}
	}
	if (flag_xyz)
	{
		ui->vectorColorXYZ_comboBox->setEnabled(true);
		ui->vectorColorXYZ_comboBox->setCurrentText(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle_XYZ);
		on_vectorColorXYZ_comboBox_currentIndexChanged(ui->vectorColorXYZ_comboBox->currentIndex());
	}
	else
	{
		ui->vectorColorXYZ_comboBox->setEnabled(true);
	}

	//add for vector glyphvector
	if (cur_pipelineObj->GetObjectType() == dVector_DataSource)
	{
		if (pipelineObj_propPtr->vectorFilter_propData.vectorColName != "None")
		{
			func_update_colorComboBox_glyphVector(true);
			ui->vectorColorXYZ_comboBox->setCurrentText(pipelineObj_propPtr->vectorFilter_propData.glyphVector_colorXYZ);
			on_vectorColorXYZ_comboBox_currentIndexChanged(ui->vectorColorXYZ_comboBox->currentIndex());
		}
		else
			func_update_colorComboBox_glyphVector(false);
	}
	else if ((cur_pipelineObj->GetObjectType() == dCalculator_DataSource) 
		&& (cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.m_colorList.count()>cur_pipelineObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.m_colorList.count()))
	{
		func_update_colorComboBox_calculator();
	}
	ui->m_cmb_ColumnList->setCurrentText(tep_column);
	
	ui->m_sliderOpacity->setValue(pipelineObj_propPtr->pipelineObj_base_propData.mOpacity*100);
    ui->m_edt_Opacity->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.mOpacity,'g',2));
	ui->m_edt_PointSize->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.mPointSize));
	ui->m_edt_linewidth->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.mLineWidth));
	ui->m_edt_tranx->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_transforming.x));
	ui->m_edt_trany->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_transforming.y));
	ui->m_edt_tranz->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_transforming.z));
	ui->m_edt_scalex->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_scale.x));
	ui->m_edt_scaley->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_scale.y));
	ui->m_edt_scalez->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_scale.z));
	ui->m_edt_orientx->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_orientation.x));
	ui->m_edt_orienty->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_orientation.y));
	ui->m_edt_orientz->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_orientation.z));
	ui->m_edt_originx->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_origin.x));
	ui->m_edt_originy->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_origin.y));
	ui->m_edt_originz->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.m_struct_origin.z));
	ui->m_cmb_interpolation->setCurrentIndex(pipelineObj_propPtr->pipelineObj_base_propData.m_enum_interpolationtype);
	ui->m_slider_specular->setValue(pipelineObj_propPtr->pipelineObj_base_propData.mSpecular * 100);
	ui->m_edt_specular->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.mSpecular, 'g', 2));
	ui->m_slider_diffuse->setValue(pipelineObj_propPtr->pipelineObj_base_propData.mDiffuse * 100);
	ui->m_edt_diffuse->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.mDiffuse, 'g', 2));
	ui->m_slider_ambient->setValue(pipelineObj_propPtr->pipelineObj_base_propData.mAmbient * 100);
	ui->m_edt_ambient->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.mAmbient, 'g', 2));
	ui->m_silder_specularpower->setValue(pipelineObj_propPtr->pipelineObj_base_propData.mSpecularPower);
	ui->m_edt_specularpower->setText(tep_str.setNum(pipelineObj_propPtr->pipelineObj_base_propData.mSpecularPower, 10));
	ui->edgeColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.r).arg(pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.g).arg(pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.b));
	ui->btnspecularcolor->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.r).arg(pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.g).arg(pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.b));
	ui->cubeAxes_checkBox->setChecked(pipelineObj_propPtr->pipelineObj_base_propData.flag_cubeAxes);
	
	if (!tep_pipeObj->flag_exist_actors)
		ui->BtnAccept->setEnabled(true);
	if (tep_scalarBar_Form != NULL)
		tep_scalarBar_Form->get_scalarBarEditForm()->updateForm(tep_pipeObj);
	//emit sig_setFlagRender(true);
}
void PipelineObjPropWidget::slot_setFlagRender(bool flag)
{
	flag_render = flag;
}
void PipelineObjPropWidget::on_BtnAccept_clicked()
{
	emit sig_refreshView_fromPropWidget();
	ui->BtnAccept->setEnabled(false);
	if (cur_pipelineObj != NULL)
	{
		if (cur_pipelineObj->GetObjectType()==dNumeric_DataSource)
			emit sig_update_filterToolBar(true, cur_pipelineObj);
	}	
	emit sig_python_applyClicked();
}

void PipelineObjPropWidget::on_m_sliderOpacity_sliderMoved(int position)
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
    QString tep_str;   
    pipelineObj_propPtr->pipelineObj_base_propData.mOpacity=(float)position/100.0;
    ui->m_edt_Opacity->setText(tep_str.setNum((float)position/100.0,'g',2));
	if (cur_pipelineObj->flag_exist_actors)
	{
		emit sig_update_opacity(cur_pipelineObj);
		emit sig_python_opacity(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_cmb_ColumnList_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	
	if (ui->m_cmb_ColumnList->count() < 1)
		return;
	Def_PipelineObjType tep_type= cur_pipelineObj->GetObjectType();
	QString tep_str;
	PipelineObject* parent_obj = NULL;
	if (tep_type == dNumeric_DataSource)
		parent_obj = cur_pipelineObj;
	else
		parent_obj = cur_pipelineObj->GetObjParent();
	tep_str = ui->m_cmb_ColumnList->currentText();
	bool flag_xyz = false;
	if (ui->m_cmb_ColumnList->itemData(index).toString().indexOf("val_on_cell.png") >= 0)
	{
		flag_xyz = cur_pipelineObj->func_isVector(tep_str.trimmed(), 1, parent_obj->mBaseGrid);
		tep_str += "_cell000";
	}
	else if (cur_pipelineObj->mBaseGrid->GetPointData()->HasArray(tep_str.toStdString().data()))
	{
		flag_xyz = cur_pipelineObj->func_isVector(tep_str.trimmed(), 0, parent_obj->mBaseGrid);
	}
	if (flag_xyz)
	{
		ui->vectorColorXYZ_comboBox->setEnabled(false);
		ui->vectorColorXYZ_comboBox->setCurrentText(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle_XYZ);
		ui->vectorColorXYZ_comboBox->setEnabled(true);
		//on_vectorColorXYZ_comboBox_currentIndexChanged(ui->vectorColorXYZ_comboBox->currentIndex());
	}
	else
	{
		ui->vectorColorXYZ_comboBox->setEnabled(false);
		cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ = "";
	}

	if (pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible)
	{
		if ((cur_pipelineObj->GetObjectType() == dCalculator_DataSource)
			&& (pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle == cur_pipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			if (parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
				parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("CalculatorResult", false);
		}
		else
		{
			if (parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle))
				parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle, false);
		}
		if ((index > 0))
		{	
			if ((cur_pipelineObj->GetObjectType() == dCalculator_DataSource)
				&&(tep_str==cur_pipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
			{
				if (parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
				{
					parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("CalculatorResult", true);
				}
			}
			else
			{
				if (parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(tep_str))
				{
					//if (!parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[tep_str])
					//{
					parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_str, true);
					//}
				}
			}
		}
		else if (index == 0)
		{
			pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = false;
		}
	}
	pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle = tep_str;
	if (flag_xyz)
	{
		cur_pipelineObj->func_update_componentXYX_lookupTableMap(tep_str);
	}
    /*pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle=ui->m_cmb_ColumnList->currentText();
	if (ui->m_cmb_ColumnList->itemData(index).toString().indexOf("val_on_cell.png") >= 0)
		pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle += "_cell000";
	if ((pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle != "SolidColor") && (pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible))
		parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle,true);*/
	if (ui->m_cmb_ColumnList->currentText() == tr("GlyphVector"))
	{
		ui->vectorColorXYZ_comboBox->setEnabled(true);
		ui->vectorColorXYZ_comboBox->setCurrentText(pipelineObj_propPtr->vectorFilter_propData.glyphVector_colorXYZ);
		on_vectorColorXYZ_comboBox_currentIndexChanged(ui->vectorColorXYZ_comboBox->currentIndex());
		/*if (pipelineObj_propPtr->vectorFilter_propData.vectorColName != "None")
			ui->vectorColorXYZ_comboBox->setEnabled(true);
		else
			ui->vectorColorXYZ_comboBox->setEnabled(false);*/
	}
	/*else
	{
		ui->vectorColorXYZ_comboBox->setEnabled(false);
	}*/
	if (cur_pipelineObj->flag_exist_actors)
	{
		emit sig_update_color(cur_pipelineObj);
		emit sig_python_color(cur_pipelineObj);
	}
    //cur_pipelineObj->ChangeColumnColor();
	emit sig_update_toolBar_objProp_column(index);	
}

void PipelineObjPropWidget::on_m_edt_PointSize_editingFinished()
{
    if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
        return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.mPointSize = ui->m_edt_PointSize->text().toInt(&ok, 10);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_PointSize->hasFocus())
	{
		emit sig_update_pointSize(cur_pipelineObj);
		emit sig_python_pointSize(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_linewidth_editingFinished()
{
    if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
        return;
    bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.mLineWidth = ui->m_edt_linewidth->text().toInt(&ok, 10);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_linewidth->hasFocus())
	{
		emit sig_update_lineWidth(cur_pipelineObj);
		emit sig_python_lineWidth(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_tranx_editingFinished()
{
    if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
        return;
    bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_transforming.x = ui->m_edt_tranx->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_tranx->hasFocus())
	{
		emit sig_update_translate(cur_pipelineObj);
		emit sig_python_translate(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_trany_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_transforming.y = ui->m_edt_trany->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_trany->hasFocus())
	{
		emit sig_update_translate(cur_pipelineObj);
		emit sig_python_translate(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_tranz_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_transforming.z = ui->m_edt_tranz->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_tranz->hasFocus())
	{
		emit sig_update_translate(cur_pipelineObj);
		emit sig_python_translate(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_scalex_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_scale.x = ui->m_edt_scalex->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_scalex->hasFocus())
	{
		emit sig_update_scale(cur_pipelineObj);
		emit sig_python_scale(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_scaley_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_scale.y = ui->m_edt_scaley->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_scaley->hasFocus())
	{
		emit sig_update_scale(cur_pipelineObj);
		emit sig_python_scale(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_scalez_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_scale.z = ui->m_edt_scalez->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_scalez->hasFocus())
	{
		emit sig_update_scale(cur_pipelineObj);
		emit sig_python_scale(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_orientx_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_orientation.x = ui->m_edt_orientx->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_orientx->hasFocus())
	{
		emit sig_update_orientation(cur_pipelineObj);
		emit sig_python_orientation(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_orienty_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_orientation.y = ui->m_edt_orienty->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_orienty->hasFocus())
	{
		emit sig_update_orientation(cur_pipelineObj);
		emit sig_python_orientation(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_orientz_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_orientation.z = ui->m_edt_orientz->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_orientz->hasFocus())
	{
		emit sig_update_orientation(cur_pipelineObj);
		emit sig_python_orientation(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_originx_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_origin.x = ui->m_edt_originx->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_originx->hasFocus())
	{
		emit sig_update_origin(cur_pipelineObj);
		emit sig_python_origin(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_originy_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_origin.y = ui->m_edt_originy->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_originy->hasFocus())
	{
		emit sig_update_origin(cur_pipelineObj);
		emit sig_python_origin(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_edt_originz_editingFinished()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	bool ok;
	pipelineObj_propPtr->pipelineObj_base_propData.m_struct_origin.z = ui->m_edt_originz->text().toDouble(&ok);
	if (cur_pipelineObj->flag_exist_actors && ui->m_edt_originz->hasFocus())
	{
		emit sig_update_origin(cur_pipelineObj);
		emit sig_python_origin(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_cmb_Representation_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if ((cur_pipelineObj == NULL) || (pipelineObj_propPtr == NULL) || (index<0) || (index>=ui->m_cmb_Representation->count()))
		return;
	pipelineObj_propPtr->pipelineObj_base_propData.m_enum_representationtype = (ERepresentationType)index;
	if (cur_pipelineObj->flag_exist_actors)
	{
		emit sig_update_representation(cur_pipelineObj);
		emit sig_python_representation(cur_pipelineObj);
	}
	emit sig_update_toolBar_objProp_present(index);
}

void PipelineObjPropWidget::on_orient_checkBox_clicked(bool checked)
{
	cur_renderview_global_prop_data.flag_axesVisible = checked;
	emit sig_update_axes_visible(checked);
	emit sig_python_view_axesVisible(checked);
}

void PipelineObjPropWidget::on_m_slider_specular_sliderMoved(int position)
{
    if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
    	return;
    QString tep_str;
    pipelineObj_propPtr->pipelineObj_base_propData.mSpecular = (float)position / 100.0;
    ui->m_edt_specular->setText(tep_str.setNum((float)position / 100.0, 'g', 2));
	if (cur_pipelineObj->flag_exist_actors)
	{
		emit sig_update_specular(cur_pipelineObj);
		emit sig_python_specular(cur_pipelineObj);
	}	
}

void PipelineObjPropWidget::on_m_silder_specularpower_sliderMoved(int position)
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	QString tep_str;
	pipelineObj_propPtr->pipelineObj_base_propData.mSpecularPower = position;
	ui->m_edt_specularpower->setText(tep_str.setNum(position ,10));
	if (cur_pipelineObj->flag_exist_actors)
	{
		emit sig_update_specularPower(cur_pipelineObj);
		emit sig_python_specularPower(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_slider_ambient_sliderMoved(int position)
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	QString tep_str;
	pipelineObj_propPtr->pipelineObj_base_propData.mAmbient = (float)position / 100.0;
	ui->m_edt_ambient->setText(tep_str.setNum((float)position / 100.0, 'g', 2));
	if (cur_pipelineObj->flag_exist_actors)
	{
		emit sig_update_ambient(cur_pipelineObj);
		emit sig_python_ambient(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_slider_diffuse_sliderMoved(int position)
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	QString tep_str;
	pipelineObj_propPtr->pipelineObj_base_propData.mDiffuse = (float)position / 100.0;
	ui->m_edt_diffuse->setText(tep_str.setNum((float)position / 100.0, 'g', 2));
	if (cur_pipelineObj->flag_exist_actors)
	{
		emit sig_update_diffuse(cur_pipelineObj);
		emit sig_python_diffuse(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_m_cmb_interpolation_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	QString tep_str;
	pipelineObj_propPtr->pipelineObj_base_propData.m_enum_interpolationtype = (EInterpolationType)index;
	if (cur_pipelineObj->flag_exist_actors)
	{
		emit sig_update_interpolation(cur_pipelineObj);
		emit sig_python_interpolation(cur_pipelineObj);
	}
}

void PipelineObjPropWidget::on_btnspecularcolor_clicked()
{
	QColor init_color0 = QColor(pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.r, pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.g, pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.b);
	QColor color = QColorDialog::getColor(init_color0, this,tr("color dialog"));
	if (!color.isValid())
		return;
	int r = color.red();
	int g = color.green();
	int b = color.blue();
	pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.r = r;
	pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.g = g;
	pipelineObj_propPtr->pipelineObj_base_propData.mSpecularColor.b = b;
	
	ui->btnspecularcolor->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	emit sig_update_specularColor(cur_pipelineObj);
	emit sig_python_specularColor(cur_pipelineObj);
}

void PipelineObjPropWidget::slot_update_pipelineObj_colorPropWidget(QString item_name,bool vis)
{
	QString tep_str;
	ui->m_cmb_ColumnList->blockSignals(true);
	ui->m_cmb_ColumnList->clear();
	ui->m_cmb_ColumnList->setInsertPolicy(QComboBox::InsertAtBottom);
	QIcon icon0 = QIcon::fromTheme("solidcolor.png", QIcon(":/images/solidcolor.png"));
	ui->m_cmb_ColumnList->addItem(icon0, "SolidColor", "solidcolor.png");//ui->m_cmb_ColumnList->addItem("SolidColor");
	QStringList listColumn = pipelineObj_propPtr->pipelineObj_base_propData.m_colorList;
	bool flag_xyz = false;
	if (item_name == "Data Arrays")
	{
		if (vis)
		{
			for (int i = 0; i < listColumn.count(); i++)
			{
				tep_str = listColumn.at(i);
				if (tep_str.indexOf("_cell000") >= 0)
				{
					QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
					ui->m_cmb_ColumnList->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
				}
				else
				{
					QIcon icon3 = QIcon::fromTheme("solidcolor.png", QIcon(":/images/solidcolor.png"));
						ui->m_cmb_ColumnList->addItem(icon3, tep_str, "solidcolor.png");
				}
			}
			ui->m_cmb_ColumnList->setCurrentText(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle);
		}
	}
	else
	{
		for (int i = 0; i < listColumn.count(); i++)
		{
			tep_str = listColumn.at(i);
			if ((tep_str == item_name) && (!vis))
				continue;
			if (tep_str.indexOf("_cell000") >= 0)
			{
				QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
				ui->m_cmb_ColumnList->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
			}
			else
			{
				if (tep_str != "SolidColor")
				{
					QIcon icon2 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
					ui->m_cmb_ColumnList->addItem(icon2, tep_str, "val_on_point.png");
				}
				else
				{
					QIcon icon3 = QIcon::fromTheme("solidcolor.png", QIcon(":/images/solidcolor.png"));
					ui->m_cmb_ColumnList->addItem(icon3, tep_str, "solidcolor.png");
				}
			}
		}
		if (ui->m_cmb_ColumnList->count() > 1)
		{
			if ((item_name == pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle) && (!vis))
			{
				ui->m_cmb_ColumnList->setCurrentIndex(0);
				pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle = "SolidColor";
			}
			else
			{
				ui->m_cmb_ColumnList->setCurrentText(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle);
				//ui->scalarBar_checkBox->setEnabled(true);
			}
		}
		else if (ui->m_cmb_ColumnList->count() == 1)
		{
			ui->m_cmb_ColumnList->setCurrentIndex(0);
		}

	}
	
	////add for multi zone
	ui->m_cmb_ColumnList->blockSignals(false);
	if (pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle != "SolidColor")
	{
		QString tep_column = pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle;
		if (tep_column.indexOf("_cell000") >= 0)
		{
			tep_column = tep_column.left(tep_column.length() - 8);
			vtkFloatArray* tep_aaa = (vtkFloatArray*)cur_pipelineObj->mBaseGrid->GetCellData()->GetArray(tep_column.toStdString().data());
			if (tep_aaa->GetNumberOfComponents() == 3)
			{
				flag_xyz = true;
			}
		}
		else if (cur_pipelineObj->mBaseGrid->GetPointData()->HasArray(tep_column.toStdString().data()))
		{
			vtkFloatArray* tep_aaa = (vtkFloatArray*)cur_pipelineObj->mBaseGrid->GetPointData()->GetArray(tep_column.toStdString().data());
			if (tep_aaa->GetNumberOfComponents() == 3)
			{
				flag_xyz = true;
			}
		}
	}
	if (flag_xyz)
	{
		ui->vectorColorXYZ_comboBox->setEnabled(true);
		ui->vectorColorXYZ_comboBox->setCurrentText(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle_XYZ);
		on_vectorColorXYZ_comboBox_currentIndexChanged(ui->vectorColorXYZ_comboBox->currentIndex());
	}
	else
	{
		ui->vectorColorXYZ_comboBox->setEnabled(true);
	}
}

void PipelineObjPropWidget::slot_enable_applyBtn()
{
	if (!ui->BtnAccept->isEnabled())
		ui->BtnAccept->setEnabled(true);
}

//SliceFilter
void PipelineObjPropWidget::slot_update_planeOrigin(PipelineObject *tep_pipelineObj)
{
	emit sig_update_planeOrigin(tep_pipelineObj);
}

void PipelineObjPropWidget::slot_update_planeNormal(PipelineObject *tep_pipelineObj)
{
	emit sig_update_planeNormal(tep_pipelineObj);
}

void PipelineObjPropWidget::slot_update_planeVis(PipelineObject *tep_pipelineObj)
{
	emit sig_update_planeVis(tep_pipelineObj);
}
void PipelineObjPropWidget::slot_update_planeCameraNormal(PipelineObject *tep_pipelineObj)
{
	emit sig_update_planeCameraNormal(tep_pipelineObj);
}


void PipelineObjPropWidget::on_scalarBar_showBtn_clicked()
{
	if ((pipelineObj_propPtr == NULL) || (cur_pipelineObj == NULL))
		return;
	if (cur_pipelineObj->mVecActors.count() < 1)
		return;
	if (ui->m_cmb_ColumnList->currentIndex() == 0)
		return;
	//////////////////////////////////////////////////
	flag_render = true;
	Def_PipelineObjType tep_type=cur_pipelineObj->GetObjectType();
	QString tep_str;
	int index = ui->m_cmb_ColumnList->currentIndex();
	if (tep_type == dNumeric_DataSource)
	{
		if (pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible)
		{
			if (pipelineObj_propPtr->pipelineObj_base_propData.scalarBar_showMap.contains(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle))
				pipelineObj_propPtr->pipelineObj_base_propData.scalarBar_showMap.insert(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle, false);
			pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = false;
		}
		else //if (pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible)
		{			
			if ((index > 0))
			{
				tep_str = ui->m_cmb_ColumnList->currentText();
				if (ui->m_cmb_ColumnList->itemData(index).toString().indexOf("val_on_cell.png") >= 0)
					tep_str += "_cell000";
				if (!pipelineObj_propPtr->pipelineObj_base_propData.scalarBar_showMap[tep_str])
				{
					pipelineObj_propPtr->pipelineObj_base_propData.scalarBar_showMap.insert(tep_str, true);
				}
				pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = true;
			}
			else if (index == 0)
			{
				pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = false;
			}
		}
		emit sig_update_scalarBar(cur_pipelineObj);
		emit sig_python_scalarBar(cur_pipelineObj);
	}
	else if ((tep_type == dClip_DataSource) || (tep_type == dSlice_DataSource) || (tep_type == dContour_DataSource) || (tep_type == dVector_DataSource)
		|| (tep_type == dReflection_DataSource) || (tep_type == dCalculator_DataSource) || (tep_type == dSmooth_DataSource) || (tep_type == dStreamLine_DataSource))
	{
		PipelineObject* parent_obj = cur_pipelineObj->GetObjParent();
		if (cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
		{
			if ((tep_type == dCalculator_DataSource) && (pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle == pipelineObj_propPtr->calculatorFilter_propData.resultName))
			{
				if (parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
					parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("CalculatorResult", false);
				pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = false;
			}
			else
			{
				if (parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle))
					parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle, false);
				pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = false;
			}
			
		}
		else //if (cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
		{
			if ((tep_type == dCalculator_DataSource) && (pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle == pipelineObj_propPtr->calculatorFilter_propData.resultName))
			{
				if (parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains("CalculatorResult"))
				{
					parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert("CalculatorResult", true);
				}
				pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = true;
			}
			else
			{
				if (index > 0)
				{
					tep_str = ui->m_cmb_ColumnList->currentText();
					if (ui->m_cmb_ColumnList->itemData(index).toString().indexOf("val_on_cell.png") >= 0)
						tep_str += "_cell000";
					if (!parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[tep_str])
					{
						parent_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_str, true);
					}
					pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = true;
				}
				else if (index == 0)
				{
					pipelineObj_propPtr->pipelineObj_base_propData.mScalarBarVisible = false;
				}
			}
		}
		emit sig_update_scalarBar(parent_obj);
		emit sig_python_scalarBar(parent_obj);
	}
}

void PipelineObjPropWidget::on_colorEdit_Btn_clicked()
{
	if (pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle == "SolidColor")
	{
		QColor init_color0 = QColor(pipelineObj_propPtr->pipelineObj_base_propData.mSolidColor.r, pipelineObj_propPtr->pipelineObj_base_propData.mSolidColor.g, pipelineObj_propPtr->pipelineObj_base_propData.mSolidColor.b);

		QColor color = QColorDialog::getColor(init_color0, this, tr("color dialog"));
		if (!color.isValid())
			return;
		int r = color.red();
		int g = color.green();
		int b = color.blue();
		pipelineObj_propPtr->pipelineObj_base_propData.mSolidColor.r = r;
		pipelineObj_propPtr->pipelineObj_base_propData.mSolidColor.g = g;
		pipelineObj_propPtr->pipelineObj_base_propData.mSolidColor.b = b;
		ui->colorEdit_Btn->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
		emit sig_update_solidColor(cur_pipelineObj);
		emit sig_python_solidColor(cur_pipelineObj);
	}
	else
	{
		emit sig_load_scalarBarEditForm(cur_pipelineObj);
	}
}

renderview_global_prop PipelineObjPropWidget::get_renderView_prop()
{
	return cur_renderview_global_prop_data;
}

void PipelineObjPropWidget::set_renderView_prop_backgroundType(int val)
{
	cur_renderview_global_prop_data.back_color_type = val;
}

void PipelineObjPropWidget::set_renderView_prop_backgroundColor(QColor color)
{
	cur_renderview_global_prop_data.back_color1 = color;
}

void PipelineObjPropWidget::set_renderView_prop_background2Color(QColor color2)
{
	cur_renderview_global_prop_data.back_color2 = color2;
}

void PipelineObjPropWidget::set_renderView_prop_axesVisible(bool val)
{
	cur_renderview_global_prop_data.flag_axesVisible = val;
}
void PipelineObjPropWidget::set_renderView_prop_remarkVisible(bool val)
{
	cur_renderview_global_prop_data.remark_para.flag_remark = val;
}
void PipelineObjPropWidget::set_renderView_prop_remarkFontColor(QColor color)
{
	cur_renderview_global_prop_data.remark_para.fontColor = color;
}
void PipelineObjPropWidget::set_renderView_prop_remarkText(QString text)
{
	QString tep_str = text;
	QStringList tep_list = text.split("\\n", QString::SkipEmptyParts);
	if (tep_list.count() > 1)
	{
		tep_str = tep_list.at(0);
		for (int k = 1; k < tep_list.count(); k++)
		{
			tep_str +="\n";
			tep_str += tep_list.at(k);
		}
	}
	cur_renderview_global_prop_data.remark_para.text = tep_str;
}
void PipelineObjPropWidget::set_renderView_prop_remarkPosition1(double* pos)
{
	cur_renderview_global_prop_data.remark_para.position1[0] = pos[0];
	cur_renderview_global_prop_data.remark_para.position1[1] = pos[1];
}
void PipelineObjPropWidget::set_renderView_prop_remarkPosition2(double* pos)
{
	cur_renderview_global_prop_data.remark_para.position2[0] = pos[0];
	cur_renderview_global_prop_data.remark_para.position2[1] = pos[1];
}
void PipelineObjPropWidget::set_renderView_prop_cameraParallel(bool val)
{
	cur_renderview_global_prop_data.flag_camera_parallel = val;
}

void PipelineObjPropWidget::script_update_global_view_prop()//renderview_global_prop tep_prop
{
	//cur_renderview_global_prop_data.back_color_type = tep_prop.back_color_type;
	if ((cur_renderview_global_prop_data.back_color_type < 0) || (cur_renderview_global_prop_data.back_color_type>1))
		cur_renderview_global_prop_data.back_color_type = 0;
	//cur_renderview_global_prop_data.back_color1 = tep_prop.back_color1;
	//cur_renderview_global_prop_data.back_color2 = tep_prop.back_color1;
	//cur_renderview_global_prop_data.flag_axesVisible = tep_prop.flag_axesVisible;
	ui->orient_checkBox->setChecked(cur_renderview_global_prop_data.flag_axesVisible);
	ui->remark_checkBox->setChecked(cur_renderview_global_prop_data.remark_para.flag_remark);
	ui->remark_groupBox->setVisible(cur_renderview_global_prop_data.remark_para.flag_remark);
	if (cur_renderview_global_prop_data.remark_para.flag_remark)
	{
		ui->fontColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(cur_renderview_global_prop_data.remark_para.fontColor.red())
			.arg(cur_renderview_global_prop_data.remark_para.fontColor.green()).arg(cur_renderview_global_prop_data.remark_para.fontColor.blue()));
		ui->remark_textEdit->setText(cur_renderview_global_prop_data.remark_para.text);
	}
	ui->camera_parallel_checkBox->setChecked(cur_renderview_global_prop_data.flag_camera_parallel);
	ui->comboBox_backgroundType->setCurrentIndex(cur_renderview_global_prop_data.back_color_type);
	ui->color1_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(cur_renderview_global_prop_data.back_color1.red())
		.arg(cur_renderview_global_prop_data.back_color1.green()).arg(cur_renderview_global_prop_data.back_color1.blue()));
	ui->color2_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(cur_renderview_global_prop_data.back_color2.red())
		.arg(cur_renderview_global_prop_data.back_color2.green()).arg(cur_renderview_global_prop_data.back_color2.blue()));
}

//pipeline object properties toolbar
void PipelineObjPropWidget::toolBar_objProp_scalarBarAct()
{
	on_scalarBar_showBtn_clicked();
}

void PipelineObjPropWidget::toolBar_objProp_solidColorAct()
{
	on_colorEdit_Btn_clicked();
}

void PipelineObjPropWidget::toolBar_objProp_presentCmb_currentIndexChanged(int tep_index)
{
	if (tep_index == -1)
		return;
	ui->m_cmb_Representation->setCurrentIndex(tep_index);
}

void PipelineObjPropWidget::toolBar_objProp_columnCmb_currentIndexChanged(int tep_index)
{
	if (tep_index == -1)
		return;
	ui->m_cmb_ColumnList->setCurrentIndex(tep_index);
}
void PipelineObjPropWidget::toolBar_objProp_GlyphVector_currentIndexChanged(int tep_index)
{
	if (tep_index == -1)
		return;
	ui->vectorColorXYZ_comboBox->setCurrentIndex(tep_index);
}
//pipeline object properties toolbar

void PipelineObjPropWidget::on_edgeColor_pushButton_clicked()
{
	QColor init_color0 = QColor(pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.r, pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.g, pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.b);

	QColor color = QColorDialog::getColor(init_color0, this, tr("color dialog"));
	if (!color.isValid())
		return;
	int r = color.red();
	int g = color.green();
	int b = color.blue();
	pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.r = r;
	pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.g = g;
	pipelineObj_propPtr->pipelineObj_base_propData.mEdgeColor.b = b;
	ui->edgeColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	emit sig_update_edgeColor(cur_pipelineObj);
	emit sig_python_edgeColor(cur_pipelineObj);
}

void PipelineObjPropWidget::on_camera_parallel_checkBox_clicked(bool checked)
{
	cur_renderview_global_prop_data.flag_camera_parallel = checked;
	emit sig_update_camera_parallel(checked);
	emit sig_python_view_cameraParallel(checked);
}

void PipelineObjPropWidget::reTranslate()
{
	int tep_rep = 0;
	int tep_interpolation = 0;
	if ((cur_pipelineObj != NULL) && (pipelineObj_propPtr != NULL))
	{
		tep_rep = pipelineObj_propPtr->pipelineObj_base_propData.m_enum_representationtype;
		tep_interpolation = pipelineObj_propPtr->pipelineObj_base_propData.m_enum_interpolationtype;
	}
	ui->retranslateUi(this);
	ui->BtnPropertiesButton->setText(tr("Properties"));
	ui->BtnDisplayButton->setText(tr("Display"));
	if ((cur_pipelineObj == NULL) || (pipelineObj_propPtr == NULL))
		return;
	if (cur_pipelineObj->GetObjectType() == dNumeric_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName));
	else if (cur_pipelineObj->GetObjectType() == dClip_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->plane_propData.filterName));
	else if (cur_pipelineObj->GetObjectType() == dSlice_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->plane_propData.filterName));
	else if (cur_pipelineObj->GetObjectType() == dContour_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->contourFilter_propData.filterName));
	else if (cur_pipelineObj->GetObjectType() == dVector_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->vectorFilter_propData.filterName));
	else if (cur_pipelineObj->GetObjectType() == dReflection_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->reflectionFilter_propData.filterName));
	else if (cur_pipelineObj->GetObjectType() == dCalculator_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->calculatorFilter_propData.filterName));
	else if (cur_pipelineObj->GetObjectType() == dSmooth_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->smoothFilter_propData.filterName));
	else if (cur_pipelineObj->GetObjectType() == dStreamLine_DataSource)
		ui->BtnPropertiesButton->setText(QString(tr("Properties(%1 <%2>)")).arg(pipelineObj_propPtr->pipelineObj_base_propData.fileName).arg(pipelineObj_propPtr->streamLineFilter_propData.filterName));
	pipelineObj_propPtr->pipelineObj_base_propData.m_enum_representationtype = (ERepresentationType)tep_rep;
	pipelineObj_propPtr->pipelineObj_base_propData.m_enum_interpolationtype = (EInterpolationType)tep_interpolation;
	ui->m_cmb_Representation->setCurrentIndex(pipelineObj_propPtr->pipelineObj_base_propData.m_enum_representationtype);
	ui->m_cmb_interpolation->setCurrentIndex(pipelineObj_propPtr->pipelineObj_base_propData.m_enum_interpolationtype);

	if ((cur_addWidget != NULL) && (cur_pipelineObj != NULL))
	{
		if ((cur_pipelineObj->GetObjectType() == dClip_DataSource) || (cur_pipelineObj->GetObjectType() == dSlice_DataSource))
			((sliceFilterPropForm*)cur_addWidget)->reTranslate();
		else if (cur_pipelineObj->GetObjectType() == dReflection_DataSource)
			((refelctionFilterPropForm*)cur_addWidget)->reTranslate();
		else if (cur_pipelineObj->GetObjectType() == dCalculator_DataSource)
			((calFilterPropForm*)cur_addWidget)->reTranslate();
		else if (cur_pipelineObj->GetObjectType() == dVector_DataSource)//
			((vectorFilterPropForm*)cur_addWidget)->reTranslate();
		else if (cur_pipelineObj->GetObjectType() == dStreamLine_DataSource)//
			((streamLineFilterPropForm*)cur_addWidget)->reTranslate();
		else if (cur_pipelineObj->GetObjectType() == dSmooth_DataSource)
			((smoothFilterPropForm*)cur_addWidget)->reTranslate();
	}
}
void PipelineObjPropWidget::on_cubeAxes_checkBox_clicked(bool checked)
{
	pipelineObj_propPtr->pipelineObj_base_propData.flag_cubeAxes = checked;
	emit sig_update_cubeAxes(cur_pipelineObj);
	emit sig_python_cubeAxes(cur_pipelineObj);
}
//void sig_python_renderView();


void PipelineObjPropWidget::on_vectorColorXYZ_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if (!ui->vectorColorXYZ_comboBox->isEnabled())
		return;
	if (cur_pipelineObj == NULL)
		return;
	if (cur_pipelineObj->GetObjectType() == dVector_DataSource)
	{
		if (pipelineObj_propPtr->vectorFilter_propData.vectorColName == "None")
			return;
		pipelineObj_propPtr->vectorFilter_propData.glyphVector_colorXYZ = ui->vectorColorXYZ_comboBox->currentText();
		emit sig_update_glyphVector(cur_pipelineObj);
	}
	else
	{
		pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle_XYZ = ui->vectorColorXYZ_comboBox->currentText();
		cur_pipelineObj->func_update_componentXYX_lookupTableMap(pipelineObj_propPtr->pipelineObj_base_propData.mColorColumnStyle);
		emit sig_update_glyphVector(cur_pipelineObj);
		emit sig_python_color_component(cur_pipelineObj);
	}
	emit sig_update_toolBar_objProp_column_GlyphVectorXYZ(index);
}

void PipelineObjPropWidget::on_remark_checkBox_clicked(bool checked)
{
	cur_renderview_global_prop_data.remark_para.flag_remark = checked;
	ui->remark_groupBox->setVisible(checked);
	ui->fontColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(cur_renderview_global_prop_data.remark_para.fontColor.red())
		.arg(cur_renderview_global_prop_data.remark_para.fontColor.green())
		.arg(cur_renderview_global_prop_data.remark_para.fontColor.blue()));
	ui->remark_textEdit->setText(cur_renderview_global_prop_data.remark_para.text);
	emit sig_update_remark_visible(checked);
	emit sig_python_view_remarkVisible(checked);
}

void PipelineObjPropWidget::on_fontColor_pushButton_clicked()
{
	QColor init_color0 = cur_renderview_global_prop_data.remark_para.fontColor;
	QColor color = QColorDialog::getColor(init_color0, this, tr("color dialog"));
	if (!color.isValid())
		return;
	cur_renderview_global_prop_data.remark_para.fontColor = color;
	int r = color.red();
	int g = color.green();
	int b = color.blue();
	ui->fontColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	emit sig_update_remark_fontColor();
	emit sig_python_view_remarkFontColor(color);
}

void PipelineObjPropWidget::on_remark_textEdit_textChanged()
{
	cur_renderview_global_prop_data.remark_para.text = ui->remark_textEdit->toPlainText();
	emit sig_update_remark_text();
	emit sig_python_view_remarkText(cur_renderview_global_prop_data.remark_para.text);

}
