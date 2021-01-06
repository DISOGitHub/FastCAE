#include "CurveScriptHandler.h"
#include "curve_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QDebug>

CurveScriptHandler::CurveScriptHandler(curve_MainWindow* mainwindow, QString fileName) :
	curveScript_mainWindow(mainwindow), curveScript_filename(fileName)
{
	QTextCodec *codec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForLocale(codec);
}

CurveScriptHandler::~CurveScriptHandler()
{
	curveScript_filename.clear();
	curveScript_mainWindow = nullptr;
}

void CurveScriptHandler::Handle()
{
	QFile file(curveScript_filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	QTextStream stream(&file);
	QString line;
	while (!stream.atEnd())
	{
		line = stream.readLine().simplified();
		if (line == "")
			continue;
		splitCode(line);
	}
	
}

void CurveScriptHandler::splitCode(QString code)
{
	if (code.startsWith("#")) 
		return;
	qDebug() << code;
	QStringList fa = code.split("(");
	if (fa.size() != 2)
	{
		return;
	}
	QString fun = fa.at(0);
	QString args = fa.at(1);
	args = args.remove(")");
	
	execCode(fun, args);
}

void CurveScriptHandler::execCode(QString name, QString args)
{
	if (name.toLower() == "openfile")
	{
		flag_openFile = false;
		flag_openFile=openfile(args);
	}
	else if (name.toLower() == "addcurve")
	{
		addCurve(args);
	}
	else if (name.toLower() == "delcurve")
	{
		delCurve(args);
	}
	else if (name.toLower() == "plot_title")
	{
		Plot_title(args);
	}
	else if (name.toLower() == "plot_titlefontcolor")
	{
		Plot_titleFontColor(args);
	}
	else if (name.toLower() == "plot_titlefontsize")
	{
		Plot_titleFontSize(args);
	}
	else if (name.toLower() == "plot_titlefonttype")
	{
		Plot_titleFontType(args);
	}
	else if (name.toLower() == "plot_backgroundcolor")
	{
		Plot_backgroundColor(args);
	}
	else if (name.toLower() == "plot_grid")
	{
		Plot_grid(args);
	}
	else if (name.toLower() == "plot_legend")
	{
		Plot_legend(args);
	}
	else if (name.toLower() == "plot_numaxis")
	{
		Plot_numAxis(args);
	}
	else if (name.toLower() == "plot_axisname")
	{
		Plot_axisName(args);
	}
	else if (name.toLower() == "plot_axisfontcolor")
	{
		Plot_axisFontColor(args);
	}
	else if (name.toLower() == "plot_axisfontsize")
	{
		Plot_axisFontSize(args);
	}
	else if (name.toLower() == "plot_axisfonttype")
	{
		Plot_axisFontType(args);
	}
	else if (name.toLower() == "plot_axisdatasource")
	{
		Plot_axisDataSource(args);
	}
	else if (name.toLower() == "plot_axisfilename")
	{
		Plot_axisFileName(args);
	}
	else if (name.toLower() == "plot_axisfilecolumn")
	{
		Plot_axisFileColumn(args);
	}
	else if (name.toLower() == "plot_axisrange")
	{
		Plot_axisrange(args);
	}
	else if (name.toLower() == "curve_color")
	{
		Curve_color(args);
	}
	else if (name.toLower() == "curve_linetype")
	{
		Curve_lineType(args);
	}
	else if (name.toLower() == "curve_linewidth")
	{
		Curve_lineWidth(args);
	}
	else if (name.toLower() == "curve_showpoint")
	{
		Curve_showPoint(args);
	}
	else if (name.toLower() == "curve_name")
	{
		Curve_name(args);
	}
	else if (name.toLower() == "curve_axisindex")
	{
		Curve_axisIndex(args);
	}
	else if (name.toLower() == "anipoint_color")
	{
		AniPoint_color(args);
	}
	else if (name.toLower() == "anipoint_type")
	{
		AniPoint_type(args);
	}
	else if (name.toLower() == "replot")
	{
		update_replot();
	}
}

void CurveScriptHandler::update_replot()
{
	curveScript_mainWindow->script_update_replot();
}

bool CurveScriptHandler::openfile(QString args)
{
	if (curveScript_mainWindow->script_openFile(args))
	{
		return true;
	}
	else
		return false;
}

void CurveScriptHandler::addCurve(QString args)
{
	bool ok;
	QString tep_filename;
	int tep_column_index;
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 2)
	{
		tep_filename = tep_list.at(0).trimmed();
		tep_column_index = tep_list.at(1).toInt(&ok);
		curveScript_mainWindow->script_addCurve(tep_filename, tep_column_index);
	}
}

