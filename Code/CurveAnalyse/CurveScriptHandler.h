#ifndef _SCRIPTHANDLER_H_
#define _SCRIPTHANDLER_H_
#include <QString>
class curve_MainWindow;

class CurveScriptHandler
{
public:
	CurveScriptHandler(curve_MainWindow* mainwindow, QString fileName);
	~CurveScriptHandler();
	void Handle();
	
private:
	void splitCode(QString code);
	void execCode(QString name, QString args);

	bool openfile(QString args);
	void addCurve(QString args);
	void delCurve(QString args);
	void Plot_title(QString args);
	void Plot_titleFontColor(QString args);
	void Plot_titleFontSize(QString args);
	void Plot_titleFontType(QString args);
	void Plot_backgroundColor(QString args);
	void Plot_grid(QString args);
	void Plot_legend(QString args);
	void Plot_numAxis(QString args);
	void Plot_axisName(QString args);
	void Plot_axisFontColor(QString args);
	void Plot_axisFontSize(QString args);
	void Plot_axisFontType(QString args);
	void Plot_axisDataSource(QString args);
	void Plot_axisFileName(QString args);
	void Plot_axisFileColumn(QString args);
	void Plot_axisrange(QString args);
	void Curve_color(QString args);
	void Curve_lineType(QString args);
	void Curve_lineWidth(QString args);
	void Curve_showPoint(QString args);
	void Curve_name(QString args);
	void Curve_axisIndex(QString args);
	void update_replot();

	void AniPoint_color(QString args);
	void AniPoint_type(QString args);

	QString curveScript_filename;
	curve_MainWindow* curveScript_mainWindow;
	bool flag_openFile;
	int cur_file_index;
};

#endif
