#include "LoadActionFile.h"

#include <qtextstream.h>
#include <qfile.h>
#include "qiodevice.h"
#include "LoadActionFile.h"
LoadActionFile::LoadActionFile()
{
}


LoadActionFile::~LoadActionFile()
{
}
bool LoadActionFile::LoadCSV(QString path)
{
	QFile file1(path);
	if (!file1.open(QIODevice::ReadOnly))
		return false;
	QTextStream stream(&file1);
	FrameDisplace val;
	displaceList.clear();
	QString strLine;
	QStringList strList;
	bool ok;
	strLine=stream.readAll();
	strList = strLine.trimmed().split(",", QString::SkipEmptyParts);
	int num = strList.count();
	if (num < 6)
		return false;
	int num_dis = num / 6;
	for (int i = 0; i < num_dis; i++)
	{
		val.time = 0.0;
		val.Surge_GX = strList.at(i*6).toFloat(&ok);
		val.Sway_GY = strList.at(i*6+1).toFloat(&ok);
		val.Heave_GZ = strList.at(i * 6 + 2).toFloat(&ok);
		val.Roll_Gxb = strList.at(i * 6 + 3).toFloat(&ok);
		val.Pitch_Gyb = strList.at(i * 6 + 4).toFloat(&ok);
		val.Yaw_Gzb = strList.at(i * 6 + 5).toFloat(&ok);
		displaceList.append(val);
	}
	return true;
}
bool LoadActionFile::Load(QString path, QString Id_str)
{
	QFile file1(path);
	if (!file1.open(QIODevice::ReadOnly))
		return false;
	QTextStream stream(&file1);
	FrameDisplace val;
	displaceList.clear();
	QString strLine;
	QStringList strList;
	bool ok;
	while (!stream.atEnd())
	{
		strLine = stream.readLine().trimmed();
		strLine.replace('\t', ' ');
		if (strLine.isEmpty())
			continue;
		//if (strLine.indexOf(Id_str) >= 0)
		//{
			strList=strLine.split(" ", QString::SkipEmptyParts);
			if (strList.count() == 7)
				break;
			else
				return false;
		//}
	}
	while (!stream.atEnd())
	{
		strLine = stream.readLine().trimmed();
		strLine.replace('\t', ' ');
		if (strLine.isEmpty())
			continue;
		strList = strLine.split(" ", QString::SkipEmptyParts);
		if (strList.count() == 7)
		{
			val.time = strList.at(0).toFloat(&ok);
			val.Surge_GX = strList.at(1).toFloat(&ok);
			val.Sway_GY = strList.at(2).toFloat(&ok);
			val.Heave_GZ = strList.at(3).toFloat(&ok);
			val.Roll_Gxb = strList.at(4).toFloat(&ok);
			val.Pitch_Gyb = strList.at(5).toFloat(&ok);
			val.Yaw_Gzb = strList.at(6).toFloat(&ok);
			displaceList.append(val);
		}
		else
			return false;
	}
	if (displaceList.count() < 1)
		return false;
	return true;
}
bool LoadActionFile::func_setPipeObjDisplaceData(PipelineObject *tep_pipelineObj, int tep_cur_frame_xh, QList<PipelineObject*>objList)
{
	bool flag = false;
	if (tep_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.fileListBak.count() != 1)
		return flag;
	QString tep_filename;
	QString tep_str;
	int new_pos;
	if (tep_pipelineObj->GetObjectType() == dNumeric_DataSource)
	{
		if (tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile)
		{
			tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace = false;
			if (tep_cur_frame_xh == 0)
				new_pos = 0;
			else
				new_pos = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.start_pos + tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.interval*tep_cur_frame_xh+1;
			if (tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count() > new_pos)
			{
				/*tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.time = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Surge_GX = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Sway_GY = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Heave_GZ = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ;*/
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).time;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Roll_Gxb;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Pitch_Gyb;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Yaw_Gzb;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Surge_GX;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Sway_GY;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Heave_GZ;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace = true;
				tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.cur_pos = new_pos;
				if (new_pos!=0)
					tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.dis_xh++;
				flag = true;
			}
			return flag;
		}
		return flag;
	}
	else
		return flag;
	for (int j = 0; j < objList.count(); j++)
	{
		if ((objList[j]->GetObjectType() == dSlice_DataSource) || (objList[j]->GetObjectType() == dClip_DataSource)
			|| (objList[j]->GetObjectType() == dContour_DataSource) || (objList[j]->GetObjectType() == dVector_DataSource)
			|| (objList[j]->GetObjectType() == dReflection_DataSource) || (objList[j]->GetObjectType() == dSmooth_DataSource)
			|| (objList[j]->GetObjectType() == dCalculator_DataSource) || (objList[j]->GetObjectType() == dStreamLine_DataSource))
		{
			if ((objList[j] != NULL) && (objList[j]->GetObjParent() == tep_pipelineObj))
			{
				if (tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace)
				{
					/*g_ani_objList[j]->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.time = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time;
					g_ani_objList[j]->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Roll_Gxb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb;
					g_ani_objList[j]->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Pitch_Gyb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb;
					g_ani_objList[j]->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Yaw_Gzb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb;
					g_ani_objList[j]->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Surge_GX = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX;
					g_ani_objList[j]->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Sway_GY = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY;
					g_ani_objList[j]->mPipeLineObjProp.aniDisplace_propData.m_pre_displace.Heave_GZ = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ;
					*/
					objList[j]->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.time = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).time;
					objList[j]->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Roll_Gxb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Roll_Gxb;
					objList[j]->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Pitch_Gyb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Pitch_Gyb;
					objList[j]->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Yaw_Gzb = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Yaw_Gzb;
					objList[j]->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Surge_GX = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Surge_GX;
					objList[j]->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Sway_GY = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Sway_GY;
					objList[j]->mPipeLineObjProp.aniDisplace_propData.m_cur_displace.Heave_GZ = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.at(new_pos).Heave_GZ;
					objList[j]->mPipeLineObjProp.aniDisplace_propData.cur_pos = new_pos;
					if (new_pos != 0)
						objList[j]->mPipeLineObjProp.aniDisplace_propData.dis_xh++;
					objList[j]->mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace = tep_pipelineObj->mPipeLineObjProp.aniDisplace_propData.flag_exeDisplace;
				}
			}
		}
	}
	return flag;
}
int LoadActionFile::func_get_max_num_displace(QList<PipelineObject*> tep_mPipelineObjs)
{
	int num = 0;
	QString tep_filename;
	QString tep_str;
	for (int j = 0; j < tep_mPipelineObjs.count(); j++)
	{
		if (tep_mPipelineObjs[j]->GetObjectType() == dNumeric_DataSource)
		{
			if (tep_mPipelineObjs[j]->mPipeLineObjProp.aniDisplace_propData.flag_AniDisplaceFile)
			{
				if (tep_mPipelineObjs[j]->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count()>num)
					num = tep_mPipelineObjs[j]->mPipeLineObjProp.aniDisplace_propData.m_aniDisplaceList.count();
			}
		}
	}
	return num;
}