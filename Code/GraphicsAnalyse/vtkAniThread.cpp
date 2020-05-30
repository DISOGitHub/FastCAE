#include "vtkAniThread.h"
#include <QTime>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>

vtkAniThread::vtkAniThread()
{
	cur_frame_xh = 0;
	cur_max_frames = 0;
	cur_dis_xh = 0;
}

vtkAniThread::~vtkAniThread()
{
}

void vtkAniThread::initThread(QList<PipelineObject*> mPipelineObjs, int max_frames, int frame_xh)
{
	cur_max_frames = max_frames;
	cur_frame_xh = frame_xh;
	cur_dis_xh = frame_xh;
	stopped = false;
}

void vtkAniThread::stop()
{
    stopped=true;
}

PipelineObject* vtkAniThread::findParentPipelineObj(QString tep_filename, QList<PipelineObject*> tep_mPipelineObjs)
{
	for (int i = 0; i < tep_mPipelineObjs.count(); i++)
	{
		if (tep_filename == tep_mPipelineObjs[i]->mPipeLineObjProp.pipelineObj_base_propData.fileName)
		{
			return tep_mPipelineObjs[i];
		}
			
	}
	return NULL;
}

void vtkAniThread::slot_update_scalarBar(PipelineObject *tep_pipelineObj)
{
	if (tep_pipelineObj->GetObjectType() != dNumeric_DataSource)
		return;
	QString tep_colorColumn;
	vtkSmartPointer<vtkScalarBarWidget> tep_scalarWidget = NULL;

	QMap<QString, bool>::iterator it;
	for (it = tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.begin(); it != tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.end(); it++)
	{
		if (tep_pipelineObj->scalarBar_widgetMap.contains(it.key()))
		{
			tep_scalarWidget = tep_pipelineObj->scalarBar_widgetMap[it.key()];

			if (it.value())
			{
				//动画颜色条范围变化
				QString tep_colorColumn = it.key();
				double mRange[2];
				tep_pipelineObj->getcolorColumnRange(tep_colorColumn, mRange);
				tep_scalarWidget->GetScalarBarActor()->GetLookupTable()->SetRange(mRange);
				//动画颜色条范围变化
			}
		}
	}
}

void vtkAniThread::initModelData(NumericSrcObject* tep_numericObj)
{
	if (tep_numericObj->mBaseGridBak == NULL)
		return;	
	tep_numericObj->origin_center0[0] = tep_numericObj->origin_center[0]; 
	tep_numericObj->origin_center0[1] = tep_numericObj->origin_center[1];
	tep_numericObj->origin_center0[2] = tep_numericObj->origin_center[2];
	double *tep_point;
	for (vtkIdType i = 0; i < tep_numericObj->mBaseGridBak->GetNumberOfPoints(); i++)
	{
		tep_point = tep_numericObj->mBaseGridBak->GetPoint(i);
		if ((fabs(tep_point[1] - tep_numericObj->origin_center[1]) <= 0.0001) && (fabs(tep_point[2] - tep_numericObj->origin_center[2]) <= 0.0001) && ((tep_point[0] - tep_numericObj->origin_center[0]) > 0.0001))
		{
			tep_numericObj->refrence_point_for_objID = i;
			tep_numericObj->refrence_point_for_obj[0] = tep_point[0];
			tep_numericObj->refrence_point_for_obj[1] = tep_point[1];
			tep_numericObj->refrence_point_for_obj[2] = tep_point[2];
			break;
		}
	}
}

