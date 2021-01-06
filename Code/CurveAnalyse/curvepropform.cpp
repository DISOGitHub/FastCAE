#include "curvepropform.h"
#include "ui_curvepropform.h"

CurvePropForm::CurvePropForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurvePropForm)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();
    curve_data_ptr=NULL;
    cur_index_curve=-1;
    cur_index_file=-1;
}

CurvePropForm::~CurvePropForm()
{
    delete ui;
}

void CurvePropForm::initForm(int index, int file_index, int curve_index)
{
	gloItemList.clear();
	if (index == 0)
	{
		ui->tabWidget->setCurrentIndex(index);
		initGlobalPropTreeWidget();
	}
	else if (index == 1)
	{
		ui->tabWidget->setCurrentIndex(index);
		initCurvePropTreeWidget(file_index,curve_index);
		tep_file_data = curve_data_ptr->curve_file_list.at(file_index);
		tep_line_data = tep_file_data.line_data.at(curve_index);
	}
}

void CurvePropForm::on_backColor_pushButton_clicked()
{
	QColor init_color0 = curve_data_ptr->glo_curve_data.backColor;
	QColor tep_color = QColorDialog::getColor(init_color0, this, tr("Select Color Dialog"));
	if (tep_color.isValid())
	{
		curve_data_ptr->glo_curve_data.backColor = tep_color;
		int r = curve_data_ptr->glo_curve_data.backColor.red();
		int g = curve_data_ptr->glo_curve_data.backColor.green();
		int b = curve_data_ptr->glo_curve_data.backColor.blue();
		backColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
		QString cmd_str = QString("plot> back_color=rgb(%1 %2 %3)\n").arg(r).arg(g).arg(b);
		emit sig_setBackgroundColor();
	}
}

void CurvePropForm::on_grid_checkBox_clicked(bool checked)
{
	curve_data_ptr->glo_curve_data.flag_grid = checked;
	emit sig_setGrid();
}

void CurvePropForm::on_legend_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	curve_data_ptr->glo_curve_data.flag_legend = (plotProp_legendPostion)index;
	emit sig_setLegend();
}

void CurvePropForm::initGlobalPropTreeWidget1()
{
	backColor_pushButton = new QPushButton;
	backColor_pushButton->setText(tr("select color"));
	QColor tep_color = curve_data_ptr->glo_curve_data.backColor;
	backColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue()));
	gloProp_background_Item = new QTreeWidgetItem;
	gloProp_background_Item->setText(0, tr("background color"));
	ui->gloProp_treeWidget->addTopLevelItem(gloProp_background_Item);//gloItemList.append(gloProp_background_Item);
	ui->gloProp_treeWidget->setItemWidget(gloProp_background_Item, 1, backColor_pushButton);
	connect(backColor_pushButton, SIGNAL(clicked()), this, SLOT(on_backColor_pushButton_clicked()));
	grid_checkBox = new QCheckBox;
	grid_checkBox->setChecked(curve_data_ptr->glo_curve_data.flag_grid);
	gloProp_grid_Item = new QTreeWidgetItem;
	gloProp_grid_Item->setText(0, tr("grid"));
	ui->gloProp_treeWidget->addTopLevelItem(gloProp_grid_Item);//gloItemList.append(gloProp_grid_Item);
	ui->gloProp_treeWidget->setItemWidget(gloProp_grid_Item, 1, grid_checkBox);
	connect(grid_checkBox, SIGNAL(clicked(bool)), this, SLOT(on_grid_checkBox_clicked(bool)));
	legend_comboBox = new QComboBox;
	QStringList tep_legendList; tep_legendList <<tr( "none") << tr("right_top") << tr("right_bottom") << tr("left_top") << tr("left_bottom");
	legend_comboBox->addItems(tep_legendList);
	legend_comboBox->setCurrentIndex(curve_data_ptr->glo_curve_data.flag_legend);
	gloProp_legend_Item = new QTreeWidgetItem;
	gloProp_legend_Item->setText(0, tr("legend"));
	ui->gloProp_treeWidget->addTopLevelItem(gloProp_legend_Item);//gloItemList.append(gloProp_legend_Item);
	ui->gloProp_treeWidget->setItemWidget(gloProp_legend_Item, 1, legend_comboBox);
	connect(legend_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_legend_comboBox_currentIndexChanged(int)));
	/////////
	numAxis_comboBox = new QComboBox;
	QStringList tep_numAxisList; tep_numAxisList << "1" << "2";
	numAxis_comboBox->addItems(tep_numAxisList);
	numAxis_comboBox->setCurrentIndex(curve_data_ptr->glo_curve_data.num_axis - 1);
	gloProp_numAxis_Item = new QTreeWidgetItem;
	gloProp_numAxis_Item->setText(0, tr("num Axis"));
	ui->gloProp_treeWidget->addTopLevelItem(gloProp_numAxis_Item);//gloItemList.append(gloProp_legend_Item);
	ui->gloProp_treeWidget->setItemWidget(gloProp_numAxis_Item, 1, numAxis_comboBox);
	connect(numAxis_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_numAxis_comboBox_currentIndexChanged(int)));
}

