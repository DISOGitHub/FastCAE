#include "curvetreedockwidget.h"
#include "curvepropform.h"
CurveTreeDockWidget::CurveTreeDockWidget(QWidget *parent) : QDockWidget(parent)
{
    red0=0;
    green0=0;
    blue0=0;
    add_count=0;
	index_curve = -1;
	index_file = -1;
    curve_data_ptr=NULL;
    cur_item=NULL;
    file_add_action=NULL;
	setStyleSheet("QWidget{background-color: #eaf2f8;}""QDockWidget::title{border-radius: 6px; background-image: url(:/images/project_title.png);}""QMenu{color:black;}""QMenu:hover{color:black;}");
	this->setWindowTitle(tr("browser"));
	initPopMenu();
    initWidget();
}

CurveTreeDockWidget::~CurveTreeDockWidget()
{

}

void CurveTreeDockWidget::initPopMenu()
{
//	curve_add_popMenu = new QMenu(this);//定义一个右键弹出菜单
//	curve_add_action = new QAction(tr("add curve"), this);
//	curve_add_action->setStatusTip(tr("add a curve"));
//	curve_add_popMenu->addAction(curve_add_action);
//    curve_del_popMenu = new QMenu(this);
//    curve_del_action = new QAction(tr("del curve"), this);
//    curve_del_action->setStatusTip(tr("del a curve"));
//	curve_del_popMenu->addAction(curve_del_action);

    file_add_popMenu = new QMenu(this);
    file_add_action = new QAction(tr("add file"), this);
    file_add_action->setStatusTip(tr("add a file"));
    file_add_popMenu->addAction(file_add_action);
    
    ///
    file_del_popMenu = new QMenu(this);
    file_del_action = new QAction(tr("del file"), this);
    file_del_action->setStatusTip(tr("delete a file"));
    file_del_popMenu->addAction(file_del_action);
    connect(file_add_action, SIGNAL(triggered()), this, SLOT(slot_addFile()));
    connect(file_del_action, SIGNAL(triggered()), this, SLOT(slot_delFile()));
}

void CurveTreeDockWidget::initWidget()
{
	curveTree = new CurveDropTreeWidget(this);
	curveTree->setHeaderHidden(true);
//	curveTree->setColumnCount(5);
//	curveTree->resizeColumnToContents(0);否
//	curveTree->setSizeAdjustPolicy(QTreeWidget::AdjustToContents);
//	curveTree->adjustSize();
//	curveTree->setSizeAdjustPolicy(QTreeWidget::AdjustToContentsOnFirstShow);
//	curveTree->column
	/*
	curveTree->setColumnWidth(0, 260);
	curveTree->setColumnWidth(1, 10);
	curveTree->setColumnWidth(2, 42);
	curveTree->setColumnWidth(3, 10);
	curveTree->setColumnWidth(4, 42);
    */
	curveTree->setContextMenuPolicy(Qt::CustomContextMenu); 

    curveTree->setItemsExpandable(true);
    curveTree->expandAll();
    connect(curveTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(projectTreeItemClicked(QTreeWidgetItem*,int)));
    connect(curveTree, SIGNAL(sig_add_dropItem(QString)), this, SLOT(slot_add_dropItem(QString)));
	//connect(curveTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(projectTreeDoubleClicked(QTreeWidgetItem*,int)));
    connect(curveTree,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_treeWidget_customContextMenuRequested(QPoint)));
    //this->setMaximumWidth(Min_Width);
	//this->setMaximumWidth(Min_Width);
	//this->setBaseSize(Min_Width,500);
	//this->setMinimumHeight(500);
    setWidget(curveTree);
	initTree();
}

void CurveTreeDockWidget::slot_add_dropItem(QString filename1)
{
    if(curve_data_ptr==NULL)
      return;
    QFile tep_file(filename1);
    if(tep_file.exists())
    {
		readDataFile(filename1);
    }
}

void CurveTreeDockWidget::initTree()
{
	curveTree->clear();
	curve_project_item = new QTreeWidgetItem(curveTree);
	curve_project_item->setText(0, tr("curve_project"));
	curve_project_item->setIcon(0, QIcon(":/images/curve.png"));
	curve_project_item->setData(0, Qt::UserRole + 1, "curve_project");
    curveTree->setCurrentItem(curve_project_item);
    if(file_add_action!=NULL)
        file_add_action->setEnabled(true);
}