void vtkAniThread::run()
{
	QString tep_filename;
	QString tep_str;
	first_frame_xh = cur_frame_xh;
	LoadActionFile* ptr = new LoadActionFile;
	num_maxDisplace = ptr->func_get_max_num_displace(g_ani_objList);
	delete ptr;
	ptr = NULL;
	for (int i = cur_frame_xh; i < cur_max_frames; i++)
	{
		if (stopped)
		{
			return;
		}
		g_aniThread_mutex.lock();
		source_mPipelineObjs.clear();
		for (int j = 0; j < g_ani_objList.count(); j++)
		{
			if (g_ani_objList[j]->GetObjectType() == dNumeric_DataSource)
			{
				if ((g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > cur_frame_xh) && 
					g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileListBak.count()>1)
				{
					tep_filename = g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(cur_frame_xh);
					NumericSrcObject*	cur_NumericSrcObj = new NumericSrcObject(g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList, cur_frame_xh);
					if (cur_NumericSrcObj->readFile(tep_filename))
					{
						cur_NumericSrcObj->mBaseGridBak->GetCenter(cur_NumericSrcObj->origin_center);
						//cur_NumericSrcObj->origin_center[2] = cur_NumericSrcObj->mBaseGridBak->GetBounds()[5];
						bool flag_generate_lookupTable = false;
						if (i!=first_frame_xh)
						{
							if (g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle != "SolidColor")
							{
								vtkColorTransferFunction* tep_colorTransferFunc = g_ani_objList[j]->getLookupTable(g_ani_objList[j]->mBaseGridBak, g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
								double tep_range[2]; tep_colorTransferFunc->GetRange(tep_range);
								if ((tep_range[0]+0.00001) == tep_range[1])
									flag_generate_lookupTable = true;
								/*else
								{
									double tep_range[2];
									if (cur_NumericSrcObj->get_column_range(cur_NumericSrcObj->mBaseGrid, glo_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle, tep_range))
										glo_ani_objList[j]->getLookupTable(glo_ani_objList[j]->mBaseGrid, glo_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle)->AdjustRange(tep_range);
								}*/ //20180227
							}
						}
						else
						{
							initModelData(cur_NumericSrcObj);
							for (int m = 0; m < 3; m++)
							{
								((NumericSrcObject*)g_ani_objList[j])->origin_center[m] = cur_NumericSrcObj->origin_center[m];
								((NumericSrcObject*)g_ani_objList[j])->origin_center0[m] = cur_NumericSrcObj->origin_center0[m];
								((NumericSrcObject*)g_ani_objList[j])->refrence_point_for_obj[m] = cur_NumericSrcObj->refrence_point_for_obj[m];								
							}
							((NumericSrcObject*)g_ani_objList[j])->refrence_point_for_objID = cur_NumericSrcObj->refrence_point_for_objID;
							if (((NumericSrcObject*)g_ani_objList[j])->model_ptr != NULL)
							{
								((NumericSrcObject*)g_ani_objList[j])->model_ptr->SetOrigin(cur_NumericSrcObj->origin_center0[0], cur_NumericSrcObj->origin_center0[1], cur_NumericSrcObj->origin_center0[2]);
							}
						}
						//add for multi zone
						
						cur_NumericSrcObj->mPipeLineObjProp.pipelineObj_base_propData.block_showMap = g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.block_showMap;
						cur_NumericSrcObj->flag_update_mBaseGrid = true;
						cur_NumericSrcObj->func_update_mBaseGrid();
						//add for multi zone
						if (g_ani_objList[j]->mBaseGrid != NULL)
						{
							g_ani_objList[j]->mBaseGrid->Delete();
						}
						if (g_ani_objList[j]->mBaseGridBak != NULL)
						{
							g_ani_objList[j]->mBaseGridBak->Delete();
						}
						if (g_ani_objList[j]->mBlockDataSet != NULL)
						{
							g_ani_objList[j]->mBlockDataSet->Delete();
						}
						g_ani_objList[j]->mBaseGrid = cur_NumericSrcObj->mBaseGrid;
						g_ani_objList[j]->mBaseGridBak = cur_NumericSrcObj->mBaseGridBak;
						g_ani_objList[j]->idTypeBlockDataSet.clear();
						g_ani_objList[j]->idTypeBlockDataSet = cur_NumericSrcObj->idTypeBlockDataSet;
						g_ani_objList[j]->mBlockDataSet = cur_NumericSrcObj->mBlockDataSet;
						g_ani_objList[j]->dataSetMap = cur_NumericSrcObj->dataSetMap;
						g_ani_objList[j]->dataSetNameMap = cur_NumericSrcObj->dataSetNameMap;
						if (flag_generate_lookupTable)
							g_ani_objList[j]->generate_scalarBar_lookupTableMap(g_ani_objList[j]->mBaseGrid);
						QStringList tep_list1 = g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.at(0).split("/"); 
						int tep_count = tep_list1.count();
						if (tep_count > 0)
						{
							g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileName = tep_list1.at(tep_count - 1);
							g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.file_xh = cur_frame_xh;
						}
					}
					delete cur_NumericSrcObj;
					cur_NumericSrcObj = NULL;
				}
				source_mPipelineObjs.append(g_ani_objList[j]);
				LoadActionFile* ptr = new LoadActionFile;
				ptr->func_setPipeObjDisplaceData(g_ani_objList[j], cur_dis_xh, g_ani_objList);
				delete ptr;
				ptr = NULL;
			}
		}
		for (int j = 0; j < g_ani_objList.count(); j++)
		{
			if ((g_ani_objList[j]->GetObjectType() == dSlice_DataSource) || (g_ani_objList[j]->GetObjectType() == dClip_DataSource)
				|| (g_ani_objList[j]->GetObjectType() == dContour_DataSource) || (g_ani_objList[j]->GetObjectType() == dVector_DataSource)				
				|| (g_ani_objList[j]->GetObjectType() == dReflection_DataSource) || (g_ani_objList[j]->GetObjectType() == dSmooth_DataSource)
				|| (g_ani_objList[j]->GetObjectType() == dCalculator_DataSource) || (g_ani_objList[j]->GetObjectType() == dStreamLine_DataSource))
			{
				if (g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileList.count() > cur_frame_xh)
				{
					tep_filename = g_ani_objList[j]->mPipeLineObjProp.pipelineObj_base_propData.fileName;
					PipelineObject *tep_pipelineObj = findParentPipelineObj(tep_filename, source_mPipelineObjs);
					if (tep_pipelineObj != NULL)
					{
						g_ani_objList[j]->mBaseGrid = tep_pipelineObj->mBaseGrid;
					}
				}
			}
		}
		source_mPipelineObjs.clear();
		if (stopped)
		{
			g_aniThread_mutex.unlock();
			return;
		}
		emit sig_refreshViewAni(cur_frame_xh);
		cur_frame_xh++;
		cur_dis_xh++;
	}
	if (cur_max_frames == 1)
	{
		bool flag = false;
		while (1)
		{
			g_aniThread_mutex.lock();
			for (int j = 0; j < g_ani_objList.count(); j++)
			{
				if (g_ani_objList[j]->GetObjectType() == dNumeric_DataSource)
				{
					LoadActionFile* ptr = new LoadActionFile;
					if (ptr->func_setPipeObjDisplaceData(g_ani_objList[j], cur_frame_xh, g_ani_objList))
						flag = true;
					delete ptr;
					ptr = NULL;
				}
				if (stopped)
				{
					g_aniThread_mutex.unlock();
					return;
				}
			}
			if (flag)
			{

				emit sig_refreshViewAni(cur_frame_xh);
				flag = false;
			}
			else
			{
				g_aniThread_mutex.unlock();
				break;
			}
			cur_frame_xh++;
		}
	}
	/*while (cur_frame_xh < num_maxDisplace)
	{
		if (stopped)
		{
			return;
		}
		g_aniThread_mutex.lock();
		runDisplace(cur_frame_xh);
		if (stopped)
		{
			g_aniThread_mutex.unlock();
			return;
		}
		emit sig_refreshViewAni();
		cur_frame_xh++;
	}*/
	emit sig_finish_aniThread(true);
}
