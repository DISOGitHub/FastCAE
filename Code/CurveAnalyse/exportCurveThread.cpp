#include "exportCurveThread.h"
#include <QTime>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>

ExportCurveThread::ExportCurveThread()
{
	stopped = false;
	curve_data_ptr = NULL;
}

ExportCurveThread::~ExportCurveThread()
{
	
}

void ExportCurveThread::initThread(curve_ini_data *curve_data, QString strDir)
{
	stopped = false;
	curve_data_ptr = curve_data;
	dirName = strDir;
}

void ExportCurveThread::stop()
{
    stopped=true;
}

void ExportCurveThread::run()
{
	int num_files = curve_data_ptr->curve_file_list.count();
	int num_curves;
	curve_file_data tep_file_prop;
	curve_line_data tep_curve_prop;
	global_plot_axis_prop* x_axis_prop_ptr = NULL;
	QVector<double> x_data;
	QVector<double> y_data;
	QString x_name, y_name, tep_filename, tep_filename0;
	QString line_str, tep_str;
	for (int i = 0; i < num_files; i++)
	{
		tep_file_prop = curve_data_ptr->curve_file_list.at(i);
		tep_filename0 = QFileInfo(tep_file_prop.file_prop.Axis_filename).fileName();
		tep_filename0 = tep_filename0.left(tep_filename0.length() - 4);
		num_curves = tep_file_prop.line_data.count();
		if (num_curves < 1)
			continue;
		for (int j = 0; j < num_curves; j++)
		{
			tep_curve_prop = tep_file_prop.line_data.at(j);
			y_data = tep_file_prop.file_prop.column_val_data[tep_curve_prop.yAxis_column];
			y_name = tep_file_prop.file_prop.columns_name_list.at(tep_curve_prop.yAxis_column);
			tep_filename = dirName + "/"; tep_filename += tep_filename0; tep_filename += QString("_%1.dat").arg(tep_curve_prop.yAxis_column);
			QFile tep_file(tep_filename);
			if (!tep_file.open(QIODevice::WriteOnly))
				continue;
			QTextStream ts(&tep_file);
			if ((tep_curve_prop.axis_index != 0) && (tep_curve_prop.axis_index != 1))
				continue;
			x_axis_prop_ptr = &(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[tep_curve_prop.axis_index]);
			if (stopped)
			{
				tep_file.close();
				return;
			}
			if (x_axis_prop_ptr->axis_dataSource == plotProp_dataSource::File)
			{
				x_data = curve_data_ptr->curve_file_list.at(x_axis_prop_ptr->axis_fileIndex).file_prop.column_val_data[x_axis_prop_ptr->axis_fileColumn];
				x_name = curve_data_ptr->curve_file_list.at(x_axis_prop_ptr->axis_fileIndex).file_prop.columns_name_list.at(x_axis_prop_ptr->axis_fileColumn);
				if (x_data.count() < y_data.count())
				{
					tep_file.close();
					continue;
				}
				line_str = x_name.trimmed().leftJustified(20, ' ');
				line_str += y_name.trimmed().leftJustified(20, ' ');
				ts << line_str << endl;
				for (int k = 0; k < y_data.count(); k++)
				{
					line_str = tep_str.setNum(x_data.at(k), 'g', 6).leftJustified(20, ' ');
					line_str += tep_str.setNum(y_data.at(k), 'g', 6).leftJustified(20, ' ');
					ts << line_str << endl;
				}
			}
			else
			{
				x_name = "index";
				line_str = x_name.trimmed().leftJustified(20, ' ');
				line_str += y_name.trimmed().leftJustified(20, ' ');
				ts << line_str << endl;
				for (int k = 0; k < y_data.count(); k++)
				{
					line_str = tep_str.setNum(k, 10).leftJustified(20, ' ');
					line_str += tep_str.setNum(y_data.at(k), 'g', 6).leftJustified(20, ' ');
					ts << line_str << endl;
				}
			}
			tep_file.close();
		}
	}
	emit sig_changeExportText(tr("file exported succeed!"));
}
