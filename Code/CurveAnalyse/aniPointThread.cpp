#include "aniPointThread.h"
#include <QTime>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>

aniPointThread::aniPointThread()
{
	cur_frame_xh = 0;
	cur_max_frames = 0;
	curveDataPtr = NULL;
}

aniPointThread::~aniPointThread()
{
	while (flag_finish)
		break;
}

void aniPointThread::initThread(curve_ini_data *tep_data,int num_frames, int frame_xh)
{
	cur_frame_xh = frame_xh;
	stopped = false;
	curveDataPtr = tep_data;
	aniPoint_para.axis_indexList.clear();
	aniPoint_para.aniPoint_IdList.clear();
	aniPoint_para.aniPoint_CoorList.clear();
	aniPoint_para.aniPoint_ColorList.clear();
	aniPoint_para.aniPoint_TypeList.clear();
	aniPoint_para.aniPoint_CurveNameList.clear();
	cur_max_frames = num_frames;
	flag_finish = false;
}

void aniPointThread::stop()
{
    stopped=true;
}
double aniPointThread::func_getAniPoint_coorX(curve_line_data tep_line_data,int tep_xh)
{
	double coor_x = 0;
	if ((tep_line_data.axis_index < 0) || (tep_line_data.axis_index >1))
		return coor_x;
	if (curveDataPtr->glo_curve_data.glo_xAxis_prop_data[tep_line_data.axis_index].axis_dataSource == plotProp_dataSource::File)
	{
		int x_fileIndex = curveDataPtr->glo_curve_data.glo_xAxis_prop_data[tep_line_data.axis_index].axis_fileIndex;
		int x_columnIndex = curveDataPtr->glo_curve_data.glo_xAxis_prop_data[tep_line_data.axis_index].axis_fileColumn;
		if ((x_fileIndex >= curveDataPtr->curve_file_list.count()) || (x_fileIndex < 0))
			return coor_x;
		curve_file_data tep_xFileData = curveDataPtr->curve_file_list.at(x_fileIndex);
		if ((x_columnIndex >= tep_xFileData.file_prop.num_column) || (x_columnIndex < 0))
			return coor_x;
		QVector<double> coor_x_array = tep_xFileData.file_prop.column_val_data[x_columnIndex];
		if (cur_frame_xh<coor_x_array.count())
			coor_x = coor_x_array.at(cur_frame_xh);
		else
			coor_x = coor_x_array.last();
	}
	else
		coor_x = tep_xh*1.0;
	return coor_x;
}
void aniPointThread::run()
{
	QString tep_filename;
	QString tep_str;
	first_frame_xh = cur_frame_xh;
	if ((cur_frame_xh + 1) >= cur_max_frames)
		return;
	int num_files = curveDataPtr->curve_file_list.count();
	int num_curves;
	int tep_column_index;
	double coor_x, coor_y;
	QVector<double> coor_x_array;
	for (int i = cur_frame_xh; i < cur_max_frames; i++)
	{
		if (stopped)
		{
			break;
		}
		g_aniThread_mutex.lock();
		if(i==first_frame_xh)
		{
			for (int i = 0; i < num_files; i++)
			{
				curve_file_data tep_file_data = curveDataPtr->curve_file_list.at(i);
				num_curves = tep_file_data.line_data.count();
				if ((cur_frame_xh + 1) < tep_file_data.file_prop.num_point)
				{
					coor_x = cur_frame_xh*1.0;
					for (int j = 0; j < num_curves; j++)
					{
						curve_line_data tep_line_data = tep_file_data.line_data.at(j);
						coor_x = func_getAniPoint_coorX(tep_line_data, cur_frame_xh);
						tep_column_index = tep_line_data.yAxis_column;
						coor_y = tep_file_data.file_prop.column_val_data[tep_column_index].at(cur_frame_xh);
						QString tepId = QString("curve_%1_%2").arg(i).arg(tep_column_index);
						aniPoint_para.axis_indexList.append(tep_line_data.axis_index);
						aniPoint_para.aniPoint_IdList.append(tepId);
						aniPoint_para.aniPoint_CoorList.append(QPointF(coor_x, coor_y));
						aniPoint_para.aniPoint_ColorList.append(tep_line_data.aniPoint_color);
						aniPoint_para.aniPoint_TypeList.append(tep_line_data.aniPoint_type);
						aniPoint_para.aniPoint_CurveNameList.append(tep_line_data.yAxis_column_name + "Ani");
					}
				}
				else
				{
					coor_x = (tep_file_data.file_prop.num_point - 1)*1.0;
					for (int j = 0; j < num_curves; j++)
					{
						curve_line_data tep_line_data = tep_file_data.line_data.at(j);
						coor_x = func_getAniPoint_coorX(tep_line_data, cur_frame_xh);
						tep_column_index = tep_line_data.yAxis_column;
						coor_y = tep_file_data.file_prop.column_val_data[tep_column_index].last();
						QString tepId = QString("curve_%1_%2").arg(i).arg(tep_column_index);
						aniPoint_para.axis_indexList.append(tep_line_data.axis_index);
						aniPoint_para.aniPoint_IdList.append(tepId);
						aniPoint_para.aniPoint_CoorList.append(QPointF(coor_x, coor_y));
						aniPoint_para.aniPoint_ColorList.append(tep_line_data.aniPoint_color);
						aniPoint_para.aniPoint_TypeList.append(tep_line_data.aniPoint_type);
						aniPoint_para.aniPoint_CurveNameList.append(tep_line_data.yAxis_column_name + "Ani");
					}
				}
			}
			if (stopped)
			{
				g_aniThread_mutex.unlock();
				break;
			}
			else
				emit sig_addAniPoint(cur_frame_xh, first_frame_xh, cur_max_frames);
		}
		else
		{
			for (int i = 0; i < num_files; i++)
			{
				curve_file_data tep_file_data = curveDataPtr->curve_file_list.at(i);
				num_curves = tep_file_data.line_data.count();
				if ((cur_frame_xh + 1) < tep_file_data.file_prop.num_point)
				{
					coor_x = cur_frame_xh*1.0;
					for (int j = 0; j < num_curves; j++)
					{
						curve_line_data tep_line_data = tep_file_data.line_data.at(j);
						coor_x = func_getAniPoint_coorX(tep_line_data, cur_frame_xh);
						tep_column_index = tep_line_data.yAxis_column;
						coor_y = tep_file_data.file_prop.column_val_data[tep_column_index].at(cur_frame_xh);
						aniPoint_para.aniPoint_CoorList.replace(i*num_curves + j, QPointF(coor_x, coor_y));
					}
				}
			}
			if (stopped)
			{
				g_aniThread_mutex.unlock();
				break;
			}
			else
				emit sig_updateAniPoint(cur_frame_xh, first_frame_xh, cur_max_frames);
		}		
		cur_frame_xh++;
	}
	emit sig_finish_aniThread(true);
	flag_finish = true;
}