void CurveScriptHandler::delCurve(QString args)
{
	bool ok;
	QString tep_filename;
	int tep_column_index;
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 2)
	{
		tep_filename = tep_list.at(0).trimmed();
		tep_column_index = tep_list.at(1).toInt(&ok);
		curveScript_mainWindow->script_delCurve(tep_filename, tep_column_index);
	}
}

void CurveScriptHandler::Plot_title(QString args)
{
	curveScript_mainWindow->script_Plot_title(args);
}

void CurveScriptHandler::Plot_titleFontColor(QString args)
{
	QStringList list = args.split(",",QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		bool ok;
		int r = list.at(0).toInt(&ok);
		int g = list.at(1).toInt(&ok);
		int b = list.at(2).toInt(&ok);
		QColor color(r,g,b);
		curveScript_mainWindow->script_Plot_titleFontColor(color);
	}
}

void CurveScriptHandler::Plot_titleFontSize(QString args)
{
	bool ok;
	int val = args.toInt(&ok);
	curveScript_mainWindow->script_Plot_titleFontSize(val);
}

void CurveScriptHandler::Plot_titleFontType(QString args)
{
	bool ok;
	int val = args.toInt(&ok);
	curveScript_mainWindow->script_Plot_titleFontType((plotProp_fontType)val);
}

void CurveScriptHandler::Plot_backgroundColor(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		bool ok;
		int r = list.at(0).toInt(&ok);
		int g = list.at(1).toInt(&ok);
		int b = list.at(2).toInt(&ok);
		QColor color(r, g, b);
		curveScript_mainWindow->script_Plot_backgroundColor(color);
	}
}

void CurveScriptHandler::Plot_grid(QString args)
{
	bool ok;
	int val = args.toInt(&ok);
	if (val==0)
		curveScript_mainWindow->script_Plot_grid(false);
	else
		curveScript_mainWindow->script_Plot_grid(true);
}

void CurveScriptHandler::Plot_legend(QString args)
{
	bool ok;
	int val = args.toInt(&ok);
	curveScript_mainWindow->script_Plot_legend((plotProp_legendPostion)val);
}

void CurveScriptHandler::Plot_numAxis(QString args)
{
	bool ok;
	int val = args.toInt(&ok);
	curveScript_mainWindow->script_Plot_numAxis(val);
}

void CurveScriptHandler::Plot_axisName(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 2)
	{
		QString axis = list.at(0).trimmed();
		QString val = list.at(1).trimmed();
		curveScript_mainWindow->script_Plot_axisName(axis, val);
	}
}

void CurveScriptHandler::Plot_axisFontColor(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 4)
	{
		bool ok;
		QString axis = list.at(0).trimmed();
		int r = list.at(1).toInt(&ok);
		int g = list.at(2).toInt(&ok);
		int b = list.at(3).toInt(&ok);
		QColor color(r, g, b);
		curveScript_mainWindow->script_Plot_axisFontColor(axis,color);
	}
}

void CurveScriptHandler::Plot_axisFontSize(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 2)
	{
		bool ok;
		QString axis = list.at(0).trimmed();
		int val = list.at(1).toInt(&ok);
		curveScript_mainWindow->script_Plot_axisFontSize(axis, val);
	}
}

void CurveScriptHandler::Plot_axisFontType(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 2)
	{
		bool ok;
		QString axis = list.at(0).trimmed();
		int val = list.at(1).toInt(&ok);
		curveScript_mainWindow->script_Plot_axisFontType(axis, (plotProp_fontType)val);
	}
}

void CurveScriptHandler::Plot_axisDataSource(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 2)
	{
		bool ok;
		QString axis = list.at(0).trimmed();
		int val = list.at(1).toInt(&ok);
		curveScript_mainWindow->script_Plot_axisDataSource(axis, (plotProp_dataSource)val);
	}
}