bool CurveTreeDockWidget::curveExist(int tep_index_file,int tep_index_col)
{
    if(curve_data_ptr==NULL)
    {
        return false;
    }
	QString tep_curve_id = QString("curve_%1_%2").arg(tep_index_file).arg(tep_index_col);
	if (curve_data_ptr->curveList.contains(tep_curve_id))
		return true;
    return false;
}

void CurveTreeDockWidget::slot_update_curveTree()
{
    if(curve_data_ptr==NULL)
        return;
    QString tepstr;
    for(int i=0;i<curve_data_ptr->curve_file_list.count();i++)
    {
		QString data_filename = curve_data_ptr->curve_file_list.at(i).file_prop.Axis_filename;
        QTreeWidgetItem *tep_file_item=new QTreeWidgetItem(curve_project_item);
		//////////////////////////////////////////////
		if (data_filename.size() > 26)
		{
			if (data_filename.size() > 52)
				data_filename.insert(52, "\n");
			data_filename.insert(26, "\n");
		}
		tep_file_item->setText(0, data_filename);
        tep_file_item->setData(0, Qt::UserRole + 1, "curve_file");
        for(int j=0;j<curve_data_ptr->curve_file_list.at(i).line_data.count();j++)
        {
            QTreeWidgetItem *tep_curve_item=new QTreeWidgetItem(tep_file_item);
            tep_curve_item->setText(0,curve_data_ptr->curve_file_list.at(i).line_data.at(j).yAxis_name);
            tepstr=QString("curve_%1_%2").arg(i).arg(curve_data_ptr->curve_file_list.at(i).line_data.at(j).yAxis_column);
            tep_curve_item->setData(0, Qt::UserRole + 1,tepstr);
        }
    }
    curveTree->setCurrentItem(curve_project_item);
    //20170314 del projectTreeItemClicked(curve_project_item,0);
    emit sig_update_clear_console_command();
    emit sig_update_clear_console_data();
}

void CurveTreeDockWidget::slot_update_tree_curve_name(int tep_index_file,int tep_index_curve,QString tep_item_name)
{
    if(curve_data_ptr==NULL)
        return;
    if(tep_index_file<0)
        return;
    if(tep_index_file>=curve_data_ptr->curve_file_list.count())
        return;
    if((tep_index_curve<0)||(tep_index_curve>=curve_data_ptr->curve_file_list.at(tep_index_file).line_data.count()))
        return;
    if(tep_index_file>=curve_project_item->childCount())
        return;
    QTreeWidgetItem *curve_parent_item=curve_project_item->child(tep_index_file);
    if(tep_index_curve>=curve_parent_item->childCount())
        return;
    QTreeWidgetItem *tep_curve_item=curve_parent_item->child(tep_index_curve);
    tep_curve_item->setText(0,tep_item_name);
}

