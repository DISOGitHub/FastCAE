#include "PipelineObjTreeDockWidget.h"
#include "FileDialogDirectory.h"
#include "qmessagebox.h"
#include <QDomProcessingInstruction>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
PipelineObjTreeDockWidget::PipelineObjTreeDockWidget(QWidget *parent) : QDockWidget(parent)
{
	cur_Item = NULL;
	mActivePiplnObj = NULL;
	openDir = "";
	flag_pipelineObj_selected = false;
	setStyleSheet("QWidget{background-color: #eaf2f8;}""QDockWidget::title{border-radius: 6px; background-image: url(:/images/project_title.png);}""QMenu{color:black;}""QMenu:hover{color:black;}");
    this->setWindowTitle(tr("Pipeline Browser"));//nodes list
	initPopMenu();
    initWidget();
	wid = 200;
	hei = 400;
}

PipelineObjTreeDockWidget::~PipelineObjTreeDockWidget()
{

}

void PipelineObjTreeDockWidget::initPopMenu()
{
    file_add_popMenu = new QMenu(this);
    file_add_action = new QAction(tr("Open"), this);
    file_add_action->setStatusTip(tr("open a file"));
    file_add_popMenu->addAction(file_add_action);

    file_del_popMenu = new QMenu(this);
    file_del_action = new QAction(tr("Delete"), this);
    file_del_action->setStatusTip(tr("delete a file"));
    file_del_popMenu->addAction(file_del_action);

    connect(file_add_action, SIGNAL(triggered()), this, SLOT(slot_addFile()));
	connect(file_del_action, SIGNAL(triggered()), this, SLOT(OnDeletePipelineItem()));
}

void PipelineObjTreeDockWidget::initTree()
{
	pipelineObjTree->clear();
	mBuiltinItem = new QTreeWidgetItem(pipelineObjTree);
	mBuiltinItem->setText(0, tr("buildIn"));
	mBuiltinItem->setIcon(0, QIcon(":/images/pipeline_list.png"));
	mBuiltinItem->setData(0, Qt::UserRole + 1, "buildIn");
	pipelineObjTree->setCurrentItem(mBuiltinItem);
	if (file_add_action != NULL)
		file_add_action->setEnabled(true);
}

void PipelineObjTreeDockWidget::initWidget()
{
	pipelineObjTree = new DropTreeWidget(this);
	pipelineObjTree->setHeaderHidden(true);
	pipelineObjTree->setContextMenuPolicy(Qt::CustomContextMenu);

	pipelineObjTree->setItemsExpandable(true);
	pipelineObjTree->expandAll();
    connect(pipelineObjTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(projectTreeItemClicked(QTreeWidgetItem*, int)));
	connect(pipelineObjTree, SIGNAL(sig_add_dropItem(QString)), this, SLOT(slot_add_dropItem(QString)));
	connect(pipelineObjTree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_treeWidget_customContextMenuRequested(QPoint)));
	//connect(pipelineObjTree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(on_treeWidget_itemChanged(QTreeWidgetItem *, int)));
	
    setWidget(pipelineObjTree);
	initTree();
}

//void PipelineObjTreeDockWidget::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
//{
//	if (item == NULL)
//		return;
//	if (item->data(0, Qt::UserRole + 1).toString() == "buildIn")
//		return;
//	bool flag_vis = item->checkState(0);
//	if (mTreeItems.contains(item))
//	{
//		emit sig_update_actor_visible_plotForm(mTreeItems[item],flag_vis);
//		//mTreeItems[item]->SetPipelineObjectVisible(false);
//	}
//}

void PipelineObjTreeDockWidget::slot_add_dropItem(QString tep_filename)
{
	QStringList selectedList;
	selectedList.push_back(tep_filename);
	emit sig_LoadDataSource(tep_filename, selectedList,0);
}

void PipelineObjTreeDockWidget::slot_addFile()
{
	QString fileName;
	QStringList fileDirectoryList;
	QStringList fileList;
	QString folder = QDir::currentPath();
	QString filter = tr("Data File (*.vtk *.vtu *.vtr *.vts *.vtp *.dat *.moor *.cgns *.msh *.obj *.sol *.obj *.igs *.iges *.neu *.stl *.cntm)");
	FileDialogDirectory *fileDirectory = new FileDialogDirectory;
	fileDirectory->initWin(openDir);
	if (fileDirectory->exec() == QDialog::Accepted)
	{
		fileName = fileDirectory->mSelectionText;
		fileDirectoryList = fileName.split(";", QString::SkipEmptyParts);
		
		//qDebug() << fileDirectoryList;
		for (int i = 0; i < fileDirectoryList.count(); i++)
		{
			QString DirectoryName;
			DirectoryName = fileDirectoryList.at(i);
			QVector<QString> GroupFileList;
			GroupFileList = fileDirectory->mFileGroup[DirectoryName];
			QStringList selectedList;
			if (DirectoryName.contains("..."))
			{
				for (int i = 0; i < GroupFileList.count(); i++)
				{
					selectedList.push_back(fileDirectory->mCurrentFilePath + GroupFileList.at(i));
				}
				emit sig_LoadDataSource(fileDirectory->mCurrentFilePath + GroupFileList.at(0), selectedList,0);
			
			}
			else
			{

				selectedList.push_back(fileDirectory->mCurrentFilePath + DirectoryName);
				emit sig_LoadDataSource(fileDirectory->mCurrentFilePath + DirectoryName, selectedList,0);

			}

		}
	}
	delete fileDirectory;
	fileDirectory = NULL;
}

QTreeWidgetItem* PipelineObjTreeDockWidget::GetTreeItemFromPipelineObj(PipelineObject* pipeObj)
{
	QMap<QTreeWidgetItem*, PipelineObject*>::iterator it;
	for (it = mTreeItems.begin(); it != mTreeItems.end(); it++)
	{
		if (pipeObj == it.value())
			return it.key();
	}
	return 0;
}