void CurveScriptHandler::Plot_axisFileName(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 2)
	{
		QString axis = list.at(0).trimmed();
		QString tep_filename= list.at(1).trimmed();
		curveScript_mainWindow->script_Plot_axisFileName(axis,tep_filename);
	}
}

void CurveScriptHandler::Plot_axisFileColumn(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 2)
	{
		bool ok;
		QString axis = list.at(0).trimmed();
		int val = list.at(1).toInt(&ok);
		curveScript_mainWindow->script_Plot_axisFileColumn(axis,val);
	}
}

void CurveScriptHandler::Plot_axisrange(QString args)
{
	bool ok;
	double val[2];
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		QString axis = list.at(0).trimmed();
		val[0] = list.at(1).toDouble(&ok);
		val[1] = list.at(2).toDouble(&ok);
		curveScript_mainWindow->script_Plot_axisrange(axis, val);
	}
}

void CurveScriptHandler::Curve_color(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 5)
	{
		bool ok;
		QString tep_filename = list.at(0).trimmed();
		int tep_column_index = list.at(1).toInt(&ok);
		int r = list.at(2).toInt(&ok);
		int g = list.at(3).toInt(&ok);
		int b = list.at(4).toInt(&ok);
		QColor color(r, g, b);
		curveScript_mainWindow->script_Curve_color(tep_filename,tep_column_index, color);
	}
}

void CurveScriptHandler::Curve_lineType(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		bool ok;
		QString tep_filename = list.at(0).trimmed();
		int tep_column_index = list.at(1).toInt(&ok);
		int val = list.at(2).toInt(&ok);
		curveScript_mainWindow->script_Curve_lineType(tep_filename, tep_column_index, (Qt::PenStyle) val);
	}
}

void CurveScriptHandler::Curve_lineWidth(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		bool ok;
		QString tep_filename = list.at(0).trimmed();
		int tep_column_index = list.at(1).toInt(&ok);
		int val = list.at(2).toInt(&ok);
		curveScript_mainWindow->script_Curve_lineWidth(tep_filename, tep_column_index, val);
	}
}

void CurveScriptHandler::Curve_showPoint(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		bool ok;
		QString tep_filename = list.at(0).trimmed();
		int tep_column_index = list.at(1).toInt(&ok);
		int val = list.at(2).toInt(&ok);
		bool flag;
		if (val == 0)
			flag = false;
		else
			flag = true;
		curveScript_mainWindow->script_Curve_showPoint(tep_filename, tep_column_index, flag);
	}
}

void CurveScriptHandler::Curve_name(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		bool ok;
		QString tep_filename = list.at(0).trimmed();
		int tep_column_index = list.at(1).toInt(&ok);
		QString name = list.at(2).trimmed();
		curveScript_mainWindow->script_Curve_name(tep_filename, tep_column_index, name);
	}
}

void CurveScriptHandler::Curve_axisIndex(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		bool ok;
		QString tep_filename = list.at(0).trimmed();
		int tep_column_index = list.at(1).toInt(&ok);
		int val = list.at(2).toInt(&ok);
		curveScript_mainWindow->script_Curve_axisIndex(tep_filename, tep_column_index, val);
	}
}

void CurveScriptHandler::AniPoint_color(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 5)
	{
		bool ok;
		QString tep_filename = list.at(0).trimmed();
		int tep_column_index = list.at(1).toInt(&ok);
		int r = list.at(2).toInt(&ok);
		int g = list.at(3).toInt(&ok);
		int b = list.at(4).toInt(&ok);
		QColor color(r, g, b);
		curveScript_mainWindow->script_AniPoint_color(tep_filename, tep_column_index, color);
	}
}
void CurveScriptHandler::AniPoint_type(QString args)
{
	QStringList list = args.split(",", QString::SkipEmptyParts);
	if (list.count() == 3)
	{
		bool ok;
		QString tep_filename = list.at(0).trimmed();
		int tep_column_index = list.at(1).toInt(&ok);
		int type = list.at(2).toInt(&ok);
		curveScript_mainWindow->script_AniPoint_type(tep_filename, tep_column_index, (aniPointType)type);
	}
}