void CurveTreeDockWidget::setAxisRange(int axis_index,int tep_file_index,int tep_column_index)
{
	double tep_ymin, tep_ymax;
	if ((axis_index < 0) || (axis_index>1))
		return;
	if (curve_data_ptr == NULL)
		return;
	if (tep_file_index >= curve_data_ptr->curve_file_list.count())
		return;
	if (tep_column_index >= curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	global_plot_axis_prop* y_axis_prop_ptr=NULL;
	global_plot_axis_prop* x_axis_prop_ptr = NULL;
	tep_ymin = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.val_min[tep_column_index];
	tep_ymax = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.val_max[tep_column_index];
	y_axis_prop_ptr = &(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[axis_index]);
	x_axis_prop_ptr = &(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[axis_index]);
	if (curve_data_ptr->curveList.count() == 0)
	{
		y_axis_prop_ptr->axis_range[0] = tep_ymin;
		y_axis_prop_ptr->axis_range[1] = tep_ymax;
		if (x_axis_prop_ptr->axis_dataSource == plotProp_dataSource::File)
		{	
			int tep_xAxis_file_index = x_axis_prop_ptr->axis_fileIndex;
			int tep_xAxis_file_column = x_axis_prop_ptr->axis_fileColumn;
			if (tep_xAxis_file_index >= curve_data_ptr->curve_file_list.count())
				return;
			if (tep_xAxis_file_column >= curve_data_ptr->curve_file_list.at(tep_xAxis_file_index).file_prop.num_column)
				return;
			x_axis_prop_ptr->axis_range[0] = curve_data_ptr->curve_file_list.at(tep_xAxis_file_index).file_prop.val_min[tep_xAxis_file_column];
			x_axis_prop_ptr->axis_range[1] = curve_data_ptr->curve_file_list.at(tep_xAxis_file_index).file_prop.val_max[tep_xAxis_file_column];
		}
		else
		{
			x_axis_prop_ptr->axis_range[0] = 0;
			x_axis_prop_ptr->axis_range[1] = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_point;
		}
	}
	else
	{
		int num_files = curve_data_ptr->curve_file_list.count();
		int real_num_lines = 0;
		for (int i = 0; i < num_files; i++)
		{
			int num_lines = curve_data_ptr->curve_file_list.at(i).line_data.count();
			for (int j = 0; j < num_lines; j++)
			{
				if (curve_data_ptr->curve_file_list.at(i).line_data.at(j).axis_index == axis_index)
				{
					real_num_lines++;
					break;
				}
			}
			if (real_num_lines > 0)
				break;
		}
		if (real_num_lines > 0)
		{
			if (tep_ymin < y_axis_prop_ptr->axis_range[0])
				y_axis_prop_ptr->axis_range[0] = tep_ymin;
			if (tep_ymax > y_axis_prop_ptr->axis_range[1])
				y_axis_prop_ptr->axis_range[1] = tep_ymax;
			/*if (x_axis_prop_ptr->axis_dataSource == plotProp_dataSource::file)
			{
				int tep_xAxis_file_index = x_axis_prop_ptr->axis_fileIndex;
				int tep_xAxis_file_column = x_axis_prop_ptr->axis_fileColumn;
				if (tep_xAxis_file_index >= curve_data_ptr->curve_file_list.count())
					return;
				if (tep_xAxis_file_column >= curve_data_ptr->curve_file_list.at(tep_xAxis_file_index).file_prop.num_column)
					return;
				if (curve_data_ptr->curve_file_list.at(tep_xAxis_file_index).file_prop.num_point)
				tep_xmin = curve_data_ptr->curve_file_list.at(tep_xAxis_file_index).file_prop.val_min[tep_xAxis_file_column];
				tep_xmax = curve_data_ptr->curve_file_list.at(tep_xAxis_file_index).file_prop.val_max[tep_xAxis_file_column];
				if (xmin < x_axis_prop_ptr->axis_range[0])
					x_axis_prop_ptr->axis_range[0] = xmin;
				if (xmax > x_axis_prop_ptr->axis_range[1])
					x_axis_prop_ptr->axis_range[1] = xmax;				
			}
			else*/
			if (x_axis_prop_ptr->axis_dataSource == plotProp_dataSource::Index)
			{
				x_axis_prop_ptr->axis_range[0] = 0;
				if (x_axis_prop_ptr->axis_range[1] < curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_point)
					x_axis_prop_ptr->axis_range[1] = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_point;
			}
		}
		else
		{
			y_axis_prop_ptr->axis_range[0] = tep_ymin;
			y_axis_prop_ptr->axis_range[1] = tep_ymax;
			/*if (x_axis_prop_ptr->axis_dataSource == plotProp_dataSource::file)
			{
				x_axis_prop_ptr->axis_range[0] = xmin;
				x_axis_prop_ptr->axis_range[1] = xmax;
			}
			else*/
			if (x_axis_prop_ptr->axis_dataSource == plotProp_dataSource::Index)
			{
				x_axis_prop_ptr->axis_range[0] = 0;
				x_axis_prop_ptr->axis_range[1] = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_point;
			}
		}
	}
}

void CurveTreeDockWidget::slot_add_curve(int tep_index_file, int tep_index_col)
{
    if(curve_data_ptr==NULL)
        return;
    if(tep_index_file<0)
        return;
    if(tep_index_file>=curve_data_ptr->curve_file_list.count())
        return;
	if (curveExist(tep_index_file, tep_index_col))
        return;
    QString tepstr;
	tepstr = QString("curve_%1_%2").arg(tep_index_file).arg(tep_index_col);
    tep_curve_file=curve_data_ptr->curve_file_list.at(tep_index_file);
	tep_curve_line.axis_index = 0;
	tep_curve_line.yAxis_name = tep_curve_file.file_prop.columns_name_list.at(tep_index_col);
	tep_curve_line.yAxis_column_name = tep_curve_file.file_prop.columns_name_list.at(tep_index_col);
	tep_curve_line.yAxis_column = tep_index_col;
	tep_curve_line.yAxis_coor_max = tep_curve_file.file_prop.val_max[tep_index_col];
	tep_curve_line.yAxis_coor_min = tep_curve_file.file_prop.val_min[tep_index_col];
    tep_curve_line.flag_showPoint =false;
	tep_curve_line.curve_lineType = Qt::PenStyle::SolidLine;
    tep_curve_line.curve_lineWidth = 2;
	add_count++;
	red0 = (red0 + add_count * 166) % 256;
	green0 = (blue0 + add_count * 16) % 256;
	blue0 = (blue0 + add_count * 66) % 256;
    tep_curve_line.curve_color=QColor(red0,green0,blue0);
	//tep_curve_line.flag_aniPoint = false;
	tep_curve_line.aniPoint_color = tep_curve_line.curve_color;
	tep_curve_line.aniPoint_type = (aniPointType)0;
    tep_curve_file.line_data.append(tep_curve_line);
    curve_data_ptr->curve_file_list.replace(tep_index_file,tep_curve_file);
    int tep_num_curves=tep_curve_file.line_data.count();
    int tep_index_curve=tep_num_curves-1;
    QTreeWidgetItem *curve_parent_item=curve_project_item->child(tep_index_file);
    QTreeWidgetItem *tep_curve_item = new QTreeWidgetItem(curve_parent_item);
	tep_curve_item->setExpanded(true);
	tep_curve_item->setText(0, tep_curve_line.yAxis_name);   
    tep_curve_item->setData(0, Qt::UserRole + 1,tepstr);
	/*QTreeWidgetItem *tep_Info_item = new QTreeWidgetItem(tep_curve_item);
	if (curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[tep_curve_line.axis_index].axis_dataSource==plotProp_dataSource::index)
		tep_Info_item->setText(0, QString("x:%1  y:%2").arg(tr("Index")).arg(tep_curve_line.yAxis_column));
	else
		tep_Info_item->setText(0, QString("x:%1  y:%2").arg(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[tep_curve_line.axis_index].axis_fileColumn).arg(tep_curve_line.yAxis_column));
	
	tep_Info_item->setExpanded(true);*/
	curveTree->setCurrentItem(tep_curve_item);
	setAxisRange(tep_curve_line.axis_index, tep_index_file, tep_index_col);
//    if(curve_data_ptr->glo_curve_data.xAxis_origin=="file")
//    {
//        if(curve_data_ptr->glo_curve_data.xAxis_file_prop.Axis_filename.isNull())
//            return;
//        QFile file1(curve_data_ptr->glo_curve_data.xAxis_name);
//        if(!file1.exists())
//            return;
//        if(curve_data_ptr->glo_curve_data.xAxis_column<0)
//            return;
//        if(curve_data_ptr->glo_curve_data.xAxis_column>=curve_data_ptr->glo_curve_data.xAxis_file_prop.columns_name_list.count())
//            return;
//    }
	emit sig_add_curve_plot(tep_index_file, tep_index_curve);
}

void CurveTreeDockWidget::slot_del_curve(int tep_index_file,int tep_index_col)
{
    if(curve_data_ptr==NULL)
        return;
    /*QMessageBox box(QMessageBox::Warning, tr("warning"), QStringLiteral("Are you sure to delete the curve?"));
    box.setStandardButtons(QMessageBox::Yes);
    box.setButtonText(QMessageBox::Yes, QStringLiteral("OK"));
    if(box.exec()!=QMessageBox::Yes)
        return;*/
	if (!curveExist(tep_index_file, tep_index_col))
        return;
    QTreeWidgetItem *parent_item=curve_project_item->child(tep_index_file);
	QString tepstr = QString("curve_%1_%2").arg(tep_index_file).arg(tep_index_col);
    int num_child=parent_item->childCount();
    for(int i=0;i<num_child;i++)
    {
        if(parent_item->child(i)->data(0, Qt::UserRole+1).toString()==tepstr)
        {
            parent_item->removeChild(parent_item->child(i));
            break;
        }
    }
    curveTree->setCurrentItem(parent_item);
	emit sig_del_curve_plot(tep_index_file,tep_index_col);
}

void CurveTreeDockWidget::slot_addFile()
{
    if(curve_data_ptr==NULL)
        return;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open"), "", tr("file(*.dat)"));
    if(fileName==NULL)
        return;
	readDataFile(fileName);
}

bool CurveTreeDockWidget::readDataFile(QString data_filename)
{
    bool ret=false;
    readDataFileProp *read_data_file1=new readDataFileProp;
    tep_curve_file.line_data.clear();
    if(read_data_file1->initNewFileProp(data_filename,&tep_curve_file.file_prop))
    {
        QTreeWidgetItem *item1 = new QTreeWidgetItem(curve_project_item);
		if (data_filename.size() > 26)
		{
			if (data_filename.size() > 52)
				data_filename.insert(52, "\n");
			data_filename.insert(26,"\n");
		}
		
        item1->setText(0, data_filename);
        
		item1->setData(0, Qt::UserRole + 1, "curve_file");
        curveTree->setCurrentItem(item1);
		curve_data_ptr->glo_curve_data.file_num++;
        curve_data_ptr->curve_file_list.append(tep_curve_file);
        emit sig_console_file_data_val_show(curve_data_ptr->curve_file_list.count()-1);
		if (data_filename.size() > 26)
			data_filename.simplified();
        
        ret=true;
    }
    delete read_data_file1;
    return ret;
}

void CurveTreeDockWidget::addFile_from_array(QString data_filename)
{
	QTreeWidgetItem *item1 = new QTreeWidgetItem(curve_project_item);
	item1->setText(0, data_filename);
	item1->setData(0, Qt::UserRole + 1, "curve_file");
	curveTree->setCurrentItem(item1);
	emit sig_console_file_data_val_show(curve_data_ptr->curve_file_list.count() - 1);

}

QStringList CurveTreeDockWidget::get_curve_filelist(QString dirname)
{
    QFileInfo fileInfo;
    QStringList fileList;
    fileList.clear();
    QDir dir(dirname);
    if(!dir.exists())
        return fileList;
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Time|QDir::Reversed);//
    QFileInfoList list=dir.entryInfoList();
    if(list.count()>0)
    {
        for(int i=0;i<list.count();i++)
        {
            fileList.append(list.at(i).filePath());
        }
    }
    if((curve_data_ptr!=NULL)&&(fileList.count()>0))
    {
        //curve_data_ptr->glo_curve_data.flag_path=true;
        curve_data_ptr->glo_curve_data.curve_fileNameList=fileList;
    }
    return fileList;
}