void PipelineObjTreeDockWidget::AddPipelineObject(PipelineObject* pipeObj)
{
	QTreeWidgetItem* parentItem = mBuiltinItem;
	PipelineObject* parentObj = pipeObj->GetObjParent();
	if (parentObj != 0)
	{
		parentItem = GetTreeItemFromPipelineObj(parentObj);
	}
	QTreeWidgetItem *itm = new QTreeWidgetItem();
	if (pipeObj->GetObjectType() == dNumeric_DataSource)
	{
		if(pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count()>1)//if (pipeObj->GetSolutionTimeSteps().count() > 1)
		{
			QString NewFileName;
			QFileInfo fileinfo;
			fileinfo = QFileInfo(pipeObj->GetName());
			//fileinfo.fileName;
			NewFileName = fileinfo.baseName() + "*";
			itm->setText(0, NewFileName);
		}
		else if (pipeObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() == 1)
		{
			itm->setText(0, pipeObj->GetName());
		}
	}
	else
	{
		itm->setText(0, pipeObj->GetName());
	}
	itm->setData(0, Qt::UserRole + 1, pipeObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
	SetPipelineIconByPipelineType(pipeObj, itm);
	itm->setFlags(itm->flags() | Qt::ItemIsUserCheckable);
	itm->setCheckState(0, Qt::Checked);
	parentItem->addChild(itm);
	mTreeItems[itm] = pipeObj;
	mActivePiplnObj = pipeObj;
	pipelineObjTree->expandAll();
	emit sig_update_aniToolBar(mBuiltinItem->childCount());
	emit sig_update_filterToolBar(true,mActivePiplnObj);
}

void PipelineObjTreeDockWidget::SetPipelineIconByPipelineType(PipelineObject* pipeObj, QTreeWidgetItem* pitem)
{
	if (pipeObj != NULL)
	{
		if (pipeObj->GetObjectType() == dNumeric_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/source.png"));
		}
		else if (pipeObj->GetObjectType() == dContour_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/isosurf.png"));
		}
		else if (pipeObj->GetObjectType() == dClip_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/clip.png"));
		}
		else if (pipeObj->GetObjectType() ==dVector_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/glyph.png"));
		}
		else if (pipeObj->GetObjectType() == dSlice_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/slice.png"));
		}
		else if (pipeObj->GetObjectType() == dReflection_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/Reflection.png"));
		}
		else if (pipeObj->GetObjectType() == dCalculator_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/calculator.png"));
		}
		else if (pipeObj->GetObjectType() == dSmooth_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/smooth.png"));
		}
		else if (pipeObj->GetObjectType() == dStreamLine_DataSource)
		{
			pitem->setIcon(0, QIcon(":/images/streamline.png"));
		}
	}
}

//void PipelineObjTreeDockWidget::slot_file_updateCgns_action()
//{
//	if (cur_Item == NULL)
//		return;
//	PipelineObject *pipeObj = mTreeItems[cur_Item];
//	emit sig_update_cgns_pipelineObj(pipeObj);
//	
//}

void PipelineObjTreeDockWidget::OnDeletePipelineItem()
{
	if (cur_Item == NULL)
		return;
	PipelineObject *pipeObj = mTreeItems[cur_Item];
	if (cur_Item->childCount() > 0)
	{
		QMessageBox box(QMessageBox::Warning, tr("warning"),tr("The pipeline object has children,you can not delete it !"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, tr("ok"));
		box.exec();
		return;
	}
	emit sig_delPipelineObj(pipeObj);
	/*mTreeItems.remove(cur_Item);
	delete cur_Item;
	cur_Item = NULL;
	pipelineObjTree->setCurrentItem(mBuiltinItem);
	cur_Item = mBuiltinItem;*/
	
}

void PipelineObjTreeDockWidget::RmvActivePipelineObject()//如果Treewidget中已经没有结点，则清空当前活动结点
{
	if (mTreeItems.count() == 0)
	{
		mActivePiplnObj = NULL;
	}
}

void PipelineObjTreeDockWidget::RmvPipelineObject(PipelineObject* pipeObj)
{
	if (pipeObj == NULL)
		return;
	QTreeWidgetItem* treeItem = this->GetTreeItemFromPipelineObj(pipeObj);
	if (treeItem != 0)
	{
		//显示父节点
		if (pipeObj->GetObjectType() != dNumeric_DataSource)
		{
			treeItem->parent()->setCheckState(0,Qt::Checked);
			pipeObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = true;
		}
		//显示父节点

		pipelineObjTree->removeItemWidget(treeItem, 0);
		mTreeItems.remove(treeItem);
		delete treeItem;
		treeItem = NULL;
	}
	//pipelineObjTree->setCurrentItem(mBuiltinItem);
	projectTreeItemClicked(mBuiltinItem, 0);
	emit sig_update_aniToolBar(mBuiltinItem->childCount());
}

void PipelineObjTreeDockWidget::on_treeWidget_customContextMenuRequested(QPoint pos)
{
    //if (!(QGuiApplication::mouseButtons() & Qt::RightButton)) return;
	QTreeWidgetItem *item = pipelineObjTree->itemAt(pos);
    if(item==NULL)
    {
        file_add_popMenu->exec(QCursor::pos());
    }
    else
    {
        cur_Item = item;
        if (item->data(0, Qt::UserRole + 1).toString() == "buildIn")
        {
            file_add_popMenu->exec(QCursor::pos());
        }
        else 
        {
			//PipelineObject *pipeObj = mTreeItems[cur_Item];
			if (item->childCount() == 0)
			{
				file_del_popMenu->exec(QCursor::pos());
			}
        }
    }
}

void PipelineObjTreeDockWidget::editScript_start(QString fileName)
{
	QString command_line;
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QTextStream stream(&tep_file1);
		stream << "#The script file is build " + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") << endl;
		command_line = QString("Set_PlotSize(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10)").arg(saveScript_prop_data.plot_wid).arg(saveScript_prop_data.plot_hei)
				.arg(saveScript_prop_data.tree_wid).arg(saveScript_prop_data.tree_hei)
				.arg(saveScript_prop_data.prop_wid).arg(saveScript_prop_data.prop_hei)
				.arg(saveScript_prop_data.console_wid).arg(saveScript_prop_data.console_hei)
				.arg(saveScript_prop_data.mainwindow_wid).arg(saveScript_prop_data.mainwindow_hei);
			stream << command_line << endl;
		tep_file1.close();
	}
}

//void PipelineObjTreeDockWidget::editScript_end(QString fileName)
//{
//	QFile tep_file1(fileName);
//	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
//	{
//		QTextStream stream(&tep_file1);
//		stream << "Apply()" << endl;
//		QString tep_str = "Save_Image(0, 800, 600," ;
//		tep_str += QDir::currentPath();
//		tep_str += "/script_bmp.jpg)";
//		stream <<tep_str << endl;
//		if (flag_script_ani)
//		{
//			QString tep_str1 = "Save_Animate(";
//			tep_str1 += QDir::currentPath();
//			tep_str1 += "/script_avi.avi)";
//			stream << tep_str1 << endl;
//			stream << "StartAnimate()" << endl;
//		}
//		stream << "#The script file is over! " << endl;
//		tep_file1.close();
//	}
//}
void PipelineObjTreeDockWidget::editScript_end(QString fileName)
{
	QString command_line;
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		stream << "Apply()" << endl;	
		/*if (saveScript_prop_data.flag_plotSize)
		{
			command_line = QString("Set_PlotSize(%1,%2,%3,%4,%5,%6,%7,%8)").arg(saveScript_prop_data.plot_wid).arg(saveScript_prop_data.plot_hei)
				.arg(saveScript_prop_data.tree_wid).arg(saveScript_prop_data.tree_hei)
				.arg(saveScript_prop_data.prop_wid).arg(saveScript_prop_data.prop_hei)
				.arg(saveScript_prop_data.console_wid).arg(saveScript_prop_data.console_hei)
				.arg(saveScript_prop_data.mainwindow_wid).arg(saveScript_prop_data.mainwindow_hei);
			stream << command_line << endl;
		}*/
		if (saveScript_prop_data.saveImage_fileName != "")
		{
			command_line = QString("Save_Image(0, %1, %2, %3)").arg(saveScript_prop_data.image_wid).arg(saveScript_prop_data.image_hei).arg(saveScript_prop_data.saveImage_fileName);
			stream << command_line << endl;
		}
		if (flag_script_ani && (saveScript_prop_data.saveAvi_fileName != ""))
		{
			command_line = QString("Save_Animate(%1)").arg(saveScript_prop_data.saveAvi_fileName);
			stream << command_line << endl;
			stream << "StartAnimate()" << endl;
		}
		if (saveScript_prop_data.flag_autoExit)
		{
			if (saveScript_prop_data.timeMS_autoExit<0)
			{
				saveScript_prop_data.timeMS_autoExit = 0;
			}
			command_line = QString("Exit_SoftWare(%1)").arg(saveScript_prop_data.timeMS_autoExit);
			stream << command_line << endl;
		}
		/*command_line = QString("ReturnToOldSize()");
		stream << command_line << endl;*/
		stream << "#The script file is over! " << endl;
		tep_file1.close();
	}
}
void PipelineObjTreeDockWidget::editScript_scalarBarPosition(QTreeWidgetItem* data_item, QString fileName)
{
	//bool flag_Obj;
	QString command_line;
	PipelineObject* tep_pipelineObj = mTreeItems[data_item];
	QMap<QString, bool>::iterator it;
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		for (it = tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.begin(); it != tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.end(); it++)
		{
			if (!it.value())
				continue;
			if (tep_pipelineObj->scalarBar_widgetMap.contains(it.key()))
			{
				vtkScalarBarWidget* tep_scalarWidget = tep_pipelineObj->scalarBar_widgetMap[it.key()];
				double* pos1 = tep_scalarWidget->GetScalarBarActor()->GetPosition();
				double* pos2 = tep_scalarWidget->GetScalarBarActor()->GetPosition2();
				command_line = QString("Properties_scalarBarPosition(%1,%2,%3,%4,%5,%6)").arg(it.key()).arg(tep_scalarWidget->GetScalarBarActor()->GetOrientation()).arg(pos1[0]).arg(pos1[1]).arg(pos2[0]).arg(pos2[1]);
				stream << command_line << endl;
			}
		}
		stream << endl;
		tep_file1.close();
	}
}
void PipelineObjTreeDockWidget::editScript_filter(QTreeWidgetItem* data_item, QString fileName)
{
	bool flag_Obj;
	QString command_line;
	PipelineObject* piplnObj_filter;
	int num_filter = data_item->childCount();
	if (num_filter > 0)
	{
		for (int j = 0; j < num_filter; j++)
		{
			flag_Obj = false;
			QTreeWidgetItem *filter_item = data_item->child(j);
			if (mTreeItems.contains(filter_item))
			{
				QFile tep_file1(fileName);
				if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
				{
					QTextStream stream(&tep_file1);
					piplnObj_filter = mTreeItems[filter_item];
					if (piplnObj_filter->GetObjectType() == dClip_DataSource)
					{
						command_line = QString("# Clip Filter");
						stream << command_line << endl;
						/*command_line = QString("Filter_Clip(%1,%2,%3,%4,%5,%6)").arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.x).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.y).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.z).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.x).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.y).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.z);*/
						command_line = QString("Filter_Clip()");
						stream << command_line << endl;
						flag_Obj = true;
					}
					else if (piplnObj_filter->GetObjectType() == dSlice_DataSource)
					{
						command_line = QString("# Slice Filter");
						stream << command_line << endl;
						/*command_line = QString("Filter_Slice(%1,%2,%3,%4,%5,%6)").arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.x).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.y).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.z).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.x).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.y).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.z);*/
						command_line = QString("Filter_Slice()");
						stream << command_line << endl;
						flag_Obj = true;
					}
					else if (piplnObj_filter->GetObjectType() == dVector_DataSource)
					{
						command_line = QString("# Glyph Filter");
						stream << command_line << endl;
						//command_line = QString("Filter_Vector(%1,%2)").arg(piplnObj_filter->mPipeLineObjProp.vectorFilter_propData.scalarColName).arg(piplnObj_filter->mPipeLineObjProp.vectorFilter_propData.vectorColName);
						command_line = QString("Filter_Vector()");
						stream << command_line << endl;
						flag_Obj = true;
					}
					else if (piplnObj_filter->GetObjectType() == dStreamLine_DataSource)
					{
						command_line = QString("# StreamLine Filter");
						stream << command_line << endl;
						//command_line = QString("Filter_Vector(%1,%2)").arg(piplnObj_filter->mPipeLineObjProp.vectorFilter_propData.scalarColName).arg(piplnObj_filter->mPipeLineObjProp.vectorFilter_propData.vectorColName);
						command_line = QString("Filter_StreamLine()");
						stream << command_line << endl;
						flag_Obj = true;
					}
					else if (piplnObj_filter->GetObjectType() == dContour_DataSource)
					{
						command_line = QString("# Contour Filter");
						stream << command_line << endl;
						//command_line = QString("Filter_Contour(%1,%2)").arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.contour_colName).arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.val);
						command_line = QString("Filter_Contour()");
						stream << command_line << endl;
						flag_Obj = true;
					}
					else if (piplnObj_filter->GetObjectType() == dReflection_DataSource)
					{
						command_line = QString("# Reflection Filter");
						stream << command_line << endl;
						//command_line = QString("Filter_Contour(%1,%2)").arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.contour_colName).arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.val);
						command_line = QString("Filter_Reflection()");
						stream << command_line << endl;
						flag_Obj = true;
					}
					else if (piplnObj_filter->GetObjectType() == dCalculator_DataSource)
					{
						command_line = QString("# Calculator Filter");
						stream << command_line << endl;
						//command_line = QString("Filter_Contour(%1,%2)").arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.contour_colName).arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.val);
						command_line = QString("Filter_Calculator()");
						stream << command_line << endl;
						flag_Obj = true;
					}
					else if (piplnObj_filter->GetObjectType() == dSmooth_DataSource)
					{
						command_line = QString("# Smooth Filter");
						stream << command_line << endl;
						//command_line = QString("Filter_Contour(%1,%2)").arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.contour_colName).arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.val);
						command_line = QString("Filter_Smooth()");
						stream << command_line << endl;
						flag_Obj = true;
					}
					tep_file1.close();
				}
				if (flag_Obj)
				{
					if ((piplnObj_filter->GetObjectType() == dClip_DataSource) || (piplnObj_filter->GetObjectType() == dSlice_DataSource))
						editScript_Properties_Plane(piplnObj_filter, fileName);
					else if (piplnObj_filter->GetObjectType() == dContour_DataSource)
						editScript_Properties_Contour(piplnObj_filter, fileName);
					else if (piplnObj_filter->GetObjectType() == dVector_DataSource)
						editScript_Properties_Vector(piplnObj_filter, fileName);
					else if (piplnObj_filter->GetObjectType() == dReflection_DataSource)
						editScript_Properties_Reflection(piplnObj_filter, fileName);
					else if (piplnObj_filter->GetObjectType() == dCalculator_DataSource)
						editScript_Properties_Calculator(piplnObj_filter, fileName);
					else if (piplnObj_filter->GetObjectType() == dSmooth_DataSource)
						editScript_Properties_Smooth(piplnObj_filter, fileName);
					else if (piplnObj_filter->GetObjectType() == dStreamLine_DataSource)
						editScript_Properties_StreamLine(piplnObj_filter, fileName);
					editScript_Properties(piplnObj_filter, fileName);
				}					
			}
		}
	}
}
void PipelineObjTreeDockWidget::editScript_camera(QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# Camera Setting");
		stream << command_line << endl;
		//command_line = QString("# Camera_Position");
		//stream << command_line << endl;
		command_line = QString("Camera_Position(%1,%2,%3)").arg(script_cameraPara.position[0]).arg(script_cameraPara.position[1]).arg(script_cameraPara.position[2]);
		stream << command_line << endl;
		//command_line = QString("# Camera_FocalPoint");
		//stream << command_line << endl;
		command_line = QString("Camera_FocalPoint(%1,%2,%3)").arg(script_cameraPara.focalPoint[0]).arg(script_cameraPara.focalPoint[1]).arg(script_cameraPara.focalPoint[2]);
		stream << command_line << endl;

		//command_line = QString("# Camera_ClippingRange");
		//stream << command_line << endl;
		//command_line = QString("Camera_ClippingRange(%1,%2)").arg(script_cameraPara.clippingRange[0]).arg(script_cameraPara.clippingRange[1]);
		//stream << command_line << endl;

		//command_line = QString("# Camera_ViewUp");
		//stream << command_line << endl;
		command_line = QString("Camera_ViewUP(%1,%2,%3)").arg(script_cameraPara.viewUp[0]).arg(script_cameraPara.viewUp[1]).arg(script_cameraPara.viewUp[2]);
		stream << command_line << endl;
		//command_line = QString("# Camera_ViewAngle");
		//stream << command_line << endl;
		command_line = QString("Camera_ViewAngle(%1)").arg(script_cameraPara.viewAngle);
		stream << command_line << endl << endl;
		command_line = QString("Camera_ParalellScale(%1)").arg(script_cameraPara.parallel_scale);
		stream << command_line << endl << endl;
		/*command_line = QString("# Camera_Zoom");
		stream << command_line << endl;
		command_line = QString("Camera_Zoom(%1)").arg(script_cameraPara.zoom);
		stream << command_line << endl;*/
		if (saveScript_prop_data.flag_adapt)
		{
			command_line = QString("Camera_SelfAdaptation()");
			stream << command_line << endl << endl;
		}
		tep_file1.close();
	}
}
/*void PipelineObjTreeDockWidget::editScript_filter(QTreeWidgetItem* data_item, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		int num_filter = data_item->childCount();
		if (num_filter > 0)
		{
			for (int j = 0; j < num_filter; j++)
			{
				QTreeWidgetItem *filter_item = data_item->child(j);
				if (mTreeItems.contains(filter_item))
				{
					PipelineObject* piplnObj_filter = mTreeItems[filter_item];
					if (piplnObj_filter->GetObjectType() == dClip_DataSource)
					{
						//command_line = QString("# Clip Filter");
						//stream << command_line << endl;
						command_line = QString("Filter_Clip(%1,%2,%3,%4,%5,%6)").arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.x).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.y).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.z).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.x).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.y).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.z);
						stream << command_line << endl;
					}
					else if (piplnObj_filter->GetObjectType() == dSlice_DataSource)
					{
						//command_line = QString("# Slice Filter");
						//stream << command_line << endl;
						command_line = QString("Filter_Slice(%1,%2,%3,%4,%5,%6)").arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.x).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.y).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_origin.z).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.x).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.y).
							arg(piplnObj_filter->mPipeLineObjProp.plane_propData.m_plane_normal.z);
						stream << command_line << endl;
					}
					else if (piplnObj_filter->GetObjectType() == dVector_DataSource)
					{
						//command_line = QString("# Glyph Filter");
						//stream << command_line << endl;
						command_line = QString("Filter_Vector(%1,%2)").arg(piplnObj_filter->mPipeLineObjProp.vectorFilter_propData.scalarColName).arg( piplnObj_filter->mPipeLineObjProp.vectorFilter_propData.vectorColName);
						stream << command_line << endl;
					}
					else if (piplnObj_filter->GetObjectType() == dContour_DataSource)
					{
						//command_line = QString("# Contour Filter");
						//stream << command_line << endl;
						command_line = QString("Filter_Contour(%1,%2)").arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.contour_colName).arg(piplnObj_filter->mPipeLineObjProp.contourFilter_propData.val);
						stream << command_line << endl;
					}
					//editScript_Properties(mActivePiplnObj, tep_filename);
				}
			}
		}
		stream << endl;
		tep_file1.close();
	}
}*/