void CurvePropForm::initGlobalPropTreeWidget()
{
	ui->gloProp_treeWidget->setColumnCount(2);
	ui->gloProp_treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

	gloProp_headerItem = ui->gloProp_treeWidget->headerItem();
	gloProp_headerItem->setText(0, tr("plot property"));
	gloProp_headerItem->setText(1, tr("value"));
	initGlobalPropTreeWidget1();

	ui->gloProp_treeWidget->addTopLevelItems(gloItemList);
}

void CurvePropForm::on_numAxis_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	curve_data_ptr->glo_curve_data.num_axis = index + 1;
	emit sig_setAxisNum();
}

void CurvePropForm::initCurvePropTreeWidget(int tep_file_index, int tep_curve_index)
{
	ui->treeWidget->setColumnCount(2);
	ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

	headerItem = ui->treeWidget->headerItem();
	headerItem->setText(0, tr("curve property"));
	headerItem->setText(1, tr("value"));

	QList<QTreeWidgetItem*> list;
	list.clear();

	name_y_lineEdit = new QLineEdit;
	name_y_lineEdit->setStyleSheet("QLineEdit{border-width:1;border-style:outset}");
	name_y_lineEdit->setText(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_name);
	name_yItem = new QTreeWidgetItem;
	name_yItem->setText(0,tr("curve name"));
	//y_PropertyItem->addChild(name_yItem);
	ui->treeWidget->addTopLevelItem(name_yItem);
	ui->treeWidget->setItemWidget(name_yItem, 1, name_y_lineEdit);
	QObject::connect(name_y_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(on_name_y_lineEdit_textChanged(QString)));
	//list.append(name_yItem);

	basePropertyItem = new QTreeWidgetItem;
	basePropertyItem->setText(0, tr("base property"));
	color_pushButton = new QPushButton;
	color_pushButton->setText(tr("select color"));
	QColor tep_color = curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_color;
	color_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue()));
	color_baseItem = new QTreeWidgetItem;
	color_baseItem->setText(0, tr("curve color"));
	basePropertyItem->addChild(color_baseItem);
	ui->treeWidget->setItemWidget(color_baseItem, 1, color_pushButton);
	connect(color_pushButton, SIGNAL(clicked()), this, SLOT(on_color_pushButton_clicked()));

	lineType_comboBox = new QComboBox;
	lineType_comboBox->clear();
	QStringList tep_typelist; tep_typelist << tr("solid") << tr("dash") << tr("dot") << tr("dash dot");
	lineType_comboBox->addItems(tep_typelist);
	QObject::connect(lineType_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_lineType_comboBox_currentIndexChanged(int)));
	lineType_comboBox->blockSignals(true);
	lineType_comboBox->setCurrentIndex(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_lineType - 1);
	lineType_comboBox->blockSignals(false);
	lineType_baseItem = new QTreeWidgetItem;
	lineType_baseItem->setText(0, tr("line type"));
	basePropertyItem->addChild(lineType_baseItem);
	ui->treeWidget->setItemWidget(lineType_baseItem, 1, lineType_comboBox);
	
	lineWidth_spinBox = new QSpinBox;
	lineWidth_spinBox->setRange(1, 10);
	QObject::connect(lineWidth_spinBox, SIGNAL(valueChanged(int)), this, SLOT(on_lineWidth_spinBox_valueChanged(int)));
	lineWidth_spinBox->blockSignals(true);
	lineWidth_spinBox->setValue(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_lineWidth);
	lineWidth_spinBox->blockSignals(false);
	lineWidth_baseItem = new QTreeWidgetItem;
	lineWidth_baseItem->setText(0, tr("line width"));
	basePropertyItem->addChild(lineWidth_baseItem);
	ui->treeWidget->setItemWidget(lineWidth_baseItem, 1, lineWidth_spinBox);

	flagPoint_checkBox = new QCheckBox;
	flagPoint_checkBox->blockSignals(true);
	flagPoint_checkBox->setChecked(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).flag_showPoint);
	flagPoint_checkBox->blockSignals(false);
	flagPoint_baseItem = new QTreeWidgetItem;
	flagPoint_baseItem->setText(0, tr("show point"));
	basePropertyItem->addChild(flagPoint_baseItem);
	ui->treeWidget->setItemWidget(flagPoint_baseItem, 1, flagPoint_checkBox);
	QObject::connect(flagPoint_checkBox, SIGNAL(stateChanged(int)), this, SLOT(on_flagPoint_checkBox_stateChanged(int)));
	xAxis_indexSelected_Item = new QTreeWidgetItem;
	xAxis_indexSelected_Item->setText(0, tr("axis select"));
	x_indexSelected_comboBox = new QComboBox;
	x_indexSelected_comboBox->addItem(tr("xAxis1"));
	if (curve_data_ptr->glo_curve_data.num_axis==2)
		x_indexSelected_comboBox->addItem(tr("xAxis2"));
	basePropertyItem->addChild(xAxis_indexSelected_Item);
	ui->treeWidget->setItemWidget(xAxis_indexSelected_Item, 1, x_indexSelected_comboBox);
	QObject::connect(x_indexSelected_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_x_indexSelected_comboBox_currentIndexChanged(int)));
	x_indexSelected_comboBox->blockSignals(true);
	if (curve_data_ptr->glo_curve_data.num_axis == 1)
		x_indexSelected_comboBox->setCurrentIndex(0);
	else if (curve_data_ptr->glo_curve_data.num_axis == 2)
		x_indexSelected_comboBox->setCurrentIndex(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).axis_index);
	x_indexSelected_comboBox->blockSignals(false);
	list.append(basePropertyItem);
	//    QObject::connect(smoothCoef_lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(on_smoothCoef_lineEdit_textChanged(const QString &)));
	y_PropertyItem = new QTreeWidgetItem;
	y_PropertyItem->setText(0, tr("data resource"));
	col_name_y_lineEdit = new QLineEdit;
	col_name_y_lineEdit->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
	col_name_y_lineEdit->setText(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_name);
	col_name_yItem = new QTreeWidgetItem;
	col_name_yItem->setText(0, tr("column name"));
	col_name_y_lineEdit->setReadOnly(true);
	y_PropertyItem->addChild(col_name_yItem);
	ui->treeWidget->setItemWidget(col_name_yItem, 1, col_name_y_lineEdit);
	// QObject::connect(col_name_y_lineEdit, SIGNAL(editingFinished()), this, SLOT(on_col_name_y_lineEdit_editingFinished()));
	column_y_lineEdit = new QLineEdit;
	column_y_lineEdit->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
	QString tepstr;
	column_y_lineEdit->setText(tepstr.setNum(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_column));
	column_y_lineEdit->setReadOnly(true);
	column_yItem = new QTreeWidgetItem;
	column_yItem->setText(0, tr("column number"));
	y_PropertyItem->addChild(column_yItem);
	ui->treeWidget->setItemWidget(column_yItem, 1, column_y_lineEdit);
	list.append(y_PropertyItem);

	///////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	aniPointItem = new QTreeWidgetItem;
	aniPointItem->setText(0, tr("animate point"));
	color_aniPointButton = new QPushButton;
	color_aniPointButton->setText(tr("select color"));
	QColor tep_color1 = curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).aniPoint_color;
	color_aniPointButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(tep_color1.red()).arg(tep_color1.green()).arg(tep_color1.blue()));
	aniPointColorItem = new QTreeWidgetItem;
	aniPointColorItem->setText(0, tr("color"));
	aniPointItem->addChild(aniPointColorItem);
	ui->treeWidget->setItemWidget(aniPointColorItem, 1, color_aniPointButton);
	connect(color_aniPointButton, SIGNAL(clicked()), this, SLOT(on_color_aniPointButton_clicked()));
	aniPointType_comboBox = new QComboBox;
	aniPointType_comboBox->blockSignals(true);
	QStringList pointTypeList;
	pointTypeList.clear();
	pointTypeList << tr("dot") << tr("cross") << tr("plus") << tr("circle") << tr("disc") << tr("square") << tr("diamond") << tr("star") << tr("triangle");
	aniPointType_comboBox->addItems(pointTypeList);
	aniPointType_comboBox->setCurrentIndex(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).aniPoint_type);
	aniPointType_comboBox->blockSignals(false);
	aniPointTypeItem = new QTreeWidgetItem;
	aniPointTypeItem->setText(0, tr("type"));
	aniPointItem->addChild(aniPointTypeItem);
	ui->treeWidget->setItemWidget(aniPointTypeItem, 1, aniPointType_comboBox);
	QObject::connect(aniPointType_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_aniPointType_comboBox_currentIndexChanged(int)));
	/*lineWidth_spinBox = new QSpinBox;
	lineWidth_spinBox->setRange(1, 10);
	QObject::connect(lineWidth_spinBox, SIGNAL(valueChanged(int)), this, SLOT(on_lineWidth_spinBox_valueChanged(int)));
	lineWidth_spinBox->blockSignals(true);
	lineWidth_spinBox->setValue(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_lineWidth);
	lineWidth_spinBox->blockSignals(false);
	lineWidth_baseItem = new QTreeWidgetItem;
	lineWidth_baseItem->setText(0, tr("line width"));
	basePropertyItem->addChild(lineWidth_baseItem);
	ui->treeWidget->setItemWidget(lineWidth_baseItem, 1, lineWidth_spinBox);*/
	list.append(aniPointItem);
	/////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	ui->treeWidget->addTopLevelItems(list);
	ui->treeWidget->expandItem(basePropertyItem);
	ui->treeWidget->expandItem(y_PropertyItem);
	ui->treeWidget->expandItem(aniPointItem);
	cur_index_file = tep_file_index;
	cur_index_curve = tep_curve_index;
}