void CurveTreeDockWidget::slot_delFile()
{
    if(curve_data_ptr==NULL)
        return;
    //20170314 del projectTreeItemClicked(curve_project_item,0);
    int tep_index_line=curve_project_item->indexOfChild(cur_item);
	if (tep_index_line == -1)
		return;
    curve_project_item->takeChild(tep_index_line);
    curve_data_ptr->curve_file_list.removeAt(tep_index_line);
    curve_data_ptr->glo_curve_data.file_num--;
    emit sig_update_clear_console_data();
    curveTree->setCurrentItem(curve_project_item);
    if(curve_project_item->childCount()==0)
    {
        file_add_action->setEnabled(true);
    }
}

void CurveTreeDockWidget::slot_delAllFiles()
{
	if (curve_data_ptr == NULL)
		return;
	emit sig_update_clear_console_data();
	curveTree->setCurrentItem(curve_project_item);
	int num_child = curve_project_item->childCount();
	for (int i = 0; i < num_child; i++)
		curve_project_item->takeChild(num_child - 1 - i);
	curve_data_ptr->glo_curve_data.curve_fileNameList.clear();
	curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_dataSource = plotProp_dataSource::Index;
	curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_fileIndex = -1;
	curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_fileColumn = -1;
	curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_dataSource = plotProp_dataSource::Index;
	curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_fileIndex = -1;
	curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_fileColumn = -1;
	curve_data_ptr->glo_curve_data.file_num = 0;
	curve_data_ptr->curve_file_list.clear();
	/*int num_fileItems = curve_project_item->childCount();
	for (int i = 0; i < num_fileItems; i++)
	{
		curve_project_item->takeChild(num_fileItems-i-1);
		curve_data_ptr->curve_file_list.removeAt(num_fileItems - i - 1);
		curve_data_ptr->glo_curve_data.file_num--;
	}	
	emit sig_update_clear_console_data();
	curveTree->setCurrentItem(curve_project_item);
	if (curve_project_item->childCount() == 0)
	{
		file_add_action->setEnabled(true);
	}*/
}

