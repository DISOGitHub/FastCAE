#include "readdatafileprop.h"

readDataFileProp::readDataFileProp(QObject *parent) : QObject(parent)
{
    codec = QTextCodec::codecForName("GBK");
}

bool readDataFileProp::initNewFileProp(QString fileName,curve_file_prop *tep_file_prop)
{

	bool ok; 
	
    double tep_val;
    QStringList col_list;
    QString line_str;
	if (fileName[0] == '.')
	{
		QString tep = QCoreApplication::applicationDirPath();
		fileName.remove(0, 1);
		tep += fileName;
		fileName = tep;
	}
    QFile file_y(fileName);
    if(!file_y.exists())
		return false;
	
    if (!file_y.open(QIODevice::ReadOnly))
        return false;
    tep_file_prop->Axis_filename=fileName;
    col_list.clear();
    while(!file_y.atEnd())
    {
        line_str=codec->toUnicode(file_y.readLine().trimmed());
        if(line_str!="")
        {
            col_list = line_str.split(" ",QString::SkipEmptyParts);
            if(col_list.count()<1)
                return false;
			if (col_list[0][0].toLatin1() >= '0' && col_list[0][0] <= '9')
			{
				file_y.close();
				col_list.clear();
				for (size_t i = 0; i < line_str.split(" ", QString::SkipEmptyParts).size(); i++)
				{
					//QObject::tr("project");
					col_list.append(QString("column %1").arg(i + 1));
				}
				if (!file_y.open(QIODevice::ReadOnly))
					return false;
			}
			tep_file_prop->columns_name_list = col_list;
            break;
        }
    }
    tep_file_prop->num_column=0;
    tep_file_prop->num_point=0;
    for(int j=0;j<tep_file_prop->columns_name_list.count();j++)
    {
		tep_file_prop->column_val_data[j].clear();
    }
    int count_point=-1;
    while(!file_y.atEnd())
    {
        line_str = file_y.readLine().trimmed();
        if(line_str!="")
        {
            col_list = line_str.split(" ",QString::SkipEmptyParts);
			if (col_list.count() > MAX_COL_NUM)
				return false;
            //if(col_list.count()!=tep_file_prop->columns_name_list.count())
            //    return false;
            count_point++;
            for(int j=0;j<col_list.count();j++)
            {
                tep_val=col_list.at(j).trimmed().toDouble(&ok);
				if (count_point == 0)
				{
					tep_file_prop->val_min[j] = tep_val;
					tep_file_prop->val_max[j] = tep_val;
				}
				else
				{
					if (tep_val<tep_file_prop->val_min[j])
						tep_file_prop->val_min[j] = tep_val;
					if (tep_val>tep_file_prop->val_max[j])
						tep_file_prop->val_max[j] = tep_val;
				}
                
				tep_file_prop->column_val_data[j].append(tep_val);//tep_file_prop->column_val_data[j][count_point]=tep_val;
            }
        }
    }
    tep_file_prop->num_point=count_point+1;
    if(tep_file_prop->num_point<2)
        return false;
    tep_file_prop->num_column=tep_file_prop->columns_name_list.count();
    return true;
}