void PipelineObjTreeDockWidget::editScript_dataSource(QTreeWidgetItem* data_item, QString fileName)
{
	if (mTreeItems.contains(data_item))
	{
		PipelineObject* piplnObj = mTreeItems[data_item];
		QFile tep_file1(fileName);
		if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
		{
			QTextStream stream(&tep_file1);
			QString command_line;
			if (piplnObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > 1)
			{
				flag_script_ani = true;
				QString name;
				QString suffix;
				int pos = piplnObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).lastIndexOf(".");
				if (pos >= 0)
				{
					suffix = piplnObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).right(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).length() - pos - 1);
					name = piplnObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).left(pos);
				}
				command_line = QString("# Animate data source");
				stream << command_line << endl;
				command_line = QString("OpenFile(%1*.%2)").arg(name).arg(suffix);
				stream << command_line << endl;
			}
			else
			{
				command_line = QString("# File data source");
				stream << command_line << endl;
				command_line = QString("OpenFile(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0));
				stream << command_line << endl;
			}
			tep_file1.close();
		}
		if (QFileInfo(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0)).suffix() == "moor")
		{
			editScript_Properties_MooringLine(piplnObj, fileName);
		}
		if (piplnObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile)
		{
			editScript_Properties_AniDisplace(piplnObj, fileName);
		}
	}
}