//void CurveTreeDockWidget::on_treeWidget_customContextMenuRequested(QTreeWidgetItem * item, int column)
void CurveTreeDockWidget::on_treeWidget_customContextMenuRequested(QPoint pos)
{
    QTreeWidgetItem *item=curveTree->itemAt(pos);
    if(item==NULL)
    {
        file_add_popMenu->exec(QCursor::pos());
    }
    else
    {
        cur_item = item;
        if (item->data(0, Qt::UserRole + 1).toString() == "curve_project")
        {
            file_add_popMenu->exec(QCursor::pos());
        }
        else if (item->data(0, Qt::UserRole + 1).toString() == "curve_file")
        {
			if (item->childCount() == 0)
			{
				file_del_popMenu->exec(QCursor::pos());
			}
        }
    }
}

void CurveTreeDockWidget::projectTreeItemClicked(QTreeWidgetItem *item,int column)
{
    if(item==NULL)
        return;
    if(item->data(0, Qt::UserRole + 1).toString() == "curve_file")
    {
        index_file = curve_project_item->indexOfChild(item);
        QVector<int> tep_curve_col_list;
        QStringList tep_list;
        bool ok;
        tep_curve_col_list.clear();
        for(int i=0;i<item->childCount();i++)
        {
            tep_list=item->child(i)->data(0,Qt::UserRole+1).toString().split("_",QString::SkipEmptyParts);
            if(tep_list.count()==3)
            {
                tep_curve_col_list.append(tep_list.at(2).toInt(&ok));
            }
        }
        emit sig_console_file_data_val_show_from_curveData(index_file,tep_curve_col_list);
    }
    index_curve=-1;
}