void CurvePropForm::on_name_y_lineEdit_textChanged(QString text)
{
	tep_line_data.yAxis_name = text;
	tep_file_data.line_data.replace(cur_index_curve, tep_line_data);
	curve_data_ptr->curve_file_list.replace(cur_index_file, tep_file_data);
	emit sig_update_tree_curve_name(cur_index_file, cur_index_curve, text);
	emit sig_update_curve_name(cur_index_file, cur_index_curve);

}

void CurvePropForm::on_color_pushButton_clicked()
{
	QColor init_color0 = tep_line_data.curve_color;
	QColor tep_color = QColorDialog::getColor(init_color0, this, tr("select color dialog"));
	if (!tep_color.isValid())
		return;
	tep_line_data.curve_color = tep_color;
	int r = tep_line_data.curve_color.red();
	int g = tep_line_data.curve_color.green();
	int b = tep_line_data.curve_color.blue();
	color_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	tep_file_data.line_data.replace(cur_index_curve, tep_line_data);
	curve_data_ptr->curve_file_list.replace(cur_index_file, tep_file_data);
	emit sig_update_curve_pen(cur_index_file, cur_index_curve);
}

void CurvePropForm::on_lineType_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	tep_line_data.curve_lineType = Qt::PenStyle(index + 1);
	tep_file_data.line_data.replace(cur_index_curve, tep_line_data);
	curve_data_ptr->curve_file_list.replace(cur_index_file, tep_file_data);
	emit sig_update_curve_pen(cur_index_file, cur_index_curve);
}