void PipelineObjTreeDockWidget::editScript_Properties_Zone(PipelineObject* piplnObj, QString fileName)
{
	if (piplnObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.count() < 1)
		return;
	if (!piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks)
		return;
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		//QMap<QString, bool> block_showMap;
		//int flag_grid_blocks;//grid:0,blocks:1;
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# DataSource properties Setting");
		stream << command_line << endl;

		int tep_flag_grid_blocks =piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_grid_blocks;
		command_line = QString("Properties_DataSource_DisplayGridOrZone(%1)").arg(tep_flag_grid_blocks);
		stream << command_line << endl;
		if (tep_flag_grid_blocks)
		{
			QMap<QString, bool>::iterator it;
			QString zoneNameList;
			int tep_count = 0;
			for (it = piplnObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.begin(); it != piplnObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap.end(); it++)
			{
				if (it.value())
				{
					if (tep_count == 0)
						zoneNameList = it.key();
					else
					{
						zoneNameList += ",";
						zoneNameList += it.key();
					}
					tep_count++;
				}
			}
			command_line = QString("Properties_DataSourceZone(%1)").arg(zoneNameList);
			stream << command_line << endl;
		}
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::editScript_Properties_Plane(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# plane properties Setting");
		stream << command_line << endl;
		command_line = QString("Properties_planeOrigin(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.x)
			.arg(piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.y)
			.arg(piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.z);
		stream << command_line << endl;
		command_line = QString("Properties_planeNormal(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.x)
			.arg(piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.y)
			.arg(piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.z);
		stream << command_line << endl;		
		command_line = QString("Properties_planeVisible(%1)").arg(piplnObj->mPipeLineObjProp.plane_propData.flag_plane);
		stream << command_line << endl;
		if (piplnObj->GetObjectType() == dClip_DataSource)
		{
			command_line = QString("Properties_insideOut(%1)").arg(piplnObj->mPipeLineObjProp.plane_propData.flag_insideOut);
			stream << command_line << endl;
		}
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::editScript_Properties_Vector(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# Vector properties Setting");
		stream << command_line << endl;
		command_line = QString("Properties_vector_scalar(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.scalarColName);
		stream << command_line << endl;
		command_line = QString("Properties_vector_normal(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.vectorColName);
		stream << command_line << endl;
		command_line = QString("Properties_vector_numPoints(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.numPoints);
		stream << command_line << endl;
		command_line = QString("Properties_vector_glyph_type(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.type);
		stream << command_line << endl;
		command_line = QString("Properties_vector_glyph_tipRes(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRes);
		stream << command_line << endl;
		command_line = QString("Properties_vector_glyph_tipRad(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRad);
		stream << command_line << endl;
		command_line = QString("Properties_vector_glyph_tipLen(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipLen);
		stream << command_line << endl;
		command_line = QString("Properties_vector_glyph_shaftRes(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRes);
		stream << command_line << endl;
		command_line = QString("Properties_vector_glyph_shaftRad(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRad);
		stream << command_line << endl;
		command_line = QString("Properties_vector_GlyphVector(%1)").arg(piplnObj->mPipeLineObjProp.vectorFilter_propData.glyphVector_colorXYZ);
		stream << command_line << endl;
		
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::editScript_Properties_Contour(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# Contour properties Setting");
		stream << command_line << endl;
		command_line = QString("Properties_contourColumn(%1)").arg(piplnObj->mPipeLineObjProp.contourFilter_propData.contour_colName);
		stream << command_line << endl;
		if (piplnObj->m_reflectionPara.flag_refleciton)
		{
			command_line = QString("Properties_contour_reflection(%1)").arg(1);
			stream << command_line << endl;
			command_line = QString("Properties_contour_reflectionAxes(%1)").arg(piplnObj->m_reflectionPara.type);
			stream << command_line << endl;
		}
		
		//double val;
		for (int i = 0; i < piplnObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.count(); i++)
		{
			if (piplnObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.at(i).colName == piplnObj->mPipeLineObjProp.contourFilter_propData.contour_colName)
			{
				command_line = QString("Properties_contourValue(%1)").arg(piplnObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.at(i).val);
				stream << command_line << endl;
				break;
			}
		}
		
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::editScript_Properties_Reflection(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# Reflection properties Setting");
		stream << command_line << endl;
		command_line = QString("Properties_reflectionAxes(%1)").arg(piplnObj->mPipeLineObjProp.reflectionFilter_propData.reflection_axis);
		stream << command_line << endl;
		int val = 0;
		if (piplnObj->mPipeLineObjProp.reflectionFilter_propData.flag_copyInput)
			val = 1;
		command_line = QString("Properties_reflectionCopyInput(%1)").arg(val);
		stream << command_line << endl;
		tep_file1.close();
	}
}
void PipelineObjTreeDockWidget::editScript_Properties_Calculator(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# Calculator properties Setting");
		stream << command_line << endl;
		command_line = QString("Properties_calculatorFormula(%1)").arg(piplnObj->mPipeLineObjProp.calculatorFilter_propData.formula);
		stream << command_line << endl;
		command_line = QString("Properties_calculatorResultName(%1)").arg(piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName);
		stream << command_line << endl;
		tep_file1.close();
	}
}
void PipelineObjTreeDockWidget::editScript_Properties_Smooth(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		/*QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# Smooth properties Setting");
		stream << command_line << endl;
		int tep_smoothType = 0;
		if (piplnObj->mPipeLineObjProp.smoothFilter_propData.flag_cellToPoint)
			tep_smoothType = 0;
		else if (piplnObj->mPipeLineObjProp.smoothFilter_propData.flag_meshSmooth = true)
			tep_smoothType = 1;
		command_line = QString("Properties_smoothCoef(%1,%2)").arg(tep_smoothType,piplnObj->mPipeLineObjProp.smoothFilter_propData.smooth_coef);
		stream << command_line << endl;*/
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::editScript_Properties_StreamLine(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# StreamLine properties Setting");
		stream << command_line << endl;
		command_line = QString("Properties_streamline_vector(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.vectorColName);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_integration_direction(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.direction);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_integration_type(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.type);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_integration_stepUnit(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.stepUnit);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_integration_initStepLen(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.initStepLen);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_integration_miniStepLen(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.miniStepLen);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_integration_maxiStepLen(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.maxiStepLen);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_stream_maxiSteps(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiSteps);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_stream_maxiStreamLen(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiStreamLen);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_stream_terminalSpeed(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.terminalSpeed);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_stream_maxiError(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiError);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_seeds_type(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.type);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_seeds_mPoint(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.x)
			.arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.y).arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.z);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_seeds_num_points(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.num_points);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_seeds_radius(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.radius);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_vorticity(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.vorticity);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_interpolatorType(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.interpolator_type);
		stream << command_line << endl;
		command_line = QString("Properties_streamline_surface_streamLines(%1)").arg(piplnObj->mPipeLineObjProp.streamLineFilter_propData.surface_streamLines);
		stream << command_line << endl;
		if (piplnObj->m_reflectionPara.flag_refleciton)
		{
			command_line = QString("Properties_streamline_reflection(%1)").arg(1);
			stream << command_line << endl;
			command_line = QString("Properties_streamline_reflectionAxes(%1)").arg(piplnObj->m_reflectionPara.type);
			stream << command_line << endl;
		}
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::editScript_Properties_MooringLine(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# MooringLine Setting");
		stream << command_line << endl;
		command_line = QString("Properties_mooringLine_Type(%1)").arg(piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.type);
		stream << command_line << endl;
		if (piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.type == 0)
		{
			command_line = QString("Properties_mooringLine_tipRes(%1)").arg(piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipRes);
			stream << command_line << endl;
			command_line = QString("Properties_mooringLine_tipRad(%1)").arg(piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipRad);
			stream << command_line << endl;
			command_line = QString("Properties_mooringLine_tipLen(%1)").arg(piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.tipLen);
			stream << command_line << endl;
			command_line = QString("Properties_mooringLine_shaftRes(%1)").arg(piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.shaftRes);
			stream << command_line << endl;
			command_line = QString("Properties_mooringLine_shaftRad(%1)").arg(piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.arrow_data.shaftRad);
			stream << command_line << endl;
			command_line = QString("Properties_mooringLine_scaleFactor(%1)").arg(piplnObj->mPipeLineObjProp.mooringLine_propData.glyph_data.type);
			stream << command_line << endl;
		}
		if (piplnObj->mPipeLineObjProp.mooringLine_propData.flag_seaBottom)
		{
			command_line = QString("Properties_mooringLine_displaySeaBottom(%1)").arg(1);
			stream << command_line << endl;
			command_line = QString("Properties_mooringLine_colorSeaBottom(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.mooringLine_propData.seaBottomColor.r)
				.arg(piplnObj->mPipeLineObjProp.mooringLine_propData.seaBottomColor.g).arg(piplnObj->mPipeLineObjProp.mooringLine_propData.seaBottomColor.b);
			stream << command_line << endl;
		}
		else
			command_line = QString("Properties_mooringLine_displaySeaBottom(%1)").arg(0);
		stream << command_line << endl;
		tep_file1.close();
	}
}
void PipelineObjTreeDockWidget::editScript_Properties_AniDisplace(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# AniDisplace Setting");
		stream << command_line << endl;
		command_line = QString("Properties_aniDisplace_fileName(%1)").arg(piplnObj->mPipeLineObjProp.aniDisplace_propData.path_AniDisplaceFile);
		stream << command_line << endl;
		command_line = QString("Properties_aniDisplace_startPosition(%1)").arg(piplnObj->mPipeLineObjProp.aniDisplace_propData.start_pos);
		stream << command_line << endl;
		command_line = QString("Properties_aniDisplace_interval(%1)").arg(piplnObj->mPipeLineObjProp.aniDisplace_propData.interval);
		stream << command_line << endl;
		command_line = QString("Properties_aniDisplace_zoomFactor(%1)").arg(piplnObj->mPipeLineObjProp.aniDisplace_propData.zoom_factor);
		stream << command_line << endl;
		if (piplnObj->mPipeLineObjProp.aniDisplace_propData.flag_inputGravity)
		{
			command_line = QString("Properties_aniDisplace_gravityCenter(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.x,
				piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.y, piplnObj->mPipeLineObjProp.aniDisplace_propData.m_origin.z);
			stream << command_line << endl;
		}
		tep_file1.close();
	}
}
QString PipelineObjTreeDockWidget::editScript_Properties_LookupTable(PipelineObject* piplnObj)
{
	QString tep_colName;
	PipelineObject* tep_pipeObj;
	vtkColorTransferFunction* tep_colorTransfer;
	QString command_line = "";
	if (piplnObj->GetObjectType() != dNumeric_DataSource)
		tep_pipeObj = piplnObj->GetObjParent();
	else
		tep_pipeObj = piplnObj;
	tep_colName = piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if ((piplnObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_pipeObj->scalarBar_lookupTableMap.contains("CalculatorResult"))
		{
			tep_colorTransfer = tep_pipeObj->scalarBar_lookupTableMap["CalculatorResult"];
			int num1 = tep_colorTransfer->GetSize();
			double *val = tep_colorTransfer->GetDataPointer();
			QString tep = "";
			QString tep_val;
			for (int i = 0; i < num1; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					tep += tep_val.setNum(val[i * 4 + j], 'g', 6); tep += ",";
				}
			}
			command_line = tep;
			command_line = command_line.left(command_line.length() - 1);
		}
	}
	else
	{
		if (tep_pipeObj->scalarBar_lookupTableMap.contains(tep_colName))
		{
			tep_colorTransfer = tep_pipeObj->scalarBar_lookupTableMap[tep_colName];
			int num1 = tep_colorTransfer->GetSize();
			double *val = tep_colorTransfer->GetDataPointer();
			QString tep = "";
			QString tep_val;
			for (int i = 0; i < num1; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					tep += tep_val.setNum(val[i * 4 + j], 'g', 6); tep += ",";
				}
			}
			command_line = tep;
			command_line = command_line.left(command_line.length() - 1);
		}
	}
	return command_line;
}
QString PipelineObjTreeDockWidget::editScript_Properties_LookupOpacity(PipelineObject* piplnObj)
{
	QString tep_colName;
	PipelineObject* tep_pipeObj;
	vtkPiecewiseFunction* tep_colorOpacity;
	QString command_line = "";
	if (piplnObj->GetObjectType() != dNumeric_DataSource)
		tep_pipeObj = piplnObj->GetObjParent();
	else
		tep_pipeObj = piplnObj;
	tep_colName = piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if ((piplnObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_pipeObj->scalarBar_lookupOpacityMap.contains("CalculatorResult"))
		{
			tep_colorOpacity = tep_pipeObj->scalarBar_lookupOpacityMap["CalculatorResult"];
			int num1 = tep_colorOpacity->GetSize();
			double *val = tep_colorOpacity->GetDataPointer();
			QString tep = "";
			QString tep_val;
			for (int i = 0; i < num1; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					tep += tep_val.setNum(val[i * 2 + j], 'g', 6); tep += ",";
				}
			}
			command_line = tep;
			command_line = command_line.left(command_line.length() - 1);
		}
	}
	else
	{
		if (tep_pipeObj->scalarBar_lookupOpacityMap.contains(tep_colName))
		{
			tep_colorOpacity = tep_pipeObj->scalarBar_lookupOpacityMap[tep_colName];
			int num1 = tep_colorOpacity->GetSize();
			double *val = tep_colorOpacity->GetDataPointer();
			QString tep = "";
			QString tep_val;
			for (int i = 0; i < num1; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					tep += tep_val.setNum(val[i * 2 + j], 'g', 6); tep += ",";
				}
			}
			command_line = tep;
			command_line = command_line.left(command_line.length() - 1);
		}
	}
	return command_line;
}
void PipelineObjTreeDockWidget::editScript_Properties(PipelineObject* piplnObj, QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# properties Setting");
		stream << command_line << endl;
		command_line = QString("Properties_Opacity(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mOpacity);
		stream << command_line << endl;
		command_line = QString("Properties_lineWidth(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mLineWidth);
		stream << command_line << endl;
		command_line = QString("Properties_pointSize(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mPointSize);
		stream << command_line << endl;
		command_line = QString("Properties_translate(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.x)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.y)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.z);
		stream << command_line << endl;
		command_line = QString("Properties_origin(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.x)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.y)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.z);
		stream << command_line << endl;
		command_line = QString("Properties_scale(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.x)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.y)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.z);
		stream << command_line << endl;
		command_line = QString("Properties_orientation(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.x)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.y)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.z);
		stream << command_line << endl;
		command_line = QString("Properties_colorColumn(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
		stream << command_line << endl;
		if (piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ != NULL)
		{
			command_line = QString("Properties_colorColumn_component(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ);
			stream << command_line << endl;
		}
		if (piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "SolidColor")
		{
			command_line = QString("Properties_LookupTableMap(%1)").arg(editScript_Properties_LookupTable(piplnObj));
			stream << command_line << endl;
			command_line = QString("Properties_LookupOpacityMap(%1)").arg(editScript_Properties_LookupOpacity(piplnObj));
			stream << command_line << endl;
			PipelineObject* tep_obj = NULL;
			if (piplnObj->GetObjectType() == dNumeric_DataSource)
				tep_obj = piplnObj;
			else
				tep_obj = piplnObj->GetObjParent();
			if ((piplnObj->GetObjectType() == dCalculator_DataSource)
				&&(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle==piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
			{
				if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains("CalculatorResult"))
				{
					if ((tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap["CalculatorResult"])
						&& (piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible))
					{
						command_line = QString("Properties_EnableOpacityMap(%1)").arg(1);
						stream << command_line << endl;
					}
				}
				scalarBarTitle_PropData tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"];
				command_line = QString("Properties_scalarBarTitle(%1,%2)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)
					.arg(tep_titlePara.title);
				stream << command_line << endl;
				command_line = QString("Properties_scalarBarFontSize(%1,%2)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)
					.arg(tep_titlePara.size);
				stream << command_line << endl;
				command_line = QString("Properties_scalarBarNumLables(%1,%2)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)
					.arg(tep_titlePara.num_lable);
				stream << command_line << endl;
			}
			else
			{
				if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
				{
					if ((tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap[piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle])
						&& (piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible))
					{
						command_line = QString("Properties_EnableOpacityMap(%1)").arg(1);
						stream << command_line << endl;
					}
				}
				scalarBarTitle_PropData tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle];
				command_line = QString("Properties_scalarBarTitle(%1,%2)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)
					.arg(tep_titlePara.title);
				stream << command_line << endl;
				command_line = QString("Properties_scalarBarFontSize(%1,%2)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)
					.arg(tep_titlePara.size);
				stream << command_line << endl;
				command_line = QString("Properties_scalarBarNumLables(%1,%2)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)
					.arg(tep_titlePara.num_lable);
				stream << command_line << endl;
			}
			
		}
		command_line = QString("Properties_specularColor(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.r)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.g)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.b);
		stream << command_line << endl;
		command_line = QString("Properties_solidColor(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.r)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.g)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.b);
		stream << command_line << endl;
		command_line = QString("Properties_edgeColor(%1,%2,%3)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.r)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.g)
			.arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.b);
		stream << command_line << endl;
		command_line = QString("Properties_representation(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_enum_representationtype);
		stream << command_line << endl;
		command_line = QString("Properties_interpolation(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_enum_interpolationtype);
		stream << command_line << endl;
		command_line = QString("Properties_specular(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecular);
		stream << command_line << endl;
		command_line = QString("Properties_diffuse(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mDiffuse);
		stream << command_line << endl;
		command_line = QString("Properties_ambient(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mAmbient);
		stream << command_line << endl;
		command_line = QString("Properties_specularPower(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularPower);
		stream << command_line << endl;
		command_line = QString("Properties_visible(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors);
		stream << command_line << endl;
		command_line = QString("Properties_Flag_scalarBars(%1)").arg(piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible);
		stream << command_line << endl ;
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::editScript_Properties_RenderView(QString fileName)
{
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# RenderView Setting");
		stream << command_line << endl;
		
		command_line = QString("Properties_view_backgroundType(%1)").arg(script_renderViewPara.back_color_type);
		stream << command_line << endl;
		
		command_line = QString("Properties_view_backgroundColor(%1,%2,%3)").arg(script_renderViewPara.back_color1.red()).arg(script_renderViewPara.back_color1.green()).
			arg(script_renderViewPara.back_color1.blue());
		stream << command_line << endl;
		
		command_line = QString("Properties_view_background2Color(%1,%2,%3)").arg(script_renderViewPara.back_color2.red()).arg(script_renderViewPara.back_color2.green()).
			arg(script_renderViewPara.back_color2.blue());
		stream << command_line << endl;
		
		command_line = QString("Properties_view_axesVisible(%1)").arg(script_renderViewPara.flag_axesVisible);
		stream << command_line << endl;
		command_line = QString("Properties_view_cameraParallel(%1)").arg(script_renderViewPara.flag_camera_parallel);
		stream << command_line << endl;
		
		if (script_renderViewPara.flag_viewInteraction)
			command_line = QString("Properties_view_interaction(%1)").arg(1);
		else
			command_line = QString("Properties_view_interaction(%1)").arg(0);
		stream << command_line << endl;
		if (script_renderViewPara.remark_para.flag_remark)
		{
			command_line = QString("Properties_view_remarkVisible(%1)").arg(script_renderViewPara.remark_para.flag_remark);
			stream << command_line << endl;
			//command_line = QString("Properties_view_remarkFontSize(%1)").arg(script_renderViewPara.remark_para.fontSize);
			//stream << command_line << endl;
			command_line = QString("Properties_view_remarkFontColor(%1,%2,%3)").arg(script_renderViewPara.remark_para.fontColor.red())
				.arg(script_renderViewPara.remark_para.fontColor.green())
				.arg(script_renderViewPara.remark_para.fontColor.blue());
			stream << command_line << endl;
			QString tep_str = script_renderViewPara.remark_para.text;
			QStringList tep_list = tep_str.split("\n", QString::SkipEmptyParts);
			if (tep_list.count() > 0)
			{
				tep_str = tep_list.at(0);;
				for (int k = 1; k < tep_list.count(); k++)
				{
					tep_str += "\\n"; tep_str += tep_list.at(k);
				}
			}
			command_line = QString("Properties_view_remarkText(%1)").arg(tep_str);
			stream << command_line << endl;
			command_line = QString("Properties_view_remarkPosition1(%1,%2)").arg(script_renderViewPara.remark_para.position1[0])
				.arg(script_renderViewPara.remark_para.position1[1]);
			stream << command_line << endl;
			command_line = QString("Properties_view_remarkPosition2(%1,%2)").arg(script_renderViewPara.remark_para.position2[0])
				.arg(script_renderViewPara.remark_para.position2[1]);
			stream << command_line << endl;
		}
		command_line = QString("Properties_renderView()");
		stream << command_line << endl<<endl;
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::script_set_renderViewPara(renderview_global_prop tep_para)
{
	script_renderViewPara = tep_para;
}

void PipelineObjTreeDockWidget::script_set_cameraPara(Camera_Para tep_para)
{
	script_cameraPara = tep_para;
}

void PipelineObjTreeDockWidget::editScript(QString tep_filename)
{
	QString command_line;
	flag_script_ani = false;
	if (!tep_filename.isNull())
	{
		QTreeWidgetItem *parent_item = pipelineObjTree->topLevelItem(0);
		int num_data = parent_item->childCount();
		if (num_data > 0)
		{
			editScript_start(tep_filename);
			for (int i = 0; i < num_data; i++)
			{
				QTreeWidgetItem *data_item = parent_item->child(i);
				editScript_dataSource(data_item, tep_filename);
				editScript_Properties_Zone(mTreeItems[data_item], tep_filename);
				editScript_Properties(mTreeItems[data_item], tep_filename);
				editScript_filter(data_item, tep_filename);
				editScript_scalarBarPosition(data_item, tep_filename);
			}
			editScript_Pick(tep_filename);			
			editScript_Properties_RenderView(tep_filename);
			editScript_camera(tep_filename);
			editScript_end(tep_filename);
		}
	}					
}

void PipelineObjTreeDockWidget::projectTreeItemClicked(QTreeWidgetItem *item, int column)
{
	if (item == NULL)
	{
		flag_pipelineObj_selected = false;
		mActivePiplnObj = NULL;
		return;
	}
	else if (item->data(0, Qt::UserRole + 1).toString() == "buildIn")
	{
		mTreeItems.count();
		pipelineObjTree->setCurrentItem(mBuiltinItem);
		flag_pipelineObj_selected = false;
		cur_Item = item;
		mActivePiplnObj = NULL;
		emit sig_update_filterToolBar(flag_pipelineObj_selected,mActivePiplnObj);
		emit sig_update_buildIn_propForm();
		return;
	}
	bool visiable = false;
	if (mTreeItems.contains(item))
	{
		visiable = item->checkState(0);
		PipelineObject* piplnObj = mTreeItems[item];
		if (piplnObj == nullptr) return;  //libaojun
		bool flag_change = false;
		if (piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors != visiable)
		{
			flag_change = true;
			piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors = visiable;
		}
		if (!visiable)
		{
			piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_cubeAxes = visiable;
		}
		mActivePiplnObj = piplnObj;
		if (piplnObj == NULL)
		{
			flag_pipelineObj_selected = false;
			return;
		}
		flag_pipelineObj_selected = true;
		cur_Item = item;
		flag_render = false;
		DoSelectPipelineObject(piplnObj);
		emit sig_update_actor_visible_plotForm(piplnObj,flag_change);
		emit sig_update_filterToolBar(flag_pipelineObj_selected, mActivePiplnObj);
	}
}

void PipelineObjTreeDockWidget::DoSelectPipelineObject(PipelineObject* pipeObj)
{
	QTreeWidgetItem* wdgtItem = mTreeItems.key(pipeObj, 0);
	if (wdgtItem != 0)
	{
		pipelineObjTree->setCurrentItem(wdgtItem);
		cur_Item = wdgtItem;
		flag_render = false;
		emit sig_update_pipelineObj_propForm(pipeObj);
		emit sig_update_pipelineObj_toolBar(pipeObj);
	}		
}

void PipelineObjTreeDockWidget::script_projectTreeItemClicked(PipelineObject* tep_pipelineObj)
{
	if (tep_pipelineObj == NULL)
	{
		projectTreeItemClicked(mBuiltinItem, 0);
	}
	else
	{
		QTreeWidgetItem* wdgtItem = mTreeItems.key(tep_pipelineObj, 0);
		if (wdgtItem != 0)
		{
			if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors)
				wdgtItem->setCheckState(0, Qt::Checked);
			else
				wdgtItem->setCheckState(0, Qt::Unchecked);
			projectTreeItemClicked(wdgtItem, 0);
		}
	}
	
}

//void PipelineObjTreeDockWidget::UpdateVisibilitySymbol()
//{
//	QMap<QTreeWidgetItem*, PipelineObject*>::iterator it;
//	for (it = mTreeItems.begin(); it != mTreeItems.end(); it++)
//	{
//		QTreeWidgetItem *item = it.key();
//		PipelineObject* ppobj = it.value();
//		/*PipelineObjProp* ppProp = mDesktop->GetPipelineObjProp(ppobj);
//		if (ppProp != 0)
//		{
//			bool beVis = ppProp->IsVisible();
//			item->setCheckState(0, beVis ? Qt::Checked : Qt::Unchecked);
//		}*/
//	}
//}

void PipelineObjTreeDockWidget::slot_treeItem_disable(PipelineObject* tep_pipelineObj)
{
	QTreeWidgetItem* item = mTreeItems.key(tep_pipelineObj, 0);
	if (item != 0)
	{
		if (!tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors)
			item->setCheckState(0,Qt::Unchecked);
		else
			item->setCheckState(0, Qt::Checked);
	}
	if ((tep_pipelineObj == mTreeItems[cur_Item]) && (tep_pipelineObj->GetObjectType() == dCalculator_DataSource))
	{
		emit sig_update_pipelineObj_propForm(tep_pipelineObj);
		emit sig_update_pipelineObj_toolBar(tep_pipelineObj);
	}
}

void PipelineObjTreeDockWidget::update_pipelineObjs(QList<PipelineObject*> tep_pipelineObjs)
{
	/*PipelineObject* tep_obj = NULL;
	QMap<QTreeWidgetItem*, PipelineObject*>::iterator it;
	for (int i = 0; i < tep_pipelineObjs.count(); i++)
	{
		tep_obj = tep_pipelineObjs[i];
		for (it = mTreeItems.begin(); it != mTreeItems.end(); it++)
		{
			QTreeWidgetItem *item = it.key();
			if (item->data(0, Qt::UserRole + 1) == tep_obj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			{
				mTreeItems.insert(item, tep_obj);
				break;
			}
		}
	}*/
	projectTreeItemClicked(mBuiltinItem, 0);//projectTreeItemClicked(cur_Item, 0);
}

void PipelineObjTreeDockWidget::slot_update_scalarBar(PipelineObject* tep_pipelineObj)
{	
	if (tep_pipelineObj->GetObjectType() == dNumeric_DataSource)
	{
		if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
		{
			if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle])
			{
				tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);
				tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
				for (int i = 0; i < cur_Item->childCount(); i++)
				{
					if (mTreeItems[cur_Item->child(i)] != NULL)
					{
						mTreeItems[cur_Item->child(i)]->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
					}
				}				
			}
			else
			{
				tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
				tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
			}
		}
		emit sig_update_scalarBar(tep_pipelineObj);
	}
	else
	{
		if (tep_pipelineObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.contains(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
		{
			if (tep_pipelineObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle])
			{
				tep_pipelineObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, false);
				tep_pipelineObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
				tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = false;
			}
			else
			{
				tep_pipelineObj->GetObjParent()->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.insert(tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, true);
				tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = true;
			}
		}
		emit sig_update_scalarBar(tep_pipelineObj->GetObjParent());
	}
	
}

void PipelineObjTreeDockWidget::slot_script_refresh_pickData(struct pick_pipelineObj_propData pick_Data)
{
	script_pickData.flag_pickMode = pick_Data.flag_pickMode;
	script_pickData.pickArea_objId = pick_Data.pickArea_objId;
	script_pickData.pick_actor = pick_Data.pick_actor;
	script_pickData.pick_areaCell = pick_Data.pick_areaCell;
	script_pickData.pick_areaPoint = pick_Data.pick_areaPoint;
	script_pickData.pick_id = pick_Data.pick_id;
	script_pickData.pick_pipelineObj = pick_Data.pick_pipelineObj;
}

void PipelineObjTreeDockWidget::editScript_Pick(QString fileName)
{
	if (script_pickData.pick_pipelineObj == NULL)
		return;
	if ((script_pickData.flag_pickMode < 0) || (script_pickData.flag_pickMode>3))
		return;
	if (script_pickData.pick_pipelineObj->mBaseGrid == NULL)
		return;
	int num_points = script_pickData.pick_pipelineObj->mBaseGrid->GetNumberOfPoints();
	int num_cells = script_pickData.pick_pipelineObj->mBaseGrid->GetNumberOfCells();
	if ((num_points < 1) || (num_cells < 1))
		return;
	if (script_pickData.flag_pickMode == 0)
	{
		if ((script_pickData.pick_id < 0) || (script_pickData.pick_id >= num_points))
			return;
	}
	if (script_pickData.flag_pickMode == 1)
	{
		if ((script_pickData.pick_id < 0) || (script_pickData.pick_id >= num_cells))
			return;
	}
	if ((script_pickData.flag_pickMode == 2) || (script_pickData.flag_pickMode == 3))
	{
		if ((script_pickData.pick_areaCell == NULL) || (script_pickData.pick_areaPoint == NULL))
			return;
		if ((script_pickData.pick_areaPoint->GetNumberOfIds() < 1) || (script_pickData.pick_areaCell->GetNumberOfIds() < 1))
			return;
	}
	QMap<QTreeWidgetItem*, PipelineObject*>::iterator it;
	for (it =mTreeItems.begin(); it!=mTreeItems.end(); it++)
	{
		PipelineObject* tep_obj = it.value();
		if (script_pickData.pick_pipelineObj == tep_obj)
			break;
	}
	if (it == mTreeItems.end())
		return;
	QFile tep_file1(fileName);
	if (tep_file1.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&tep_file1);
		QString command_line;
		command_line = QString("# Pick Setting");
		stream << command_line << endl;
		if (script_pickData.flag_pickMode == 0)
		{
			if (!script_pickData.flag_draw_curve)
			{
				command_line = QString("Pick_Point(%1)").arg(script_pickData.pick_id);
				stream << command_line << endl;
			}
			else
			{
				command_line = QString("Pick_PointCurve(%1)").arg(script_pickData.pick_id);
				stream << command_line << endl;
			}
		}
		else if (script_pickData.flag_pickMode == 1)
		{
			if (!script_pickData.flag_draw_curve)
			{
				command_line = QString("Pick_Cell(%1)").arg(script_pickData.pick_id);
				stream << command_line << endl;
			}
			else
			{
				command_line = QString("Pick_CellCurve(%1)").arg(script_pickData.pick_id);
				stream << command_line << endl;
			}
		}
		tep_file1.close();
	}
}

void PipelineObjTreeDockWidget::reTranslate()
{
	setWindowTitle(tr("Pipeline Browser"));
	file_add_action->setText(tr("Open"));
	file_add_action->setStatusTip(tr("open a file"));
	file_del_action->setText(tr("Delete"));
	file_del_action->setStatusTip(tr("delete a file"));
	mBuiltinItem->setText(0, tr("buildIn"));
}

int PipelineObjTreeDockWidget::getNodeNum() const
{
	return mTreeItems.size();
}

PipelineObject * PipelineObjTreeDockWidget::getNodeByIndex(int index)
{
	if (index >= mTreeItems.size()){ return nullptr; }
	return mTreeItems.values().at(index);
}

void PipelineObjTreeDockWidget::writeVRXml(QString tep_filename)
{
	int num_clip = 0;
	int num_slice = 0;
	int num_streamline = 0;
	int num_vector = 0;
	int num_reflection = 0;
	int num_calculator = 0;
	int num_smooth = 0;
	int num_contour = 0;
	if (!tep_filename.isNull())
	{
		QTreeWidgetItem *parent_item = pipelineObjTree->topLevelItem(0);
		int num_data = parent_item->childCount();
		if (num_data > 0)
		{
			QString tepstr;
			QDomDocument doc;
			QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
			doc.appendChild(instruction);
			QDomElement root = doc.createElement("DocumentElement");
			doc.appendChild(root);
			QDomElement num_dataSource = doc.createElement(QString("MeshCount"));
			QDomText text;
			text = doc.createTextNode(tepstr.setNum(num_data,10));
			num_dataSource.appendChild(text);
			root.appendChild(num_dataSource);
			for (int i = 0; i < num_data; i++)
			{
				QTreeWidgetItem *tep_item= parent_item->child(i);
				if (!mTreeItems.contains(tep_item))
					continue;
				PipelineObject* tep_obj = mTreeItems[tep_item];
				if (tep_obj == NULL)
					continue;
				QDomElement dataSource = doc.createElement(QString("FilaeAttributeAll%1").arg(i+1));
				root.appendChild(dataSource);

				QDomElement fileName = doc.createElement(QString("FilePath"));
				if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.fileList.count()>1)
				{
					QFileInfo tep_info(tep_obj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0));
					int tep_len = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).length();
					tepstr = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).left(tep_len - tep_info.suffix().length()-1)+"*";
					text = doc.createTextNode(tepstr);
				}
				else
					text = doc.createTextNode(tep_obj->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(tep_obj->mPipeLineObjProp.pipelineObj_base_propData.file_xh));
				//text = doc.createTextNode(tep_obj->mPipeLineObjProp.pipelineObj_base_propData.fileName);
				fileName.appendChild(text);
				dataSource.appendChild(fileName);
				//////////////////////////////////
				QDomElement fileXh = doc.createElement(QString("CurrentFrame"));
				text = doc.createTextNode(tepstr.setNum(tep_obj->mPipeLineObjProp.pipelineObj_base_propData.file_xh,10));
				fileXh.appendChild(text);
				dataSource.appendChild(fileXh);
				//////////////////////////////////
				QDomElement columnColor = doc.createElement(QString("AttributeName"));
				text = doc.createTextNode(tep_obj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
				columnColor.appendChild(text);
				dataSource.appendChild(columnColor);
				//////////////////////////////////
				int num_subChild=tep_item->childCount();
				if (num_subChild < 1)
					continue;
				for (int j = 0; j < num_subChild; j++)
				{
					QTreeWidgetItem *tep_subItem = tep_item->child(j);
					if (!mTreeItems.contains(tep_subItem))
						continue;
					PipelineObject* tep_subObj = mTreeItems[tep_subItem];
					if (tep_subObj == NULL)
						continue;
					Def_PipelineObjType tep_objType=tep_subObj->GetObjectType();
					if (tep_objType == dClip_DataSource)
					{
						num_clip++;
						QDomElement clipNode = doc.createElement(QString("Clip%1").arg(num_clip));
						dataSource.appendChild(clipNode);
						QDomElement origin = doc.createElement(QString("ClipOrigin"));
						tepstr = QString("Origin(%1,%2,%3)").arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_origin.x)
							.arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_origin.y)
							.arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_origin.z);
						text = doc.createTextNode(tepstr);
						origin.appendChild(text);
						clipNode.appendChild(origin);
						//////////////////////////////////
						QDomElement normal = doc.createElement(QString("ClipNormal"));
						tepstr = QString("Normal(%1,%2,%3)").arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_normal.x)
							.arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_normal.y)
							.arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_normal.z);
						text = doc.createTextNode(tepstr);
						normal.appendChild(text);
						clipNode.appendChild(normal);
					}
					else if (tep_objType == dSlice_DataSource)
					{
						num_slice++;
						QDomElement sliceNode = doc.createElement(QString("Slice%1").arg(num_slice));
						dataSource.appendChild(sliceNode);
						QDomElement origin = doc.createElement(QString("SliceOrigin"));
						tepstr = QString("Origin(%1,%2,%3)").arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_origin.x)
							.arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_origin.y)
							.arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_origin.z);
						text = doc.createTextNode(tepstr);
						origin.appendChild(text);
						sliceNode.appendChild(origin);
						//////////////////////////////////
						QDomElement normal = doc.createElement(QString("SliceNormal"));
						tepstr = QString("Normal(%1,%2,%3)").arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_normal.x)
							.arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_normal.y)
							.arg(tep_subObj->mPipeLineObjProp.plane_propData.m_plane_normal.z);
						text = doc.createTextNode(tepstr);
						normal.appendChild(text);
						sliceNode.appendChild(normal);
					}
					else if (tep_objType == dContour_DataSource)
					{
						num_contour++;
						QDomElement contourNode = doc.createElement(QString("Contour%1").arg(num_contour));
						dataSource.appendChild(contourNode);
						QDomElement columnContour = doc.createElement(QString("columnContour"));
						text = doc.createTextNode(tep_subObj->mPipeLineObjProp.contourFilter_propData.contour_colName);
						columnContour.appendChild(text);
						contourNode.appendChild(columnContour);
						QDomElement valueContour = doc.createElement(QString("valueContour"));
						for (int a = 0; a < tep_subObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.count(); a++)
						{
							if (tep_subObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.at(a).colName == tep_subObj->mPipeLineObjProp.contourFilter_propData.contour_colName)
							{
								text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.contourFilter_propData.contour_dataList.at(a).val, 'g', 6));
								valueContour.appendChild(text);
								contourNode.appendChild(valueContour);
								break;
							}
						}
						/*text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.contourFilter_propData.val,'g',6));
						valueContour.appendChild(text);
						contourNode.appendChild(valueContour);*/
					}
					else if (tep_objType == dVector_DataSource)
					{
						num_vector++;
						QDomElement vectorNode = doc.createElement(QString("Vector%1").arg(num_vector));
						dataSource.appendChild(vectorNode);
						/////////////////////////////////////
						QDomElement columnVector = doc.createElement(QString("columnVector"));
						text = doc.createTextNode(tep_subObj->mPipeLineObjProp.vectorFilter_propData.vectorColName);
						columnVector.appendChild(text);
						vectorNode.appendChild(columnVector);
						/////////////////////////////////////
						QDomElement glyphType = doc.createElement(QString("glyphType"));
						if(tep_subObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.type==0)
							text = doc.createTextNode("arrow");
						else
							text = doc.createTextNode("invalid");
						glyphType.appendChild(text);
						vectorNode.appendChild(glyphType);
						/////////////////////////////////////
						QDomElement tipResolution = doc.createElement(QString("tipResolution"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRes, 10));
						tipResolution.appendChild(text);
						vectorNode.appendChild(tipResolution);
						QDomElement tipRadius = doc.createElement(QString("tipRadius"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipRad,'g',6));
						tipRadius.appendChild(text);
						vectorNode.appendChild(tipRadius);
						QDomElement tipLength = doc.createElement(QString("tipLength"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.tipLen, 'g', 6));
						tipLength.appendChild(text);
						vectorNode.appendChild(tipLength);
						QDomElement shaftResolution = doc.createElement(QString("shaftResolution"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRes, 10));
						shaftResolution.appendChild(text);
						vectorNode.appendChild(shaftResolution);
						QDomElement shaftRadius = doc.createElement(QString("shaftRadius"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.arrow_data.shaftRad, 'g', 6));
						shaftRadius.appendChild(text);
						vectorNode.appendChild(shaftRadius);
						/////////////////////////////////////
						QDomElement scaleFactor = doc.createElement(QString("scaleFactor"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.vectorFilter_propData.glyph_data.scaleFactor, 'g',6));
						scaleFactor.appendChild(text);
						vectorNode.appendChild(scaleFactor);
					}
					else if (tep_objType == dSmooth_DataSource)
					{
						num_smooth++;
						QDomElement smoothNode = doc.createElement(QString("Smooth%1").arg(num_smooth));
						dataSource.appendChild(smoothNode);
						QDomElement flag_cellToPoint = doc.createElement(QString("flag_cellToPoint"));
						bool flag1 = tep_subObj->mPipeLineObjProp.smoothFilter_propData.flag_cellToPoint;
						if (flag1)
							text = doc.createTextNode("true");
						else
							text = doc.createTextNode("false");
						flag_cellToPoint.appendChild(text);
						smoothNode.appendChild(flag_cellToPoint);
						QDomElement flag_meshSmooth = doc.createElement(QString("flag_meshSmooth"));
						bool flag2 = tep_subObj->mPipeLineObjProp.smoothFilter_propData.flag_meshSmooth;
						if (flag2)
							text = doc.createTextNode("true");
						else
							text = doc.createTextNode("false");
						flag_meshSmooth.appendChild(text);
						smoothNode.appendChild(flag_meshSmooth);
						if (flag2)
						{
							QDomElement smooth_coef = doc.createElement(QString("smooth_coef"));
							tepstr = tepstr.setNum(tep_subObj->mPipeLineObjProp.smoothFilter_propData.smooth_coef);
							text = doc.createTextNode(tepstr);
							smooth_coef.appendChild(text);
							smoothNode.appendChild(smooth_coef);
						}
					}
					else if (tep_objType == dReflection_DataSource)
					{
						num_reflection++;
						QDomElement reflectionNode = doc.createElement(QString("Reflection%1").arg(num_reflection));
						dataSource.appendChild(reflectionNode);
						QDomElement reflection = doc.createElement(QString("reflection"));
						tepstr =tepstr.setNum((int)tep_subObj->mPipeLineObjProp.reflectionFilter_propData.reflection_axis,10);
						text = doc.createTextNode(tepstr);
						reflection.appendChild(text);
						reflectionNode.appendChild(reflection);
						
						QDomElement flag_CopyInput = doc.createElement(QString("flag_CopyInput"));
						bool flag2 = tep_subObj->mPipeLineObjProp.reflectionFilter_propData.flag_copyInput;
						if (flag2)
							text = doc.createTextNode("true");
						else
							text = doc.createTextNode("false");
						flag_CopyInput.appendChild(text);
						reflectionNode.appendChild(flag_CopyInput);
					}
					else if (tep_objType == dCalculator_DataSource)
					{
						num_calculator++;
						QDomElement calculatorNode = doc.createElement(QString("Calculator%1").arg(num_calculator));
						dataSource.appendChild(calculatorNode);
						QDomElement formula = doc.createElement(QString("formula"));
						tepstr =tep_subObj->mPipeLineObjProp.calculatorFilter_propData.formula;
						text = doc.createTextNode(tepstr);
						formula.appendChild(text);
						calculatorNode.appendChild(formula);
						QDomElement resultName = doc.createElement(QString("resultName"));
						tepstr = tep_subObj->mPipeLineObjProp.calculatorFilter_propData.resultName;
						text = doc.createTextNode(tepstr);
						resultName.appendChild(text);
						calculatorNode.appendChild(resultName);
					}
					else if (tep_objType == dStreamLine_DataSource)
					{
						num_streamline++;
						QDomElement streamNode = doc.createElement(QString("FilamentLine%1").arg(num_streamline));
						dataSource.appendChild(streamNode);
						/////////////////////////////////////
						QDomElement columnVector = doc.createElement(QString("AttributeName"));
						text = doc.createTextNode(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.vectorColName);
						columnVector.appendChild(text);
						streamNode.appendChild(columnVector);
						/////////////////////////////////////
						QDomElement columnColor = doc.createElement(QString("StreamlineColoring"));
						text = doc.createTextNode(tep_subObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
						columnColor.appendChild(text);
						streamNode.appendChild(columnColor);
						/////////////////////////////////////
						QDomElement InterpolatorType = doc.createElement(QString("InterpolatorType"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.interpolator_type, 10));
						InterpolatorType.appendChild(text);
						streamNode.appendChild(InterpolatorType);
						/////////////////////////////////////
						QDomElement surface_streamLines = doc.createElement(QString("surface_streamLines"));
						if (tep_subObj->mPipeLineObjProp.streamLineFilter_propData.surface_streamLines)
							text = doc.createTextNode("true");
						else
							text = doc.createTextNode("false");
						surface_streamLines.appendChild(text);
						streamNode.appendChild(surface_streamLines);
						/////////////////////////////////////
						QDomElement IntegrationDirection = doc.createElement(QString("IntegrationDirection"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.direction, 10));
						IntegrationDirection.appendChild(text);
						streamNode.appendChild(IntegrationDirection);
						/////////////////////////////////////
						QDomElement IntegratorType = doc.createElement(QString("IntegratorType"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.type, 10));
						IntegratorType.appendChild(text);
						streamNode.appendChild(IntegratorType);
						/////////////////////////////////////
						QDomElement IntegrationStepUnit = doc.createElement(QString("IntegrationStepUnit"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.stepUnit, 10));
						IntegrationStepUnit.appendChild(text);
						streamNode.appendChild(IntegrationStepUnit);
						/////////////////////////////////////
						QDomElement initStepLen = doc.createElement(QString("initStepLen"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.initStepLen, 'f', 6));
						initStepLen.appendChild(text);
						streamNode.appendChild(initStepLen);
						/////////////////////////////////////
						QDomElement miniStepLen = doc.createElement(QString("miniStepLen"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.miniStepLen, 'f', 6));
						miniStepLen.appendChild(text);
						streamNode.appendChild(miniStepLen);
						/////////////////////////////////////
						QDomElement maxiStepLen = doc.createElement(QString("maxiStepLen"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.integration_data.maxiStepLen, 'f', 6));
						maxiStepLen.appendChild(text);
						streamNode.appendChild(maxiStepLen);
						/////////////////////////////////////
						QDomElement maxiSteps = doc.createElement(QString("maxiSteps"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiSteps, 10));
						maxiSteps.appendChild(text);
						streamNode.appendChild(maxiSteps);
						/////////////////////////////////////
						QDomElement terminalSpeed = doc.createElement(QString("terminalSpeed"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.terminalSpeed, 'gf', 6));
						terminalSpeed.appendChild(text);
						streamNode.appendChild(terminalSpeed);
						/////////////////////////////////////
						QDomElement maxiError = doc.createElement(QString("maxiError"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.stream_data.maxiError, 'g', 6));
						maxiError.appendChild(text);
						streamNode.appendChild(maxiError);
						/////////////////////////////////////
						QDomElement vorticity = doc.createElement(QString("vorticity"));
						if (tep_subObj->mPipeLineObjProp.streamLineFilter_propData.vorticity)
							text = doc.createTextNode("true");
						else
							text = doc.createTextNode("false");
						vorticity.appendChild(text);
						streamNode.appendChild(vorticity);
						/////////////////////////////////////
						QDomElement Seedstype = doc.createElement(QString("Seedstype"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.type, 10));
						Seedstype.appendChild(text);
						streamNode.appendChild(Seedstype);
						QDomElement mPoint = doc.createElement(QString("mPoint"));
						tepstr = QString("FVector(%1,%2,%3)").arg(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.x)
							.arg(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.y)
							.arg(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.mPoint.z);
						text = doc.createTextNode(tepstr);
						mPoint.appendChild(text);
						streamNode.appendChild(mPoint);
						/////////////////////////////////////
						QDomElement num_points = doc.createElement(QString("num_points"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.num_points, 10));
						num_points.appendChild(text);
						streamNode.appendChild(num_points);
						/////////////////////////////////////
						QDomElement radius = doc.createElement(QString("radius"));
						text = doc.createTextNode(tepstr.setNum(tep_subObj->mPipeLineObjProp.streamLineFilter_propData.seeds_data.radius, 'f', 6));
						radius.appendChild(text);
						streamNode.appendChild(radius);
					}
					else
					{
						continue;
					}
				}
			}
			QFile file(tep_filename);
			if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
				return;
			QTextStream out(&file);
			doc.save(out, 4);
			file.close();
		}
	}	
}