void CurveTreeDockWidget::func_projectTreeItemClicked(int index_file)
{
	QVector<int> tep_curve_col_list;
	QStringList tep_list;
	bool ok;
	tep_curve_col_list.clear();
	QTreeWidgetItem *item = curve_project_item->child(index_file);
	for (int i = 0; i<item->childCount(); i++)
	{
		tep_list = item->child(i)->data(0, Qt::UserRole + 1).toString().split("_", QString::SkipEmptyParts);
		if (tep_list.count() == 3)
		{
			tep_curve_col_list.append(tep_list.at(2).toInt(&ok));
		}
	}
	emit sig_console_file_data_val_show_from_curveData(index_file, tep_curve_col_list);
	index_curve = -1;
}

void CurveTreeDockWidget::reTranslate()
{
	this->setWindowTitle(tr("browser"));
	file_add_action->setText(tr("add file"));
	file_add_action->setStatusTip(tr("add a file"));
	file_del_action->setText(tr("del file"));
	file_del_action->setStatusTip(tr("delete a file"));
	if (curve_project_item!=NULL)
		curve_project_item->setText(0, tr("curve_project"));
}
/*void CurveTreeDockWidget::projectTreeDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (item == NULL)
		return;
	QString tepstr = item->data(0, Qt::UserRole + 1).toString().trimmed();
	QStringList tep_list = tepstr.split("_", QString::SkipEmptyParts);
	if (tep_list.count() != 3)
		return;
	bool ok;
	int tep_file_index = tep_list.at(1).toInt(&ok);
	int tep_column_index = tep_list.at(2).toInt(&ok);
	int tep_curve_index = -1;
	if (tep_file_index >= curve_data_ptr->curve_file_list.count())
		return;
	if (tep_column_index >= curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	for (int i = 0; i < curve_data_ptr->curve_file_list.at(tep_file_index).line_data.count(); i++)
	{
		if (curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(i).yAxis_column == tep_column_index)
		{
			tep_curve_index = i;
			break;
		}
	}
	if (tep_curve_index == -1)
		return;
	//cur_selected_curve = tep_curve;
	CurvePropForm* curve_PropForm = new CurvePropForm;
	curve_PropForm->curve_data_ptr = curve_data_ptr;
	curve_PropForm->initForm(1, tep_file_index, tep_curve_index);
	//connect(curve_PropForm, SIGNAL(sig_update_curve_name(int, int)), this, SLOT(slot_update_curve_name(int, int)));
	//connect(curve_PropForm, SIGNAL(sig_update_curve_pen(int, int)), this, SLOT(slot_update_curve_pen(int, int)));
	//connect(curve_PropForm, SIGNAL(sig_update_curve_linePoint(int, int)), this, SLOT(slot_update_curve_linePoint(int, int)));
	//connect(curve_PropForm, SIGNAL(sig_update_curve_axis(int, int)), this, SLOT(slot_update_curve_axis(int, int)));
	curve_PropForm->exec();
}*/