void CurvePropForm::on_lineWidth_spinBox_valueChanged(int index)
{
	tep_line_data.curve_lineWidth = index;
	tep_file_data.line_data.replace(cur_index_curve, tep_line_data);
	curve_data_ptr->curve_file_list.replace(cur_index_file, tep_file_data);
	emit sig_update_curve_pen(cur_index_file, cur_index_curve);
}

void CurvePropForm::on_flagPoint_checkBox_stateChanged(int state)
{
	tep_line_data.flag_showPoint = flagPoint_checkBox->isChecked();
	int val;
	if (tep_line_data.flag_showPoint)
		val = 1;
	else
		val = 0;
	tep_file_data.line_data.replace(cur_index_curve, tep_line_data);
	curve_data_ptr->curve_file_list.replace(cur_index_file, tep_file_data);
	emit sig_update_curve_linePoint(cur_index_file, cur_index_curve);
}

void CurvePropForm::on_x_indexSelected_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	tep_line_data.axis_index = index;
	tep_file_data.line_data.replace(cur_index_curve, tep_line_data);
	curve_data_ptr->curve_file_list.replace(cur_index_file, tep_file_data);
	emit sig_update_curve_axis(cur_index_file, cur_index_curve);
}

void CurvePropForm::reTranslate()
{
	ui->retranslateUi(this);
}

void CurvePropForm::on_aniPointType_comboBox_currentIndexChanged(int state)
{
	if (state == -1)
		return;
	tep_line_data.aniPoint_type =(aniPointType) aniPointType_comboBox->currentIndex();
	tep_file_data.line_data.replace(cur_index_curve, tep_line_data);
	curve_data_ptr->curve_file_list.replace(cur_index_file, tep_file_data);
	//emit sig_update_curve_linePoint(cur_index_file, cur_index_curve);
}
void CurvePropForm::on_color_aniPointButton_clicked()
{
	QColor init_color0 = tep_line_data.aniPoint_color;
	QColor tep_color = QColorDialog::getColor(init_color0, this, tr("select color dialog"));
	if (!tep_color.isValid())
		return;
	tep_line_data.aniPoint_color = tep_color;
	int r = tep_line_data.aniPoint_color.red();
	int g = tep_line_data.aniPoint_color.green();
	int b = tep_line_data.aniPoint_color.blue();
	color_aniPointButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	tep_file_data.line_data.replace(cur_index_curve, tep_line_data);
	curve_data_ptr->curve_file_list.replace(cur_index_file, tep_file_data);
	//emit sig_update_curve_pen(cur_index_file, cur_index_curve);
}





